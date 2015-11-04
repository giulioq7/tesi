#include "problemnode.h"

ProblemNode::ProblemNode()
{
}


Component* ProblemNode::find_component(std::string id)
{
    Component* ref = NULL;
    for(vector<Component>::iterator it = concrete_components.begin(); it != concrete_components.end(); it++)
    {
        if((*it).name == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}
