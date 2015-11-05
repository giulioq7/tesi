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
        ar & wc_;
        ar & state_name;
    }

    //hash value
    int wc_;
public:
    std::string state_name;


    StateData_str();
    StateData_str(std::string str) { state_name = str; }

    //members for make hash
    StateData_str& operator=(const astl::statedata_tag&) { return *this; }
    int& wc()       { return wc_; }
    int  wc() const { return wc_; }
    //void operator=(const StateData s) {state_name = s.state_name;}
    bool operator==(const StateData_str s) {return state_name == s.state_name;}
};

#endif // STATEDATA_H
