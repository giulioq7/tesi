#ifndef INTERFACESTATE_H
#define INTERFACESTATE_H

#include "astl.h"
#include "nfa_mmap.h"
#include <set>
#include <sstream>
#include "decoration.h"
#include "interfacetrans.h"
#include "behaviorstate.h"

class InterfaceState
{

    std::set<unsigned int> states;
    astl::NFA_mmap<InterfaceTrans,BehaviorState> *automaton;
    std::map<unsigned int, unsigned int> state_map;
    std::set<std::set<std::string> > delta;
    std::set<std::string> pattern_events;
public:
    InterfaceState();
    InterfaceState(std::set<unsigned int> s);
    ~InterfaceState();

    std::set<unsigned int> get_states() const;
    astl::NFA_mmap<InterfaceTrans,BehaviorState>* get_automaton() const;
    std::map<unsigned int, unsigned int> get_state_map() const;
    std::set<std::set<std::string> > get_delta() const;
    std::set<std::string> get_pattern_events() const;

    void set_automaton(astl::NFA_mmap<InterfaceTrans,BehaviorState> *a);
    void set_state_map(map<unsigned int, unsigned int> smap);
    void set_delta(std::set<std::set<std::string> > d);
    void set_pattern_events(std::set<std::string> events);
};


std::ostream& operator<<(std::ostream& out, const InterfaceState& s);

#endif // INTERFACESTATE_H
