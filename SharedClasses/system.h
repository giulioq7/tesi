#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include "systemnode.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class System
{
    bool acyclic;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & dependency_graph;
        ar & acyclic;
        ar & node_list;
        ar & emergence;
    }

public:
    std::string name;

    astl::DFA_map<astl::strings,StateData_str> dependency_graph;

    vector<SystemNode> node_list;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  emergence;

    System();
    System(std::string str);

    bool is_acyclic() const;
    void set_acyclic();

    SystemNode* find_node(std::string);
    int pts_count();
};

#endif // SYSTEM_H
