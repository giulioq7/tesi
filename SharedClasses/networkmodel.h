#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <map>
#include <sstream>
#include "utils.h"
#include "netcomponent.h"
#include "nettransition.h"
#include "pattern.h"
#include "statedata_strlist.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class NetworkModel
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & components;
        ar & inputs;
        ar & outputs;
        ar & links;
        ar & patterns;
        ar & initials;
        ar & viewer;
        ar & ruler;
        ar & pattern_space;
        ar & count;
        ar & conv_int_str;
        ar & conv_str_int;
    }

public:
    std::string name;
    vector<NetComponent> components;
    vector<std::string> inputs;
    vector<std::string> outputs;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > links;

    vector<Pattern> patterns;
    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;

    vector<astl::DFA_map<NetTransition,StateData_strList> *> pattern_space;

    int count;
    std::map<std::pair<std::string,std::string>,int> conv_str_int;
    std::map<int,std::pair<std::string,std::string> > conv_int_str;

    NetworkModel();
    NetworkModel(std::string str) { name = str; count = 0;}

    std::string not_trans(std::string operand);
    NetComponent* find_component(std::string id);
};

#endif // NETWORKMODEL_H
