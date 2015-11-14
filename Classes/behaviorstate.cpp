#include "behaviorstate.h"

using namespace astl;

BehaviorState::BehaviorState()
{
}

BehaviorState::BehaviorState(int n, int m, int m2, int k, int i)
{
    n_comps = n;
    n_inputs = m;
    n_complex_inputs = m2;
    n_pts = k;
    n_isp = i;
    S = new astl::forward_cursor<astl::DFA_map<Transition,StateData_str> >[n_comps];
    E = new std::string[n_inputs];
    Complex_E = new std::string[n_complex_inputs];
    P = new astl::forward_cursor<astl::DFA_map<NetTransition,StateData_str> >[n_pts];
    I = new astl::forward_cursor<astl::DFA_map<astl::strings,StateData_str> >[n_isp];
}
