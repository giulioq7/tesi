#ifndef DECORATION_H
#define DECORATION_H

#include "astl.h"
#include "nfa_mmap.h"
#include <sstream>


class Decoration
{
public:

    template <typename SIGMA,typename TAG, typename M1, typename M2>
    static void decorate(astl::DFA_map<SIGMA,TAG> &dfa, unsigned int beta, set<set<string> > diagnosis, map<M1,M2> &ruler);

    template <typename SIGMA,typename TAG, typename M1, typename M2>
    static void decorate(astl::NFA_mmap<SIGMA,TAG> &dfa, unsigned int beta, set<set<string> > diagnosis, map<M1,M2> &ruler);
};

using namespace astl;

template <typename SIGMA,typename TAG, typename M1, typename M2>
void Decoration::decorate(DFA_map<SIGMA,TAG> &dfa, unsigned int beta, set<set<string> > diagnosis, map<M1,M2> &ruler)
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
                stringstream ss;
                ss << fc.letter();
                string name = ss.str();
                bool fault = (ruler.find(name) != ruler.end());
                if(fault)
                {
                    std::string fault_label = ruler[name];
                    set<string> singleton;
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


template <typename SIGMA,typename TAG, typename M1, typename M2>
void Decoration::decorate(NFA_mmap<SIGMA,TAG> &dfa, unsigned int beta, set<set<string> > diagnosis, map<M1,M2> &ruler)
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
                stringstream ss;
                ss << fc.letter();
                string name = ss.str();
                bool fault = (ruler.find(name) != ruler.end());
                if(fault)
                {
                    std::string fault_label = ruler[name];
                    set<string> singleton;
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

#endif // DECORATION_H
