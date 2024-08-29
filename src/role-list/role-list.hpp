#ifndef ROLE_LIST_HPP
#define ROLE_LIST_HPP

#include <functional>
#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include "error/error.hpp"
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include "alignment/alignment.hpp"
#include "faction/faction.hpp"
#include "group/group.hpp"

class RoleList
{
  std::vector<std::string> query;
  std::vector<ListEntry *> data;
  std::vector<int> counts;

  unsigned int line = 0;

  Role *generate_role_from_role(int i);
  Role *generate_role_from_alignment(int i);
  Role *generate_role_from_faction(int i);
  Role *generate_role_from_group(int i);
public:
  RoleList(std::vector<std::string> input, std::vector<ListEntry *> data);

  std::string generate(bool verbose=false, bool color=true);

  static std::string process_role_entry(std::string input);
};

#endif
