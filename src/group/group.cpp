#include "group.hpp"
#include "alignment/alignment.hpp"
#include "faction/faction.hpp"

Group::Group(int pos, std::string name, int limit,
  std::vector<std::string> aliases, std::vector<ListEntry *> entries,
  std::vector<std::string> factions, std::vector<std::string> alignments,
  std::vector<std::string> roles)
  : ListEntry(pos, name, limit, aliases)
{
  for (unsigned int i = 0; i < entries.size(); i++) {
    for (auto faction : factions) {
      if (entries[i]->name == faction) {
        for (auto alignment : static_cast<Faction *>(entries[i])->alignments) {
          for (auto role : alignment->roles) {
            this->roles.push_back(role);
          }
        }
      }
    }

    for (auto alignment : alignments) {
      if (entries[i]->name == alignment) {
        for (auto role : static_cast<Alignment *>(entries[i])->roles) {
          this->roles.push_back(role);
        }
      }
    }

    for (auto role : roles) {
      if (entries[i]->name == role) {
        this->roles.push_back(static_cast<Role *>(entries[i]));
      }
    }
  }
}

Group::~Group() { }

ListEntry::Type Group::type() { return ListEntry::Type::GROUP; }
