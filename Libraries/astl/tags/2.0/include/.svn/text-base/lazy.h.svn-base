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

#ifndef ASTL_LAZY_H
#define ASTL_LAZY_H

#include <astl.h>
#include <map>
#include <utility>

#define CLASSICAL_LAZY

using namespace std;

namespace astl {


// - solution using template template: the user does not have to instanciate 
//   the DFA by himself, he only has to pass a template as argument
//   for the cursor template (the DFA must be instanciated with Tag ==
//   Cursor::state_type) 
// - This implementation uses a smart pointer to reference the cache
//   and the map between the visited states of the cursor and their copy
//   in the cache for two reasons: 
//   1. We really need a constant time copy constructor
//   2. We want to keep the cache and the mapping as long as the lazy
//      cursor is alive 
template <typename Cursor, 
	  template <typename Sigma, typename Tag> class DFA = DFA_matrix>
class lazy_cursor : public cursor_concept
{
public:
#ifdef CLASSICAL_LAZY
  typedef DFA<typename Cursor::char_traits, typename Cursor::state_type> DFA_type;
#else
  typedef DFA<typename Cursor::char_traits, const typename Cursor::state_type*> DFA_type;
#endif

protected:
  typedef map<typename Cursor::state_type, 
    safe<typename DFA_type::state_type, DFA_type::null_state> > mapper;
  typename DFA_type::state_type my_sink;
  Cursor              c;
  smart_ptr<DFA_type> dfa;
  smart_ptr<mapper>   mapping;  
  safe<typename DFA_type::state_type> q;

public:
  typedef lazy_cursor                   self;
  typedef typename DFA_type::state_type state_type;
  typedef empty_tag                     tag_type;
  typedef typename Cursor::char_type    char_type;
  typedef typename Cursor::char_traits  char_traits;

  lazy_cursor(const Cursor &x)
    : c(x)
    { 
      if (!x.sink()) 
      {
	q = dfa->new_state();
#ifdef CLASSICAL_LAZY
	dfa->tag(q) = c.src();
	(*mapping)[c.src()] = q;
#else
	pair<typename mapper::iterator, bool> p = mapping->insert(make_pair(c.src(), q));
	dfa->tag(q) = &(p.first->first);
#endif
	dfa->final(q) = c.src_final();
	dfa->initial(q);
	my_sink = dfa->new_state();
      }
    }

  // @deprecated
  const Cursor& cursor() {
    c = dfa->tag(q);
    return c;
  }

  const Cursor& adaptee() {
#ifdef CLASSICAL_LAZY
    c = dfa->tag(q);
#else
    c = *dfa->tag(q);
#endif
    return c;
  }

  const DFA_type& cache() const {
    return *dfa;
  }

  state_type src() const {
    return q;
  }

  self& operator=(state_type p) {
    q = p;
    return *this;
  }

  bool src_final() const {
    return dfa->final(q);
  }

  empty_tag src_tag() const {
    return empty_tag();
  }

  bool sink() const {
    return q == DFA_type::null_state;
  }

  state_type sink_state() const {
    return DFA_type::null_state;
  }

  bool exists(const char_type &a) const {
    state_type aim = dfa->delta1(q, a);
    switch (aim) {
    case my_sink :
      return false;

    case DFA_type::null_state :
      Cursor tmp = c;
      tmp = dfa->tag(q);
      return tmp.exists(a);

    default :
      return true;
    }
  }

  bool forward(const char_type &a) {
    state_type tmp = dfa->delta1(q, a);
    if (tmp == my_sink) {
      q = DFA_type::null_state;
      return false;
    }
    if (tmp == DFA_type::null_state)     // transition not in the cache ?
    {
#ifdef CLASSICAL_LAZY
      c = dfa->tag(q);
#else
      c = *dfa->tag(q);
#endif
      if (c.forward(a))    // any transition labelled with 'a' ?
      { 
#ifndef CLASSICAL_LAZY
	pair<typename mapper::iterator, bool> p = mapping->insert(make_pair(c.src(), DFA_type::null_state));
	if (p.second == true) {
	  state_type tmp2 = dfa->new_state();
	  p.first->second = tmp2;
	  dfa->final(tmp2) = c.src_final();
	  dfa->tag(tmp2) = &(p.first->first);
	}
	tmp = p.first->second;
#else
	safe<state_type, DFA_type::null_state> &tmp2 = (*mapping)[c.src()];
	if (tmp2 == DFA_type::null_state) {   // not already been there ?
	  tmp2 = dfa->new_state();
	  dfa->final(tmp2) = c.src_final();
	  dfa->tag(tmp2) = c.src();
	  (*mapping)[c.src()] = tmp2;
	}
	tmp = tmp2;
#endif
	dfa->set_trans(q, a, tmp);
      }
      else {
	dfa->set_trans(q, a, my_sink);
	q = DFA_type::null_state;
	return false;
      }
    }
    q = tmp;
    return true;
  }
};

template <class Cursor>
inline
lazy_cursor<Cursor> lazyc(const Cursor &c) {
  return lazy_cursor<Cursor>(c);
}


// A lazy cursor which copy tags into the cache
template <class Cursor, 
	  template <class Sigma, class Tag> class DFA = DFA_matrix>
class lazy_cursor_tag : public cursor_concept
{
public:
  typedef DFA<typename Cursor::char_traits, 
    pair<typename Cursor::tag_type, typename Cursor::state_type> > DFA_type;

protected:
  typedef map<typename Cursor::state_type, 
    safe<typename DFA_type::state_type, DFA_type::null_state> > mapper;
  typename DFA_type::state_type my_sink;
  Cursor              c;
  smart_ptr<DFA_type> dfa;
  smart_ptr<mapper>   mapping;  
  safe<typename DFA_type::state_type, DFA_type::null_state> q;

public:
  typedef lazy_cursor_tag                   self;
  typedef typename DFA_type::state_type state_type;
  typedef typename Cursor::tag_type     tag_type;
  typedef typename Cursor::char_type    char_type;
  typedef typename Cursor::char_traits  char_traits;

  lazy_cursor_tag(const Cursor &x)
    : c(x)
    { 
      if (!x.sink()) 
      {
	q = dfa->new_state();
	dfa->tag(q).second = c.src();
	dfa->tag(q).first = c.src_tag();
	(*mapping)[c.src()] = q;
	dfa->final(q) = c.src_final();
	dfa->initial(q);
	my_sink = dfa->new_state();
      }
    }

  const DFA_type& cache() const {
    return *dfa;
  }

  state_type src() const {
    return q;
  }

  self& operator=(state_type p) {
    q = p;
    return *this;
  }

  bool src_final() const {
    return dfa->final(q);
  }

  tag_type src_tag() const {
    return dfa->tag(q).first;
  }

  bool sink() const {
    return q == DFA_type::null_state;
  }

  state_type sink_state() const {
    return DFA_type::null_state;
  }

  bool forward(const char_type &a) {
    state_type tmp = dfa->delta1(q, a);
    if (tmp == my_sink) {
      q = DFA_type::null_state;
      return false;
    }
    if (tmp == DFA_type::null_state)     // transition not in the cache ?
    {
      c = dfa->tag(q).second;
      if (c.forward(a))    // any transition labelled with 'a' ?
      { 
	safe<state_type, DFA_type::null_state> &tmp2 = (*mapping)[c.src()];
	if (tmp2 == DFA_type::null_state) {   // not already been there ?
	  tmp2 = dfa->new_state();
	  dfa->final(tmp2) = c.src_final();
	  dfa->tag(tmp2).second = c.src();
	  dfa->tag(tmp2).first = c.src_tag();
	  (*mapping)[c.src()] = tmp2;
	}
	tmp = tmp2;
	dfa->set_trans(q, a, tmp);
      }
      else {
	dfa->set_trans(q, a, my_sink);
	q = DFA_type::null_state;
	return false;
      }
    }
    q = tmp;
    return true;
  }
};

template <class Cursor>
inline
lazy_cursor_tag<Cursor> lazyc_tag(const Cursor &c) {
  return lazy_cursor_tag<Cursor>(c);
}

} // namespace astl
     
#endif // ASTL_LAZY_H
    





