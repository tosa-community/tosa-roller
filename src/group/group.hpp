#ifndef GROUP_HPP
#define GROUP_HPP

#include "error/error.hpp"
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include <string>
#include <vector>

class Group : public ListEntry
{
public:
  std::vector<Role *> roles;

  Group(int pos, std::string name, int limit, std::vector<std::string> aliases,
    std::vector<ListEntry *> entries, std::vector<std::string> factions,
    std::vector<std::string> alignments, std::vector<std::string> roles);
  ~Group();

  ListEntry::Type type();
};

#endif
