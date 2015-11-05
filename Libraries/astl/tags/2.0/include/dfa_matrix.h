
#include <cassert>

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

#ifndef ASTL_DFA_MATRIX_H
#define ASTL_DFA_MATRIX_H


#include <dfa_base.h>
#include <iterator>   // iterator_tag
#include <functional> // less<>
#include <utility>    // pair<>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

namespace astl {

  // g++ 3.2.2 & VC++ bug workaround
  // g++ 3.2.2 sees matrix_line as a variably modified type
  // VC++ sees unsigned int v[Sigma::size] as a zero-length array (??)
#if (_MSC_VER) || ((__GNUG__ == 3) && (__GNUC_MINOR__ == 2) && (__GNUC_PATCHLEVEL__ == 2))
#define MATRIX_PATCH
#endif

  template <typename Sigma, typename State>
  class matrix_line
  {
  public:
    unsigned int size_;//    a count of the outgoing transitions
#ifdef MATRIX_PATCH
    State *v;
#else
    State v[Sigma::size];
#endif

    typedef matrix_line self;

    matrix_line() : size_(0) {
#ifdef MATRIX_PATCH
      v = new State[Sigma::size];
#endif
      memset(v, 0, Sigma::size * sizeof(State));

    }

#ifdef MATRIX_PATCH
    ~matrix_line() {
      delete [] v;
    }
#endif

    matrix_line(const self &x)
      : size_(x.size_) {
#ifdef MATRIX_PATCH
      v = new State[Sigma::size];
#endif
      memcpy(v, x.v, Sigma::size * sizeof(State));
    }

    self& operator=(const self &x) {
      size_ = x.size_;
      memcpy(v, x.v, Sigma::size * sizeof(State));
      return (*this);
    }

    bool operator==(const self &x) const {
      return size_ == x.size_ && equal(v, v + Sigma::size, x.v);
    }

    size_t size() const {
      return size_;
    }

    unsigned int& size() {
      return size_;
    }
  };

  /** A deterministic automaton container class that stores transitions
      targets in a matrix state x letter: each state has a line the size
      of the alphabet. It has a fast constant-time access to
      transitions but tends to consume too much memory space when the
      matrix is sparse. It is also not recommended for depth- or
      breadth-first traversals.

      This container is advisable for efficient pattern matching and
      for almost nothing else.

      The constant-time access to transitions is provided by the
      character traits through the mapping between the alphabet and the
      integers implemented by @c CharTraits::to_char_type() and @c
      CharTraits::to_int_type().

      @tparams <tr><td>@c CharTraits</td><td>Character traits describing
      the letters that the transitions bear.</td><td>@c plain</td><td>@c
      CharTraits is a model of #EnumerableAlphabet</td></tr>
      <tr><td>@c Tag</td><td>@tagtype.</td><td>@c empty_tag</td><td>
      </table>
  */
  template <class CharTraits = plain,
	    class Tag        = empty_tag,
	    class StateType  = unsigned int>
  class DFA_matrix_base : public DFA_base<CharTraits, Tag, matrix_line<CharTraits, StateType>, StateType>
  {
  public:
    typedef DFA_base<CharTraits, Tag, matrix_line<CharTraits, StateType>, StateType> super;
    typedef CharTraits                                                    char_traits;
    typedef Tag                                                           tag_type;
    typedef typename CharTraits::char_type                                char_type;
    typedef unsigned int                                                  state_type;
    typedef DFA_matrix_base<CharTraits, Tag, StateType>                   self;
    using super::null_state;

    class edges_type
    {
    protected:
      typedef matrix_line<char_traits, StateType> Container;
      const Container *container;

    public:
      typedef char_type                         key_type;
      typedef state_type                        data_type;
      typedef pair<const char_type, state_type> value_type;

      struct key_compare : public binary_function<key_type, key_type, bool>
      {
	bool operator()(const key_type &x, const key_type &y) const {
	  return char_traits::to_int_type(x) < char_traits::to_int_type(y);
	}
      };

      struct value_compare : public binary_function<value_type, value_type, bool>
      {
	bool operator()(const value_type &x, const value_type &y) const {
	  return char_traits::to_int_type(x.first) <
	    char_traits::to_int_type(y.first);
	}
      };
      typedef const value_type& const_reference;
      typedef int               difference_type;

      typedef typename super::char_traits::int_type    size_type;

      class const_iterator
	: public iterator<bidirectional_iterator_tag, value_type>
      {
	friend class edges_type;
	typedef const_iterator self;

      protected:
	const state_type *i;
	const Container *c;

	const_iterator(const state_type *_i, const Container *_c)
	  : i(_i), c(_c)
	{ }

      public:
	const_iterator()
	{ }

	bool operator== (const self& x) const {
	  return x.i == i;
	}

	bool operator!= (const self& x) const {
	  return !(*this == x);
	}

	typename const_iterator::value_type operator* () const {
	  return make_pair(DFA_matrix_base<CharTraits, Tag>::char_traits::to_char_type(i - c->v), *i);
	}

	self& operator ++ ()
	{
	  for(++i; i != (c->v + DFA_matrix_base<CharTraits, Tag>::char_traits::size) && *i == 0; ++i);
	  return *this;
	}

	self operator ++ (int)
	{
	  const_iterator tmp = *this;
	  ++(*this);
	  return tmp;
	}

	self& operator -- ()
	{
	  for(--i; *i == 0; --i);
	  return *this;
	}

	self operator -- (int)
	{
	  const_iterator tmp = *this;
	  --(*this);
	  return tmp;
	}
      };

      typedef reverse_iterator<const_iterator> const_reverse_iterator;

      // allocation/deallocation:
      edges_type(const Container *c = NULL)
	: container(c) { }

      const_iterator begin() const {
	const_iterator result(container->v, container);
	if (*(result.i) == 0)
	  ++result;
	return result;
      }

      const_iterator end() const {
	const_iterator result(container->v + self::char_traits::size, container);
	return result;
      }

      const_reverse_iterator rbegin() const {
	return const_reverse_iterator(end());
      }

      const_reverse_iterator rend() const {
	return const_reverse_iterator(begin());
      }

      bool empty() const {
	return size() == 0;
      }

      size_type size() const {
	return container->size();
      }

      size_type max_size() const {
	return char_traits::size;
      }

      // map operations:
      key_compare key_comp() const {
	return key_compare();
      }

      value_compare value_comp() const {
	return value_compare();
      }

      const_iterator find(const key_type& x) const {
	if (container->v[char_traits::to_int_type(x)] == 0)
	  return end();
	else
	  return const_iterator(container->v + char_traits::to_int_type(x),
				container);
      }

      size_type count(const key_type& x) const {
	return container->v[char_traits::to_int_type(x)] == 0 ? 0 : 1;
      }

      // comparison:
      friend bool operator==(const edges_type& x, const edges_type& y) {
	return x.container == y.container || *x.container == *y.container;
      }

      const_iterator lower_bound(const key_type &k) const {
	return find(k);
      }

      const_iterator upper_bound(const key_type &k) const {
	const_iterator i = find(k);
	return i == end() ? i : ++i;
      }

      pair<const_iterator, const_iterator> equal_range(const key_type &k) const {
	const_iterator i = find(k);
	return i == end() ? make_pair(i, i) : make_pair(i, ++i);
      }
    };

    // Backward compatibility
    typedef edges_type Edges;

    void del_trans(state_type s, const char_type &l)
    {
      Q[s]->edges().v[char_traits::to_int_type(l)] = 0;
      --Q[s]->edges().size();
      --trans_count_;
    }

    void change_trans(state_type s, const char_type &l, state_type new_aim) {
      Q[s]->edges().v[char_traits::to_int_type(l)] = new_aim;
    }

    state_type delta1(state_type s, const char_type &l) const {
      return Q[s]->edges().v[super::char_traits::to_int_type(l)];
    }

    void set_trans(state_type s, const char_type &l, state_type aim)
    {
      Q[s]->edges().v[super::char_traits::to_int_type(l)] = aim;
      ++Q[s]->edges().size();
      ++trans_count_;
    }

    edges_type delta2(state_type s) const {
      return edges_type(&(Q[s]->edges()));
    }

    DFA_matrix_base(unsigned long n = 0)
      : super(n)
    { }

  protected:
    using super::Q;
    using super::trans_count_;
  };

  template <class CharTraits = plain,
	    class Tag        = empty_tag>
  class DFA_matrix : public DFA_matrix_base<CharTraits, Tag, unsigned int>
  {
  public:
    typedef DFA_matrix_base<CharTraits, Tag, unsigned int> super;
    typedef typename super::char_traits                          char_traits;
    typedef typename super::tag_type                             tag_type;
    typedef typename super::char_type                            char_type;
    typedef typename super::state_type                           state_type;
    typedef DFA_matrix<CharTraits, Tag>      self;
    // using super::null_state;

    DFA_matrix(unsigned long n = 0)
      : super(n)
    { }
  };

  template <class CharTraits = plain,
	    class Tag        = empty_tag>
  class DFA_matrix_mini : public DFA_matrix_base<CharTraits, Tag, unsigned short>
  {
  public:
    typedef DFA_matrix_base<CharTraits, Tag, unsigned short> super;
    typedef typename super::char_traits                          char_traits;
    typedef typename super::tag_type                             tag_type;
    typedef typename super::char_type                            char_type;
    typedef typename super::state_type                           state_type;
    typedef DFA_matrix_mini<CharTraits, Tag>      self;
    // using super::null_state;

    DFA_matrix_mini(unsigned long n = 0)
      : super(n)
    { }
  };

} // namespace astl

#endif  // ASTL_DFA_MATRIX_H





