/*
 * ASTL - the Automaton Standard Template Library.
 * C++ generic components for Finite State Machine handling.
 * Copyright (C) 2000 Vincent Le Maout (vlemaout@lexiquest.fr).
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


//
//	File:	      dft_matrix.h
//  Version:    ASTL 1.2
//	Copyright:  Vincent LE MAOUT
//	Date:	      Wed Feb 14 12:37:51  2001
//	Descrition: Determinisic Finite Transducer Class Template ASTL1.2
//              Representation by matrix Q x Sigma
// 

#ifndef ASTL_CLASS_DFT_MATRIX
#define ASTL_CLASS_DFT_MATRIX

#include <iterator>   // iterator_tag
#include <functional> // less
#include <utility>    // pair
#include <vector>

#include <astl.h>     // alphabets
#include <tag.h>      // empty_tag

// DECISION D'IMPLEMENTATION ET DE CONCEPTION: la valeur par défaut de l'alphabet de sortie est le mot vide
// L'alphabet d'entrée doit se conformer à l'interface standard (char_trait standard + size, begin() et end())
// Avec éventuellement un symbole pour le mot vide (utiliser char_trait<Sigma1>::eof())
// L'alphabet de sortie doit posséder deux symboles spéciaux:
// Sigma2() => mot vide (constructeur par défaut)
// char_trait<Sigma2>::eof() => symbole d'entrée

template <class Tag, class Sigma1, class Sigma2>
struct MatrixStateData__
{
  Tag          _tag;
  unsigned int size;//    a count of the outgoing transitions
  typedef pair<unsigned int, typename Sigma2::char_type> value_type;
  value_type v[Sigma1::size];

  typedef MatrixStateData__ self;

  MatrixStateData__() : size(0) {
    uninitialized_fill(v, v + Sigma1::size, value_type(0, typename Sigma2::char_type()));
  }
  MatrixStateData__(const self &x)
    : _tag(x._tag), size(x.size) {
    uninitialized_copy(x.v, x.v + Sigma1::size, v);
  }
  self& operator= (const self &x) {
    _tag = x._tag;
    size = x.size;
    copy(x.v, x.v + Sigma1::size, v);
    return (*this);
  }
  bool operator== (const self &x) const {
    return size == x.size && equal(v, v + Sigma1::size, x.v);
  }
  const Tag& tag() const {
    return _tag;
  }
  Tag& tag() {
    return _tag;
  }
};

// Specialization of the state structure for empty tags

#ifndef WIN32
// VC++6.0 does not support partial template specialization
template <class Sigma1, class Sigma2>
struct MatrixStateData__<empty_tag, Sigma1, Sigma2>
{
  unsigned int size;//    a count of the outgoing transitions
  typedef pair<unsigned int, Sigma2> value_type;
  value_type v[Sigma1::size];
  static empty_tag _tag;      // remains consistent with generic version

  typedef MatrixStateData__ self;

  MatrixStateData__() : size(0) {
    uninitialized_fill(v, v + Sigma1::size, value_type(0, Sigma2()));
  }
  MatrixStateData__(const self &x)
    : size(x.size) {
    uninitialized_copy(x.v, x.v + Sigma1::size, v);
  }
  self& operator= (const self &x) {
    size = x.size;
    copy(x.v, x.v + Sigma1::size, v);
    return (*this);
  }
  bool operator== (const self &x) const {
    return size == x.size && equal(v, v + Sigma1::size, x.v);
  }
  const empty_tag& tag() const {
    return self::_tag;
  }

  empty_tag& tag() {
    return self::_tag;
  }
};

template <class Sigma1, class Sigma2> 
empty_tag MatrixStateData__<empty_tag, Sigma1, Sigma2>::_tag;
#endif  // WIN32

template <class _Sigma  = plain,
          class _Sigma2 = plain,
          class _Tag    = empty_tag>
class DFT_matrix : public DFA_concept
{
public:
  typedef _Sigma                     Sigma;
  typedef _Sigma2                    Sigma2;
  typedef typename _Sigma::char_type Alphabet;
  typedef _Sigma2                    Alphabet2;
  typedef _Tag                       Tag;
  typedef unsigned int               State;

private:
  typedef MatrixStateData__<Tag, Sigma, Sigma2> StateData;
  typedef DFT_matrix   self;
  typedef vector<char> set_F;

public:  
  typedef skip_blanks_iterator<StateData> const_iterator;

  class Edges
  {
  protected:
    typedef StateData Container;
    const Container *container;

  public:
    typedef Alphabet                    key_type;
    typedef State                       data_type;
    typedef pair<const Alphabet, State> value_type;
    typedef typename Sigma::int_type     size_type;

#ifdef WIN32
    class const_iterator : public iterator<bidirectional_iterator_tag, value_type>
#else
    class const_iterator : public bidirectional_iterator<value_type, ptrdiff_t>
#endif
    {
      friend class Edges;
      typedef const_iterator self;

    protected:
      const State *i;
      const Container *c;

      const_iterator(const State *_i, const Container *_c)
	: i(_i), c(_c) 
      { }

    public:
      const_iterator()
      { }

      bool operator== (const self& x) const {
	return (x.i == i); 
      }

#ifdef WIN32
      bool operator!= (const self& x) const {
	return !(*this == x);
      }
#endif

      typename const_iterator::value_type operator* () const { 
	return (make_pair(Sigma::unmap(i - c->v), *i)); 
      }

      self& operator ++ ()       
      { 
	for(++i; i != (c->v + Sigma::size) && *i == 0; ++i);
	return (*this);
      }

      self operator ++ (int) 
      { 
	const_iterator tmp = *this;
	++(*this);
	return (tmp);
      }

      self& operator -- () 
      { 
	for(--i; *i == 0; --i);
	return (*this);
      }

      self operator -- (int) 
      { 
	const_iterator tmp = *this;
	--(*this);
	return (tmp);
      }
    };

#ifdef WIN32
    typedef reverse_iterator<const_iterator, const_iterator::value_type> const_reverse_iterator;
#else
    typedef reverse_iterator<const_iterator> const_reverse_iterator;
#endif

    // allocation/deallocation:
    Edges(const Container *c = NULL)
      : container(c) { }
    
    const_iterator begin() const { 
      const_iterator result(container->v, container);
      if (*(result.i) == 0)
	++result;
      return (result); 
    }
    
    const_iterator end() const {
      const_iterator result(container->v + Sigma::size, container);
      return (result);
    }

    const_reverse_iterator rbegin() const { 
      return (const_reverse_iterator(end())); 
    }
    
    const_reverse_iterator rend() const {
      return (const_reverse_iterator(begin())); 
    }
    
    bool empty() const { 
      return (container->size == 0); 
    }
    
    size_type size() const { 
      return (container->size); 
    }

    size_type max_size() const { 
      return (Sigma::size); 
    }
    
    // map operations:
    const_iterator find(const key_type& x) const { 
      return (container->v[Sigma::map(x)] == 0) ? 
	end() : const_iterator(container->v + Sigma::map(x), container);
    }
    
    size_type count(const key_type& x) const { 
      return (container->v[Sigma::map(x)] == 0) ? 0 : 1;
    }
    
    // comparison:
    friend bool operator == (const Edges& x, const Edges& y) {
      return (x.container == y.container || *x.container == *y.container);
    }
  };

  // Back to DFA_matrix class

private:
  vector<StateData*> Q;
  State              I;     // Initial state
  set_F              F;     // Final states
  unsigned long      _state_count;
  unsigned long      _trans_count;

  State new_state(State q) {
    Q.push_back(new StateData(*Q[q]));
    F.push_back(F[q]);
    ++_state_count;
    _trans_count += Q[q]->size;
    return (Q.size() - 1);
  }

public:
  State null_state;

  const_iterator begin() const { 
    const_iterator result(&Q, 0);
    ++result;
    return (result);
  }

  const_iterator  end() const { 
    return (const_iterator(&Q, Q.size())); 
  }

  void initial(State s) { 
    I = s; 
  }

  State initial() const { 
    return (I); 
  }

  bool final(State s) const { 
    return (F[s] != '\0'); 
  }
  
  char& final(State s) { 
    return (F[s]); 
  }

  State new_state() {
    Q.push_back(new StateData);
    F.push_back('\0');
    ++_state_count;
    return (Q.size() - 1);
  }

  template <class OutputIterator>
  OutputIterator new_state(unsigned long how_many, OutputIterator x)
  {
    for(; how_many > 0; --how_many)
      *x++ = new_state();
    return (x);
  }
      
  void del_state(State s) 
  {
    if (s == initial()) initial(null_state);
    _trans_count -= Q[s]->size;
    delete Q[s];
    Q[s] = NULL;
    --_state_count;
    F[s] = '\0';
  }

  void del_trans(State s, const Alphabet &l)
  {
    Q[s]->v[Sigma::map(l)] = 0;
    --(Q[s]->size);
    --_trans_count;
  }

  void change_trans(State s, const Alphabet &l, State new_aim) {
    Q[s]->v[Sigma::map(l)].first = new_aim;
  }

  //////////////// new method //////////////////////////////////////////////
  void change_trans(State s, const Alphabet &i, const Alphabet2 &o, State new_aim) {
    pair<State, Alphabet2> &p = Q[s]->v[Sigma::map(i)];
    p.first = new_aim;
    p.second = o;
  }
  ///////////////////////////////////////////////////////////////////////////

  // Ancienne méthode (DFA) permettant de voir le transducteur comme l'automate
  // de la première projection:
  State delta1(State s, const Alphabet &l) const {
    return Q[s]->v[Sigma::map(l)].first;
  }

  //////////////// new method //////////////////////////////////////////////
  // Méthode permettant de voir le transducteur comme l'automate soujacent
  // i.e. l'alphabet est constitué de paires <Alphabet, Alphabet2>:
  State delta1(State s, const pair<Alphabet, Alphabet2> &p) const {
    return delta1(s, p.first);
  }

  State delta3(State s, const Alphabet &i, Alphabet2 &o) const {
    const pair<State, Alphabet2> &p = Q[s]->v[Sigma::map(i)];
    o = p.second;
    return p.first;
  }

  const pair<State, Alphabet2>& delta3(const pair<State, Alphabet> &p) const {
    return Q[p.first]->v[Sigma::map(p.second)];
  }

  const pair<State, Alphabet2>& delta3(State q, const Alphabet &a) const {
    return Q[q]->v[Sigma::map(a)];
  }
  ///////////////////////////////////////////////////////////////////////////

  void set_trans(State s, const Alphabet &l, State aim)
  {
    Q[s]->v[Sigma::map(l)].first = aim;
    ++(Q[s]->size);
    ++_trans_count;
  }

  //////////////// new methods //////////////////////////////////////////////
  void set_trans(State s, const Alphabet &i, const Alphabet2 &o, State aim)
  {
    Q[s]->v[Sigma::map(i)] = make_pair(aim, o);
    ++(Q[s]->size);
    ++_trans_count;
  }

  void set_trans(State s, const pair<Alphabet, Alphabet2> &p, State aim)
  {
    Q[s]->v[Sigma::map(p.first)] = make_pair(aim, p.second);
    ++(Q[s]->size);
    ++_trans_count;
  }
  //////////////////////////////////////////////////////////////////////////

  void copy_state(State from, State to) {
    _trans_count += Q[from]->size - Q[to]->size;
    *Q[to] = *Q[from];
    final(to) = final(from);
  }

  State duplicate_state(State q) {
    return new_state(q);  // private method
  }

  unsigned long state_count() const { 
    return (_state_count); 
  }
  
  unsigned long trans_count() const { 
    return (_trans_count); 
  }
  
  Tag& tag(State s) { 
    return (Q[s]->tag()); 
  }
  
  const Tag& tag(State s) const { 
    return (Q[s]->tag()); 
  }

  Edges delta2(State s) const { 
    return (Edges(Q[s])); 
  }

  DFT_matrix(unsigned long n = 0) :
    Q(1, (StateData*) 0), I(0), F(1, '\0'), _state_count(0UL), 
    _trans_count(0UL), null_state(0) { 
    Q.reserve(n + 1);
  }

  ~DFT_matrix()
  {
    const_iterator start, finish = end();
    for(start = begin(); start != finish; ++start)
      del_state(*start);
  }
};

#endif  // CLASS_DFA_MATRIX





