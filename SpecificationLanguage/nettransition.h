#ifndef NETTRANSITION_H
#define NETTRANSITION_H

#include "transition.h"
#include "component.h"

class NetTransition
{
public:
    Transition* trans;
    Component* component;

    NetTransition();

    //required definitions to use a NetTransition as automata alphabet for astl lib
    typedef NetTransition char_type;
    typedef long        int_type;
    static const size_t size;
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    bool operator<(const NetTransition t) const
    {
        std::string str1 = trans->name; str1.append("("); str1.append(component->name); str1.append(")");
        std::string str2 = t.trans->name; str2.append("("); str2.append(t.component->name); str2.append(")");
        return (str1 < str2);
    }
    bool operator==(const NetTransition t) const
    {
        std::string str1 = trans->name; str1.append("("); str1.append(component->name); str1.append(")");
        std::string str2 = t.trans->name; str2.append("("); str2.append(t.component->name); str2.append(")");
        return (str1 == str2);
    }

};

#endif // NETTRANSITION_H
