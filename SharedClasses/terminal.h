#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Terminal
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & value;
        ar & linked_terminals;
    }
public:
    std::string name;
    std::string value;
    std::vector<Terminal*> linked_terminals;

    Terminal();
    Terminal(std::string n);
};

#endif // TERMINAL_H
