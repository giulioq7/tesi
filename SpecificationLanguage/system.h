#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include "systemnode.h"

class System
{
public:
    std::string name;

    astl::DFA_map<astl::strings,StateData> dependency_graph;

    vector<SystemNode> node_list;
    std::string id_root;
    SystemNode* root;
    map<pair<string,string>,string>  emergence;

    System();
    System(std::string str) { name = str;}

    SystemNode* find_node(std::string);
};

#endif // SYSTEM_H
