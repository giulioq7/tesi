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

#ifndef ASTL_HASH_H
#define ASTL_HASH_H

#include <astl.h>
#include <iostream>

using namespace std;

namespace astl {

// Requirements (see the algorithm make_hash)
// 1. DFA::Tag::wc()
template <typename ForwardCursor>
class hash_cursor : public ForwardCursor
{
protected:
  int hashv;

public:
  typedef hash_cursor                 self;
  typedef ForwardCursor               super;
  typedef typename super::state_type  state_type;
  typedef typename super::char_type   char_type;
  typedef typename super::tag_type    tag_type;
  typedef typename super::char_traits char_traits;
  typedef typename super::concept     concept;

  // Backward compatibility typedefs:
  typedef state_type  State;
  typedef tag_type    Tag;
  typedef char_type   Alphabet;
  typedef char_traits Sigma;

  hash_cursor()
    : super()
  { }

  hash_cursor(const ForwardCursor &c)
    : super(c), hashv(0)
  { }

  self& operator=(state_type p) {
    super::operator=(p);
    hashv = 0;       // warning: might get into troubles if p != dfa->initial()
    return *this;
  }

  bool first() {
    if (super::first()) {
      if (super::aim_final()) ++hashv;
      return true;
    }
    return false;
  }

  bool next() {
    hashv += super::aim_tag().wc();
    if (super::next()) {
      if (super::aim_final()) ++hashv;
      return true;
    }
    return false;
  }

  bool forward(const char_type &a) {
    if (first()) {
      while (super::letter() != a)
	if (!next()) {
	  *this = super::sink_state();
	  return false;
	}
      forward();
      return true;
    }
    return false;
  }

  void forward() {
    super::forward();
  }

  int hash_value() const {
    return hashv;
  }
};

template <typename DFA>
inline
hash_cursor<forward_cursor<DFA> > hashc(const DFA &a) {
  return hashc(forwardc(a, a.initial()));
}

template <typename DFA>
inline
hash_cursor<forward_cursor<DFA> > 
hashc(const DFA &a, const typename DFA::state_type &q) {
  return hash_cursor<forward_cursor<DFA> >(forwardc(a, q));
}

struct hash_tag
{
  int wc_;

  hash_tag(int i = 0)
    : wc_(i)
  { }

  // needed by stream input with clone_cursor & clone:
  hash_tag& operator=(const empty&) { return *this; }
  int& wc()       { return wc_; }
  int  wc() const { return wc_; }
};

inline bool operator<(const hash_tag &x, const hash_tag &y) {
  return x.wc() < y.wc();
}

inline bool operator==(const hash_tag &x, const hash_tag &y) {
  return x.wc() == y.wc();
}

inline ostream& operator<<(ostream& out, const hash_tag& h) {
  return out << h.wc();
}

inline istream& operator>>(istream& in, hash_tag& h) {
  return in >> h.wc();
}

// make_hash computes the size of each state language (recognized words count)
// it stores the result in the state tag which must at least have the same
// interface as struct hash_tag above, that is:
// type requirements:
// int DFA::Tag::wc() const;
// int& DFA::Tag::wc(); 
template <typename DFA>
inline
void make_hash(DFA& a) 
{
#ifndef _MSC_VER
  make_hash_(a, dfirst_markc(a));
#else
  make_hash_(a, dfirst_markc(forwardc(a)));
#endif
}

template <typename DFA, typename DFirstCursor>
void make_hash_(DFA& a, DFirstCursor first, DFirstCursor last = DFirstCursor())
{
  while (first != last) {
    while (first.forward());
    do {
      if (first.aim_final()) a.tag(first.src()).wc() += 1;
      a.tag(first.src()).wc() += a.tag(first.aim()).wc();
    } while (!first.forward());
  }
}

/** Return the hash value for the word [first, last) computed from a
    hashing DFA */
// Requirements: c points to the initial state
template <typename ForwardCursorOrDFA, typename InputIterator>
inline
int hash_value(const ForwardCursorOrDFA &x, InputIterator first, 
	       InputIterator last) {
  return hash_value(x, first, last, x);
}

template <typename ForwardCursor, typename InputIterator>
inline
int hash_value(const ForwardCursor &c, InputIterator first, 
	       InputIterator last, const cursor_concept&)
{
  if (!c.sink()) {
    hash_cursor<ForwardCursor> h(c);
    while (first != last && h.forward(*first)) 
      ++first;
    if (first == last && h.src_final()) 
      return h.hash_value();
  }
  return 0;  // word not found
}

template <typename DFA, typename InputIterator>
inline
int hash_value(const DFA &x, InputIterator first, 
	       InputIterator last, const DFA_concept&) {
  return hash_value(forwardc(x), first, last);
}

template <typename ForwardCursor, typename Container>
inline
int hash_value(const ForwardCursor &c, const Container &C)
{
  if (!c.sink()) {
    hash_cursor<ForwardCursor> h(c);
    typename Container::const_iterator first = C.begin();
    while (first != C.end() && h.forward(*first)) 
      ++first;
    if (first == C.end() && h.src_final()) 
      return h.hash_value();
  }
  return 0;  // word not found
}

// Write the word corresponding to the id into x
// Requirements: 
// - c is a forward cursor pointing to a hashing DFA
// - id > 0
// - int ForwardCursor::tag_type::wc() const;
template <typename ForwardCursor, typename OutputIterator>
inline
int value_hash(ForwardCursor c, int id, OutputIterator x)
{
  int how_far = 0;
  while(id > 0 && c.first()) {
    if (c.aim_final()) --id;
    while (id - c.aim_tag().wc() > 0) {
      id -= c.aim_tag().wc();
      if (!c.next()) return 0;  // id does not match any word
      if (c.aim_final()) --id;
    }
    *x = c.letter(); ++x; 
    c.forward(); ++how_far;  
  }
  return how_far;
}

} // namespace astl

#endif // ASTL_HASH_H

