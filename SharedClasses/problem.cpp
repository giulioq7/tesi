#include "problem.h"

Problem::Problem()
{
}

ProblemNode* Problem::find_node(std::string id)
{
    ProblemNode* ref = NULL;
    for(vector<ProblemNode>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if(it->name == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}

int Problem::find_index(std::string id)
{
    for(int i=0; i<nodes.size(); i++)
    {
        if(nodes.at(i).name == id)
            return i;
    }
    return -1;
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
