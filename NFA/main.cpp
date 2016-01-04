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
    //bhv.final(s_[5]) = true;
    //bhv.final(s_[6]) = true;
    //bhv.final(s_[7]) = true;
    //bhv.final(s_[8]) = true;
    //bhv.final(s_[9]) = true;
    //bhv.final(s_[10]) = true;
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
    //nfa.final(s[5]) = true;
    //nfa.final(s[6]) = true;
    //nfa.final(s[7]) = true;
    //nfa.final(s[8]) = true;
    //nfa.final(s[9]) = true;
    //nfa.final(s[10]) = true;
    nfa.final(s[11]) = true;

    map<string,string> ruler;
    ruler["b3(l)"] = "nol";
    ruler["b3(r)"] = "nor";
    ruler["b4(l)"] = "ncl";
    ruler["b4(r)"] = "ncr";
    ruler["p3(p)"] = "fop";
    ruler["p4(p)"] = "fcp";

//    DFA_map<strings,SatelliteData> bhv;
//    NFA_mmap<strings,SatelliteData>::state_type s_[6];
//    bhv.new_state(6,s_);
//    bhv.initial(s_[0]);
//    bhv.set_trans(s_[0],"p1(p)",s_[1]);
//    bhv.set_trans(s_[1],"b1(b)",s_[2]);
//    bhv.set_trans(s_[2],"p2(p)",s_[3]);
//    bhv.set_trans(s_[2],"p4(p)",s_[4]);
//    bhv.set_trans(s_[3],"b4(b)",s_[5]);
//    bhv.set_trans(s_[4],"b6(b)",s_[5]);
//    //bhv.final(s_[3]) = true;
//    //bhv.final(s_[4]) = true;
//    bhv.final(s_[5]) = true;

//    NFA_mmap<strings,SatelliteData> nfa;
//    NFA_mmap<strings,SatelliteData>::state_type s[6];
//    nfa.new_state(6,s);
//    set<NFA_mmap<strings,SatelliteData>::state_type> init;
//    init.insert(s[0]);
//    nfa.initial() = init;
//    nfa.set_trans(s_[0],EPS_TRANS,s_[1]);
//    nfa.set_trans(s_[1],"b1(b)",s_[2]);
//    nfa.set_trans(s_[2],EPS_TRANS,s_[3]);
//    nfa.set_trans(s_[2],"p4(p)",s_[4]);
//    nfa.set_trans(s_[3],"b4(b)",s_[5]);
//    nfa.set_trans(s_[4],EPS_TRANS,s_[5]);
//    //nfa.final(s[3]) = true;
//    //nfa.final(s[4]) = true;
//    nfa.final(s[5]) = true;

//    map<string,string> ruler;
//    ruler["b3(b)"] = "nob";
//    ruler["b4(b)"] = "ncb";
//    ruler["p3(p)"] = "fop";
//    ruler["p4(p)"] = "fcp";

    DFA_map<Transition,InterfaceState> dfa;
    Determinization::NFAtoDFA(bhv,nfa,ruler,dfa);


    ofstream f1;
    f1.open("bhv.dot");
    full_dot(f1, dfirst_markc(bhv));

    ofstream f3;
    f3.open("dfa.dot");
    full_dot(f3, dfirst_markc(dfa));


    return 0;
}

