#ifndef BEHAVIORSTATE_H
#define BEHAVIORSTATE_H

#include "astl.h"
#include "transition.h"
#include "nettransition.h"
#include "statedata.h"
#include "systransition.h"

class BehaviorState
{
    bool marked;
public:
    int n_comps, n_inputs, n_complex_inputs, n_pts, n_isp;

    int  *S;
    std::string *E;
    std::string *Complex_E;
    int  *P;
    int  *I;

    BehaviorState();
    BehaviorState(int n, int m, int m2, int k, int i);
    ~BehaviorState();

    BehaviorState copy();
    void set_E(vector<Terminal*> terms);
    void set_Complex_E(vector<Terminal*> terms);
    bool empty_terminals();
    void mark_state(){marked = true;}
};

std::ostream& operator<<(std::ostream& out, const BehaviorState& s);

#endif // BEHAVIORSTATE_H
