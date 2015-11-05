#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "networkmodel.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class SystemNode
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & net_model;
        ar & initials;
        ar & viewer;
        ar & ruler;
    }

public:
    std::string name;

    NetworkModel* net_model;

    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;

    SystemNode();
    SystemNode(std::string str) { name = str;}
};

#endif // NODE_H
