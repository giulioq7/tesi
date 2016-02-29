#include "systemnode.h"

SystemNode::SystemNode()
{
}

std::string SystemNode::get_name() const
{
    return name;
}

NetworkModel* SystemNode::get_net_model() const
{
    return net_model;
}

vector<pair<std::string,std::string> > SystemNode::get_initials() const
{
    return initials;
}

map<pair<string,string>,string>   SystemNode::get_viewer() const
{
    return viewer;
}

map<pair<string,string>,string>   SystemNode::get_ruler() const
{
    return ruler;
}

void SystemNode::set_net_model(NetworkModel* nm)
{
    net_model = nm;
}

void SystemNode::set_initials(vector<pair<std::string,std::string> > ins)
{
    initials = ins;
}

void SystemNode::set_viewer(map<pair<string,string>,string> vwr)
{
    viewer = vwr;
}

void SystemNode::set_ruler(map<pair<string,string>,string> rlr)
{
    ruler = rlr;
}
