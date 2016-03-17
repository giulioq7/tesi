#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The Terminal class represents concrete terminals of components and problem nodes
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class Terminal
{
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief value current event value ("<eps>" if empty)
     */
    std::string value;
    /**
     * @brief linked_terminals vector of connected terminals
     */
    std::vector<Terminal*> linked_terminals;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & value;
        ar & linked_terminals;
    }
public:
    /**
     * @brief Terminal
     */
    Terminal();
    /**
     * @brief Terminal
     * @param n
     */
    Terminal(std::string n);

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_value
     * @return
     */
    std::string get_value() const;
    /**
     * @brief get_linked_terminals
     * @return
     */
    std::vector<Terminal*> get_linked_terminals() const;

    /**
     * @brief set_value
     * @param v
     */
    void set_value(std::string v);
    /**
     * @brief add_linked_terminal
     * @param t
     */
    void add_linked_terminal(Terminal* t);
};

#endif // TERMINAL_H
