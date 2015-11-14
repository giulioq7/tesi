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
            for(vector<std::string>::iterator it3 = it2->model->inputs.begin(); it3 != it2->model->inputs.end(); it3++)
                count++;
        }
    }
    return count;
}
