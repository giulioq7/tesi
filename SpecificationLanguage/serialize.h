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

//namespace boost {
//namespace serialization {

//template<class Archive>
//void serialize(Archive & ar,vector<ComponentModel> & vec, const unsigned int version)
//{
//    for(vector<ComponentModel>::iterator it = vec.begin(); it != vec.end(); it++)
//        ar & (*it);

//}

//template<class Archive>
//void serialize(Archive & ar,vector<NetworkModel> & vec, const unsigned int version)
//{
//    for(vector<NetworkModel>::iterator it = vec.begin(); it != vec.end(); it++)
//        ar & (*it);

//}

//template<class Archive>
//void serialize(Archive & ar,vector<ProblemNode> & vec, const unsigned int version)
//{
//    for(vector<ProblemNode>::iterator it = vec.begin(); it != vec.end(); it++)
//        ar & (*it);

//}


//template<class Archive>
//void serialize(Archive & ar,vector<Terminal> & vec, const unsigned int version)
//{
//    for(vector<Terminal>::iterator it = vec.begin(); it != vec.end(); it++)
//        ar & (*it);

//}


//} // namespace serialization
//} // namespace boost

#endif // SERIALIZE_H
