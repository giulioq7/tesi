#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <astl.h>
#include <ccopy.h>
#include <dot.h>

#include <statedata.h>

using namespace std;

class Utils
{
public:
    template <typename T> static std::vector<T> merge(std::vector<T> &v1, std::vector<T> &v2);
    template <typename T> static bool contain(vector<T> &v, T &elem);
    template <typename T> static bool duplicate_content(vector<T> &v);
    template <typename SIGMA, typename TAG> static bool cyclic_graph(astl::DFA_map<SIGMA,TAG> &dfa);
    template <typename SIGMA, typename TAG> static bool disconnected_graph(astl::DFA_map<SIGMA,TAG> &dfa);
};

template <typename T>
vector<T> Utils::merge(vector<T> &v1, vector<T> &v2)
{
    vector<T> result;
    result = v1;
    for(typename vector<T>::iterator it = v2.begin(); it != v2.end(); it++)
    {
        result.push_back(*it);
    }

    return result;
}

template <typename T>
bool Utils::contain(vector<T> &v, T &elem)
{
    for(typename vector<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        if(*it == elem)
            return true;
    }

    return false;
}



template <typename T>
bool Utils::duplicate_content(vector<T> &v)
{
    for(typename vector<T>::iterator it1 = v.begin(); it1 != v.end(); it1++)
    {
        for(typename vector<T>::iterator it2 = it1 + 1; it2 != v.end(); it2++)
        {
            if(*it1 == *it2)
                return true;
        }
    }
    return false;
}


template <typename SIGMA, typename TAG>
bool Utils::cyclic_graph(astl::DFA_map<SIGMA,TAG>& dfa)
{
    using namespace astl;

    DFA_map<SIGMA,TAG> graph;
    clone(graph,dfirst_markc(dfa));
    graph.initial(dfa.initial());

    while(graph.begin() != graph.end())
    {
        typename DFA_map<SIGMA,TAG>::const_iterator c;
        bool leaf_exists = false;
        for(c = graph.begin(); c != graph.end(); c++)
        {
            typename DFA_map<SIGMA,TAG>::edges_type aims = graph.delta2(*c);
            if(aims.empty())
            {
                graph.del_state(*c);
                typename DFA_map<SIGMA,TAG>::const_iterator c2;
                for(c2 = graph.begin(); c2 != graph.end(); c2++)
                {
                    forward_cursor<DFA_map<SIGMA,TAG> > fc(graph,*c2);
                    if(fc.first())
                    {
                        do
                        {
                            if(fc.aim() == *c)
                                graph.del_trans(fc.src(),fc.letter());
                        }while(fc.next());
                    }
                }
                leaf_exists = true;
            }
        }
        if(!leaf_exists && graph.begin() != graph.end())
            return true;
    }
    return false;
}


template <typename SIGMA, typename TAG>
bool Utils::disconnected_graph(astl::DFA_map<SIGMA,TAG> &dfa)
{
    using namespace astl;

    vector<unsigned int> visited;
    bfirst_cursor<queue_cursor<forward_cursor<DFA_map<SIGMA,TAG> > > > bfc = bfirstc(dfa), bfc_end;
    while(bfc != bfc_end)
    {
        do
        {
            //cout << bfc.src() << bfc.letter() << bfc.aim() << endl;
            unsigned int s1 = bfc.src();
            if(!contain(visited, s1))
                visited.push_back(s1);
            unsigned int s2 = bfc.aim();
            if(!contain(visited,s2))
                visited.push_back(s2);
        }while(bfc.next());
    }

    //cout << "visited: " << visited.size();
    //cout << "total: " << dfa.state_count();
    if(visited.size() < dfa.state_count())
        return true;

   return false;
}

#endif // UTILS_H
