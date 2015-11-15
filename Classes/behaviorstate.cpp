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
    for(int i=0; i<m; i++)
        E[i] = "<eps>";
    Complex_E = new std::string[n_complex_inputs];
    for(int i=0; i<m2; i++)
        Complex_E[i] = "<eps>";
    P = new astl::forward_cursor<astl::DFA_map<NetTransition,StateData_str> >[n_pts];
    I = new astl::forward_cursor<astl::DFA_map<astl::strings,StateData_str> >[n_isp];
}

BehaviorState::~BehaviorState()
{
}
