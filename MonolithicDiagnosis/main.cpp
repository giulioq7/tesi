#include <iostream>
#include <boost/functional/hash.hpp>
#include <sys/time.h>
#include "ostream_util.h"
#include "astl.h"
#include "systransition.h"
#include "behaviorstate.h"

using namespace std;
using namespace astl;

int main()
{
    // ... some time later restore the class instance to its orginal state
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

   //constructs partial ordered observation from paper HDES example
   DFA_map<strings,StateData_str> obs;
   DFA_map<strings,StateData_str>::state_type i1 = obs.new_state();
   DFA_map<strings,StateData_str>::state_type i2 = obs.new_state();
   DFA_map<strings,StateData_str>::state_type i3 = obs.new_state();
   DFA_map<strings,StateData_str>::state_type i4 = obs.new_state();
   DFA_map<strings,StateData_str>::state_type i5 = obs.new_state();
   DFA_map<strings,StateData_str>::state_type i6 = obs.new_state();
   obs.tag(i1) = StateData_str("I1");
   obs.tag(i2) = StateData_str("I2");
   obs.tag(i3) = StateData_str("I3");
   obs.tag(i4) = StateData_str("I4");
   obs.tag(i5) = StateData_str("I5");
   obs.tag(i6) = StateData_str("I6");
   obs.initial(i1);
   obs.set_trans(i1,"awk",i2);
   obs.set_trans(i1,"opl",i3);
   obs.set_trans(i2,"opl",i3);
   obs.set_trans(i3,"ide",i4);
   obs.set_trans(i3,"clr",i5);
   obs.set_trans(i4,"clr",i6);
   obs.set_trans(i5,"ide",i6);
   obs.final(i4) = true;
   obs.final(i6) = true;
   //problem.nodes[1].index_space = &obs;

   /*ofstream f;
   f.open("paper_obs.xdot");
   full_dot(f, dfirst_markc(obs));*/

   timeval start, stop;
   double elapsedTime;

   gettimeofday(&start, NULL);

   /*vector<SysTransition> sys_trans;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
         {
            for(vector<Transition>::iterator it3 = it2->model->trans.begin(); it3 != it2->model->trans.end(); it3++)
            {
                SysTransition t(&(*it3), &(*it2), it->ref_node);
                cout << t.trans->name << " is triggerable: " << t.is_triggerable() << endl;
                sys_trans.push_back(t);
            }
         }
     }*/
     vector<Component*> all_components;
     vector< forward_cursor<DFA_map<Transition,StateData_str> > > fc_S;
     vector< forward_cursor<DFA_map<NetTransition,StateData_strList> > > fc_P;
     vector< forward_cursor<DFA_map<strings,StateData_str> > > fc_I;
     vector<Terminal*> input_terminals;
     DFA_map<SysTransition,BehaviorState> behavior;
     BehaviorState tag_s0(problem.concrete_components_count(),problem.input_terminals_count(),system.pts_count(),problem.nodes.size());
     int index_comp = 0;
     int index_term = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
         {
             for(vector<Terminal*>::iterator it3 = it2->input_terminals.begin(); it3 != it2->input_terminals.end(); it3++)
             {
                 input_terminals.push_back(*it3);
                 tag_s0.E[index_term++] = (*it3)->value;
             }
             forward_cursor<DFA_map<Transition,StateData_str> > fc(*it2->automaton,it2->automaton->initial());
             fc_S.push_back(fc);
             tag_s0.S[index_comp] = fc.src();
             index_comp++;
             all_components.push_back(&(*it2));
         }
     }
     int pattern_indexes[system.node_list.size()][2];
     int node_index = 0;
     int patt_index = 0;
     for(vector<SystemNode>::iterator it = system.node_list.begin(); it != system.node_list.end(); it++)
     {
         if(it->net_model->pattern_space.empty())
             pattern_indexes[node_index][0] = pattern_indexes[node_index][1] = -1;
         else
            pattern_indexes[node_index][0] = patt_index;
         for(vector<astl::DFA_map<NetTransition,StateData_strList> *>::iterator it2 = it->net_model->pattern_space.begin(); it2 != it->net_model->pattern_space.end(); it2++)
         {
             forward_cursor<DFA_map<NetTransition,StateData_strList> > fc(**it2,(*it2)->initial());
             fc_P.push_back(fc);
             tag_s0.P[patt_index] = fc.src();
             patt_index++;
         }
         if(!it->net_model->pattern_space.empty())
             pattern_indexes[node_index][1] = patt_index;
         node_index++;
     }
     index_comp = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         forward_cursor<DFA_map<strings,StateData_str> > fc(*it->index_space,it->index_space->initial());
         fc_I.push_back(fc);
         tag_s0.I[index_comp] = fc.src();
         index_comp++;
     }
     map<size_t,DFA_map<SysTransition,BehaviorState>::state_type> hash_values;
     boost::hash<std::string> string_hash;
     stringstream ss;
     ss << tag_s0;
     size_t h = string_hash(ss.str());
     //cout << h;
     DFA_map<SysTransition,BehaviorState>::state_type s0 = behavior.new_state();
     behavior.tag(s0) = tag_s0;
     behavior.initial(s0);
     hash_values[h] = s0;
     behavior.set_trans(s0,SysTransition(),s0);
     typename bfirst_mark_cursor_type<DFA_map<SysTransition,BehaviorState>, DFA_concept, set_marker
             <unsigned int, std::less<unsigned int> > >::model bfc = bfirst_markc(behavior), bfc_end;

     while(bfc != bfc_end)
     {
         do
         {
             s0 = bfc.aim();
             if(behavior.tag(s0).is_marked())
                 continue;
             index_comp = 0;
             int index_node = 0;

             for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
             {

                 for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
                 {
                     for(int i =0; i<problem.concrete_components_count(); i++)
                         fc_S[i] = (DFA_map<SysTransition,BehaviorState>::state_type) behavior.tag(s0).S[i];
                     for(unsigned int i=0; i<input_terminals.size(); i++)
                         input_terminals[i]->value = behavior.tag(s0).E[i];
                     for(int i=0; i<system.pts_count(); i++)
                         fc_P[i] = behavior.tag(s0).P[i];
                     for(unsigned int i =0; i<problem.nodes.size(); i++)
                         fc_I[i] = behavior.tag(s0).I[i];

                     if(fc_S[index_comp].first())
                     {
                         do
                         {
                             for(int i =0; i<problem.concrete_components_count(); i++)
                                 fc_S[i] = (DFA_map<SysTransition,BehaviorState>::state_type) behavior.tag(s0).S[i];
                             for(unsigned int i=0; i<input_terminals.size(); i++)
                                 input_terminals[i]->value = behavior.tag(s0).E[i];
                             for(int i=0; i<system.pts_count(); i++)
                                 fc_P[i] = behavior.tag(s0).P[i];
                             for(unsigned int i =0; i<problem.nodes.size(); i++)
                                 fc_I[i] = behavior.tag(s0).I[i];

                             int val = behavior.tag(s0).S[index_comp];
                             std::string s = fc_S[index_comp].letter().name;
                             Transition *simple_t = it2->model->find_trans(s);
                             SysTransition t(simple_t,&(*it2),it->ref_node);
                             if(t.is_triggerable())
                             {
                                 BehaviorState tag_s1 = behavior.tag(s0).copy();
                                 tag_s1.S[index_comp] = fc_S[index_comp].aim();
                                 std::string label;
                                 //transition is observable
                                 try
                                 {label = it->viewer.at(make_pair(t.trans->name,t.component->name));}
                                 //transition is not observable
                                 catch (const std::out_of_range&)
                                 {label = "";}
                                 bool consistent = fc_I[index_node].find(label);
                                 //label is not consistent with observation
                                 if(label != "" && !consistent)
                                     continue;
                                 //label is consistent with observation: updates index space
                                 if(label != "" && consistent)
                                     tag_s1.I[index_node] = fc_I[index_node].aim();
                                 //else: index space is unchanged (transition is not observable)

                                 t.effects();
                                 tag_s1.set_E(input_terminals);
                                 NetTransition nt(t.trans,t.component);
                                 for(int i=pattern_indexes[index_node][0];i<pattern_indexes[index_node][1];i++)
                                 {
                                    if(fc_P[i].find(nt))
                                    {
                                        tag_s1.P[i] = fc_P[i].aim();
                                        if(fc_P[i].aim_final())
                                        {
                                            //cout << tag_s1;
                                            StateData_strList patt_events = fc_P[i].aim_tag();
                                            //cout << "Pattern event: " << patt_events;
                                            std::set<std::string>::iterator it_patt;
                                            for(it_patt = patt_events.elements.begin(); it_patt != patt_events.elements.end(); it_patt++)
                                            {
                                                Pattern *p = Utils::find_from_id(it->ref_node->net_model->patterns,*it_patt);
                                                //cout << *p;
                                                //retrieves network output terminal relative to pattern
                                                Terminal *out = Utils::findptr_from_id(it->output_terminals,p->get_terminal_id());
                                                //cout << *out << endl;
                                                vector<Terminal*>::iterator it_term;
                                                //searches all network input terminals linked to out terminal
                                                for(it_term = out->linked_terminals.begin(); it_term != out->linked_terminals.end(); it_term++)
                                                {
                                                    vector<Terminal*>::iterator it_term2;
                                                    bool empty_t = true;
                                                    //searches all component input terminals linked to network input terminal
                                                    for(it_term2 = (*it_term)->linked_terminals.begin(); it_term2 != (*it_term)->linked_terminals.end(); it_term2++)
                                                    {
                                                        if((*it_term2)->value != "<eps>")
                                                            empty_t = false;
                                                    }
                                                    if(empty_t)
                                                    {
                                                        for(it_term2 = (*it_term)->linked_terminals.begin(); it_term2 != (*it_term)->linked_terminals.end(); it_term2++)
                                                        {
                                                            (*it_term2)->value = *it_patt;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                 }

                                 tag_s1.set_E(input_terminals);
                                 stringstream s_str;
                                 s_str << tag_s1;
                                 size_t h = string_hash(s_str.str());
                                 //cout << h << endl;
                                 DFA_map<SysTransition,BehaviorState>::state_type s1;
                                 try{ s1 = hash_values.at(h);}
                                 catch (const std::out_of_range&)
                                 {
                                    s1 = behavior.new_state();
                                    behavior.tag(s1) = tag_s1;
                                    hash_values[h] = s1;

                                    //checks if the new state is final, so if all current states of index spaces are final and all links are empty
                                    bool is_final = true;
                                    vector< forward_cursor<DFA_map<strings,StateData_str> > >::iterator it_fcI;
                                    vector<ProblemNode>::iterator it_node;
                                    for(it_fcI = fc_I.begin(), it_node = problem.nodes.begin(); it_fcI != fc_I.end(), it_node != problem.nodes.end(); it_fcI++, it_node++)
                                    {
                                        if(!it_node->index_space->final(it_fcI->src()))
                                        {
                                            is_final = false;
                                            break;
                                        }
                                    }
                                    if(is_final)
                                        is_final = tag_s1.empty_terminals();
                                    behavior.final(s1) = is_final;
                                 }
                                 behavior.set_trans(s0,t,s1);
                             }

                         }
                         while(fc_S[index_comp].next());
                     }
                     index_comp++;
                 }
                 index_node++;
             }
             behavior.tag(s0).mark_state();
         }
         while(bfc.next());
     }

     //deletes first fictitious transition needed to start the loop
     behavior.del_trans(behavior.initial(),SysTransition());

     ofstream file;
     file.open("behavior_spurious.xdot");
     full_dot(file, dfirst_markc(behavior));

     //removes all spurious states and transitions that are not in a path from the initial state to a final state
     DFA_map<SysTransition,BehaviorState>::state_type init = trim(behavior,dfirst_markc(behavior));
     behavior.initial(init);

     //qui si riprende il tempo finale per fare la differenze
     gettimeofday(&stop, NULL);

     //calcolo delle differenze
     elapsedTime = (stop.tv_sec - start.tv_sec) * 1000.0;               // sec to ms
     elapsedTime += (stop.tv_usec - start.tv_usec) / 1000.0;            // us to ms

     //stampa
     cout << elapsedTime << " ms.\n";

     ofstream file2;
     file2.open("behavior.xdot");
     full_dot(file2, dfirst_markc(behavior));



     return 0;
}

