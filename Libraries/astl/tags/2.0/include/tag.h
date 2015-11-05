/*
 * ASTL - the Automaton Standard Template Library.
 * C++ generic components for Finite State Automata handling.
 * Copyright (C) 2000-2003 Vincent Le Maout (vincent.lemaout@chello.fr).
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef ASTL_TAG_H
#define ASTL_TAG_H

#include <iostream>
using namespace std;

namespace astl {

struct empty_tag 
{
  template <typename T>
  empty_tag& operator=(const T&) { return *this; }
};

struct statedata_tag 
{
  template <typename T>
  statedata_tag& operator=(const T&) { return *this; }
};

typedef empty_tag empty;

inline bool operator==(const empty_tag&, const empty_tag&) { return true; }
inline bool operator<(const empty_tag&, const empty_tag&) { return false; }
inline ostream& operator<<(ostream &out, const empty_tag&) { return out; }
inline istream& operator>>(istream &in, empty_tag&) { return in; }

inline bool operator==(const statedata_tag&, const statedata_tag&) { return true; }
inline bool operator<(const statedata_tag&, const statedata_tag&) { return false; }
inline ostream& operator<<(ostream &out, const statedata_tag&) { return out; }
inline istream& operator>>(istream &in, statedata_tag&) { return in; }

#if (__GNUG__ > 0) && (__GNUG__ < 4)
// Specialized type_traits for empty_tag
template <>
struct __type_traits<empty_tag>
{
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};
#endif

} // namespace astl

#endif // ASTL_TAG_H
