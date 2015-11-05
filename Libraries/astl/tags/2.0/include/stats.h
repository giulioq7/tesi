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

#ifndef ASTL_STATS_H
#define ASTL_STATS_H

#include <astl.h>

namespace astl {

struct DFA_stats
{
  typedef unsigned long state_type;
  typedef empty tag_type;
  unsigned long _state_count, _trans_count, _final_states;
  static const state_type null_state = 0;
  tag_type dummy;

  DFA_stats()
    : _state_count(0), _trans_count(0), _final_states(0)
  { }

  state_type new_state() { return ++_state_count; }

  template <typename CharType>
  void set_trans(state_type from, const CharType&, state_type to) {
    ++_trans_count;
  }

  struct bool_reference
  {
    state_type q;
    DFA_stats &dfa;

    bool_reference(state_type p, DFA_stats &a)
      : q(p), dfa(a)
    { }

    bool_reference& operator= (bool b) {
      if (b == true)
	++dfa._final_states;
      return *this;
    }
  };

  tag_type& tag(state_type q) { return dummy; }
  const tag_type& tag(state_type q) const { return dummy; }

  bool_reference final(state_type q) { return bool_reference(q, *this); }

  unsigned long state_count() const { return _state_count; }

  unsigned long trans_count() const { return _trans_count; }

  unsigned long final_count() const { return _final_states; }
};

template <typename DFirstCursor>
int word_count(DFirstCursor first, DFirstCursor last = DFirstCursor())
{
  int count = 0;
  if (first != last && first.src_final()) ++count;

  while (first != last) {
    do if (first.aim_final()) ++count;
    while (first.forward());
    while (!first.forward());
  }
  return count;
}

} // namespace astl				

#endif // ASTL_STATS_H







