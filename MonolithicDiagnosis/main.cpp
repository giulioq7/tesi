#include <iostream>
#include <boost/functional/hash.hpp>
#include "serialize.h"
#include "ostream_util.h"
#include "astl.h"
#include "systransition.h"
#include "behaviorstate.h"

using namespace std;
using namespace astl;

int main()
{
     vector<ComponentModel> comp_models = vector<ComponentModel>(2);
     vector<NetworkModel> net_models = vector<NetworkModel>(2);
     System system;
     Problem problem;
     problem.nodes = vector<ProblemNode>(7);
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

     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
         it->make_terminals();

     vector<SysTransition> sys_trans;
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
     }
     vector<Component*> all_components;
     vector< forward_cursor<DFA_map<Transition,StateData_str> > > fc_S;
     vector< forward_cursor<DFA_map<NetTransition,StateData_str> > > fc_P;
     vector< forward_cursor<DFA_map<strings,StateData_str> > > fc_I;
     vector<Terminal*> input_terminals;
     DFA_map<SysTransition,BehaviorState> behavior;
     BehaviorState tag_s0(problem.concrete_components_count(),problem.input_terminals_count(),system.emergence.size(), problem.nodes.size(),problem.nodes.size());
     int index_comp = 0;
     int index_term = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
         {
             for(vector<Terminal>::iterator it3 = it2->input_terminals.begin(); it3 != it2->input_terminals.end(); it3++)
             {
                 input_terminals.push_back(&(*it3));
                 tag_s0.E[index_term++] = it3->value;
             }
             forward_cursor<DFA_map<Transition,StateData_str> > fc(it2->automaton,it2->automaton.initial());
             fc_S.push_back(fc);
             tag_s0.S[index_comp] = fc.src();
             index_comp++;
             all_components.push_back(&(*it2));
         }
     }
     index_comp = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         forward_cursor<DFA_map<NetTransition,StateData_str> > fc(it->ref_node->net_model->pattern_space,it->ref_node->net_model->pattern_space.initial());
         fc_P.push_back(fc);
         tag_s0.P[index_comp] = fc.src();
         index_comp++;
     }
     index_comp = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         forward_cursor<DFA_map<strings,StateData_str> > fc(it->index_space,it->index_space.initial());
         fc_I.push_back(fc);
         tag_s0.I[index_comp] = fc.src();
         index_comp++;
     }
     map<size_t,DFA_map<SysTransition,BehaviorState>::state_type> hash_values;
     boost::hash<std::string> string_hash;
     stringstream ss;
     ss << tag_s0;
     size_t h = string_hash(ss.str());
     cout << h;
     DFA_map<SysTransition,BehaviorState>::state_type s0 = behavior.new_state();
     behavior.tag(s0) = tag_s0;
     behavior.initial(s0);
     hash_values[h] = s0;
     index_comp = 0;
     int index_node = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
         {
             if(fc_S[index_comp].first())
             {
                 do
                 {
                     for(unsigned int i=0; i<input_terminals.size(); i++)
                         input_terminals[i]->value = tag_s0.E[i];
                     SysTransition t(it2->model->find_trans(fc_S[index_comp].letter().name),&(*it2),it->ref_node);
                     if(t.is_triggerable())
                     {
                         BehaviorState tag_s1 = tag_s0.copy();
                         tag_s1.S[index_comp] = fc_S[index_comp].aim();
                         std::string label;
                         try
                         {label = it->viewer.at(make_pair(t.trans->name,t.component->name));}
                         catch (const std::out_of_range&)
                         {label = "";}
                         //label is not consistent with observation
                         if(label != "" && !fc_I[index_node].find(label))
                             continue;
                         //label is consistent with observation: updates index space
                         if(label != "" && fc_I[index_node].find(label))
                         {
                             tag_s1.I[index_node] = fc_I[index_node].aim();
                             fc_I[index_node].forward();
                         }
                         //else: index space is unchanged

                         t.effects();
                         tag_s1.set_E(input_terminals);
                         NetTransition nt; nt.trans = t.trans; nt.component = t.component;
                         if(fc_P[index_node].find(nt))
                         {
                             tag_s1.P[index_node] = fc_P[index_node].aim();
                             fc_P[index_node].forward();
                         }
                         stringstream s_str;
                         s_str << tag_s1;
                         size_t h = string_hash(s_str.str());
                         cout << h << endl;
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
                                if(!it_node->index_space.final(it_fcI->src()))
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

     ofstream file;
     file.open("behavior.xdot");
     full_dot(file, dfirst_markc(behavior));

     return 0;
}

