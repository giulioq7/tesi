#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include "systemnode.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class System
{
    std::string name;
    astl::DFA_map<astl::strings,StateData_str>* dependency_graph;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  emergence;
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
    System();
    System(std::string str);

    vector<SystemNode> node_list;
    std::string get_name() const;
    astl::DFA_map<astl::strings,StateData_str>* get_dependency_graph() const;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > get_emergence() const;

    void build_dependency_graph();
    void add_node(SystemNode sn);
    void add_emergence(pair<pair<std::string,std::string>,pair<std::string,std::string> >  e);
    void set_node_list(vector<SystemNode> nl);
    void set_emergence(vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  emer);

    void set_dependency_graph(astl::DFA_map<astl::strings,StateData_str>* dg);
    bool is_acyclic() const;
    void set_acyclic();

    SystemNode* find_node(std::string);
    int pts_count();
};

#endif // SYSTEM_H
