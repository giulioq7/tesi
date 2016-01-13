#include "problemnode.h"

using namespace astl;



Component* ProblemNode::find_component(std::string id)
{
    Component* ref = NULL;
    for(vector<Component>::iterator it = concrete_components.begin(); it != concrete_components.end(); it++)
    {
        if((*it).name == id)
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
            NetComponent *c = ref_node->net_model->find_component(str);
            DFA_map<Transition,StateData_str>::state_type state = c->model->find_state(it->first);
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
        for(vector<std::string>::iterator it2 = it->model->inputs.begin(); it2 != it->model->inputs.end(); it2++)
        {
            Terminal *t = new Terminal(*it2);
            it->input_terminals.push_back(t);
        }
        for(vector<std::string>::iterator it2 = it->model->outputs.begin(); it2 != it->model->outputs.end(); it2++)
        {
            Terminal *t = new Terminal(*it2);
            it->output_terminals.push_back(t);
        }
    }
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >::iterator it;
    for(it = ref_node->net_model->links.begin(); it != ref_node->net_model->links.end(); it++)
    {
        Component* c = find_component(it->first.second);
        Terminal *t;
        if(c == NULL)
            t = Utils::findptr_from_id(input_terminals,it->first.first);
        else
            t = c->find_output_terminal(it->first.first);
        t->linked_terminals.push_back(find_component(it->second.second)->find_input_terminal(it->second.first));
    }
}


int ProblemNode::input_terminals_count()
{
    int count = 0;
    for(vector<Component>::iterator it = concrete_components.begin(); it != concrete_components.end(); it++)
    {
        for(vector<Terminal*>::iterator it2 = it->input_terminals.begin(); it2 != it->input_terminals.end(); it2++)
            count++;
    }

    return count;
}
