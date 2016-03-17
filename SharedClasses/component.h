#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "componentmodel.h"
#include "systransition.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>

/**
 * @brief The Component class represents a concrete component in a problem node
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class Component : public NetComponent
{
    /**
     * @brief automaton behavioral model
     */
    astl::DFA_map<SysTransition,StateData_str> *automaton;
    /**
     * @brief input_terminals
     */
    vector<Terminal*> input_terminals;
    /**
     * @brief output_terminals
     */
    vector<Terminal*> output_terminals;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<NetComponent>(*this);
        ar & automaton;
        ar & input_terminals;
        ar & output_terminals;
    }

public:
    /**
     * @brief Component default constructor
     */
    Component();
    /**
     * @brief Component constructor with id
     * @param str name
     */
    Component(std::string str){name = str;}
    ~Component(){}

    /**
     * @brief get_automaton
     * @return
     */
    astl::DFA_map<SysTransition,StateData_str>* get_automaton() const;
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
     * @brief set_automaton
     * @param autom
     */
    void set_automaton(astl::DFA_map<SysTransition,StateData_str>* autom);
    /**
     * @brief add_input_term
     * @param t
     */
    void add_input_term(Terminal* t);
    /**
     * @brief add_output_term
     * @param t
     */
    void add_output_term(Terminal* t);

    /**
     * @brief find_input_terminal find input terminal from id
     * @param id name of terminal
     * @return pointer to Terminal
     */
    Terminal* find_input_terminal(std::string id);
    /**
     * @brief find_output_terminal find output terminal from id
     * @param id name
     * @return pointer to Terminal
     */
    Terminal* find_output_terminal(std::string id);

    /**
     * @brief operator == overload of equality operator
     * @param c
     * @return
     */
    bool operator==(const Component c) const {return get_name() == c.get_name();}
};

#endif // COMPONENT_H
