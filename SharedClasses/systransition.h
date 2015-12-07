#ifndef SYSTRANSITION_H
#define SYSTRANSITION_H

#include "transition.h"
#include "component.h"
#include "systemnode.h"
#include "terminal.h"

class SysTransition : public CHAR_TRAITS<SysTransition>
{
public:
    std::string name;
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
        return (name < t.name);
    }
    bool operator==(const SysTransition t) const
    {
        return (name == t.name);
    }

};

#endif // SYSTRANSITION_H
