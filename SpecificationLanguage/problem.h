#ifndef PROBLEM_H
#define PROBLEM_H

#include "problemnode.h"

class Problem
{
public:
    std::string name;
    vector<ProblemNode> nodes;

    Problem();
    Problem(std::string str) { name = str;}
};

#endif // PROBLEM_H
