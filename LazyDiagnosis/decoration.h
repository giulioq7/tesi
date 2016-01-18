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
    static void decorate(astl::NFA_mmap<SIGMA,TAG> &nfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler);

    template <typename SIGMA,typename TAG, typename M1, typename M2, typename INTER_SIGMA, typename INTER_TAG>
    static void decorate_lazy_bhv(astl::DFA_map<SIGMA,TAG> &dfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler, vector<astl::DFA_map<INTER_SIGMA,INTER_TAG> *> &interfaces, vector<int> &dependency);

    template <typename SIGMA,typename TAG, typename M1, typename M2>
    static void decorate_lazy_bhv(astl::NFA_mmap<SIGMA,TAG> &nfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler);

    template <typename T>
    static set<set<T> > join_op(set<set<T> > &d1, set<set<T> > &d2);
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
void Decoration::decorate(NFA_mmap<SIGMA,TAG> &nfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler)
{
    forward_cursor<NFA_mmap<SIGMA,TAG> > fc(nfa,beta);
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
                    nfa.tag(fc.aim()).candidate_diagnosis.insert(delta);
                    diagnosis_plus.insert(delta);
                }
            }
            if(!diagnosis_plus.empty())
                //recursive call
                decorate(nfa,fc.aim(),diagnosis_plus,ruler);
        }
        while(fc.next());
    }
}


template <typename SIGMA,typename TAG, typename M1, typename M2, typename INTER_SIGMA, typename INTER_TAG>
void Decoration::decorate_lazy_bhv(astl::DFA_map<SIGMA,TAG> &dfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler, vector<DFA_map<INTER_SIGMA,INTER_TAG> *> &interfaces, vector<int> &dependency)
{
    forward_cursor<DFA_map<SIGMA,TAG> > fc(dfa,beta);
    if(fc.first())
    {
        do
        {
            SIGMA t = fc.letter();
            set<set<string> > diagnosis_plus;
            set<set<string> >::iterator it;
            if(!t.delta.empty())
            {
                set<set<M2> > temp;
                temp = join_op(diagnosis,t.delta);
                /*if(fc.aim_final())
                {
                    vector<int>::iterator i,j;
                    TAG tag = fc.aim_tag();
                    for(i = tag.interfaces.begin(), j = dependency.begin(); i != tag.interfaces.end(), j != dependency.end(); i++, j++)
                    {
                        set<set<string> > d = interfaces[*j]->tag(*i).get_delta();
                        if(!d.empty())
                            temp = join_op(temp, d);
                    }
                }*/

                for(it = temp.begin(); it != temp.end(); it++)
                {
                    set<set<string> > temp2;
                    temp2.insert(*it);
                    if(!includes(fc.aim_tag().candidate_diagnosis.begin(),fc.aim_tag().candidate_diagnosis.end(),temp2.begin(),temp2.end()))
                    {
                        dfa.tag(fc.aim()).candidate_diagnosis.insert(*it);
                        diagnosis_plus.insert(*it);
                    }
                }

            }
            else
            {
                set<set<M2> > temp;
                for(it = diagnosis.begin(); it != diagnosis.end(); it++)
                {
                    set<string> delta;
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
                    temp.insert(delta);
                }

                if(fc.aim_final())
                {
                    vector<int>::iterator i,j;
                    TAG tag = fc.aim_tag();
                    for(i = tag.interfaces.begin(), j = dependency.begin(); i != tag.interfaces.end(), j != dependency.end(); i++, j++)
                    {
                        set<set<string> > d = interfaces[*j]->tag(*i).get_delta();
                        if(!d.empty())
                            temp = join_op(temp, d);
                    }
                }
                for(it = temp.begin(); it != temp.end(); it++)
                {
                    set<set<string> > temp2;
                    temp2.insert(*it);
                    if(!includes(fc.aim_tag().candidate_diagnosis.begin(),fc.aim_tag().candidate_diagnosis.end(),temp2.begin(),temp2.end()))
                    {
                        dfa.tag(fc.aim()).candidate_diagnosis.insert(*it);
                        diagnosis_plus.insert(*it);
                    }
                }
            }

            if(!diagnosis_plus.empty())
                //recursive call
                decorate_lazy_bhv(dfa,fc.aim(),diagnosis_plus,ruler,interfaces,dependency);
        }
        while(fc.next());
    }
}


template <typename SIGMA,typename TAG, typename M1, typename M2>
void Decoration::decorate_lazy_bhv(astl::NFA_mmap<SIGMA,TAG> &nfa, unsigned int beta, set<set<M2> > diagnosis, map<M1,M2> &ruler)
{
    forward_cursor<NFA_mmap<SIGMA,TAG> > fc(nfa,beta);
    if(fc.first())
    {
        do
        {
            SIGMA t = fc.letter();
            set<set<string> > diagnosis_plus;
            set<set<string> >::iterator it;
            if(!t.delta.empty())
            {
                set<set<M2> > temp;
                temp = join_op(diagnosis,t.delta);
                /*if(fc.aim_final())
                {
                    set<set<string> > d =  fc.aim_tag().candidate_diagnosis;
                    temp = join_op(temp,d);
                }*/

                for(it = temp.begin(); it != temp.end(); it++)
                {
                    set<set<string> > temp2;
                    temp2.insert(*it);
                    if(!includes(fc.aim_tag().candidate_diagnosis.begin(),fc.aim_tag().candidate_diagnosis.end(),temp2.begin(),temp2.end()))
                    {
                        nfa.tag(fc.aim()).candidate_diagnosis.insert(*it);
                        diagnosis_plus.insert(*it);
                    }
                }

            }
            else
            {
                for(it = diagnosis.begin(); it != diagnosis.end(); it++)
                {
                    set<string> delta;
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
                        nfa.tag(fc.aim()).candidate_diagnosis.insert(delta);
                        diagnosis_plus.insert(delta);
                    }
                }
            }

            if(!diagnosis_plus.empty())
                //recursive call
                decorate_lazy_bhv(nfa,fc.aim(),diagnosis_plus,ruler);
        }
        while(fc.next());
    }
}


template <typename T>
set<set<T> > Decoration::join_op(set<set<T> > &d1, set<set<T> > &d2)
{
    if(d1.empty())
        return d2;
    if(d2.empty())
        return d1;

    set<set<T> > result;

    typename set<set<T> >::iterator it1,it2;
    for(it1 = d1.begin(); it1 != d1.end(); it1++)
    {
        for(it2 = d2.begin(); it2 != d2.end(); it2++)
        {
            set<T> temp;
            set_union(it1->begin(),it1->end(),it2->begin(),it2->end(),std::inserter(temp,temp.end()));
            result.insert(temp);
        }

    }

    return result;
}

#endif // DECORATION_H
