#include "ostream_util.h"

using namespace std;

ostream& operator<<(ostream& out, const ComponentModel& cm)
{
    return out << "Component model: " << cm.name << endl
               << "Events: " << cm.events << endl
               << "Input: "  << cm.inputs << endl
               << "Output: " << cm.outputs << endl
               << "States: " << cm.states << endl
               << "Trans "  << cm.trans  << endl;
}


ostream& operator<<(ostream& out, const NetComponent& c)
{
    out << "NetComponent: " << c.name
               << ", model ";
    if(c.model != NULL) out << c.model->name;
    else out << "<null>";
    return out;
}



ostream& operator<<(ostream& out, const Component& c)
{
    out << "Component: " << c.name
               << ", model ";
    if(c.model != NULL) out << c.model->name;
    else out << "<null>";
    return out;
}


std::ostream& operator<<(std::ostream& out, const Transition& t)
{
    return out /*<< "Transition: "*/ << t.name << endl
               << "In event: " << t.input_event << endl
               << "From - To: " << t.s1_s2 << endl
               << "Out events: " << t.out_events << endl;
}


std::ostream& operator<<(std::ostream& out, const NetworkModel& net)
{
    return out << "Network model: " << net.name << endl
               << "Components: "   << net.components << endl
               << "Links: " << net.links << endl
               << "Patterns: " << net.patterns << endl
               << "Initial states: " << net.initials << endl
               << "Viewer: " << net.viewer << endl
               << "Ruler: " << net.ruler << endl;
}


std::ostream& operator<<(std::ostream& out, const SystemNode& node)
{
    return out << "Node: " << node.name
               << ", model: " << node.net_model->name << endl
               << "Initial states: " << node.initials << endl
               << "Viewer: " << node.viewer << endl
               << "Ruler: " << node.ruler << endl;
}


std::ostream& operator<<(std::ostream& out, const System& sys)
{
   out << "System: " << sys.name << endl
               << "Nodes: " << sys.node_list;
   out << "Emergence: " << sys.emergence << endl;

   return out;
}


std::ostream& operator<<(std::ostream& out, const ProblemNode& node)
{
    return out << "Node: " << node.name << endl
               << "Ref_node: " << node.ref_node->name << endl
               << "Initial states: " << node.initials << endl
               << "Viewer: " << node.viewer << endl
               << "Ruler: " << node.ruler << endl
               << "Observation: [ " << node.observation << " ]" << endl;
}


std::ostream& operator<<(std::ostream& out, const Problem& probl)
{
    return out << "Problem: " << probl.name << endl
                << "Nodes: " << probl.nodes << endl;
}




std::ostream& operator<<(std::ostream& out, const StateData_str& s)
{
    return out << s.state_name;
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
    return out << t.trans->name << "(" << t.component->name << ")";
}

std::ostream& operator<<(std::ostream& out, const SysTransition& t)
{
    return out << t.name;
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
    out << "Terminal " << t.name << " = " << t.value;
    out << ", linked terminals: ";
    vector<Terminal*>::const_iterator it;
    for(it = t.linked_terminals.begin(); it != t.linked_terminals.end(); it++)
            out << **it;
    return out;
}
