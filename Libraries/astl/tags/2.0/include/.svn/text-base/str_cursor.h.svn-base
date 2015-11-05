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

#ifndef ASTL_STR_CURSOR_H
#define ASTL_STR_CURSOR_H

#include <astl.h>
#include <string>

using namespace std;

namespace astl {

template <class ForwardIterator>
class string_cursor : public forward_cursor_concept
{
protected:
  ForwardIterator first, last;
  bool sink_state;

public:
  typedef empty_tag       tag_type;
  typedef ForwardIterator state_type;
  typedef string_cursor   self;
  typedef typename iterator_traits<ForwardIterator>::value_type char_type;
  typedef std::char_traits<char_type> char_traits;

  typedef tag_type       Tag;
  typedef state_type      State;
  typedef char_type Alphabet;

  string_cursor(ForwardIterator first, ForwardIterator last)
    : first(first), last(last), sink_state(false)
    { }

  bool sink() const {
    return sink_state;
  }

  void forward() {
    ++first;
  }

  bool forward(const char_type &letter) {
    sink_state = !(*first == letter);
    ++first;
    return !sink_state;
  }

  bool first_transition() {
    return !(first == last);
  }

  bool next_transition() {
    return false;
  }

  char_type letter() const {
    return *first;
  }

  state_type src() const {
    return first;
  }

  state_type aim() const {
    ForwardIterator tmp = first;
    return ++tmp;
  }

  bool aim_final() const {
    ForwardIterator tmp = first;
    return ++tmp == last;
  }
  
  bool src_final() const {
    return first == last;
  }

  bool operator==(const self &x) const {
    return first == x.first;
  }

  bool find(const char_type &letter) {
    return exists(letter);
  }

  // Warning: x must be part of the [first, last) sequence:
  self& operator=(const ForwardIterator &x) { 
    first = x;
    return *this;
  }

  bool exists(const char_type &letter) const {
    return *first == letter;
  }
};

template <>
class string_cursor<const char*> : public forward_cursor_concept
{
protected:
  const char *p;

public:
  typedef empty_tag     tag_type;
  typedef const char*   state_type;
  typedef string_cursor self;
  typedef char          char_type;
  
  typedef tag_type       Tag;
  typedef state_type      State;
  typedef char_type Alphabet;

  string_cursor(const char *p_ = NULL)
    : p(p_)
    { }

  bool sink() const {
    return p == NULL;
  }
  
  bool exists(char letter) const {
    return *p == letter;
  }

  void forward() {
    ++p;
  }

  bool forward(char letter) {
    p = (*p == letter) ? p + 1 : NULL;
    return !sink();
  }

  bool first_transition() {
    return *p != 0;
  }

  bool next_transition() {
    return false;
  }

  char letter() const {
    return *p;
  }

  State src() const {
    return p;
  }

  State aim() const {
    return p + 1;
  }

  bool src_final() const {
    return *p == 0;
  }

  bool aim_final() const {
    return p[1] == 0;
  }

  bool operator==(const string_cursor<const char*> &x) const {
    return p == x.p;
  }

  bool find(char letter) {
    return exists(letter);
  }
  
  self& operator= (const char *c) {
    p = c;
    return *this;
  }
};

template <typename InputIterator>
inline
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
#endif 

} // namespace astl

#endif 








