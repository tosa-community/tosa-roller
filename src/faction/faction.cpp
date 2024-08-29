#include "faction.hpp"

Faction::Faction(std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> alignments) : ListEntry(name, limit, aliases)
{
  for (auto alignment : alignments)
  {
    this->alignments.push_back(static_cast<Alignment *>(alignment));
  }
}

Faction::~Faction()
{}

ListEntry::Type Faction::type() { return ListEntry::Type::FACTION; }
