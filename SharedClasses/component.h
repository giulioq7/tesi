#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "componentmodel.h"
#include "terminal.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Component
{

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & model;
        ar & automaton;
        ar & input_terminals;
        ar & output_terminals;
    }

public:
    Component();
    Component(std::string str) { name = str; automaton = new astl::DFA_map<Transition,StateData_str>(); }
    ~Component(){}

    std::string name;
    ComponentModel* model;
    astl::DFA_map<Transition,StateData_str> *automaton;
    vector<Terminal> input_terminals;
    vector<Terminal> output_terminals;

    Terminal* find_input_terminal(std::string id);
    Terminal* find_output_terminal(std::string id);

     bool operator==(const Component c) const {return name == c.name;}
};


#endif // COMPONENT_H
