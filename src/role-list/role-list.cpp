#include "role-list.hpp"

RoleList::RoleList(std::vector<std::string> input, std::vector<ListEntry *> data)
{
  this->data = data;
  for (auto entry : input) this->query.push_back(process_role_entry(entry));
}

void RoleList::generate()
{
  std::srand(std::time(nullptr));

  counts.resize(this->data.size(), 0);

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
          output.push_back(std::pair<ListEntry *, Role *>(data[i], static_cast<Role *>(data[i])));

          break;
        }
        case ListEntry::Type::ALIGNMENT:
        {
          Role *role = generate_role_from_alignment(i);
          output.push_back(std::pair<ListEntry *, Role *>(data[i], role));

          break;
        }
        case ListEntry::Type::FACTION:
        {
          Role *role = generate_role_from_faction(i);
          output.push_back(std::pair<ListEntry *, Role *>(data[i], role));

          break;
        }
        case ListEntry::Type::GROUP:
        {
          Role *role = generate_role_from_group(i);
          output.push_back(std::pair<ListEntry *, Role *>(data[i], role));
        }
        }

        role_is_valid = true;
        break;
      }
    }

    if (!role_is_valid) throw Error("Invalid role name: %s (at line %d)", input_entry.c_str(), line);
  }
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
  counts[i]++;

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

std::string RoleList::process_role_entry(std::string input)
{
  std::string output;

  for (auto c : input)
  {
    output.push_back(std::tolower(c));
  }

  output.erase(output.begin(), std::find_if(output.begin(), output.end(), [](unsigned char c) {
    return !std::isspace(c);
  }));
  output.erase(std::find_if(output.rbegin(), output.rend(), [](unsigned char c) {
    return !std::isspace(c);
  }).base(), output.end());

  return output;
}

std::string RoleList::to_string(bool verbose, bool color)
{
  std::string res;

  for (int i = 0; i < output.size(); i++)
  {
    res.append("[");
    res.append(std::to_string(i+1));
    res.append("] - ");
    res.append(output[i].second->get_colored_str(color));

    if (verbose)
    {
      res.append(" (");
      if (output[i].first->type() == ListEntry::Type::FACTION)
      {
        res.append("Random ");
      }
      res.append(output[i].first->name);
      res.append(")");
    }

    res.append("\n");
  }

  return res;
}

void RoleList::shuffle()
{
  std::vector<std::string> vec;
  vec.resize(output.size(), "");
  _shuffle(vec);
}

void RoleList::shuffle(std::vector<std::string> scrolls)
{
  _shuffle(scrolls);
}

void RoleList::_shuffle(std::vector<std::string> scrolls)
{
  std::vector<std::tuple<Role *, Alignment *, Faction *>> roles;
  for (int i = 0; i < data.size(); i++)
  {
    for (auto j : output)
    {
      if (data[i]->name == j.second->name)
      {
        Role *role = j.second;

        int alignment_index = i;
        while (data[alignment_index]->type() != ListEntry::Type::ALIGNMENT) alignment_index++;

        Alignment *alignment = static_cast<Alignment *>(data[alignment_index]);

        int faction_index = i;
        while (data[faction_index]->type() != ListEntry::Type::FACTION) faction_index++;

        Faction *faction = static_cast<Faction *>(data[faction_index]);

        roles.push_back(std::tuple<Role *, Alignment *, Faction *>(role, alignment, faction));
      }
    }
  }

  std::map<int, std::vector<ListEntry *>> blessed_map;
  std::map<int, std::vector<ListEntry *>> cursed_map;

  int i = 1;
  for (auto entry : scrolls)
  {
    std::vector<ListEntry *> blessed_scrolls;
    std::vector<ListEntry *> cursed_scrolls;
    std::vector<std::string> strs;

    std::stringstream stream(entry);
    std::string buf;
    while (std::getline(stream, buf, ','))
    {
      strs.push_back(process_role_entry(buf));
    }

    for (auto data_entry : data)
    {
      for (auto str : strs)
      {
        if (std::find(data_entry->aliases.begin(), data_entry->aliases.end(), str) != data_entry->aliases.end())
        {
          blessed_scrolls.push_back(data_entry);
        }

        if (str[0] == '-')
        {
          std::string substr = str.substr(1);

          if (std::find(data_entry->aliases.begin(), data_entry->aliases.end(), substr) != data_entry->aliases.end())
          {
            cursed_scrolls.push_back(data_entry);
          }
        }
      }
    }

    blessed_map[i] = blessed_scrolls;
    cursed_map[i] = cursed_scrolls;
    i++;
  }

  std::vector<int> rand_list;

  for (int i = 1; i < roles.size() + 1; i++)
  {
    rand_list.push_back(i);
  }

  std::shuffle(rand_list.begin(), rand_list.end(), std::default_random_engine(std::time(nullptr)));

  std::vector<std::pair<ListEntry *, Role *>> temp;
  temp.resize(output.size(), std::pair<ListEntry *, Role *>(nullptr, nullptr));

  for (auto i : rand_list)
  {
    std::vector<Role *> choices;

    for (auto role : roles)
    {
      int count = 10;

      auto blessed_vec = blessed_map[i];

      for (auto list_entry : blessed_vec)
      {
        if (std::get<0>(role)->name == list_entry->name) count += 90;
        if (std::get<1>(role)->name == list_entry->name) count += 90;
        if (std::get<2>(role)->name == list_entry->name) count += 90;
      }

      auto cursed_vec = cursed_map[i];

      for (auto list_entry : cursed_vec)
      {
        if (std::get<0>(role)->name == list_entry->name) count = 1;
        break;
        if (std::get<1>(role)->name == list_entry->name) count = 1;
        break;
        if (std::get<2>(role)->name == list_entry->name) count = 1;
        break;
      }

      choices.resize(choices.size() + count, std::get<0>(role));
    }

    Role *role = choices[std::rand() % choices.size()];

    std::vector<std::tuple<Role *, Alignment *, Faction *>>::iterator it;
    int to_rm = 0;
    for (auto j = roles.begin(); j != roles.end(); j++)
    {
      if (std::get<0>(*j)->name == role->name)
      {
        it = j;
        to_rm = std::distance(roles.begin(), j);
        temp[i - 1] = output[to_rm];
        break;
      }
    }

    output.erase(output.begin() + to_rm);
    roles.erase(it);
  }

  output.clear();
  output = temp;
}
