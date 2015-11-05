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

#ifndef ASTL_DFA_BIN_H
#define ASTL_DFA_BIN_H


#include <dfa_base.h>
#include <vector>
#include <algorithm>    // lower_bound()
#include <utility>      // pair<>
#include <functional>

namespace astl {

/**
   A deterministic automaton container class that stores the
   transition of a state in a standard sorted @c vector of pairs of
   letters and transitions targets. Access is in logarithmic time
   through a binary search but insertion and deletion are in linear
   time. 

   It is the best when it comes to depth- and breadth-first
   traversals and when saving memory space is important but is slow at
   building and maintaining.

   The ordering used for sorting the transition uses the one defined
   by the character traits @c CharTraits::lt().

   @tparams <tr><td>@c CharTraits</td><td>Character traits describing
   the letters that the transitions bear.</td><td>@c plain</td><td>@c
   CharTraits is a model of #Alphabet</td></tr>
   <tr><td>@c Tag</td><td>@tagtype.</td><td>@c empty_tag</td><td>
   </table>
 */

template <class Sigma_ = plain, 
	  class Tag_   = empty_tag>
class DFA_bin 
  : public DFA_base<Sigma_, Tag_, 
		    vector<pair<typename Sigma_::char_type, unsigned int> > >
{
public:
  typedef DFA_base<Sigma_, Tag_, vector<pair<typename Sigma_::char_type, 
					     unsigned int> > > super;
  typedef DFA_bin                    self;
  typedef Sigma_                     char_traits;
  typedef Tag_                       tag_type;
  typedef typename Sigma_::char_type char_type;
  typedef unsigned int               state_type;
  using super::null_state;
  
  // Backward compatibility
  typedef char_traits Sigma;
  typedef char_type   Alphabet;
  typedef tag_type    Tag;
  typedef state_type  State;

protected:
  typedef vector<pair<typename Sigma_::char_type, 
		      unsigned int> > transition_container;
  mutable pair<char_type, unsigned int> p;  // optimization
	
public:
  class edges_type
  {
    friend class DFA_bin;
  protected:
    const transition_container *container;

  public:
    typedef char_type                 key_type;
    typedef state_type                data_type;
    typedef pair<key_type, data_type> value_type;
    typedef value_type*               pointer;
    typedef const value_type*         const_pointer;
    typedef value_type&               reference;
    typedef const value_type&         const_reference;
    typedef unsigned int              size_type;
    typedef int                       difference_type;

    struct key_compare : public binary_function<key_type, key_type, bool>
    {
      bool operator () (const key_type &x, const key_type &y) const {
	return char_traits::lt(x, y);
      }
    }; 
    
    struct value_compare : public binary_function<value_type, value_type, bool>
    {
      bool operator () (const value_type &x, const value_type &y) const {
	return self::char_traits::lt(x.first, y.first);
      }
    }; 

    typedef typename transition_container::const_iterator         
    const_iterator ;
    typedef typename transition_container::const_reverse_iterator 
    const_reverse_iterator ;

    // allocation/deallocation:
    edges_type(const transition_container *c = NULL)
      : container(c) 
    { }
    
    const_iterator begin() const { 
      return container->begin(); 
    }
    const_iterator end() const { 
      return container->end(); 
    }

    const_reverse_iterator rbegin() const { 
      return container->rbegin(); 
    }
    const_reverse_iterator rend() const { 
      return container->rend(); 
    }

    bool empty() const { 
      return container->empty(); 
    }

    size_type size() const { 
      return container->size(); 
    }

    size_type max_size() const { 
      return container->max_size(); 
    }
    
    // map operations:
    key_compare key_comp() const {
      return key_compare();
    }

    value_compare value_comp() const {
      return value_compare();
    }

    const_iterator find(const key_type& x) const { 
      typename transition_container::const_iterator i = 
	std::lower_bound(container->begin(), container->end(), 
			 make_pair(x, 0), value_compare());

      if (i == container->end() || (*i).first != x)
	return container->end();
      else
	return i;
    }

    size_type count(const key_type& x) const { 
      return find(x) == end() ? 0 : 1;
    }

    const_iterator lower_bound(const key_type& x) const {
      return std::lower_bound(container->begin(), container->end(), 
			      make_pair(x, 0), value_compare()); 
    }

    const_iterator upper_bound(const key_type& x) const { 
      return std::upper_bound(container->begin(), container->end(), 
			      make_pair(x, 0), value_compare());
    }

    pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
      return std::equal_range(container->begin(), container->end(),
			      make_pair(x, 0), value_compare());
    }
    
    friend bool operator == (const edges_type& x, const edges_type& y) {
      return x.container == y.container || *x.container == *y.container;
    }
  };

  typedef edges_type Edges;

  void set_trans(state_type s, const char_type &a, state_type aim)
  {
    p.first = a;
    p.second = aim;
    Q[s]->edges().insert(lower_bound(Q[s]->edges().begin(), 
				     Q[s]->edges().end(), p, 
				     typename edges_type::value_compare()), p);
    ++trans_count_;
  }

  void del_trans(state_type s, const char_type &a)
  {
    p.first = a;
    Q[s]->edges().erase(lower_bound(Q[s]->edges().begin(), 
				    Q[s]->edges().end(), p, 
				    typename edges_type::value_compare()));
    --trans_count_;
  }

  void change_trans(state_type s, const char_type &a, state_type new_aim) {
    p.first = a;
    (*lower_bound(Q[s]->edges().begin(), Q[s]->edges().end(), 
		  p, typename edges_type::value_compare())).second = new_aim;
  }

  state_type delta1(state_type s, const char_type &a) const
  {
    p.first = a;
    typename transition_container::const_iterator i = 
      lower_bound(Q[s]->edges().begin(), Q[s]->edges().end(), 
		  p, typename edges_type::value_compare());

    return (i == Q[s]->edges().end() || 
	    !((*i).first == a)) ? null_state : (*i).second;
  }

  edges_type delta2(state_type s) const {
    return edges_type(&Q[s]->edges()); 
  }

  DFA_bin(unsigned long n = 0)
    : super(n)
  {}

protected:
  using super::Q;
  using super::trans_count_;
};

} // namespace astl

#endif

