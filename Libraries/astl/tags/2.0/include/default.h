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

#ifndef ASTL_DEFAULT_H
#define ASTL_DEFAULT_H



namespace astl {

//
// A def_trans_cursor (default transition cursor) uses the transition labelled 
// with default_letter when the required transition is undefined.
template <typename Cursor>
class def_trans_cursor : public Cursor
{
public:
  typedef Cursor                     super;
  typedef def_trans_cursor           self;
  typedef typename super::char_type  char_type;
  typedef typename super::state_type state_type;

  def_trans_cursor()
  { }

  def_trans_cursor(const Cursor &x, const char_type& default_letter)
    : super(x), def_letter(default_letter)
  { }

  bool forward(const char_type &a) {
    if (this->exists(a)) {
      super::forward(a);
      return true;
    }
    return super::forward(def_letter);
  }

  self& operator=(const state_type &p) {
    super::operator=(p);
    return *this;
  }

protected:
  typename Cursor::char_type def_letter;
};

// A def_state_cursor (default state cursor) moves to default state
// when required transition is undefined (makes the DFA complete and
// make the default state the new sink state).
// Requirements:
// the default state is an existing state
// Warning: this makes the automaton cyclic
template <class ForwardC>
class def_state_cursor : public ForwardC
{
public:
  typedef typename ForwardC::state_type  state_type;
  typedef typename ForwardC::char_type   char_type;
  typedef typename ForwardC::char_traits char_traits;
  typedef ForwardC                       super;
  typedef def_state_cursor               self;
  
  def_state_cursor()
  { }

  def_state_cursor(const ForwardC &x, const state_type &default_state)
    : super(x), def_state(default_state)
  { }

  void forward() { super::forward(); }

  bool forward(const char_type &letter) {
    if (!super::forward(letter))
      *this = def_state;
    return true;
  }

  self& operator=(const state_type &x) {
    super::operator=(x);
    return *this;
  }

protected:
  state_type def_state;
};

// A substitute_cursor follows a default transition when the required
// transition is undefined and tries to move forward from this state
// again. (cf. Aho & Corasick search algorithm). 
template <typename ForwardC>
class substitute_cursor : public ForwardC
{
public:
  typedef ForwardC                    super;
  typedef substitute_cursor           self;
  typedef typename super::char_type   char_type;
  typedef typename super::state_type  state_type;
  typedef typename super::tag_type    tag_type;
  typedef typename super::char_traits char_traits;
  
  substitute_cursor()
  { }

  substitute_cursor(const ForwardC &x, const char_type &c)
    : super(x), a(c)
  { }

  void forward() { super::forward(); }

  bool forward(const char_type& letter) {
    do {
      if (super::find(letter)) {
	forward();
	return true;
      }
      if (super::find(a) && super::aim() == super::src())
	  return true;
    } while (super::forward(a));
    return false;
  }

  self& operator=(const state_type &x) {
    super::operator=(x);
    return *this;
  }

protected:
  typename super::char_type a;  // default transition label
};

template <typename Cursor>
inline
def_trans_cursor<Cursor> 
def_transc(const Cursor &x, const typename Cursor::char_type& def_letter) {
  return def_trans_cursor<Cursor>(x, def_letter);
}

template <class ForwardC>
inline
def_state_cursor<ForwardC> 
def_statec(const ForwardC &x, const typename ForwardC::state_type& p) {
  return def_state_cursor<ForwardC>(x, p);
}

template <class ForwardC>
inline
substitute_cursor<ForwardC> 
substitutec(const ForwardC &x, const typename ForwardC::char_type &c) {
  return substitute_cursor<ForwardC>(x, c);
} 

} // namespace astl

#endif // ASTL_DEFAULT_H

