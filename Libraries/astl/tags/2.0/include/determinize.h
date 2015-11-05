/*
 * ASTL - the Automaton Standard Template Library.
 * C++ generic components for Finite State Automata handling.
 * Copyright (C) 2000-2005 Vincent Le Maout (vincent.lemaout@chello.fr).
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

#ifndef ASTL_DETERMINIZE_H
#define ASTL_DETERMINIZE_H

#include <astl.h>
#include <closure.h>
#include <set> 
#include <iterator>     // insert_iterator 

using namespace std; 

namespace astl { 

// Map a set of tag to a single tag (tag determinization)
template <typename NFA>
struct default_tag_mapper 
{ 
  typedef empty result_type; 

  // Requirements on types:
  // InputIterator::value_type == NFA::state_type
  template <typename InputIterator> 
  result_type operator()(const NFA&, InputIterator, 
			 InputIterator) const { 
    return empty();
  } 
}; 

/**
   A plain cursor to a non-deterministic automaton computing the
   determinization on-the-fly.

   @helpers #plaindc()
*/
template <typename NFA, typename TagMapper = default_tag_mapper<NFA> > 
class dcursor : public cursor_concept 
{ 
public: 
  typedef dcursor                         self; 
  typedef set<typename NFA::state_type>   state_type; 
  typedef typename TagMapper::result_type tag_type; 
  typedef typename NFA::char_traits       char_traits; 
  typedef typename NFA::char_type         char_type; 

protected: 
  const NFA *nfa; 
  state_type q; 

public: 
  dcursor() 
  { } 

  dcursor(const NFA &a) 
    : nfa(&a) 
  { } 
 
  dcursor(const NFA &a, typename NFA::state_type p) 
    : nfa(&a) { 
    if (p != a.null_state) q.insert(p);
  } 

  dcursor(const NFA &a, const state_type& p) 
    : nfa(&a), q(p) 
  { } 

  state_type src() const { return q; } 

  self& operator=(const typename NFA::state_type& p) { 
    q.clear();
    q.insert(p);
    return *this; 
  } 

  self& operator=(const state_type& p) { 
    q = p; 
    return *this; 
  } 

  self& operator=(const self &x) { 
    nfa = x.nfa; 
    q = x.q; 
    return *this; 
  } 

  bool operator==(const self &x) const { return q == x.q && nfa == x.nfa; } 

  bool sink() const { return q.empty(); } 

  state_type sink_state() const { return state_type(); } 
  
  bool forward(const char_type& a) { 
    state_type p; 
    insert_iterator<state_type> j(p, p.begin()); 
    for(typename state_type::const_iterator i = q.begin(); i != q.end(); ++i) 
      nfa->delta1(*i, a, j); 
    q.swap(p);   // constant time assignment 
    return !sink(); 
  } 
  
  bool src_final() const { 
    for(typename state_type::const_iterator i = q.begin(); i != q.end(); ++i) 
      if (nfa->final(*i)) return true; 
    return false; 
  } 

  tag_type src_tag() const { return TagMapper()(*nfa, q.begin(), q.end()); } 
  
  bool exists(const char_type& a) const { 
    state_type p; 
    insert_iterator<state_type> j(p, p.begin()); 
    for(typename state_type::const_iterator i = q.begin(); i != q.end(); ++i) { 
      nfa->delta1(*i, a, j); 
      if (!p.empty()) return true; 
    } 
    return false; 
  } 
}; 

/**
   A forward cursor to a non-deterministic automaton computing the
   determinization on-the-fly.

   @helpers #forwarddc()
*/
template <typename NFA, typename TagMapper = default_tag_mapper<NFA> > 
class forward_dcursor 
  : public dcursor<NFA, TagMapper>, public forward_cursor_concept 
{ 
public: 
  typedef forward_dcursor             self; 
  typedef dcursor<NFA, TagMapper>     super; 
  typedef typename super::char_traits char_traits;
  typedef typename super::tag_type    tag_type;
  typedef typename super::char_type   char_type;
  typedef typename super::state_type  state_type;
  typedef forward_cursor_concept      concept;


protected: 
  char_type a;
  state_type p; 

public: 
  forward_dcursor() 
  { } 

  forward_dcursor(const NFA &n) 
    : super(n) 
  { } 

  forward_dcursor(const NFA &n, typename NFA::state_type p) 
    : super(n, p) 
  { } 

  forward_dcursor(const NFA &n, const state_type& s) 
    : super(n, s) 
  { } 
  
  self& operator=(const self &x) { 
    super::operator=(x); 
    a = x.a;
    p = x.p;
    return *this; 
  } 

  self& operator=(const state_type &x) { 
    super::operator=(x); 
    return *this; 
  } 

  self& operator=(typename NFA::state_type x) { 
    super::operator=(x); 
    return *this; 
  } 

  bool operator==(const self &x) const { 
    return super::operator==(x) && super::char_traits::eq(a, x.a)
      && p == x.p;
  } 

  char_type letter() const { return a;} 

  bool first() { 
    p.clear();
    insert_iterator<state_type> j(p, p.begin()); 

    typename state_type::const_iterator i;
    typename NFA::edges_type::const_iterator first 
      = nfa->delta2(*q.begin()).end();
    typename NFA::edges_type::const_iterator last = first, k;
    for(i = q.begin(); i != q.end(); ++i) {
      k = nfa->delta2(*i).begin();
      if (k != nfa->delta2(*i).end()) 
	if (first == last || super::char_traits::lt((*k).first, (*first).first)) 
	  first = k;
    }

    if (first == last) return false;
    a = (*first).first;
    for(i = q.begin(); i != q.end(); ++i)
      nfa->delta1(*i, a, j);
      
    return !p.empty();
  } 

  bool next() { 
    p.clear();
    insert_iterator<state_type> j(p, p.begin()); 
    char_type tmp = a;

    typename state_type::const_iterator i;
    typename NFA::edges_type::const_iterator k;
    for(i = q.begin(); i != q.end(); ++i) {
      k = nfa->delta2(*i).upper_bound(tmp);
      if (k != nfa->delta2(*i).end())
	if (tmp == a || super::char_traits::lt((*k).first, a)) 
	  a = (*k).first;
    }

    if (a == tmp) return false;
    for(i = q.begin(); i != q.end(); ++i)
      nfa->delta1(*i, a, j);

    return !p.empty();
  }                

  void forward() { q.swap(p); } 

  bool forward(const char_type& c) { return super::forward(c); }

  bool find(const char_type& c) {
    p.clear();
    insert_iterator<state_type> j(p, p.begin()); 
    a = c;

    for(typename state_type::const_iterator i = q.begin(); 
	i != q.end(); ++i) 
      nfa->delta1(*i, c, j); 

    return !p.empty();
  } 

  state_type aim() const { return p; } 

  bool aim_final() const { 
    for(typename state_type::const_iterator i = p.begin(); i != p.end(); ++i) 
      if (nfa->final(*i)) return true; 
    return false; 
  } 
                 
  tag_type aim_tag() const { 
    return TagMapper()(*nfa, p.begin(), p.end()); 
  } 

protected:
  using super::nfa;
  using super::q;
}; 

template <typename NFA, typename TagMapper = default_tag_mapper<NFA> >
class async_forward_dcursor : public forward_dcursor<NFA, TagMapper>
{
public:
  typedef async_forward_dcursor           self;
  typedef forward_dcursor<NFA, TagMapper> super;
  typedef typename super::char_type       char_type;
  typedef typename super::state_type      state_type;

protected:
  char_type x;

public:
  async_forward_dcursor()
    : super()
  { }

  async_forward_dcursor(const NFA& n, const char_type &a)
    : super(n), x(a)
  { }

  async_forward_dcursor(const NFA &n, const char_type &a, 
			typename NFA::state_type p) 
    : super(n, p), x(a) { 
    epsilon_closure(q);
  } 

  async_forward_dcursor(const NFA &n, const char_type &a, const state_type& s) 
    : super(n, s), x(a) { 
    epsilon_closure(q);
  } 
  
  self& operator=(const self &y) { 
    super::operator=(y);
    x = y.x;
    return *this; 
  } 

  self& operator=(const state_type &y) { 
    super::operator=(y);
    epsilon_closure(q);
    return *this; 
  } 

  self& operator=(typename NFA::state_type y) { 
    super::operator=(y);
    epsilon_closure(q);
    return *this; 
  } 

  void forward() { super::forward(); }
    
  bool forward(const char_type& a) {
    if (super::forward(a)) {
      epsilon_closure(q);
      return true;
    }
    return false;
  }

  bool find(const char_type& a) {
    if (super::find(a)) {
      epsilon_closure(p);
      return true;
    }
    return false;
  }
      
  bool first() {
    if (super::first()) {
      if (super::letter() == x)
	return next();
      else {
	epsilon_closure(p);
	return true;
      }
    }
    return false;
  }
      
  bool next() {
    if (super::next()) {
      if (super::letter() == x) 
	return next();
      else {
	epsilon_closure(p);
	return true;
      }
    }
    return false;
  }      

protected:
  using super::q;
  using super::nfa;
  using super::p;

  void epsilon_closure(state_type &s) {
    state_type tmp = s;
    insert_iterator<state_type> c(s, s.begin());
    closure_cursor<transition_cursor<NFA> > t(transitionc(*nfa, NFA::null_state), x);
    for(typename state_type::const_iterator i = tmp.begin(); i != tmp.end(); ++i) {
      t = *i;
      crop(c, dfirst_markc(t));
    }
  }
};

/** Builds a plain determinization cursor pointing to the initial
    state of the non-deterministic automaton @c a.
*/
template <typename NFA>
inline
dcursor<NFA> plaindc(const NFA &a) {
  return dcursor<NFA>(a, a.initial());
}

/** Builds a plain determinization cursor pointing to the state
    @c q of the non-deterministic automaton @c a.
*/
template <typename NFA>
inline
dcursor<NFA> plaindc(const NFA &a, typename NFA::state_type q) {
  return dcursor<NFA>(a, q);
}

/** Builds a forward determinization cursor pointing to the initial
    state of the non-deterministic automaton @c a.
*/
template <typename NFA>
inline
forward_dcursor<NFA> forwarddc(const NFA &a) {
  return forward_dcursor<NFA>(a, a.initial());
}

/** Builds a forward determinization cursor pointing to the state
    @c q of the non-deterministic automaton @c a.
*/
template <typename NFA>
inline
forward_dcursor<NFA> forwarddc(const NFA &a, typename NFA::state_type q) {
  return forward_dcursor<NFA>(a, q);
}

template <typename NFA>
inline
async_forward_dcursor<NFA> 
async_forwarddc(const NFA &a, const typename NFA::char_type &c) {
  return async_forward_dcursor<NFA>(a, c, a.initial());
}

template <typename NFA>
inline
async_forward_dcursor<NFA> 
async_forwarddc(const NFA &a, const typename NFA::char_type &c,
		typename NFA::state_type q) {
  return async_forward_dcursor<NFA>(a, c, q);
}

#ifdef NEWSCHOOL
template <typename TransitionCursor, typename TagMapper = default_tag_mapper<NFA> > 
class forward_dcursor 
  : public public forward_cursor_concept 
{ 
public: 
  struct transitionc_order : public binary_function<TransitionCursor, TransitionCursor, bool>
  {
    bool operator()(const TransitionCursor &x, const TransitionCursor &y) const {
      return x.src() < y.src();
    }
  };
  typedef forward_dcursor             self; 
  typedef typename TransitionCursor::char_traits char_traits;
  typedef typename TagMapper::result_type tag_type; 
  typedef typename TransitionCursor::char_type   char_type;
  typedef typename set<TransitionCursor, transitionc_order>  state_type;
  typedef forward_cursor_concept      concept;


protected:
  
};

#endif

} // namespace astl 

#endif // ASTL_DETERMINIZE_H




