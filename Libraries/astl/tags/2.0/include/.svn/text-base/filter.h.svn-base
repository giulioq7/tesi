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

#ifndef ASTL_FILTER_H
#define ASTL_FILTER_H



namespace astl {

template <class ForwardCursor, class ObjectFunction>
class filter_in_cursor : public ForwardCursor
{
public:
  typedef filter_in_cursor            self;
  typedef ForwardCursor               super;
  typedef typename super::state_type  state_type;
  typedef typename super::char_type   char_type;
  typedef typename super::char_traits char_traits;
  typedef forward_cursor_concept      concept;
  typedef typename super::tag_type    tag_type;

  filter_in_cursor(const super &x = super(), 
		   const ObjectFunction &of = ObjectFunction())
    : super(x), f(of)
  { }

  void forward() {
    super::forward();
  }

  bool forward(const char_type &a) {
    return super::forward(f(a));
  }

  bool find(const char_type &a) {
    return super::find(f(a));
  }

  self& operator=(const state_type &x) {
    super::operator=(x);
    return *this;
  }

protected:
  ObjectFunction f;
};


template <class ForwardCursor, class ObjectFunction>
class filter_out_cursor : public ForwardCursor
{
protected:
  ObjectFunction f;

public:
  typedef filter_out_cursor                    self;
  typedef ForwardCursor                        super;
  typedef typename ObjectFunction::result_type char_type;
  typedef typename super::state_type           state_type;

  filter_out_cursor(const super &x, 
		    const ObjectFunction &of = ObjectFunction())
    : super(x), f(of)
  { }

  char_type letter() const {
    return f(super::letter());
  }
};

// A filter cursor hides the transitions (q, a, p) which do not verify
// a given predicate. The predicate takes a forward cursor as
// argument and returns true if the pointed transition is "viewable"
template <typename ForwardCursor, typename Predicate>
class filter_cursor : public ForwardCursor
{
protected:
  Predicate p;

public:
  typedef ForwardCursor              super;
  typedef filter_cursor              self;
  typedef typename super::char_type  char_type;
  typedef typename super::state_type state_type;

  filter_cursor(const ForwardCursor &x, 
		const Predicate &pr = Predicate()) 
    : super(x), p(pr)
  { }

  bool exists(const char_type &c) const {
    if (!super::exists(c)) return false; // useful optimization ?
    self tmp = *this;
    return tmp.find(c);
  }

  bool find(const char_type &c) {
    return super::find(c) && p(*this);
  }

  bool forward(const char_type &c) {
    if (find(c)) forward();
    else *this = super::sink_state();
    return !super::sink();
  }

  bool first() {
    return super::first() && (p(*this) || super::next());
  }

  bool next() {
    while (super::next())
      if (p(*this)) return true;
    return false;
  }

  self& operator=(const state_type &q) {
    super::x = q;
    return *this;
  }

  self& operator=(const self &s) {
    super::x = s.x;
    return *this;
  }

  // warning: s may point to an invalid transition (p(s) == false)
  self& operator=(const super &s) {
    super::x = s;
    return *this;
  }
};

// Helper functions:
template <typename ForwardCursor, typename ObjectFunction>
inline
filter_out_cursor<ForwardCursor, ObjectFunction>
filter_outc(const ForwardCursor &x, const ObjectFunction &f) {
  return filter_out_cursor<ForwardCursor, ObjectFunction>(x, f);
}

template <typename ForwardCursor, typename ObjectFunction>
inline
filter_in_cursor<ForwardCursor, ObjectFunction>
filter_inc(const ForwardCursor &x, const ObjectFunction &f) {
  return filter_in_cursor<ForwardCursor, ObjectFunction>(x, f);
}

template <typename ForwardCursor, typename Predicate>
inline
filter_cursor<ForwardCursor, Predicate>
filterc(const ForwardCursor &x, const Predicate &pr) {
  return filter_cursor<ForwardCursor, Predicate>(x, pr);
}

} // namespace astl

#endif // ASTL_FILTER_H



