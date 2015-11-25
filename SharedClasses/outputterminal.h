#ifndef OUTPUTTERMINAL_H
#define OUTPUTTERMINAL_H

#include <vector>
#include <boost/serialization/base_object.hpp>
#include "terminal.h"

class OutputTerminal : public Terminal
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        // serialize base class information
        ar & boost::serialization::base_object<Terminal>(*this);
        std::vector<Terminal*>::iterator it;
        ar & linked_terminals;
    }
public:
    OutputTerminal();
    OutputTerminal(std::string n);
    std::vector<Terminal*> linked_terminals;

};

#endif // OUTPUTTERMINAL_H
