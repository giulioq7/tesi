#include <iostream>

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
     vector<Terminal*> input_terminals;
     DFA_map<SysTransition,BehaviorState> behavior;
     BehaviorState tag_s0(problem.concrete_components_count(),problem.input_terminals_count(),system.emergence.size(), problem.nodes.size(),problem.nodes.size());
     int index = 0;
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
             int value = fc.src();
             tag_s0.S[index] = value;
             index++;
             all_components.push_back(&(*it2));
         }
     }
     index = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         forward_cursor<DFA_map<NetTransition,StateData_str> > fc(it->ref_node->net_model->pattern_space,it->ref_node->net_model->pattern_space.initial());
         tag_s0.P[index] = fc;
         index++;
     }
     index = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         forward_cursor<DFA_map<strings,StateData_str> > fc(it->index_space,it->index_space.initial());
         tag_s0.I[index] = fc;
         index++;
     }
     DFA_map<SysTransition,BehaviorState>::state_type s0 = behavior.new_state();
     behavior.tag(s0) = tag_s0;
     behavior.initial(s0);
     index = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
         {
             if(fc_S[index].first())
             {
                 do
                 {
                     SysTransition t(it2->model->find_trans(fc_S[index].letter().name),&(*it2),it->ref_node);
                     if(t.is_triggerable())
                     {
                         DFA_map<SysTransition,BehaviorState>::state_type s1 = behavior.new_state();
                         BehaviorState tag_s1 = tag_s0.copy();
                         tag_s1.S[index] = fc_S[index].aim();
                         t.effects();
                         tag_s1.set_E(input_terminals);
                         behavior.tag(s1) = tag_s1;
                         behavior.set_trans(s0,t,s1);

                     }
                     for(unsigned int i=0; i<input_terminals.size(); i++)
                         input_terminals[i]->value = tag_s0.E[i];
                 }
                 while(fc_S[index].next());
             }
             index++;
         }
     }


     ofstream file;
     file.open("prova");
     full_dot(file, dfirst_markc(behavior));

     return 0;
}

