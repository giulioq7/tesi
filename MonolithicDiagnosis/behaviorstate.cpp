#include "behaviorstate.h"

using namespace astl;

BehaviorState::BehaviorState()
{
    n_comps = 0;
    n_inputs = 0;
    n_pts = 0;
    n_isp = 0;
    marked = false;
}

BehaviorState::BehaviorState(int n, int m, int k, int i)
{
    n_comps = n;
    n_inputs = m;
    n_pts = k;
    n_isp = i;
    S = new int[n_comps];
    E = new std::string[n_inputs];
    for(int i=0; i<m; i++)
        E[i] = "<eps>";
    P = new int[n_pts];
    I = new int[n_isp];
    marked = false;
}

BehaviorState::~BehaviorState()
{
}


bool BehaviorState::is_marked() const
{
    return marked;
}

BehaviorState BehaviorState::copy()
{
    BehaviorState b(n_comps,n_inputs,n_pts,n_isp);
    for(int i=0; i<n_comps; i++)
        b.S[i] = S[i];
    for(int i=0; i<n_inputs; i++)
        b.E[i] = E[i];
    for(int i=0; i<n_pts; i++)
        b.P[i] = P[i];
    for(int i=0; i<n_isp; i++)
        b.I[i] = I[i];

    return b;
}


void BehaviorState::set_E(vector<Terminal*> terms)
{
    vector<Terminal*>::iterator it;
    int index = 0;
    for(it = terms.begin(); it != terms.end(); it++)
        E[index++] = (*it)->value;
}


bool BehaviorState::empty_terminals()
{
    for(int i=0; i<n_inputs; i++)
    {
        if(E[i] != "<eps>")
                return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const BehaviorState& s)
{
    out << "[";
    for(int i=0; i<s.n_comps-1; i++)
        out << s.S[i] << ",";
    out << s.S[s.n_comps-1] << "]" << endl;

    out << "[";
    for(int i=0; i<s.n_inputs-1; i++)
        out << s.E[i] << ",";
    out << s.E[s.n_inputs-1] << "]" << endl;

    out << "[";
    for(int i=0; i<s.n_pts-1; i++)
        out << s.P[i] << ",";
    out << s.P[s.n_pts-1] << "]" << endl;

    out << "[";
    for(int i=0; i<s.n_isp-1; i++)
        out << s.I[i] << ",";
    out << s.I[s.n_isp-1] << "]" << endl;

    if(!s.candidate_diagnosis.empty())
        out << s.candidate_diagnosis;


    return out;
}
