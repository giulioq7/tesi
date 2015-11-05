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


#ifndef ASTL_AHO_CORASICK_H
#define ASTL_AHO_CORASICK_H

#include <astl.h>

using namespace std;

namespace astl {

// Requirements: A is a tree
template <typename DFA>
void aho_corasick(DFA &A, const typename DFA::char_type &c)
{
  bfirst_cursor<queue_cursor<forward_cursor<DFA> >, set_marker<unsigned int> >
    i = bfirst_markc(A), j;
  A.set_trans(A.initial(), c, A.initial());
  while (i != j) {
    do {
      if (i.letter() != c) {
	forward_cursor<DFA> f(A, i.src());
	while (f.forward(c) && f.src() != A.initial() && !f.find(i.letter()));
	if (!f.sink() && f.find(i.letter()) && i.aim() != f.aim()) {
	  A.set_trans(i.aim(), c, f.aim());
	  A.final(i.aim()) = i.aim_final() || f.aim_final();
	}
	else 
	  A.set_trans(i.aim(), c, A.initial());
      }
    } while (i.next());
  } 
}

} // namespace astl

#endif // ASTL_AHO_CORASICK_H






