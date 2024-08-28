#ifndef LIST_ENTRY_HPP
#define LIST_ENTRY_HPP

#include <vector>
#include <string>

class ListEntry
{
public:
  enum class Type
  {
    ROLE,
    ALIGNMENT,
    FACTION,
  };
  std::string name;
  int limit;
  std::vector<std::string> aliases;

  ListEntry(std::string name, int limit, std::vector<std::string> aliases);
  virtual ~ListEntry() = 0;

  virtual Type type() = 0;
};

#endif
