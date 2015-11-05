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

#ifndef ASTL_CHECK_H
#define ASTL_CHECK_H

#include <concept.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace astl {

// Check if the interface conforms to the concept:

template <typename T>
void check(const T &c, const string& class_name, 
#ifdef _MSC_VER
	   const typename T::char_type &a = T::char_type())
#else
	   const typename T::char_type &a = typename T::char_type())
#endif
{
  cout << "Checking " << class_name << "..." << endl;
  check_(c, c, class_name, a);
}

template <typename T>
void check_trivial_cursor_requirements(const T &c1, const string& class_name, 
				       const typename T::char_type & = typename T::char_type())
{
  typedef typename T::state_type state_type;
  typedef typename T::tag_type   tag_type;
  typedef typename T::char_type  char_type;
  cout << "* trivial cursor requirements" << endl;
  cout << "\t" << class_name << "::" << class_name << "()" << endl;
  T c2;
  cout << "\t" << class_name << "::" << class_name << "(" << class_name << ")" << endl;
  T c3 = c2;
  cout << "\t" << class_name << "::operator=(" << class_name << ")" << endl;
  c3 = c2 = c1;                      // assignable, copy constructible
  cout << "\t" << class_name << "::operator==(" << class_name << ")" << endl;
  bool b = c1 == c2;                   // equality comparable
  cout << "\t" << class_name << "::sink()" << endl;
  b = c1.sink();                       // bool sink() const;
  cout << "\t" << class_name << "::sink_state()" << endl;
  c1.sink_state();      // state_type sink_state() const;
  cout << "\t" << class_name << "::state_type::state_type()" << endl;
  state_type s;
  cout << "\t" << class_name << "::state_type::state_type(state_type)" << endl;
  state_type ss = c1.sink_state();
  cout << "\t" << class_name << "::state_type::operator=(state_type)" << endl;
  s = ss;
  cout << "\t" << class_name << "::state_type::operator==(state_type)" << endl;
  b = ss == s;
  cout << "\t" << class_name << "::state_type::operator<(state_type)" << endl;
  b = ss < s;                
  cout << "\t" << class_name << "::src()" << endl;
  state_type q = c1.src();  
  q = q;
  cout << "\t" << class_name << "::src_final()" << endl;
  b = c1.src_final();                  // bool src_final() const;
  cout << "\t" << class_name << "::operator=(state_type)" << endl;
  c2 = c1.sink_state();                // T::operator=(state_type);
  cout << "\t" << class_name << "::tag_type::tag_type()" << endl;
  tag_type t2;
  cout << "\t" << class_name << "::tag_type::tag_type(tag_type)" << endl;
  tag_type t1 = t2;
  cout << "\t" << class_name << "::tag_type::operator=(tag_type)" << endl;
  t1 = t2;
  cout << "\t" << class_name << "::src_tag()" << endl;
  t1 = c1.src_tag();          // tag_type src_tag() const, tag_type::tag_type(tag_type)
}

// The cursor must be in a valid state:
template <typename T>
void check_(const T &c, cursor_concept, const string& class_name, 
	    const typename T::char_type &a = typename T::char_type())
{
  T cc = c;
  check_trivial_cursor_requirements(c, class_name, a);
  cout << "* plain cursor requirements" << endl;
  cout << "\t" << class_name << "::exists(char_type)" << endl;
  bool b = cc.exists(a);
  cout << "\t" << class_name << "::forward(char_type)" << endl;
  b = cc.forward(a);
}

template <typename T>
void check_(const T &c, transition_cursor_concept, const string& class_name, 
	    const typename T::char_type &a = typename T::char_type())
{
  bool b;
  T cc = c;
  check_trivial_cursor_requirements(c, class_name, a);
  cout << "* transition cursor requirements" << endl;
  typedef typename T::char_traits char_traits;
  cout << "\t" << class_name << "::first()" << endl;
  b = cc.first();
  cout << "\t" << class_name << "::letter()" << endl;
  typename T::char_type aa = cc.letter();
  aa = aa; // just to avoid annoying warnings about unused variables
  T d = cc;
  cout << "\t" << class_name << "::forward()" << endl;
  d.forward();
  cout << "\t" << class_name << "::next()" << endl;
  b = cc.next();
  cc.first();
  cout << "\t" << class_name << "::aim()" << endl;
  typename T::state_type q = cc.aim();
  q = q;
  cout << "\t" << class_name << "::aim_final()" << endl;
  b = cc.aim_final();
  cout << "\t" << class_name << "::aim_tag()" << endl;
  typename T::tag_type t = cc.aim_tag();
}

template <typename T>
void check_(const T &c, forward_cursor_concept, const string& class_name, 
	    const typename T::char_type &a = typename T::char_type())
{
  T cc = c;
  check_(cc, cursor_concept(), class_name, a);
  check_(cc, transition_cursor_concept(), class_name, a);
  cout << "* forward cursor requirements" << endl;
  cout << "\t" << class_name << "::find(char_type)" << endl;
  bool b = cc.find(a);
  b = b;
}

} // namespace astl

#endif // ASTL_CHECK_H
