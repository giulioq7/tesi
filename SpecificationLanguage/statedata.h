#ifndef STATEDATA_H
#define STATEDATA_H

#include <string>
#include <utility>
#include <astl.h>

class StateData
{
    //hash value
    int wc_;
public:
    std::string state_name;


    StateData();
    StateData(std::string str) { state_name = str; }

    //members for make hash
    StateData& operator=(const astl::statedata_tag&) { return *this; }
    int& wc()       { return wc_; }
    int  wc() const { return wc_; }
    //void operator=(const StateData s) {state_name = s.state_name;}
    bool operator==(const StateData s) {return state_name == s.state_name;}
};

#endif // STATEDATA_H
