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
//   ASTL 2.0 Determinisic Finite Automaton Class Template DFA_tr
//   Representation by arrays with transpose method

#ifndef ASTL_DFA_TR_H
#define ASTL_DFA_TR_H


#include <dfa_base.h>
#include <vector>
#include <algorithm>         // find_if(), iter_swap()
#include <functional>        // compose1(), bind1st(), select1st<>

#if (__GNUG__ && __GNUG__ > 2)
using namespace __gnu_cxx;
#endif

namespace astl {

template <class Sigma_ = plain,
          class Tag_   = empty_tag>
class DFA_tr 
  : public DFA_base<Sigma_, Tag_, 
		    vector<pair<typename Sigma_::char_type, unsigned int> > >
{
protected:
  typedef vector<pair<typename Sigma_::char_type, unsigned int> > 
  transition_container;
	
public:
  typedef DFA_base<Sigma_, Tag_, vector<pair<typename Sigma_::char_type, 
					     unsigned int> > > super;
  typedef typename super::char_traits     char_traits;
  typedef Tag_                            tag_type;
  typedef typename char_traits::char_type char_type;
  typedef unsigned int                    state_type;
  using super::null_state;
  
  // Backward compatibility
  typedef char_traits Sigma;
  typedef char_type   Alphabet;
  typedef tag_type    Tag;
  typedef state_type  State;
  
  class edges_type
  {
  protected:
    const transition_container *container;
    typedef typename transition_container::iterator iterator;
    //    friend DFA_tr;

  public:
    typedef char_type              key_type;
    typedef state_type            data_type;
    typedef pair<key_type, state_type> value_type;
    typedef value_type*                 pointer;
    typedef const value_type*           const_pointer;
    typedef value_type&                 reference;
    typedef const value_type&           const_reference;
    typedef unsigned long               size_type;
    typedef long                        difference_type;
    
    struct key_compare : public binary_function<key_type, key_type, bool>
    {
      bool operator () (const key_type &x, const key_type &y) const {
	return char_traits::eq(x, y);
      }
    }; 

    struct value_compare : public binary_function<value_type, value_type, bool>
    {
      bool operator () (const value_type& x, const value_type& y) const {
	return key_compare()(x.first, y.first);
      }
    };
    
    typedef typename transition_container::const_iterator 
    const_iterator;
    typedef typename transition_container::const_reverse_iterator 
    const_reverse_iterator;

    // allocation/deallocation:
    
    edges_type(const transition_container *c = NULL)
      : container(c) { }
    
    edges_type(const edges_type& x) 
      : container(x.container) { }
    
    key_compare key_comp() const { 
      return key_compare();
    }

    value_compare value_comp() const { 
      return value_compare(); 
    }

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
    const_iterator find(const key_type& x) const { 
      return find_if(container->begin(), container->end(), 
		     compose1(bind2nd(key_compare(), x), 
			      select1st<value_type>()));
    }

    size_type count(const key_type& x) const { 
      return find(x) == end() ? 0 : 1; 
    }

    const_iterator lower_bound(const key_type& x) const { 
      return find(x);
    }

    const_iterator upper_bound(const key_type& x) const { 
      const_iterator i = find(x);
      return i == end() ? i : ++i;
    }

    pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
      const_iterator i = find(x);
      return i == end() ? make_pair(i, i) : make_pair(i, ++i);
    }

    friend bool operator==(const edges_type& x, const edges_type& y) {
      return x.container == y.container || *x.container == *y.container;
    }
  };

  void set_trans(state_type s, const char_type &a, state_type aim)
  {
    Q[s]->edges().push_back(make_pair(a, aim));
    ++trans_count_;
  }

  void del_trans(state_type s, const char_type &a)
  {
    Q[s]->edges().erase(find_if(Q[s]->edges().begin(), Q[s]->edges().end(), 
				compose1(bind2nd(typename DFA_tr<Sigma_,Tag_>::edges_type::key_compare(), a), 
					 select1st<pair<char_type, state_type> >())));
    --trans_count_;
  }

  void change_trans(state_type s, const char_type &a, state_type new_aim) {
    (*find_if(Q[s]->edges().begin(), Q[s]->edges().end(), 
	      compose1(bind2nd(typename DFA_tr<Sigma_,Tag_>::edges_type::key_compare(), a), 
		       select1st<pair<char_type, state_type> >()))).second = new_aim;
  }

  state_type delta1(state_type s, const char_type &a) const
  {
    transition_container &edges = Q[s]->edges();
    typename transition_container::iterator i = 
      find_if(edges.begin(), edges.end(), 
	      compose1(bind2nd(typename DFA_tr<Sigma_,Tag_>::edges_type::key_compare(), a), 
		       select1st<pair<char_type, state_type> >()));
    if (i == edges.end()) return null_state;
    else {
      if (i != edges.begin()) {
	iter_swap(i, i - 1);
	return (*(i - 1)).second;
      }
      return (*i).second;
    }
  }

  edges_type delta2(state_type s) const {
    return edges_type(&Q[s]->edges());
  }

  DFA_tr(unsigned long n = 0)
    : super(n)
  { }

protected:
  using super::Q;
  using super::trans_count_;
};

} // namespace astl

#endif // ASTL_DFA_TR_H






