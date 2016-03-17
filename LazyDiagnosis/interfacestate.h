#ifndef INTERFACESTATE_H
#define INTERFACESTATE_H

#include "astl.h"
#include "nfa_mmap.h"
#include <set>
#include <sstream>
#include "decoration.h"
#include "interfacetrans.h"
#include "behaviorstate.h"

/**
 * @brief The InterfaceState class represents a state of a interface
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class InterfaceState
{

    /**
     * @brief states list of contained states
     */
    std::set<unsigned int> states;
    /**
     * @brief automaton with the involved states
     */
    astl::NFA_mmap<InterfaceTrans,BehaviorState> *automaton;
    /**
     * @brief state_map maps between state of behavior and current state
     */
    std::map<unsigned int, unsigned int> state_map;
    /**
     * @brief delta diagnosis relative to the single state
     */
    std::set<std::set<std::string> > delta;
    /**
     * @brief pattern_events
     */
    std::set<std::string> pattern_events;
public:
    /**
     * @brief InterfaceState
     */
    InterfaceState();
    /**
     * @brief InterfaceState
     * @param s
     */
    InterfaceState(std::set<unsigned int> s);
    ~InterfaceState();

    /**
     * @brief get_states
     * @return
     */
    std::set<unsigned int> get_states() const;
    /**
     * @brief get_automaton
     * @return
     */
    astl::NFA_mmap<InterfaceTrans,BehaviorState>* get_automaton() const;
    /**
     * @brief get_state_map
     * @return
     */
    std::map<unsigned int, unsigned int> get_state_map() const;
    /**
     * @brief get_delta
     * @return
     */
    std::set<std::set<std::string> > get_delta() const;
    /**
     * @brief get_pattern_events
     * @return
     */
    std::set<std::string> get_pattern_events() const;

    /**
     * @brief set_automaton
     * @param a
     */
    void set_automaton(astl::NFA_mmap<InterfaceTrans,BehaviorState> *a);
    /**
     * @brief set_state_map
     * @param smap
     */
    void set_state_map(map<unsigned int, unsigned int> smap);
    /**
     * @brief set_delta
     * @param d
     */
    void set_delta(std::set<std::set<std::string> > d);
    /**
     * @brief set_pattern_events
     * @param events
     */
    void set_pattern_events(std::set<std::string> events);

    /**
     * @brief delete_automaton
     */
    void delete_automaton();
};


/**
 * @brief operator << print operator
 * @param out
 * @param s
 * @return
 */
std::ostream& operator<<(std::ostream& out, const InterfaceState& s);

#endif // INTERFACESTATE_H
