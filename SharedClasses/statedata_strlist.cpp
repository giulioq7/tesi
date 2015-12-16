#include "statedata_strlist.h"

StateData_strList::StateData_strList(std::set<std::string> set_str)
{
    elements = set_str;
}


void StateData_strList::add_elem(std::string str)
{
    elements.insert(str);
}
