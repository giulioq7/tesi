#include "nettransition.h"

NetTransition::NetTransition()
{
}


NetTransition::NetTransition(Transition* t, NetComponent *c)
{
    trans = t;
    component = c;
    std::string str = t->name; str.append("("); str.append(c->name); str.append(")");
    name = str;
}
