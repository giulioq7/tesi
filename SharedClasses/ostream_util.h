#ifndef OSTREAM_UTIL_H
#define OSTREAM_UTIL_H

#include <iostream>
#include "componentmodel.h"
#include "component.h"
#include "networkmodel.h"
#include "nettransition.h"
#include "system.h"
#include "systemnode.h"
#include "problem.h"
#include "problemnode.h"
#include "systransition.h"


std::ostream& operator<<(std::ostream& out, const ComponentModel& cm);
std::ostream& operator<<(std::ostream& out, const Component& cm);

template <class T>
ostream& operator<<(ostream& out, const vector<T>& l)
{
    for (typename vector<T>::const_iterator it = l.begin(); it != l.end(); ++it)
    {
        out << *it << " ";
    }

    if(l.empty())
        out << "<empty>";

    return out;
}

template <class T1,class T2>
std::ostream& operator<<(std::ostream& out, const pair<T1,T2>& p)
{
    return out << "(" << p.first << " - " << p.second << ")";
}


template <class T1,class T2>
std::ostream& operator<<(std::ostream& out, const map<T1,T2> m)
{
    for(typename map<T1,T2>::const_iterator it=m.begin();it!=m.end();++it)
        out << it->first << " -> " << it->second << " ";
    return out;
}


std::ostream& operator<<(std::ostream& out, const Transition& t);

std::ostream& operator<<(std::ostream& out, const NetworkModel& net);

std::ostream& operator<<(std::ostream& out, const SystemNode& node);

std::ostream& operator<<(std::ostream& out, const System& sys);

std::ostream& operator<<(std::ostream& out, const ProblemNode& node);

std::ostream& operator<<(std::ostream& out, const Problem& sys);

std::ostream& operator<<(std::ostream& out, const StateData_str& d);

std::ostream& operator<<(std::ostream& out, const StateData_strList& d);

std::ostream& operator<<(std::ostream& out, const NetTransition& t);

std::ostream& operator<<(std::ostream& out, const SysTransition& t);

std::ostream& operator<<(std::ostream& out, const Pattern& p);

std::ostream& operator<<(std::ostream& out, const Terminal& t);

#endif // OSTREAM_UTIL_H
