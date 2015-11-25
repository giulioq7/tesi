#include "systransition.h"

SysTransition::SysTransition()
{
}

SysTransition::SysTransition(Transition* t, Component *c, SystemNode *n)
{
    trans = t;
    component = c;
    node = n;
    input_event = make_pair(trans->input_event.first, component->find_terminal(trans->input_event.second));
    vector<pair<std::string,std::string> >::iterator it;
    for(it = trans->out_events.begin(); it!= trans->out_events.end(); it++)
        output_events.push_back(make_pair(it->first,component->find_output_terminal(it->second)));
}

bool SysTransition::is_triggerable()
{
    //verifies that event is available at input terminal of the component
    if(input_event.first != "" && input_event.first != input_event.second->value)
        return false;
    //verifies that all links outgoing from the output terminal involved in the transition are empty (saturation policy: WAIT)
    vector<pair<std::string, OutputTerminal*> >::iterator it;
    for(it = output_events.begin(); it != output_events.end(); it++)
    {
        vector<Terminal*>::iterator it2;
        for(it2 = it->second->linked_terminals.begin(); it2 != it->second->linked_terminals.end(); it2++)
        {
            if((*it2)->value != "<eps>")
                return false;
        }
    }
    return true;
}

void SysTransition::effects()
{
    if(input_event.first != "")
        input_event.second->value = "<eps>";
    vector<pair<std::string, OutputTerminal*> >::iterator it;
    for(it = output_events.begin(); it != output_events.end(); it++)
    {
        vector<Terminal*>::iterator it2;
        for(it2 = it->second->linked_terminals.begin(); it2 != it->second->linked_terminals.end(); it2++)
            (*it2)->value = it->first;
    }
}
