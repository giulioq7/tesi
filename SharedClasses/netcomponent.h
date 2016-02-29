#ifndef NETCOMPONENT_H
#define NETCOMPONENT_H

#include <iostream>
#include "componentmodel.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class NetComponent
{
protected:
    std::string name;
    ComponentModel* model;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & model;
    }
public:
    NetComponent();
    NetComponent(std::string str);

    std::string get_name() const;
    ComponentModel* get_model() const;

    void set_model(ComponentModel* cm);

    bool operator==(const NetComponent c) const {return name == c.name;}
};

#endif // NETCOMPONENT_H
