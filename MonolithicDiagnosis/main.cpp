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

     DFA_map<SysTransition,BehaviorState> behavior;
     BehaviorState tag_s0(problem.concrete_components_count(),problem.input_terminals_count(),system.emergence.size(), net_models.size(),problem.nodes.size());
     int index = 0;
     for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); it++)
     {
         for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
         {
             forward_cursor<DFA_map<Transition,StateData_str> > fc(it2->automaton,it2->automaton.initial());
             tag_s0.S[index] = fc;
             index++;
         }
     }
     DFA_map<SysTransition,BehaviorState>::state_type s0 = behavior.new_state();
     behavior.tag(s0) = tag_s0;
     behavior.initial(s0);
     DFA_map<SysTransition,BehaviorState>::state_type s1 = behavior.new_state();
     behavior.tag(s1) = tag_s0;
     SysTransition t;
     t.trans = &comp_models[0].trans[0];
     t.component = &problem.nodes[0].concrete_components[0];
     t.node = &system.node_list[0];
     behavior.set_trans(s0,t,s1);
     ofstream file;
     file.open("prova");
     full_dot(file, dfirst_markc(behavior));


     return 0;
}

