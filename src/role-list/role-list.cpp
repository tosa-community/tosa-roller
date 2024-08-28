#include "role-list.hpp"

RoleList::RoleList(std::vector<std::string> input, std::vector<ListEntry *> data)
{
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
  std::vector<int> counts;
  counts.resize(this->data.size(), 0);

  std::string res;

  for (auto input_entry : this->query)
  {
    for (int i = 0; i < this->data.size(); i++)
    {
      std::vector<std::string> aliases = data[i]->aliases;
      if (std::find(aliases.begin(), aliases.end(), input_entry) != aliases.end())
      {
        if (counts[i] == data[i]->limit) throw Error("Invalid role: %s", input_entry.c_str());

        switch (data[i]->type())
        {
        case ListEntry::Type::ROLE:
          res.append(static_cast<Role *>(data[i])->get_colored_str());
          res.append("\n");
          break;
        case ListEntry::Type::ALIGNMENT:
        case ListEntry::Type::FACTION:
          break;
        }

        counts[i]++;
        break;
      }
    }
  }

  return res;
}
