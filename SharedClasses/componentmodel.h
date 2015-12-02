#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H
#include <astl.h>
#include <nfa_mmap.h>
#include "transition.h"
#include "statedata.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class ComponentModel{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & autom_states;
        ar & name;
        ar & automaton;
        ar & events;
        ar & inputs;
        ar & outputs;
        ar & states;
        ar & trans;
    }

public:
    std::vector<astl::DFA_map<Transition,StateData_str>::state_type> autom_states;

    std::string name ;

    astl::DFA_map<Transition,StateData_str> *automaton;

    vector<std::string> events;
    vector<std::string> inputs;
    vector<std::string> outputs;
    vector<std::string> states;

    vector<Transition> trans;

    ComponentModel();
    ComponentModel(std::string str) { name = str; automaton = new astl::DFA_map<Transition,StateData_str>();}

    void build_automaton();
    astl::DFA_map<Transition,StateData_str>::state_type find_state(std::string name);
    Transition* find_trans(std::string name_tr);

private:
    void build_states();

};


#endif // COMPONENTMODEL_H
