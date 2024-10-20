#include "role-list.hpp"

RoleList::RoleList(std::string filename, std::vector<std::string> input, std::vector<ListEntry *> data, std::vector<Faction *> rq_factions)
{
  this->filename = filename;
  this->data = data;
  this->rq_factions = rq_factions;

  std::vector<Role *> roles;

  std::map<int, bool> wincons;

  if (input.size() < 5) {
    throw Error("Rolelist too short.");
  }

  for (auto entry : input)
  {
    std::string q = process_role_entry(entry);
    this->query.push_back(q);

    for (int i = 0; i < this->data.size(); i++)
    {
      std::vector<std::string> aliases = data[i]->aliases;
      if (std::find(aliases.begin(), aliases.end(), q) != aliases.end())
      {
        switch (data[i]->type())
        {
        case ListEntry::Type::ROLE:
        {
          Role *role = static_cast<Role *>(data[i]);
          wincons[role->wincon] = true;
          roles.push_back(role);
          break;
        }
        case ListEntry::Type::ALIGNMENT:
        {
          Alignment *alignment = static_cast<Alignment *>(data[i]);
          for (auto role : alignment->roles) {
            wincons[role->wincon] = true;
            roles.push_back(role);
          }
          break;
        }
        case ListEntry::Type::FACTION:
        {
          Faction *faction = static_cast<Faction *>(data[i]);
          for (auto alignment : faction->alignments)
          {
            for (auto role : alignment->roles) {
              wincons[role->wincon] = true;
              roles.push_back(role);
            }
          }
          break;
        }
        case ListEntry::Type::GROUP:
        {
          Group *group = static_cast<Group *>(data[i]);
          for (auto role : group->roles) wincons[role->wincon] = true;
          break;
        }
        }
        break;
      }
    }
  }
  if (wincons.size() < 2) throw Error("Invalid rolelist");
  for (auto entry : roles)
  {
    if (entry->oppose != -1 && !wincons.contains(entry->oppose)) throw Error("Invalid rolelist");
  }

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

    if (!is_valid) throw Error(filename, line, "Invalid role name: %s", ientry.c_str());
  }

  targets.resize(output.size(), std::vector<int>());

  std::map<int, bool> wincons;
  std::vector<int> rq_faction_counts;
  rq_faction_counts.resize(rq_factions.size(), 0);

  for (auto role : output)
  {
    if (role.second->wincon != -1)
    {
      if (!wincons.contains(role.second->wincon)) wincons[role.second->wincon] = true;
    }

    auto it = std::find(rq_factions.begin(), rq_factions.end(), data[role.second->faction_pos]);
    if (it != rq_factions.end()) rq_faction_counts[std::distance(rq_factions.begin(), it)]++;
  }

  for (int i = 0; i < rq_factions.size(); i++)
  {
    std::vector<int> faction_roles;
    bool cont = false;
    for (int j = 0; j < output.size(); j++)
    {
      if (rq_factions[i]->pos == output[j].second->faction_pos) faction_roles.push_back(j);
      if (std::find(rq_factions[i]->require.begin(), rq_factions[i]->require.end(), output[j].second) != rq_factions[i]->require.end())
      {
        cont = true;
        break;
      }
    }

    if (cont) continue;

    if (rq_faction_counts[i] < rq_factions[i]->require_min) continue;

    int index = faction_roles[std::rand() % faction_roles.size()];

    counts[output[index].second->pos]--;
    counts[output[index].second->alignment_pos]--;
    counts[output[index].second->faction_pos]--;

    switch (rq_factions[i]->fallback->type())
    {
    case ListEntry::Type::ROLE:
      output[index].second = generate_role_from_role(rq_factions[i]->fallback->pos);
      break;
    case ListEntry::Type::ALIGNMENT:
      output[index].second = generate_role_from_alignment(rq_factions[i]->fallback->pos);
      break;
    }
  }

  if (wincons.size() < 2)
  {
    output.clear();
    counts.clear();
    generate();
  }
  for (auto role : output)
  {
    if (role.second->oppose != -1 && !wincons.contains(role.second->oppose))
    {
      output.clear();
      counts.clear();
      generate();
    }
  }
}

Role *RoleList::generate_role_from_role(int i)
{
  Role *role = static_cast<Role *>(data[i]);

  if (counts[role->faction_pos] == data[role->faction_pos]->limit) throw Error(filename, line, "Too many of faction: %s", data[role->faction_pos]->name.c_str());
  if (counts[role->alignment_pos] == data[role->alignment_pos]->limit) throw Error(filename, line, "Too many of alignment: %s", data[role->alignment_pos]->name.c_str());
  if (counts[i] == data[i]->limit) throw Error(filename, line, "Too many of role: %s", role->name.c_str());

  counts[role->faction_pos]++;
  counts[role->alignment_pos]++;
  counts[i]++;

  return role;
}

Role *RoleList::generate_role_from_alignment(int i)
{
  Alignment *alignment = static_cast<Alignment *>(data[i]);

  if (counts[alignment->faction_pos] == data[alignment->faction_pos]->limit) throw Error(filename, line, "Too many of faction: %s", data[alignment->faction_pos]->name.c_str());
  if (counts[i] == data[i]->limit) throw Error(filename, line, "Too many of alignment: %s", alignment->name.c_str());

  counts[alignment->faction_pos]++;
  counts[i]++;

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

  if (counts[i] == data[i]->limit) throw Error(filename, line, "Too many of faction: %s", faction->name.c_str());
  counts[i]++;

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

        if (index == i) continue;

        if (std::find(targets[i].begin(), targets[i].end(), index) != targets[i].end()) continue;

        bool cont = false;
        for (auto excl : target.exclude)
        {
          Role *target_role = output[index].second;
          if (excl == target_role->name)
          {
            cont = true;
            break;
          }

          ListEntry *target_alignment = data[target_role->alignment_pos];
          if (excl == target_alignment->name)
          {
            cont = true;
            break;
          }

          ListEntry *target_faction = data[target_role->faction_pos];
          if (excl == target_faction->name)
          {
            cont = true;
            break;
          }

          auto it = std::find(excl.begin(), excl.end(), ':');
          if (it != excl.end())
          {
            int num = std::stoi(excl.substr(0, std::distance(excl.begin(), it)));
            std::string type = excl.substr(std::distance(excl.begin(), it) + 1);

            if (num >= targets[i].size()) throw Error("Invalid data file");

            Role *cf = output[targets[i][num]].second;

            if (type == "role" && cf == target_role)
            {
              cont = true;
              break;
            }

            if (type == "alignment" && data[cf->alignment_pos] == target_alignment)
            {
              cont = true;
              break;
            }

            if (type == "faction" && data[cf->faction_pos] == target_faction)
            {
              cont = true;
              break;
            }

            if (type == "wincon" && cf->wincon == target_role->wincon)
            {
              cont = true;
              break;
            }
          }
        }

        if (cont) continue;

        targets[i].push_back(index);
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
        res.append(std::to_string(targets[i][j] + 1));
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
