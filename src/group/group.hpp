#ifndef GROUP_HPP
#define GROUP_HPP

#include <vector>
#include <string>
#include <tuple>
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include "alignment/alignment.hpp"
#include "faction/faction.hpp"

class Group : public ListEntry
{
public:
  struct RoleData
  {
    int faction_index;
    int alignment_index;
    int role_index;
    Role *role;
  };
  std::vector<RoleData> roles;

  Group(std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> entries, std::vector<std::string> factions, std::vector<std::string> alignments, std::vector<std::string> roles);
  ~Group();

  ListEntry::Type type();
};

#endif
