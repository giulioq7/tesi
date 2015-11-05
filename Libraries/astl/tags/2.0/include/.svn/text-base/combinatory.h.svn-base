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

#ifndef ASTL_COMBINATORY_H
#define ASTL_COMBINATORY_H

#include <astl.h>
#include <vector>
#include <string>

namespace astl {

// A combination_cursor simulates a DFA whose language is the combinations
// with p intergers out of n (C(n,p)).
// Example: combination_cursor(3, 2) recognizes:
//               1 2
//               2 1
//               1 3
//               3 1
//               2 3
//               3 2
// Constructor:
// 1. n, p

class combination_cursor : public forward_cursor_concept
{
protected:
  pair<std::vector<bool>, int> q;
  int current; // current letter
  // q.second is p - q.first's count of 1

public:
  typedef combination_cursor           self;
  typedef empty                        tag_type;
  typedef pair<std::vector<bool>, int> state_type;
  typedef int                          char_type;

#if (__GNUG__ && __GNUG__ < 3)
  typedef string_char_traits<int>      char_traits;
#else
  typedef std::char_traits<int>        char_traits;
#endif

  combination_cursor()
  { }

  combination_cursor(int n, int p) 
    : q(vector<bool>(n, false), p), current(-1)
  { }

  state_type src() const { return q; }

  state_type aim() const {
    state_type tmp(q.first, q.second - 1);
    tmp.first[current] = true;
    return tmp;
  }

  bool sink() const { return q.first.empty(); }

  state_type sink_state() const { 
    return state_type(vector<bool>(), -1); 
  }

  bool exists(int i) const { return q.second > 0 && !q.first[i - 1]; }

  void forward() {
    --q.second;
    q.first[current] = true;
  }

  bool forward(int letter) {
    if (q.second-- > 0 && q.first[letter - 1] == false) {
      q.first[letter - 1] = true;
      return true;
    }
    q.first.clear(); // move to sink state
    q.second = -1;
    return false;
  }

  bool first() {
    if (q.second > 0)
      for(vector<bool>::const_iterator i = q.first.begin(); 
	  i != q.first.end(); ++i)
	if (*i == false) {
	  current = i - q.first.begin();
	  return true;
	}
    current = -1;
    return false;
  }

  bool next() {
    for(vector<bool>::const_iterator i = q.first.begin() + current + 1; 
	i != q.first.end(); ++i)
      if (*i == false) {
	current = i - q.first.begin();
	return true;
      }
    current = -1;
    return false;
  }

  int letter() const { return current + 1; }

  tag_type src_tag() const { return tag_type(); }
  tag_type aim_tag() const { return tag_type(); }

  bool src_final() const { return q.second == 0; }

  bool aim_final() const { return q.second == 1; }

  bool operator==(const self &x) const {
    return q == x.q && current == x.current;
  }

  bool find(int letter) {
    if (q.second > 0 && q.first[letter - 1] == false) {
      current = letter - 1;
      return true;
    }
    current = -1;
    return false;
  }

  self& operator=(const state_type &p) {
    q = p;
    return *this;
  }
};

// Helper functions:
inline
combination_cursor permutationc(int n) {
  return combination_cursor(n, n);
}

inline
combination_cursor combinationc(int n, int p) {
  return combination_cursor(n, p);
}

} // namespace astl

#endif // ASTL_COMBINATORY_H
	
    
	 
    
	 
