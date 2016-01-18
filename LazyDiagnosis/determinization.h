#ifndef DETERMINIZATION_H
#define DETERMINIZATION_H

#include "astl.h"
#include "nfa_mmap.h"
#include <algorithm>
#include <boost/functional/hash.hpp>
#include "utils.h"
#include "decoration.h"

#define EPS_TRANS ""

class Determinization
{
    template<typename SIGMA, typename TAG>
    static bool is_final(astl::NFA_mmap<SIGMA,TAG> &nfa, set<unsigned int> states);

    template<typename SIGMA_DFA, typename SIGMA, typename TAG>
    static void extract_subNFA(astl::DFA_map<SIGMA_DFA,TAG> &bhv, set<unsigned int> states,  set<unsigned int> starts, astl::NFA_mmap<SIGMA,TAG> &subnfa);
public:
    template<typename SIGMA, typename TAG>
    static set<unsigned int> eps_closure(astl::NFA_mmap<SIGMA,TAG> &nfa, unsigned int state);

    template<typename SIGMA, typename TAG>
    static set<unsigned int> eps_closure(astl::NFA_mmap<SIGMA,TAG> &nfa, set<unsigned int> states);

    template<typename SIGMA, typename SIGMA_DFA, typename TAG, typename TAG_DFA>
    static set<unsigned int> move(NFA_mmap<SIGMA,TAG> &nfa, set<unsigned int> states, DFA_map<SIGMA_DFA,TAG_DFA> &dfa, unsigned int dfa_state, SIGMA_DFA letter);

    template<typename SIGMA, typename SIGMA_DFA, typename TAG, typename TAG_DFA, typename M1, typename M2>
    static void NFAtoDFA(astl::DFA_map<SIGMA,TAG> &bhv, astl::NFA_mmap<SIGMA,TAG> &nfa, map<M1,M2> &ruler, astl::DFA_map<SIGMA_DFA,TAG_DFA> &dfa);
};

using namespace astl;

template<typename SIGMA, typename TAG>
set<unsigned int> Determinization::eps_closure(NFA_mmap<SIGMA,TAG> &nfa, unsigned int state)
{
    set<unsigned int> result;
    result.insert(state);
    forward_cursor<NFA_mmap<SIGMA,TAG> > fc(nfa, state);
    if(fc.first())
    {
        do
        {
            if(fc.letter().trans.name == EPS_TRANS)
            {
                if(fc.aim() == state)
                    continue;
                set<unsigned int> closure = eps_closure(nfa, fc.aim());
                set_union(result.begin(),result.end(),closure.begin(),closure.end(),std::inserter(result,result.end()));
            }
        }
        while(fc.next());
    }

    return result;
}


template<typename SIGMA, typename TAG>
set<unsigned int> Determinization::eps_closure(NFA_mmap<SIGMA,TAG> &nfa, set<unsigned int> states)
{
    set<unsigned int> result;
    for(set<unsigned int>::iterator it = states.begin(); it != states.end(); it++)
    {
        set<unsigned int> closure = eps_closure(nfa,*it);
        set_union(result.begin(),result.end(),closure.begin(),closure.end(),std::inserter(result,result.end()));
    }

    return result;
}


template<typename SIGMA, typename SIGMA_DFA, typename TAG, typename TAG_DFA>
set<unsigned int> Determinization::move(NFA_mmap<SIGMA,TAG> &nfa, set<unsigned int> states, DFA_map<SIGMA_DFA,TAG_DFA> &dfa, unsigned int dfa_state, SIGMA_DFA letter)
{
    set<unsigned int> result;
    set<unsigned int>::iterator it;
    NFA_mmap<SIGMA,TAG> *aut = dfa.tag(dfa_state).get_automaton();
    typename NFA_mmap<SIGMA,TAG>::const_iterator it2;
    for(it = states.begin(), it2 = aut->begin(); it != states.end(), it2 != aut->end(); it++, it2++)
    {
        forward_cursor<NFA_mmap<SIGMA,TAG> > fc(nfa, *it);
        if(fc.first())
        {
            do
            {
                if(fc.letter().trans.name == letter.trans.name && dfa.tag(dfa_state).get_automaton()->tag(*it2).candidate_diagnosis == letter.delta)
                    result.insert(fc.aim());
            }
            while(fc.next());
        }
    }

    return result;
}


template<typename SIGMA, typename SIGMA_DFA, typename TAG, typename TAG_DFA, typename M1, typename M2>
void Determinization::NFAtoDFA(astl::DFA_map<SIGMA,TAG> &bhv, astl::NFA_mmap<SIGMA,TAG> &nfa, map<M1,M2> &ruler, astl::DFA_map<SIGMA_DFA,TAG_DFA> &dfa)
{
    unsigned int s0 = dfa.new_state();
    TAG_DFA t(eps_closure(nfa,nfa.initial()));
    cout<<"S0 eps-clos: " << t;
    NFA_mmap<SIGMA,TAG> autom;
    extract_subNFA(bhv,t.get_states(),nfa.initial(),autom);
    set<unsigned int> initials = autom.initial();
    for(set<unsigned int>::iterator it = initials.begin(); it != initials.end(); it++)
    {
        autom.tag(*it).candidate_diagnosis = set<set<string> >();
        autom.tag(*it).candidate_diagnosis.insert(set<string>());
        Decoration::decorate_lazy_bhv(autom,*it,autom.tag(*it).candidate_diagnosis, ruler);
    }
    t.set_automaton(&autom);
    dfa.tag(s0) = t;
    dfa.initial(s0);
    dfa.final(s0) = is_final(nfa,dfa.tag(s0).get_states());
    if(dfa.final(s0))
    {
        set<set<string> > delta;
        NFA_mmap<SIGMA,TAG> *aut = dfa.tag(s0).get_automaton();
        for(typename NFA_mmap<SIGMA,TAG>::const_iterator it = aut->begin(); it != aut->end(); it++)
        {
            if(aut->final(*it))
                set_union(delta.begin(),delta.end(),aut->tag(*it).candidate_diagnosis.begin(),aut->tag(*it).candidate_diagnosis.end(),std::inserter(delta,delta.end()));
        }
        dfa.tag(s0).set_delta(delta);
    }

    map<string,unsigned int> hash_values;
    stringstream ss;
    ss << t.get_states();
    hash_values[ss.str()] = s0;

    vector<triple<unsigned int, SIGMA_DFA,unsigned int> > done_trans;
    for(typename DFA_map<SIGMA_DFA,TAG_DFA>::const_iterator c = dfa.begin(); c != dfa.end(); c++)
    {
        set<unsigned int> states = dfa.tag(*c).get_states();
        cout << states << endl;
        NFA_mmap<SIGMA,TAG> *aut = dfa.tag(*c).get_automaton();
        set<unsigned int>::iterator it;
        typename NFA_mmap<SIGMA,TAG>::const_iterator it2;
        for(it = states.begin(), it2 = aut->begin(); it != states.end(), it2 != aut->end(); it++, it2++)
        {
            forward_cursor<NFA_mmap<SIGMA,TAG> > fc(nfa,*it);
            if(fc.first())
            {
                do
                {
                    if(fc.letter() != SIGMA())
                    {
                        set<set<string> > delta = dfa.tag(*c).get_automaton()->tag(*it2).candidate_diagnosis, delta2;
                        M1 name = fc.letter().trans.t_name_c_name;
                        bool fault = (ruler.find(name) != ruler.end());
                        if(fault)
                        {
                            M2 fault_label = ruler[name];
                            set<M2> singleton;
                            singleton.insert(fault_label);
                            for(set<set<string> >::iterator it = delta.begin(); it != delta.end(); it++)
                            {
                                set<string> un;
                                set_union(it->begin(),it->end(),singleton.begin(),singleton.end(),std::inserter(un,un.end()));
                                delta2.insert(un);
                            }
                        }
                        else
                            delta2 = delta;
                        cout << delta2 << endl;
                        SIGMA_DFA trans(fc.letter().trans,delta);
                        set<unsigned int> aim_set = move(nfa,states,dfa,*c,trans);
                        cout << "aim set: " << aim_set << endl;
                        trans = SIGMA_DFA(fc.letter().trans, delta2);
                        trans.pattern_events = fc.letter().pattern_events;
                        set<unsigned int> U = eps_closure(nfa,aim_set);
                        TAG_DFA t(U);
                        stringstream s_str;
                        s_str << t.get_states();
                        unsigned int state;
                        if(hash_values.find(s_str.str()) != hash_values.end())
                        {
                            state = hash_values[s_str.str()];
                        }
                        //catch (const std::out_of_range&)
                        else
                        {
                            autom = NFA_mmap<SIGMA,TAG>();
                            extract_subNFA(bhv,t.get_states(),aim_set,autom);
                            set<unsigned int> initials = autom.initial();
                            for(set<unsigned int>::iterator it = initials.begin(); it != initials.end(); it++)
                            {
                                autom.tag(*it).candidate_diagnosis = set<set<string> >();
                                autom.tag(*it).candidate_diagnosis.insert(set<string>());
                                Decoration::decorate_lazy_bhv(autom,*it,autom.tag(*it).candidate_diagnosis, ruler);
                            }
                            t.set_automaton(&autom);
                            state = dfa.new_state();
                            dfa.tag(state) = t;
                            hash_values[s_str.str()] = state;
                            dfa.final(state) = is_final(nfa,dfa.tag(state).get_states());
                            if(dfa.final(state))
                            {
                                set<set<string> > delta;
                                NFA_mmap<SIGMA,TAG> *aut = dfa.tag(state).get_automaton();
                                for(typename NFA_mmap<SIGMA,TAG>::const_iterator it = aut->begin(); it != aut->end(); it++)
                                {
                                    if(aut->final(*it))
                                        set_union(delta.begin(),delta.end(),aut->tag(*it).candidate_diagnosis.begin(),aut->tag(*it).candidate_diagnosis.end(),std::inserter(delta,delta.end()));
                                }
                                dfa.tag(state).set_delta(delta);
                            }
                        }
                        triple<unsigned int, SIGMA_DFA,unsigned int> tri = make_triple((unsigned int)*c,trans,state);
                        if(!Utils::contain(done_trans,tri))
                        {
                            dfa.set_trans(*c, trans, state);
                            done_trans.push_back(tri);
                        }
                    }
                }
                while(fc.next());
            }
        }
    }
}


template<typename SIGMA, typename TAG>
bool Determinization::is_final(astl::NFA_mmap<SIGMA,TAG> &nfa, set<unsigned int> states)
{
    set<unsigned int>::iterator it;
    for(it = states.begin(); it != states.end(); it++)
    {
        if(nfa.final(*it))
            return true;
    }
    return false;
}


template<typename SIGMA_DFA, typename SIGMA, typename TAG>
void Determinization::extract_subNFA(astl::DFA_map<SIGMA_DFA,TAG> &bhv, set<unsigned int> states, set<unsigned int> starts, astl::NFA_mmap<SIGMA,TAG> &subnfa)
{
    map<unsigned int, unsigned int> state_map;
    set<unsigned int> sub_starts;
    for(set<unsigned int>::iterator it = states.begin(); it != states.end(); it++)
    {
        unsigned int s = subnfa.new_state();
        subnfa.tag(s) = TAG();
        unsigned int current = *it;
        state_map[current] = s;
        if(Utils::contain(starts,current))
            sub_starts.insert(s);
        subnfa.final(s) = bhv.final(current);
        subnfa.tag(s).candidate_diagnosis = bhv.tag(current).candidate_diagnosis;
    }
    subnfa.initial() = sub_starts;

    for(set<unsigned int>::iterator it = states.begin(); it != states.end(); it++)
    {
        forward_cursor<DFA_map<SIGMA,TAG> > fc(bhv,*it);
        if(fc.first())
        {
            do
            {
                unsigned int aim = fc.aim();
                if(Utils::contain(states,aim))
                    subnfa.set_trans(state_map[fc.src()],fc.letter(), state_map[fc.aim()]);
                //cout << fc.letter() << endl;
            }
            while(fc.next());
        }
    }
}

#endif // DETERMINIZATION_H
