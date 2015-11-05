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

#ifndef ASTL_NEIGHBOR_H
#define ASTL_NEIGHBOR_H

#include <astl.h>
#include <stack>
#include <vector>
#include <utility>
#include <iostream>
#include <cstring>   // strlen

using namespace std;

namespace astl {

template <typename CharType>
struct default_costs
{
  int distance(const CharType &x, const CharType &y) const {
    return x == y ? 0 : 1000;
  }

  template <typename RandomAccessIterator>
  int insertion(RandomAccessIterator last, RandomAccessIterator where, const CharType &c) const {
    return 1000;
  }

  template <typename RandomAccessIterator>
  int deletion(RandomAccessIterator last, RandomAccessIterator where) const {
    return 1000;
  }

  template <typename RandomAccessIterator>
  int deletion_suffix(RandomAccessIterator first, RandomAccessIterator last) const {
    return 1000;
  }
};

// template <typename CharType>
// struct default_distance 
//   : public binary_function<CharType, CharType, int>
// {
//   // x is a word's letter, y is a transition letter:
//   int operator()(const CharType& x, const CharType& y) const {
//     return x == y ? 0 : 1000;
//   }
// };

// template <int Insertion = 1000, int Deletion = 1000, 
//   int Del_last = Deletion> 
// struct edition_cost_traits
// {
//   static const int INSERTION;
//   static const int DELETION;
//   static const int DELETION_LAST;
// };

// template <int Insertion, int Deletion, int Del_last> 
// const int edition_cost_traits<Insertion, Deletion, Del_last>::INSERTION = Insertion;
// template <int Insertion, int Deletion, int Del_last> 
// const int edition_cost_traits<Insertion, Deletion, Del_last>::DELETION = Deletion;
// template <int Insertion, int Deletion, int Del_last> 
// const int edition_cost_traits<Insertion, Deletion, Del_last>::DELETION_LAST = Del_last;

template <typename ForwardCursor, typename RandomAccessIterator, 
	  //	  typename DistanceFunction = default_distance<typename ForwardCursor::char_type>, 
	  //	  typename CostTraits = edition_cost_traits<> >
	  typename Costs = default_costs<typename ForwardCursor::char_type> >
class neighbor_cursor : public forward_cursor_concept
{
protected:
  ForwardCursor x;
  RandomAccessIterator last; 
  typedef vector<pair<RandomAccessIterator, int> > container; // position, distance 
  container d;    

public:
  typedef neighbor_cursor          self;
  typedef typename ForwardCursor::tag_type    tag_type;
  typedef typename ForwardCursor::char_type   char_type;  
  typedef typename ForwardCursor::char_traits char_traits;
  typedef typename ForwardCursor::state_type  state_type;

  neighbor_cursor(const ForwardCursor& c, RandomAccessIterator i, 
		  RandomAccessIterator j, int dist)
    : x(c), last(j) { 
    d.reserve(64);
    d.push_back(make_pair(i, dist));
  }

  neighbor_cursor()
  { }
  
  neighbor_cursor(const self &n) 
    : x(n.x), last(n.last) {
    d.reserve(64);
    for(typename container::const_iterator i = n.d.begin();
	i != n.d.end(); ++i)
      if (i->second >= 0) d.push_back(*i);
  }

  state_type src() const {
    return x.src();
  }

  const ForwardCursor& cursor() const {
    return x;
  }

  bool sink() const {
    return x.sink();
  }

  state_type sink_state() const {
    return x.sink_state();
  }

  bool forward(const char_type &a) {
    typename container::size_type i, j = d.size();
    bool ok = false;

    if (x.forward(a))
      for(i = 0; i < j; ++i) {
	// substitution & deletion
	int dist = d[i].second;
	for(RandomAccessIterator first = d[i].first; 
	    first != last && dist >= 0; 
	    dist -= Costs().deletion(last, first++)) {
	  //	    ++first, dist -= CostTraits::DELETION) { 
	  //	  int delta = DistanceFunction()(*first, a);
	  int delta = Costs().distance(*first, a);
	  if (dist >= delta) {
	    ok = true;
	    d.push_back(make_pair(first + 1, dist - delta));
	  }
	}
	// insertion:
	//	d[i].second -= CostTraits::INSERTION;
	//	int delta = Costs().insertion(last, d[i].first, a);
	d[i].second -= Costs().insertion(last, d[i].first, a);
	ok = ok || d[i].second >= 0;
      }
    return ok;
  }

  void forward() {
    typename container::size_type i, j = d.size();

    for(i = 0; i < j; ++i) {
      // substitution & deletion
      int dist = d[i].second;
      for(RandomAccessIterator first = d[i].first; 
	  first != last && dist >= 0; 
	    dist -= Costs().deletion(last, first++)) {
	  //	  ++first, dist -= CostTraits::DELETION) {
	//	int delta = DistanceFunction()(*first, x.letter());
	int delta = Costs().distance(*first, x.letter());
	if (dist >= delta)
	  d.push_back(make_pair(first + 1, dist - delta));
      }
      // insertion:
      //      d[i].second -= CostTraits::INSERTION;
      d[i].second -= Costs().insertion(last, d[i].first, x.letter());
    }
    x.forward();
  }

  bool src_final() const {
    if (x.src_final())
      for(typename container::const_iterator i = d.begin(); i != d.end(); ++i)
	//	if (i->second >= (last - i->first) *
	//	CostTraits::DELETION_LAST) 
	if (i->second >= Costs().deletion_suffix(i->first, last))
	  return true;
    return false;
  }
    
  tag_type src_tag() const {
    return x.src_tag();
  }

  bool exists(const char_type &a) const {
    if (x.exists(a)) 
      for(typename container::const_iterator i = d.begin(); i != d.end(); ++i)
	//	if (i->second >= CostTraits::INSERTION || 
	if (i->second >= Costs().insertion(last, i->first, a) || 
	    (i->first != last 
	     //	     && i->second >= DistanceFunction()(*i->first,
	     //	     a))) 
	     && i->second >= Costs().distance(*i->first, a))) 
	  return true;
    return false;
  }

  bool operator==(const self &s) const {
    return x == s.x;
  }

  char_type letter() const {
    return x.letter();
  }

  bool first() {
    if (x.first())
      do
	for(typename container::const_iterator i = d.begin(); 
	    i != d.end(); ++i) {
	  if (i->first != last) {
	    //	    if (i->second >= DistanceFunction()(*i->first,
	    //	    x.letter())) 
	    if (i->second >= Costs().distance(*i->first, x.letter())) 
	      return true;
	  }
    //	  else if (i->second >= CostTraits::INSERTION) return true;
	  if (i->second >= Costs().insertion(last, i->first, x.letter()))
		   return true;
	}
      while (x.next());
    return false;
  }
  
  bool next() {
    while (x.next())
      for(typename container::const_iterator i = d.begin(); i != d.end(); ++i) {
	if (i->first != last) {
	  //	  if (i->second >= DistanceFunction()(*i->first,
	  //	  x.letter())) 
	  if (i->second >= Costs().distance(*i->first, x.letter())) 
	    return true;
	}
    //	else if (i->second >= CostTraits::INSERTION) return true;
	if (i->second >= Costs().insertion(last, i->first, x.letter()))
		 return true;
      }
    return false;
  }

  bool find(const char_type &a) {
    return true;     // todo
  }

  state_type aim() const {
    return x.aim();
  }

  bool aim_final() const {
    self tmp = *this;
    tmp.forward();
    return tmp.src_final();
  }

  tag_type aim_tag() const {
    return x.aim_tag();
  }

  // WARNING: call is only allowed if the cursor is dereferenceable
  int distance() const {
    self tmp = *this;
    tmp.forward();
    typename container::const_iterator i, m = tmp.d.begin(); 
    for(i = tmp.d.begin() + 1; i != tmp.d.end(); ++i)
      if (i->second > m->second) m = i;
    
    return m->second;
  }
   
};

// template <typename ForwardCursor, typename ForwardIterator>
// inline
// neighbor_cursor<ForwardCursor, ForwardIterator>
// neighborc(const ForwardCursor& x, 
// 	  ForwardIterator i, ForwardIterator j, int d) {
//   return neighbor_cursor<ForwardCursor, ForwardIterator>(x, i, j, d);
// }

template <typename ForwardCursor, typename ForwardIterator, typename Distance>
inline
neighbor_cursor<ForwardCursor, ForwardIterator, Distance>
neighborc(const ForwardCursor& x, 
	  ForwardIterator i, ForwardIterator j, int d, const Distance&) {
  return neighbor_cursor<ForwardCursor, ForwardIterator, Distance>
    (x, i, j, d);
}

template <typename ForwardCursor>
inline
neighbor_cursor<ForwardCursor, const char*>
neighborc(const ForwardCursor& x, const char *word, int d) {
  return neighbor_cursor<ForwardCursor, const char*>
    (x, word, word + strlen(word), d);
}

} // namespace astl

#endif
