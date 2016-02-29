#include "problemnode.h"

using namespace astl;

std::string ProblemNode::get_name() const
{
    return name;
}

SystemNode* ProblemNode::get_ref_node()const
{
    return ref_node;
}

//vector<Component> ProblemNode::get_concrete_components() const
//{
//    return concrete_components;
//}

vector<Terminal*> ProblemNode::get_input_terminals() const
{
    return input_terminals;
}

vector<Terminal*> ProblemNode::get_output_terminals() const
{
    return output_terminals;
}

vector<pair<std::string,std::string> > ProblemNode::get_initials() const
{
    return initials;
}

map<pair<string,string>,string>   ProblemNode::get_viewer() const
{
    return viewer;
}

map<pair<string,string>,string>   ProblemNode::get_ruler() const
{
    return ruler;
}

vector<std::string> ProblemNode::get_observation() const
{
    return observation;
}

astl::DFA_map<astl::strings,StateData_str>* ProblemNode::get_index_space() const
{
    return index_space;
}

vector<int> ProblemNode::get_depends() const
{
    return depends;
}

bool ProblemNode::is_radix()
{
    return radix;
}

map<std::string,Terminal*> ProblemNode::get_patt_map() const
{
    return patt_map;
}

map<std::string,vector<int> > ProblemNode::get_patt_indexes_map() const
{
    return patt_indexes_map;
}

map<std::string,vector<int> > ProblemNode::get_lazy_patt_indexes_map() const
{
    return lazy_patt_indexes_map;
}

void ProblemNode::set_ref_node(SystemNode* sn)
{
    ref_node = sn;
}

void ProblemNode::add_concrete_component(Component comp)
{
    concrete_components.push_back(comp);
}

void ProblemNode::add_input_terminal(Terminal* t)
{
    input_terminals.push_back(t);
}

void ProblemNode::add_output_terminal(Terminal* t)
{
    output_terminals.push_back(t);
}

void ProblemNode::add_initial(std::string state, std::string c)
{
    initials.push_back(make_pair(state,c));
}

void ProblemNode::set_initials(vector<pair<std::string,std::string> > in)
{
    initials = in;
}

void ProblemNode::add_label(pair<string,string> t, std::string l)
{
    viewer[t] = l;
}

void ProblemNode::add_fault(pair<string,string> t, string f)
{
    ruler[t] = f;
}

void ProblemNode::set_viewer(map<pair<string,string>,string> v)
{
    viewer = v;
}

void ProblemNode::set_ruler(map<pair<string,string>,string> r)
{
    ruler = r;
}

void ProblemNode::add_obs_label(string l)
{
    observation.push_back(l);
}

void ProblemNode::set_observation(vector<std::string> obs)
{
    observation = obs;
}

void ProblemNode::set_index_space(astl::DFA_map<astl::strings,StateData_str>* idxsp)
{
    index_space = idxsp;
}

void ProblemNode::add_depend(int i)
{
    depends.push_back(i);
}

void ProblemNode::set_radix()
{
    radix = true;
}

void ProblemNode::set_patt_map(map<std::string,Terminal*> m)
{
    patt_map = m;
}

void ProblemNode::set_patt_indexes_map(map<std::string,vector<int> > m)
{
    patt_indexes_map = m;
}

void ProblemNode::set_lazy_patt_indexes_map(map<std::string,vector<int> > m)
{
    lazy_patt_indexes_map = m;
}

void ProblemNode::add_patt_map(std::string s,Terminal* t)
{
    patt_map[s] = t;
}

void ProblemNode::add_patt_indexes_map(std::string s, vector<int>  is)
{
    patt_indexes_map[s] = is;
}

void ProblemNode::add_lazy_patt_indexes_map(std::string s, vector<int> is)
{
    lazy_patt_indexes_map[s] = is;
}


Component* ProblemNode::find_component(std::string id)
{
    Component* ref = NULL;
    for(vector<Component>::iterator it = concrete_components.begin(); it != concrete_components.end(); it++)
    {
        if((*it).get_name() == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}

DFA_map<Transition,StateData_str>::state_type ProblemNode::find_initial_state(std::string str)
{
    for(vector<pair<std::string,std::string> >::iterator it = initials.begin(); it != initials.end(); it++)
    {
        if(it->second == str)
        {
            NetComponent *c = ref_node->get_net_model()->find_component(str);
            DFA_map<Transition,StateData_str>::state_type state = c->get_model()->find_state(it->first);
            return state;
        }
    }
    //this return statement should never be reached
    return DFA_map<Transition,StateData_str>::state_type();
}

void ProblemNode::make_terminals()
{
    for(vector<Component>::iterator it = concrete_components.begin(); it != concrete_components.end(); it++)
    {
        vector<std::string> inputs = it->get_model()->get_inputs();
        for(vector<std::string>::iterator it2 = inputs.begin(); it2 != inputs.end(); it2++)
        {
            Terminal *t = new Terminal(*it2);
            it->add_input_term(t);
        }
        vector<std::string> outputs = it->get_model()->get_outputs();
        for(vector<std::string>::iterator it2 = outputs.begin(); it2 != outputs.end(); it2++)
        {
            Terminal *t = new Terminal(*it2);
            it->add_output_term(t);
        }
    }
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > links = ref_node->get_net_model()->get_links();
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >::iterator it;
    for(it = links.begin(); it != links.end(); it++)
    {
        Component* c = find_component(it->first.second);
        Terminal *t;
        if(c == NULL)
            t = Utils::findptr_from_id(input_terminals,it->first.first);
        else
            t = c->find_output_terminal(it->first.first);
        t->add_linked_terminal(find_component(it->second.second)->find_input_terminal(it->second.first));
    }
}


int ProblemNode::input_terminals_count()
{
    int count = 0;
    for(vector<Component>::iterator it = concrete_components.begin(); it != concrete_components.end(); it++)
    {
        vector<Terminal*> terms = it->get_input_terminals();
        for(vector<Terminal*>::iterator it2 = terms.begin(); it2 != terms.end(); it2++)
            count++;
    }

    return count;
}
