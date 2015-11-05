#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include "componentmodel.h"
#include "transition.h"

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, Transition & trans, const unsigned int version)
{
    ar & trans.input_event;
    ar & trans.name;
    ar & trans.out_events;
    ar & trans.s1_s2;
}

//template<class Archive>
//void serialize(Archive & ar, astl::DFA_map<Transition,StateData> & dfa, const unsigned int version)
//{
//    //ar & dfa.;
//}

template<class Archive>
void serialize(Archive & ar, ComponentModel & cm, const unsigned int version)
{
    ar & cm.events;
    ar & cm.inputs;
    ar & cm.name;
    ar & cm.outputs;
    ar & cm.states;
    ar & cm.trans;
    ar & cm.automaton;
}

} // namespace serialization
} // namespace boost

#endif // SERIALIZE_H
