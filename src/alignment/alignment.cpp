#include "alignment.hpp"

Alignment::Alignment(int pos, std::string name, int limit,
  std::vector<std::string> aliases, std::vector<ListEntry *> roles)
  : ListEntry(pos, name, limit, aliases)
{
  for (auto role : roles) {
    Role *casted = static_cast<Role *>(role);
    casted->alignment_pos = pos;
    this->roles.push_back(casted);
  }
}

Alignment::~Alignment() { }

ListEntry::Type Alignment::type() { return ListEntry::Type::ALIGNMENT; }
