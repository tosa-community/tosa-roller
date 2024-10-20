#include "faction.hpp"
#include <algorithm>

Faction::Faction(int pos, std::string name, int limit,
  std::vector<std::string> aliases, std::vector<ListEntry *> alignments)
  : ListEntry(pos, name, limit, aliases)
{
  for (auto alignment : alignments) {
    Alignment *casted = static_cast<Alignment *>(alignment);
    casted->faction_pos = pos;
    for (auto role : casted->roles) {
      role->faction_pos = pos;
    }
    this->alignments.push_back(casted);
  }
}

Faction::Faction(int pos, std::string name, int limit,
  std::vector<std::string> aliases, std::vector<std::string> require,
  int require_min, std::string fallback, std::vector<ListEntry *> alignments)
  : ListEntry(pos, name, limit, aliases)
{
  this->require_min = require_min;

  for (auto alignment : alignments) {
    Alignment *casted = static_cast<Alignment *>(alignment);
    casted->faction_pos = pos;

    if (casted->name == fallback)
      this->fallback = casted;

    bool is_required = false;
    if (std::find(require.begin(), require.end(), casted->name)
      != require.end())
      is_required = true;

    for (auto role : casted->roles) {
      role->faction_pos = pos;

      if (role->name == fallback)
        this->fallback = role;

      if (is_required
        || std::find(require.begin(), require.end(), role->name)
          != require.end())
        this->require.push_back(role);
    }
    this->alignments.push_back(casted);
  }
}

Faction::~Faction() { }

ListEntry::Type Faction::type() { return ListEntry::Type::FACTION; }
