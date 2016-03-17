#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include "systemnode.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The System class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class System
{
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief dependency_graph automaton representing dependencies between nodes (pattern events links)
     */
    astl::DFA_map<astl::strings,StateData_str>* dependency_graph;
    /**
     * @brief emergence list of links between nodes
     */
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  emergence;
    /**
     * @brief acyclic true if system dependency graph is acyclic (and lazy diagnosis is possible)
     */
    bool acyclic;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & dependency_graph;
        ar & acyclic;
        ar & node_list;
        ar & emergence;
    }

public:
    /**
     * @brief System default constructor
     */
    System();
    /**
     * @brief System constructor from id
     * @param str name
     */
    System(std::string str);

    /**
     * @brief node_list system nodes
     */
    vector<SystemNode> node_list;
    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_dependency_graph
     * @return
     */
    astl::DFA_map<astl::strings,StateData_str>* get_dependency_graph() const;
    /**
     * @brief get_emergence
     * @return
     */
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > get_emergence() const;

    /**
     * @brief build_dependency_graph
     */
    void build_dependency_graph();
    /**
     * @brief add_node
     * @param sn
     */
    void add_node(SystemNode sn);
    /**
     * @brief add_emergence
     * @param e
     */
    void add_emergence(pair<pair<std::string,std::string>,pair<std::string,std::string> >  e);
    /**
     * @brief set_node_list
     * @param nl
     */
    void set_node_list(vector<SystemNode> nl);
    /**
     * @brief set_emergence
     * @param emer
     */
    void set_emergence(vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  emer);

    /**
     * @brief set_dependency_graph
     * @param dg
     */
    void set_dependency_graph(astl::DFA_map<astl::strings,StateData_str>* dg);
    /**
     * @brief is_acyclic
     * @return
     */
    bool is_acyclic() const;
    /**
     * @brief set_acyclic
     */
    void set_acyclic();

    /**
     * @brief find_node finds node from id
     * @return pointer to SystemNode
     */
    SystemNode* find_node(std::string);
    /**
     * @brief pts_count counts number of pattern spaces
     * @return
     */
    int pts_count();
};

#endif // SYSTEM_H
