#include "component.h"


Component::Component(string name)
{
    this->name = name;
}

string Component::getName()
{
    return name;
}
