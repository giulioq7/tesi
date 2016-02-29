#ifndef PROBLEM_H
#define PROBLEM_H

#include "problemnode.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Problem
{
    std::string name;
    vector<int> topological_order;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & nodes;
        ar & topological_order;
    }

public:
    Problem();
    Problem(std::string str) { name = str;}

    vector<ProblemNode> nodes;

    std::string get_name() const;
    vector<ProblemNode> get_nodes() const;
    vector<int> get_topological_order() const;

    void add_node(ProblemNode pn);
    void set_nodes(vector<ProblemNode> v);
    void set_topological_order(vector<int> order);


    ProblemNode* find_node(std::string id);
    int find_index(std::string id);

    int concrete_components_count();
    int input_terminals_count();

};

#endif // PROBLEM_H
