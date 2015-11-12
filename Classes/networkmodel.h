#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <map>
#include <sstream>
#include "utils.h"
#include "component.h"
#include "nettransition.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class NetworkModel
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & components;
        ar & links;
        ar & patterns;
        ar & initials;
        ar & viewer;
        ar & ruler;
        ar & pattern_space;
    }

public:
    std::string name;
    vector<Component> components;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > links;

    vector<pair<std::string,std::string> > patterns;
    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;

    astl::DFA_map<NetTransition,StateData_str> pattern_space;

    int count;
    std::map<std::pair<std::string,std::string>,int> conv_str_int;
    std::map<int,std::pair<std::string,std::string> > conv_int_str;

    NetworkModel();
    NetworkModel(std::string str) { name = str; count = 0;}

    std::string not_trans(std::string operand);
    Component* find_component(std::string id);
};

#endif // NETWORKMODEL_H
