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

#ifndef ASTL_BUILD_H
#define ASTL_BUILD_H

#include <astl.h>
#include <iostream>
#include <map>

using namespace std;

namespace astl {

#if 0
void skip_comments(istream &in)
{
  char comments;
  if (!in.eof()) {
    in >> comments;
    if (comments == '#') {
      string bogus;
      getline(in, bogus);
      skip_comments(in);
    }
    else in.unget();
  }
}
#endif
 
template <typename State> 
int next_state(istream &in, State &i)
{
  //  skip_comments(in);
  if (in.eof()) return -1;
  in >> i;
  return 0;
}

template <typename Char>
int next_letter(istream &in, Char &c)
{
  //  skip_comments(in);
  if (in.eof()) return -1;
  in >> c;
  return 0;
}

template <typename State, typename Char>
int next_transition(istream &in, State &q, Char &a, State &p)
{
  if (next_state(in, q) == -1) return -1;
  if (next_letter(in, a) == -1) return -1;
  if (next_state(in, p) == -1) return -1;
  return 0;
} 

template <typename FA>
typename FA::state_type build(FA &A, istream &in)
{
  map<int, safe<typename FA::state_type, FA::null_state> > m;
  typename FA::state_type i = FA::null_state;
  int q, p;
  typename FA::char_type c;

  if (next_transition(in, q, c, p) != -1) {
    m[q] = A.new_state();
    A.final(m[q]) = q < 0;
    safe<typename FA::state_type, FA::null_state> &pp = m[p];
    if (pp == FA::null_state) {
      pp = A.new_state();
      A.final(pp) = p < 0;
    }
    A.set_trans(m[q], c, pp);
    i = m[q];
  }

  while (next_transition(in, q, c, p) != -1) {
    safe<typename FA::state_type, FA::null_state> &qq = m[q];
    safe<typename FA::state_type, FA::null_state> &pp = m[p];
    if (qq == FA::null_state) {
      qq = A.new_state();
      A.final(qq) = q < 0;
    }
    if (pp == FA::null_state) {
      pp = A.new_state();
      A.final(pp) = p < 0;
    }
    A.set_trans(qq, c, pp);
  }

  return i;
}
    
} // namespace astl

#endif
