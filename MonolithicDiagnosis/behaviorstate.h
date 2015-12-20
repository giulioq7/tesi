#ifndef BEHAVIORSTATE_H
#define BEHAVIORSTATE_H

#include "astl.h"
#include "transition.h"
#include "nettransition.h"
#include "statedata.h"
#include "systransition.h"

class BehaviorState
{
    int number;
    bool marked;
public:
    int n_comps, n_inputs, n_pts, n_isp;

    vector<int>  S;
    vector<std::string> E;
    vector<int>  P;
    vector<int>  I;

    std::set<std::set<std::string> > candidate_diagnosis;

    BehaviorState();
    BehaviorState(int n, int m, int k, int i);
    ~BehaviorState();

    BehaviorState copy();
    void set_E(vector<Terminal*> terms);
    bool empty_terminals();
    bool is_marked() const;
    int get_number() const{return number;}
    void mark_state() { marked = true; }
    void set_number(int n) { number = n; }
};

std::ostream& operator<<(std::ostream& out, const BehaviorState& s);

#endif // BEHAVIORSTATE_H
