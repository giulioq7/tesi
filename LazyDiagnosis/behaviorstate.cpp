#include "behaviorstate.h"

using namespace astl;

BehaviorState::BehaviorState()
{
    n_comps = 0;
    n_inputs = 0;
    n_pts = 0;
    S = vector<int>(0);
    E = vector<std::string>(0);
    P = vector<int>(0);
    interfaces = vector<int>(0);
    I = 0;
    marked = false;
    number = -1;
}

BehaviorState::BehaviorState( int n, int m, int k, int i)
{
    n_comps = n;
    n_inputs = m;
    n_pts = k;
    n_intfcs = i;
    S = vector<int>(n_comps);
    E = vector<std::string>(n_inputs);
    for(int i=0; i<m; i++)
        E[i] = "<eps>";
    P = vector<int>(n_pts);
    interfaces = vector<int>(n_intfcs);
    marked = false;
    number = -1;
}

BehaviorState::~BehaviorState()
{
}


bool BehaviorState::is_marked() const
{
    return marked;
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
    //out << s.get_number() << endl;
    if(s.n_comps == 0)
        out << "[ ]" << endl;
    else
    {
        out << "[";
        for(int i=0; i<s.n_comps-1; i++)
            out << s.S[i] << ",";
        out << s.S[s.n_comps-1] << "]" << endl;
    }

    if(s.n_inputs == 0)
        out << "[ ]" << endl;
    else
    {
        out << "[";
        for(int i=0; i<s.n_inputs-1; i++)
            out << s.E[i] << ",";
        out << s.E[s.n_inputs-1] << "]" << endl;
    }

    if(s.n_pts == 0)
        out << "[ ]" << endl;
    else
    {
        out << "[";
        for(int i=0; i<s.n_pts-1; i++)
            out << s.P[i] << ",";
        out << s.P[s.n_pts-1] << "]" << endl;
    }

    out << s.I << endl;

    if(s.n_intfcs == 0)
        out << "[ ]" << endl;
    else
    {
        out << "[";
        for(int i=0; i<s.n_intfcs-1; i++)
            out << s.interfaces[i] << ",";
        out << s.interfaces[s.n_intfcs-1] << "]" << endl;
    }

    if(!s.candidate_diagnosis.empty())
        out << s.candidate_diagnosis;
    if(!s.interface_delta.empty())
        out << "inter_delta: " << s.interface_delta;

    return out;
}
