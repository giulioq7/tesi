#include "component.h"

using namespace astl;

Component::Component()
{
    set_model(NULL);
    //automaton = new astl::DFA_map<Transition,StateData_str>();
}

astl::DFA_map<SysTransition,StateData_str>* Component::get_automaton() const
{
    return automaton;
}

vector<Terminal*> Component::get_input_terminals() const
{
    return input_terminals;
}

vector<Terminal*> Component::get_output_terminals() const
{
    return output_terminals;
}

void Component::set_automaton(astl::DFA_map<SysTransition,StateData_str>* autom)
{
    automaton = autom;
}

void Component::add_input_term(Terminal* t)
{
    input_terminals.push_back(t);
}

void Component::add_output_term(Terminal* t)
{
    output_terminals.push_back(t);
}

Terminal* Component::find_input_terminal(std::string id)
{
    Terminal* ref = NULL;
    for(vector<Terminal*>::iterator it = input_terminals.begin(); it != input_terminals.end(); it++)
    {
        if((*it)->get_name() == id)
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
        if((*it)->get_name() == id)
        {
            ref = *it;
            break;
        }
    }
    return ref;
}
