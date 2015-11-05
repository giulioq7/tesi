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

#ifndef ASTL_DEBUG_H
#define ASTL_DEBUG_H

#include <astl.h>
#include <iostream>
#include <utility>  // pair<>

using namespace std;

namespace astl {

// A plain cursor with default behavior checking its state before any
// processing:
template <class DFA>
class cursor_debug : public cursor_concept
{
public:
  typedef cursor_debug              self;
  typedef typename DFA::state_type  state_type;
  typedef typename DFA::tag_type    tag_type;
  typedef typename DFA::char_type   char_type;
  typedef typename DFA::char_traits char_traits;

  // Backward compatibility typedefs:
  typedef state_type  State;
  typedef tag_type    Tag;
  typedef char_type   Alphabet;
  typedef char_traits Sigma;

protected:
  const DFA *dfa;
  state_type q;
  bool singular;
  ostream &out;

public:
  cursor_debug(ostream &o = cerr)
    : dfa(NULL), singular(true), out(o) 
  { }

  cursor_debug(const DFA &a, ostream &o = cerr)
    : dfa(&a), singular(true), out(o) 
  { }

  cursor_debug(const DFA &a, state_type p, ostream &o = cerr)
    : dfa(&a), q(p), out(o)
  {
    if (q == dfa->null_state) {
      out << "cursor::cursor(DFA, state_type) : WARNING" << endl;
      out << "\t cursor set on the sink state" << endl;
    }
  }

  state_type sink_state() const {
    if (singular) {
      out << "cursor::sink_state(): CRITICAL" << endl;
      out << "\t accessing sink state through a singular cursor" << endl;
    }
    return dfa->null_state;
  }

  state_type src() const {
    if (singular) {
      out << "cursor::src(): CRITICAL" << endl;
      out << "\t trying to access state through a singular cursor" << endl;
    }
    return q;
  }

  self& operator= (state_type p) { 
    q = p;
    if (q == dfa->null_state) {
      out << "cursor::operator=(state_type) : WARNING" << endl;
      out << "\t assigning sink state to the cursor" << endl;
    }
    singular = false;
    return *this;
  }

  self& operator=(const self &x) {  
    if (this == &x) {
      out << "cursor::operator=(cursor &x) : WARNING" << endl;
      out << "\t assigning cursor to itself (this == &x)" << endl;
    }
    q = x.q;
    dfa = x.dfa;
    singular = x.singular;
    if (singular) {
      out << "cursor::operator=(cursor &x): CRITICAL" << endl;
      out << "\t x has a singular value" << endl;
    }
    return *this;
  }

  bool operator==(const self &x) const {
    if (this == &x) {
      out << "cursor::operator==(cursor &x) : WARNING" << endl;
      out << "\t comparing cursor to itself (this == &x)" << endl;
    }
    if (singular) {
      out << "cursor::operator==(cursor &x): CRITICAL" << endl;
      out << "\t comparing a singular cursor" << endl;
    }
    return q == x.q;
  }

  bool sink() const {  
    if (singular) {
      out << "cursor::sink(): CRITICAL" << endl;
      out << "\t testing a singular cursor" << endl;
    }
    return q == dfa->null_state;
  }

  bool forward(const char_type &letter) {  
    if (singular) {
      out << "cursor::forward(letter): CRITICAL" << endl;
      out << "\t trying to move along a transition through" 
	"a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {
	out << "cursor::forward(letter): CRITICAL" << endl;
	out << "\t source state is the sink state" << endl;
      }
    q = dfa->delta1(q, letter);
    return q != dfa->null_state;
  }

  bool src_final() const {  
    if (singular) {
      out << "cursor::src_final(): CRITICAL" << endl;
      out << "\t trying to access source state of a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {
	out << "cursor::src_final(): CRITICAL" << endl;
	out << "\t trying to access sink state" << endl;
      }
    return dfa->final(q);
  }

  const tag_type& src_tag() const  {   
    if (singular) {
      out << "cursor::src_tag(): CRITICAL" << endl;
      out << "\t testing a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {
	out << "cursor::src_tag(): CRITICAL" << endl;
	out << "\t trying to access tag of sink state" << endl;
      }
    return dfa->tag(q);
  }

  bool exists(const char_type &letter) const {  // transition exists ?
    if (singular) {
      out << "cursor::exists(): CRITICAL" << endl;
      out << "\t testing for a transition through a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {  
	out << "cursor::exists(): CRITICAL" << endl;
	out << "\t testing for an outgoing transition of sink state" << endl;
      }
    return dfa->delta1(q, letter) != dfa->null_state;
  }
};

// A forward cursor with default behavior checking its state before
// any processing:
template <class DFA>
class forward_cursor_debug : public forward_cursor_concept
{
public:
  typedef forward_cursor_debug self;
  typedef typename DFA::state_type  state_type;
  typedef typename DFA::tag_type    tag_type;
  typedef typename DFA::char_type   char_type;
  typedef typename DFA::char_traits char_traits;

  // Backward compatibility typedefs:
  typedef state_type  State;
  typedef tag_type    Tag;
  typedef char_type   Alphabet;
  typedef char_traits Sigma;

protected:
  typedef typename DFA::edges_type::const_iterator edges_iterator;
  const DFA *dfa;
  state_type q;
  edges_iterator transition;
  bool dereferenceable, singular;  // singular: the only allowed
				   // operation is assignment 
  // dereferenceable: letter and aim of current transition
  //                  are accessible
  ostream &out;

public:
  forward_cursor_debug(const DFA &a, state_type p, 
		       const char_type& Letter, ostream &o = cerr) 
    : dfa(&a), q(p), dereferenceable(false), singular(false), out(o)
  { 
    if (q == dfa->null_state) 
      out << "forward_cursor::forward_cursor(DFA, state_type, letter): warning" << endl
	   << "\t cursor initialized with null state" << endl;
    else
      if ((transition = dfa->delta2(q).find(Letter)) == dfa->delta2(q).end()) {
	out << "forward_cursor::forward_cursor(DFA, state_type, letter): warning" << endl;
	out << "\t cursor set on a sink transition" << endl; 
      }
      else
	dereferenceable = true;
  }
  
  forward_cursor_debug(const DFA &a, state_type p, 
		       edges_iterator t, ostream &o = cerr)
    : dfa(&a), q(p), transition(t), dereferenceable(false), singular(false), out(o)
  { 
    if (q == dfa->null_state) {
      out << "forward_cursor::forward_cursor(DFA, state_type, edges_iterator):";
      out << " warning" << endl << "\t cursor initialized with null state" << endl;
    } 
    else 
      dereferenceable = t != dfa->delta2(q).end();
  }
  
  forward_cursor_debug(const DFA &a, state_type p, ostream &o = cerr)
    : dfa(&a), q(p), dereferenceable(false), singular(false), out(o)
  { 
    if (q == dfa->null_state) {
      out << "forward_cursor::forward_cursor(DFA, state_type): warning" << endl;
      out << "\t cursor initialized with null state" << endl;
    } 
  }

  forward_cursor_debug(const DFA &a, ostream &o = cerr)
    : dfa(&a), q(a.initial()), dereferenceable(false), singular(true), out(o)
  { 
    if (q == dfa->null_state) {
      out << "forward_cursor::forward_cursor(DFA): warning" << endl;
      out << "\t cursor initialized with initial state which is null" << endl;
    } 
  }

  forward_cursor_debug(ostream &o = cerr)
    : dfa(NULL), dereferenceable(false), singular(true), out(o)
  { }

  self& operator= (state_type p) { 
    //    out << "\t assigning" << endl;
    q = p;
    dereferenceable = false;
    if (q == dfa->null_state) {
      out << "forward_cursor::operator= (state_type): warning" << endl;
      out << "\t assigning null state" << endl;
    }
    singular = false;
    return *this;
  }

  self& operator= (const self &x) { 
    // out << "\t assigning self " << endl;
    if (this == &x) {
      out << "forward_cursor::operator= (forward_cursor x): warning" << endl;
      out << "\t assigning cursor to itself (this == &x)" << endl;
    }
    q = x.q;
    transition = x.transition;
    dereferenceable = x.dereferenceable;
    singular = x.singular;
    dfa = x.dfa;
    if (singular) {
      out << "forward_cursor::operator= (forward_cursor x): warning" << endl;
      out << "\t x has a singular value" << endl;
    }
    if (dfa == NULL) {
      out << "forward_cursor::operator= (forward_cursor x): WARNING" << endl;
      out << "\t assigning a non-initialized cursor x (dfa == NULL)" << endl;
    }
      
    return *this;
  }

  bool operator== (const self &x) const {
    if (this == &x) {
      out << "forward_cursor::operator== : warning" << endl;
      out << "\t comparing cursor with itself" << endl;
    }
    return (q == x.q && transition == x.transition);
  }

  char_type letter() const {
    if (singular) {
      out << "forward_cursor::letter() : CRITICAL" << endl;
      out << "\t trying to access transition through a singular cursor" << endl;
    } 
    else
      if (!dereferenceable) {
	out << "forward_cursor::letter() : CRITICAL" << endl;
	out << "\t trying to access transition through a non dereferenceable cursor" << endl;
	out << "\t state = " << q << endl;
      }
    return (*transition).first;
  }

  bool sink() const {
    if (singular) {
      out << "forward_cursor::sink(): CRITICAL" << endl;
      out << "\t testing for sink state on a singular cursor" << endl;
    } 
    return q == dfa->null_state;
  }

  state_type sink_state() const { 
    // out << "\t sink_state(  " << endl;
    if (singular) {
      out << "forward_cursor::sink_state(): CRITICAL" << endl;
      out << "\t requesting the sink state value from a singular cursor" << endl;
    } 
    return dfa->null_state;
  }

  // Obsolet:
  void to_sink() {
    if (singular) {
      out << "forward_cursor::to_sink(): CRITICAL" << endl;
      out << "\t moving a singular cursor to the sink state" << endl;
    } 
    if (dfa == NULL) {
      out << "forward_cursor::to_sink() : CRITICAL" << endl;
      out << "\t trying to move to the sink state with a non-initialized cursor (dfa == NULL)" << endl;
    } 
    q = dfa->null_state;
  }
    

  bool first() {
    if (singular) {
      out << "forward_cursor::first() : CRITICAL" << endl;
      out << "\t seeking first outgoing transition of a singular cursor" << endl;
    } 
    else
      if (q == dfa->null_state) {
	out << "forward_cursor::first() : CRITICAL" << endl;
	out << "\t trying to access first transition of sink source state" << endl;
      }
    return dereferenceable = (transition = dfa->delta2(q).begin()) != dfa->delta2(q).end();
  }
  
  bool next() { 
    if (singular) {
      out << "forward_cursor::next() : CRITICAL" << endl;
      out << "\t seeking next transition of a singular cursor" << endl;
    } 
    else
      if (q == dfa->null_state) {
	out << "forward_cursor::next() : CRITICAL" << endl;
	out << "\t seeking next transition of sink source state" << endl;
      }
      else
	if (!dereferenceable) {
	  out << "forward_cursor::next(): CRITICAL" << endl;
	  out << "\t seeking next transition of a non dereferenceable cursor" << endl;
	}
    return dereferenceable = (++transition != dfa->delta2(q).end());
  }

  void forward() {
    if (singular) {
      out << "forward_cursor::forward() : CRITICAL" << endl;
      out << "\t trying to move along transition of a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {
	out << "forward_cursor::forward() : CRITICAL" << endl;
	out << "\t trying to move along transition of sink source state" << endl;
      }
      else
	if (!dereferenceable) {
	  out << "forward_cursor::forward() : CRITICAL" << endl;
	  out << "\t trying to move along transition of a non dereferenceable cursor" << endl;
	}
    q = (*transition).second;
    dereferenceable = false;
  }

  bool find(const char_type &letter)  {
    if (singular) {
      out << "forward_cursor::find() : CRITICAL" << endl;
      out << "\t trying to find an outgoing transition through a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {
	out << "forward_cursor::find() : CRITICAL" << endl;
	out << "\t trying to find an outgoing transition of sink source state" << endl;
      }
    edges_iterator tmp = dfa->delta2(q).find(letter);
    if (tmp != dfa->delta2(q).end()) {
      transition = tmp;
      dereferenceable = true; // out << " Found " << endl ;
      return true;
    }
    //    out << "not Found " << endl ;
    return false;
  }

  bool forward(const char_type &letter) {
    if (singular) {
      out << "forward_cursor::forward(letter) : CRITICAL" << endl;
      out << "\t trying to move along transition through a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {
	out << "forward_cursor::forward(letter) : CRITICAL" << endl;
	out << "\t trying to move along outgoing transition of sink state" << endl;
      }
    dereferenceable = false;
    q = dfa->delta1(q, letter);
    return q != dfa->null_state;
  }

  state_type src() const {
    if (singular) {
      out << "forward_cursor::src(): CRITICAL" << endl;
      out << "\t trying to access source state of a singular cursor" << endl;
    }
    return q;
  }

  state_type aim() const {
    if (singular) {
      out << "forward_cursor::aim() : CRITICAL" << endl;
      out << "\t trying to access aim state of a singular cursor" << endl;
    }
    else
      if (!dereferenceable) {
	out << "forward_cursor::aim() : CRITICAL" << endl;
	out << "\t trying to access aim state of a non dereferenceable cursor" << endl;
      }
    return (*transition).second;
  }

  bool src_final() const {
    if (singular) {
      out << "forward_cursor::src_final(): CRITICAL" << endl;
      out << "\t trying to access source state of a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {
	out << "forward_cursor::src_final(): CRITICAL" << endl;
	out << "\t trying to access sink state" << endl;
      }
    return dfa->final(q);
  }

  bool aim_final() const {
    if (singular) {
      out << "forward_cursor::aim_final() : CRITICAL" << endl;
      out << "\t trying to access aim state of a singular cursor" << endl;
    }
    else
      if (!dereferenceable) {
	out << "forward_cursor::aim_final() : CRITICAL" << endl;
	out << "\t trying to access aim state of a non dereferenceable cursor" << endl;
      }
    return dfa->final((*transition).second);
  }

  bool exists(const char_type &letter) const {  // transition exists ?
    //    out << "\t exist " << endl;
    if (singular) {
      out << "forward_cursor::exist() : CRITICAL" << endl;
      out << "\t testing for a transition through a singular cursor" << endl;
    }
    else if (!dereferenceable) {
      out << "forward_cursor::exist() : CRITICAL" << endl;
      out << "\t testing for a transition through a non dereferenceable cursor" << endl;
    }
    else
      if (q == dfa->null_state) {  
	out << "forward_cursor::exist() : CRITICAL" << endl;
	out << "\t testing for a transition from sink state" << endl;
      }
    return dfa->delta1(q, letter) != dfa->null_state;
  }

  const tag_type& src_tag() const  {
    if (singular) {
      out << "forward_cursor::src_tag() : CRITICAL" << endl;
      out << "\t trying to access source tag of a singular cursor" << endl;
    }
    else
      if (q == dfa->null_state) {
	out << "forward_cursor::src_tag(): CRITICAL" << endl;
	out << "\t trying to access tag of sink state" << endl;
      }
    return dfa->tag(q);
  }

  const tag_type& aim_tag() const {
    if (singular) {
      out << "forward_cursor::aim_tag() : CRITICAL" << endl;
      out << "\t trying to access aim tag of a singular cursor" << endl;
    }
    else
      if (!dereferenceable) {
	out << "forward_cursor::aim_tag() : CRITICAL" << endl;
	out << "\t trying to access aim tag of a non dereferenceable cursor";
      }
    return dfa->tag((*transition).second);
  }

  // Not a standard requirement:
  friend ostream& operator << (ostream &out, const self &x) {
    out << x.q << " , '";
    if (x.singular) out << "singular'";
    else
      if (!x.dereferenceable) {
	if (x.transition == x.dfa->delta2(x.q).end()) out << "end of range'";
	else
	  out << "not dereferenceable'";
      }
      else
	out << (*(x.transition)).first << "', " << (*(x.transition)).second;
    return out;
  }

  // Not a standard requirement:
  bool operator < (const self &x) const {   
    // STL containers need operator < on
    // their value_type (to define < on themselves)
    if (singular) 
      out << "forward_cursor::operator< (forward_cursor) : caution" << endl
	   << "\t comparing singular left cursor with"
	   << (x.singular ? " singular " : " ") << "right cursor" << endl; 
    else
      if (x.singular)
	out << "forward_cursor::operator< (forward_cursor) : caution" << endl
	     << "\t comparing left cursor with singular right cursor" << endl; 
    return q < x.q || (q == x.q && transition < x.transition);
  }
};

// Helper functions:
// Build a debug plain cursor from a DFA:
template <class DFA>
cursor_debug<DFA> debugplainc(const DFA &a, ostream &out = cerr) {
  return cursor_debug<DFA>(a, a.initial(), out);
}

// Build a debug forward cursor from a DFA:
template <class DFA>
forward_cursor_debug<DFA> debugc(const DFA &a, ostream &out = cerr) {
  return forward_cursor_debug<DFA>(a, a.initial(), out);
}

template <class Cursor>
class cursor_trace : public cursor_concept
{
public:
  typedef cursor_trace              self;
  typedef typename Cursor::state_type  state_type;
  typedef typename Cursor::tag_type    tag_type;
  typedef typename Cursor::char_type   char_type;
  typedef typename Cursor::char_traits char_traits;
  static int id;

  // Backward compatibility typedefs:
  typedef state_type  State;
  typedef tag_type    Tag;
  typedef char_type   Alphabet;
  typedef char_traits Sigma;

protected:
  Cursor c;
  int ID;
  ostream &out;

public:
  cursor_trace(ostream &o = cerr)
    : c(), ID(++id), out(o) { 
    out << "cursor[" << ID << "]::cursor()" << endl;
  }

  cursor_trace(const Cursor &x, ostream &o = cerr)
    : c(x), ID(++id), out(o) {
    out << "cursor[" << ID << "]::cursor(Cursor " << &x << ")" << endl;
  }

  cursor_trace(const self &x) 
    : c(x.c), ID(x.ID), out(x.out) {
    out << "cursor[" << ID << "]::cursor(self)" << endl;
  }

  ~cursor_trace() {
    out << "cursor[" << ID << "]::~cursor()" << endl;
  }

  state_type src() const {
    out << "cursor[" << ID << "]::src() == " << c.src() << endl;
    return c.src();
  }

  state_type sink_state() const {
    out << "cursor[" << ID << "]::sink_state() == " << c.sink_state() << endl;
    return c.sink_state();
  }

  self& operator= (state_type p) {
    out << "cursor[" << ID << "]::operator=(state_type " << p << ")" << endl;
    c = p;
    return *this;
  }

  self& operator= (const self &x) {
    out << "cursor[" << ID << "]::operator=(self [" << x.ID << "])" << endl;
    c = x.c;
    return *this;
  }

  bool operator== (const self &x) const {
    out << "cursor[" << ID << "]::operator==(self [" << x.ID << "]) == " << (c == x.c) << endl;
    return c == x.c;
  }

  bool sink() const {
    out << "cursor[" << ID << "]::sink() == " << c.sink() << endl;
    return c.sink();
  }

  bool forward(const char_type &letter) {
    out << "cursor[" << ID << "]::forward('" << letter << "') == ";
    out << c.forward(letter) << endl;
    return !c.sink();
  }

  bool src_final() const {
    out << "cursor[" << ID << "]::src_final() == " << c.src_final() << endl;
    return c.src_final();
  }

  const tag_type& src_tag() const  {
    out << "cursor[" << ID << "]::src_tag()" << endl;
    return c.src_tag();
  }

  bool exists(const char_type &letter) const {      // transition exists ?
    out << "cursor[" << ID << "]::exists(int " << letter << ") == " << c.exists(letter) << endl;
    return c.exists(letter);
  }
};

template <class Cursor> int cursor_trace<Cursor>::id = 0;

template <class ForwardCursor>
class forward_cursor_trace : public forward_cursor_concept
{
public:
  typedef forward_cursor_trace             self;
  typedef typename ForwardCursor::state_type  state_type;
  typedef typename ForwardCursor::tag_type    tag_type;
  typedef typename ForwardCursor::char_type   char_type;
  typedef typename ForwardCursor::char_traits char_traits;

  // Backward compatibility typedefs:
  typedef state_type  State;
  typedef tag_type    Tag;
  typedef char_type   Alphabet;
  typedef char_traits Sigma;

protected:
  ForwardCursor c;
  static int id;
  int ID;
  ostream &out;

public:
  forward_cursor_trace(const ForwardCursor &x, ostream &o = cerr)
    : c(x), ID(++id), out(o) { 
    out << "fcursor[" << ID << "]::fcursor(forward_cursor " << &x << ")" << endl;
  }
  
  forward_cursor_trace(ostream &o = cerr)
    : c(), ID(++id), out(o) { 
    out << "fcursor[" << ID << "]::fcursor()";
  }

  const ForwardCursor& cursor() const { return c; }

  forward_cursor_trace(const self &x) 
    : c(x.c), ID(++id), out(x.out) { 
    out << "fcursor[" << ID << "](self[" << x.ID << "])" << endl;
  }

  ~forward_cursor_trace() {
    out << "fcursor[" << ID << "]::~fcursor()" << endl;
  }
  
  self& operator= (state_type p) {
    out << "fcursor[" << ID << "]::operator=(state_type " << p << ")" << endl;
    c = p;
    return *this;
  }

  self& operator= (const self &x) {
    out << "fcursor[" << ID << "]::operator=(self [" << x.ID << "])" << endl;
    c = x.c;
    return *this;
  }

  bool operator== (const self &x) const {
    out << "fcursor[" << ID << "]::operator==(self [" << x.ID << "]) == " << (c == x.c) << endl;
    return (c == x.c);
  }

  char_type letter() const {
    out << "fcursor[" << ID << "]::letter() == '" << c.letter() << "'" << endl;
    return c.letter();
  }

  bool first() {
    out << "fcursor[" << ID << "]::first() == ";
    if (c.first())
      out << "(" << c.src() << ", '" << c.letter() << "', " << c.aim() << ")" << endl;
    else
      out << "0" << endl;
    return c.first();
  }
  
  bool next() {
    bool r = c.next();
    out << "fcursor[" << ID << "]::next() == ";
    if (r)
       out << "(" << c.src() << ", '" << c.letter() << "', " << c.aim() << ")" << endl;
    else
      out << "0" << endl;     
    return r;
  }

  bool forward(const char_type &letter) {
    out << "fcursor[" << ID << "]::forward('" << letter << "') == ";
    if (c.forward(letter))
      out << c.src() << endl;
    else {
      out << "0 && sink() == " << c.sink() << endl;
    }
    return !c.sink();
  }

  void forward() {
    c.forward();
    out << "fcursor[" << ID << "]::forward() == " << c.src() << endl;
  }

  bool find(const char_type &letter)  {
    out << "fcursor[" << ID << "]::find('" << letter << "') == ";
    if (c.find(letter))
      out << "(" << c.src() << ", '" << c.letter() << "', " << c.aim() << ")" << endl;
    else
      out << "0" << endl;     
    return c.find(letter);
  }

  state_type src() const {
    out << "fcursor[" << ID << "]::src() == " << c.src() << endl;
    return c.src();
  }

  bool src_final() const {
    out << "fcursor[" << ID << "]::src_final() == " << c.src_final() << endl;
    return c.src_final();
  }

  state_type aim() const {

    out << "fcursor[" << ID << "]::aim() == " << c.aim() << endl;
    return c.aim();
  }

  bool aim_final() const {
    out << "fcursor[" << ID << "]::aim_final() == " << c.aim_final() << endl;
    return c.aim_final();
  }

  tag_type aim_tag() const {
    out << "fcursor[" << ID << "]::aim_tag() == " << c.aim_tag() << endl;
    return c.aim_tag();
  }

  tag_type src_tag() const {
    out << "fcursor[" << ID << "]::src_tag() == " << c.src_tag() << endl;
    return c.src_tag();
  }

  bool sink() const {
    out << "fcursor[" << ID << "]::sink() == " << c.sink() << endl;
    return c.sink();
  }

  state_type sink_state() const {
    out << "fcursor[" << ID << "]::sink_state() == " << c.sink_state() << endl;
    return c.sink_state();
  }

  bool exists(const char_type &letter) const {      // transition exists ?
    out << "fcursor[" << ID << "]::exists(" << letter << ") == " << c.exists(letter) << endl;
    return c.exists(letter);
  }
};

template <class ForwardCursor> int forward_cursor_trace<ForwardCursor>::id = 0;

template <class ForwardCursor>
forward_cursor_trace<ForwardCursor> 
tracec(const ForwardCursor &x, ostream &out = cerr) {
  return forward_cursor_trace<ForwardCursor>(x, out);
}

} // namespace astl

#endif // ASTL_CURSOR_DEBUG
