#include <iostream>
#include <boost/functional/hash.hpp>
#include <sys/time.h>
#include <stdlib.h>
#include "ostream_util.h"
#include "astl.h"
#include "systransition.h"
#include "behaviorstate.h"
#include <unordered_map>
#include <sys/resource.h>

using namespace std;
using namespace astl;

void decorate(DFA_map<SysTransition,BehaviorState> &dfa, DFA_map<SysTransition,BehaviorState>::state_type beta, set<set<string> > diagnosis, Problem &problem);

int main()
{
    struct rusage r_usage;

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


     timeval start, stop;
     double elapsedTime;

     gettimeofday(&start, NULL);

     vector< forward_cursor<DFA_map<SysTransition,StateData_str> > > fc_S;
     vector< forward_cursor<DFA_map<NetTransition,StateData_strList> > > fc_P;
     vector< forward_cursor<DFA_map<strings,StateData_str> > > fc_I;
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
                 tag_s0.E[index_term] = (*it3)->value;
                 index_term++;
             }
             forward_cursor<DFA_map<SysTransition,StateData_str> > fc(*it2->automaton,it2->automaton->initial());
             fc_S.push_back(fc);
             tag_s0.S[index_comp] = fc.src();
             index_comp++;
         }
     }
     int pattern_indexes[system.node_list.size()][2];
     int index_node = 0;
     int index_patt = 0;
     for(vector<SystemNode>::iterator it = system.node_list.begin(); it != system.node_list.end(); it++)
     {
         if(it->net_model->pattern_space.empty())
             pattern_indexes[index_node][0] = pattern_indexes[index_node][1] = -1;
         else
            pattern_indexes[index_node][0] = index_patt;
         for(vector<astl::DFA_map<NetTransition,StateData_strList> *>::iterator it2 = it->net_model->pattern_space.begin(); it2 != it->net_model->pattern_space.end(); it2++)
         {
             forward_cursor<DFA_map<NetTransition,StateData_strList> > fc(**it2,(*it2)->initial());
             fc_P.push_back(fc);
             tag_s0.P[index_patt] = fc.src();
             index_patt++;
         }
         if(!it->net_model->pattern_space.empty())
             pattern_indexes[index_node][1] = index_patt;
         index_node++;
     }
     index_node = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         forward_cursor<DFA_map<strings,StateData_str> > fc(*it->index_space,it->index_space->initial());
         fc_I.push_back(fc);
         tag_s0.I[index_node] = fc.src();
         index_node++;
     }


     unordered_map<string,DFA_map<SysTransition,BehaviorState>::state_type> hash_values;
     stringstream ss;
     ss << tag_s0;
     DFA_map<SysTransition,BehaviorState>::state_type s0 = behavior.new_state();
     behavior.tag(s0) = tag_s0;
     //behavior.tag(s0).set_number(s0);
     behavior.initial(s0);
     hash_values[ss.str()] = s0;

     cout << "Starting cycle..." << endl;
     DFA_map<SysTransition,BehaviorState>::const_iterator it_s;
     for(it_s = behavior.begin(); it_s != behavior.end(); it_s++)
     {
         s0 = *it_s;
         //if(behavior.tag(s0).is_marked())
             //continue;
         index_comp = 0;
         index_node = 0;

         for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
         {
             for(int i=pattern_indexes[index_node][0];i<pattern_indexes[index_node][1];i++)
                fc_P[i] = behavior.tag(s0).P[i];
             fc_I[index_node] = behavior.tag(s0).I[index_node];

             for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
             {
                 fc_S[index_comp] = behavior.tag(s0).S[index_comp];

                 if(fc_S[index_comp].first())
                 {
                     do
                     {
                         SysTransition t = fc_S[index_comp].letter();
                         if(t.is_triggerable(behavior.tag(s0).E))
                         {
                             std::string label;
                             //transition is observable
                             try
                             {label = it->viewer.at(t.t_name_c_name);}
                             //transition is not observable
                             catch (const std::out_of_range&)
                             {label = "";}
                             bool consistent = fc_I[index_node].find(label);
                             //label is not consistent with observation
                             if(label != "" && !consistent)
                                 continue;

                             BehaviorState tag_s1 = behavior.tag(s0);
                             tag_s1.S[index_comp] = fc_S[index_comp].aim();

                             //label is consistent with observation: updates index space
                             if(label != "" && consistent)
                                 tag_s1.I[index_node] = fc_I[index_node].aim();
                             //else: index space is unchanged (transition is not observable)

                             //updates input terminal values after doing component transition
                             t.effects(tag_s1.E);

                             bool triggerable_pattern = true;

                             NetTransition nt = t.net_trans;

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
                                        if(patt_events.elements.size() > 1)
                                            cout << "Multiple patterns!" << endl;
                                        std::set<std::string>::iterator it_patt;
                                        for(it_patt = patt_events.elements.begin(); it_patt != patt_events.elements.end(); it_patt++)
                                        {
                                            bool empty_t = true;
                                            vector<int> list = it->patt_indexes_map[*it_patt];
                                            vector<int>::iterator it_i;
                                            for(it_i = list.begin(); it_i != list.end(); it_i++)
                                            {
                                                if(tag_s1.E[*it_i] != "<eps>")
                                                {
                                                    empty_t = false;
                                                    triggerable_pattern = false;
                                                    break;
                                                }
                                            }
                                            if(empty_t)
                                            {
                                                for(it_i = list.begin(); it_i != list.end(); it_i++)
                                                    tag_s1.E[*it_i] = *it_patt;
                                            }
                                            else
                                                break;
                                        }
                                        if(!triggerable_pattern)
                                            break;
                                    }
                                }
                             }
                             if(!triggerable_pattern)
                                 continue;
                             stringstream s_str;
                             s_str << tag_s1;
                             string str = s_str.str();
                             DFA_map<SysTransition,BehaviorState>::state_type s1;
                             try{ s1 = hash_values.at(str);}
                             catch (const std::out_of_range&)
                             {
                                s1 = behavior.new_state();
                                behavior.tag(s1) = tag_s1;
                                //behavior.tag(s1).set_number(s1);
                                hash_values[str] = s1;

                                //checks if the new state is final, so if all current states of index spaces are final and all links are empty
                                bool is_final = true;
                                for(unsigned int i = 0; i< problem.nodes.size(); i++)
                                {
                                    if(!problem.nodes[i].index_space->final(tag_s1.I[i]))
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
         //behavior.tag(s0).mark_state();
     }
     cout << "... Cycle terminated!" << endl;


     DFA_map<SysTransition,BehaviorState> bhv;

     //removes all spurious states and transitions that are not in a path from the initial state to a final state
     DFA_map<SysTransition,BehaviorState>::state_type init = trim(bhv,dfirst_markc(behavior));
     cout << "trim terminated" << endl;

     if(init == DFA_map<SysTransition,BehaviorState>::null_state)
     {
         cout << "Empty behavior" << endl;
         exit(1);
     }

     bhv.initial(init);

     //candidate diagnosis of the initial state is the empty set
     bhv.tag(bhv.initial()).candidate_diagnosis.insert(set<string>());

     decorate(bhv,bhv.initial(),  bhv.tag(bhv.initial()).candidate_diagnosis, problem);

     set<set<string> > diagnosis;
     DFA_map<SysTransition,BehaviorState>::const_iterator c;
     for(c = bhv.begin(); c != bhv.end(); c++)
     {
         //cout << "State " << *c << " : " << bhv.tag(*c).candidate_diagnosis << endl;
         if(bhv.final(*c))
         {
             std::set<std::set<std::string> >::iterator it2;
             for(it2 = bhv.tag(*c).candidate_diagnosis.begin(); it2 != bhv.tag(*c).candidate_diagnosis.end(); it2++)
                diagnosis.insert(*it2);
         }
     }

     cout << "Diagnosis: " << diagnosis << endl;

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

     cout << "Number of states(Spurious bhv) : " << behavior.state_count() << endl;
     cout << "Number of transitions(Spurious bhv) : " << behavior.trans_count() << endl;


     ofstream file;
     file.open("behavior_spurious.xdot");
     full_dot(file, dfirst_markc(behavior));
     //std::system("dot -Tjpg behavior_spurious.xdot > behavior_spurious.jpg");

     ofstream file2;
     file2.open("behavior.xdot");
     full_dot(file2, dfirst_markc(bhv));
     //std::system("dot -Tjpg behavior.xdot > behavior.jpg");


     //delete pointers to free memory
     for(vector<ComponentModel>::iterator it = comp_models.begin(); it != comp_models.end(); it++)
         delete it->automaton;
     for(vector<NetworkModel>::iterator it = net_models.begin(); it != net_models.end(); it++)
     {
         for(vector<astl::DFA_map<NetTransition,StateData_strList> *>::iterator it2 = it->pattern_space.begin(); it2 != it->pattern_space.end(); it2++)
            delete *it2;
     }
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         delete it->index_space;
         for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
             delete it2->automaton;
     }


     return 0;
}


void decorate(DFA_map<SysTransition,BehaviorState> &dfa, DFA_map<SysTransition,BehaviorState>::state_type beta, set<set<string> > diagnosis, Problem &problem)
{
    forward_cursor<DFA_map<SysTransition,BehaviorState> > fc(dfa,beta);
    if(fc.first())
    {
        do
        {
            set<set<string> > diagnosis_plus;
            set<set<string> >::iterator it;
            for(it = diagnosis.begin(); it != diagnosis.end(); it++)
            {
                set<string> delta;
                std::pair<string,string> net_t = make_pair(fc.letter().trans->name,fc.letter().component->name);
                ProblemNode *node = Utils::find_from_id(problem.nodes,fc.letter().node->name);
                bool fault = (node->ruler.find(net_t) != node->ruler.end());
                if(fault)
                {
                    std::string fault_label = node->ruler[net_t];
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
                decorate(dfa,fc.aim(),diagnosis_plus,problem);
        }
        while(fc.next());
    }
}
