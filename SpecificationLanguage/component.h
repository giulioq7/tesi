#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "componentmodel.h"
#include "outputterminal.h"

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
    Component(std::string str) { name = str; }
    ~Component(){automaton = astl::DFA_map<Transition,StateData_str>();}

    std::string name;
    ComponentModel* model;
    astl::DFA_map<Transition,StateData_str> automaton;
    vector<Terminal> input_terminals;
    vector<OutputTerminal> output_terminals;

    Terminal* find_terminal(std::string id);
    OutputTerminal* find_output_terminal(std::string id);

     bool operator==(const Component c) const {return name == c.name;}
};


#endif // COMPONENT_H
