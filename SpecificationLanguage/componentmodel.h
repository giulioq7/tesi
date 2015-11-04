#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H
#include <astl.h>
#include <nfa_mmap.h>
#include "transition.h"
#include "statedata.h"

class ComponentModel{


    std::vector<astl::DFA_map<Transition,StateData>::state_type> autom_states;
public:
    std::string name ;

    astl::DFA_map<Transition,StateData> automaton;

    vector<std::string> events;
    vector<std::string> inputs;
    vector<std::string> outputs;
    vector<std::string> states;

    vector<Transition> trans;

    ComponentModel();
    ComponentModel(std::string str) { name = str;}

    void build_automaton();
    Transition* find_trans(std::string name_tr);

private:
    void build_states();
    astl::DFA_map<Transition,StateData>::state_type find_state(std::string name);
};


#endif // COMPONENTMODEL_H
