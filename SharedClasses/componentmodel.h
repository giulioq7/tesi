#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H
#include <astl.h>
#include <nfa_mmap.h>
#include "transition.h"
#include "statedata.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class ComponentModel{
    std::string name;
    vector<std::string> events;
    vector<std::string> inputs;
    vector<std::string> outputs;
    vector<std::string> states;
    vector<Transition> trans;
    astl::DFA_map<Transition,StateData_str> *automaton;
    std::vector<astl::DFA_map<Transition,StateData_str>::state_type> autom_states;

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
    ComponentModel();
    ComponentModel(std::string str) { name = str; /*automaton = new astl::DFA_map<Transition,StateData_str>();*/}

    std::string get_name() const;
    vector<std::string> get_events() const;
    vector<std::string> get_inputs() const;
    vector<std::string> get_outputs() const;
    vector<std::string> get_states() const;
    vector<Transition> get_trans() const;
    astl::DFA_map<Transition,StateData_str>* get_automaton() const;

    void set_events(vector<std::string> v);
    void set_inputs(vector<std::string> v);
    void set_outputs(vector<std::string> v);
    void set_states(vector<std::string> v);
    void set_trans(vector<Transition> v);


    void add_event(std::string ev);
    void add_input(std::string in);
    void add_output(std::string out);
    void add_state(std::string st);

    void build_automaton();
    astl::DFA_map<Transition,StateData_str>::state_type find_state(std::string name);
    Transition* find_trans(std::string name_tr);

private:
    void build_states();

};

#endif // COMPONENTMODEL_H
