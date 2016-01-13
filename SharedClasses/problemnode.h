#ifndef PROBLEMNODE_H
#define PROBLEMNODE_H


#include "systemnode.h"
#include "component.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class ProblemNode
{

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & ref_node;
        ar & input_terminals;
        ar & output_terminals;
        ar & concrete_components;
        ar & initials;
        ar & viewer;
        ar & ruler;
        ar & observation;
        ar & index_space;
        ar & depends;
        ar & patt_map;
        ar & patt_indexes_map;
        ar & lazy_patt_indexes_map;
    }

public:
    std::string name;

    SystemNode* ref_node;

    vector<Component> concrete_components;
    vector<Terminal*> input_terminals;
    vector<Terminal*> output_terminals;


    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;
    vector<std::string> observation;

    astl::DFA_map<astl::strings,StateData_str> *index_space;

    vector<int> depends;

    map<std::string,Terminal*> patt_map;
    map<std::string,vector<int> > patt_indexes_map;
    map<std::string,vector<int> > lazy_patt_indexes_map;

    ProblemNode(){/*index_space = new  astl::DFA_map<astl::strings,StateData_str>();*/}
    ProblemNode(std::string str){ name = str; /*index_space = new  astl::DFA_map<astl::strings,StateData_str>();*/}
    Component* find_component(std::string id);
    void make_terminals();

    int input_terminals_count();

    astl::DFA_map<Transition,StateData_str>::state_type find_initial_state(std::string str);
};

#endif // PROBLEMNODE_H
