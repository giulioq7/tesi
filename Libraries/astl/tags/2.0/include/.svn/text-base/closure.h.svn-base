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

#ifndef ASTL_CLOSURE_H
#define ASTL_CLOSURE_H

// A closure cursor is a forward cursor adapter
// hiding all outgoing transitions not labelled with the user-specified
// letter (TODO: useless class, that can be done with the
// filter_cursor, moreover the asynchrone determinizing cursor
// implemented with this class is, I'm afraid, far from optimal)

#include <astl.h>

namespace astl {

template <typename ForwardCursor>
class closure_cursor : public ForwardCursor
{
public:
  typedef ForwardCursor             super;
  typedef closure_cursor            self;
  typedef typename super::char_type char_type;
  typedef typename super::state_type state_type;

protected:
  char_type x;

public:
  closure_cursor()
    : super()
  { }

  closure_cursor(const ForwardCursor &c, const char_type &a)
    : super(c), x(a)
  { }

  self& operator=(const self &c) {
    super::operator=(c);
    x = c.x;
    return *this;
  }

  self& operator=(state_type q) {
    super::operator=(q);
    return *this;
  }

  bool first() {
    if (super::first()) {
      if (super::letter() != x)
	return next();
      return true;
    }
    return false;
  }
    
  bool next() {
    while (super::next())
      if (super::letter() == x) return true;
    return false;
  }
  
  bool forward(const char_type &a) {
    if (a == x) return super::forward(a);
    *this = super::sink_state();
    return false;
  }

  void forward() {
    super::forward();
  }
};

template <class ForwardCursor>
inline
closure_cursor<ForwardCursor> 
closurec(const ForwardCursor &c, const typename ForwardCursor::char_type &a)
{
  return closure_cursor<ForwardCursor>(c, a);
}
  
template <typename DFirstCursor, typename OutputCursor>
OutputCursor 
crop(OutputCursor out, DFirstCursor first, DFirstCursor last = DFirstCursor())
{
  while (first != last) {
    *out++ = first.src();
    do 
      *out++ = first.aim();
    while (first.forward());
    while (!first.forward());
  }
  return out;
}

} // namespace astl

#endif // ASTL_CLOSURE_CURSOR
