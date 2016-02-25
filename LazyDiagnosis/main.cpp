#include <iostream>
#include <boost/functional/hash.hpp>
#include <sys/time.h>
#include <stdlib.h>
#include "ostream_util.h"
#include "astl.h"
#include "behaviorstate.h"
#include "decoration.h"
#include "determinization.h"
#include "interfacestate.h"
#include "interfacetrans.h"
#include "sys/resource.h"
#include <unistd.h>

#define GRAPHS_DIR "../Graphs/LazyDiagnosis/"

#define EMPTY_LINK_FINAL true

using namespace std;
using namespace astl;

void build_behavior(DFA_map<InterfaceTrans, BehaviorState> &behavior, DFA_map<InterfaceTrans, BehaviorState> &bhv, Problem &problem, System &system, int index_node,vector<DFA_map<InterfaceTrans,InterfaceState>*> &interfaces, vector<int> &dependency);
void to_nfa(DFA_map<InterfaceTrans, BehaviorState> &dfa, NFA_mmap<InterfaceTrans, BehaviorState> &nfa);

int main()
{
    struct rusage r_usage;

    vector<ComponentModel> comp_models;
    vector<NetworkModel> net_models;
    System system;
    Problem problem;
    {
        // create and open an archive for input
        std::ifstream ifs("../CompiledData/component_models.dat");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> comp_models;
        // archive and stream closed when destructors are called
    }
    {
       // create and open an archive for input
       std::ifstream ifs("../CompiledData/network_models.dat");
       boost::archive::text_iarchive ia(ifs);
       // read class state from archive
       ia >> net_models;
       // archive and stream closed when destructors are called
    }
    {
       // create and open an archive for input
       std::ifstream ifs("../CompiledData/system.dat");
       boost::archive::text_iarchive ia(ifs);
       // read class state from archive
       ia >> system;
       // archive and stream closed when destructors are called
    }
    {
       // create and open an archive for input
       std::ifstream ifs("../CompiledData/problem.dat");
       boost::archive::text_iarchive ia(ifs);
       // read class state from archive
       ia >> problem;
       // archive and stream closed when destructors are called
    }

    cout << "Nodes: " << problem.nodes.size() << ", Components: " << problem.concrete_components_count() << endl;

    timeval start, stop;
    double elapsedTime;
    gettimeofday(&start, NULL);

    vector<DFA_map<InterfaceTrans,InterfaceState>*> interfaces(problem.nodes.size());
    for(int i=0; i<interfaces.size(); i++)
        interfaces[i] = new DFA_map<InterfaceTrans,InterfaceState>;
    vector<DFA_map<InterfaceTrans,BehaviorState>*> bhvs(problem.nodes.size());
    for(int i=0; i<bhvs.size(); i++)
        bhvs[i] = new DFA_map<InterfaceTrans,BehaviorState>;
    vector<DFA_map<InterfaceTrans,BehaviorState>*> spurious_bhvs(problem.nodes.size());
    for(int i=0; i<spurious_bhvs.size(); i++)
        spurious_bhvs[i] = new DFA_map<InterfaceTrans,BehaviorState>;
    int i=0;

    for(vector<int>::iterator it = problem.topological_order.begin(); it != problem.topological_order.end(); it++)
    {
        vector<int> dependency = problem.nodes[*it].depends;
        build_behavior(*spurious_bhvs[*it], *bhvs[*it], problem, system, *it, interfaces, dependency);

         if(problem.nodes[*it].radix)
         {
             if(bhvs[*it]->state_count() == 0)
                 break;
             bhvs[*it]->tag(bhvs[*it]->initial()).candidate_diagnosis.insert(set<string>());
             Decoration::decorate_lazy_bhv(*bhvs[*it],bhvs[*it]->initial(),bhvs[*it]->tag(bhvs[*it]->initial()).candidate_diagnosis, problem.nodes[*it].ruler, interfaces, dependency);

             if(bhvs[*it]->final(bhvs[*it]->initial()))
                 bhvs[*it]->tag(bhvs[*it]->initial()).candidate_diagnosis = Decoration::join_op(bhvs[*it]->tag(bhvs[*it]->initial()).candidate_diagnosis, bhvs[*it]->tag(bhvs[*it]->initial()).interface_delta);


             set<set<string> > diagnosis;
             DFA_map<InterfaceTrans,BehaviorState>::const_iterator c;
             for(c = bhvs[*it]->begin(); c != bhvs[*it]->end(); c++)
             {
                 //cout << "State " << *c << " : " << bhv.tag(*c).candidate_diagnosis << endl;
                 if(bhvs[*it]->final(*c))
                 {
                     std::set<std::set<std::string> >::iterator it2;
                     for(it2 = bhvs[*it]->tag(*c).candidate_diagnosis.begin(); it2 != bhvs[*it]->tag(*c).candidate_diagnosis.end(); it2++)
                        diagnosis.insert(*it2);
                 }
             }
             cout << "Diagnosis: " << diagnosis << endl;

             continue;
         }

         NFA_mmap<InterfaceTrans,BehaviorState> nfa;
         to_nfa(*bhvs[*it],nfa);
         Determinization::NFAtoDFA(*bhvs[*it],nfa,problem.nodes[*it].ruler,*interfaces[*it]);

        i++;
    }

    //qui si riprende il tempo finale per fare la differenze
     gettimeofday(&stop, NULL);

     //calcolo delle differenze
     elapsedTime = (stop.tv_sec - start.tv_sec) * 1000.0;               // sec to ms
     elapsedTime += (stop.tv_usec - start.tv_usec) / 1000.0;            // us to ms

     //stampa
     string udm = "ms";
     if(elapsedTime > 60000)
     {
         elapsedTime /= 60000;
         udm = "min";
     }
     if(elapsedTime > 100)
     {
         elapsedTime /= 1000;
         udm = "sec";
     }
     cout << elapsedTime << udm << endl;

     //print memory usage
     getrusage(RUSAGE_SELF,&r_usage);
     cout << "Memory usage = " << r_usage.ru_maxrss/1000.0 << " MB" << endl;

     chdir(GRAPHS_DIR);

     for(unsigned int i=0; i<bhvs.size(); i++)
     {
         string name = "behavior_";
         stringstream ss;
         ss << i;
         name.append(ss.str());
         ofstream file;
         file.open(name.c_str());
         //Utils::my_dot(file,*bhvs[i]);
         full_dot(file, dfirst_markc(*bhvs[i]));

         delete bhvs[i];
     }


     for(unsigned int i=0; i<spurious_bhvs.size(); i++)
     {
         string name = "behavior_spurious_";
         stringstream ss;
         ss << i;
         name.append(ss.str());
         ofstream file;
         file.open(name.c_str());
         //Utils::my_dot(file,*spurious_bhvs[i]);
         full_dot(file, dfirst_markc(*spurious_bhvs[i]));

         delete spurious_bhvs[i];
     }


     for(unsigned int i=0; i<interfaces.size(); i++)
     {
         string name = "interface_";
         stringstream ss;
         ss << i;
         name.append(ss.str());
         ofstream file;
         file.open(name.c_str());
         //Utils::my_dot(file, *interfaces[i]);
         full_dot(file, dfirst_markc(*interfaces[i]));

         delete interfaces[i];
     }

    return 0;
}



void build_behavior(DFA_map<InterfaceTrans, BehaviorState> &behavior, DFA_map<InterfaceTrans, BehaviorState> &bhv, Problem &problem, System &system, int index_node,vector<DFA_map<InterfaceTrans,InterfaceState>*> &interfaces, vector<int> &dependency)
{
    ProblemNode prob_node = problem.nodes[index_node];
    SystemNode sys_node = system.node_list[index_node];

    vector< forward_cursor<DFA_map<SysTransition,StateData_str> > > fc_S;
    vector< forward_cursor<DFA_map<NetTransition,StateData_strList> > > fc_P;
    vector<unsigned int> initial_P;
    forward_cursor<DFA_map<strings,StateData_str> >  fc_I;
    vector< forward_cursor<DFA_map<InterfaceTrans,InterfaceState> > > fc_interfaces;

    BehaviorState tag_s0(prob_node.concrete_components.size(), prob_node.input_terminals_count(),sys_node.net_model->pattern_space.size(), dependency.size());

    int index_comp = 0;
    for(vector<Component>::iterator it = prob_node.concrete_components.begin(); it != prob_node.concrete_components.end(); it++)
    {
        forward_cursor<DFA_map<SysTransition,StateData_str> > fc(*it->automaton,it->automaton->initial());
        fc_S.push_back(fc);
        tag_s0.S[index_comp] = fc.src();
        index_comp++;
    }

    int index_patt = 0;
    for(vector<astl::DFA_map<NetTransition,StateData_strList> *>::iterator it = sys_node.net_model->pattern_space.begin(); it != sys_node.net_model->pattern_space.end(); it++)
    {
        forward_cursor<DFA_map<NetTransition,StateData_strList> > fc(**it,(*it)->initial());
        fc_P.push_back(fc);
        initial_P.push_back(fc.src());
        tag_s0.P[index_patt] = fc.src();
        index_patt++;
    }

    fc_I = forward_cursor<DFA_map<strings,StateData_str> >(*prob_node.index_space,prob_node.index_space->initial());
    tag_s0.I = fc_I.src();

    int interface_index = 0;
    for(vector<int>::iterator it = dependency.begin(); it != dependency.end(); it++)
    {
        forward_cursor<DFA_map<InterfaceTrans,InterfaceState> > fc(*interfaces[*it],interfaces[*it]->initial());
        fc_interfaces.push_back(fc);
        tag_s0.interfaces[interface_index] = fc.src();
        interface_index++;
    }


    map<string,DFA_map<InterfaceTrans,BehaviorState>::state_type> hash_values;
    stringstream ss;
    ss << tag_s0;
    DFA_map<InterfaceTrans,BehaviorState>::state_type s0 = behavior.new_state();
    behavior.tag(s0) = tag_s0;
    //behavior.tag(s0).set_number(s0);
    behavior.initial(s0);
    hash_values[ss.str()] = s0;

    bool is_final = true;
    if(!prob_node.index_space->final(tag_s0.I))
        is_final = false;
    if(is_final)
    {
        set<set<string> > diagn;
        int i = 0;
        for(vector<int>::iterator it = dependency.begin(); it != dependency.end(); it++)
        {
            if(!interfaces[*it]->final(tag_s0.interfaces[i]))
            {
                is_final = false;
                break;
            }
            set<set<string> > current = interfaces[*it]->tag(tag_s0.interfaces[i]).get_delta();
            set_union(diagn.begin(),diagn.end(),current.begin(),current.end(),std::inserter(diagn,diagn.end()));
            i++;
        }
        if(EMPTY_LINK_FINAL)
        {
            if(is_final)
                 is_final = tag_s0.empty_terminals();
        }
        if(is_final)
        {
            behavior.tag(s0).interface_delta = diagn;
        }
        behavior.final(s0) = is_final;
    }

    DFA_map<InterfaceTrans,BehaviorState>::const_iterator it_s;
    for(it_s = behavior.begin(); it_s != behavior.end(); it_s++)
    {
        s0 = *it_s;
        //if(behavior.tag(s0).is_marked())
            //continue;
        index_comp = 0;

        for(int i=0; i<fc_P.size(); i++)
           fc_P[i] = behavior.tag(s0).P[i];
        fc_I = behavior.tag(s0).I;
        for(int i=0; i<fc_interfaces.size(); i++)
           fc_interfaces[i] = behavior.tag(s0).interfaces[i];

        for(vector<Component>::iterator it = prob_node.concrete_components.begin(); it != prob_node.concrete_components.end(); it++)
        {
            fc_S[index_comp] = behavior.tag(s0).S[index_comp];

            if(fc_S[index_comp].first())
            {
                do
                {
                    InterfaceTrans t(fc_S[index_comp].letter(),set<set<string> >());
                    if(t.trans.is_triggerable(behavior.tag(s0).E, true))
                    {
                        std::string label;
                        //transition is observable
                        if(prob_node.viewer.find(t.trans.t_name_c_name) != prob_node.viewer.end())
                            label = prob_node.viewer[t.trans.t_name_c_name];
                        //transition is not observable
                        else
                            label = "";
                        bool consistent = fc_I.find(label);
                        //label is not consistent with observation
                        if(label != "" && !consistent)
                            continue;

                        BehaviorState tag_s1 = behavior.tag(s0);
                        tag_s1.S[index_comp] = fc_S[index_comp].aim();

                        //label is consistent with observation: updates index space
                        if(label != "" && consistent)
                            tag_s1.I = fc_I.aim();
                        //else: index space is unchanged (transition is not observable)

                        //updates input terminal values after doing component transition
                        t.trans.effects(tag_s1.E, true);

                        NetTransition nt = t.trans.net_trans;

                        for(int i=0; i<fc_P.size(); i++)
                        {
                           if(fc_P[i].find(nt))
                           {
                               tag_s1.P[i] = fc_P[i].aim();
                               if(fc_P[i].aim_final())
                               {
                                    t.pattern_gen = true;
                                    set<string> pe = fc_P[i].aim_tag().elements;
                                    set_union(t.pattern_events.begin(),t.pattern_events.end(),pe.begin(),pe.end(),std::inserter(t.pattern_events,t.pattern_events.end()));
                               }
                           }
                           else //if(system.node_list[index_node].net_model->pattern_languages[i].empty() || Utils::contain(system.node_list[index_node].net_model->pattern_languages[i],nt.name))
                               tag_s1.P[i] = initial_P[i];
                        }
                        stringstream s_str;
                        s_str << tag_s1;
                        string str = s_str.str();
                        DFA_map<InterfaceTrans,BehaviorState>::state_type s1;
                        if(hash_values.find(str) != hash_values.end())
                            s1 = hash_values[str];
                        else
                        {
                           s1 = behavior.new_state();
                           behavior.tag(s1) = tag_s1;
                           //behavior.tag(s1).set_number(s1);
                           hash_values[str] = s1;

                           //checks if the new state is final, so if current state of index spaces is final and all interfaces states are final (and all links are empty)
                           bool is_final = true;

                           if(!prob_node.index_space->final(tag_s1.I))
                               is_final = false;

                           int i=0;
                           set<set<string> > diagn;
                           for(vector<int>::iterator it = dependency.begin(); it != dependency.end(); it++)
                           {
                               if(!interfaces[*it]->final(tag_s1.interfaces[i]))
                               {
                                   is_final = false;
                                   break;
                               }
                               set<set<string> > current = interfaces[*it]->tag(tag_s1.interfaces[i]).get_delta();
                               //set_union(diagn.begin(),diagn.end(),current.begin(),current.end(),std::inserter(diagn,diagn.end()));
                               diagn = Decoration::join_op(diagn,current);
                               i++;
                           }
                           if(EMPTY_LINK_FINAL)
                           {
                               if(is_final)
                                    is_final = tag_s1.empty_terminals();
                           }
                           if(is_final)
                               behavior.tag(s1).interface_delta = diagn;
                           behavior.final(s1) = is_final;
                        }
                        behavior.set_trans(s0,t,s1);
                    }
                }
                while(fc_S[index_comp].next());
            }
            index_comp++;
        }

        for(interface_index=0; interface_index<fc_interfaces.size(); interface_index++)
        {
            if(fc_interfaces[interface_index].first())
            {
                do
                {
                    BehaviorState tag_s1 = behavior.tag(s0);
                    //cout << tag_s1;
                    set<string> patt_events = fc_interfaces[interface_index].letter().pattern_events;
                    //cout << "Pattern event: " << patt_events;
                    if(patt_events.size() > 1)
                        cout << "Multiple patterns!" << endl;
                    set<string>::iterator it_patt;
                    bool empty_t = true;
                    for(it_patt = patt_events.begin(); it_patt != patt_events.end(); it_patt++)
                    {
                        vector<int> list = problem.nodes[dependency[interface_index]].lazy_patt_indexes_map[*it_patt];
                        vector<int>::iterator it_i;
                        for(it_i = list.begin(); it_i != list.end(); it_i++)
                        {
                            if(tag_s1.E[*it_i] != "<eps>")
                            {
                                empty_t = false;
                                break;
                            }
                        }
                        if(empty_t)
                        {
                            tag_s1.interfaces[interface_index] = fc_interfaces[interface_index].aim();
                            for(it_i = list.begin(); it_i != list.end(); it_i++)
                                tag_s1.E[*it_i] = *it_patt;
                        }
                        else
                            break;
                    }
                    if(!empty_t)
                        break;
                    InterfaceTrans t = fc_interfaces[interface_index].letter();
                    stringstream s_str;
                    s_str << tag_s1;
                    string str = s_str.str();
                    DFA_map<InterfaceTrans,BehaviorState>::state_type s1;
                    if(hash_values.find(str) != hash_values.end())
                        s1 = hash_values[str];
                    else
                    {
                       s1 = behavior.new_state();
                       behavior.tag(s1) = tag_s1;
                       //behavior.tag(s1).set_number(s1);
                       hash_values[str] = s1;

                       //checks if the new state is final, so if current state of index spaces is final and all interfaces states are final (and all links are empty)
                       bool is_final = true;

                       if(!prob_node.index_space->final(tag_s1.I))
                           is_final = false;

                       int i=0;
                       set<set<string> > diagn;
                       for(vector<int>::iterator it = dependency.begin(); it != dependency.end(); it++)
                       {
                           if(!interfaces[*it]->final(tag_s1.interfaces[i]))
                           {
                               is_final = false;
                               break;
                           }
                           set<set<string> > current = interfaces[*it]->tag(tag_s1.interfaces[i]).get_delta();
                           //set_union(diagn.begin(),diagn.end(),current.begin(),current.end(),std::inserter(diagn,diagn.end()));
                           diagn = Decoration::join_op(diagn,current);
                           i++;
                       }
                       if(EMPTY_LINK_FINAL)
                       {
                           if(is_final)
                                is_final = tag_s1.empty_terminals();
                       }
                       if(is_final)
                           behavior.tag(s1).interface_delta = diagn;
                       behavior.final(s1) = is_final;
                    }
                    behavior.set_trans(s0,t,s1);
                }
                while(fc_interfaces[interface_index].next());
            }
        }

        //behavior.tag(s0).mark_state();
    }

    DFA_map<InterfaceTrans,BehaviorState>::state_type init;
    //removes all spurious states and transitions that are not in a path from the initial state to a final state
    if(behavior.state_count()>1)
         init = trim(bhv,dfirst_markc(behavior));
    /*else
    {
        init = bhv.new_state();
        bhv.tag(init) = behavior.tag(behavior.initial());
        bhv.final(init) = behavior.final(behavior.initial());
    }*/


    if(init == DFA_map<InterfaceTrans,BehaviorState>::null_state)
    {
        cout << "Empty behavior" << endl;
        //exit(1);
    }

    bhv.initial(init);
}


void to_nfa(DFA_map<InterfaceTrans, BehaviorState> &dfa, NFA_mmap<InterfaceTrans, BehaviorState> &nfa)
{
    map<unsigned int, unsigned int> state_map;
    DFA_map<InterfaceTrans, BehaviorState>::const_iterator c;
    for(c = dfa.begin(); c != dfa.end(); c++)
    {
        NFA_mmap<InterfaceTrans, BehaviorState>::state_type s = nfa.new_state();
        state_map[*c] = s;
        if(dfa.final(*c))
            nfa.final(s) = true;
    }
    set<unsigned int> starts;
    unsigned int s0 = dfa.initial();
    starts.insert(state_map[s0]);
    nfa.initial() = starts;

    typename bfirst_mark_cursor_type<DFA_map<InterfaceTrans, BehaviorState>, DFA_concept, set_marker
            <unsigned int, std::less<unsigned int> > >::model bfc = bfirst_markc(dfa), bfc_end;

    while(bfc != bfc_end)
    {
        do
        {
            InterfaceTrans t;
            if(!bfc.letter().pattern_gen)
                t = InterfaceTrans();
            else
                t = bfc.letter();
            nfa.set_trans(state_map[bfc.src()],t,state_map[bfc.aim()]);
            //cout << state_map[bfc.src()] << " - " << t << " -> " << state_map[bfc.aim()] << endl;
        }while(bfc.next());
    }

}
