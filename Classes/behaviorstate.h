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

    int  *S;
    std::string *E;
    std::string *Complex_E;
    astl::forward_cursor<astl::DFA_map<NetTransition,StateData_str> >  *P;
    astl::forward_cursor<astl::DFA_map<astl::strings,StateData_str> >  *I;
    bool marked;

    BehaviorState();
    BehaviorState(int n, int m, int m2, int k, int i);
    ~BehaviorState();

    BehaviorState copy();
    void set_E(vector<Terminal*> terms);
    void set_Complex_E(vector<Terminal*> terms);
};

#endif // BEHAVIORSTATE_H
