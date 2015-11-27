#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include "systemnode.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class System
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & dependency_graph;
        ar & node_list;
        ar & id_root;
        ar & root;
        ar & emergence;
    }

public:
    std::string name;

    astl::DFA_map<astl::strings,StateData_str> dependency_graph;

    vector<SystemNode> node_list;
    std::string id_root;
    SystemNode* root;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  emergence;

    System();
    System(std::string str) { name = str;}

    SystemNode* find_node(std::string);
};

#endif // SYSTEM_H
