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
    std::string str = trans->name;
    str.append("("); str.append(component->name);
    str.append("("); str.append(node->name); str.append("))");
    name = str;
    net_trans = NetTransition(trans,component);
    t_name_c_name = make_pair(trans->name, component->name);
}

bool SysTransition::is_triggerable(vector<string> &E)
{
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

void SysTransition::effects(vector<string> &E)
{
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
