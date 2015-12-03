#ifndef PROBLEMNODE_H
#define PROBLEMNODE_H


#include "systemnode.h"

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

    ProblemNode(){index_space = new  astl::DFA_map<astl::strings,StateData_str>();}
    ProblemNode(std::string str){ name = str; index_space = new  astl::DFA_map<astl::strings,StateData_str>();}
    Component* find_component(std::string id);
    void make_terminals();

    astl::DFA_map<Transition,StateData_str>::state_type find_initial_state(std::string str);
};

#endif // PROBLEMNODE_H
