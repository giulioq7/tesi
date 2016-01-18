#include "interfacestate.h"

using namespace std;
using namespace astl;

InterfaceState::InterfaceState()
{
    //automaton = new astl::NFA_mmap<astl::strings,SatelliteData>;
}

InterfaceState::InterfaceState(std::set<unsigned int> s)
{
    states = s;
    //automaton = new astl::NFA_mmap<astl::strings,SatelliteData>;
}

InterfaceState::~InterfaceState()
{
    //delete automaton;
}


set<unsigned int> InterfaceState::get_states() const
{
    return states;
}


astl::NFA_mmap<InterfaceTrans, BehaviorState> *InterfaceState::get_automaton() const
{
    return automaton;
}


std::map<unsigned int, unsigned int> InterfaceState::get_state_map() const
{
    return state_map;
}

std::set<std::set<std::string> > InterfaceState::get_delta() const
{
    return delta;
}

std::set<std::string> InterfaceState::get_pattern_events() const
{
    return pattern_events;
}


void InterfaceState::set_automaton(astl::NFA_mmap<InterfaceTrans,BehaviorState> *a)
{
    automaton = new astl::NFA_mmap<InterfaceTrans,BehaviorState>;
    *automaton = *a;
}


void InterfaceState::set_state_map(map<unsigned int, unsigned int> smap)
{
    state_map = smap;
}

void InterfaceState::set_delta(std::set<std::set<std::string> > d)
{
    delta = d;
}

void InterfaceState::set_pattern_events(std::set<std::string> events)
{
    pattern_events = events;
}


void InterfaceState::delete_automaton()
{
    delete automaton;
}


std::ostream& operator<<(std::ostream& out, const InterfaceState& s)
{
    out << s.get_states();
    if(!s.get_delta().empty())
        out << endl << s.get_delta();

    return out;
}
