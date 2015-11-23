#ifndef PROBLEM_H
#define PROBLEM_H

#include "problemnode.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Problem
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & nodes;
    }

public:
    std::string name;
    vector<ProblemNode> nodes;


    Problem();
    Problem(std::string str) { name = str;}

    int concrete_components_count();
    int input_terminals_count();
    ProblemNode *find_problem_node(std::string id);

};

#endif // PROBLEM_H
