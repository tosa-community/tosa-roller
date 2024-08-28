#include <CLI/CLI.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "error/error.hpp"

int main(int argc, char **argv)
{
  try
  {
    CLI::App app("A role-list generator for Town of Salem: Anticipation.");

    std::string data_source;
    app.add_option("-d,--data", data_source, "Path to data source.")->type_name("FILENAME")->required();

    CLI11_PARSE(app, argc, argv);

    std::cout << "Enter role list (Leave empty to continue):" << std::endl;

    std::vector<std::string> rolelist;

    while (true)
    {
      std::string line;
      std::getline(std::cin, line);

      if (line.length() == 0) break;

      rolelist.push_back(line);
    }
  }
  catch (Error& e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
