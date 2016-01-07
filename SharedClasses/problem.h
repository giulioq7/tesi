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
        ar & topological_order;
    }

public:
    std::string name;
    vector<ProblemNode> nodes;
    vector<int> topological_order;


    Problem();
    Problem(std::string str) { name = str;}

    ProblemNode* find_node(std::string id);
    int find_index(std::string id);

    int concrete_components_count();
    int input_terminals_count();

};

#endif // PROBLEM_H
