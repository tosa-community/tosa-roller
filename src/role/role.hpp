#ifndef ROLE_HPP
#define ROLE_HPP

#include <sstream>
#include <vector>
#include <string>
#include "list-entry/list-entry.hpp"

class Role : public ListEntry
{
  unsigned int r;
  unsigned int g;
  unsigned int b;
public:
  struct TargetData
  {
    std::string name;
    std::vector<std::string> exclude;
  };
  std::vector<TargetData> target_data;

  int wincon;
  int oppose;

  int alignment_pos;
  int faction_pos;

  Role(int pos, std::string name, int limit, std::vector<std::string> aliases, int wincon, int oppose, std::vector<int> color, std::vector<TargetData> target_data=std::vector<TargetData>());
  ~Role();

  std::string get_colored_str(bool color=true);

  ListEntry::Type type();
};

#endif
