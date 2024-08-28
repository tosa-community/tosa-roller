#ifndef FACTION_HPP
#define FACTION_HPP

#include <vector>
#include <string>
#include "list-entry/list-entry.hpp"

class Faction : public ListEntry
{
public:
  std::vector<ListEntry *> alignments;

  Faction(std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> alignments);
  ~Faction();
};

#endif
