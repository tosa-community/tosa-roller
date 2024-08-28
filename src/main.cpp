#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "error/error.hpp"
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include "alignment/alignment.hpp"
#include "faction/faction.hpp"

int main(int argc, char **argv)
{
  try
  {
    CLI::App app("A role-list generator for Town of Salem: Anticipation.");

    std::string data_source;
    app.add_option("-d,--data", data_source, "Path to data source.")->type_name("FILENAME")->required();

    CLI11_PARSE(app, argc, argv);

    std::ifstream data_stream(data_source);
    nlohmann::json data = nlohmann::json::parse(data_stream);

    std::cout << "Enter role list (Leave empty to continue):" << std::endl;

    std::vector<std::string> rolelist;

    while (true)
    {
      std::string line;
      std::getline(std::cin, line);

      if (line.length() == 0) break;

      rolelist.push_back(line);
    }

    std::vector<ListEntry *> entries;

    for (auto faction : data["factions"])
    {
      std::vector<ListEntry *> faction_roles;

      for (auto alignment : faction["alignments"])
      {
        std::vector<ListEntry *> alignment_roles;

        for (auto role : alignment["roles"])
        {
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
  }
  catch (Error& e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
