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

#ifndef ASTL_DFA_COMPACT_H
#define ASTL_DFA_COMPACT_H

// Descrition:	  
//   ASTL 2.0 Determinisic Finite Automaton Class Template DFA_compact
//   Compact Representation with a single transition array 
//  



#ifndef _MSC_VER
#include <unistd.h>   // close(int)
#include <sys/mman.h>  // mmap
#include <fcntl.h>     // read
#include <errno.h>
#endif

#include <vector>
#include <utility>     // pair<>
#include <functional>  // identity<>
#include <iostream>

using namespace std;

namespace astl {

// ** WARNING: to be able to use tags and the binary save/load
// functionality (along side memory mapping), the tag type MUST BE A POD TYPE **

// Implementation notes
// This representation makes use of 4 arrays:
// 1. vector v1 for transitions labels (value_type == DFA::Alphabet)
// 2. vector v2 for transitions aims (value_type == unsigned long)
// 3. a bit vector for final states flags
// 4. vector of tags

// A matrix cell is made of two cells from vectors v1 and v2.
// if (v1[n] == 0) then there is no transition is this cell
//   then if (v2[n] != 0) then v2[n] is the tag index for the state in cell n+1
//   else the cell is free
// else v1[n] is the transition label and v2[n] is the transition target

template <class DFA, class tag_mapping = identity<typename DFA::tag_type> >
class DFA_compact : public DFA_concept
{
public:
  typedef typename DFA::char_traits         char_traits;
  typedef typename DFA::char_type           char_type;
  typedef unsigned long                     state_type;
  typedef typename tag_mapping::result_type tag_type;
 
private:
  typedef mapable_vector<int>::size_type size_type;
  typedef mapable_vector<bool> set_F;
  typedef DFA_compact self;

public:
  class const_iterator : public iterator<forward_iterator_tag, state_type>
  {
    friend class DFA_compact<DFA, tag_mapping>;
  private:
    state_type state;
    const mapable_vector<char_type> *ml;
    const mapable_vector<state_type> *ma;

    const_iterator(state_type x, const mapable_vector<char_type> *a, 
		   const mapable_vector<state_type> *s) 
      : state(x), ml(a), ma(s) { }    

  public:

    const_iterator() : ml(NULL), ma(NULL) { }
    state_type operator * () const { return (state); }
    const_iterator& operator ++ ()
    {
      for (; state < ml->size(); ++state) {
	if ((*ml)[state] == (char_type) 0 && (*ma)[state] != 0) {
	  // this is a cell holding a tag index
	  ++state; // there is a state after a tag
	  break;
	}
      }
      return (*this);
    }
    const_iterator operator ++ (int)
    {
      const_iterator tmp = *this;
      ++(*this);
      return (tmp);
    }
    const_iterator& operator = (const const_iterator &x) 
    {
      state = x.state;
      ml = x.ml;
      ma = x.ma;
      return (*this);
    }
    bool operator == (const const_iterator &x) const {
      return (state == x.state);
    }
    bool operator != (const const_iterator &x) const {
      return (!(*this == x));
    }
  };

  class edges_type
  {
  private:
    state_type                s;
    const mapable_vector<char_type>   *ml;
    const mapable_vector<state_type> *ma;

  public:
    edges_type(state_type st, const mapable_vector<char_type> &l, 
	       const mapable_vector<state_type> &a) 
      : s(st), ml(&l), ma(&a) { }

    typedef char_type key_type;
    typedef pair<const key_type, state_type> value_type;
    typedef less<char_type> key_compare;

    typedef value_type* pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef state_type size_type;
    typedef typename DFA::edges_type::difference_type difference_type;
    
    class value_compare : public binary_function<value_type, value_type, bool>
    {
      friend class edges_type;
      
    protected:
      key_compare comp;
      value_compare(key_compare c) : comp(c) { }
      
    public:
      bool operator () (const value_type& x, const value_type& y) {
	return (comp(x.first, y.first));
      }
    };

	class const_iterator : std::iterator<bidirectional_iterator_tag, value_type>
    {
    public:
      const_iterator(const char_type *l, 
		     const state_type *a, size_type pos_ = 0)
	: ml(l), ma(a), pos(pos_) { }

      typedef pair<char_type, state_type> value_type;

      const_iterator() { }
      const_iterator(const const_iterator& i) 
	: ml(i.ml), ma(i.ma), pos(i.pos) 
      { }

      bool operator == (const const_iterator &x) const {
	return ml == x.ml;
      }

      value_type operator * () const {
	return typename const_iterator::value_type(*ml, *ma);
      }

      const_iterator& operator ++ ()
      {
	for (++pos, ++ml, ++ma; pos < self::char_traits::size; 
	     ++pos, ++ml, ++ma)
	  if (*ml != (char_type) 0 &&
	      (size_t) self::char_traits::to_int_type(*ml) == pos)
	    break;
	return (*this);
      }
      const_iterator operator ++ (int)
      {
	const_iterator tmp = (*this);
	++(*this);
	return tmp;
      }
      const_iterator& operator -- ()
      {
	for (--pos, --ml, --ma; pos != 0; --pos, --ml, --ma)
	  if (self::char_traits::to_int_type(*ml) == pos) 
	    break;
	return (*this);
      }
      const_iterator operator -- (int)
      {
	const_iterator tmp = (*this);
	--(*this);
	return tmp;
      }

    protected:
      const char_type  *ml;
      const state_type *ma;
      size_type        pos;
    };

    // Back to edges_type class

    typedef const_iterator iterator;

    edges_type() : s((state_type) 0), ml(NULL), ma(NULL) { }

    key_compare key_comp() const
    { 
      key_compare tmp;
      return tmp; 
    }

    value_compare value_comp() const { return (value_compare(key_comp())); }

    state_type operator [] (const key_type& k) const
    {
      state_type q = s + self::char_traits::to_int_type(k);
      if ((*ml)[q] == k) return ((*ma)[q]);
      return (state_type) 0;
    }

    const_iterator begin() const {
      const_iterator r(ml->begin() + s, ma->begin() + s, 0);
      return ++r;
    }

    const_iterator end() const {
      return const_iterator(ml->begin() + s + self::char_traits::size, 
			    ma->begin() + s + self::char_traits::size, 
			    self::char_traits::size);
    }

    size_type size() const
    {
      typename self::char_traits::int_type offset;
      typename self::char_traits::int_type result = 0;
      
      for (offset = 0; offset < self::char_traits::size; ++offset)
	if ((*ml)[s + offset] != (char_type) 0 
	    && self::char_traits::to_int_type((*ml)[s + offset]) == offset)
	  ++result;
    
      return result;
    }

    bool empty() const { return size() == 0; }

    const_iterator find(const key_type &k) const
    {
      long mapped = self::char_traits::to_int_type(k);
      state_type result = s + mapped;
      if ((*ml)[result] == k)
	return const_iterator(ml->begin() + result, ma->begin() + result, mapped);
      return end();
    }
    
    size_type count(const key_type& k) const {
      return (*ml)[s + self::char_traits::to_int_type(k)] == k ? 1 : 0;
    }

    iterator lower_bound(const key_type &k) const { return find(k); }

    iterator upper_bound(const key_type &k) const { 
      iterator tmp = find(k);
      return tmp == end() ? tmp : ++tmp;
    }

    pair<iterator, iterator> equal_range(const key_type &k) const
    {
      return make_pair(lower_bound(k), upper_bound(k));
    }
  };

  // Back to DFA_compact class
protected:
  mapable_vector<char_type>  letters;
  mapable_vector<state_type> aims;
  mapable_vector<tag_type>   tags;
  state_type I;
  unsigned long _state_count;
  unsigned long _trans_count;

#ifndef _MSC_VER
  int fd;        // Memory mapping
#endif
  set_F F;

  bool free_cell(state_type s) const { 
    return (letters[s] == (char_type) 0 && aims[s] == (state_type) 0);
  }

  bool tagged_cell(state_type s) const {
    return (letters[s] == (char_type) 0 && aims[s] != (state_type) 0);
  }

public:
  static const state_type null_state = 0;

  ostream& write(ostream &out) const // binary
  { 
    out.write((const char *) &I, sizeof(I));
    out.write((const char *) &_state_count, sizeof(_state_count));
    out.write((const char *) &_trans_count, sizeof(_trans_count));
    letters.write(out);
    aims.write(out);
    tags.write(out);
    F.write(out);
    return out;
  }

  istream& read(istream &in) // binary
  {
    in.read((char *)&I, sizeof(I));
    in.read((char *)&_state_count, sizeof(_state_count));
    in.read((char *)&_trans_count, sizeof(_trans_count));
    letters.read(in);
    aims.read(in);
    tags.read(in);
    F.read(in);
    return in;
  }

#ifndef _MSC_VER
  // Memory mapping
  bool mmap(const string &path)
  {
    fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) return false;
    return mmap(fd);
  }

  bool mmap(int f)
  {
    ::read(f, (void*) &I, sizeof(I));
    ::read(f, (void*) &_state_count, sizeof(_state_count));
    ::read(f, (void*) &_trans_count, sizeof(_trans_count));
    if (!letters.mmap(f) || !aims.mmap(f) || !tags.mmap(f) || !F.mmap(f))
      return false;
    return true;
  }

  void munmap()
  {
    letters.munmap();
    aims.munmap();
    tags.munmap();
    F.munmap();
  }
#endif

  state_type initial() const { 
    return I; 
  }

  set_F::const_reference final(state_type s) const {
    return F[s];
  }

  state_type delta1(state_type s, const char_type &l) const
  {
    state_type q = s + self::char_traits::to_int_type(l);
    if (letters[q] == l) return aims[q];
    return null_state;
  }

  unsigned long state_count() const { 
    return _state_count; 
  }

  unsigned long trans_count() const { 
    return _trans_count; 
  }
  
  const tag_type& tag(state_type s) const { 
    return tags[aims[s - 1]]; 
  }

  edges_type delta2(state_type s) const { 
    return edges_type(s, letters, aims); 
  }

  const_iterator begin() const
  {
    if (state_count() > 0)
      return const_iterator((state_type) 1, &letters, &aims);
    else
      return end();
  }

  const_iterator end() const {
    return const_iterator(letters.size(), &letters, &aims);
  }

  float density() const
  {
    unsigned long occupied_cells = 0;
    state_type i, fin = letters.size();
    
    for (i = 0; i != fin; ++i)
      if (!free_cell(i))
	++occupied_cells;

    return ((float) occupied_cells / (float) fin);
  }

  void stats(ostream &out = cout) const
  {
    out << "Matrix size       : " << letters.size() << " cells (" 
	<< aims.size() * sizeof(state_type) + letters.size() 
      * sizeof(char_type) + tags.size() * sizeof(tag_type) + 
      F.size() * sizeof(bool) << " bytes)" << endl;
    out << "Matrix density    : " << density() << endl;
    out << "States            : " << state_count() 
	<< " (" << aims.size() * sizeof(state_type)
	<< " bytes)" << endl;
    out << "Transitions       : " << trans_count() << " (" 
	<< letters.size() * sizeof(char_type) 
	<< " bytes)" << endl;
    out << "Tags              : " << tags.size() << " (" 
	<< tags.size() * sizeof(tag_type)
	<< " bytes)" << endl;
    out << "Final states bool : " << F.size() << " (" 
	<< F.size() * sizeof(bool) 
	<< " bytes)" << endl;
    out << "Space waste ratio : " << (1. - density()) * 100. << "%" << endl;
  }

  ~DFA_compact() {
#ifndef _MSC_VER
    letters.munmap();
    aims.munmap();
    tags.munmap();
    F.munmap();
    if (fd > 0) close(fd);
#endif
  }

  // TEMPORARY CODE:
  DFA_compact()
#ifndef _MSC_VER
    : fd(0)
#endif
  { 
    I = null_state;
  }

  DFA_compact(istream &in) 
#ifndef _MSC_VER
    : fd(0)
#endif
  {
    I = null_state;
    read(in);
  }
  
  DFA_compact(const DFA &dfa, long opt_value = 2) 
    : letters((unsigned int) (dfa.state_count() * 2 + dfa.trans_count())), 
      aims((unsigned int) (dfa.state_count() * 2 + dfa.trans_count())), 
      tags((unsigned int) 1), I(null_state), _state_count(0), _trans_count(0),
#ifndef _MSC_VER
      fd(0), 
#endif
      F((unsigned int) dfa.state_count())
  {
    if (dfa.state_count() > 0) {
      tag_mapping map_tag;
      typename DFA::const_iterator i, end_i = dfa.end();
      vector<state_type> old2new(dfa.state_count());//indexed by DFA::state_type
      state_type pos, solid_first_free = 0UL, first_free = 0UL;
      typename DFA::edges_type::const_iterator trans, edg_end;
      long count = 0;  // For time optimization of compacting process

      for (i = dfa.begin(); i != end_i; ++i) {
	++count;
	++_state_count;

	if (count == opt_value) {  // discard solid_first_free ?
	  for(++solid_first_free; solid_first_free < letters.size() && 
		!free_cell(solid_first_free); ++solid_first_free);
	  count = 0;
	}

	while (solid_first_free + self::char_traits::size 
	       >= letters.size()) {  // Matrix is too small
	  // Doubling the size
	  letters.resize(2 * letters.size(), (char_type) 0);
	  aims.resize(2 * aims.size(), (state_type) 0);
	}
	    
	first_free = solid_first_free;

	while (1) {
	  pos = first_free + 1;  // Because we put the tag at first_free
	  typename DFA::edges_type edg = dfa.delta2(*i);
	  edg_end = edg.end();

	  // Trying to put every transition by beginning at pos (first_free)
	  for (trans = edg.begin(); trans != edg_end; ++trans)
	    if (!free_cell(pos + self::char_traits::to_int_type((*trans).first)))
	      break;
	      
	  if (trans == edg_end) {  // Ok, insert state
	    // Storing new "name" of the state
	    if (old2new.size() <= (unsigned long) *i)
	      old2new.resize((unsigned long) (*i) * 2, null_state);
	    old2new[(unsigned long) *i] = pos;

	    // Putting the tag if not already in vector tags
	    tag_type t_tmp = map_tag(dfa.tag(*i));
	    typename mapable_vector<tag_type>::iterator position 
	      = find(tags.begin() + 1, tags.end(), t_tmp);
	    aims[pos - 1] = (state_type) (position - tags.begin()); 
	    if (position == tags.end()) 
	      tags.push_back(t_tmp);
		  
	    // Put the state in F if needed
	    if (dfa.final(*i)) {
	      if (F.size() <= pos) F.resize(pos * 2, false);
	      F[pos] = true;
	    }
		  
	    // Adding transitions
	    unsigned long mapped_letter;
	    for (trans = edg.begin(); trans != edg_end; ++trans) {
	      mapped_letter = 
		self::char_traits::to_int_type((*trans).first);
	      letters[pos + mapped_letter] = (*trans).first;
	      aims[pos + mapped_letter] = 
		(state_type) (*trans).second;
	      ++_trans_count;
	    }
	    if (first_free == solid_first_free)
	      count = opt_value - 1; // Force solid_first_free updating
	    break;    // move on to the next state
	  }
		
	  // first_free is updated
	  for(++first_free; first_free < letters.size() && 
		!free_cell(first_free); ++first_free);
		
	  while (first_free + self::char_traits::size >= 
		 letters.size()) {  // Matrix is too small
	    // Doubling the size
	    letters.resize(2 * letters.size(), (char_type) 0);
	    aims.resize(2 * aims.size(), (state_type) 0);
	  }
	}
      }
	
      // Setting edges aim to states new "names"
      state_type last_one = 0;
      for (pos = 0; pos != letters.size(); ++pos)
	if (letters[pos] != (char_type) 0)
	  aims[pos] = old2new[aims[pos]];
	else
	  if (aims[pos] != 0) // letter == 0, aim != 0 => aim is a tag index
	    last_one = pos + 1 + self::char_traits::size;

      if (dfa.initial() != dfa.null_state)
	I = old2new[(unsigned long) dfa.initial()];
      letters.resize(last_one + 1, (char_type) 0);
      aims.resize(last_one + 1, (state_type) 0);
      F.resize(last_one + 1, false);
    }
  }
};

template <typename DFA, typename TagMapping>
const typename DFA_compact<DFA, TagMapping>::state_type 
DFA_compact<DFA, TagMapping>::null_state;

} // namespace astl

#endif   // DFA_COMPACT_H





