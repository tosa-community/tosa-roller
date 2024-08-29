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

std::string RoleList::generate(bool verbose)
{
  std::srand(std::time(nullptr));

  counts.resize(this->data.size(), 0);

  std::vector<std::pair<ListEntry *, Role *>> out;

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
        {
          Role *role = generate_role_from_role(i);
          out.push_back(std::pair<ListEntry *, Role *>(data[i], static_cast<Role *>(data[i])));

          break;
        }
        case ListEntry::Type::ALIGNMENT:
        {
          Role *role = generate_role_from_alignment(i);
          out.push_back(std::pair<ListEntry *, Role *>(data[i], role));

          break;
        }
        case ListEntry::Type::FACTION:
        {
          Role *role = generate_role_from_faction(i);
          out.push_back(std::pair<ListEntry *, Role *>(data[i], role));

          break;
        }
        case ListEntry::Type::GROUP:
        {
          Role *role = generate_role_from_group(i);
          out.push_back(std::pair<ListEntry *, Role *>(data[i], role));
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

  for (auto it : out)
  {
    res.append(it.second->get_colored_str());

    if (verbose)
    {
      res.append(" (");
      if (it.first->type() == ListEntry::Type::FACTION)
      {
        res.append("Random ");
      }
      res.append(it.first->name);
      res.append(")");
    }

    res.append("\n");
  }

  return res;
}

Role *RoleList::generate_role_from_role(int i)
{
  int faction_index = i;
  while (data[faction_index]->type() != ListEntry::Type::FACTION) faction_index++;

  if (counts[faction_index] == data[faction_index]->limit) throw Error("Too many of faction: %s (at line %d)", data[faction_index]->name.c_str(), line);

  counts[faction_index]++;

  int alignment_index = i;
  while (data[alignment_index]->type() != ListEntry::Type::ALIGNMENT) alignment_index++;

  if (counts[alignment_index] == data[alignment_index]->limit) throw Error("Too many of alignment: %s (at line %d)", data[alignment_index]->name.c_str(), line);

  counts[alignment_index]++;

  return static_cast<Role *>(data[i]);
}

Role *RoleList::generate_role_from_alignment(int i)
{
  int faction_index = i;
  while (data[faction_index]->type() != ListEntry::Type::FACTION) faction_index++;

  if (counts[faction_index] == data[faction_index]->limit) throw Error("Too many of faction: %s (at line %d)", data[faction_index]->name.c_str(), line);

  counts[faction_index]++;

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
    Alignment *alignment = entry;
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

Role *RoleList::generate_role_from_group(int i)
{
  Group *group = static_cast<Group *>(data[i]);
  while (true)
  {
    int rand = std::rand() % group->roles.size();

    if (counts[group->roles[rand].faction_index] == data[group->roles[rand].faction_index]->limit) continue;
    if (counts[group->roles[rand].alignment_index] == data[group->roles[rand].alignment_index]->limit) continue;
    if (counts[group->roles[rand].role_index] == data[group->roles[rand].role_index]->limit) continue;

    counts[group->roles[rand].faction_index]++;
    counts[group->roles[rand].alignment_index]++;
    counts[group->roles[rand].role_index]++;

    return group->roles[rand].role;
  }
}
