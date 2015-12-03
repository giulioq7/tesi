#include "problem.h"

Problem::Problem()
{
}

int Problem::concrete_components_count()
{
    int count = 0;
    for(vector<ProblemNode>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
            count++;
    }
    return count;
}

int Problem::input_terminals_count()
{
    int count = 0;
    for(vector<ProblemNode>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
        {
            for(vector<Terminal*>::iterator it3 = it2->input_terminals.begin(); it3 != it2->input_terminals.end(); it3++)
                count++;
        }
    }
    return count;
}
