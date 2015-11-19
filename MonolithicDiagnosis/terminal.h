#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>

class Terminal
{
public:
    std::string name;
    std::string value;

    Terminal();
    Terminal(std::string n);
};

#endif // TERMINAL_H
