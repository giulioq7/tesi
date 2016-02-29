#include "interfacetrans.h"

using namespace astl;

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
    if(t.delta.empty())
        out << t.trans.get_name();
    else
        out << "[" << t.trans.get_name() << "," << t.delta << "]";

    return out;
}
