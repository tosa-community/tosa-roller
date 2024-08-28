#ifndef ROLE_LIST_HPP
#define ROLE_LIST_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include "error/error.hpp"
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include "alignment/alignment.hpp"
#include "faction/faction.hpp"

class RoleList
{
  std::vector<std::string> query;
  std::vector<ListEntry *> data;
public:
  RoleList(std::vector<std::string> input, std::vector<ListEntry *> data);

  std::string generate();
};

#endif
