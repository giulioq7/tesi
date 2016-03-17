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

/**
 * @brief The Transition class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class Transition : public CHAR_TRAITS<Transition>
{
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief input_event pair event-input terminal
     */
    std::pair<std::string,std::string> input_event;
    /**
     * @brief s1_s2 origin state-aim state
     */
    std::pair<std::string,std::string> s1_s2;
    /**
     * @brief out_events vector of pairs event-output terminal
     */
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
    /**
     * @brief Transition
     */
    Transition();
    /**
     * @brief Transition
     * @param n
     */
    Transition(std::string n){name = n;}
    ~Transition();

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_input_event
     * @return
     */
    std::pair<std::string,std::string> get_input_event() const;
    /**
     * @brief get_s1_s2
     * @return
     */
    std::pair<std::string,std::string> get_s1_s2() const;
    /**
     * @brief get_out_events
     * @return
     */
    std::vector<std::pair<std::string,std::string> > get_out_events() const;
    /**
     * @brief set_input_event
     * @param e
     * @param t
     */
    void set_input_event(std::string e,std::string t);
    /**
     * @brief set_s1_s2
     * @param s1
     * @param s2
     */
    void set_s1_s2(std::string s1,std::string s2);
    /**
     * @brief add_out_event
     * @param e
     * @param t
     */
    void add_out_event(std::string e,std::string t);
    /**
     * @brief set_out_events
     * @param evs
     */
    void set_out_events(std::vector<std::pair<std::string,std::string> > evs);

    //required definitions to use a Transition as automata alphabet for astl lib
    /**
     * @brief char_type
     */
    typedef Transition char_type;
    /**
     * @brief int_type
     */
    typedef long        int_type;
    /**
     * @brief size
     */
    static const size_t size;
    /**
     * @brief eq
     * @param x
     * @param y
     * @return
     */
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    /**
     * @brief lt
     * @param x
     * @param y
     * @return
     */
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    /**
     * @brief operator <
     * @param t
     * @return
     */
    bool operator<(const Transition t) const {return name<t.name;}
    /**
     * @brief operator ==
     * @param t
     * @return
     */
    bool operator==(const Transition t) const {return name == t.name;}
};



#endif // TRANSITION_H
