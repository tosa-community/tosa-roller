#include "role-list.hpp"

RoleList::RoleList(std::vector<std::string> input, std::vector<ListEntry *> data)
{
  this->data = data;
  for (auto entry : input) this->query.push_back(process_role_entry(entry));

  std::srand(std::time(nullptr));
}

RoleList::~RoleList()
{
  for (auto entry : data) delete entry;
}

void RoleList::generate()
{
  counts.resize(this->data.size(), 0);

  for (auto ientry : this->query)
  {
    line++;

    bool is_valid = false;
    for (int i = 0; i < data.size(); i++)
    {
      std::vector<std::string> aliases = data[i]->aliases;
      if (std::find(aliases.begin(), aliases.end(), ientry) != aliases.end())
      {
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

        is_valid = true;
        break;
      }
    }

    if (!is_valid) throw Error("Invalid role name: %s (at line %d)", ientry.c_str(), line);
  }

  targets.resize(output.size(), std::vector<int>());
}

Role *RoleList::generate_role_from_role(int i)
{
  Role *role = static_cast<Role *>(data[i]);

  if (counts[role->faction_pos] == data[role->faction_pos]->limit) throw Error("Too many of faction: %s (at line %d)", data[role->faction_pos]->name.c_str(), line);
  if (counts[role->alignment_pos] == data[role->alignment_pos]->limit) throw Error("Too many of alignment: %s (at line %d)", data[role->alignment_pos]->name.c_str(), line);
  if (counts[role->pos] == data[role->pos]->limit) throw Error("Too many of role: %s (at line %d)", data[role->pos]->name.c_str(), line);

  counts[role->faction_pos]++;
  counts[role->alignment_pos]++;
  counts[role->pos]++;

  return role;
}

Role *RoleList::generate_role_from_alignment(int i)
{
  Alignment *alignment = static_cast<Alignment *>(data[i]);

  if (counts[alignment->faction_pos] == data[alignment->faction_pos]->limit) throw Error("Too many of faction: %s (at line %d)", data[alignment->faction_pos]->name.c_str(), line);
  if (counts[alignment->pos] == data[alignment->pos]->limit) throw Error("Too many of alignment: %s (at line %d)", data[alignment->pos]->name.c_str(), line);

  counts[alignment->faction_pos]++;
  counts[alignment->pos]++;

  while (true)
  {
    Role *role = alignment->roles[std::rand() % alignment->roles.size()];

    if (counts[role->pos] != data[role->pos]->limit)
    {
      counts[role->pos]++;
      return static_cast<Role *>(data[role->pos]);
    }
  }
}

Role *RoleList::generate_role_from_faction(int i)
{
  Faction *faction = static_cast<Faction *>(data[i]);

  if (counts[faction->pos] == data[faction->pos]->limit) throw Error("Too many of faction: %s (at line %d)", data[faction->pos]->name.c_str(), line);
  counts[faction->pos]++;

  while (true)
  {
    Alignment *alignment = faction->alignments[std::rand() % faction->alignments.size()];

    if (counts[alignment->pos] != data[alignment->pos]->limit)
    {
      counts[alignment->pos]++;

      while (true)
      {
        Role *role = alignment->roles[std::rand() % alignment->roles.size()];

        if (counts[role->pos] != data[role->pos]->limit)
        {
          counts[role->pos]++;
          return static_cast<Role *>(data[role->pos]);
        }
      }
    }
  }
}

Role *RoleList::generate_role_from_group(int i)
{
  Group *group = static_cast<Group *>(data[i]);

  while (true)
  {
    Role *role = group->roles[std::rand() % group->roles.size()];

    if (counts[role->faction_pos] == data[role->faction_pos]->limit) continue;
    if (counts[role->alignment_pos] == data[role->alignment_pos]->limit) continue;
    if (counts[role->pos] == data[role->pos]->limit) continue;

    counts[role->faction_pos]++;
    counts[role->alignment_pos]++;
    counts[role->pos]++;

    return role;
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
  std::map<int, std::vector<ListEntry *>> bmap;
  std::map<int, std::vector<ListEntry *>> cmap;

  int i = 1;
  for (auto pscrolls : scrolls)
  {
    std::vector<ListEntry *> bscrolls;
    std::vector<ListEntry *> cscrolls;
    std::vector<std::string> strs;

    std::stringstream stream(pscrolls);
    std::string buf;
    while (std::getline(stream, buf, ',')) strs.push_back(process_role_entry(buf));

    for (auto str : strs)
    {
      for (auto e : data)
      {
        if (std::find(e->aliases.begin(), e->aliases.end(), str) != e->aliases.end())
        {
          bscrolls.push_back(e);
          break;
        }

        if (str[0] == '-')
        {
          std::string substr = str.substr(1);

          if (std::find(e->aliases.begin(), e->aliases.end(), substr) != e->aliases.end())
          {
            cscrolls.push_back(e);
            break;
          }
        }
      }
    }

    bmap[i] = bscrolls;
    cmap[i] = cscrolls;
    i++;
  }

  std::vector<int> shuffled;
  for (int i = 0; i < output.size(); i++)
  {
    shuffled.push_back(i + 1);
  }
  std::shuffle(shuffled.begin(), shuffled.end(), std::default_random_engine(std::time(nullptr)));

  std::vector<std::pair<ListEntry *, Role *>> temp;
  temp.resize(output.size(), std::pair<ListEntry *, Role *>(nullptr, nullptr));

  for (auto i : shuffled)
  {
    std::vector<Role *> choices;

    for (auto role : output)
    {
      int count = 10;

      auto bvec = bmap[i];

      for (auto e : bvec)
      {
        if (role.second->name == e->name) count += 90;
        if (data[role.second->alignment_pos]->name == e->name) count += 90;
        if (data[role.second->faction_pos]->name == e->name) count += 90;
      }

      auto cvec = cmap[i];

      for (auto e : cvec)
      {
        if (role.second->name == e->name) count = 1;
        if (data[role.second->alignment_pos]->name == e->name) count = 1;
        if (data[role.second->faction_pos]->name == e->name) count = 1;
      }

      choices.resize(choices.size() + count, role.second);
    }

    Role *role = choices[std::rand() % choices.size()];

    auto it = output.begin();
    for (auto j = output.begin(); j != output.end(); j++)
    {
      if (j->second->name == role->name)
      {
        it = j;
        temp[i - 1] = *j;
        break;
      }
    }

    output.erase(it);
  }

  output = temp;
}

void RoleList::generate_targets()
{
  for (int i = 0; i < output.size(); i++)
  {
    Role *role = output[i].second;
    for (auto target : role->target_data)
    {
      while (true)
      {
        int index = std::rand() % output.size();
        Role *target_role = output[index].second;

        std::cout << target_role->name << std::endl;

        if (std::find(target.exclude.begin(), target.exclude.end(), target_role->name) != target.exclude.end()) continue;

        ListEntry *target_alignment = data[target_role->alignment_pos];

        if (std::find(target.exclude.begin(), target.exclude.end(), target_alignment->name) != target.exclude.end()) continue;

        ListEntry *target_faction = data[target_role->faction_pos];

        if (std::find(target.exclude.begin(), target.exclude.end(), target_faction->name) != target.exclude.end()) continue;

        targets[i].push_back(index + 1);
        break;
      }
    }
  }
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

    auto target_data = output[i].second->target_data;
    if (targets[i].size())
    {
      res.append(" [");
      for (int j = 0; j < targets[i].size(); j++)
      {
        res.append(target_data[j].name);
        res.append(": ");
        res.append(std::to_string(targets[i][j]));
        if (j + 1 != targets[i].size()) res.append(", ");
      }
      res.append("]");
    }

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
