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
  std::srand(std::time(nullptr));

  counts.resize(this->data.size(), 0);

  std::vector<Role *> out;

  for (auto input_entry : this->query)
  {
    line++;

    bool role_is_valid = false;
    for (int i = 0; i < this->data.size(); i++)
    {
      std::vector<std::string> aliases = data[i]->aliases;
      if (std::find(aliases.begin(), aliases.end(), input_entry) != aliases.end())
      {
        if (counts[i] == data[i]->limit)
        {
          switch (data[i]->type())
          {
          case ListEntry::Type::ROLE:
            throw Error("Too many of role: %s (at line %d)", data[i]->name.c_str(), line);
          case ListEntry::Type::ALIGNMENT:
            throw Error("Too many of alignment: %s (at line %d)", data[i]->name.c_str(), line);
          case ListEntry::Type::FACTION:
            throw Error("Too many of faction: %s (at line %d)", data[i]->name.c_str(), line);
          }
        }

        switch (data[i]->type())
        {
        case ListEntry::Type::ROLE:
          out.push_back(static_cast<Role *>(data[i]));
          break;
        case ListEntry::Type::ALIGNMENT:
        {
          Role *role = generate_role_from_alignment(i);
          out.push_back(role);

          break;
        }
        case ListEntry::Type::FACTION:
        {
          Role *role = generate_role_from_faction(i);
          out.push_back(role);

          break;
        }
        }

        counts[i]++;
        role_is_valid = true;
        break;
      }
    }

    if (!role_is_valid) throw Error("Invalid role name: %s (at line %d)", input_entry.c_str(), line);
  }

  std::string res;

  std::shuffle(out.begin(), out.end(), std::default_random_engine(std::time(nullptr)));

  for (auto role : out)
  {
    res.append(role->get_colored_str());
    res.append("\n");
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
      counts[index]++;
      return static_cast<Role *>(data[index]);
    }
  }
}

Role *RoleList::generate_role_from_faction(int i)
{
  Faction *faction = static_cast<Faction *>(data[i]);
  std::vector<std::pair<int, Alignment *>> alignments;

  if (counts[i] == data[i]->limit) throw Error("Too many of faction: %s (at line %d)", faction->name.c_str(), line);

  int temp = 0;
  for (auto entry : faction->alignments)
  {
    Alignment *alignment = static_cast<Alignment *>(entry);
    temp += alignment->roles.size() + 1;

    alignments.push_back(std::pair<int, Alignment *>(temp, alignment));
  }

  int onset = i - temp - 1;
  int len = alignments.size();

  while (true)
  {
    int index = std::rand() % len;

    int pos = onset + alignments[index].first;
    if (counts[pos] != data[pos]->limit)
    {
      Role *role = generate_role_from_alignment(pos);
      counts[pos]++;

      return role;
    }
  }
}
