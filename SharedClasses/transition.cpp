#include "transition.h"

Transition::Transition()
{
}

Transition::~Transition()
{
}

std::string Transition::get_name() const
{
    return name;
}

std::pair<std::string,std::string> Transition::get_input_event() const
{
    return input_event;
}

std::pair<std::string,std::string> Transition::get_s1_s2() const
{
    return s1_s2;
}

std::vector<std::pair<std::string,std::string> > Transition::get_out_events() const
{
    return out_events;
}

void Transition::set_input_event(std::string e,std::string t)
{
    input_event = make_pair(e,t);
}

void Transition::set_s1_s2(std::string s1,std::string s2)
{
    s1_s2 = make_pair(s1,s2);
}

void Transition::add_out_event(std::string e,std::string t)
{
    out_events.push_back(make_pair(e,t));
}

void Transition::set_out_events(std::vector<std::pair<std::string,std::string> > evs)
{
    out_events = evs;
}
