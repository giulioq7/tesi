#ifndef SYSTRANSITION_H
#define SYSTRANSITION_H

#include "transition.h"
#include "systemnode.h"
#include "terminal.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class SysTransition : public CHAR_TRAITS<SysTransition>
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & trans;
        ar & component;
        ar & node;
        ar & input_event;
        ar & output_events;
        ar & lazy_input_event;
        ar & lazy_output_events;
        ar & net_trans;
        ar & t_name_c_name;
    }
public:
    std::string name;
    Transition* trans;
    NetComponent* component;
    SystemNode* node;
    pair<std::string, int> input_event;
    vector<pair<std::string,vector<int> > > output_events;
    pair<std::string, int> lazy_input_event;
    vector<pair<std::string,vector<int> > > lazy_output_events;
    NetTransition net_trans;
    pair<string, string> t_name_c_name;


    SysTransition();
    SysTransition(Transition *t, NetComponent* c, SystemNode* n);

    bool is_triggerable(vector<string> &E, bool lazy = false);
    void effects(vector<string> &E, bool lazy = false);

    //required definitions to use a SysTransition as automata alphabet for astl lib
    typedef SysTransition char_type;
    typedef long        int_type;
    static const size_t size;
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    bool operator<(const SysTransition t) const
    {
        return (name < t.name);
    }
    bool operator==(const SysTransition t) const
    {
        return (name == t.name);
    }

};

#endif // SYSTRANSITION_H
