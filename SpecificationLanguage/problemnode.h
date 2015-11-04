#ifndef PROBLEMNODE_H
#define PROBLEMNODE_H


#include "systemnode.h"

class ProblemNode
{
public:
    std::string name;

    SystemNode* ref_node;

    vector<Component> concrete_components;

    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;
    vector<std::string> observation;

    ProblemNode();
    ProblemNode(std::string str){ name = str;}
    Component* find_component(std::string id);
};

#endif // PROBLEMNODE_H
