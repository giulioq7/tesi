#ifndef STATEDATA_H
#define STATEDATA_H

#include <string>
#include <utility>
#include <astl.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class StateData_str
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & state_name;
    }

public:
    std::string state_name;


    StateData_str();
    StateData_str(std::string str) { state_name = str; }
    ~StateData_str(){}

    //bool operator==(const StateData_str s) {return state_name == s.state_name;}
};

inline bool operator==(const StateData_str s1, const StateData_str s2)
{
    return s1.state_name == s2.state_name;
}

#endif // STATEDATA_H
