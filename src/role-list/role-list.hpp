#ifndef ROLE_LIST_HPP
#define ROLE_LIST_HPP

#include <functional>
#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>
#include <string>
#include <map>
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

  std::vector<Faction *> rq_factions;

  std::vector<std::pair<ListEntry *, Role *>> output;
  std::vector<std::vector<int>> targets;

  std::string filename;
  unsigned int line = 0;

  Role *generate_role_from_role(int i);
  Role *generate_role_from_alignment(int i);
  Role *generate_role_from_faction(int i);
  Role *generate_role_from_group(int i);

  void _shuffle(std::vector<std::string> scrolls);
public:
  RoleList(std::string filename, std::vector<std::string> input, std::vector<ListEntry *> data, std::vector<Faction *> rq_factions);
  ~RoleList();

  void generate();

  void shuffle();
  void shuffle(std::vector<std::string> scrolls);

  void generate_targets();

  std::string to_string(bool verbose=false, bool color=true);

  static std::string process_role_entry(std::string input);
};

#endif
