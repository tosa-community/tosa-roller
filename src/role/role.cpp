#include "role.hpp"

Role::Role(int pos, std::string name, int limit, std::vector<std::string> aliases, std::vector<int> color) : ListEntry(pos, name, limit, aliases)
{
  this->r = color[0];
  this->g = color[1];
  this->b = color[2];
}

Role::Role(int pos, std::string name, int limit, std::vector<std::string> aliases, std::vector<int> color, std::vector<TargetData> target_data) : ListEntry(pos, name, limit, aliases)
{
  this->r = color[0];
  this->g = color[1];
  this->b = color[2];
  this->target_data = target_data;
}

Role::~Role()
{}

std::string Role::get_colored_str(bool color)
{
  if (!color) return name;

  std::stringstream stream;
  stream << "\033[1;38;2;" << this->r << ";" << this->g << ";" << this->b << "m" << name << "\033[0;m";

  return stream.str();
}

ListEntry::Type Role::type() { return ListEntry::Type::ROLE; }
