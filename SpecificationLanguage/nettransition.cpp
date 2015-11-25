#include "nettransition.h"

NetTransition::NetTransition(Transition* t, Component* c)
{
    trans = t;
    component = c;
    std::string str = t->name; str.append("("); str.append(c->name); str.append("))");
}
