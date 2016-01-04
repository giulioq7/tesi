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

    template<typename SIGMA, typename SIGMA_DFA, typename TAG>
    static set<unsigned int> move(astl::NFA_mmap<SIGMA,TAG> &nfa, set<unsigned int> states, SIGMA_DFA letter);

    template<typename SIGMA, typename SIGMA_DFA, typename TAG, typename TAG_DFA, typename M1, typename M2>
    static void NFAtoDFA(astl::DFA_map<SIGMA,TAG> &bhv, astl::NFA_mmap<SIGMA,TAG> &nfa, map<M1,M2> &ruler, astl::DFA_map<SIGMA_DFA,TAG_DFA> &dfa);
};

using namespace astl;

template<typename SIGMA, typename TAG>
set<unsigned int> Determinization::eps_closure(NFA_mmap<SIGMA,TAG> &nfa, unsigned int state)
{
    set<unsigned int> result;
    result.insert(state);

    for(set<unsigned int>::iterator it = result.begin(); it != result.end(); it++)
    {
        forward_cursor<NFA_mmap<SIGMA,TAG> > fc(nfa, *it);
        if(fc.first())
        {
            do
            {
                if(fc.letter() == EPS_TRANS)
                    result.insert(fc.aim());
            }
            while(fc.next());
        }
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


template<typename SIGMA, typename SIGMA_DFA, typename TAG>
set<unsigned int> Determinization::move(NFA_mmap<SIGMA,TAG> &nfa, set<unsigned int> states, SIGMA_DFA letter)
{
    set<unsigned int> result;
    for(set<unsigned int>::iterator it = states.begin(); it != states.end(); it++)
    {
        forward_cursor<NFA_mmap<SIGMA,TAG> > fc(nfa, *it);
        if(fc.first())
        {
            do
            {
                if(fc.letter() == letter.name)
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
    astl::NFA_mmap<SIGMA,TAG> autom;
    extract_subNFA(bhv,t.get_states(),nfa.initial(),autom);
    set<unsigned int> initials = autom.initial();
    for(set<unsigned int>::iterator it = initials.begin(); it != initials.end(); it++)
    {
        autom.tag(*it).candidate_diagnosis = set<set<string> >();
        autom.tag(*it).candidate_diagnosis.insert(set<string>());
        Decoration::decorate(autom,*it,autom.tag(*it).candidate_diagnosis, ruler);
    }
    t.set_automaton(&autom);
    dfa.tag(s0) = t;
    dfa.initial(s0);
    dfa.final(s0) = is_final(nfa,dfa.tag(s0).get_states());

    map<size_t,unsigned int> hash_values;
    boost::hash<std::string> string_hash;
    stringstream ss;
    ss << t.get_states();
    size_t h = string_hash(ss.str());
    hash_values[h] = s0;

    for(typename DFA_map<SIGMA_DFA,TAG_DFA>::const_iterator c = dfa.begin(); c != dfa.end(); c++)
    {
        set<typename SIGMA_DFA::char_type> inputs;
        set<unsigned int> states = dfa.tag(*c).get_states();
        cout << states << endl;
        for(set<unsigned int>::iterator it = states.begin(); it != states.end(); it++)
        {
            forward_cursor<NFA_mmap<SIGMA,TAG> > fc(nfa,*it);
            if(fc.first())
            {
                do
                {
                    if(fc.letter() != EPS_TRANS)
                    {

                        inputs.insert(SIGMA_DFA(fc.letter(),set<set<string> >()));
                    }
                }
                while(fc.next());
            }
        }
        cout << inputs << endl;
        for(typename set<typename SIGMA_DFA::char_type>::iterator it = inputs.begin(); it != inputs.end(); it++)
        {
            set<unsigned int> U = eps_closure(nfa, move(nfa, dfa.tag(*c).get_states(), *it));
            TAG_DFA t(eps_closure(nfa, U));
            stringstream s_str;
            s_str << t.get_states();
            size_t h = string_hash(s_str.str());
            unsigned int state;
            try{ state = hash_values.at(h); }
            catch (const std::out_of_range&)
            {
                NFA_mmap<SIGMA,TAG> autom;
                extract_subNFA(bhv,t.get_states(),set<unsigned int>(),autom);
                t.set_automaton(&autom);
                state = dfa.new_state();
                dfa.tag(state) = t;
                hash_values[h] = state;
                dfa.final(state) = is_final(nfa,dfa.tag(state).get_states());
            }
            dfa.set_trans(*c, *it ,state);
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
            sub_starts.insert(current);
        subnfa.final(s) = bhv.final(current);
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
                cout << fc.letter() << endl;
            }
            while(fc.next());
        }
    }

}

#endif // DETERMINIZATION_H
