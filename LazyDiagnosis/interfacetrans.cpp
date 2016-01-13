#include "interfacetrans.h"

InterfaceTrans::InterfaceTrans()
{
    trans = SysTransition();
    delta = set<set<string> >();
    pattern_gen = false;
}

InterfaceTrans::InterfaceTrans(SysTransition t, std::set<std::set<std::string> > d)
{
    trans = t;
    delta = d;
    pattern_gen = false;
}


bool InterfaceTrans::operator<(const InterfaceTrans t) const
{
    stringstream ss1,ss2;
    ss1 << (*this);
    ss2 << t;

    return (ss1.str() < ss2.str());
}

bool InterfaceTrans::operator==(const InterfaceTrans t) const
{
    stringstream ss1,ss2;
    ss1 << (*this);
    ss2 << t;

    return (ss1.str() == ss2.str());
}


std::ostream& operator<<(std::ostream& out, const InterfaceTrans& t)
{
    stringstream ss;
    string str;
    ss << "[" << t.trans.name << ",";
    set<set<string> > d = t.delta;
    ss << "(";
    str = ss.str();
    for(set<set<string> >::iterator it = d.begin(); it != d.end(); it++)
    {
        ss << "(";
        for(set<string>::iterator it2 = it->begin(); it2 != it->end(); it2++)
            ss << *it2 << ",";
        str = ss.str();
        if(!it->empty())
            str.resize(str.size()-1);
        str.append(")");
    }
    str.append(")");
    out << str << "]";

    return out;
}
