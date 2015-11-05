/*
 * ASTL - the Automaton Standard Template Library.
 * C++ generic components for Finite State Automata handling.
 * Copyright (C) 2000-2006 Vincent Le Maout (vincent.lemaout@chello.fr).
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

/** @file */

#ifndef ASTL_CCOPY_H
#define ASTL_CCOPY_H


#include <iostream>
#include <map>
#include <stack>
#include <string>

using namespace std;

namespace astl {

/** Copies the source automaton defined by the range <tt> [first,
    last) </tt> into the destination automaton by performing a
    depth-first traversal during which only the paths leading to final
    states are copied : this algorithm trims uneeded states and
    transitions. It does not copy tags and works on acyclic DFAs, see
    #clone() for copying cyclic structures.
    @remarks This algorithm is not just a duplication procedure, it is
    the only way (along side #clone() which is a slight variant of
    @c trim) to apply an algorithm and to get the result of it as
    an automaton container. It is also enough to use almost all of the
    operations implemented in ASTL. 
    @param out The target automaton container
    @param first The start-of-range depth-first cursor
    @param last The end-of-range depth-first cursor. By default a
    depth-first cursor with an empty stack
    @return The first state of the iteration (the last duplicated
    state).
    @note This algorithm does not take care of the initial state, it
    is up to the user to manage any operations related to it through
    the value returned by the function. 
    @see #clone() */

template <typename DFirstC, typename DFA>
typename DFA::state_type 
trim(DFA &out, DFirstC first, DFirstC last = DFirstC())
{
  typedef map<typename DFirstC::state_type,
    typename DFA::state_type> mapper;
  mapper mapping;
  typename DFirstC::state_type i = first.src();

  while (first != last) {     // while not end of range
    while(first.forward());   // get down as much as possible

    // copy aim state if final:
    typename DFA::state_type p;
    if (first.aim_final()) {
      typename mapper::iterator i = 
	mapping.insert(make_pair(first.aim(), DFA::null_state)).first;
      if (i->second == DFA::null_state) {   // inserted ?
	i->second = out.new_state();
	out.tag(i->second) = first.aim_tag(); // don't reorder (dot)
	out.final(i->second) = true;
      }
    }

    do { // while moving backward (pop) copy current
      // transition (q, first.letter(), p) 
      // copy source state if needed:
      typename mapper::const_iterator dest = mapping.find(first.aim());
      if (dest != mapping.end()) p = dest->second;
      else p = DFA::null_state;
      
      if (p != DFA::null_state || first.src_final()) {
	typename mapper::iterator i = mapping.insert(make_pair(first.src(), DFA::null_state)).first;
	if (i->second == DFA::null_state) {
	  i->second = out.new_state();
	  out.tag(i->second) = first.src_tag(); // don't reorder (dot)
	  out.final(i->second) = first.src_final();
	}
	if (p != DFA::null_state) 	// copy transition ?
	  out.set_trans(i->second, first.letter(), p);
	
	// shift (q, first.letter(), p) => (q', first.letter()', q)
	p = i->second;
      }
    } while (! first.forward());
  }
  return mapping[i];
}

/** Copies the source automaton defined by the range <tt> [first,
    last) </tt> into the destination automaton by performing a
    depth-first traversal during which all transitions are
    duplicated. On a cyclic automaton, a depth-first cursor with a
    state marker must be used such as the one returned by the helper
    function #dfirst_markc(). 
    @remarks This algorithm is not just a duplication procedure, it is
    the only way (along side #trim() which is a slight variant of
    @c clone) to apply an algorithm and to get the result of it as
    an automaton container. It is also enough to use almost all of the
    operations implemented in ASTL. 
    @param out The target automaton container
    @param first A start-of-range depth-first cursor
    @param last An end-of-range depth-first cursor. By default a
    depth-first cursor with an empty stack
    @return The first state of the iteration (the first duplicated
    state).  
    @note This algorithm does not take care of the initial state, it
    is up to the user to manage any operations related to it through
    to the value returned by the function. 
    @see #trim() */

template <class DFirstC, class DFA>
typename DFA::state_type 
clone(DFA &out, DFirstC first, DFirstC last = DFirstC())
{
  if (first == last) return DFA::null_state;

#ifdef _MSC_VER
  // VC++6.0 has a non standard behavior and refuses 'typename' keyword:
  typedef DFirstC::state_type cursor_state;
  typedef DFA::state_type          dfa_state;
#else
  typedef typename DFirstC::state_type cursor_state;
  typedef typename DFA::state_type          dfa_state;
#endif

  map<cursor_state, dfa_state> mapping;
  cursor_state i = first.src();
  dfa_state q;

  while (first != last) {     // while not end of range

    // copy source state if needed:
    typename map<cursor_state, dfa_state>::iterator i 
      = mapping.insert(make_pair(first.src(), DFA::null_state)).first;
    if (i->second == DFA::null_state) {  // inserted ?
      i->second = out.new_state();
      out.tag(i->second) = first.src_tag(); // don't reorder (dot)
      out.final(i->second) = first.src_final();
    }
    q = i->second;

    do {                      // while moving forward
      // copy current transition (q, first.letter(), p)
      // copy aim state if needed:
      typename map<cursor_state, dfa_state>::iterator i 
	= mapping.insert(make_pair(first.aim(), DFA::null_state)).first;

      if (i->second == DFA::null_state) {
	i->second = out.new_state();
	out.tag(i->second) = first.aim_tag(); // don't reorder
	out.final(i->second) = first.aim_final();
      }

      // copy transition:
      out.set_trans(q, first.letter(), i->second);
      // shift (q, first.letter(), p) => (p, first.letter()', p')
      q = i->second;
    } while (first.forward());

    while (!first.forward());   // pop
  }
  return mapping[i];
}

} // namespace astl

#endif      










