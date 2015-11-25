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

vector<Component> spec_driver::build_components(vector<string> ids, string id_model)
{
    ComponentModel* ref = NULL;
    for (vector<ComponentModel>::iterator it = components.begin(); it != components.end(); ++it)
    {
        if(it->name.compare(id_model) == 0)
        {
            ref = &(*it);
            break;
        }
    }
    if(ref == NULL)
        error(loc, "ID type component model not defined.");
    vector<Component> l;
    for (vector<string>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
        Component c(*it);
        c.model = ref;
        l.push_back(c);
    }

    return l;
}


NetworkModel* spec_driver::find_netmodel(std::string id_model)
{
    NetworkModel* ref = NULL;
    for (vector<NetworkModel>::iterator it = networks.begin(); it != networks.end(); ++it)
    {
        if(it->name == id_model)
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
         if(it->name == id)
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
    for(vector<ProblemNode>::iterator it = problem.nodes.begin(); it != problem.nodes.end(); ++it)
    {
        if((*it).initials.empty())
        {
            if((*it).ref_node->initials.empty())
            {
                if((*it).ref_node->net_model->initials.empty())
                {
                    string msg = "Initial states undefined for problem node ";
                    msg.append((*it).name);
                    error(loc,msg);
                }
                else
                    (*it).initials = (*it).ref_node->net_model->initials;
            }
            else
                (*it).initials = (*it).ref_node->initials;
        }

        if((*it).viewer.empty())
        {
            if((*it).ref_node->viewer.empty())
            {
                if((*it).ref_node->net_model->viewer.empty())
                {
                    string msg = "Viewer undefined for problem node ";
                    msg.append((*it).name);
                    error(loc,msg);
                }
                else
                    (*it).viewer = (*it).ref_node->net_model->viewer;
            }
            else
                (*it).viewer = (*it).ref_node->viewer;
        }

        if((*it).ruler.empty())
        {
            if((*it).ref_node->ruler.empty())
            {
                if((*it).ref_node->net_model->ruler.empty())
                {
                    string msg = "Ruler undefined for problem node ";
                    msg.append((*it).name);
                    error(loc,msg);
                }
                else
                    (*it).ruler = (*it).ref_node->net_model->ruler;
            }
            else
                (*it).ruler = (*it).ref_node->ruler;
        }
    }

    for(vector<SystemNode>::iterator it = system.node_list.begin(); it != system.node_list.end(); ++it)
    {
        if((*it).initials.empty())
        {
            if((*it).net_model->initials.empty())
            {
                string msg = "Initial states undefined for system node ";
                msg.append((*it).name);
                error(loc,msg);
            }
            else
                (*it).initials = (*it).net_model->initials;
        }

        if((*it).viewer.empty())
        {
            if((*it).net_model->viewer.empty())
            {
                string msg = "Viewer undefined for system node ";
                msg.append((*it).name);
                error(loc,msg);
            }
            else
                (*it).viewer = (*it).net_model->viewer;
        }

        if((*it).ruler.empty())
        {
            if((*it).net_model->ruler.empty())
            {
                string msg = "Ruler undefined for system node ";
                msg.append((*it).name);
                error(loc,msg);
            }
            else
                (*it).ruler = (*it).net_model->ruler;
        }
    }
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
     vector<ProblemNode>::iterator it;
    for(it = problem.nodes.begin(); it != problem.nodes.end(); it++)
    {
        DFA_map<strings,StateData_str>::state_type s = it->index_space.new_state();
        it->index_space.tag(s) = StateData_str("I1");
        it->index_space.initial(s);
        vector<std::string>::iterator it2;
        int count_state = 2;
        for(it2 = it->observation.begin(); it2 != it->observation.end(); it2++)
        {
            DFA_map<strings,StateData_str>::state_type s1 = it->index_space.new_state();
            std::stringstream ss;
            ss << "I";
            ss << count_state;
            it->index_space.tag(s1) = StateData_str(ss.str());
            it->index_space.set_trans(s,*it2,s1);
            s = s1;
            count_state++;
        }
        it->index_space.final(s) = true;
    }
}


bool spec_driver::duplicate_component_model_id(std::string id)
{
    for(vector<ComponentModel>::iterator it = components.begin(); it != components.end(); ++it)
    {
        if((*it).name == id)
            return true;
    }
    return false;
}


bool spec_driver::duplicate_network_model_id(std::string id)
{
    for(vector<NetworkModel>::iterator it = networks.begin(); it != networks.end(); ++it)
    {
        if((*it).name == id)
            return true;
    }
    return false;
}


void spec_driver::semantic_checks(ComponentModel cm)
{
    if(Utils::duplicate_content(cm.events))
        error(loc, "Semantic error: duplicate event ID");
    if(Utils::duplicate_content(cm.inputs))
       error(loc, "Semantic error: duplicate input terminal ID");
    if(Utils::duplicate_content(cm.outputs))
       error(loc, "Semantic error: duplicate output terminal ID");
    if(Utils::duplicate_content(cm.states))
       error(loc, "Semantic error: duplicate state ID");
    if(Utils::duplicate_content(cm.trans))
       error(loc, "Semantic error: duplicate transition ID");
    for(vector<Transition>::iterator it = cm.trans.begin(); it != cm.trans.end(); it++)
    {
        if((*it).input_event.first != "" && !Utils::contain(cm.events,(*it).input_event.first))
            error(loc, "event ID does not exist");
        if((*it).input_event.first != "" && !Utils::contain(cm.inputs,(*it).input_event.second))
            error(loc, "input terminal ID does not exist");
        if(!Utils::contain(cm.states,(*it).s1_s2.first) || !Utils::contain(cm.states,(*it).s1_s2.second))
            error(loc, "state ID does not exist");
        for(vector<pair<string,string> >::iterator it2 = (*it).out_events.begin(); it2 != (*it).out_events.end(); it2++)
        {
            if(!Utils::contain(cm.events,(*it2).first))
                error(loc, "event ID does not exist");
            if(!Utils::contain(cm.outputs,(*it2).second))
                error(loc, "output terminal ID does not exist");
        }
    }
}


void spec_driver::semantic_checks(NetworkModel nm)
{
    //verifies that components ids are unique
    if(Utils::duplicate_content(nm.components))
        error(loc, "Semantic error: duplicate component ID");

    //saves a list of components ids
    vector<string> comp_ids;
    for(vector<Component>::iterator it = nm.components.begin(); it != nm.components.end(); it++)
        comp_ids.push_back((*it).name);

    //Verifies terminal and components existence within link declaration
    for(vector<pair<pair<string,string>,pair<string,string> > >::iterator it = nm.links.begin();
        it != nm.links.end(); it++)
    {
        if(!Utils::contain(comp_ids,(*it).first.second))
            error(loc, "component ID in link does not exist");
        if(!Utils::contain(nm.find_component((*it).first.second)->model->outputs,(*it).first.first))
            error(loc, "output terminal ID in link does not exist");
        if(!Utils::contain(comp_ids,(*it).second.second))
            error(loc, "component ID in link does not exist");
        if(!Utils::contain(nm.find_component((*it).second.second)->model->inputs,(*it).second.first))
            error(loc, "inputs terminal ID in link does not exist");
    }

    //verifies that an input terminal is connected at most with one link (output terminals do not need this check)
    for(vector<pair<pair<string,string>,pair<string,string> > >::iterator it1 = nm.links.begin();
        it1 != nm.links.end(); it1++)
    {
        for(vector<pair<pair<string,string>,pair<string,string> > >::iterator it2 = it1 + 1;
            it2 != nm.links.end(); it2++)
        {
            if(it1->second == it2->second)
                error(loc, "an input terminal can be connected with at most one link");
        }
    }

    //saves a list of patterns ids
    vector<string> patt_ids;
    for(vector<pair<string,string> >::iterator it = nm.patterns.begin(); it != nm.patterns.end(); it++)
        patt_ids.push_back((*it).first);
    //verifies that patterns ids are unique
    if(Utils::duplicate_content(patt_ids))
        error(loc, "Semantic error: duplicate pattern ID");

    //verifies existence of component transitions in pattern declaration
    for(map<pair<string,string>,int>::iterator it = nm.conv_str_int.begin(); it != nm.conv_str_int.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in pattern declaration does not exist");
        if(nm.find_component(it->first.second)->model->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in pattern declaration does not exist");
    }

    //verifies existence of initial states components in initial states declaration
    for(vector<pair<string,string> >::iterator it = nm.initials.begin(); it != nm.initials.end(); it++)
    {
        if(!Utils::contain(comp_ids,it->second))
            error(loc, "component ID in initial states declaration does not exist");
         if(!Utils::contain(nm.find_component(it->second)->model->states,it->first))
             error(loc, "state ID in initial states declaration does not exist");
    }

    //verifies existence of component transitions in viewer declaration
    for(map<pair<string,string>, string>::iterator it = nm.viewer.begin(); it != nm.viewer.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in viewer declaration does not exist");
        if(nm.find_component(it->first.second)->model->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in viewer declaration does not exist");
    }

    //verifies existence of component transitions in ruler declaration
    for(map<pair<string,string>, string>::iterator it = nm.ruler.begin(); it != nm.ruler.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in ruler declaration does not exist");
        if(nm.find_component(it->first.second)->model->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in ruler declaration does not exist");
    }

    //verifies that the ruler mapping is a bijection (while the viewer function is surijective and does not need this check)
    for(map<pair<string,string>, string>::iterator it1 = nm.ruler.begin(); it1 != nm.ruler.end(); it1++)
    {
        for(map<pair<string,string>, string>::iterator it2 = it1 ; it2 != nm.ruler.end(); )
        {
            it2++;
            if(it2 != nm.ruler.end() && it1->second == it2->second)
                error(loc, "ruler mapping must be a bijection, duplicate fault label used");
        }
    }

}


void spec_driver::semantic_checks(System sys)
{
    for(vector<SystemNode>::iterator it = sys.node_list.begin(); it != sys.node_list.end(); it++)
        semantic_checks(*it);

    //verifies components and transitions existence involved in emergence declarations
    for(map<pair<string,string>,string>::iterator it = sys.emergence.begin(); it != sys.emergence.end(); it++)
    {
        if(sys.find_node(it->first.second) == NULL || sys.find_node(it->second) == NULL)
            error(loc, "system node ID in emergence declaration does not exist");
        if(sys.find_node(it->first.second)->net_model->find_component(it->first.first) == NULL)
            error(loc, "component ID in emergence declaration does not exist");
    }

    //verifies one-to-one mapping in emergence between nodes, in order to be a tree structure
    for(map<pair<string,string>,string>::iterator it = sys.emergence.begin(); it != sys.emergence.end(); it++)
    {
        for(map<pair<string,string>,string>::iterator it2 = it; it2 != sys.emergence.end(); )
        {
            it2++;
            if(it2 == sys.emergence.end())
                break;
            if(it->first == it2->first || it->second == it2->second)
                error(loc, "system nodes dependencies must be a tree");
        }
    }

}


void spec_driver::semantic_checks(SystemNode node)
{
    //saves a list of components ids
    vector<string> comp_ids;
    for(vector<Component>::iterator it = node.net_model->components.begin(); it != node.net_model->components.end(); it++)
        comp_ids.push_back((*it).name);

    //verifies existence of initial states components in initial states declaration
    for(vector<pair<string,string> >::iterator it = node.initials.begin(); it != node.initials.end(); it++)
    {
        if(!Utils::contain(comp_ids,it->second))
            error(loc, "component ID in initial states declaration does not exist");
         if(!Utils::contain(node.net_model->find_component(it->second)->model->states,it->first))
             error(loc, "state ID in initial states declaration does not exist");
    }

    //verifies existence of component transitions in viewer declaration
    for(map<pair<string,string>, string>::iterator it = node.viewer.begin(); it != node.viewer.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in viewer declaration does not exist");
        if(node.net_model->find_component(it->first.second)->model->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in viewer declaration does not exist");
    }

    //verifies existence of component transitions in ruler declaration
    for(map<pair<string,string>, string>::iterator it = node.ruler.begin(); it != node.ruler.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in ruler declaration does not exist");
        if(node.net_model->find_component(it->first.second)->model->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in ruler declaration does not exist");
    }

    //verifies that the ruler mapping is a bijection (while the viewer function is surijective and does not need this check)
    for(map<pair<string,string>, string>::iterator it1 = node.ruler.begin(); it1 != node.ruler.end(); it1++)
    {
        for(map<pair<string,string>, string>::iterator it2 = it1 ; it2 != node.ruler.end(); )
        {
            it2++;
            if(it1->second == it2->second)
                error(loc, "ruler mapping must be a bijection, duplicate fault label used");
        }
    }
}


void spec_driver::semantic_checks(Problem pb)
{
    for(vector<ProblemNode>::iterator it = pb.nodes.begin(); it != pb.nodes.end(); it++)
        semantic_checks(*it);
    //verifies that all system nodes are considered in problem
    if(system.node_list.size() > pb.nodes.size())
        error(loc, "some node are not considered in problem");
}


void spec_driver::semantic_checks(ProblemNode node)
{
    //saves a list of components ids
    vector<string> comp_ids;
    for(vector<Component>::iterator it = node.ref_node->net_model->components.begin(); it != node.ref_node->net_model->components.end(); it++)
        comp_ids.push_back((*it).name);

    //verifies existence of initial states components in initial states declaration
    for(vector<pair<string,string> >::iterator it = node.initials.begin(); it != node.initials.end(); it++)
    {
        if(!Utils::contain(comp_ids,it->second))
            error(loc, "component ID in initial states declaration does not exist");
         if(!Utils::contain(node.ref_node->net_model->find_component(it->second)->model->states,it->first))
             error(loc, "state ID in initial states declaration does not exist");
    }

    //verifies existence of component transitions in viewer declaration
    for(map<pair<string,string>, string>::iterator it = node.viewer.begin(); it != node.viewer.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in viewer declaration does not exist");
        if(node.ref_node->net_model->find_component(it->first.second)->model->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in viewer declaration does not exist");
    }

    //verifies existence of component transitions in ruler declaration
    for(map<pair<string,string>, string>::iterator it = node.ruler.begin(); it != node.ruler.end(); it++)
    {
        if(!Utils::contain(comp_ids,(string&)it->first.second))
            error(loc, "component ID in ruler declaration does not exist");
        if(node.ref_node->net_model->find_component(it->first.second)->model->find_trans(it->first.first) == NULL)
            error(loc, "transition ID in ruler declaration does not exist");
    }

    //verifies that the ruler mapping is a bijection (while the viewer function is surijective and does not need this check)
    for(map<pair<string,string>, string>::iterator it1 = node.ruler.begin(); it1 != node.ruler.end(); it1++)
    {
        for(map<pair<string,string>, string>::iterator it2 = it1 ; it2 != node.ruler.end(); )
        {
            it2++;
            if(it1->second == it2->second)
                error(loc, "ruler mapping must be a bijection, duplicate fault label used");
        }
    }

    //assigns the current viewer in a hereditary way
    if(node.viewer.empty())
        node.viewer = node.ref_node->viewer;
    if(node.viewer.empty())
        node.viewer = node.ref_node->net_model->viewer;

    //verifies existence of observation labels
    for(vector<string>::iterator it = node.observation.begin(); it != node.observation.end(); it++)
    {
        bool label_find = false;
        for(map<pair<string,string>, string>::iterator it2 = node.viewer.begin(); it2 != node.viewer.end(); it2++)
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
    for(map<pair<string,string>,string>::iterator it = system.emergence.begin(); it != system.emergence.end(); it++)
    {
        DFA_map<strings,StateData_str>::state_type s1 =  DFA_map<strings,StateData_str>::null_state;
        DFA_map<strings,StateData_str>::state_type s2 = DFA_map<strings,StateData_str>::null_state;
        DFA_map<strings,StateData_str>::const_iterator c;
        //linear search in the state space (better improve efficiency in future)
        for(c = system.dependency_graph.begin(); c != system.dependency_graph.end(); c++)
        {
            if(system.dependency_graph.tag(*c) == StateData_str(it->first.second))
                s1 = *c;
            if(system.dependency_graph.tag(*c) == StateData_str(it->second))
                s2 = *c;
        }

        if(s1 == DFA_map<strings,StateData_str>::null_state)
        {
            s1 = system.dependency_graph.new_state();
            system.dependency_graph.tag(s1) = StateData_str(it->first.second);
            if(system.root->name == it->first.second)
                system.dependency_graph.initial(s1);
        }
        if(s2 == DFA_map<strings,StateData_str>::null_state)
        {
            s2 = system.dependency_graph.new_state();
            system.dependency_graph.tag(s2) = StateData_str(it->second);
        }
        else if(system.dependency_graph.tag(s2) == system.root->name)
            error(loc,"Root node cannot have ingoing transitions");

        string label;
        label = it->first.first;
        label.append("("); label.append(it->first.second); label.append(")");
        label.append(" -> ");
        label.append(it->second);
        system.dependency_graph.set_trans(s1,label,s2);

        //make_hash(system.dependency_graph);
    }

    if(Utils::cyclic_graph(system.dependency_graph))
        error(loc, "Cyclic dependencies between system nodes");
    if(system.dependency_graph.state_count() < system.node_list.size()  //detects single disconnected nodes
            || Utils::disconnected_graph(system.dependency_graph))      //detects disconnected subgraphs
        error(loc, "System nodes graph is not connected");
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
