#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "componentmodel.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Component
{

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & model;
        ar & automaton;
    }

public:
    Component();
    Component(std::string str) { name = str; }

    std::string name;
    ComponentModel* model;
    astl::DFA_map<Transition,StateData_str> automaton;

     bool operator==(const Component c) const {return name == c.name;}
};


#endif // COMPONENT_H
