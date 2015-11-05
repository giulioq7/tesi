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

// Descrition:	
//  ASTL 2.0 Non Determinisic Finite Automaton Class Template NFA_mmap
//  Representation by STL multimap

#ifndef ASTL_NFA_MMAP_H
#define ASTL_NFA_MMAP_H

#include <set>            // set<>
#include <map>            // multimap<>
#include <algorithm>      // transform()
#include <utility>        // pair<>
#include <astl.h>

#if (__GNUG__ && __GNUG__ > 2)
using namespace __gnu_cxx;
#endif

using namespace std;

namespace astl {

template <class CharTraits>
struct nfa_mmap_key_compare 
  : public binary_function<typename CharTraits::char_type, 
  typename CharTraits::char_type, bool>
{
  bool operator()(const typename CharTraits::char_type &x,
		  const typename CharTraits::char_type &y) const {
    return CharTraits::lt(x, y);
  }
};

template <typename State, typename CharTraits, typename Tag>
struct nfa_mmap_state_data
{
  Tag t;
  multimap<typename CharTraits::char_type, State, 
	   nfa_mmap_key_compare<CharTraits> > edges;
};

// #ifndef _MSC_VER
// Partial specialization for empty tag:
template <typename State, typename CharTraits>
struct nfa_mmap_state_data<State, CharTraits, empty_tag>
{
  static empty_tag t; // consistent interface with previous definition
  multimap<typename CharTraits::char_type, State, 
	   nfa_mmap_key_compare<CharTraits> > edges;
};
	
template <class State, class Alphabet>
empty_tag nfa_mmap_state_data<State, Alphabet, empty_tag>::t;
// #endif

template <class _Sigma = plain,
          class _Tag   = empty_tag> 
class NFA_mmap : public NFA_concept
{
public:
  typedef _Sigma                     char_traits;
  typedef typename _Sigma::char_type char_type;
  typedef _Tag                       tag_type;
  typedef unsigned int               state_type;

  // Backward compatibility:
  typedef char_type   Alphabet; 
  typedef char_traits Sigma; 
  typedef tag_type    Tag;
  typedef state_type  State;

protected:
  typedef multimap<char_type, state_type, 
		   nfa_mmap_key_compare<char_traits> > transition_container; 
  typedef vector<char> set_F;
  typedef nfa_mmap_state_data<state_type, char_traits, tag_type> state_data;

  // Copy construction for duplicate:
  state_type new_state(const state_data &x) {
    Q.push_back(new state_data(x));
    ++state_count_;
    trans_count_ += x.edges.size();
    return Q.size() - 1;
  }

  vector<state_data*> Q;
  set<state_type>     I;     // Initial states
  set_F               F;     // Final states
  unsigned long       state_count_, trans_count_;

public:
  class edges_type
  {
    friend class NFA_mmap<_Sigma, _Tag>;
  protected:
    const transition_container *container;
    
  public:
    typedef typename transition_container::key_type        key_type;
    typedef typename transition_container::mapped_type       mapped_type;
    typedef typename transition_container::mapped_type       data_type;
    typedef typename transition_container::value_type      value_type;
    typedef typename transition_container::key_compare     key_compare;
    typedef typename transition_container::const_reference const_reference;
    typedef typename transition_container::size_type       size_type;
    typedef typename transition_container::difference_type difference_type;
    typedef typename transition_container::value_compare   value_compare;

    typedef typename transition_container::const_iterator         const_iterator;
    typedef typename transition_container::const_reverse_iterator const_reverse_iterator;

    // allocation/deallocation:
    
  protected:
    edges_type(const transition_container *c)
      : container(c) { }

  public:
    edges_type(const edges_type& x) : container(x.container) { }
    ~edges_type() { }
    
    // accessors:
    key_compare key_comp() const { return container->key_comp(); } 
    value_compare value_comp() const { return container->value_comp(); }
    const_iterator begin() const { return container->begin(); }
    const_iterator end() const { return container->end(); }
    const_reverse_iterator rbegin() const { return container->rbegin(); }
    const_reverse_iterator rend() const { return container->rend(); }
    bool empty() const { return container->empty(); }
    size_type size() const { return container->size(); }
    size_type max_size() const { return container->max_size(); }
    
    // map operations:
    const_iterator find(const key_type& x) const { return container->find(x); }
    size_type count(const key_type& x) const { return container->count(x); }
    const_iterator lower_bound(const key_type& x) const { return container->lower_bound(x); }
    const_iterator upper_bound(const key_type& x) const { return container->upper_bound(x); }
    pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
      return container->equal_range(x);
    }

    // comparison:
    friend bool operator == (const edges_type& x, const edges_type& y) {
      return x.container == y.container || *x.container == *y.container;
    }
  };

  typedef skip_blanks_iterator<state_data>       const_iterator;

  // Backward compatibility:
  typedef edges_type Edges;

  static const state_type null_state = 0;

  set<state_type>& initial() { 
    return I; 
  }

  const set<state_type>& initial() const { 
    return I; 
  }

  set_F::reference final(state_type s) { 
    return F[s]; 
  }

  bool final(state_type s) const { 
    return F[s] != '\0'; 
  }

  state_type new_state() 
  {
    Q.push_back(new state_data);
    ++state_count_;
    F.push_back('\0');
    return Q.size() - 1;
  }

  template <class OutputIterator>
  OutputIterator new_state(unsigned long how_many, OutputIterator x)
  {
    for(; how_many > 0; --how_many)
      *x++ = new_state();
    return (x);
  }

  void del_state(state_type s) 
  { 
    trans_count_ -= Q[s]->edges.size();
    delete Q[s];
    --state_count_;
    Q[s] = NULL;
    I.erase(s);
  }

  void set_trans(state_type s, const char_type &l, state_type aim)
  {
    Q[s]->edges.insert(make_pair(l, aim));
    ++trans_count_;
  }

  void del_trans(state_type s, const char_type &l) {
    trans_count_ -= Q[s]->edges.erase(l);
  }

  void del_trans(state_type s, const char_type &l, state_type aim)
  {
    pair<typename multimap<char_type, state_type>::iterator, 
      typename multimap<char_type, state_type>::iterator>
      p = Q[s]->edges.equal_range(l);
    for(; p.first != p.second && (*p).second != aim; ++p.first);
    Q[s]->edges.erase(p.first);
    --trans_count_;
  }

  void change_trans(state_type s, const char_type &l, 
		    state_type former_aim, state_type new_aim)
  {
    transition_container &e = Q[s]->edges;
    typename transition_container::iterator p = e.find(l);
    for(; (*p).second != former_aim; ++p);
    (*p).second = new_aim;
  }

  void copy_state(state_type from, state_type to)
  {
    trans_count_ += Q[from]->edges.size() - Q[to]->edges.size();
    *Q[to] = *Q[from];
    F[to] = F[from];
  }

  template <class OutputIterator>
  OutputIterator delta1(state_type s, const char_type &l, 
			OutputIterator x) const
  {
    return std::transform(Q[s]->edges.lower_bound(l), 
			  Q[s]->edges.upper_bound(l), 
			  x, select2nd<pair<char_type, state_type> >()); 
  }

  state_type duplicate_state(state_type q) {
    state_type s = new_state(*Q[q]);  // protected method for copy-construction
    F[s] = F[q];
    return s;
  }

  unsigned long state_count() const { 
    return state_count_;
  }

  unsigned long trans_count() const { 
    return trans_count_; 
  }
  
  Tag& tag(state_type s) { 
    return Q[s]->t; 
  }

  const Tag& tag(state_type s) const { 
    return Q[s]->t; 
  }

  edges_type delta2(state_type s) const { 
    return edges_type(&Q[s]->edges); 
  }

  const_iterator begin() const { 
    const_iterator x(&Q, 0);
    return ++x;
  }

  const_iterator end() const { 
    return const_iterator(&Q, Q.size());
  }
  
  NFA_mmap(unsigned long n = 0)
    : Q(1, (state_data*) NULL), I(), F(1, '\0'), state_count_(0), 
      trans_count_(0) { 
    Q.reserve(n + 1);
  }

  ~NFA_mmap()
  {
    const_iterator start, finish = end();
    for(start = begin(); start != finish; ++start)
      del_state(*start);
  }
};

template <typename CharTraits, typename Tag>
const typename NFA_mmap<CharTraits, Tag>::state_type
NFA_mmap<CharTraits, Tag>::null_state;

} // namespace astl

#endif   // ASTL_NFA_MMAP_H








