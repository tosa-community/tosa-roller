#include "alignment.hpp"

Alignment::Alignment(std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> roles) : ListEntry(name, limit, aliases)
{
  this->roles = roles;
}

Alignment::~Alignment()
{}
