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

#ifndef ASTL_SET_OPERATION_H
#define ASTL_SET_OPERATION_H

#include <astl.h>

// Requirements: 
// Outgoing transitions of adapted cursors must be sorted in increasing order

#include <list>
#include <vector>
#include <utility>
#include <string>
#include <functional>
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

namespace astl {

template <class Alphabet1, class Alphabet2>
struct default_lower_than : public binary_function<Alphabet1, Alphabet2, bool>
{
  bool operator() (const Alphabet1 &x, const Alphabet2 &y) const {
    return x < y;
  }
};

template <class ForwardCursor1, class ForwardCursor2, 
  class LowerThan = default_lower_than<typename ForwardCursor1::char_type, 
				       typename ForwardCursor2::char_type> >
class union_cursor : public forward_cursor_concept
{
protected:
  ForwardCursor1 x1;
  ForwardCursor2 x2;
  bool x1_at_end, x2_at_end;

public:
  typedef union_cursor self;

  typedef pair<typename ForwardCursor1::state_type, 
	       typename ForwardCursor2::state_type> state_type;
  typedef pair<typename ForwardCursor1::tag_type, 
	       typename ForwardCursor2::tag_type>   tag_type;
  typedef typename ForwardCursor1::char_type        char_type;
  typedef typename ForwardCursor1::char_traits      char_traits;

  union_cursor(const ForwardCursor1 &x1, const ForwardCursor2 &x2)
    : x1(x1), x2(x2)
  { }

  bool operator==(const self &x) const { return x1 == x.x1 && x2 == x.x2; }

  void forward() {
    if (x1_at_end) {
      x1 = x1.sink_state();
      x2.forward();  // case (0, q2)
      return;
    }
    if (x2_at_end) {
      x2 = x2.sink_state();
      x1.forward();  // case (q1, 0)
      return;
    }
    // case (q1, q2)
    if (LowerThan()(x1.letter(), x2.letter())) {  // x1.letter() < x2.letter()
      x2 = x2.sink_state();
      x1.forward(); 
      return;
    }
    if (LowerThan()(x2.letter(),x1.letter())) {   // x2.letter() < x1.letter()
      x1 = x1.sink_state();
      x2.forward();
      return;
    }
    // x1.letter() == x2.letter()
    x1.forward();
    x2.forward();
  }

  bool first() {
    x1_at_end = x1.sink() || !x1.first();
    x2_at_end = x2.sink() || !x2.first();
    return !(x1_at_end && x2_at_end);
  }

  bool next() {
    if (x1_at_end) {
      x2_at_end = !x2.next();   // (0, q2)
      return !x2_at_end;
    }
    if (x2_at_end) {
      x1_at_end = !x1.next();   // (q1, 0)
      return !x1_at_end;
    }
    // case (q1, q2)
    if (LowerThan()(x1.letter(), x2.letter())) { // x1.letter() < x2.letter()
      x1_at_end = !x1.next();
      return true;
    }
    if (LowerThan()(x2.letter(), x1.letter())) { // x2.letter() < x1.letter()
      x2_at_end = !x2.next();
      return true;
    } 
    // x2.letter() == x1.letter()
    x1_at_end = !x1.next();              
    x2_at_end = !x2.next();
    return !(x1_at_end && x2_at_end);
  }

  char_type letter() const {
    return x1_at_end ? x2.letter() : x2_at_end ? x1.letter() 
      : min__(x1.letter(), x2.letter(), LowerThan());
  }

  bool find(const char_type &a) {
    x1_at_end = x1.sink() || !x1.find(a);
    x2_at_end = x2.sink() || !x2.find(a);
    return !(x1_at_end && x2_at_end);
  }

  bool forward(const char_type &a) {
    if (!x1.sink()) x1.forward(a);
    if (!x2.sink()) x2.forward(a);
    return !sink();
  }

  state_type src() const { return state_type(x1.src(), x2.src()); }

  state_type aim() const {
    if (x1_at_end)
      return state_type(x1.sink_state(), x2.aim());
    if (x2_at_end)
      return state_type(x1.aim(), x2.sink_state());
    if (LowerThan()(x2.letter(), x1.letter()))
      return state_type(x1.sink_state(), x2.aim());
    if (LowerThan()(x1.letter(), x2.letter())) 
      return state_type(x1.aim(), x2.sink_state());
    return state_type(x1.aim(), x2.aim());
  }

  bool src_final() const {
    return (!x1.sink() && x1.src_final()) || (!x2.sink() && x2.src_final());
  }

  bool aim_final() const {
    if (x1_at_end) return x2.aim_final();
    if (x2_at_end) return x1.aim_final();
    if (LowerThan()(x1.letter(), x2.letter()))
      return x1.aim_final();
    if (LowerThan()(x2.letter(), x1.letter()))
      return x2.aim_final();
    return x1.aim_final() || x2.aim_final();
  }

  bool sink() const { return x1.sink() && x2.sink(); }

  void to_sink() {
    x1.to_sink();
    x2.to_sink();
  }

  bool exists(const char_type &a) const {
    if (x1.sink()) return x2.exists(a);
    return x1.exists(a) || (!x2.sink() && x2.exists(a));
  }

  tag_type aim_tag() const {
    if (x1_at_end) return tag_type(x2.aim_tag(), x2.aim_tag());
    if (x2_at_end) return tag_type(x1.aim_tag(), x1.aim_tag());
    return tag_type(x1.sink() ? x2.aim_tag() : x1.aim_tag(), 
	       x2.sink() ? x1.aim_tag() : x2.aim_tag());
  }

  tag_type src_tag() const {
    return tag_type(x1.sink() ? x2.src_tag() : x1.src_tag(), 
	       x2.sink() ? x1.src_tag() : x2.src_tag());
  }

  self& operator= (const state_type &x) {
    x1 = x.first;
    x2 = x.second;
    x1_at_end = x2_at_end = true;
    return *this;
  }

  // Not a standard requirement, do not use in algorithms (needed by wgrep):
  ForwardCursor1& first_() { return x1; }
  ForwardCursor2& second_() { return x2; }
  const ForwardCursor1& first_() const { return x1; }
  const ForwardCursor2& second_() const { return x2; }
  void reset() {
    x1_at_end = x1.sink();
    x2_at_end = x2.sink();
  }

};

template <class ForwardC1, class ForwardC2, 
  class LowerThan = default_lower_than<typename ForwardC1::char_type,
  typename ForwardC2::char_type> >
class intersection_cursor : public forward_cursor_concept
{
public:
  typedef intersection_cursor                  self;
  typedef pair<typename ForwardC1::state_type, 
	       typename ForwardC2::state_type> state_type;
  typedef pair<typename ForwardC1::tag_type, 
	       typename ForwardC2::tag_type>   tag_type;
  typedef typename ForwardC1::char_type        char_type;
  typedef typename ForwardC1::char_traits      char_traits;

  intersection_cursor(const ForwardC1 &x, const ForwardC2 &y)
    : x1(x), x2(y)
  { }

  bool sink() const { return x1.sink() || x2.sink(); }

  state_type sink_state() const {
    return state_type(x1.sink_state(), x2.sink_state());
  }

  state_type src() const { return state_type(x1.src(), x2.src()); }

  state_type aim() const { return state_type(x1.aim(), x2.aim()); }

  self& operator=(const state_type &p) {
    x1 = p.first;
    x2 = p.second;
    return *this;
  }

  void forward() {
    x1.forward();
    x2.forward();
  }

  bool first() {
    if (x1.first() && x2.first())
      while(1) 
	if (LowerThan()(x1.letter(), x2.letter())) {
	  if (!x1.next()) return false;
	}
	else
	  if (LowerThan()(x2.letter(), x1.letter())) {
	    if (!x2.next()) return false;
	  }
	  else    //  *first == *second 
	    return true;
    return false;
  }

  bool next() {
    if (x1.next() && x2.next())
      while(1) 
	if (LowerThan()(x1.letter(), x2.letter())) {
	  if (!x1.next()) return false;
	}
	else
	  if (LowerThan()(x2.letter(), x1.letter())) {
	    if (!x2.next()) return false;
	  }
	  else    //  *first == *second 
	    return true;
    return false;
  }

  char_type letter() const { return x1.letter(); }

  bool find(const char_type &a) {
    bool r = x1.find(a);  // we don't want any lazy boolean evaluation
    return x2.find(a) && r;
  }

  bool forward(const char_type &a) {
    if (x1.forward(a) && x2.forward(a)) return true;
    *this = sink_state();
    return false;
  }

  bool src_final() const { return x1.src_final() && x2.src_final(); }

  bool aim_final() const { return x1.aim_final() && x2.aim_final(); }

  bool exists(const char_type &a) const { return x1.exists(a) && x2.exists(a); }

  tag_type src_tag() const { return tag_type(x1.src_tag(), x2.src_tag()); }

  tag_type aim_tag() const { return tag_type(x1.aim_tag(), x2.aim_tag()); }

  bool operator==(const self &x) const { return x1 == x.x1 && x2 == x.x2; }

  // Not a standard requirement, do not use in algorithms (needed by wgrep):
  ForwardC1& first_() { return x1; }
  ForwardC2& second_() { return x2; }
  const ForwardC1& first_() const { return x1; }
  const ForwardC2& second_() const { return x2; }

protected:
  ForwardC1 x1;
  ForwardC2 x2;
};
  
template <class ForwardCursor1, class ForwardCursor2>
class diff_cursor : public forward_cursor_concept
{
protected:
  ForwardCursor1         x1;
  mutable ForwardCursor2 x2;  // mutable because methods *_final() are const 

public:
  typedef pair<typename ForwardCursor1::state_type, 
	       typename ForwardCursor2::state_type> state_type;
  typedef typename ForwardCursor1::char_type        char_type;
  typedef typename ForwardCursor1::tag_type         tag_type;
  typedef typename ForwardCursor1::char_traits      char_traits;

  diff_cursor(const ForwardCursor1 &x1, const ForwardCursor2 &x2)
    : x1(x1), x2(x2)
  { }
  
  bool operator== (const diff_cursor &y) const {
    return x1 == y.x1 && x2 == y.x2;
  }

  char_type letter() const { return x1.letter(); }

  void forward() {
    if (!x2.sink()) x2.forward(x1.letter());
    x1.forward();
  }

  bool first() { return x1.first(); }

  bool next() { return x1.next(); }

  bool find(const char_type &a) { return x1.find(a); }

  bool forward(const char_type &a) {
    if (!x1.forward(a)) {
      x2 = x2.sink_state();
      return false;
    }
    if (!x2.sink()) x2.forward(a);
    return true;
  }

  state_type src() const { return state_type(x1.src(), x2.src()); }

  state_type aim() const {
    if (!x2.sink() && x2.find(x1.letter()))
      return state_type(x1.aim(), x2.aim());
    return state_type(x1.aim(), x2.sink_state());
  }

  bool src_final() const {
    return x1.src_final() && (x2.sink() || !x2.src_final());
  }

  bool aim_final() const {
    return x1.aim_final() && (x2.sink() || !x2.find(x1.letter()) || !x2.aim_final());
  }

  bool sink() const { return x1.sink(); }
  
  state_type sink_state() const {
    return state_type(x1.sink_state(), x2.sink_state());
  }

  bool exists(const char_type &a) const { return x1.exists(a); }

  tag_type src_tag() const { return x1.src_tag(); }

  tag_type aim_tag() const { return x1.aim_tag(); }
};

// Symmetrical Difference
// (A1 || A2) \ (A1 && A2)
template <class C1, class C2>   // C1,C2 forward cursor types
class sym_diff_cursor 
  : public diff_cursor<union_cursor<C1, C2>, intersection_cursor<C1, C2> >
{
public:
  typedef diff_cursor<union_cursor<C1, C2>, intersection_cursor<C1, C2> > super; 
  typedef sym_diff_cursor self;
  typedef typename super::state_type state_type;
  typedef typename super::tag_type   tag_type;

  sym_diff_cursor(const C1 &x1, const C2 &x2)
    : super(unionc(x1, x2), intersectionc(x1, x2))
  { }

  self& operator=(const state_type &q) {
    super::operator=(q);
    return *this;
  }
};


template <class ForwardCursor1, class ForwardCursor2>
class concatenation_cursor : public forward_cursor_concept
{
protected:
  typedef list<pair<bool, ForwardCursor2> > container;

  ForwardCursor1 x1;
  ForwardCursor2 i;
  container      x2;
  bool ok1; // ok1 == true signifie x1 est actif et sur une transition
  // déréférençable. Invariant: x1.sink() && ok1 == false (on ne peut
  // pas avoir x1 sur le puits ET ok1 vrai simultanément
  // x1.sink() == true => ok1 == false). Même chose pour les curseurs de
  // x2 qui possèdent leur booléen associé (cf typedef container).
  typename ForwardCursor2::char_type current_letter;

public:
  typedef typename ForwardCursor1::tag_type                  tag_type;
  typedef pair<typename ForwardCursor1::state_type, 
	       vector<typename ForwardCursor2::state_type> > state_type;
  typedef typename ForwardCursor1::char_type                 char_type;
  typedef typename ForwardCursor1::char_traits               char_traits;

  concatenation_cursor(const ForwardCursor1 &c1, const ForwardCursor2 &c2)
    : x1(c1), i(c2), ok1(false) { 
    if (!x1.sink() && x1.src_final() && !i.sink()) 
      x2.push_back(make_pair(false, i));
  }

  bool sink() const { return x1.sink() && x2.empty(); }

  state_type sink_state() const {
    return state_type(x1.sink_state(), container());
  }

  state_type src() const {
    state_type result(x1.src(), vector<typename ForwardCursor2::state_type>());
    result.second.reserve(4);
    for(typename container::const_iterator j = x2.begin(); j != x2.end(); ++j)
      result.second.push_back((*j).second.src());
    return result;
  }

  state_type aim() const {
    state_type result;

    result.second.reserve(4);
    for(typename container::const_iterator j = x2.begin(); j != x2.end(); ++j)
      if ((*j).first && (*j).second.letter() == current_letter) 
	result.second.push_back((*j).second.aim());

    if (ok1 && x1.letter() == current_letter) {
      result.first = x1.aim();
      if (x1.aim_final()) result.second.push_back(i.src());
    }
    else result.first = x1.sink_state();
    return result;
  }
    
  void forward() {
    for(typename container::iterator j = x2.begin(); j != x2.end(); )
      if ((*j).first && (*j).second.letter() == current_letter) (*j++).second.forward();
      else x2.erase(j++);
    
    if (ok1 && x1.letter() == current_letter) {
      x1.forward();
      if (x1.src_final()) x2.push_back(make_pair(false, i));
    }
    else {
      x1 = x1.sink_state();
      ok1 = false;
    }
  }

  bool first() {
    if (!x2.empty()) {
      typename container::iterator j;
      for(j = x2.begin(); j != x2.end() && !(*j).second.first(); ++j)
	(*j).first = false;

      if (j != x2.end()) {
	(*j).first = true;
	current_letter = (*j).second.letter();
	for(++j; j != x2.end(); ++j) {
	  (*j).first = (*j).second.first();
	  if ((*j).first) current_letter = min__(current_letter, (*j).second.letter());
	}
	ok1 = !x1.sink() && x1.first();
	if (ok1) current_letter = min__(current_letter, x1.letter());
	
	return true;
      }
    }
    ok1 = !x1.sink() && x1.first();
    if (ok1) current_letter = x1.letter();
    return ok1;
  }

  bool next() {
    char_type next_letter;
    if (!x2.empty()) {
      typename container::iterator j;
      for(j = x2.begin(); j != x2.end(); ++j)
	if ((*j).first) {
	  if ((*j).second.letter() == current_letter)
	    (*j).first = (*j).second.next();
	  if ((*j).first) {
	    next_letter = (*j).second.letter();
	    break;
	  }
	}

      if (j != x2.end()) {
	for(++j; j != x2.end(); ++j) {
	  if ((*j).first) {
	    if ((*j).second.letter() == current_letter)
	      (*j).first = (*j).second.next();
	    if ((*j).first) next_letter = min__(next_letter, (*j).second.letter());
	  }
	}

	if (ok1 && x1.letter() == current_letter) {
	  ok1 = x1.next();
	  if (ok1) next_letter = min__(next_letter, x1.letter());
	}

	current_letter = next_letter;
	return true;
      }
    }
    if (ok1 && x1.letter() == current_letter) {
      ok1 = x1.next();
      if (ok1) current_letter = x1.letter();
      return ok1;
    }
    return false;
  }
    
  char_type letter() const { return current_letter; }
 
  bool find(const char_type &a) {
    bool result = ok1 = !x1.sink() && x1.find(a);
    for(typename container::iterator j = x2.begin(); j != x2.end(); ++j) {
      (*j).first = (*j).second.find(a);
      result = result || (*j).first;
    }
    return result;
  } 
  
  bool forward(const char_type &a) {
    bool result = false;
    for(typename container::iterator j = x2.begin(); j != x2.end();) 
      if ((*j).second.forward(a)) {
	result = true;
	++j;
      }
      else x2.erase(j++);
    
    if (!x1.sink())
      if (x1.forward(a)) { 
	if (x1.src_final())
	  x2.push_back(i);
	return true;
      }
    
    return result;
  }
  
  bool src_final() const {
    for(typename container::const_iterator j = x2.begin(); j != x2.end(); ++j)
      if ((*j).second.src_final()) return true;
    return false;
  }

  bool aim_final() const {
    for(typename container::const_iterator j = x2.begin(); j != x2.end(); ++j)
      if ((*j).first && (*j).second.letter() == current_letter && (*j).second.aim_final()) 
	return true;

    return false;
  }

  bool operator== (const concatenation_cursor &x) const {
    return x1 == x.x1 && ok1 == x.ok1 && x2 == x.x2;
  }

  tag_type src_tag() const {
    if (!x1.sink()) return x1.src_tag();
    else return tag_type();
  }

  tag_type aim_tag() const {
    if (ok1) return x1.aim_tag();
    else return tag_type();
  }
};    

template <typename EnumerableCharTraits>
class sigma_star_cursor : public forward_cursor_concept
{
protected:
  typename EnumerableCharTraits::iterator i;

public:
  typedef sigma_star_cursor                        self;
  typedef typename EnumerableCharTraits::char_type char_type;
  typedef empty_tag                                tag_type;
  typedef int                                      state_type;
  typedef EnumerableCharTraits                     char_traits;

  bool operator==(const self &x) const { return i == x.i; }

  self& operator=(state_type&) { return *this; }

  state_type src() const { return 1; }

  state_type aim() const { return 1; }    

  state_type sink_state() const { return 0; }

  tag_type src_tag() const { return empty_tag(); }
  
  tag_type aim_tag() const { return empty_tag(); }    
  
  bool src_final() const { return true; }
  
  bool aim_final() const { return true; }

  bool sink() const { return false; }

  void forward() { }
  
  bool forward(const char_type&) { return true; }
  
  char_type letter() const { return *i; }
  
  bool first() {
    i = char_traits::begin();
    return true;
  }
  
  bool next() { return ++i != char_traits::end(); }

  bool find(const char_type& a) {
    i = find(char_traits::begin(), char_traits::end(), a);
    return true;
  }

  bool exists(const char_type&) const { return true; }
};
// #define NEW_NOT 
// Requirements on types: ForwardCursor::char_traits must be an EnumerableCharTraits
#ifdef NEW_NOT
template <typename ForwardCursor>
class not_cursor : public ForwardCursor
{
public:
  typedef not_cursor                             self;
  typedef ForwardCursor                          super;
  typedef typename super::tag_type               tag_type;
  typedef typename super::char_type              char_type;
  typedef pair<typename super::state_type, bool> state_type;
  typedef typename super::char_traits            char_traits;

protected:
  bool vstate;    // is the cursor pointing to the virtual default state ?
  typename char_traits::const_iterator i;   // current transition letter

public:
  not_cursor(const ForwardCursor &x)
    : super(x), vstate(false)
  { }

  state_type src() const {
    return state_type(super::src(), vstate);
  }

  self& operator=(const self &x) {
    super::operator=(x);
    vstate = x.vstate;
    return *this;
  }

  bool sink() const { return super::sink() && !vstate; }

  bool forward(const char_type &c) {
    vstate = vstate || !super::forward(c);
    return true;
  }

  bool src_final() const { return vstate || !super::src_final(); }

  const tag_type& src_tag() const {
    if (vstate) return tag_type();
    return super::src_tag();
  }

  bool exists(const char_type &) const { return true; }

  self& operator=(const state_type &q) {
    super::operator=(q.first);
    vstate = q.second;
    return *this;
  }

  state_type sink_state() const {
    return state_type(super::sink_state(), false);
  }
  
  bool operator==(const self &x) const {
    return super::operator==(x) && vstate == x.vstate;
  }

  char_type letter() const { return *i; }

  bool first() {
    return (i = char_traits::begin()) != char_traits::end();
  }

  bool next() { return ++i != char_traits::end(); }
   
  void forward() { if (!vstate) super::forward(); }

  // Linear time, sorry
  bool find(const char_type &c) {
    i = find(char_traits::begin(), char_traits::end(), a);
    if (!vstate) super::find(c);
    return true;
  }

  state_type aim() const {
    if (vstate || !super::exists(letter())) 
      return state_type(super::sink_state(), true);
    return state_type(super::aim(), false);
  }

  bool aim_final() const {
    if (vstate || !super::exists(letter())) return true;
    return !super::aim_final();
  }
    
  const tag_type& aim_tag() const {
    if (vstate || !super::exists(letter())) return tag_type();
    return super::aim_tag();
  }
};

#else
template <typename ForwardCursor>
class not_cursor 
  : public diff_cursor<sigma_star_cursor<typename ForwardCursor::char_traits>, ForwardCursor>
{
public:
  typedef diff_cursor<sigma_star_cursor<typename ForwardCursor::char_traits>, 
		      ForwardCursor> super;
  typedef not_cursor self;
  typedef typename super::tag_type tag_type;
  typedef typename super::char_type char_type;
  typedef typename super::state_type state_type;
  typedef typename super::char_traits char_traits;

  not_cursor(const ForwardCursor &x)
    : super(sigma_star_cursor<typename ForwardCursor::char_traits>(), x)
  { }

  self& operator=(const state_type& q) {
    super::operator=(q);
    return *this;
  }
};
#endif

template <typename DFirstCursor1, typename DFirstCursor2>
bool isomorph(DFirstCursor1 left, DFirstCursor2 first, 
	      DFirstCursor2 last = DFirstCursor2())
{
  typedef map<typename DFirstCursor2::state_type, 
    typename DFirstCursor1::state_type> mapping;
  mapping M;
  bool synchrone = true;  // both cursors should keep going up & down simultaneously
  while (first != last) {
    do {
      if (!synchrone) return false;
      if (left.letter() != first.letter()) 
	return false;
      if (M.find(first.src()) == M.end()) 
	M[first.src()] = left.src();
      else
	if (M[first.src()] != left.src())
	  return false;
      synchrone = left.forward();
    } while (first.forward());
    if (synchrone) return false;
    if (M.find(first.aim()) == M.end()) 
      M[first.aim()] = left.aim();
    else
      if (M[first.aim()] != left.aim())
	  return false;
    synchrone = true;
    do {
      if (!synchrone) return false;
      synchrone = !left.forward();
    } while (!first.forward());
    if (synchrone) return false;
    synchrone = true;
  }
  return true;
}    


// Helper functions:
// Build a forward cursor adapter intersection from two forward cursors:
template <typename ForwardC1, typename ForwardC2>
inline
intersection_cursor<ForwardC1, ForwardC2>
intersectionc(const ForwardC1 &x1, const ForwardC2 &x2) {
  return intersection_cursor<ForwardC1, ForwardC2>(x1, x2);
}

// Use a non-default order relation on letters:
template <typename ForwardC1, typename ForwardC2, typename LowerThan>
inline
intersection_cursor<ForwardC1, ForwardC2, LowerThan>
intersectionc(const ForwardC1 &x1, const ForwardC2 &x2, const LowerThan&) {
  return intersection_cursor<ForwardC1, ForwardC2, LowerThan>(x1, x2);
}

template <typename ForwardC1, typename ForwardC2>
inline
union_cursor<ForwardC1, ForwardC2>
unionc(const ForwardC1 &x1, const ForwardC2 &x2) {
  return union_cursor<ForwardC1, ForwardC2>(x1, x2);
}

template <typename ForwardC1, typename ForwardC2>
inline
diff_cursor<ForwardC1, ForwardC2>
diffc(const ForwardC1 &x1, const ForwardC2 &x2) {
  return diff_cursor<ForwardC1, ForwardC2>(x1, x2);
}

template <typename ForwardC1, typename ForwardC2>
inline
sym_diff_cursor<ForwardC1, ForwardC2>
sym_diffc(const ForwardC1 &x1, const ForwardC2 &x2) {
  return sym_diff_cursor<ForwardC1, ForwardC2>(x1, x2);
}

template <typename ForwardC1, typename ForwardC2>
inline
concatenation_cursor<ForwardC1, ForwardC2>
concatenationc(const ForwardC1 &x1, const ForwardC2 &x2) {
  return concatenation_cursor<ForwardC1, ForwardC2>(x1, x2);
}

template <typename ForwardC>
inline 
not_cursor<ForwardC> notc(const ForwardC &x) {
  return not_cursor<ForwardC>(x);
}

template <typename EnumerableCharTraits>
inline
sigma_star_cursor<EnumerableCharTraits> sigma_starc() {
  return sigma_star_cursor<EnumerableCharTraits>();
}

} // namespace astl

#endif




