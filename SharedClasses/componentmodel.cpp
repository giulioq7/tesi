#include "componentmodel.h"

using namespace astl;

ComponentModel::ComponentModel()
{
}


void ComponentModel::build_automaton()
{
    build_states();
    for(unsigned int i=0; i<trans.size(); i++)
    {
        automaton.set_trans(find_state(trans.at(i).s1_s2.first),trans.at(i),find_state(trans.at(i).s1_s2.second));
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
       DFA_map<Transition,StateData_str>::state_type s = automaton.new_state();
       automaton.tag(s) = StateData_str(*it);
       autom_states.push_back(s);
    }
}


DFA_map<Transition,StateData_str>::state_type ComponentModel::find_state(std::string name)
{
     for(unsigned int i=0; i<autom_states.size(); i++)
     {
         if(automaton.tag(autom_states.at(i)).state_name == name)
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
        if((*it).name == name_tr)
        {
            ref = &(*it);
            return ref;
        }
    }
    return ref;
}
