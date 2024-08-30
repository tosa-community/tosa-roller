#include "faction.hpp"

Faction::Faction(int pos, std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> alignments) : ListEntry(pos, name, limit, aliases)
{
  for (auto alignment : alignments)
  {
    Alignment *casted = static_cast<Alignment *>(alignment);
    casted->faction_pos = pos;
    for (auto role : casted->roles)
    {
      role->faction_pos = pos;
    }
    this->alignments.push_back(casted);
  }
}

Faction::~Faction()
{}

ListEntry::Type Faction::type() { return ListEntry::Type::FACTION; }
