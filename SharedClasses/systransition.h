#ifndef SYSTRANSITION_H
#define SYSTRANSITION_H

#include <string>
#include "transition.h"
#include "systemnode.h"
#include "terminal.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>

/**
 * @brief The SysTransition class represents a transition in a specific problem node
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class SysTransition : public NetTransition
{
    /**
     * @brief node
     */
    SystemNode* node;
    /**
     * @brief input_event maps input pattern event to linked input terminals in greedy diagnosis
     */
    pair<std::string, int> input_event;
    /**
     * @brief output_events maps output pattern events to linked input terminals in greedy diagnosis
     */
    vector<pair<std::string,vector<int> > > output_events;
    /**
     * @brief input_event maps input pattern event to linked input terminals in lazy diagnosis
     */
    pair<std::string, int> lazy_input_event;
    /**
     * @brief lazy_output_events maps output pattern events to linked input terminals in lazy diagnosis
     */
    vector<pair<std::string,vector<int> > > lazy_output_events;
    /**
     * @brief t_name_c_name transition name and component name
     */
    pair<string, string> t_name_c_name;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<NetTransition>(*this);
        ar & node;
        ar & input_event;
        ar & output_events;
        ar & lazy_input_event;
        ar & lazy_output_events;
        ar & t_name_c_name;
    }
public:
    /**
     * @brief SysTransition
     */
    SysTransition();
    /**
     * @brief SysTransition
     * @param t
     * @param c
     * @param n
     */
    SysTransition(Transition *t, NetComponent* c, SystemNode* n);

    /**
     * @brief get_node
     * @return
     */
    SystemNode* get_node() const;
    /**
     * @brief get_t_name_c_name
     * @return
     */
    pair<string, string> get_t_name_c_name() const;

    /**
     * @brief set_input_event
     * @param e
     * @param index
     */
    void set_input_event(std::string e, int index);
    /**
     * @brief add_output_event
     * @param e
     * @param indexes
     */
    void add_output_event(std::string e, vector<int> indexes);
    /**
     * @brief set_lazy_input_event
     * @param e
     * @param index
     */
    void set_lazy_input_event(std::string e, int index);
    /**
     * @brief add_lazy_output_event
     * @param e
     * @param indexes
     */
    void add_lazy_output_event(std::string e, vector<int> indexes);

    /**
     * @brief is_triggerable checks if current transition is triggerable (saturation policy WAIT)
     * @param E current configuration of input terminals content
     * @param lazy true if diagnosis is in lazy mode, false otherwise
     * @return true of transition is triggerable, false otherwise
     */
    bool is_triggerable(vector<string> &E, bool lazy = false);
    /**
     * @brief effects computes new configuration of input terminals after transition execution
     * @param E input terminals values
     * @param lazy true if diagnosis is in lazy mode, false otherwise
     */
    void effects(vector<string> &E, bool lazy = false);

    //required definitions to use a SysTransition as automata alphabet for astl lib
    /**
     * @brief char_type
     */
    typedef SysTransition char_type;
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
    bool operator<(const SysTransition t) const
    {
        return (name < t.name);
    }
    /**
     * @brief operator ==
     * @param t
     * @return
     */
    bool operator==(const SysTransition t) const
    {
        return (name == t.name);
    }

};

#endif // SYSTRANSITION_H
