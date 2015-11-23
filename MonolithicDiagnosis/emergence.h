#ifndef EMERGENCE_H
#define EMERGENCE_H

#include <iostream>
#include <vector>
#include "terminal.h"

class Emergence
{
public:
    Emergence();

    std::vector<std::string> values;
    Terminal *linked_terminal;
};

#endif // EMERGENCE_H
