#ifndef INTERFACETRANS_H
#define INTERFACETRANS_H

#include "astl.h"
#include "systransition.h"

class InterfaceTrans
{
public:
    SysTransition trans;
    std::set<std::set<std::string> > delta;
    bool pattern_gen;
    std::set<std::string> pattern_events;

    InterfaceTrans();
    InterfaceTrans(SysTransition t, std::set<std::set<std::string> > d);

    //required definitions to use a SysTransition as automata alphabet for astl lib
    typedef InterfaceTrans char_type;
    typedef long        int_type;
    static const size_t size;
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    bool operator<(const InterfaceTrans t) const;
    bool operator==(const InterfaceTrans t) const;
};

std::ostream& operator<<(std::ostream& out, const InterfaceTrans& t);

#endif // INTERFACETRANS_H
