#ifndef TRANSITION_H
#define TRANSITION_H

#include <astl.h>
#include <string>
#include <utility>
#include <vector>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

class Transition : public CHAR_TRAITS<Transition>
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & input_event;
        ar & name;
        ar & out_events;
        ar & s1_s2;
    }
public:

    std::string name;
    std::pair<std::string,std::string> input_event;
    std::pair<std::string,std::string> s1_s2;
    std::vector<std::pair<std::string,std::string> > out_events;

    //required definitions to use a Transition as automata alphabet for astl lib
    typedef Transition char_type;
    typedef long        int_type;
    static const size_t size;
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    bool operator<(const Transition t) const {return name<t.name;}
    bool operator==(const Transition t) const {return name == t.name;}

    Transition();
    ~Transition();
};



#endif // TRANSITION_H
