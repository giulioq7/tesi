#ifndef COMPONENTMODEL_H
#define COMPONENTMODEL_H
#include <astl.h>
#include <nfa_mmap.h>
#include "transition.h"
#include "statedata.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The ComponentModel class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class ComponentModel{
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief events
     */
    vector<std::string> events;
    /**
     * @brief inputs
     */
    vector<std::string> inputs;
    /**
     * @brief outputs
     */
    vector<std::string> outputs;
    /**
     * @brief states
     */
    vector<std::string> states;
    /**
     * @brief trans
     */
    vector<Transition> trans;
    /**
     * @brief automaton behavioral model
     */
    astl::DFA_map<Transition,StateData_str> *automaton;
    /**
     * @brief autom_states used during automaton construction
     */
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
    /**
     * @brief ComponentModel default constructor
     */
    ComponentModel();
    /**
     * @brief ComponentModel constructor from id
     * @param str
     */
    ComponentModel(std::string str) { name = str; /*automaton = new astl::DFA_map<Transition,StateData_str>();*/}

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_events
     * @return
     */
    vector<std::string> get_events() const;
    /**
     * @brief get_inputs
     * @return
     */
    vector<std::string> get_inputs() const;
    /**
     * @brief get_outputs
     * @return
     */
    vector<std::string> get_outputs() const;
    /**
     * @brief get_states
     * @return
     */
    vector<std::string> get_states() const;
    /**
     * @brief get_trans
     * @return
     */
    vector<Transition> get_trans() const;
    /**
     * @brief get_automaton
     * @return
     */
    astl::DFA_map<Transition,StateData_str>* get_automaton() const;

    /**
     * @brief set_events
     * @param v
     */
    void set_events(vector<std::string> v);
    /**
     * @brief set_inputs
     * @param v
     */
    void set_inputs(vector<std::string> v);
    /**
     * @brief set_outputs
     * @param v
     */
    void set_outputs(vector<std::string> v);
    /**
     * @brief set_states
     * @param v
     */
    void set_states(vector<std::string> v);
    /**
     * @brief set_trans
     * @param v
     */
    void set_trans(vector<Transition> v);


    /**
     * @brief add_event
     * @param ev
     */
    void add_event(std::string ev);
    /**
     * @brief add_input
     * @param in
     */
    void add_input(std::string in);
    /**
     * @brief add_output
     * @param out
     */
    void add_output(std::string out);
    /**
     * @brief add_state
     * @param st
     */
    void add_state(std::string st);

    /**
     * @brief build_automaton
     */
    void build_automaton();
    /**
     * @brief find_state from name
     * @param name
     * @return automaton state
     */
    astl::DFA_map<Transition,StateData_str>::state_type find_state(std::string name);
    /**
     * @brief find_trans finds transition from id
     * @param name_tr transition name
     * @return pointer to Transition
     */
    Transition* find_trans(std::string name_tr);

private:
    /**
     * @brief build_states of automaton
     */
    void build_states();

};

#endif // COMPONENTMODEL_H
