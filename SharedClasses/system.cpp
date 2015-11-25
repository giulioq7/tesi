#include "system.h"

System::System()
{
}

SystemNode* System::find_node(std::string id)
{
    SystemNode* ref = NULL;
    for(vector<SystemNode>::iterator it = node_list.begin(); it != node_list.end(); it++)
    {
        if(it->name == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}
