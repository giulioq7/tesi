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


std::ostream& operator<<(std::ostream& out, const InterfaceState& s)
{
    set<unsigned int> states = s.get_states();
    stringstream ss;
    ss << "{";
    for(set<unsigned int>::iterator it = states.begin(); it != states.end(); it++)
        ss << *it << ",";
    string str(ss.str());
    str.resize(ss.str().size()-1);
    str.append("}");

    set<set<string> > delta = s.get_delta();
    if(!delta.empty())
    {
        str.append("\n(");
        for(set<set<string> >::iterator it = delta.begin(); it != delta.end(); it++)
        {
            str.append("(");
            for(set<string>::iterator it2 = it->begin(); it2 != it->end(); it2++)
            {
                str.append(*it2);
                str.append(",");
            }
            if(!it->empty())
                str.resize(str.size()-1);
            else
                str.append(" ");
            str.append(")");
        }
        str.append(")");
    }

    return out << str;
}
