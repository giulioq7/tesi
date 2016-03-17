#ifndef PROBLEM_H
#define PROBLEM_H

#include "problemnode.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The Problem class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class Problem
{
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief topological_order indexes of nodes in topological order
     */
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
    /**
     * @brief Problem default constructor
     */
    Problem();
    /**
     * @brief Problem constructor from id
     * @param str name
     */
    Problem(std::string str) { name = str;}

    /**
     * @brief nodes
     */
    vector<ProblemNode> nodes;

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_nodes
     * @return
     */
    vector<ProblemNode> get_nodes() const;
    /**
     * @brief get_topological_order
     * @return
     */
    vector<int> get_topological_order() const;

    /**
     * @brief add_node
     * @param pn
     */
    void add_node(ProblemNode pn);
    /**
     * @brief set_nodes
     * @param v
     */
    void set_nodes(vector<ProblemNode> v);
    /**
     * @brief set_topological_order
     * @param order
     */
    void set_topological_order(vector<int> order);


    /**
     * @brief find_node find problem node from id
     * @param id
     * @return pointer to ProblemNode
     */
    ProblemNode* find_node(std::string id);
    /**
     * @brief find_index find index of problem node from id
     * @param id
     * @return integer index
     */
    int find_index(std::string id);

    /**
     * @brief concrete_components_count counts all components in all nodes
     * @return
     */
    int concrete_components_count();
    /**
     * @brief input_terminals_count counts all terminals of components of nodes
     * @return
     */
    int input_terminals_count();

};

#endif // PROBLEM_H
