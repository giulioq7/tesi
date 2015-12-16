#ifndef STATEDATA_STRLIST_H
#define STATEDATA_STRLIST_H

#include <string>
#include <utility>
#include <astl.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/set.hpp>

class StateData_strList
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & elements;
    }

public:
    std::set<std::string> elements;

    //StateData_strList();
    StateData_strList(){ }
    StateData_strList(std::set<std::string> set_str);

    void add_elem(std::string str);
    //StateData_strList& operator=(const astl::empty &){ return *this; }
    //bool operator==(const StateData_strList s) {return elements == s.elements;}
    //bool operator!=(const StateData_strList s) {return elements != s.elements;}
};

inline bool operator==(const StateData_strList s1, const StateData_strList s2)
{
    return s1.elements == s2.elements;
}



#endif // STATEDATA_STRLIST_H
