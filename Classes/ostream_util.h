#ifndef OSTREAM_UTIL_H
#define OSTREAM_UTIL_H

#include <iostream>
#include "componentmodel.h"
#include "component.h"
#include "networkmodel.h"
#include "spec_driver.h"
#include "nettransition.h"


std::ostream& operator<<(std::ostream& out, const ComponentModel& cm);
std::ostream& operator<<(std::ostream& out, const Component& cm);

template <class T>
std::ostream& operator<<(std::ostream& out, const vector<T>& cm);

template <class T1,class T2>
std::ostream& operator<<(std::ostream& out, const pair<T1,T2>& p);

template <class T1,class T2>
std::ostream& operator<<(std::ostream& out, const map<T1,T2> m);

std::ostream& operator<<(std::ostream& out, const Transition& t);

std::ostream& operator<<(std::ostream& out, const NetworkModel& net);

std::ostream& operator<<(std::ostream& out, const SystemNode& node);

std::ostream& operator<<(std::ostream& out, const System& sys);

std::ostream& operator<<(std::ostream& out, const ProblemNode& node);

std::ostream& operator<<(std::ostream& out, const Problem& sys);

std::ostream& operator<<(std::ostream& out, const spec_driver& d);

std::ostream& operator<<(std::ostream& out, const StateData_str& d);

std::ostream& operator<<(std::ostream& out, const NetTransition& t);

#endif // OSTREAM_UTIL_H
