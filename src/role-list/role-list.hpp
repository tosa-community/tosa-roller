#ifndef ROLE_LIST_HPP
#define ROLE_LIST_HPP

#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include "error/error.hpp"
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include "alignment/alignment.hpp"
#include "faction/faction.hpp"

class RoleList
{
  std::vector<std::string> query;
  std::vector<ListEntry *> data;
  std::vector<int> counts;

  unsigned int line = 0;

  Role *generate_role_from_alignment(int i);
public:
  RoleList(std::vector<std::string> input, std::vector<ListEntry *> data);

  std::string generate();
};

#endif
