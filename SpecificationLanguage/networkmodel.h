#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <map>
#include <sstream>
#include "utils.h"
#include "component.h"
#include "nettransition.h"

class NetworkModel
{
public:
    std::string name;
    vector<Component> components;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > links;

    vector<pair<std::string,std::string> > patterns;
    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;

    astl::DFA_map<NetTransition,StateData> pattern_space;

    int count;
    std::map<std::pair<std::string,std::string>,int> conv_str_int;
    std::map<int,std::pair<std::string,std::string> > conv_int_str;

    NetworkModel();
    NetworkModel(std::string str) { name = str; count = 0;}

    std::string not_trans(std::string operand);
    Component* find_component(std::string id);
};

#endif // NETWORKMODEL_H
