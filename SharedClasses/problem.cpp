#include "problem.h"

Problem::Problem()
{
}

std::string Problem::get_name() const
{
    return name;
}

vector<ProblemNode> Problem::get_nodes() const
{
    return nodes;
}

vector<int> Problem::get_topological_order() const
{
    return topological_order;
}

void Problem::add_node(ProblemNode pn)
{
    nodes.push_back(pn);
}

void Problem::set_nodes(vector<ProblemNode> v)
{
    nodes = v;
}

void Problem::set_topological_order(vector<int> order)
{
    topological_order = order;
}


ProblemNode* Problem::find_node(std::string id)
{
    ProblemNode* ref = NULL;
    for(vector<ProblemNode>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if(it->get_name() == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}

int Problem::find_index(std::string id)
{
    for(unsigned int i=0; i<nodes.size(); i++)
    {
        if(nodes.at(i).get_name ()== id)
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
            vector<Terminal*> terms = it2->get_input_terminals();
            for(vector<Terminal*>::iterator it3 = terms.begin(); it3 != terms.end(); it3++)
                count++;
        }
    }
    return count;
}
