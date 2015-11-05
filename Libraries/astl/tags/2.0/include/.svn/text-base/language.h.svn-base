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

#ifndef ASTL_LANGUAGE_H
#define ASTL_LANGUAGE_H

#include <astl.h>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace astl {

template <class DFirstCursor>
void language(ostream &out,                        // where to write
	      DFirstCursor first,                  // start position
	      DFirstCursor last = DFirstCursor())  // stop condition
{
#ifndef _MSC_VER
  vector<typename DFirstCursor::char_type> word;
#else
  vector<DFirstCursor::char_type> word;
#endif
  for(word.reserve(16); first != last; ) {
    word.push_back(first.letter());
    if (first.aim_final()) {
#ifndef _MSC_VER
      for(typename vector<typename DFirstCursor::char_type>::const_iterator i =
#else
	    for(typename vector<DFirstCursor::char_type>::const_iterator i =
#endif
		  word.begin(); i != word.end(); ++i)
	    out << *i;
	  out << endl;
	  }
      while (!first.forward())  word.pop_back();
    }
  }

template <typename DFirstCursor>
class language_iterator
  : public iterator<input_iterator_tag,
                    basic_string<typename DFirstCursor::char_type> >
{
public:
  typedef basic_string<typename DFirstCursor::char_type> value_type;

public:
  typedef language_iterator self;

  language_iterator()  // end of range
    : first(), last()
  { }

  language_iterator(const DFirstCursor &x)  // beginning of range
    : first(x), last()
  {
    current.reserve(128);
    if (first != last) {
      current += first.letter();
      if (!first.aim_final()) ++*this;
    }
  }

  const DFirstCursor& cursor() const { return first; }

  const value_type& operator*() const { return current; }

  const value_type* operator->() const { return &current; }


  bool operator==(const self &x) const { return first == x.first; }

  bool operator!= (const self &x) const { return first != x.first; }

  self& operator++ () {
    while (1)
      if (!first.forward())                      // ascending
        current.erase(current.size() - 1, 1);    // pop
      else
        if (first == last) return *this;
        else {
          current += first.letter();  // descending, push & test
          if (first.aim_final()) return *this;
        }
    return *this;
  }

protected:
  value_type current;
  DFirstCursor first, last;
};

template <typename DFirstCursor>
inline
language_iterator<DFirstCursor> languagei(const DFirstCursor &x) {
  return language_iterator<DFirstCursor>(x);
}

/*
TODO:
template <typename DFA, typename InputIterator>
inline
typename DFA::state_type
add_word(DFA &dfa, InputIterator first, InputIterator last)
{
  ccursor<DFA> c(dfa);
  forward(c, first, last);
  c.src_final(true);
  return c.src();
}

template <typename DFA, typename InputIterator>
inline
typename DFA::state_type
add_word(DFA &dfa, InputIterator first, InputIterator last,
	 const typename DFA::tag_type &t)
{
  typename DFA::state_type q = add_word(dfa, first, last);
  dfa.tag(q) = t;
  return q;
}
*/

template <typename DFA, typename InputIterator>
inline
typename DFA::state_type
add_word(DFA &dfa, InputIterator first, InputIterator last,
	 const typename DFA::tag_type &t = typename DFA::tag_type())
{
  typename DFA::state_type q, i = dfa.initial();
  typename DFA::char_type a;
  if (i == DFA::null_state) {
    i = dfa.new_state();
    dfa.initial(i);
  }
  // for each letter in w:
  for (; !(first == last); ++first) {  
    a = *first;
    q = dfa.delta1(i, a);
    if (q == DFA::null_state) {  // transition does not exist ?
      q = dfa.new_state();
      dfa.set_trans(i, a, q);
    }
    i = q;
  }
  dfa.final(i) = true;
  dfa.tag(i) = t;
  return (i);
}

template <typename DFA, typename InputIterator>
inline
void add_words(DFA &dfa, InputIterator start, InputIterator finish)
{
  // for each word in the sequence:
  for(; start != finish; ++start)
    add_word(dfa, (*start).begin(), (*start).end());
}

template <class ForwardI>
class string_cursor : public forward_cursor_concept
{
protected:
  ForwardI start, finish;
  bool sink_state;

public:
  typedef empty          tag_type;
  typedef ForwardI state_type;
  typedef string_cursor   self;
  typedef typename iterator_traits<ForwardI>::value_type char_type;
  typedef CHAR_TRAITS<char_type> char_traits;

  string_cursor(ForwardI start, ForwardI finish)
    : start(start), finish(finish), sink_state(false)
    { }

  bool sink() const { return sink_state; }

  void forward() { ++start; }

  bool forward(const char_type &letter) {
    sink_state = !(*start == letter);
    ++start;
    return !sink_state;
  }

  bool first() { return !(start == finish); }

  bool next() { return false; }

  char_type letter() const { return *start; }

  state_type src() const { return start; }

  state_type aim() const {
    ForwardI tmp = start;
    return ++tmp;
  }

  bool aim_final() const {
    ForwardI tmp = start;
    return ++tmp == finish;
  }
  
  bool src_final() const { return start == finish; }

  bool operator==(const self &x) const { return start == x.start; }

  bool find(const char_type &letter) { return exists(letter); }

  // Warning: x must be part of the [start, finish) sequence:
  self& operator=(const ForwardI &x) { 
    start = x;
    return *this;
  }

  bool exists(const char_type &letter) const { return *start == letter; }

  tag_type src_tag() const { return tag_type(); }
  tag_type aim_tag() const { return tag_type(); }
};


template <>
class string_cursor<const char*> : public forward_cursor_concept
{
protected:
  const char *p;

public:
  typedef plain         char_traits;
  typedef empty         tag_type;
  typedef const char*   state_type;
  typedef string_cursor self;
  typedef char          char_type;
  
  typedef tag_type       Tag;
  typedef state_type      State;
  typedef char_type Alphabet;

  string_cursor(const char *p_ = NULL)
    : p(p_)
    { }

  bool sink() const { return p == NULL; }
  
  bool exists(char letter) const { return *p == letter; }

  void forward() { ++p; }

  bool forward(char letter) {
    p = (*p == letter) ? p + 1 : NULL;
    return !sink();
  }

  bool first() { return *p != 0; }

  bool next() { return false; }

  char letter() const { return *p; }

  State src() const { return p; }

  State aim() const { return p + 1; }

  bool src_final() const { return *p == 0; }

  bool aim_final() const { return p[1] == 0; }

  bool operator==(const string_cursor<const char*> &x) const {
    return p == x.p;
  }

  bool find(char letter) { return exists(letter); }
  
  self& operator= (const char *c) {
    p = c;
    return *this;
  }

  tag_type src_tag() const { return tag_type(); }
  tag_type aim_tag() const { return tag_type(); }
};

template <typename InputIterator>
string_cursor<InputIterator> stringc(InputIterator first, InputIterator last) {
  return string_cursor<InputIterator>(first, last);
}

inline
string_cursor<const char*> stringc(const char *s) {
  return string_cursor<const char*>(s);
}

inline
string_cursor<const char*> stringc(char *s) {
  return string_cursor<const char*>(s);
}

template <typename FA>
class transition_iterator : public iterator<forward_iterator_tag,
  triple<typename FA::state_type, typename FA::char_type, 
  typename FA::state_type> >, public bfirst_cursor_concept
{
protected:
  const FA *A;
  typename FA::const_iterator q;
  typename FA::edges_type::const_iterator t;

public:
  typedef transition_iterator     self;
  typedef typename FA::state_type state_type;
  typedef typename FA::char_type  char_type;
  typedef typename FA::tag_type   tag_type;
  typedef bfirst_cursor_concept   concept;

  transition_iterator()
  { }

  transition_iterator(const FA &a)
    : A(&a), q(a.begin()) {
    if (q != a.end()) {
      t = a.delta2(*q).begin();
      if (t == a.delta2(*q).end())
	while (++q != a.end()) {
	  t = A->delta2(*q).begin();
	  if (t != A->delta2(*q).end()) break;
	}
    }
  }
  
  transition_iterator(const FA &a, typename FA::const_iterator i)
    : A(&a), q(i) {
    if (q != a.end()) {
      t = a.delta2(*q).begin();
      if (t == a.delta2(*q).end())
	while (++q != a.end()) {
	  t = A->delta2(*q).begin();
	  if (t != A->delta2(*q).end()) break;
	}
    }
  }

  transition_iterator(typename FA::const_iterator i) // end of range
    : q(i)
  { }

  state_type sink_state() const {
    return FA::null_state;
  }

  triple<state_type, char_type, state_type> operator*() const {
    return make_triple(*q, t->first, t->second);
  }

//  const triple<state_type, char_type, state_type>* operator->() const {
//    return &triple<state_type, char_type, state_type>(*q, t->first, t->second);
//  }

  self& operator++() {
    if (++t == A->delta2(*q).end())
      while (++q != A->end()) {
	t = A->delta2(*q).begin();
	if (t != A->delta2(*q).end()) break;
      }
    return *this;
  }

  self operator++(int) {
    transition_iterator tmp = *this;
    ++*this;
    return tmp;
  }
  
  state_type src() const { return *q; }

  state_type aim() const { return t->second; }

  const tag_type& src_tag() const { return A->tag(*q); }

  const tag_type& aim_tag() const { return A->tag(t->second); }

  bool src_final() const { return A->final(*q); }

  bool aim_final() const { return A->final(t->second); }

  char_type letter() const { return t->first; }

  bool next() {
    typename FA::const_iterator tmp = q;
    ++*this;
    return q == tmp;
  }

  bool operator==(const self &x) const { return q == x.q; }
};

template <typename FA>
inline
transition_iterator<FA> transitioni(const FA &a) {
  return transition_iterator<FA>(a);
}

template <typename FA>
inline
transition_iterator<FA> 
transitioni(const FA &a, typename FA::const_iterator i) {
  return transition_iterator<FA>(a, i);
}

// create a path along its way if it does not exist already:
template <typename DFA>
class build_cursor : public cursor_concept
{
public:
  typedef typename DFA::state_type  state_type;
  typedef typename DFA::tag_type    tag_type;
  typedef typename DFA::char_type   char_type;
  typedef typename DFA::char_traits char_traits;
  typedef build_cursor              self;

  build_cursor()
  { }

  build_cursor(DFA &dfa)
    : A(&dfa) { 
    q = A->initial();
    if (q == DFA::null_state) {
      q = A->new_state();
      A->initial(q);
    }
  }

  build_cursor(DFA &dfa, state_type i)
    : A(&dfa), q(i) 
  { }

  self& operator=(state_type p) {
    if (p == DFA::null_state)
      p = A->new_state();
    q = p;
    return *this;
  }

  state_type      src() const        { return q; }
  bool            src_final() const  { return A->final(q); }
  void            src_final(bool b)  { A->final(q) = b; }
  bool            sink() const       { return src() == sink_state(); }
  state_type      sink_state() const { return DFA::null_state; }
  const tag_type& src_tag() const    { return A->tag(q); }
  tag_type&       src_tag()          { return A->tag(q); }

  bool exists(const char_type &c) const { 
    return A->delta1(q, c) != DFA::null_state; 
  }

  bool forward(const char_type &c) {
    state_type tmp = q;
    q = A->delta1(q, c);
    if (q == DFA::null_state) {
      q = A->new_state();
      A->set_trans(tmp, c, q);
    }
    return true;
  }    

  bool forward(const char_type &c, state_type a) {
    state_type tmp = q;
    q = A->delta1(q, c);
    if (q != a) {
      if (q == DFA::null_state)
	A->set_trans(tmp, c, a);
      else
	A->change_trans(tmp, c, a); // return false ?
      q = a;
    }
    return true;
  }    

  bool operator==(const self &x) const { return q == x.q; }

protected:
  DFA *A;
  state_type q;
};

template <typename DFA>
inline
build_cursor<DFA> buildc(DFA &a) { return build_cursor<DFA>(a); }

} // namespace astl
		
#endif // ASTL_LANGUAGE_ALGORITHMS
