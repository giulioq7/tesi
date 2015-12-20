#include "component.h"

using namespace astl;

Component::Component()
{
    model = NULL;
    //automaton = new astl::DFA_map<Transition,StateData_str>();
}

Terminal* Component::find_input_terminal(std::string id)
{
    Terminal* ref = NULL;
    for(vector<Terminal*>::iterator it = input_terminals.begin(); it != input_terminals.end(); it++)
    {
        if((*it)->name == id)
        {
            ref = *it;
            break;
        }
    }
    return ref;
}

Terminal* Component::find_output_terminal(std::string id)
{
    Terminal* ref = NULL;
    for(vector<Terminal*>::iterator it = output_terminals.begin(); it != output_terminals.end(); it++)
    {
        if((*it)->name == id)
        {
            ref = *it;
            break;
        }
    }
    return ref;
}
