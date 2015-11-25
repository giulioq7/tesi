#ifndef NETTRANSITION_H
#define NETTRANSITION_H

#include "transition.h"
#include "component.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class NetTransition : public CHAR_TRAITS<NetTransition>
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & trans;
        ar & component;
        ar & name;
    }

public:
    Transition* trans;
    Component* component;
    std::string name;

    NetTransition(){}
    NetTransition(Transition* t, Component* c);

    //required definitions to use a NetTransition as automata alphabet for astl lib
    typedef NetTransition char_type;
    typedef long        int_type;
    static const size_t size;
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    bool operator<(const NetTransition t) const
    {
       return (name < t.name);
    }
    bool operator==(const NetTransition t) const
    {
        return (name == t.name);
    }

};

#endif // NETTRANSITION_H
