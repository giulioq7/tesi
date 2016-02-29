#include "spec_driver.h"
#include "specparser.hpp"
#include "ostream_util.h"
#include <cursor.h>

extern yy::location loc;

using namespace astl;

spec_driver::spec_driver(): trace_scanning (false), trace_parsing (false)
{
}

int spec_driver::parse (const std::string &f)
{
  file = f;
  scan_begin();
  yy::spec_parser parser(*this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_end ();
  return res;
}

void spec_driver::error (const yy::location& l, const std::string& m)
{
  std::cerr << l << ": " << m << std::endl;
  exit(1);
}

void spec_driver::error (const std::string& m)
{
  std::cerr << m << std::endl;
}

vector<NetComponent> spec_driver::build_components(vector<string> ids, string id_model)
{
    ComponentModel* ref = NULL;
    for (vector<ComponentModel>::iterator it = components.begin(); it != components.end(); ++it)
    {
        if(it->get_name().compare(id_model) == 0)
        {
            ref = &(*it);
            break;
        }
    }
    if(ref == NULL)
        error(loc, "ID type component model not defined.");
    vector<NetComponent> l;
    for (vector<string>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
        NetComponent c(*it);
        c.set_model(ref);
        l.push_back(c);
    }

    return l;
}


NetworkModel* spec_driver::find_netmodel(std::string id_model)
{
    NetworkModel* ref = NULL;
    for (vector<NetworkModel>::iterator it = networks.begin(); it != networks.end(); ++it)
    {
        if(it->get_name() == id_model)
        {
            ref = &(*it);
            break;
        }
    }

    if(ref == NULL)
        error(loc, "ID type network model not defined.");

    return ref;
}


 SystemNode* spec_driver::find_node(std::string id)
 {
     SystemNode* ref = NULL;
     for (vector<SystemNode>::iterator it = system.node_list.begin(); it != system.node_list.end(); ++it)
     {
         if(it->get_name() == id)
         {
             ref = &(*it);
             break;
         }
     }

     if(ref == NULL)
         error(loc, "ID type node not defined.");

     return ref;
 }


void spec_driver::adjust_inherited()
{
    vector<ProblemNode> nodes = problem.get_nodes();
    for(vector<ProblemNode>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        if((*it).get_initials().empty())
        {
            if((*it).get_ref_node()->get_initials().empty())
            {
                if((*it).get_ref_node()->get_net_model()->get_initials().empty())
                {
                    string msg = "Initial states undefined for problem node ";
                    msg.append((*it).get_name());
                    error(loc,msg);
                }
                else
                    (*it).set_initials((*it).get_ref_node()->get_net_model()->get_initials());
            }
            else
                (*it).set_initials((*it).get_ref_node()->get_initials());
        }

        if((*it).get_viewer().empty())
        {
            if((*it).get_ref_node()->get_viewer().empty())
            {
                /*if((*it).ref_node->net_model->viewer.empty())
                {
                    string msg = "Viewer undefined for problem node ";
                    msg.append((*it).name);
                    error(loc,msg);
                }
                else*/
                    (*it).set_viewer((*it).get_ref_node()->get_net_model()->get_viewer());
            }
            else
                (*it).set_viewer((*it).get_ref_node()->get_viewer());
        }

        if((*it).get_ruler().empty())
        {
            if((*it).get_ref_node()->get_ruler().empty())
            {
                /*if((*it).ref_node->net_model->ruler.empty())
                {
                    string msg = "Ruler undefined for problem node ";
                    msg.append((*it).name);
                    error(loc,msg);
                }
                else*/
                    (*it).set_ruler((*it).get_ref_node()->get_net_model()->get_ruler());
            }
            else
                (*it).set_ruler((*it).get_ref_node()->get_ruler());
        }
    }


    for(vector<SystemNode>::iterator it = system.node_list.begin(); it != system.node_list.end(); ++it)
    {
        if((*it).get_initials().empty())
        {
            if((*it).get_net_model()->get_initials().empty())
            {
                string msg = "Initial states undefined for system node ";
                msg.append((*it).get_name());
                error(loc,msg);
            }
            else
                (*it).set_initials((*it).get_net_model()->get_initials());
        }

        if((*it).get_viewer().empty())
        {
            /*if((*it).net_model->viewer.empty())
            {
                string msg = "Viewer undefined for system node ";
                msg.append((*it).name);
                error(loc,msg);
            }
            else*/
                (*it).set_viewer((*it).get_net_model()->get_viewer());
        }

        if((*it).get_ruler().empty())
        {
            /*if((*it).net_model->ruler.empty())
            {
                string msg = "Ruler undefined for system node ";
                msg.append((*it).name);
                error(loc,msg);
            }
            else*/
                (*it).set_ruler((*it).get_net_model()->get_ruler());
        }
    }
    problem.set_nodes(nodes);
}


void spec_driver::build_automata_comp()
{
    for(vector<ComponentModel>::iterator it = components.begin(); it != components.end(); ++it)
    {
        (*it).build_automaton();
    }
}


void spec_driver::build_Isp()
{
    vector<ProblemNode> nodes = problem.get_nodes();
    vector<ProblemNode>::iterator it;
    for(it = nodes.begin(); it != nodes.end(); it++)
    {
        //it->set_index_space(new astl::DFA_map<astl::strings,StateData_str>());
        DFA_map<strings,StateData_str>::state_type s = it->get_index_space()->new_state();
        it->get_index_space()->tag(s) = StateData_str("I1");
        it->get_index_space()->initial(s);
        vector<std::string>::iterator it2;
        int count_state = 2;
        vector<std::string> obs = it->get_observation();
        for(it2 = obs.begin(); it2 != obs.end(); it2++)
        {
            DFA_map<strings,StateData_str>::state_type s1 = it->get_index_space()->new_state();
            std::stringstream ss;
            ss << "I";
            ss << count_state;
            it->get_index_space()->tag(s1) = StateData_str(ss.str());
            it->get_index_space()->set_trans(s,*it2,s1);
            s = s1;
            count_state++;
        }
        it->get_index_space()->final(s) = true;
    }
}


bool spec_driver::duplicate_component_model_id(std::string id)
{
    for(vector<ComponentModel>::iterator it = components.begin(); it != components.end(); ++it)
    {
        if((*it).get_name() == id)
            return true;
    }
    return false;
}


bool spec_driver::duplicate_network_model_id(std::string id)
{
    for(vector<NetworkModel>::iterator it = networks.begin(); it != networks.end(); ++it)
    {
        if((*it).get_name() == id)
            return true;
    }
    return false;
}


void spec_driver::semantic_checks(ComponentModel cm)
{
    vector<std::string> events = cm.get_events();
    if(Utils::duplicate_content(events))
        error(loc, "Semantic error: duplicate event ID");
    vector<std::string> inputs = cm.get_inputs();
    if(Utils::duplicate_content(inputs))
       error(loc, "Semantic error: duplicate input terminal ID");
    vector<std::string> outputs = cm.get_outputs();
    if(Utils::duplicate_content(outputs))
       error(loc, "Semantic error: duplicate output terminal ID");
    vector<std::string> states = cm.get_states();
    if(Utils::duplicate_content(states))
       error(loc, "Semantic error: duplicate state ID");
    vector<Transition> trans = cm.get_trans();
    if(Utils::duplicate_content(trans))
       error(loc, "Semantic error: duplicate transition ID");
    for(vector<Transition>::iterator it = trans.begin(); it != trans.end(); it++)
    {
        std::pair<std::string,std::string> in_ev = it->get_input_event();
        if(in_ev.first != "" && !Utils::contain(events,in_ev.first))
            error(loc, "event ID does not exist");
        if(in_ev.first != "" && !Utils::contain(inputs,in_ev.second))
            error(loc, "input terminal ID does not exist");
        vector<std::string> states = cm.get_states();
        std::pair<std::string,std::string> s12 = it->get_s1_s2();
        if(!Utils::contain(states,s12.first) || !Utils::contain(states,s12.second))
            error(loc, "state ID does not exist");
        std::vector<std::pair<std::string,std::string> > out_evs = it->get_out_events();
        for(vector<pair<string,string> >::iterator it2 = out_evs.begin(); it2 != out_evs.end(); it2++)
        {
            if(!Utils::contain(events,(*it2).first))
                error(loc, "event ID does not exist");
            if(!Utils::contain(outputs,(*it2).second))
                error(loc, "output terminal ID does not exist");
        }
    }
}


void spec_driver::semantic_checks(NetworkModel nm)
{
    //verifies that components ids are unique
    vector<NetComponent> comps = nm.get_components();
    if(Utils::duplicate_content(comps))
        error(loc, "Semantic error: duplicate component ID");

    //verifies that input terminals are unique
    vector<std::string> inputs = nm.get_inputs();
    if(Utils::duplicate_content(inputs))
        error(loc, "Semantic error: duplicate input terminal ID");

    //verifies that output terminals are unique
    vector<std::string> outputs = nm.get_outputs();
    if(Utils::duplicate_content(outputs))
        error(loc, "Semantic error: duplicate output terminal ID");

    //saves a list of components ids
    vector<string> comp_ids;
    for(vector<NetComponent>::iterator it = comps.begin(); it != comps.end(); it++)
        comp_ids.push_back((*it).get_name());

    //Verifies terminal and components existence within link declaration
    vector<pair<pair<string,string>,pair<string,string> > > links = nm.get_links();
    for(vector<pair<pair<string,string>,pair<string,string> > >::iterator it = links.begin();
        it != links.end(); it++)
    {
        if(it->first.second != "this")
        {
            if(!Utils::contain(comp_ids,(*it).first.second))
                error(loc, "component ID in link does not exist");
            vector<std::string> outputs = nm.find_component((*it).first.second)->get_model()->get_outputs();
            if(!Utils::contain(outputs,(*it).first.first))
                error(loc, "output terminal ID in link does not exist");
        }
        else
        {
            if(!Utils::contain(inputs,it->first.first))
                error(loc, "input terminal ID in network does not exist");
        }
        if(!Utils::contain(comp_ids,(*it).second.second))
            error(loc, "component ID in link does not exist");
        vector<std::string> inputs = nm.find_component((*it).second.second)->get_model()->get_inputs();
        if(!Utils::contain(inputs,(*it).second.first))
            error(loc, "inputs terminal ID in link does not exist");
    }

    //verifies that an input terminal is connected at most with one link (output terminals do not need this check)
    for(vector<pair<pair<string,string>,pair<string,string> > >::iterator it1 = links.begin();
        it1 != links.end(); it1++)
    {
        for(vector<pair<pair<string,string>,pair<string,string> > >::iterator it2 = it1 + 1;
            it2 != links.end(); it2++)
        {
            if(it1->second == it2->second)
                error(loc, "an input terminal can be connected with at most one link");
        }
    }

    //saves a list of patterns ids
    vector<string> patt_ids;
    vector<Pattern> patts = nm.get_patterns();
    for(vector<Pattern>::iterator it = patts.begin(); it != patts.end(); it++)
    {
        patt_ids.push_back(it->get_name());
        std::string  id = it->get_terminal_id();
        if(!Utils::contain(outputs,id))
            error(loc, "Semantic error: output pattern terminal ID of network does not exist");
    }
    //verifies that patterns ids are unique
    if(Utils::duplicate_content(patt_ids))
        error(loc, "Semantic error: duplicate pattern ID");

    //verifies existence of component transitions in pattern declaration
    for(map<pair<string,string>,int>::iterator it = nm.conv_str_int.begin(); it != nm.conv_str_int.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
        {
            std::string msg = "component ID in pattern declaration does not exist ";
            cout << nm;
            error(loc, msg);
        }
        if(nm.find_component(it->first.second)->get_model()->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in pattern declaration does not exist");
    }

    //verifies existence of initial states components in initial states declaration
    vector<pair<string,string> > inits = nm.get_initials();
    for(vector<pair<string,string> >::iterator it = inits.begin(); it != inits.end(); it++)
    {
        if(!Utils::contain(comp_ids,it->second))
            error(loc, "component ID in initial states declaration does not exist");
        vector<std::string> states = nm.find_component(it->second)->get_model()->get_states();
         if(!Utils::contain(states,it->first))
             error(loc, "state ID in initial states declaration does not exist");
    }

    //verifies existence of component transitions in viewer declaration
    map<pair<string,string>, string> viewers = nm.get_viewer();
    for(map<pair<string,string>, string>::iterator it = viewers.begin(); it != viewers.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in viewer declaration does not exist");
        if(nm.find_component(it->first.second)->get_model()->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in viewer declaration does not exist");
    }

    //verifies existence of component transitions in ruler declaration
    map<pair<string,string>, string> ruler = nm.get_ruler();
    for(map<pair<string,string>, string>::iterator it = ruler.begin(); it != ruler.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in ruler declaration does not exist");
        if(nm.find_component(it->first.second)->get_model()->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in ruler declaration does not exist");
    }

    //verifies that the ruler mapping is a bijection (while the viewer function is surijective and does not need this check)
    /*for(map<pair<string,string>, string>::iterator it1 = nm.ruler.begin(); it1 != nm.ruler.end(); it1++)
    {
        for(map<pair<string,string>, string>::iterator it2 = it1 ; it2 != nm.ruler.end(); )
        {
            it2++;
            if(it2 != nm.ruler.end() && it1->second == it2->second)
                error(loc, "ruler mapping must be a bijection, duplicate fault label used");
        }
    }*/

}


void spec_driver::semantic_checks(System sys)
{
    for(vector<SystemNode>::iterator it = sys.node_list.begin(); it != sys.node_list.end(); it++)
        semantic_checks(*it);

    //verifies nodes and relative terminals existence involved in emergence declarations
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > emer = sys.get_emergence();
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >::iterator it;
    for(it = emer.begin(); it != emer.end(); it++)
    {
        if(sys.find_node(it->first.second) == NULL || sys.find_node(it->second.second) == NULL)
            error(loc, "system node ID in emergence declaration does not exist");
        vector<std::string> outputs = sys.find_node(it->first.second)->get_net_model()->get_outputs();
        if(!Utils::contain(outputs,it->first.first))
            error(loc, "network output terminal ID in emergence declaration does not exist");
        vector<std::string> inputs = sys.find_node(it->second.second)->get_net_model()->get_inputs();
        if(!Utils::contain(inputs,it->second.first))
            error(loc, "network input terminal ID in emergence declaration does not exist");
    }

    //verifies one-to-one mapping in emergence between nodes, in order to be a tree structure OLD REQUIREMENT
    /*for(map<pair<string,string>,string>::iterator it = sys.emergence.begin(); it != sys.emergence.end(); it++)
    {
        for(map<pair<string,string>,string>::iterator it2 = it; it2 != sys.emergence.end(); )
        {
            it2++;
            if(it2 == sys.emergence.end())
                break;
            if(it->first == it2->first || it->second == it2->second)
                error(loc, "system nodes dependencies must be a tree");
        }
    }*/

}


void spec_driver::semantic_checks(SystemNode node)
{
    //saves a list of components ids
    vector<string> comp_ids;
    vector<NetComponent> comps = node.get_net_model()->get_components();
    for(vector<NetComponent>::iterator it = comps.begin(); it != comps.end(); it++)
        comp_ids.push_back((*it).get_name());

    //verifies existence of initial states components in initial states declaration
    vector<pair<string,string> > inis = node.get_initials();
    for(vector<pair<string,string> >::iterator it = inis.begin(); it != inis.end(); it++)
    {
        if(!Utils::contain(comp_ids,it->second))
            error(loc, "component ID in initial states declaration does not exist");
        vector<std::string> states = node.get_net_model()->find_component(it->second)->get_model()->get_states();
         if(!Utils::contain(states,it->first))
             error(loc, "state ID in initial states declaration does not exist");
    }

    //verifies existence of component transitions in viewer declaration
    map<pair<string,string>, string> vwr = node.get_viewer();
    for(map<pair<string,string>, string>::iterator it = vwr.begin(); it != vwr.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in viewer declaration does not exist");
        if(node.get_net_model()->find_component(it->first.second)->get_model()->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in viewer declaration does not exist");
    }

    //verifies existence of component transitions in ruler declaration
    map<pair<string,string>, string> rlr = node.get_ruler();
    for(map<pair<string,string>, string>::iterator it = rlr.begin(); it != rlr.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in ruler declaration does not exist");
        if(node.get_net_model()->find_component(it->first.second)->get_model()->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in ruler declaration does not exist");
    }

    //verifies that the ruler mapping is a bijection (while the viewer function is surijective and does not need this check)
    /*for(map<pair<string,string>, string>::iterator it1 = node.ruler.begin(); it1 != node.ruler.end(); it1++)
    {
        for(map<pair<string,string>, string>::iterator it2 = it1 ; it2 != node.ruler.end(); )
        {
            it2++;
            if(it1->second == it2->second)
                error(loc, "ruler mapping must be a bijection, duplicate fault label used");
        }
    }*/
}


void spec_driver::semantic_checks(Problem pb)
{
    vector<ProblemNode> nodes = pb.get_nodes();
    for(vector<ProblemNode>::iterator it = nodes.begin(); it != nodes.end(); it++)
        semantic_checks(*it);
    //verifies that all system nodes are considered in problem
    if(system.node_list.size() > nodes.size())
        error(loc, "some node are not considered in problem");
}


void spec_driver::semantic_checks(ProblemNode node)
{
    //saves a list of components ids
    vector<string> comp_ids;
    NetworkModel *nm = node.get_ref_node()->get_net_model();
    vector<NetComponent> comps = nm->get_components();
    for(vector<NetComponent>::iterator it = comps.begin(); it != comps.end(); it++)
        comp_ids.push_back((*it).get_name());

    //verifies existence of initial states components in initial states declaration
    vector<pair<std::string,std::string> > inis = node.get_initials();
    for(vector<pair<string,string> >::iterator it = inis.begin(); it != inis.end(); it++)
    {
        if(!Utils::contain(comp_ids,it->second))
            error(loc, "component ID in initial states declaration does not exist");
        vector<std::string> states = node.get_ref_node()->get_net_model()->find_component(it->second)->get_model()->get_states();
         if(!Utils::contain(states,it->first))
             error(loc, "state ID in initial states declaration does not exist");
    }

    //verifies existence of component transitions in viewer declaration
    map<pair<std::string,std::string>,std::string> vwr = node.get_viewer();
    for(map<pair<string,string>, string>::iterator it = vwr.begin(); it != vwr.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in viewer declaration does not exist");
        if(node.get_ref_node()->get_net_model()->find_component(it->first.second)->get_model()->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in viewer declaration does not exist");
    }

    //verifies existence of component transitions in ruler declaration
    map<pair<std::string,std::string>,std::string> rlr = node.get_ruler();
    for(map<pair<string,string>, string>::iterator it = rlr.begin(); it != rlr.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in ruler declaration does not exist");
        if(node.get_ref_node()->get_net_model()->find_component(it->first.second)->get_model()->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in ruler declaration does not exist");
    }

    //verifies that the ruler mapping is a bijection (while the viewer function is surijective and does not need this check)
    /*for(map<pair<string,string>, string>::iterator it1 = node.ruler.begin(); it1 != node.ruler.end(); it1++)
    {
        for(map<pair<string,string>, string>::iterator it2 = it1 ; it2 != node.ruler.end(); )
        {
            it2++;
            if(it1->second == it2->second)
                error(loc, "ruler mapping must be a bijection, duplicate fault label used");
        }
    }*/

    //assigns the current viewer in a hereditary way
    if(node.get_viewer().empty())
        node.set_viewer(node.get_ref_node()->get_viewer());
    if(node.get_viewer().empty())
        node.set_viewer(node.get_ref_node()->get_net_model()->get_viewer());

    //verifies existence of observation labels
    vector<std::string> obs = node.get_observation();
    for(vector<string>::iterator it = obs.begin(); it != obs.end(); it++)
    {
        bool label_find = false;
        map<pair<string,string>, string> vwr = node.get_viewer();
        for(map<pair<string,string>, string>::iterator it2 = vwr.begin(); it2 != vwr.end(); it2++)
        {
            if(*it == it2->second)
            {
                label_find = true;
                break;
            }
        }
        if(!label_find)
            error(loc, "label in observation does not exist");
    }
}


void spec_driver::build_dependency_graph()
{
    //system.set_dependency_graph(new astl::DFA_map<astl::strings,StateData_str>());
    if(system.get_emergence().empty())
    {
        DFA_map<strings,StateData_str>::state_type single_state;
        single_state = system.get_dependency_graph()->new_state();
        system.get_dependency_graph()->tag(single_state) = StateData_str(system.node_list.at(0).get_name());
        system.get_dependency_graph()->initial(single_state);
    }
    bool radix[problem.get_nodes().size()];
    for(unsigned int i=0; i<problem.get_nodes().size();i++)
        radix[i] = true;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > emer = system.get_emergence();
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >::iterator it;
    for(it = emer.begin(); it != emer.end(); it++)
    {
        DFA_map<strings,StateData_str>::state_type s1 =  DFA_map<strings,StateData_str>::null_state;
        DFA_map<strings,StateData_str>::state_type s2 = DFA_map<strings,StateData_str>::null_state;
        DFA_map<strings,StateData_str>::const_iterator c;
        //linear search in the state space (better improve efficiency in future: hash)
        for(c = system.get_dependency_graph()->begin(); c != system.get_dependency_graph()->end(); c++)
        {
            if(system.get_dependency_graph()->tag(*c) == StateData_str(it->second.second))
                s1 = *c;
            if(system.get_dependency_graph()->tag(*c) == StateData_str(it->first.second))
                s2 = *c;
        }

        if(s1 == DFA_map<strings,StateData_str>::null_state)
        {
            s1 = system.get_dependency_graph()->new_state();
            system.get_dependency_graph()->tag(s1) = StateData_str(it->second.second);
            //initial node (arbitrary) made just for automaton visualitation
            //if(system.dependency_graph.initial() == DFA_map<strings,StateData_str>::null_state)
                //system.dependency_graph.initial(s1);
        }
        if(s2 == DFA_map<strings,StateData_str>::null_state)
        {
            s2 = system.get_dependency_graph()->new_state();
            system.get_dependency_graph()->tag(s2) = StateData_str(it->first.second);
        }

        string label;
        label = it->first.first;
        label.append("("); label.append(it->first.second); label.append(")");
        label.append(" -> ");
        label.append(it->second.first);
        label.append("("); label.append(it->second.second); label.append(")");
        system.get_dependency_graph()->set_trans(s1,label,s2);
        radix[s2] = false;
    }

    set<unsigned int> radixes;
    for(DFA_map<strings,StateData_str>::const_iterator c = system.get_dependency_graph()->begin(); c != system.get_dependency_graph()->end(); c++)
    {
        if(radix[*c])
            radixes.insert(*c);
    }
    for(set<unsigned int>::iterator it = radixes.begin(); it != radixes.end(); it++)
    {
        problem.find_node(system.get_dependency_graph()->tag(*it).get_state_name())->set_radix();
    }

    if(system.get_dependency_graph()->state_count() < system.node_list.size()  //detects single disconnected nodes
            || Utils::disconnected_graph(*system.get_dependency_graph(), radixes))      //detects disconnected subgraphs
        error(loc, "System nodes graph is not connected");

    if(!Utils::cyclic_graph(*system.get_dependency_graph()))
    {
        //error(loc, "Cyclic dependencies between system nodes"); //Old specification: system graph had to be a tree
        system.set_acyclic();
        vector<unsigned int> sorted = Utils::topological_sort(*system.get_dependency_graph());
        vector<int> top_order;
        for(vector<unsigned int>::iterator it = sorted.begin(); it != sorted.end(); it++)
            top_order.push_back(problem.find_index(system.get_dependency_graph()->tag(*it).get_state_name()));
        problem.set_topological_order(top_order);
        for(DFA_map<strings,StateData_str>::const_iterator c = system.get_dependency_graph()->begin(); c != system.get_dependency_graph()->end(); c++)
        {
            forward_cursor<DFA_map<strings,StateData_str> > fc(*system.get_dependency_graph(), *c);
            if(fc.first())
            {
                ProblemNode *pn = problem.find_node(system.get_dependency_graph()->tag(*c).get_state_name());
                do
                {
                    int index = problem.find_index(fc.aim_tag().get_state_name());
                    pn->add_depend(index);
                }
                while(fc.next());
            }
        }
    }

}

#include "ostream_util.h"

std::ostream& operator<<(std::ostream& out, const spec_driver& d)
{
    out << d.components;
    out << d.networks;
    out << d.system;
    out << d.problem;

    return out;
}
