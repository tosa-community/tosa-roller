#ifndef FACTION_HPP
#define FACTION_HPP

#include <vector>
#include <string>
#include "list-entry/list-entry.hpp"
#include "alignment/alignment.hpp"

class Faction : public ListEntry
{
public:
  std::vector<Alignment *> alignments;

  Faction(int pos, std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> alignments);
  ~Faction();

  ListEntry::Type type();
};

#endif
