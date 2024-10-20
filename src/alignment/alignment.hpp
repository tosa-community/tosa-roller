#ifndef ALIGNMENT_HPP
#define ALIGNMENT_HPP

#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include <string>
#include <vector>

class Alignment : public ListEntry
{
public:
  int faction_pos;

  std::vector<Role *> roles;

  Alignment(int pos, std::string name, int limit,
    std::vector<std::string> aliases, std::vector<ListEntry *> roles);
  ~Alignment();

  ListEntry::Type type();
};

#endif
