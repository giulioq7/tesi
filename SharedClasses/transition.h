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
    std::string name;
    std::pair<std::string,std::string> input_event;
    std::pair<std::string,std::string> s1_s2;
    std::vector<std::pair<std::string,std::string> > out_events;

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
    Transition();
    Transition(std::string n){name = n;}
    ~Transition();

    std::string get_name() const;
    std::pair<std::string,std::string> get_input_event() const;
    std::pair<std::string,std::string> get_s1_s2() const;
    std::vector<std::pair<std::string,std::string> > get_out_events() const;
    void set_input_event(std::string e,std::string t);
    void set_s1_s2(std::string s1,std::string s2);
    void add_out_event(std::string e,std::string t);
    void set_out_events(std::vector<std::pair<std::string,std::string> > evs);

    //required definitions to use a Transition as automata alphabet for astl lib
    typedef Transition char_type;
    typedef long        int_type;
    static const size_t size;
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    bool operator<(const Transition t) const {return name<t.name;}
    bool operator==(const Transition t) const {return name == t.name;}
};



#endif // TRANSITION_H
