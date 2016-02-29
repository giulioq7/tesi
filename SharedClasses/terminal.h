#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Terminal
{
    std::string name;
    std::string value;
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
    Terminal();
    Terminal(std::string n);

    std::string get_name() const;
    std::string get_value() const;
    std::vector<Terminal*> get_linked_terminals() const;

    void set_value(std::string v);
    void add_linked_terminal(Terminal* t);
};

#endif // TERMINAL_H
