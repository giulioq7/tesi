#include "problemnode.h"

using namespace astl;

ProblemNode::ProblemNode()
{
}


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
            Component *c = ref_node->net_model->find_component(str);
            DFA_map<Transition,StateData_str>::state_type state = c->model->find_state(it->first);
            return state;
        }
    }
    //this return statement should never be reached
    return DFA_map<Transition,StateData_str>::state_type();
}
