#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "error/error.hpp"
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include "alignment/alignment.hpp"
#include "faction/faction.hpp"
#include "role-list/role-list.hpp"

int main(int argc, char **argv)
{
  try
  {
    CLI::App app("A role-list generator for Town of Salem: Anticipation");

    std::string bans_str;
    app.add_option("-b,--bans", bans_str, "Comma-separated roles to prevent from being rolled");

    std::string data_source;
    app.add_option("-d,--data", data_source, "Path to data source")->type_name("FILENAME")->required();

    std::string file;
    app.add_option("-f,--from-file", file, "Generate list from file")->type_name("FILENAME");

    bool verbose = false;
    app.add_flag("-V,--verbose", verbose, "Show extra information");

    CLI11_PARSE(app, argc, argv);

    std::vector<std::string> bans;
    if (bans_str.size() != 0)
    {
      std::stringstream bans_stream(bans_str);
      std::string entry;

      while (std::getline(bans_stream, entry, ',')) bans.push_back(RoleList::process_role_entry(entry));
    }

    std::ifstream data_stream(data_source);
    nlohmann::json data = nlohmann::json::parse(data_stream);

    std::vector<std::string> list_query;

    if (file.size() == 0)
    {

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

    std::vector<ListEntry *> entries;

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

          Role *entry = new Role(role["name"], role["limit"], role["aliases"], role["color"]);
          entries.push_back(entry);
          alignment_roles.push_back(entry);
        }

        Alignment *alignment_entry = new Alignment(alignment["name"], alignment["limit"], alignment["aliases"], alignment_roles);
        entries.push_back(alignment_entry);
        faction_roles.push_back(alignment_entry);
      }

      Faction *faction_entry = new Faction(faction["name"], faction["limit"], faction["aliases"], faction_roles);
      entries.push_back(faction_entry);
    }

    for (auto group : data["groups"])
    {
      Group *group_entry = new Group(group["name"], -1, group["aliases"], entries, group["factions"], group["alignments"], group["roles"]);
      entries.push_back(group_entry);
    }

    RoleList list(list_query, entries);
    std::cout << list.generate(verbose) << std::endl;
  }
  catch (Error& e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
