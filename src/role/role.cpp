#include "role.hpp"

Role::Role(std::string name, int limit, std::vector<std::string> aliases, std::vector<int> color) : ListEntry(name, limit, aliases)
{
  this->r = color[0];
  this->g = color[1];
  this->b = color[2];
}

Role::~Role()
{}

std::string Role::get_colored_str()
{
  std::stringstream stream;
  stream << "\033[1;38;2;" << this->r << ";" << this->g << ";" << this->b << "m" << name << "\033[0;m";

  return stream.str();
}
