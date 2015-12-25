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
    template <typename T> static bool contain(set<T> &v, T &elem);
    template <typename T> static bool duplicate_content(vector<T> &v);
    template <typename T> static T* find_from_id(vector<T> &v,std::string id);
    template <typename T> static T findptr_from_id(vector<T> &v,std::string id);
    template <typename SIGMA, typename TAG> static bool cyclic_graph(astl::DFA_map<SIGMA,TAG> &dfa);
    template <typename SIGMA, typename TAG> static bool disconnected_graph(astl::DFA_map<SIGMA,TAG> &dfa);
    template <typename SIGMA, typename TAG> static void minimize_by_partition(astl::DFA_map<SIGMA,TAG> &dfa);
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
bool Utils::contain(set<T> &v, T &elem)
{
    for(typename set<T>::iterator it = v.begin(); it != v.end(); it++)
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


template <typename T>
T* Utils::find_from_id(vector<T> &v,std::string id)
{
    T* ref = NULL;
    for(typename vector<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        if((*it).name == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}

template <typename T>
T Utils::findptr_from_id(vector<T> &v,std::string id)
{
    T ref = NULL;
    for(typename vector<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        if((*it)->name == id)
        {
            ref = *it;
            break;
        }
    }
    return ref;
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
                            {
                                typename DFA_map<SIGMA,TAG>::state_type s = fc.src();
                                typename DFA_map<SIGMA,TAG>::char_type l = fc.letter();
                                if(fc.next())
                                {
                                    graph.del_trans(s,l);
                                    continue;
                                }
                                else
                                {
                                    graph.del_trans(s,l);
                                    break;
                                }
                            }
                            if(fc.next())
                                continue;
                            else
                                break;
                        }while(true);
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

    if(dfa.state_count() == 1)
        return false;

    vector<unsigned int> visited;
    typename bfirst_mark_cursor_type<DFA_map<SIGMA, TAG>, DFA_concept, set_marker
            <unsigned int, std::less<unsigned int> > >::model bfc = bfirst_markc(dfa), bfc_end;

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


template <typename SIGMA, typename TAG>
void Utils::minimize_by_partition(astl::DFA_map<SIGMA,TAG> &dfa)
{
    using namespace astl;

    vector<vector<bool> > partition_table;
    for(unsigned int i=0; i<dfa.state_count()-1;i++)
    {
        vector<bool> v(i+1,false);
        partition_table.push_back(v);
    }
    typename DFA_map<SIGMA,TAG>::const_iterator it_r, it_c;

    map<int,int> map_r, map_c;

    //First step: flag state pair (x,y) for all x final and y not final
    //            flag state pair (x,y) if x and y have different tags
    it_r = dfa.begin(); it_r++;
    for(unsigned int i=0; i<partition_table.size(); i++)
    {
        map_r[*it_r] = i;
        it_c = dfa.begin();
        for(unsigned int j=0; j<partition_table.at(i).size(); j++)
        {
            map_c[*it_c] = j;
            if(dfa.final(*it_r) != dfa.final(*it_c))
                partition_table.at(i).at(j) = true;
            else if(dfa.tag(*it_r) != dfa.tag(*it_c))
                partition_table.at(i).at(j) = true;

            it_c++;
        }
        it_r++;
    }

    it_r = dfa.begin(); it_r++;
    for(unsigned int i=0; i<partition_table.size(); i++)
    {
        it_c = dfa.begin();
        for(unsigned int j=0; j<partition_table.at(i).size(); j++)
        {
            if(partition_table.at(i).at(j) == false)
            {
                typename DFA_map<SIGMA,TAG>::Edges trans1 = dfa.delta2(*it_r);
                typename DFA_map<SIGMA,TAG>::Edges trans2 = dfa.delta2(*it_c);
                //cout << "(" << *it_r << "," << *it_c << ") : ";
                //cout << trans1.size() << " e " << trans2.size() << endl;
                typename DFA_map<SIGMA,TAG>::Edges::const_iterator it_edges;
                bool distinguished = false;
                if(trans1.size() != trans2.size())
                    distinguished = true;
                if(!distinguished)
                {
                    for(it_edges = trans1.begin(); it_edges != trans1.end(); it_edges++)
                    {
                        typename DFA_map<SIGMA,TAG>::state_type aim1, aim2;
                        aim1 = dfa.delta1(*it_r,it_edges->first);
                        aim2 = dfa.delta1(*it_c,it_edges->first);
                        if(aim1 == 0 || aim2 == 0)
                        {
                            distinguished = true;
                            break;
                        }
                        //cout << aim1 << " " << aim2 << " ";
                        if(aim1 != aim2)
                        {
                            unsigned int w = map_r[aim1];
                            unsigned int z = map_c[aim2];
                            if(w >= partition_table.size() || z >= partition_table.at(w).size())
                            {
                                unsigned int temp = w;
                                w = z;
                                z = temp;
                            }
                            //cout << " (w,z) : " << w << " " << z << endl;
                            if(partition_table[w][z] == true)
                            {
                                distinguished = true;
                                break;
                            }
                        }
                    }
                }
                if(!distinguished)
                {
                    for(it_edges = trans2.begin(); it_edges != trans2.end(); it_edges++)
                    {
                        typename DFA_map<SIGMA,TAG>::state_type aim1, aim2;
                        aim1 = dfa.delta1(*it_r,it_edges->first);
                        aim2 = dfa.delta1(*it_c,it_edges->first);
                        if(aim1 == 0 || aim2 == 0)
                        {
                            distinguished = true;
                            break;
                        }
                        //cout << aim1 << " " << aim2 << " ";
                        if(aim1 != aim2)
                        {
                            unsigned int w = map_r[aim1];
                            unsigned int z = map_c[aim2];
                            if(w >= partition_table.size() || z >= partition_table.at(w).size())
                            {
                                unsigned int temp = w;
                                w = z;
                                z = temp;
                            }
                            if(partition_table[w][z] == true)
                            {
                                distinguished = true;
                                break;
                            }
                        }
                    }
                }
                if(distinguished)
                    partition_table.at(i).at(j) = true;

            }
            it_c++;
        }
        it_r++;
    }

    vector<set<unsigned int> > parts;
    it_r = dfa.begin(); it_r++;
    for(unsigned int i=0; i<partition_table.size(); i++)
    {
        it_c = dfa.begin();
        for(unsigned int j=0; j<partition_table.at(i).size(); j++)
        {
            if(partition_table.at(i).at(j) == false)
            {
               bool contain_r = false;
               bool contain_c = false;
               for(vector<set<unsigned int> >::iterator it = parts.begin(); it != parts.end(); it++)
               {
                   unsigned int s1 = *it_r;
                   unsigned int s2 = *it_c;
                   if(contain(*it,s1))
                       contain_r = true;
                   if(contain(*it,s2))
                       contain_c = true;
                   if(contain_r && !contain_c)
                   {
                       it->insert(*it_c);
                       break;
                   }
                   if(!contain_r && contain_c)
                   {
                       it->insert(*it_r);
                       break;
                   }
                   if(contain_r && contain_c)
                       break;
               }
               if(!contain_r && ! contain_c)
               {
                   set<unsigned int> new_set;
                   new_set.insert(*it_r);
                   new_set.insert(*it_c);
                   parts.push_back(new_set);
               }

            }
            it_c++;
        }
        it_r++;
    }

    cout << parts << endl;

    typename bfirst_mark_cursor_type<DFA_map<SIGMA, TAG>, DFA_concept, set_marker
            <unsigned int, std::less<unsigned int> > >::model bfc = bfirst_markc(dfa), bfc_end;

    while(bfc != bfc_end)
    {
        do
        {
            //cout << bfc.src() << bfc.letter() << bfc.aim() << endl;
            bool done = false;
            vector<set<unsigned int> >::iterator it;
            for(it = parts.begin(); it != parts.end(); it++)
            {
                set<unsigned int>::iterator it2 = it->begin();
                it2++;
                for(; it2 != it->end(); it2++)
                {
                    if(*it2 == bfc.src())
                    {
                        dfa.del_trans(*it2,bfc.letter());
                        done = true;
                        break;
                    }
                    else if(*it2 == bfc.aim())
                    {
                        dfa.change_trans(bfc.src(),bfc.letter(),*it->begin());
                        done = true;
                        break;
                    }
                }
                if(done)
                    break;

            }

        }while(bfc.next());
    }

    vector<set<unsigned int> >::iterator it;
    for(it = parts.begin(); it != parts.end(); it++)
    {
        set<unsigned int>::iterator it2 = it->begin();
        it2++;
        for(; it2 != it->end(); it2++)
        {
            if(*it2 != 88 && *it2 != 90)
                dfa.del_state(*it2);
        }
    }

}

#endif // UTILS_H
