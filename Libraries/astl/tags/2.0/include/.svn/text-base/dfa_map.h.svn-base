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

#ifndef ASTL_DFA_MAP_H
#define ASTL_DFA_MAP_H


#include <dfa_base.h>
#include <map>            
#include <vector>  
#include <functional>   // binary_function<>       

using namespace std;

namespace astl {

template <typename CharTraits>
struct dfa_map_key_compare 
  : public binary_function<typename CharTraits::char_type, 
			   typename CharTraits::char_type, bool>
{
  bool operator()(const typename CharTraits::char_type &x, 
		  const typename CharTraits::char_type &y) const {
    return CharTraits::lt(x, y);
  }
};

/**
   A deterministic automaton container class that stores the
   transition of a state in a standard @c map associating letters to
   transitions targets. It is a pretty versatile container: access,
   insertion and deletion of a transition are computed in logarithmic
   time and it is rather efficient at depth- and breadth-first
   traversals without being too memory-consuming.

   It is a good space/time tradeoff and should be used whenever no
   assumption can be made on the nature of the data to be processed. 

   The ordering used for instanciating the @c map uses the one defined
   by the character traits @c CharTraits::lt().

   @tparams <tr><td>@c CharTraits</td><td>Character traits describing
   the letters that the transitions bear.</td><td>@c plain</td><td>@c
   CharTraits is a model of #Alphabet</td></tr>
   <tr><td>@c Tag</td><td>@tagtype.</td><td>@c empty_tag</td><td>
   </table>
 */
template <class CharTraits = plain,
          class Tag   = empty_tag>
class DFA_map 
  : public DFA_base<CharTraits, Tag, 
		    map<typename CharTraits::char_type, 
			unsigned int, dfa_map_key_compare<CharTraits> > > 
{
protected:
  typedef map<typename CharTraits::char_type, unsigned int, 
	      dfa_map_key_compare<CharTraits> > transition_container;

public:
  typedef DFA_base<CharTraits, Tag, transition_container> super;
  typedef CharTraits                          char_traits;
  typedef Tag                            tag_type;
  typedef typename CharTraits::char_type      char_type;
  typedef unsigned int                    state_type;
  using super::null_state;
  
  class edges_type
  {
    friend class DFA_map<CharTraits, Tag>;
  private:
    const transition_container *container;
    
  public:
    typedef typename transition_container::key_type        key_type;
    typedef unsigned int                                   data_type;
    typedef typename transition_container::value_type      value_type;
    typedef typename transition_container::key_compare     key_compare;
    typedef typename transition_container::const_reference const_reference;
    typedef typename transition_container::size_type       size_type;
    typedef typename transition_container::difference_type difference_type;
    typedef typename transition_container::value_compare   value_compare;

    typedef typename transition_container::const_iterator 
    const_iterator;
    typedef typename transition_container::const_reverse_iterator 
    const_reverse_iterator;

  protected:
    edges_type(const transition_container *c)
      : container(c) { }

  public:
    edges_type(const edges_type& x) : container(x.container) { }
    ~edges_type() { }
    
    // accessors:
    key_compare key_comp() const { return (container->key_comp()); } 
    value_compare value_comp() const { return (container->value_comp()); }
    const_iterator begin() const { return (container->begin()); }
    const_iterator end() const { return (container->end()); }
    const_reverse_iterator rbegin() const { return (container->rbegin()); }
    const_reverse_iterator rend() const { return (container->rend()); }
    bool empty() const { return (container->empty()); }
    size_type size() const { return (container->size()); }
    size_type max_size() const { return (container->max_size()); }
    
    // map operations:
    const_iterator find(const key_type& x) const { 
      return (container->find(x)); 
    }
    size_type count(const key_type& x) const { return (container->count(x)); }
    const_iterator lower_bound(const key_type& x) const { 
      return (container->lower_bound(x)); 
    }
    const_iterator upper_bound(const key_type& x) const { 
      return (container->upper_bound(x)); 
    }
    pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
      return (container->equal_range(x));
    }

    // comparison:
    friend bool operator == (const edges_type& x, const edges_type& y) {
      return (x.container == y.container || *x.container == *y.container);
    }
  };

  // Backward compatibility
  typedef edges_type Edges;
  
  void del_trans(state_type s, const char_type &l)
  {
    Q[s]->edges().erase(l);
    --trans_count_;
  }

  void change_trans(state_type s, const char_type &l, state_type aim) {
    Q[s]->edges()[l] = aim;
  }

  state_type delta1(state_type s, const char_type &l) const
  {
    typename transition_container::iterator vi = Q[s]->edges().find(l);
    return vi == Q[s]->edges().end() ? null_state : (*vi).second;
  }

  void set_trans(state_type s, const char_type &l, state_type aim)
  {
    Q[s]->edges()[l] = aim;
    ++trans_count_;
  }

  edges_type delta2(state_type s) const { 
    return edges_type(&Q[s]->edges()); 
  }

  DFA_map(unsigned long n = 0) 
    : super(n)
  { }

protected:
  using super::Q;
  using super::trans_count_;
};

} // namespace astl

#endif   // ASTL_DFA_MAP_H








