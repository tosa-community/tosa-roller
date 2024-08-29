#include "alignment.hpp"

Alignment::Alignment(std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> roles) : ListEntry(name, limit, aliases)
{
  for (auto role : roles)
  {
    this->roles.push_back(static_cast<Role *>(role));
  }
}

Alignment::~Alignment()
{}

ListEntry::Type Alignment::type() { return ListEntry::Type::ALIGNMENT; }
