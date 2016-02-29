#include "ostream_util.h"

using namespace std;

ostream& operator<<(ostream& out, const ComponentModel& cm)
{
    return out << "Component model: " << cm.get_name() << endl
               << "Events: " << cm.get_events() << endl
               << "Input: "  << cm.get_inputs() << endl
               << "Output: " << cm.get_outputs() << endl
               << "States: " << cm.get_states() << endl
               << "Trans "  << cm.get_trans()  << endl;
}


ostream& operator<<(ostream& out, const NetComponent& c)
{
    out << "NetComponent: " << c.get_name()
               << ", model ";
    if(c.get_model() != NULL) out << c.get_model()->get_name();
    else out << "<null>";
    return out;
}



ostream& operator<<(ostream& out, const Component& c)
{
    out << "Component: " << c.get_name()
               << ", model ";
    if(c.get_model() != NULL) out << c.get_model()->get_name();
    else out << "<null>";
    return out;
}


std::ostream& operator<<(std::ostream& out, const Transition& t)
{
    return out /*<< "Transition: "*/ << t.get_name();/* << endl
               << "In event: " << t.input_event << endl
               << "From - To: " << t.s1_s2 << endl
               << "Out events: " << t.out_events << endl;*/
}


std::ostream& operator<<(std::ostream& out, const NetworkModel& net)
{
    return out << "Network model: " << net.get_name() << endl
               << "Components: "   << net.get_components() << endl
               << "Links: " << net.get_links() << endl
               << "Patterns: " << net.get_patterns() << endl
               << "Initial states: " << net.get_initials() << endl
               << "Viewer: " << net.get_viewer() << endl
               << "Ruler: " << net.get_ruler() << endl;
}


std::ostream& operator<<(std::ostream& out, const SystemNode& node)
{
    return out << "Node: " << node.get_name()
               << ", model: " << node.get_net_model()->get_name() << endl
               << "Initial states: " << node.get_initials() << endl
               << "Viewer: " << node.get_viewer() << endl
               << "Ruler: " << node.get_ruler() << endl;
}


std::ostream& operator<<(std::ostream& out, const System& sys)
{
   out << "System: " << sys.get_name() << endl
               << "Nodes: " << sys.node_list;
   out << "Emergence: " << sys.get_emergence() << endl;

   return out;
}


std::ostream& operator<<(std::ostream& out, const ProblemNode& node)
{
    return out << "Node: " << node.get_name() << endl
               << "Ref_node: " << node.get_ref_node()->get_name() << endl
               << "Initial states: " << node.get_initials() << endl
               << "Viewer: " << node.get_viewer() << endl
               << "Ruler: " << node.get_ruler() << endl
               << "Observation: [ " << node.get_observation() << " ]" << endl;
}


std::ostream& operator<<(std::ostream& out, const Problem& probl)
{
    return out << "Problem: " << probl.get_name() << endl
                << "Nodes: " << probl.get_nodes() << endl;
}




std::ostream& operator<<(std::ostream& out, const StateData_str& s)
{
    return out << s.get_state_name();
}


std::ostream& operator<<(std::ostream& out, const StateData_strList& s)
{
    if(s.elements.empty())
        return out;
    std::string str = "{";
    for(std::set<std::string>::iterator it = s.elements.begin(); it != s.elements.end(); it++)
    {
        str.append(*it);
        str.append(",");
    }
    str.resize(str.size()-1);
    str.append("}");

    return out << str;
}

std::ostream& operator<<(std::ostream& out, const NetTransition& t)
{
    return out << t.get_trans()->get_name() << "(" << t.get_component()->get_name() << ")";
}

std::ostream& operator<<(std::ostream& out, const SysTransition& t)
{
    return out << t.get_name();
}


std::ostream& operator<<(std::ostream& out, const Pattern& p)
{
    out << make_pair(p.get_name(),p.get_terminal_id()) << ": ";
    out << p.get_expr();
    out << ", language: " << p.get_language();
    return out;
}


std::ostream& operator<<(std::ostream& out, const Terminal& t)
{
    out << "Terminal " << t.get_name() << " = " << t.get_value();
    out << ", linked terminals: ";
    vector<Terminal*> terms = t.get_linked_terminals();
    vector<Terminal*>::const_iterator it;
    for(it = terms.begin(); it != terms.end(); it++)
            out << **it;
    return out;
}
