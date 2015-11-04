#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "networkmodel.h"

class SystemNode
{
public:
    std::string name;

    NetworkModel* net_model;

    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;

    SystemNode();
    SystemNode(std::string str) { name = str;}
};

#endif // NODE_H
