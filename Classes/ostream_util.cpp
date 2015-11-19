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


ostream& operator<<(ostream& out, const Component& c)
{
    out << "Component: " << c.name
               << ", model ";
    if(c.model != NULL) out << c.model->name;
    else out << "<null>";
    return out;
}


template <class T>
ostream& operator<<(ostream& out, const vector<T>& l)
{
    for (typename vector<T>::const_iterator it = l.begin(); it != l.end(); ++it)
    {
        out << *it << " ";
    }

    if(l.empty())
        out << "<empty>";

    return out;
}


template <class T1,class T2>
std::ostream& operator<<(std::ostream& out, const pair<T1,T2>& p)
{
    return out << "(" << p.first << " - " << p.second << ")";
}


template <class T1,class T2>
std::ostream& operator<<(std::ostream& out, const map<T1,T2> m)
{
    for(typename map<T1,T2>::const_iterator it=m.begin();it!=m.end();++it)
        out << it->first << " -> " << it->second << " ";
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
               << "Nodes: " << sys.node_list
               << "Root: ";
   if(sys.root != NULL)
       out << sys.root->name << endl;
   else
       out << "<none>" << endl;

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


std::ostream& operator<<(std::ostream& out, const NetTransition& t)
{
    return out << t.trans->name << "(" << t.component->name << ")";
}

std::ostream& operator<<(std::ostream& out, const SysTransition& t)
{
    return out << t.trans->name << "(" << t.component->name << "(" << t.node->name << "))";
}


std::ostream& operator<<(std::ostream& out, const BehaviorState& s)
{
    out << "[";
    for(int i=0; i<s.n_comps-1; i++)
    {
        out << s.S[i] << ",";
    }
    out << s.S[s.n_comps-1];
    out << "]";
    out << endl;
    out << "[";
    for(int i=0; i<s.n_inputs-1; i++)
    {
        out << s.E[i] << ",";
    }
    out << s.E[s.n_inputs-1];
    out << "]" << endl;
    out << "[";
    for(int i=0; i<s.n_complex_inputs-1; i++)
    {
        out << s.Complex_E[i] << ",";
    }
    out << s.Complex_E[s.n_complex_inputs-1];
    out << "]" << endl;
    out << "[";
    for(int i=0; i<s.n_pts-1; i++)
    {
        out << s.P[i].src() << ",";
    }
    out << s.P[s.n_pts-1].src();
    out << "]" << endl;
    out << "[";
    for(int i=0; i<s.n_isp-1; i++)
    {
        out << s.I[i].src() << ",";
    }
    out << s.I[s.n_isp-1].src();
    out << "]";

    return out;
}
