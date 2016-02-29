#include "componentmodel.h"

using namespace astl;

ComponentModel::ComponentModel()
{
    //automaton = new DFA_map<Transition,StateData_str>();
}

std::string ComponentModel::get_name() const
{
    return name;
}

vector<std::string> ComponentModel::get_events() const
{
    return events;
}

vector<std::string> ComponentModel::get_inputs() const
{
    return inputs;
}

vector<std::string> ComponentModel::get_outputs() const
{
    return outputs;
}

vector<std::string> ComponentModel::get_states() const
{
    return states;
}

vector<Transition> ComponentModel::get_trans() const
{
    return trans;
}

astl::DFA_map<Transition,StateData_str>* ComponentModel::get_automaton() const
{
    return automaton;
}

void ComponentModel::set_events(vector<std::string> v)
{
    events = v;
}

void ComponentModel::set_inputs(vector<std::string> v)
{
    inputs = v;
}

void ComponentModel::set_outputs(vector<std::string> v)
{
    outputs = v;
}

void ComponentModel::set_states(vector<std::string> v)
{
    states = v;
}

void ComponentModel::set_trans(vector<Transition> v)
{
    trans = v;
}


void ComponentModel::build_automaton()
{
    automaton = new DFA_map<Transition,StateData_str>();
    build_states();
    for(unsigned int i=0; i<trans.size(); i++)
    {
        automaton->set_trans(find_state(trans.at(i).get_s1_s2().first),trans.at(i),find_state(trans.at(i).get_s1_s2().second));
    }
    //initial state is unknown in the model: it can be defined in a inherited way in
    //network model, system and problem declaration within initial-section
    //automaton.initial(autom_states[0]);

}

void ComponentModel::build_states()
{
    vector<std::string>::iterator it;
    for(it = states.begin(); it != states.end(); it++)
    {
       DFA_map<Transition,StateData_str>::state_type s = automaton->new_state();
       automaton->tag(s) = StateData_str(*it);
       autom_states.push_back(s);
    }
}


DFA_map<Transition,StateData_str>::state_type ComponentModel::find_state(std::string name)
{
     for(unsigned int i=0; i<autom_states.size(); i++)
     {
         if(automaton->tag(autom_states.at(i)).get_state_name() == name)
            return autom_states.at(i);
     }
     //this return statement should never be reached
     return DFA_map<Transition,StateData_str>::state_type();
}


Transition* ComponentModel::find_trans(std::string name_tr)
{

    Transition* ref = NULL;
    for(vector<Transition>::iterator it = trans.begin(); it != trans.end(); it++)
    {
        if((*it).get_name() == name_tr)
        {
            ref = &(*it);
            return ref;
        }
    }
    return ref;
}
