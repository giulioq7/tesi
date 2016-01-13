#ifndef DECORATION_H
#define DECORATION_H

#include "astl.h"
#include "nfa_mmap.h"
#include <sstream>


class Decoration
{
public:

    template <typename SIGMA,typename TAG, typename M1, typename M2>
    static void decorate(astl::DFA_map<SIGMA,TAG> &dfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler);

    template <typename SIGMA,typename TAG, typename M1, typename M2>
    static void decorate(astl::NFA_mmap<SIGMA,TAG> &dfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler);

    template <typename SIGMA,typename TAG, typename M1, typename M2, typename INTER_SIGMA, typename INTER_TAG>
    static void decorate_lazy_bhv(astl::DFA_map<SIGMA,TAG> &dfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler, vector<astl::DFA_map<INTER_SIGMA,INTER_TAG>*> &interfaces);
};

using namespace astl;

template <typename SIGMA,typename TAG, typename M1, typename M2>
void Decoration::decorate(DFA_map<SIGMA,TAG> &dfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler)
{
    forward_cursor<DFA_map<SIGMA,TAG> > fc(dfa,beta);
    if(fc.first())
    {
        do
        {
            set<set<string> > diagnosis_plus;
            set<set<string> >::iterator it;
            for(it = diagnosis.begin(); it != diagnosis.end(); it++)
            {
                set<string> delta;
                SIGMA t = fc.letter();
                M1 name = t.t_name_c_name;
                bool fault = (ruler.find(name) != ruler.end());
                if(fault)
                {
                    M2 fault_label = ruler[name];
                    set<M2> singleton;
                    singleton.insert(fault_label);
                    set_union(it->begin(),it->end(),singleton.begin(),singleton.end(),std::inserter(delta,delta.end()));
                }
                else
                    delta = *it;
                set<set<M2> > temp;
                temp.insert(delta);
                if(!includes(fc.aim_tag().candidate_diagnosis.begin(),fc.aim_tag().candidate_diagnosis.end(),temp.begin(),temp.end()))
                {
                    dfa.tag(fc.aim()).candidate_diagnosis.insert(delta);
                    diagnosis_plus.insert(delta);
                }
            }
            if(!diagnosis_plus.empty())
                //recursive call
                decorate(dfa,fc.aim(),diagnosis_plus,ruler);
        }
        while(fc.next());
    }
}


template <typename SIGMA,typename TAG, typename M1, typename M2>
void Decoration::decorate(NFA_mmap<SIGMA,TAG> &dfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler)
{
    forward_cursor<NFA_mmap<SIGMA,TAG> > fc(dfa,beta);
    if(fc.first())
    {
        do
        {
            set<set<string> > diagnosis_plus;
            set<set<string> >::iterator it;
            for(it = diagnosis.begin(); it != diagnosis.end(); it++)
            {
                set<string> delta;
                M1 name = fc.letter().trans.t_name_c_name;
                bool fault = (ruler.find(name) != ruler.end());
                if(fault)
                {
                    M2 fault_label = ruler[name];
                    set<M2> singleton;
                    singleton.insert(fault_label);
                    set_union(it->begin(),it->end(),singleton.begin(),singleton.end(),std::inserter(delta,delta.end()));
                }
                else
                    delta = *it;
                set<set<string> > temp;
                temp.insert(delta);
                if(!includes(fc.aim_tag().candidate_diagnosis.begin(),fc.aim_tag().candidate_diagnosis.end(),temp.begin(),temp.end()))
                {
                    dfa.tag(fc.aim()).candidate_diagnosis.insert(delta);
                    diagnosis_plus.insert(delta);
                }
            }
            if(!diagnosis_plus.empty())
                //recursive call
                decorate(dfa,fc.aim(),diagnosis_plus,ruler);
        }
        while(fc.next());
    }
}


template <typename SIGMA,typename TAG, typename M1, typename M2, typename INTER_SIGMA, typename INTER_TAG>
void Decoration::decorate_lazy_bhv(astl::DFA_map<SIGMA,TAG> &dfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler, vector<DFA_map<INTER_SIGMA,INTER_TAG>*> &interfaces)
{
    forward_cursor<DFA_map<SIGMA,TAG> > fc(dfa,beta);
    if(fc.first())
    {
        do
        {
            set<set<string> > diagnosis_plus;
            set<set<string> >::iterator it;
            for(it = diagnosis.begin(); it != diagnosis.end(); it++)
            {
                set<string> delta;
                SIGMA t = fc.letter();
                M1 name = t.trans.t_name_c_name;
                bool fault = (ruler.find(name) != ruler.end());
                if(fault)
                {
                    M2 fault_label = ruler[name];
                    set<M2> singleton;
                    singleton.insert(fault_label);
                    set_union(it->begin(),it->end(),singleton.begin(),singleton.end(),std::inserter(delta,delta.end()));
                }
                else
                    delta = *it;
                set<set<M2> > temp;
                temp.insert(delta);
                if(!includes(fc.aim_tag().candidate_diagnosis.begin(),fc.aim_tag().candidate_diagnosis.end(),temp.begin(),temp.end()))
                {
                    dfa.tag(fc.aim()).candidate_diagnosis.insert(delta);
                    diagnosis_plus.insert(delta);
                }
            }
            if(!diagnosis_plus.empty())
                //recursive call
                decorate(dfa,fc.aim(),diagnosis_plus,ruler);
        }
        while(fc.next());
    }
}


#endif // DECORATION_H
