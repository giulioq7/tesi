#ifndef STATEDATA_STRLIST_H
#define STATEDATA_STRLIST_H

#include <string>
#include <utility>
#include <astl.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/set.hpp>

/**
 * @brief The StateData_strList class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class StateData_strList
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & elements;
    }

public:
    /**
     * @brief elements data info
     */
    std::set<std::string> elements;

    //StateData_strList();
    /**
     * @brief StateData_strList
     */
    StateData_strList(){ }
    /**
     * @brief StateData_strList
     * @param set_str
     */
    StateData_strList(std::set<std::string> set_str);

    /**
     * @brief add_elem
     * @param str
     */
    void add_elem(std::string str);
    //StateData_strList& operator=(const astl::empty &){ return *this; }
    //bool operator==(const StateData_strList s) {return elements == s.elements;}
    //bool operator!=(const StateData_strList s) {return elements != s.elements;}
};

/**
 * @brief operator ==
 * @param s1
 * @param s2
 * @return
 */
inline bool operator==(const StateData_strList s1, const StateData_strList s2)
{
    return s1.elements == s2.elements;
}



#endif // STATEDATA_STRLIST_H
