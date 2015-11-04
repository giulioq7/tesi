#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "componentmodel.h"

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, astl::DFA_map<Transition,StateData> & dfa, const unsigned int version)
{

}

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
