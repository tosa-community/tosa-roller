#ifndef ALIGNMENT_HPP
#define ALIGNMENT_HPP

#include <vector>
#include <string>
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"

class Alignment : public ListEntry
{
public:
  std::vector<Role *> roles;

  Alignment(std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> roles);
  ~Alignment();

  ListEntry::Type type();
};

#endif
