#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "componentmodel.h"

class Component
{
public:
    Component();
    Component(std::string str) { name = str; }

    std::string name;
    ComponentModel* model;
    astl::DFA_map<Transition,StateData> automaton;

     bool operator==(const Component c) const {return name == c.name;}
};


#endif // COMPONENT_H
