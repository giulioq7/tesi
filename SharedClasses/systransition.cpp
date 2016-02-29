#include "systransition.h"

SysTransition::SysTransition()
{
    name = "";
}

SysTransition::SysTransition(Transition* t, NetComponent *c, SystemNode *n)
{
    trans = t;
    component = c;
    node = n;
    std::string str = trans->get_name();
    str.append("("); str.append(component->get_name());
    str.append("("); str.append(node->get_name()); str.append("))");
    name = str;
    t_name_c_name = make_pair(trans->get_name(), component->get_name());
}


SystemNode* SysTransition::get_node() const
{
    return node;
}

pair<string, string> SysTransition::get_t_name_c_name() const
{
    return t_name_c_name;
}

void SysTransition::set_input_event(std::string e, int index)
{
    input_event = make_pair(e,index);
}

void SysTransition::add_output_event(std::string e, vector<int> indexes)
{
    output_events.push_back(make_pair(e,indexes));
}

void SysTransition::set_lazy_input_event(std::string e, int index)
{
    lazy_input_event = make_pair(e,index);
}

void SysTransition::add_lazy_output_event(std::string e, vector<int> indexes)
{
    lazy_output_events.push_back(make_pair(e,indexes));
}


bool SysTransition::is_triggerable(vector<string> &E, bool lazy)
{
    if(lazy)
    {
        input_event = lazy_input_event;
        output_events = lazy_output_events;
    }
    //verifies that event is available at input terminal of the component
    if(input_event.first != "" && input_event.first != E[input_event.second])
        return false;
    //verifies that all links outgoing from the output terminal involved in the transition are empty (saturation policy: WAIT)
    vector<pair<std::string, vector<int> > >::iterator it;
    for(it = output_events.begin(); it != output_events.end(); it++)
    {
        vector<int>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            if(E[*it2] != "<eps>")
                return false;
        }
    }
    return true;
}

void SysTransition::effects(vector<string> &E, bool lazy)
{
    if(lazy)
    {
        input_event = lazy_input_event;
        output_events = lazy_output_events;
    }
    if(input_event.first != "")
        E[input_event.second] = "<eps>";
    vector<pair<std::string, vector<int> > >::iterator it;
    for(it = output_events.begin(); it != output_events.end(); it++)
    {
        vector<int>::iterator it2;
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++)
            E[*it2] = it->first;
    }
}
