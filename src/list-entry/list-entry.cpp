#include "list-entry.hpp"

ListEntry::ListEntry(std::string name, int limit, std::vector<std::string> aliases)
{
  this->name = name;
  this->limit = limit;
  this->aliases = aliases;
}

ListEntry::~ListEntry()
{}
