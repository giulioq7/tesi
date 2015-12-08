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
    int n_comps, n_inputs, n_pts, n_isp;

    int  *S;
    std::string *E;
    int  *P;
    int  *I;

    std::set<std::set<std::string> > candidate_diagnosis;

    BehaviorState();
    BehaviorState(int n, int m, int k, int i);
    ~BehaviorState();

    BehaviorState copy();
    void set_E(vector<Terminal*> terms);
    bool empty_terminals();
    bool is_marked() const;
    void mark_state(){marked = true;}
};

std::ostream& operator<<(std::ostream& out, const BehaviorState& s);

#endif // BEHAVIORSTATE_H
