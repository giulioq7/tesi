#ifndef TRANSITION_H
#define TRANSITION_H

#include "astl.h"
#include <sstream>
#include <algorithm>


class Transition : public CHAR_TRAITS<Transition>
{
public:
    std::string name;
    std::set<std::set<std::string> > delta;

    Transition();
    Transition(std::string str, std::set<std::set<std::string> > d);

    //required definitions to use a Transition as automata alphabet for astl lib
    typedef Transition char_type;
    typedef long        int_type;
    static const size_t size;
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    bool operator<(const Transition t) const;
    bool operator==(const Transition t) const;
};


std::ostream& operator<<(std::ostream& out, const Transition& t);

#endif // TRANSITION_H
