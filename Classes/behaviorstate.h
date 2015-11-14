#ifndef BEHAVIORSTATE_H
#define BEHAVIORSTATE_H

#include "astl.h"
#include "transition.h"
#include "nettransition.h"
#include "statedata.h"

class BehaviorState
{
public:
    int n_comps, n_inputs, n_complex_inputs, n_pts, n_isp;

    astl::forward_cursor<astl::DFA_map<Transition,StateData_str> >  *S;
    std::string *E;
    std::string *Complex_E;
    astl::forward_cursor<astl::DFA_map<NetTransition,StateData_str> >  *P;
    astl::forward_cursor<astl::DFA_map<astl::strings,StateData_str> >  *I;

    BehaviorState();
    BehaviorState(int n, int m, int m2, int k, int i);
};

#endif // BEHAVIORSTATE_H
