#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "componentmodel.h"
#include "systransition.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>

class Component : public NetComponent
{
    astl::DFA_map<SysTransition,StateData_str> *automaton;
    vector<Terminal*> input_terminals;
    vector<Terminal*> output_terminals;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<NetComponent>(*this);
        ar & automaton;
        ar & input_terminals;
        ar & output_terminals;
    }

public:
    Component();
    Component(std::string str){name = str;}
    ~Component(){}

    astl::DFA_map<SysTransition,StateData_str>* get_automaton() const;
    vector<Terminal*> get_input_terminals() const;
    vector<Terminal*> get_output_terminals() const;

    void set_automaton(astl::DFA_map<SysTransition,StateData_str>* autom);
    void add_input_term(Terminal* t);
    void add_output_term(Terminal* t);

    Terminal* find_input_terminal(std::string id);
    Terminal* find_output_terminal(std::string id);

    bool operator==(const Component c) const {return get_name() == c.get_name();}
};

#endif // COMPONENT_H
