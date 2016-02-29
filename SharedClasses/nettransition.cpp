#include "nettransition.h"

NetTransition::NetTransition()
{
}


NetTransition::NetTransition(Transition* t, NetComponent *c)
{
    trans = t;
    component = c;
    std::string str = t->get_name(); str.append("("); str.append(c->get_name()); str.append(")");
    name = str;
}

std::string NetTransition::get_name() const
{
    return name;
}

Transition* NetTransition::get_trans() const
{
    return trans;
}

NetComponent* NetTransition::get_component() const
{
    return component;
}
