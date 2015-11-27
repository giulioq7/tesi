#ifndef SYSTRANSITION_H
#define SYSTRANSITION_H

#include "transition.h"
#include "component.h"
#include "systemnode.h"
#include "terminal.h"

class SysTransition : public CHAR_TRAITS<SysTransition>
{
public:
    Transition* trans;
    Component* component;
    SystemNode* node;
    pair<std::string, Terminal*> input_event;
    vector<pair<std::string,Terminal*> > output_events;

    SysTransition();
    SysTransition(Transition *t, Component* c, SystemNode* n);

    bool is_triggerable();
    void effects();

    //required definitions to use a SysTransition as automata alphabet for astl lib
    typedef SysTransition char_type;
    typedef long        int_type;
    static const size_t size;
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    bool operator<(const SysTransition t) const
    {
        std::string str1 = trans->name; str1.append("("); str1.append(component->name); str1.append("("); str1.append(node->name); str1.append("))");
        std::string str2 = t.trans->name; str2.append("("); str2.append(t.component->name); str2.append("("); str2.append(t.node->name); str2.append("))");
        return (str1 < str2);
    }
    bool operator==(const SysTransition t) const
    {
        std::string str1 = trans->name; str1.append("("); str1.append(component->name); str1.append("("); str1.append(node->name); str1.append("))");
        std::string str2 = t.trans->name; str2.append("("); str2.append(t.component->name); str2.append("("); str2.append(t.node->name); str2.append("))");
        return (str1 == str2);
    }

};

#endif // SYSTRANSITION_H
