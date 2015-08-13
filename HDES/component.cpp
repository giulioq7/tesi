#include "component.h"

Component::Component(string name)
{
    this->name = name;
    this->complex = false;
    this->children = new vector<Component>;
}

string Component::get_name()
{
    return this->name;
}

bool Component::is_complex()
{
    return this->complex;
}

vector<Component> *Component::get_children()
{
    return this->children;
}

void Component::add_child(Component *child)
{
    this->children->push_back(*child);
    if(this->complex == false)
        this->complex = true;
}


string Component::to_string()
{
    string description = "Component named '" + this->name + "', complex: " + string(this->complex ? "true" : "false");
    description += ", children: ";
    for(int i=0; i<this->children->size();i++)
    {
        Component child = this->children->at(i);
        description += child.get_name() + " ";
    }
    description += "\n";

    return description;
}


Component::~Component()
{
    this->children->clear();
}
