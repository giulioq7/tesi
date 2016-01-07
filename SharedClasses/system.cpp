#include "system.h"

System::System()
{
    name = "";
    acyclic = false;
}

System::System(std::string str)
{
    name = str;
    acyclic = false;
}

bool System::is_acyclic() const
{
    return acyclic;
}

void System::set_acyclic()
{
    acyclic = true;
}


SystemNode* System::find_node(std::string id)
{
    SystemNode* ref = NULL;
    for(vector<SystemNode>::iterator it = node_list.begin(); it != node_list.end(); it++)
    {
        if(it->name == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}


int System::pts_count()
{
    int count = 0;
    vector<SystemNode>::iterator it;
    for(it = node_list.begin(); it != node_list.end(); it++)
    {
        vector<astl::DFA_map<NetTransition,StateData_strList> *>::iterator it2;
        for(it2 = it->net_model->pattern_space.begin(); it2 != it->net_model->pattern_space.end(); it2++)
            count++;
    }
    return count;
}
