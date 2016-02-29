#include "netcomponent.h"

NetComponent::NetComponent()
{
}

NetComponent::NetComponent(std::string str)
{
    name = str;
}

std::string NetComponent::get_name() const
{
    return name;
}

ComponentModel* NetComponent::get_model() const
{
    return model;
}

void NetComponent::set_model(ComponentModel* cm)
{
    model = cm;
}
