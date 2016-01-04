#include <iostream>

#include "astl.h"
#include "nfa_mmap.h"
#include "transition.h"
#include "satellitedata.h"
#include "dot.h"
#include "determinization.h"
#include "interfacestate.h"

#define EPS_TRANS ""

using namespace std;
using namespace astl;

int main()
{
    DFA_map<strings,SatelliteData> bhv;
    NFA_mmap<strings,SatelliteData>::state_type s_[12];
    bhv.new_state(12,s_);
    bhv.initial(s_[0]);
    bhv.set_trans(s_[0],"p1(p)",s_[1]);
    bhv.set_trans(s_[1],"b1(l)",s_[2]);
    bhv.set_trans(s_[1],"b3(r)",s_[3]);
    bhv.set_trans(s_[2],"b3(r)",s_[4]);
    bhv.set_trans(s_[3],"b1(l)",s_[4]);
    bhv.set_trans(s_[4],"p2(p)",s_[5]);
    bhv.set_trans(s_[4],"p4(p)",s_[6]);
    bhv.set_trans(s_[5],"b4(l)",s_[7]);
    bhv.set_trans(s_[5],"b5(r)",s_[8]);
    bhv.set_trans(s_[6],"b3(r)",s_[9]);
    bhv.set_trans(s_[6],"b6(l)",s_[10]);
    bhv.set_trans(s_[7],"b5(r)",s_[11]);
    bhv.set_trans(s_[8],"b4(l)",s_[11]);
    bhv.set_trans(s_[9],"b6(l)",s_[11]);
    bhv.set_trans(s_[10],"b3(r)",s_[11]);
    bhv.final(s_[11]) = true;

    NFA_mmap<strings,SatelliteData> nfa;
    NFA_mmap<strings,SatelliteData>::state_type s[12];
    nfa.new_state(12,s);
    set<NFA_mmap<strings,SatelliteData>::state_type> init;
    init.insert(s[0]);
    nfa.initial() = init;
    nfa.set_trans(s[0],EPS_TRANS,s[1]);
    nfa.set_trans(s[1],"b1(l)",s[2]);
    nfa.set_trans(s[1],EPS_TRANS,s[3]);
    nfa.set_trans(s[2],EPS_TRANS,s[4]);
    nfa.set_trans(s[3],"b1(l)",s[4]);
    nfa.set_trans(s[4],EPS_TRANS,s[5]);
    nfa.set_trans(s[4],"p4(p)",s[6]);
    nfa.set_trans(s[5],"b4(l)",s[7]);
    nfa.set_trans(s[5],EPS_TRANS,s[8]);
    nfa.set_trans(s[6],EPS_TRANS,s[9]);
    nfa.set_trans(s[6],EPS_TRANS,s[10]);
    nfa.set_trans(s[7],EPS_TRANS,s[11]);
    nfa.set_trans(s[8],"b4(l)",s[11]);
    nfa.set_trans(s[9],EPS_TRANS,s[11]);
    nfa.set_trans(s[10],EPS_TRANS,s[11]);
    nfa.final(s[11]) = true;

    set<set<unsigned int> > all_closures;
    for(unsigned int i=0; i<nfa.state_count(); i++)
    {
        set<unsigned int> closure = Determinization::eps_closure(nfa, s[i]);
        all_closures.insert(closure);
    }
    cout << all_closures << endl;

    set<unsigned int> closure;
    closure.insert(s[6]); closure.insert(s[10]); //closure.insert(s[8]);
    cout << Determinization::eps_closure(nfa,closure) << endl;

    map<string,string> ruler;
    ruler["b3(l)"] = "nol";
    ruler["b3(r)"] = "nor";
    ruler["b4(l)"] = "ncl";
    ruler["b4(r)"] = "ncr";
    ruler["p3(p)"] = "fop";
    ruler["p4(p)"] = "fcp";

    DFA_map<Transition,InterfaceState> dfa;
    Determinization::NFAtoDFA(bhv,nfa,ruler,dfa);


    ofstream f;
    f.open("dfa.dot");
    full_dot(f, dfirst_markc(dfa));


    return 0;
}

