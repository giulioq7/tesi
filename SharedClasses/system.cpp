#include "system.h"

System::System()
{
    name = "";
    acyclic = false;
    dependency_graph = new astl::DFA_map<astl::strings,StateData_str>();
}

System::System(std::string str)
{
    name = str;
    acyclic = false;
    dependency_graph = new astl::DFA_map<astl::strings,StateData_str>();
}

std::string System::get_name() const
{
    return name;
}

astl::DFA_map<astl::strings,StateData_str>* System::get_dependency_graph() const
{
    return dependency_graph;
}

vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > System::get_emergence() const
{
    return emergence;
}

void System::set_dependency_graph(astl::DFA_map<astl::strings,StateData_str>* dg)
{
    dependency_graph = dg;
}

void System::add_node(SystemNode sn)
{
    node_list.push_back(sn);
}

void System::add_emergence(pair<pair<std::string,std::string>,pair<std::string,std::string> >  e)
{
    emergence.push_back(e);
}

void System::set_node_list(vector<SystemNode> nl)
{
    node_list = nl;
}

void System::set_emergence(vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  emer)
{
    emergence = emer;
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
        if(it->get_name() == id)
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
        vector<astl::DFA_map<NetTransition,StateData_strList> *> pts = it->get_net_model()->get_pattern_space();
        vector<astl::DFA_map<NetTransition,StateData_strList> *>::iterator it2;
        for(it2 = pts.begin(); it2 != pts.end(); it2++)
            count++;
    }
    return count;
}
