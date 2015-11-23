#ifndef OUTPUTTERMINAL_H
#define OUTPUTTERMINAL_H

#include <vector>
#include "terminal.h"

class OutputTerminal : public Terminal
{
public:
    OutputTerminal();
    OutputTerminal(std::string n);
    std::vector<Terminal*> linked_terminals;

};

#endif // OUTPUTTERMINAL_H
