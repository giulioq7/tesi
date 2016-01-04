#ifndef INTERFACESTATE_H
#define INTERFACESTATE_H

#include "astl.h"
#include "nfa_mmap.h"
#include "satellitedata.h"
#include <set>
#include <sstream>
#include "decoration.h"

class InterfaceState
{

    std::set<unsigned int> states;
    astl::NFA_mmap<astl::strings,SatelliteData> *automaton;
    std::map<unsigned int, unsigned int> state_map;
public:
    InterfaceState();
    InterfaceState(std::set<unsigned int> s);
    ~InterfaceState();

    std::set<unsigned int> get_states() const;
    astl::NFA_mmap<astl::strings,SatelliteData>* get_automaton() const;
    std::map<unsigned int, unsigned int> get_state_map() const;

    void set_automaton(astl::NFA_mmap<astl::strings,SatelliteData> *a);
    void set_state_map(map<unsigned int, unsigned int> smap);
};


std::ostream& operator<<(std::ostream& out, const InterfaceState& s);

#endif // INTERFACESTATE_H
