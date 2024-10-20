#include "list-entry.hpp"

ListEntry::ListEntry(
  int pos, std::string name, int limit, std::vector<std::string> aliases)
  : pos(pos)
{
  this->name = name;
  this->limit = limit;
  this->aliases = aliases;
}

ListEntry::~ListEntry() { }
