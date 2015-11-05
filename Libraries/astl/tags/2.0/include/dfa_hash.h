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

#ifndef ASTL_DFA_HASH_H
#define ASTL_DFA_HASH_H

// Descrition:	
//   ASTL 2.0 Determinisic Finite Automaton Class Template DFA_hash
//   Representation by a unique hash table
//   This class implements a restricted subset of the DFA requirements
//   (no tags, no edges traversal, no state/transition deletions)   

#ifdef __GNUG__
#if __GNUG__ < 4
#include <ext/hash_map>
#define HASH_TABLE_TYPE __gnu_cxx::hash_map
#else
#include <tr1/unordered_map>
#define HASH_TABLE_TYPE tr1::unordered_map
#endif
#include <utility>  // pair<>
#include <functional>


namespace astl {

template <class CharTraits = plain>
class DFA_hash : public DFA_concept
{
public:
  typedef CharTraits char_traits;
  typedef typename char_traits::char_type char_type;
  typedef empty_tag tag_type;
  typedef size_t state_type;

protected:
  typedef pair<state_type, char_type> key;
  typedef state_type                 data;
  struct hash_function 
    : public unary_function<pair<state_type, char_type>, size_t>
  {
    size_t operator() (const pair<state_type, char_type> &x) const {
      return x.first * 1023 + char_traits::to_int_type(x.second);
    }
  };

  typedef HASH_TABLE_TYPE<key, data, 
			  hash_function, equal_to<key> > hasher_type;
  hasher_type hasher;

  unsigned long state_count_;
  state_type    current;
  state_type    I;
  vector<char>  F;
  tag_type      bogus;

public:
  static const state_type null_state = 0;

  DFA_hash(unsigned long = 0)
    : state_count_(0), current(0), 
      I(0), F(1, '\0')
  { }

  state_type new_state() {
    ++state_count_;
    F.push_back('\0');
    return ++current;
  }

  template <class OutputIterator>
  OutputIterator new_state(unsigned long how_many, OutputIterator x) {
    for(; how_many > 0; --how_many)
      *x++ = new_state();
    return x;
  }

  bool final(state_type s) const {
    return F[s] != '\0';
  }

  char& final(state_type s) {
    return F[s];
  }

  state_type initial() const { 
    return (I); 
  }

  void initial(state_type s) { 
    I = s; 
  }

  void set_trans(state_type s, const char_type &a, state_type aim) {
    hasher[make_pair(s, a)] = aim;
  }

  void del_trans(state_type s, const char_type &a) {
    hasher.erase(make_pair(s, a));
  }

  void change_trans(state_type s, const char_type &a, state_type new_aim) {
    hasher[make_pair(s, a)] = new_aim;
  }

  state_type delta1(state_type s, const char_type &a) const {
    typename hasher_type::const_iterator i = hasher.find(make_pair(s, a));
    return i == hasher.end() ? null_state : i->second;
  }

  unsigned long state_count() const { 
    return (state_count_); 
  } 

  unsigned long trans_count() const { 
    return hasher.size();
  }

  tag_type& tag(state_type) {
    return bogus;
  }
	
  const tag_type& tag(state_type) const {
    return bogus;
  }
};

template <class T>
const typename DFA_hash<T>::state_type DFA_hash<T>::null_state;


} // namespace astl

#endif // _GNUG_

#endif // ASTL_CLASS_DFA_HASH	
	
	
