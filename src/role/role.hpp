#ifndef ROLE_HPP
#define ROLE_HPP

#include <sstream>
#include <vector>
#include <string>
#include "list-entry/list-entry.hpp"

class Role : public ListEntry
{
  unsigned int r;
  unsigned int g;
  unsigned int b;
public:
  Role(std::string name, int limit, std::vector<std::string> aliases, std::vector<int> color);
  ~Role();

  std::string get_colored_str();
};

#endif
