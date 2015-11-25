#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include "componentmodel.h"
#include "networkmodel.h"
#include "problem.h"
#include "system.h"

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar,vector<ComponentModel> & vec, const unsigned int version)
{
    for(vector<ComponentModel>::iterator it = vec.begin(); it != vec.end(); it++)
        ar & (*it);

}

template<class Archive>
void serialize(Archive & ar,vector<NetworkModel> & vec, const unsigned int version)
{
    for(vector<NetworkModel>::iterator it = vec.begin(); it != vec.end(); it++)
        ar & (*it);

}

template<class Archive>
void serialize(Archive & ar,vector<ProblemNode> & vec, const unsigned int version)
{
    for(vector<ProblemNode>::iterator it = vec.begin(); it != vec.end(); it++)
        ar & (*it);

}


//template<class Archive>
//void serialize(Archive & ar,vector<Terminal> & vec, const unsigned int version)
//{
//    for(vector<Terminal>::iterator it = vec.begin(); it != vec.end(); it++)
//        ar & (*it);

//}

//template<class Archive>
//inline void save_construct_data(Archive & ar, const ProblemNode  n, const unsigned int file_version)
//{
//    // save data required to construct instance
//    ar << n.name;
//}

//template<class Archive>
//inline void load_construct_data(Archive & ar, ProblemNode n, const unsigned int file_version)
//{
//    // retrieve data from archive required to construct new instance
//    std::string name;
//    ar >> name;
//    // invoke inplace constructor to initialize instance of my_class
//    ::new(&n)ProblemNode(name);
//}


} // namespace serialization
} // namespace boost

#endif // SERIALIZE_H
