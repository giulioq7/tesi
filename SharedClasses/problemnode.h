#ifndef PROBLEMNODE_H
#define PROBLEMNODE_H

#include <iostream>
#include "systemnode.h"
#include "component.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class ProblemNode
{
    std::string name;

    SystemNode* ref_node;

    vector<Terminal*> input_terminals;
    vector<Terminal*> output_terminals;
    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;
    vector<std::string> observation;

    astl::DFA_map<astl::strings,StateData_str> *index_space;

    vector<int> depends;
    bool radix;

    map<std::string,Terminal*> patt_map;
    map<std::string,vector<int> > patt_indexes_map;
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
    ProblemNode(){index_space = new  astl::DFA_map<astl::strings,StateData_str>();}
    ProblemNode(std::string str){ name = str; radix = false; index_space = new  astl::DFA_map<astl::strings,StateData_str>();}

    vector<Component> concrete_components;
    std::string get_name() const;
    SystemNode* get_ref_node()const;
    //vector<Component> get_concrete_components() const;
    vector<Terminal*> get_input_terminals() const;
    vector<Terminal*> get_output_terminals() const;
    vector<pair<std::string,std::string> > get_initials() const;
    map<pair<string,string>,string>   get_viewer() const;
    map<pair<string,string>,string>   get_ruler() const;
    vector<std::string> get_observation() const;
    astl::DFA_map<astl::strings,StateData_str>* get_index_space() const;
    vector<int> get_depends() const;
    bool is_radix();
    map<std::string,Terminal*> get_patt_map() const;
    map<std::string,vector<int> > get_patt_indexes_map() const;
    map<std::string,vector<int> > get_lazy_patt_indexes_map() const;

    void set_ref_node(SystemNode* sn);
    void add_concrete_component(Component comp);
    void add_input_terminal(Terminal* t);
    void add_output_terminal(Terminal* t);
    void add_initial(std::string state, std::string c);
    void set_initials(vector<pair<std::string,std::string> > in);
    void add_label(pair<string,string> t, std::string l);
    void add_fault(pair<string,string> t, std::string f);
    void set_viewer(map<pair<string,string>,string> v);
    void set_ruler(map<pair<string,string>,string> r);
    void add_obs_label(std::string l);
    void set_observation(vector<std::string> obs);
    void set_index_space(astl::DFA_map<astl::strings,StateData_str>* idxsp);
    void add_depend(int i);
    void set_radix();
    void set_patt_map(map<std::string,Terminal*> m);
    void set_patt_indexes_map(map<std::string,vector<int> > m);
    void set_lazy_patt_indexes_map(map<std::string,vector<int> > m);
    void add_patt_map(std::string s,Terminal* t);
    void add_patt_indexes_map(std::string s, vector<int>  is);
    void add_lazy_patt_indexes_map(std::string s, vector<int> is);


    Component* find_component(std::string id);
    void make_terminals();
    int input_terminals_count();
    astl::DFA_map<Transition,StateData_str>::state_type find_initial_state(std::string str);
};

#endif // PROBLEMNODE_H
