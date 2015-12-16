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

#ifndef ASTL_MINIMIZE_H
#define ASTL_MINIMIZE_H

#include <astl.h>
#include <determinize.h>

#include <iostream>
#ifndef __DFA_GLIMPSE_H_
#include <vector>
#endif
#include <iterator>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

namespace astl {

struct minimization_tag 
{
  unsigned long _degree_in, _equivalent_to;
  unsigned char _height;

  minimization_tag() : _degree_in(0), _equivalent_to(0), _height(0) { }
  unsigned char height() const { return (_height); }
  void          height(unsigned char x) { _height = x; }
  unsigned long degree_in() const { return (_degree_in); }
  void          degree_in(unsigned long x) { _degree_in = x; }
  unsigned long equivalent_to() const { return (_equivalent_to); }
  void          equivalent_to(unsigned long x) { _equivalent_to = x; }
  friend bool operator == (const minimization_tag &x, const minimization_tag &y) {
    return (x.degree_in() == y.degree_in() && x.equivalent_to() == y.equivalent_to());
  }
  minimization_tag& operator=(const empty&) { return *this; }
};

ostream& operator << (ostream &out, const struct minimization_tag &t)
{
  out << t.degree_in() << " " << t.equivalent_to();
  return (out);
}

// Computes the height of each state in A and
// stores it in a matrix (height x state)
// yields a partition of Q 
// if q is in Q, height(q) = Max {height(p) such that (q, a, p) is in T}
// and height(q) = 0 if q has no outgoing edges
// Uses method Tag::degree_in(long) to store the number of ingoing edges of each state
// Needs template functions swap and max 

template <class DFA, class Dstate>
unsigned char 
height(DFA& A, Dstate q, vector<vector < Dstate> *> &partition)
{
  const typename DFA::Edges edg = A.delta2(q);
  unsigned char max_height = 0;
  typename DFA::Edges::const_iterator q_trans, edg_end = edg.end();
  Dstate aim;

  for (q_trans = edg.begin(); q_trans != edg_end; ++q_trans)
    {
      aim = (*q_trans).second;
      A.tag(aim).degree_in(A.tag(aim).degree_in() + 1);
      if (A.tag(aim).degree_in()>1) //la hauteur est deja calculée
	max_height = max__(max_height,(unsigned char)(A.tag(aim).height()+1));       
      else 
	max_height = max__(max_height,(unsigned char)(height(A,aim,partition)+1));
    }
  if (partition.size() <= max_height)
    partition.push_back(new vector<Dstate>(1, q));
  else
    partition[max_height]->push_back(q);
  A.tag(q).height(max_height); 
  return (max_height);
}

// Set default transition of each state in [start, finish) toward
// start state (start is the reference state of the partition)

template <class DFA, class iterator>
void 
fusion(DFA &A, iterator start, iterator finish)
{
  typedef typename DFA::state_type Dstate;
  typename vector<Dstate>::iterator where;
  Dstate reference = *start;
  
  for (where = start + 1; where != finish; ++where)
    A.tag(*where).equivalent_to((unsigned long) reference);  
}

// Redirect the outgoing edges of a partition based on height
// to the partition reference state and decrease the number of
// ingoing edges of aim. (delete aim if it has no ingoing edges any more) 
template <class DFA, class State>
void
redirect_edges(DFA &A, vector<State> &v)
{
  typename vector<State>::iterator where;
  for (where = v.begin(); where != v.end(); ++where)
    {
      const typename DFA::Edges edg = A.delta2(*where);
      typename DFA::Edges::const_iterator trans, edg_end = edg.end();
      typename DFA::state_type new_aim, aim;
      for (trans = edg.begin(); trans != edg_end; ++trans)
	{
	  aim = (*trans).second;
	  new_aim = (typename DFA::state_type) A.tag(aim).equivalent_to();
	  if (new_aim != A.null_state)
	    {
	      A.change_trans(*where, (*trans).first, new_aim);
	      A.tag(aim).degree_in(A.tag(aim).degree_in() - 1);
	      if (A.tag(aim).degree_in() == 0)  // no more ingoing edges to the aim
		A.del_state(aim);
	    }
	}
    }
}

// Compares two states and returns int <0 or ==0 or >0
template <class DFA, class State>
int
compare_states(DFA &A, State q, State p)
{
  bool fq = A.final(q);
  bool fp = A.final(p);
  if (fq != fp)
    return ((int) fq - (int) fp);

  typename DFA::Edges::size_type q_trans_count = A.delta2(q).size();
  typename DFA::Edges::size_type p_trans_count = A.delta2(p).size();
  if (q_trans_count - p_trans_count != 0) 
    return (q_trans_count - p_trans_count);
  if (q_trans_count == 0) return (0);

  // Both states have the same finality and the same number of transitions
  // We apply a kind of lexicographical comparison
  typename DFA::Edges::const_iterator left_edg_end = A.delta2(q).end();
  typename DFA::Edges::const_iterator left = A.delta2(q).begin();
  typename DFA::Edges::const_iterator right = A.delta2(p).begin();

  for (; left != left_edg_end; ++left, ++right)
    {
      typedef typename DFA::char_type Dalph;
      typedef typename DFA::state_type Dstate;
      const pair<const Dalph, Dstate>& q_trans = *left;
      const pair<const Dalph, Dstate>& p_trans = *right;
    
      if (q_trans.first == p_trans.first) // Same letter
	{
	  if (q_trans.second < p_trans.second)
	    return (-1);
	  else
	    if (p_trans.second < q_trans.second)
	      return (1);
	}
      else
	if (q_trans.first < p_trans.first)
	  return (-1);
	else
	  return (1);
    }
  return (0);
}
  

// Groups equivalent states and returns the range they are in
template <class DFA, class Size_type, class State>
void
group_equivalent_states(DFA &A, vector<State> &v, Size_type start, 
			Size_type finish, pair<Size_type, Size_type> &rang)
{
  // Result is placed in 'range' argument
  Size_type g, d, pg;
  pg = g = start + 1;
  d = finish;
  State e = v[start];  // Reference state
  int res;
  
  while (g <= d) {
    res = compare_states(A, v[g], e);
    if (res == 0) {
      swap(v[pg],v[g]);
      pg++;
      g++;
    }
    else
      if (res < 0) g++;
      else {
	res = compare_states(A, v[d], e);
	if (res == 0)
	  if(g == pg) {
	    swap(v[pg],v[d]);
	    pg++;
	    g++;
	  }
	  else {  
	    swap(v[pg],v[g]);
	    swap(v[pg],v[d]);
	    pg++;
	  }
	else
	  if(res > 0) d--;
	  else {   
	    swap(v[g],v[d]);
	    g++;
	  }
      }
  }
  
  rang.second = d;
  rang.first  = pg;
}   

// Refine the partition based on state height
// All equivalent states are placed together in the vector then merged
template <class DFA, class Size_type, class State>
void 
refine_partition(DFA &A, vector<State> &v,
		 Size_type start, Size_type finish)
{
  if (start < finish)
    {
      pair<typename vector<State>::size_type, typename vector<State>::size_type> range;
  
      group_equivalent_states(A, v, start, finish, range);
      if (start < range.first - 1) fusion(A, v.begin() + start, v.begin() + range.first);
      if (range.first < range.second) refine_partition(A, v, range.first, range.second);
      if (range.second + 1 < finish) refine_partition(A, v, range.second + 1, finish);
    }
}


// Main minimization function
template <class DFA>
void 
acyclic_minimization(DFA &dfa)
{
  if (dfa.state_count() > 0) {
    typedef typename DFA::state_type Dstate;
    typedef vector<vector < Dstate> *> matrix;
    matrix partition(1, new vector< Dstate>); 
    height(dfa,dfa.initial(),partition);
    
    if (partition[1]->empty() == false) {
      typename matrix::iterator where;
      where = partition.begin();
      where++;
      for (; where != partition.end(); ++where) {
	redirect_edges(dfa, **where);
	refine_partition(dfa, **where, (typename matrix::size_type) 0, 
			 (**where).size() - 1);
	delete (*where);
      }
    }
  }
}	  

template <typename DFA, typename NFA>
void
reverse(const DFA &a1, NFA &a2)
{
  map<typename DFA::state_type, 
    safe<typename NFA::state_type, NFA::null_state> > m;

  // for each state:
  for(typename DFA::const_iterator src = a1.begin();
      src != a1.end(); ++src) {
    // create a new state if needed:
    safe<typename NFA::state_type, NFA::null_state> &new_aim = m[*src];
    if (new_aim == NFA::null_state) {
      new_aim = a2.new_state();
      a2.final(new_aim) = (a1.initial() == *src);
      if (a1.final(*src))
	a2.initial().insert(new_aim);
    }

    // for each transition:
    typename DFA::edges_type t = a1.delta2(*src);
    for(typename DFA::edges_type::const_iterator i = t.begin();
	i != t.end(); ++i) {
      // create a new state if needed:
      safe<typename NFA::state_type, NFA::null_state> &new_src = m[(*i).second];
      if (new_src == NFA::null_state) {
	new_src = a2.new_state();
	a2.final(new_src) = a1.initial() == new_src;
	if (a1.final((*i).second))
	  a2.initial().insert(new_src);
      }
      a2.set_trans(new_src, (*i).first, new_aim);
    }
  }
}

// Requirements: DFA1::char_type convertible to DFA2::char_type
template <typename DFA1, typename DFA2>
void
brzozowski(const DFA1 &a1, DFA2 &a2)
{
  typedef NFA_mmap<typename DFA1::char_traits> nfa_type;

  if (a1.state_count() > 0) {
    nfa_type tmp2;
    {
      DFA1 tmp3;
      {
	nfa_type tmp1;
	reverse(a1, tmp1);
	tmp3.initial(clone(tmp3, dfirst_markc(forwarddc(tmp1))));
      } 
      reverse(tmp3, tmp2);
    }
    a2.initial(clone(a2, dfirst_markc(forwarddc(tmp2))));
  }
}
    
} // namespace astl

#endif // ASTL_MINIMIZE_H
