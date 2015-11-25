#include "component.h"

using namespace astl;

Component::Component()
{
    model = NULL;
}

Terminal* Component::find_terminal(std::string id)
{
    Terminal* ref = NULL;
    for(vector<Terminal>::iterator it = input_terminals.begin(); it != input_terminals.end(); it++)
    {
        if((*it).name == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}

OutputTerminal* Component::find_output_terminal(std::string id)
{
    OutputTerminal* ref = NULL;
    for(vector<OutputTerminal>::iterator it = output_terminals.begin(); it != output_terminals.end(); it++)
    {
        if((*it).name == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}
