#ifndef STATEDATA_H
#define STATEDATA_H

#include <string>
#include <utility>
#include <astl.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The StateData_str class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class StateData_str
{
    /**
     * @brief state_name data info
     */
    std::string state_name;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & state_name;
    }

public:
    /**
     * @brief StateData_str
     */
    StateData_str();
    /**
     * @brief StateData_str
     * @param str
     */
    StateData_str(std::string str) { state_name = str; }
    ~StateData_str(){}

    /**
     * @brief get_state_name
     * @return
     */
    std::string get_state_name() const;

    //bool operator==(const StateData_str s) {return state_name == s.state_name;}
};

/**
 * @brief operator == overload of equality operator
 * @param s1
 * @param s2
 * @return
 */
inline bool operator==(const StateData_str s1, const StateData_str s2)
{
    return s1.get_state_name() == s2.get_state_name();
}

#endif // STATEDATA_H
