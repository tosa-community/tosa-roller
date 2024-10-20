#ifndef LIST_ENTRY_HPP
#define LIST_ENTRY_HPP

#include <string>
#include <vector>

class ListEntry
{
public:
  enum class Type
  {
    ROLE,
    ALIGNMENT,
    FACTION,
    GROUP,
  };
  std::string name;
  int limit;
  std::vector<std::string> aliases;

  const int pos;

  ListEntry(
    int pos, std::string name, int limit, std::vector<std::string> aliases);
  virtual ~ListEntry() = 0;

  virtual Type type() = 0;
};

#endif
