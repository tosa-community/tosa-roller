#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "error/error.hpp"
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include "alignment/alignment.hpp"
#include "faction/faction.hpp"
#include "role-list/role-list.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char **argv)
{
  #ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  std::setvbuf(stdout, nullptr, _IOFBF, 1000);
  #endif
  try
  {
    CLI::App app("A role-list generator for Town of Salem: Anticipation");

    std::string bans_str;
    app.add_option("-b,--bans", bans_str, "Comma-separated list of roles that should be prevented from rolling");

    std::string data_source;
    app.add_option("-d,--data", data_source, "Path to the roles data JSON")->type_name("FILENAME");

    std::string file;
    app.add_option("-f,--from-file", file, "Path to the file to generate the rolelist from")->type_name("FILENAME");

    bool no_color = false;
    app.add_flag("--no-color", no_color, "Display output without colour");

    std::string output_file;
    app.add_option("-o,--to-file", output_file, "Path of the file to write the output to")->type_name("FILENAME");

    bool no_scroll = false;
    app.add_flag("-r,--skip-scrolls", no_scroll, "Skip scrolls prompt");

    bool skip_targets = false;
    app.add_flag("-s,--skip-targets", skip_targets, "Skip target generation");

    bool verbose = false;
    app.add_flag("-V,--verbose", verbose, "Show extra information in the output");

    bool version = false;
    app.add_flag("-v,--version", version, "Print version information on exit");

    CLI11_PARSE(app, argc, argv);

    if (version)
    {
      std::cout << PACKAGE << " " << VERSION << std::endl;
      return EXIT_SUCCESS;
    }

    if (data_source.size() == 0) throw Error("No data file specified");

    std::vector<std::string> bans;
    if (bans_str.size() != 0)
    {
      std::stringstream bans_stream(bans_str);
      std::string entry;

      while (std::getline(bans_stream, entry, ',')) bans.push_back(RoleList::process_role_entry(entry));
    }

    std::vector<std::string> list_query;

    if (file.size() == 0)
    {
      file = "stdin";

      std::cout << "Enter role list (Leave empty to continue):" << std::endl;

      while (true)
      {
        std::string line;
        std::getline(std::cin, line);

        if (line.length() == 0) break;

        list_query.push_back(line);
      }
    }
    else
    {
      std::ifstream stream(file);
      while (true)
      {
        std::string line;
        std::getline(stream, line);

        if (line.length() == 0) break;

        list_query.push_back(line);
      }
    }

    std::vector<std::string> scrolls;

    if (!no_scroll)
    {
      std::cout << "Enter scrolls (Prefix with '-' to enter cursed scroll):" << std::endl;

      for (int i = 1; i < list_query.size() + 1; i++)
      {
        std::string buf;
        std::cout << "[" << i << "] - ";
        std::getline(std::cin, buf);

        scrolls.push_back(buf);
      }
    }

    std::vector<ListEntry *> entries;
    std::vector<Faction *> rq_factions;
    int pos = 0;

    try
    {
      std::ifstream data_stream(data_source);
      nlohmann::json data = nlohmann::json::parse(data_stream);

      for (auto faction : data["factions"])
      {
        std::vector<ListEntry *> faction_roles;

        bool is_banned = false;

        for (int i = 0; i < faction["aliases"].size(); i++)
        {
          for (int j = 0; j < bans.size(); j++)
          {
            if (faction["aliases"][i] == bans[j])
            {
              is_banned = true;
              break;
            }
          }
        }

        if (is_banned) continue;

        for (auto alignment : faction["alignments"])
        {
          std::vector<ListEntry *> alignment_roles;

          bool is_banned = false;

          for (int i = 0; i < alignment["aliases"].size(); i++)
          {
            for (int j = 0; j < bans.size(); j++)
            {
              if (alignment["aliases"][i] == bans[j])
              {
                is_banned = true;
                break;
              }
            }
          }

          if (is_banned) continue;

          for (auto role : alignment["roles"])
          {
            bool is_banned = false;
            std::string name;
            name = role["name"];

            for (int i = 0; i < role["aliases"].size(); i++)
            {
              for (int j = 0; j < bans.size(); j++)
              {
                if (role["aliases"][i] == bans[j])
                {
                  is_banned = true;
                  break;
                }
              }
            }

            if (is_banned) continue;

            Role *entry;
            if (role.contains("targets"))
            {
              std::vector<Role::TargetData> target_data;
              for (auto target : role["targets"])
              {
                Role::TargetData target_info;
                target_info.name = target["name"];
                for (auto entry : target["exclude"])
                {
                  target_info.exclude.push_back(entry);
                }

                target_data.push_back(target_info);
              }
              if (role.contains("oppose"))
                entry = new Role(pos, name, role["limit"], role["aliases"], role["wincon"], role["oppose"], role["color"], target_data);
              else
                entry = new Role(pos, name, role["limit"], role["aliases"], role["wincon"], -1, role["color"], target_data);
            }
            else if (role.contains("oppose"))
              entry = new Role(pos, name, role["limit"], role["aliases"], role["wincon"], role["oppose"], role["color"]);
            else
              entry = new Role(pos, name, role["limit"], role["aliases"], role["wincon"], -1, role["color"]);
            entries.push_back(entry);
            alignment_roles.push_back(entry);

            pos++;
          }

          Alignment *alignment_entry = new Alignment(pos, alignment["name"], alignment["limit"], alignment["aliases"], alignment_roles);
          if (alignment_entry->roles.size() == 0) continue;

          entries.push_back(alignment_entry);
          faction_roles.push_back(alignment_entry);

          pos++;
        }
        Faction *faction_entry;

        if (faction.contains("require") && faction.contains("require_min") && faction.contains("fallback"))
        {
          faction_entry = new Faction(pos, faction["name"], faction["limit"], faction["aliases"], faction["require"], faction["require_min"], faction["fallback"], faction_roles);
          if (faction_entry->alignments.size() != 0) rq_factions.push_back(faction_entry);
        }
        else
          faction_entry = new Faction(pos, faction["name"], faction["limit"], faction["aliases"], faction_roles);
        if (faction_entry->alignments.size() == 0) continue;

        entries.push_back(faction_entry);

        pos++;
      }

      for (auto group : data["groups"])
      {
        Group *group_entry = new Group(pos, group["name"], -1, group["aliases"], entries, group["factions"], group["alignments"], group["roles"]);
        entries.push_back(group_entry);

        pos++;
      }
    }
    catch (std::exception e)
    {
      throw Error("Invalid data file");
    }

    RoleList list(file, list_query, entries, rq_factions);
    list.generate();

    if (no_scroll) list.shuffle();
    else list.shuffle(scrolls);

    if (!skip_targets) list.generate_targets();

    if (output_file.size() == 0)
    {
      std::cout << list.to_string(verbose, !no_color);
    }
    else
    {
      std::ofstream ostream(output_file, std::ios::out);
      if (ostream.is_open())
      {
        ostream << list.to_string(verbose, false);
        ostream.close();
      }
    }
  }
  catch (Error& e)
  {
    std::cout << "\033[31m" << argv[0] << ":";
    if (e.line) std::cout << e.filename << ":" << e.line << ":";
    std::cout << " " << e.what() << "\033[0m" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
