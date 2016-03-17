#ifndef PROBLEMNODE_H
#define PROBLEMNODE_H

#include <iostream>
#include "systemnode.h"
#include "component.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The ProblemNode class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class ProblemNode
{
    /**
     * @brief name
     */
    std::string name;

    /**
     * @brief ref_node pointer to the relative system node
     */
    SystemNode* ref_node;

    /**
     * @brief input_terminals
     */
    vector<Terminal*> input_terminals;
    /**
     * @brief output_terminals
     */
    vector<Terminal*> output_terminals;
    /**
     * @brief initials
     */
    vector<pair<std::string,std::string> > initials;
    /**
     * @brief viewer
     */
    map<pair<string,string>,string>   viewer;
    /**
     * @brief ruler
     */
    map<pair<string,string>,string>   ruler;
    /**
     * @brief observation
     */
    vector<std::string> observation;

    /**
     * @brief index_space
     */
    astl::DFA_map<astl::strings,StateData_str> *index_space;

    /**
     * @brief depends indexes of nodes  linked to current nodes by pattern event links
     */
    vector<int> depends;
    /**
     * @brief radix true if is a radix node
     */
    bool radix;

    /**
     * @brief patt_map maps pattern event to node output terminal
     */
    map<std::string,Terminal*> patt_map;
    /**
     * @brief patt_indexes_map maps pattern event to indexes of tuple of linked input terminals in greedy diagnosis
     */
    map<std::string,vector<int> > patt_indexes_map;
    /**
     * @brief lazy_patt_indexes_map maps pattern event to indexes of tuple of linked input terminals in lazy diagnosis
     */
    map<std::string,vector<int> > lazy_patt_indexes_map;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & ref_node;
        ar & input_terminals;
        ar & output_terminals;
        ar & concrete_components;
        ar & initials;
        ar & viewer;
        ar & ruler;
        ar & observation;
        ar & index_space;
        ar & depends;
        ar & radix;
        ar & patt_map;
        ar & patt_indexes_map;
        ar & lazy_patt_indexes_map;
    }

public:
    /**
     * @brief ProblemNode default constructor
     */
    ProblemNode(){index_space = new  astl::DFA_map<astl::strings,StateData_str>();}
    /**
     * @brief ProblemNode constructor from id
     * @param str name
     */
    ProblemNode(std::string str){ name = str; radix = false; index_space = new  astl::DFA_map<astl::strings,StateData_str>();}

    /**
     * @brief concrete_components
     */
    vector<Component> concrete_components;
    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_ref_node
     * @return
     */
    SystemNode* get_ref_node()const;

    //vector<Component> get_concrete_components() const;

    /**
     * @brief get_input_terminals
     * @return
     */
    vector<Terminal*> get_input_terminals() const;
    /**
     * @brief get_output_terminals
     * @return
     */
    vector<Terminal*> get_output_terminals() const;
    /**
     * @brief get_initials
     * @return
     */
    vector<pair<std::string,std::string> > get_initials() const;
    /**
     * @brief get_viewer
     * @return
     */
    map<pair<string,string>,string>   get_viewer() const;
    /**
     * @brief get_ruler
     * @return
     */
    map<pair<string,string>,string>   get_ruler() const;
    /**
     * @brief get_observation
     * @return
     */
    vector<std::string> get_observation() const;
    /**
     * @brief get_index_space
     * @return
     */
    astl::DFA_map<astl::strings,StateData_str>* get_index_space() const;
    /**
     * @brief get_depends
     * @return
     */
    vector<int> get_depends() const;
    /**
     * @brief is_radix
     * @return
     */
    bool is_radix();
    /**
     * @brief get_patt_map
     * @return
     */
    map<std::string,Terminal*> get_patt_map() const;
    /**
     * @brief get_patt_indexes_map
     * @return
     */
    map<std::string,vector<int> > get_patt_indexes_map() const;
    /**
     * @brief get_lazy_patt_indexes_map
     * @return
     */
    map<std::string,vector<int> > get_lazy_patt_indexes_map() const;

    /**
     * @brief set_ref_node
     * @param sn
     */
    void set_ref_node(SystemNode* sn);
    /**
     * @brief add_concrete_component
     * @param comp
     */
    void add_concrete_component(Component comp);
    /**
     * @brief add_input_terminal
     * @param t
     */
    void add_input_terminal(Terminal* t);
    /**
     * @brief add_output_terminal
     * @param t
     */
    void add_output_terminal(Terminal* t);
    /**
     * @brief add_initial
     * @param state
     * @param c
     */
    void add_initial(std::string state, std::string c);
    /**
     * @brief set_initials
     * @param in
     */
    void set_initials(vector<pair<std::string,std::string> > in);
    /**
     * @brief add_label
     * @param t
     * @param l
     */
    void add_label(pair<string,string> t, std::string l);
    /**
     * @brief add_fault
     * @param t
     * @param f
     */
    void add_fault(pair<string,string> t, std::string f);
    /**
     * @brief set_viewer
     * @param v
     */
    void set_viewer(map<pair<string,string>,string> v);
    /**
     * @brief set_ruler
     * @param r
     */
    void set_ruler(map<pair<string,string>,string> r);
    /**
     * @brief add_obs_label
     * @param l
     */
    void add_obs_label(std::string l);
    /**
     * @brief set_observation
     * @param obs
     */
    void set_observation(vector<std::string> obs);
    /**
     * @brief set_index_space
     * @param idxsp
     */
    void set_index_space(astl::DFA_map<astl::strings,StateData_str>* idxsp);
    /**
     * @brief add_depend
     * @param i
     */
    void add_depend(int i);
    /**
     * @brief set_radix
     */
    void set_radix();
    /**
     * @brief set_patt_map
     * @param m
     */
    void set_patt_map(map<std::string,Terminal*> m);
    /**
     * @brief set_patt_indexes_map
     * @param m
     */
    void set_patt_indexes_map(map<std::string,vector<int> > m);
    /**
     * @brief set_lazy_patt_indexes_map
     * @param m
     */
    void set_lazy_patt_indexes_map(map<std::string,vector<int> > m);
    /**
     * @brief add_patt_map
     * @param s
     * @param t
     */
    void add_patt_map(std::string s,Terminal* t);
    /**
     * @brief add_patt_indexes_map
     * @param s
     * @param is
     */
    void add_patt_indexes_map(std::string s, vector<int>  is);
    /**
     * @brief add_lazy_patt_indexes_map
     * @param s
     * @param is
     */
    void add_lazy_patt_indexes_map(std::string s, vector<int> is);


    /**
     * @brief find_component find component from id
     * @param id
     * @return pointer to Component
     */
    Component* find_component(std::string id);
    /**
     * @brief make_terminals build concrete terminals of components, based on relative model
     */
    void make_terminals();
    /**
     * @brief input_terminals_count counts total inpute terminal of components
     * @return
     */
    int input_terminals_count();
    /**
     * @brief find_initial_state find initial state of a component
     * @param str component name
     * @return automaton state
     */
    astl::DFA_map<Transition,StateData_str>::state_type find_initial_state(std::string str);
};

#endif // PROBLEMNODE_H
