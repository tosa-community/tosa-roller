#include "group.hpp"

Group::Group(std::string name, int limit, std::vector<std::string> aliases, std::vector<ListEntry *> entries, std::vector<std::string> factions, std::vector<std::string> alignments, std::vector<std::string> roles) : ListEntry(name, limit, aliases)
{
  for (unsigned int i = 0; i < entries.size(); i++)
  {
    for (auto faction : factions)
    {
      if (entries[i]->name == faction)
      {
        int j = i - 1;
        int alignment_index;

        while (entries[j]->type() != ListEntry::Type::FACTION)
        {
          if (entries[j]->type() == ListEntry::Type::ALIGNMENT) alignment_index = j;
          else if (entries[j]->type() == ListEntry::Type::ROLE)
          {
            Group::RoleData role_data;
            role_data.faction_index = i;
            role_data.alignment_index = alignment_index;
            role_data.role_index = j;
            role_data.role = static_cast<Role *>(entries[j]);

            this->roles.push_back(role_data);
          }

          j--;
          if (j == -1) break;
        }
      }
    }

    for (auto alignment : alignments)
    {
      if (entries[i]->name == alignment)
      {
        int faction_index = i;

        while (entries[faction_index]->type() != ListEntry::Type::FACTION) faction_index++;

        auto roles = static_cast<Alignment *>(entries[i])->roles;
        for (int j = 0; j < roles.size(); j++)
        {
          Group::RoleData role_data;
          role_data.faction_index = faction_index;
          role_data.alignment_index = i;
          role_data.role_index = i - roles.size() + j;
          role_data.role = roles[j];

          this->roles.push_back(role_data);
        }

        break;
      }
    }

    for (auto role : roles)
    {
      if (entries[i]->name == role)
      {
        Group::RoleData role_data;
        role_data.faction_index = i;
        role_data.alignment_index = i;
        role_data.role_index = i;
        role_data.role = static_cast<Role *>(entries[i]);

        while (entries[role_data.faction_index]->type() != ListEntry::Type::FACTION) role_data.faction_index++;

        while (entries[role_data.alignment_index]->type() != ListEntry::Type::ALIGNMENT) role_data.alignment_index++;

        this->roles.push_back(role_data);

        break;
      }
    }
  }

  for (unsigned int i = 1; i < this->roles.size(); i++)
  {
    for (unsigned int j = 0; j < i; j++)
    {
      if (this->roles[i].role_index == this->roles[j].role_index)
      {
        this->roles.erase(this->roles.begin() + i);
        i--;
        break;
      }
    }
  }
}

Group::~Group()
{}

ListEntry::Type Group::type() { return ListEntry::Type::GROUP; }
