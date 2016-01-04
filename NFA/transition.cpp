#include "transition.h"

using namespace std;

Transition::Transition()
{
    name = "";
}


Transition::Transition(string str,set<set<string> > d)
{
    name = str;
    delta = d;
}


bool Transition::operator<(const Transition t) const
{
    stringstream ss1,ss2;
    ss1 << (*this);
    ss2 << t;

    return (ss1.str() < ss2.str());
}

bool Transition::operator==(const Transition t) const
{
    stringstream ss1,ss2;
    ss1 << (*this);
    ss2 << t;

    return (ss1.str() == ss2.str());
}


std::ostream& operator<<(std::ostream& out, const Transition& t)
{
    stringstream ss;
    string str;
    ss << "[" << t.name << ",";
    set<set<string> > d = t.delta;
    ss << "(";
    str = ss.str();
    for(set<set<string> >::iterator it = d.begin(); it != d.end(); it++)
    {
        ss << "(";
        for(set<string>::iterator it2 = it->begin(); it2 != it->end(); it2++)
            ss << *it2 << ",";
        str = ss.str();
        str.resize(str.size()-1);
        str.append(")");
    }
    str.append(")");
    out << str << "]";

    return out;
}
