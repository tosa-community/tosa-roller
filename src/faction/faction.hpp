#ifndef FACTION_HPP
#define FACTION_HPP

#include <vector>
#include <string>
#include "list-entry/list-entry.hpp"
#include "role/role.hpp"
#include "alignment/alignment.hpp"

class Faction : public ListEntry
{
public:
  std::vector<Alignment *> alignments;

  std::vector<Role *> require;
  int require_min = -1;
  ListEntry *fallback;

  Faction(int pos, std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> alignments);
  Faction(int pos, std::string name, int limit, std::vector<std::string> aliases, std::vector<std::string> require, int require_min, std::string fallback, std::vector<ListEntry *> alignments);
  ~Faction();

  ListEntry::Type type();
};

#endif
