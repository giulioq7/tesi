#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <astl.h>
#include <ccopy.h>
#include <dot.h>

#include <statedata.h>

using namespace std;

/**
 * @brief The Utils class contains utility methods for generic tasks
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class Utils
{
public:
    template <typename T>
    /**
     * @brief merge
     * @param v1
     * @param v2
     * @return merged vector
     */
    static std::vector<T> merge(std::vector<T> &v1, std::vector<T> &v2);

    template <typename T>
    /**
    /** * @brief contain membership operator
    /** * @param v
    /** * @param elem
    /** * @return true if elem belongs to v, false otherwise
    /** */
    static bool contain(vector<T> &v, T &elem);
    template <typename T>
    /**
    /** * @brief contain membership operator for set containers
    /** * @param v
    /** * @param elem
    /** * @return
    /** */
    static bool contain(set<T> &v, T &elem);
    template <typename T>
    /**
    /** * @brief duplicate_content checks if a vector contains two equal elements
    /** * @param v
    /** * @return
    /** */
    static bool duplicate_content(vector<T> &v);
    template <typename T>
    /**
    /** * @brief find_from_id
    /** * @param v
    /** * @param id
    /** * @return
    /** */static T* find_from_id(vector<T> &v,std::string id);
    template <typename T>
    /**
    /** * @brief findptr_from_id
    /** * @param v
    /** * @param id
    /** * @return
    /** */
    static T findptr_from_id(vector<T> &v,std::string id);
    template <typename SIGMA, typename TAG>
    /**
    /** * @brief cyclic_graph
    /** * @param dfa
    /** * @return true if graph is cyclic, false otherwise
    /** */
    static bool cyclic_graph(astl::DFA_map<SIGMA,TAG> &dfa);
    template <typename SIGMA, typename TAG>
    /**
    /** * @brief topological_sort
    /** * @param dfa
    /** * @return return a total topological sort of a graph (automaton)
    /** */
    static std::vector<unsigned int> topological_sort(astl::DFA_map<SIGMA,TAG> &dfa);
    template <typename SIGMA, typename TAG>
    /**
    /** * @brief disconnected_graph checks if graph is disconnected
    /** * @param dfa
    /** * @param radixes
    /** * @return
    /** */
    static bool disconnected_graph(astl::DFA_map<SIGMA,TAG> &dfa, set<unsigned int> &radixes);
    template <typename SIGMA, typename TAG>
    /**
    /** * @brief minimize_by_partition Hopcroft's Algorithm
    /** * @param dfa
    /** */
    static void minimize_by_partition(astl::DFA_map<SIGMA,TAG> &dfa);

    template <typename SIGMA, typename TAG>
    /**
    /** * @brief my_dot generates dot file from automaton
    /** * @param ostr
    /** * @param dfa
    /** */
    static void my_dot(ostream &ostr, astl::DFA_map<SIGMA,TAG> &dfa);
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
        if((*it).get_name() == id)
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
        if((*it)->get_name() == id)
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

//Call this only on acyclic graphs!
template <typename SIGMA, typename TAG>
vector<unsigned int> Utils::topological_sort(astl::DFA_map<SIGMA,TAG>& dfa)
{
    using namespace astl;

    vector<unsigned int> sorted;

    DFA_map<SIGMA,TAG> graph;
    typename DFA_map<SIGMA,TAG>::const_iterator it_c;
    for(it_c = dfa.begin(); it_c != dfa.end(); it_c++)
        graph.new_state();
    graph.initial(dfa.initial());
    for(it_c = dfa.begin(); it_c != dfa.end(); it_c++)
    {
        forward_cursor<DFA_map<SIGMA,TAG> > fc(dfa,*it_c);
        if(fc.first())
        {
            do
            {
                graph.set_trans(fc.src(),fc.letter(),fc.aim());
            }
            while(fc.next());
        }
    }

    while(graph.begin() != graph.end())
    {
        typename DFA_map<SIGMA,TAG>::const_iterator c;

        for(c = graph.begin(); c != graph.end(); c++)
        {
            typename DFA_map<SIGMA,TAG>::edges_type aims = graph.delta2(*c);
            if(aims.empty())
            {
                sorted.push_back(*c);
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

            }
        }
    }

    return sorted;
}


template <typename SIGMA, typename TAG>
bool Utils::disconnected_graph(astl::DFA_map<SIGMA,TAG> &dfa, set<unsigned int> &radixes)
{
    using namespace astl;

    if(dfa.state_count() == 1)
        return false;

    vector<unsigned int> visited;

    if(radixes.empty())
        radixes.insert(0);

    for(set<unsigned int>::iterator it = radixes.begin(); it != radixes.end(); it++)
    {
        dfa.initial(*it);
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
    }
    dfa.initial(DFA_map<SIGMA,TAG>::null_state);

    //cout << "visited: " << visited.size();
    //cout << "total: " << dfa.state_count();
    if(visited.size() < dfa.state_count())
        return true;

    return false;
}


template <typename SIGMA, typename TAG>
void Utils::my_dot(ostream &ostr, astl::DFA_map<SIGMA,TAG> &dfa)
{
    using namespace astl;

    ostr << "digraph G {" << endl;

    typename DFA_map<SIGMA,TAG>::const_iterator c;
    for(c = dfa.begin(); c != dfa.end(); c++)
    {
        string border = "circle";
        if(dfa.final(*c))
            border = "doublecircle";
        ostr << *c << "[shape=" << border << ",label=\"" << *c << "\n" << dfa.tag(*c) << "\"];" << endl;
        if(dfa.initial() == *c)
        {
            ostr << "node [shape = point]; start;" << endl;
            ostr << "start -> " << *c << ";" << endl;
        }
        forward_cursor<DFA_map<SIGMA,TAG> > fc(dfa,*c);
        if(fc.first())
        {
            do
            {
                ostr << *c << " -> " << fc.aim() << " [label=\"" << fc.letter() << "\"];\n";
            }
            while(fc.next());
        }
    }

    ostr << "}";
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


    for(typename DFA_map<SIGMA,TAG>::const_iterator c = dfa.begin(); c != dfa.end(); c++)
    {
        forward_cursor<DFA_map<SIGMA,TAG> > fc(dfa,*c);
        if(fc.first())
        {
            do
            {
                //cout << fc.src() << fc.letter() << fc.aim() << endl;
                bool done = false;
                vector<set<unsigned int> >::iterator it;
                for(it = parts.begin(); it != parts.end(); it++)
                {
                    set<unsigned int>::iterator it2 = it->begin();
                    it2++;
                    for(; it2 != it->end(); it2++)
                    {
                        if(*it2 == fc.src())
                        {
                            dfa.del_trans(*it2,fc.letter());
                            done = true;
                            break;
                        }
                        else if(*it2 == fc.aim())
                        {
                            dfa.change_trans(fc.src(),fc.letter(),*it->begin());
                            done = true;
                            break;
                        }
                    }
                    if(done)
                        break;

                }

            }while(fc.next());
        }
    }

    /*vector<set<unsigned int> >::iterator it;
    for(it = parts.begin(); it != parts.end(); it++)
    {
        set<unsigned int>::iterator it2 = it->begin();
        it2++;
        for(; it2 != it->end(); it2++)
        {
            dfa.del_state(*it2);
        }
    }*/


}

#endif // UTILS_H
