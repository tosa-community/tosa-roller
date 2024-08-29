#include "role-list.hpp"

RoleList::RoleList(std::vector<std::string> input, std::vector<ListEntry *> data)
{
  std::srand(std::time(nullptr));

  this->data = data;
  for (auto entry : input)
  {
    std::string processed_string;

    for (auto c : entry)
    {
      processed_string.push_back(std::tolower(c));
    }

    this->query.push_back(processed_string);
  }
}

std::string RoleList::generate()
{
  counts.resize(this->data.size(), 0);

  std::string res;

  for (auto input_entry : this->query)
  {
    line++;

    bool role_is_valid = false;
    for (int i = 0; i < this->data.size(); i++)
    {
      std::vector<std::string> aliases = data[i]->aliases;
      if (std::find(aliases.begin(), aliases.end(), input_entry) != aliases.end())
      {
        if (counts[i] == data[i]->limit) throw Error("Too many of role: %s (at line %d)", data[i]->name, line);

        switch (data[i]->type())
        {
        case ListEntry::Type::ROLE:
          res.append(static_cast<Role *>(data[i])->get_colored_str());
          break;
        case ListEntry::Type::ALIGNMENT:
        {
          Role *role = generate_role_from_alignment(i);

          res.append(role->get_colored_str());
          break;
        }
        case ListEntry::Type::FACTION:
          break;
        }

        res.append("\n");

        counts[i]++;
        role_is_valid = true;
        break;
      }
    }

    if (!role_is_valid) throw Error("Invalid role name: %s (at line %d)", input_entry.c_str(), line);
  }

  return res;
}

Role *RoleList::generate_role_from_alignment(int i)
{
  Alignment *alignment = static_cast<Alignment *>(data[i]);
  int len = alignment->roles.size();

  if (counts[i] == data[i]->limit) throw Error("Too many of alignment: %s (at line %d)", alignment->name.c_str(), line);

  while (true)
  {
    int index = i - len + std::rand() % len;

    if (counts[index] != data[index]->limit)
    {
      return static_cast<Role *>(data[index]);
    }
  }
}
