#include "faction.hpp"

Faction::Faction(std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> alignments) : ListEntry(name, limit, aliases)
{
  this->alignments = alignments;
}

Faction::~Faction()
{}
