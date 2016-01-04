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
//	File:		nfa_epsilon.hh
//	Copyright:	Vincent LE MAOUT
//	Date:		Thu Jul 30 14:15:30 MET DST 1998


#ifndef ASTL_NFA_Epsilon
#define ASTL_NFA_Epsilon

#include <set>
#include <stack>
#include <vector>
#include <functional>
#include <iterator>
#include <algorithm>
#include <map>
#include <state_ref.h>
#include <dfa_default.h>
#include <tag.h>

#ifdef WIN32
using namespace std;
#endif

template <class Alphabet, class Tag>
struct t_state {
  Tag _leTag;
    
  // si _lalettre==Alphabet() on a 0, 1 ou 2 deux epsilon transitions (aim1 et/ou aim2) 
  // sinon on a une transition (aim1) et peut-etre une epsilon trans (aim2)
  // la transition par defaut est dans default_t
  Alphabet _laLettre;
  t_state<Alphabet, Tag> *_aim1, *_aim2, *default_t;

  t_state() 
    : _leTag(Tag()), _laLettre(0), _aim1(0),_aim2(0), default_t(0)
  { }
};


template <class edges_iterator, class State>
edges_iterator& operator_plus_plus (edges_iterator &t, const State &)
{
  // Precondition:all states in q have at least 1 transition and/or a default trans				
  if (t.i == t.e->letters.end())    // at edges end ?
  {
    t.e = NULL;
    return (t);
  }
  t.result = State();
  t.result = t.e->nfa->delta1(t.e->q, *(t.i));
  ++t.i;
  return (t);
}

template <class _Sigma,class _Tag> 
class NFA_Epsilon
{
public:
  typedef _Sigma                    Sigma;
  typedef _Tag                      Tag;
  typedef typename _Sigma::Alphabet Alphabet; 		
private:
  typedef NFA_Epsilon<Sigma, Tag> self;
  typedef t_state<Alphabet, Tag> _state;
  typedef _state * p_state;
  
public:
  struct compare_state : binary_function<p_state, p_state, bool> 
  {
    bool operator ()(const p_state & x, const p_state & y) const 
    {
      if (x->_leTag == y->_leTag)
        return (x < y);
      else
        return (!(x->_leTag < y->_leTag)) ;
    }
  };

  //////////////////////////////  typedef state_ref<p_state , compare_state> State;
  typedef set<p_state, compare_state> State;
	
private:
  mutable vector<p_state> closure;
  vector<State> Q;
  State         I;
  Alphabet      lettre_nulle;
  Tag           tag_nul;
  unsigned long _trans_count, _state_count;

public: 
  class Edges
  {
    friend class NFA_Epsilon<_Sigma, _Tag>;  
  public:
    State q;
    const NFA_Epsilon<_Sigma, _Tag> *nfa;
    vector<Alphabet> letters;

  public:
    // typedefs:

    typedef Alphabet                     key_type;
    typedef pair<Alphabet, State>  value_type;
    typedef less<Alphabet>               key_compare;
    typedef value_type*                  pointer;
    typedef const value_type   &         const_reference;
    typedef unsigned int                 size_type; 
    typedef int                          difference_type;
    //    typedef    value_compare;

#ifdef WIN32    
    class const_iterator
      : public iterator<forward_iterator_tag, value_type, difference_type>
#else
    class const_iterator 
      : public forward_iterator<value_type, difference_type>
#endif

    {
      friend class Edges;
    public:
      const Edges *e;
      vector<Alphabet>::const_iterator i;
      State result;

      const_iterator(const Edges *_e) : e(_e), i(_e->letters.begin())
      { 
	++(*this);
      }

    public:
      const_iterator() : e(NULL)
      { }

      const_iterator(const const_iterator &x)
	: e(x.e), i(x.i)
      { }

      bool operator == (const const_iterator& x) const
      {
	// WARNING:
	return (x.e == e);
      }

      value_type operator * () const {
	return (make_pair(i[-1], result));
      }

      const_iterator& operator ++ () {
	// Precondition:all states in q have at least 1 transition and/or a default trans				
	if (i == e->letters.end())    // at edges end ?
	{
	  e = NULL;
	  return (*this);
	}
	result = State();
	result = e->nfa->delta1(e->q, *i);
	++i;
	return (*this);
  //	return (operator_plus_plus(*this, State()));
      }

      const_iterator operator ++ (int) 
      { 
	const_iterator tmp = *this;
	++(*this);
	return (tmp);
      }
    };
    
    // allocation/deallocation:

    
    Edges() { }
    
    Edges(const Edges &e) : q(e.q), nfa(e.nfa), letters(e.letters)
    { }

  private:
    Edges(const State & s, const NFA_Epsilon<_Sigma, _Tag> *_nfa, vector<Alphabet> &l) 
      : q(s), nfa(_nfa), letters(l)
    { }

  public:
    ~Edges() { }
    
    // accessors:
        
    //    key_compare key_comp() const { return (edg.key_comp()); } 
    //    value_compare value_comp() const { return (edg.value_comp()); }
    const_iterator begin() const { 	
      return (const_iterator(this)); 
      // Postcondition:no states without transitions in q
    }
    
    const_iterator end() const { 
      const_iterator i;
      return (i); 
    }
    
    //    bool empty() const { 
    //     return (q.empty()); 
    //   }

    //   size_type size() const {
    //    return (q.size());
      /*
      size_type how_many = q.size();
      State::const_iterator first, last = q.end();
      for(first = q.begin(); first != last; ++first)
	if ((*first)->_aim2)
	  ++how_many;
      return (how_many); 
      */
    //   }
 
    //		size_type max_size() const { return (edg.max_size()); }

    // multimap operations:

    //        const_iterator find(const key_type& x) const { return (const_iterator(edg.find(x))); }
    //      size_type count(const key_type& x) const { return (edg.count(x)); }
    //      const_iterator lower_bound(const key_type& x) const { return (const_iterator(edg.lower_bound(x))); }
    //      const_iterator upper_bound(const key_type& x) const { return (const_iterator(edg.upper_bound(x))); }
    //      pair<const_iterator, const_iterator> equal_range(const key_type& x) const
    //      {
    //      pair<typename NFA::Edges::const_iterator, typename NFA::Edges::const_iterator> p = edg.equal_range(x);
    //      return (make_pair(const_iterator(p.first), const_iterator(p.second)));
    //      }

    // comparison:
        
    friend bool operator == (const Edges& x, const Edges& y) {
      return (x.q == y.q);
    }
  };
    
   

  // Back to NFA_Epsilon class

  State null_state;

  NFA_Epsilon(unsigned long n = 0) 
    : Q(), I(), lettre_nulle(0),
      tag_nul(Tag()), _trans_count(0), _state_count(0), null_state() 
  { 
    closure.reserve(2048);
  }

  typedef vector<State>::const_iterator const_iterator;

  State new_state()
  {    
    _state *q = new _state; 
    State r;
    r.insert(q);
    Q.push_back(r);
    ++_state_count;
    //    cout << "NFA_epsilon::new_state() = " << q << endl;
    return (r);
  }

  template <class OutputIterator>
  OutputIterator new_state(unsigned long how_many, OutputIterator x)
  {
    for(; how_many > 0; --how_many)
      *x++ = new_state();
    return x;
  }

  /*
    void set_trans(State &from, const Alphabet &a, State &to)
    {
    //ne fait pas grand chose 
    }
    */
  private :
  void set_trans(_state *from, const Alphabet &a, _state *to)
  {
    from->_aim1 = to;
    from->_laLettre = a;
    ++_trans_count;
    //    cout << "set_trans(" << from << ", '" << a << "', " << to << ")" << endl;
  }

  void set_trans(_state *from, _state *to)
  {
    from->_aim2 = to;   
    I = epsilon_closure(I);
    ++_trans_count;
    //    cout << "set_epsilon_trans(" << from << ", " << to << ")" << endl;
  }
  void set_trans(_state *from, _state *to1, _state *to2)
  {
    from->_laLettre = Alphabet(0);
    from->_aim1 = to1; 
    from->_aim2 = to2; 
    I = epsilon_closure(I);
    _trans_count += 2;
    //   cout << "set_2_epsilon_trans(" << from << ", " << to1 << ", " << to2 << ")" << endl;
  }

  void set_default_trans(_state *from, _state *to)
  {
    from->default_t = to;
    ++_trans_count;
    //    cout << "set_default_trans(" << from << ", " << to << ")" << endl;
  }

public:
  // attention : a finir
  void set_trans(const State & from, const Alphabet &a, const State & to)
  {
    set_trans(*(from.begin()), a, *(to.begin()));
  }
  void set_trans(const State & from, const State & to)
  {
    set_trans(*(from.begin()), *(to.begin()));
  }
  void set_trans(const State & from, const State & to1, const State & to2)
  {
    set_trans(*(from.begin()), *(to1.begin()), *(to2.begin()));
  }
  void set_default_trans(const State & from, const State & to)
  {
    set_default_trans(*(from.begin()), *(to.begin()));
  }

  /////////////////////////////


  public :

  template <class InputIterator>  
  void del_trans(const State & s, const Alphabet &a)
  {
    //
  }


  void del_trans(const State & s, const Alphabet &l, const State & aim)
  {
    //
  }

  void change_trans(const State & s, const Alphabet &l, const State & former_aim, const State & new_aim)
  {
    //
  }

  void copy_state(const State & from, const State & to) {
    **(to.begin()) = **(from.begin());
  }

  void del_state(const State &s) {
    --_state_count;
  }

  State duplicate_state(const State & s) {
    return State(s);
  }

  bool final(const State & s) const {
///		cout << "nfa_epsilon::final : tag = " << tag(s) << " et tag nul = " << tag_nul << endl;
    return (this->tag(s) > tag_nul);
  }


  Tag& tag(State & s) {
    return (*(s.begin()))->_leTag ;
  }

  const Tag& tag(const State & s) const {
    return (*(s.begin()))->_leTag ;
  }

  Tag& tag(p_state p) {
    return (p->_leTag);
  }

  const Tag& tag(p_state p) const {
    return (p->_leTag);
  }

  State epsilon_closure(const State & s) const
  {
    State r;
    _state *t;

    State::const_iterator itb = s.begin();
    State::const_iterator ite = s.end() ; 

    for(; itb != ite ; ++itb)  
      closure.push_back(*itb);

//    cout << "epsilon cloture: tags = ";
    for(; ! closure.empty();)
    {
      t = closure.back();
      closure.pop_back();
      if (t->_aim2)    // epsilon trans ?
        closure.push_back(t->_aim2);
//      cout << t->_leTag << " ";
      if (t->_leTag != tag_nul || t->_laLettre != lettre_nulle || t->default_t)
        r.insert(t);
      if (t->_laLettre == lettre_nulle && t->_aim1)
        closure.push_back(t->_aim1);
    }

//    cout << endl;
    return r;
  }
      
  /*
  State epsilon_closure(const State & s) const
  {
    State r;
    _state *t;

    State::const_iterator itb = s.begin();
    State::const_iterator ite = s.end() ; 

    for(; itb != ite ; ++itb)  
      closure.push_back(*itb);

    for(; ! closure.empty();)
    {
      t = closure.back();
      closure.pop_back();
      if (t->_laLettre == lettre_nulle) 
      {
	if (t->_aim2)  // des epsilon transitions ?
	{
	  closure.push_back(t-> _aim2);
	  if (t->_aim1)
	    closure.push_back(t-> _aim1);

          if (t->default_t != NULL)
            r.insert(t);
	}
	else  // lettre nulle + pas d epsilon trans =>
	  r.insert(t);
      }
      else
      {
	r.insert(t);
	if (t->_aim2)
	  closure.push_back(t-> _aim2);
      }
    }
    return r;
  }
*/

  State delta1_default(const State & s) const
  { 
    
    State r;

    State::const_iterator itb = s.begin();
    State::const_iterator ite = s.end() ; 

    for(; itb != ite ; ++itb)  
      if ((*itb)->default_t != NULL)         // default trans
	      r.insert((*itb)->default_t);

    return (epsilon_closure(r));
  }
    /*
    for(; ! closure.empty();)
    {
      t = closure.back();
      closure.pop_back();
      ///      r.insert(t);   				  

      if (t->_laLettre == lettre_nulle) 
      {
	if (t->_aim2)
        {
	  closure.push_back(t-> _aim2);
	  if (t->_aim1)
	    closure.push_back(t-> _aim1);
	  if (t->default_t)
	    r.insert(t);
        }
	else
	  r.insert(t);
      }
      else
      {
	if (t->_aim2)
	  closure.push_back(t-> _aim2);
	r.insert(t);
      }
    }
    return  r;
  }
*/
  State delta1(const State & s, const Alphabet &l) const
  { 
    
    State r;

    State::const_iterator itb = s.begin();
    State::const_iterator ite = s.end() ; 

    for(; itb != ite ; ++itb)  
      if ((*itb)->_laLettre == l )
    	  r.insert((*itb)->_aim1);
/////      else
/////	      if ((*itb)->default_t != NULL)         // default trans
/////	        r.insert((*itb)->default_t);
/////   remplace par:
		if (r.empty())
			return (delta1_default(s));

    return (epsilon_closure(r));
  }
    /*
    for(; ! closure.empty();)
    {
      t = closure.back();
      closure.pop_back();
      ///      r.insert(t);   				  

      if (t->_laLettre == lettre_nulle) 
      {
	if (t->_aim2)
        {
	  closure.push_back(t-> _aim2);
	  if (t->_aim1)
	    closure.push_back(t-> _aim1);
	  if (t->default_t)
	    r.insert(t);
        }
	else
	  r.insert(t);
      }
      else
      {
	r.insert(t);
	if (t->_aim2)
	  closure.push_back(t-> _aim2);
      }
    }
    return  r;
  }
*/
  Edges delta2(const State & s) const
  {
    State q;
    vector<Alphabet> v;
    State::const_iterator first, last = s.end();
    for(first = s.begin(); first != last; ++first)
      if ((*first)->_laLettre != lettre_nulle)
      {
//				cout << "lettre nulle == " << lettre_nulle << " et la lettre = " << (*first)->_laLettre << endl;
//		cout << "delta2:test de l'etat " << *first << "   1er but = " << (*first)->_aim1 << endl;
	q.insert(*first);
	v.push_back((*first)->_laLettre);
      }
///////      else
//////	if  ((*first)->default_t)
///////	  q.insert(*first);
   
    return Edges(q, this, v);
  }
      
  State initial() const {
    return (I);
  }

  void initial(const State & i) {
    I = epsilon_closure(i);
  }

  unsigned long state_count() const {
    return (_state_count);
  }

  unsigned long trans_count() const {
    return (_trans_count);
  }

  const_iterator begin() const {
    return (Q.begin());
  }

  const_iterator end() const {
    return (Q.end());
  }
};

template <class NFA, class InputIterator>
class t_iterator : public iterator<input_iterator_tag, NFA::Tag>
{
  InputIterator where;
  const NFA &nfa;

public:
  t_iterator(InputIterator i, const NFA &_nfa) : where(i), nfa(_nfa)
  { }

  t_iterator(const t_iterator &x) : where(x.where), nfa(x.nfa)
  { }

  const NFA::Tag& operator * () const {
    return (nfa.tag(*where));
  }

  t_iterator& operator ++ () {
    ++where;
    return (*this);
  }

  t_iterator operator ++ (int) {
    t_iterator tmp = *this;
    ++(*this);
    return (tmp);
  }

  bool operator == (const t_iterator &x) const {
    return (where == x.where);
  }

  bool operator != (const t_iterator &x) const {
    return (!(*this == x));
  }
};

template <class _DFA, class Sigma, class Tag>
void nfa_epsilon_to_dfa(const NFA_Epsilon<Sigma, Tag> &nfa,
                        DFA_default<_DFA> &dfa)
{
  typedef NFA_Epsilon<Sigma, Tag> NFA;
  typedef DFA_default<_DFA> DFA;
  NFA::State Q = nfa.initial();
  if (Q == nfa.null_state)
    return;
  DFA::State q = dfa.new_state(), p;
  dfa.initial(q);
//	cout << "dterminisation de l'etat initial" << endl;
  tag_traits<NFA::Tag>::tag_determinize(t_iterator<NFA, NFA::State::const_iterator>(Q.begin(), nfa),
                                        t_iterator<NFA, NFA::State::const_iterator>(Q.end(), nfa),
                                        dfa.tag(q));         
  map<NFA::State, DFA::State> mapping;
  typedef map<NFA::State, DFA::State>::iterator it;
  it where, where_to;
  it path[10240];
  it *top = path;

  // cout << "Q (initial) size = " << Q.size() << endl;
  *top = mapping.insert(make_pair(Q, q)).first;
  //  path.push_back((mapping.insert(make_pair(Q, q))).first);
  //  cout << "titi" << endl;
  ++top;
  pair<NFA::Alphabet, NFA::State> trans;
  while (top != path)
  {
    //  cout << "top != path" << endl;
    --top;
    // cout << "Q size = " << (**top).first.size() << endl;
    where = *top;
    q = (*where).second;

    //  cout << "(*where).second = " << q << endl; 
    // default transitions:
//		cout << "transitions par defaut" << endl;
    Q = nfa.delta1_default((*where).first);
    if (!(Q == nfa.null_state))
    {
      //      cout << "dans default Q size = " << Q.size() << endl;
      where_to = mapping.find(Q);
      if (where_to != mapping.end())
      {
	///	cout << "dfa set default trans" << endl;
        dfa.set_trans(q, dfa.default_letter, (*where_to).second);
      }
      else
      {
	//	cout << "default:creating state " << ( p = dfa.new_state()) << endl;
        p = dfa.new_state();
        tag_traits<NFA::Tag>::tag_determinize(t_iterator<NFA, NFA::State::const_iterator>(Q.begin(), nfa),
                                              t_iterator<NFA, NFA::State::const_iterator>(Q.end(), nfa),
                                              dfa.tag(p));         
        dfa.set_trans(q, dfa.default_letter, p);
        dfa.final(p) = nfa.final(Q);
        *top = mapping.insert(make_pair(Q, p)).first;
	++top;
	//       path.push_back((mapping.insert(make_pair(Q, p))).first);
      }
    }

    // normal transition:
//		cout << "transitions normales" << endl;
    NFA::Edges edges = nfa.delta2((*where).first);
    NFA::Edges::const_iterator first, last = edges.end();
    for(first = edges.begin(); first != last; ++first)
    {
//			cout << "lettre = " << (*first).first << endl;
      trans = *first;
      where_to = mapping.find(trans.second);
      if (where_to != mapping.end())
        dfa.set_trans(q, trans.first, (*where_to).second);
      else
      {
      	p = dfa.new_state();
        tag_traits<NFA::Tag>::tag_determinize(t_iterator<NFA, NFA::State::const_iterator>(trans.second.begin(), nfa),
                                              t_iterator<NFA, NFA::State::const_iterator>(trans.second.end(), nfa),
                                              dfa.tag(p));         
	//	cout << "set_trans(" << q << ", " << trans.first << ", " << p << ")" << endl;
        dfa.set_trans(q, trans.first, p);
        dfa.final(p) = nfa.final(trans.second);
        *top = (mapping.insert(make_pair(trans.second, p))).first;
	++top;
	//        path.push_back((mapping.insert(make_pair(trans.second, p))).first);
      }
    }
  }
}


#endif // ASTL_NFA_Epsilon














