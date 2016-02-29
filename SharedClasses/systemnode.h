#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "networkmodel.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class SystemNode
{
    std::string name;
    NetworkModel* net_model;
    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;

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
    SystemNode();
    SystemNode(std::string str) { name = str;}

    std::string get_name() const;
    NetworkModel* get_net_model() const;
    vector<pair<std::string,std::string> > get_initials() const;
    map<pair<string,string>,string>   get_viewer() const;
    map<pair<string,string>,string>   get_ruler() const;

    void set_net_model(NetworkModel* nm);
    void set_initials(vector<pair<std::string,std::string> > ins);
    void set_viewer(map<pair<string,string>,string> vwr);
    void set_ruler(map<pair<string,string>,string> rlr);
};

#endif // NODE_H
