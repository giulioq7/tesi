#include "behaviorstate.h"

using namespace astl;

BehaviorState::BehaviorState()
{
    n_comps = 0;
    n_inputs = 0;
    n_complex_inputs = 0;
    n_pts = 0;
    n_isp = 0;
    marked = false;
}

BehaviorState::BehaviorState(int n, int m, int m2, int k, int i)
{
    n_comps = n;
    n_inputs = m;
    n_complex_inputs = m2;
    n_pts = k;
    n_isp = i;
    S = new int[n_comps];
    E = new std::string[n_inputs];
    for(int i=0; i<m; i++)
        E[i] = "<eps>";
    Complex_E = new std::string[n_complex_inputs];
    for(int i=0; i<m2; i++)
        Complex_E[i] = "<eps>";
    P = new int[n_pts];
    I = new int[n_isp];
    marked = false;
}

BehaviorState::~BehaviorState()
{
}


BehaviorState BehaviorState::copy()
{
    BehaviorState b(n_comps,n_inputs,n_complex_inputs,n_pts,n_isp);
    for(int i=0; i<n_comps; i++)
        b.S[i] = S[i];
    for(int i=0; i<n_inputs; i++)
        b.E[i] = E[i];
    for(int i=0; i<n_complex_inputs; i++)
        b.Complex_E[i] = Complex_E[i];
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


void BehaviorState::set_Complex_E(vector<Terminal*> terms)
{
    vector<Terminal*>::iterator it;
    int index = 0;
    for(it = terms.begin(); it != terms.end(); it++)
        Complex_E[index++] = (*it)->value;
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
