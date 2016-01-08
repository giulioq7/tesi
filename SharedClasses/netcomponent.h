#ifndef NETCOMPONENT_H
#define NETCOMPONENT_H

#include <iostream>
#include "componentmodel.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class NetComponent
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & model;
    }
public:
    std::string name;
    ComponentModel* model;

    NetComponent();
    NetComponent(std::string str);

    bool operator==(const NetComponent c) const {return name == c.name;}
};

#endif // NETCOMPONENT_H
