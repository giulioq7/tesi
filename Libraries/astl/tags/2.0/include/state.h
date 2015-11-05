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

#ifndef ASTL_STATE_H
#define ASTL_STATE_H

// A generic state structure storing a pair<tag, transition container>
// Used by DFA_base to allocate/deallocate states
//
// Type requirements:
// - Tag is default contructible, copy constructible, assignable
// - TransitionContainer is default constructible, copy constructible, assignable, 
//   equality comparable and defines two methods size()
//
// This template is specialized for empty_tag in order to save memory space when
// no tags are required

#include <astl.h>

namespace astl {

template <typename Tag, typename TransitionContainer>
class state_data
{
protected:
  Tag                 t;
  TransitionContainer e;

public:
  Tag& tag() {
    return t;
  }
  const Tag& tag() const {
    return t;
  }
  TransitionContainer& edges() {
    return e;
  }
  const TransitionContainer& edges() const {
    return e;
  }
};

// A partial specialization of the state structure to save
// space when no tags are required:

#ifndef _MSC_VER
template <typename TransitionContainer>
class state_data<empty_tag, TransitionContainer>
{
protected:
  static empty_tag    t;  // consistent with the previous interface
  TransitionContainer e;

public:
  empty_tag& tag() {
    return t;
  }
  const empty_tag& tag() const {
    return t;
  }
  TransitionContainer& edges() {
    return e;
  }
  const TransitionContainer& edges() const {
    return e;
  }
};

template <typename TransitionContainer>
empty_tag state_data<empty_tag, TransitionContainer>::t;
#endif // _MSC_VER

} // namespace astl

#endif // ASTL_STATE_H
