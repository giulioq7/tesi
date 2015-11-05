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

#ifndef LILIAN_FA_COMPRESS_H
#define LILIAN_FA_COMPRESS_H


#include <concept.h>
#include <alphabet.h>
#include <tag.h>
#include <tools.h>

#ifndef _MSC_VER
#include <unistd.h>   // close(int)
#include <sys/mman.h>  // mmap
#include <fcntl.h>     // read
#include <errno.h>
#endif

#include <vector>
#include <utility>     // pair<>
#include <functional>  // identity<>
#include <algorithm>   // rotate<>()

using namespace std;

// Notes: transition access time is linear
//        can be binary read/written from/to disk and memory mapped
//        provided that *** the tag type is a POD type ***
//        *** do not use the letter '\0' ***

namespace astl {

template <typename CharTraits = plain, typename TagType = empty_tag>
class FA_compress : public FA_concept
{
public:
  typedef CharTraits       char_traits;
  typedef typename CharTraits::char_type char_type;
  typedef unsigned int     state_type;
  typedef TagType          tag_type;
  typedef FA_concept       concept;

#ifndef _MSC_VER
protected:
#else
public:
#endif
  typedef mapable_vector<bool> set_F;
  typedef FA_compress self;

  mapable_vector<TagType>    tags;
  mapable_vector<char_type>  letters;
  mapable_vector<state_type> aims_tags;

public:
  class const_iterator : public iterator<forward_iterator_tag, state_type>
  {
  protected:
    state_type state;
    const mapable_vector<char_type> *ml;

  public:
    const_iterator() {}
    const_iterator(state_type x, const mapable_vector<char_type> &a)
      : state(x), ml(&a) 
    { }

    state_type operator*() const { 
      return state; 
    }

    const_iterator& operator++() {
      while ((*ml)[++state] != (char_type) 0); // sentry at the end anyway
      return *this;
    }

    const_iterator operator ++ (int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator &x) const {
      return (state == x.state);
    }
    bool operator != (const const_iterator &x) const {
      return (!(*this == x));
    }
  };

  class edges_type
  {
  private:
    //    const mapable_vector<char_type> *a;
    //    const mapable_vector<state_type> *p;
    state_type q;
    const FA_compress *fa;

  public:
    edges_type()
    { }

    edges_type(state_type s, const FA_compress &f)
      //	       const mapable_vector<char_type> &i,
      //	       const mapable_vector<state_type> &j)
      //      : a(&i), p(&j), q(s)
      : q(s + 1), fa(&f)
    { }

    typedef char_type key_type;
    typedef pair<const key_type, state_type> value_type;
    typedef less<char_type> key_compare;

    typedef value_type* pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef state_type size_type;
    typedef int difference_type;
    
    struct value_compare 
      : public binary_function<value_type, value_type, bool>
    {
      key_compare comp;
      value_compare(key_compare c) : comp(c) { }
      bool operator ()(const value_type& x, const value_type& y) {
	return comp(x.first, y.first);
      }
    };

    class const_iterator 
      : iterator<bidirectional_iterator_tag, value_type>
    {
    protected:
      typename mapable_vector<char_type>::const_iterator a;
      typename mapable_vector<state_type>::const_iterator q;

    public:
      typedef pair<char_type, state_type> value_type;

      const_iterator() 
      { }

      const_iterator(typename mapable_vector<char_type>::const_iterator i,
		     typename mapable_vector<state_type>::const_iterator j)
	: a(i), q(j)
      { }

      // end of range:
      const_iterator(typename mapable_vector<char_type>::const_iterator i)
	: a(i)
      { }
	
      bool operator==(const const_iterator &x) const {
	return (*a == (char_type) 0 && *x.a == (char_type) 0) // end
							      // of range
	  || (a == x.a && q == x.q);
      }

      pair<char_type, state_type> operator *() const {
	return make_pair(*a, *q);
      }

      const_iterator& operator ++() {
	++a; ++q;
	return *this;
      }

      const_iterator operator ++(int) {
	const_iterator tmp = (*this);
	++(*this);
	return tmp;
      }

      const_iterator& operator --() {
	--a; --q;
	return (*this);
      }
      const_iterator operator --(int) {
	const_iterator tmp = (*this);
	--(*this);
	return tmp;
      }
    };

    // Back to edges_type class

    key_compare key_comp() const { 
      key_compare tmp;
      return (tmp); 
    }

    value_compare value_comp() const { 
      return value_compare(key_comp()); 
    } 

    /*
    state_type operator[](key_type k) const
    {
      typename mapable_vector<char_type>::const_iterator i;
      for(i = a->begin() + q + 1;
	  *i != (char_type) 0 && *i != k; ++i);
      return *i == (char_type) 0 ? null_state : (*p)[i - a->begin()];
    }
    */

    const_iterator begin() const {
      return const_iterator(fa->letters.begin() + q, fa->aims_tags.begin() + q);
      //      return const_iterator(a->begin() + q + 1, p->begin() + q + 1);
    }

    const_iterator end() const {
      return const_iterator(fa->letters.end() - 1);
      //      return const_iterator(a->end() - 1);
    }

    /*
    size_type size() const {
      return find(a->begin() + q + 1, a->end(), 
		  (char_type) 0) - (a->begin() + q + 1);
    }
    */

    bool empty() const {
      return fa->letters[q] == (char_type) 0;
      //      return (*a)[q + 1] == (char_type) 0;
    }

    /*
    const_iterator find(const key_type &k) const
    {
      typename mapable_vector<char_type>::const_iterator i; 
      for(i = a->begin() + q + 1;
	  *i != (char_type) 0 && *i != k; ++i);
      return *i == (char_type) 0 ? 
	end() : const_iterator(i, p->begin() + (i - a->begin()));
    }

    size_type count(const key_type& k) const
    {
      const_iterator i = find(k);
      size_type c = 0;
      for(; i->first == k; ++i, ++c);
      return c;
    }
    */

    // assume transitions are sorted:
    const_iterator lower_bound(const key_type &k) const {
      const_iterator j;
      for(j = begin(); j != end() && (*j).first < k; ++j);
      return j;
    }

    // assume transitions are sorted:
    const_iterator upper_bound(const key_type &k) const {
      const_iterator j;
      for(j = begin(); j != end() && !(k < (*j).first); ++j);
      return j;
    }

    pair<const_iterator, const_iterator> equal_range(const key_type &k) const
    {
      return make_pair(lower_bound(k), upper_bound(k));
    }
  };

  // Back to FA class
protected:
  state_type initial_;
  set<state_type> I;
  unsigned int _state_count;
  unsigned int _trans_count;

  int fd;        // Memory mapping under UNIX
  set_F F;

public:
#ifndef _MSC_VER
  static const state_type null_state = 0;
#else
  static const state_type null_state;
#endif

  bool write(const string &path) {
    ofstream out(path.c_str(), ios::out | ios::binary);
    if (out && write(out)) {
      out.close();
      return true;
    }
    return false;
  }

  bool write(ostream &out) const // binary
  { 
    out.write((const char *) &initial_, sizeof(initial_));
    out.write((const char *) &_state_count, sizeof(_state_count));
    out.write((const char *) &_trans_count, sizeof(_trans_count));
    letters.write(out);
    aims_tags.write(out);
    tags.write(out);
    F.write(out);
    return (bool) out;
  }

  bool read(const string &path) {
    ifstream in(path.c_str(), ios::in | ios::binary);
    if (in && read(in)) {
      in.close();
      return true;
    }
    return false;
  }

  bool read(istream &in) // binary
  {
    in.read((char *)&initial_, sizeof(initial_));
    in.read((char *)&_state_count, sizeof(_state_count));
    in.read((char *)&_trans_count, sizeof(_trans_count));
    I.clear();
    I.insert(initial_);
    return letters.read(in) && aims_tags.read(in) && 
      tags.read(in) && F.read(in);
  }

#ifndef _MSC_VER
  bool mmap(const string &path)  // memory mapping
  {
    fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) return false;
    return mmap(fd);
  }

  bool mmap(int f)
  {
    ::read(f, (void*) &initial_, sizeof(initial_));
    ::read(f, (void*) &_state_count, sizeof(_state_count));
    ::read(f, (void*) &_trans_count, sizeof(_trans_count));
    I.clear();
    I.insert(initial_);
    return letters.mmap(f) && aims_tags.mmap(f) 
      && tags.mmap(f) && F.mmap(f);
  }

  void munmap()
  {
    initial_ = null_state;
    I.clear();
    letters.munmap();
    aims_tags.munmap();
    tags.munmap();
    F.munmap();
  }
#endif

  state_type initial() const { 
    return initial_; 
  }

  const set<state_type>& initials() const {
    return I;
  }

  set_F::const_reference final(state_type s) const {
    return F[s];
  }

  state_type delta1(state_type s, char_type l) const
  {
    typename mapable_vector<char_type>::const_iterator a 
      = letters.begin() + s + 1;
    for(; *a != (char_type) 0 && *a != l; ++a);
    return *a == (char_type) 0 ? 
      null_state : aims_tags[a - letters.begin()];
  }

  template <typename OutputIterator>
  OutputIterator delta1(state_type q, char_type c, OutputIterator x) const
  {
    typename mapable_vector<char_type>::const_iterator a 
      = letters.begin() + q + 1;
    for(; *a != (char_type) 0 && *a != c; ++a);
    for(; *a != (char_type) 0 && *a == c; ++a, ++x)
      *x = aims_tags[a - letters.begin()];
    return x;
  }

  unsigned int state_count() const { 
    return _state_count; 
  }

  unsigned int trans_count() const { 
    return _trans_count; 
  }
  
  const tag_type& tag(state_type s) const { 
    return tags[aims_tags[s]]; 
  }

  // beware if tags are compressed:
  tag_type& tag(state_type s) { 
    return tags[aims_tags[s]]; 
  }

  edges_type delta2(state_type s) const { 
    return edges_type(s, *this);
    //    return edges_type(s, letters, aims_tags); 
  }

  const_iterator begin() const
  {
    return state_count() > 0 ?
      const_iterator((state_type) 1, letters) : end();
  }

  const_iterator end() const {
    return const_iterator(letters.size() - 1, letters);
  }

//   void del_trans(state_type q, char_type c) {
//      mapable_vector<char_type>::iterator last
//        = letters.begin() + q + 1;
//      for(; *last != (char_type) 0 && *last != c; ++last); // find
//      mapable_vector<char_type>::iterator first = last, middle = last;
//      for(; *last != (char_type) 0; ++last) // count
//        if (*last == c) middle = last + 1;
//      std::rotate(first, middle, last + 1);
//      std::rotate(aims_tags.begin() + (first - letters.begin()),
// 		 aims_tags.begin() + (middle - letters.begin()),
// 		 aims_tags.begin() + (last - letters.begin()) + 1);
//   }

  vector<char_type> del_trans(state_type q, state_type p) {
     typename mapable_vector<char_type>::iterator last
       = letters.begin() + q + 1;
     typename mapable_vector<char_type>::iterator first = last;
     for(; *last != (char_type) 0; ++last);
     vector<char_type> v;
     v.reserve(128);
     for(++last; *first != '\0';) {
       if (aims_tags[first - letters.begin()] == p) {
	 v.push_back(*first);
	 std::rotate(aims_tags.begin() + (first - letters.begin()), 
		     aims_tags.begin() + (first - letters.begin()) + 1, 
		     aims_tags.begin() + (last - letters.begin()) - 1);
	 // the tag position is pointed to by last
	 std::rotate(first, first + 1, last);
       }
       else ++first;
     }
     return v;
  }

  ~FA_compress() {
#ifndef _MSC_VER
    letters.munmap();
    aims_tags.munmap();
    tags.munmap();
    F.munmap();
    if (fd > 0) close(fd);
#endif
  }
  
  FA_compress()
    : initial_(null_state), fd(0) { 
    I.insert(initial_);
  }
  
  FA_compress(istream &in) 
    : initial_(null_state), fd(0) {
    read(in);
  }
  
  void clear() {
    letters.clear();
    aims_tags.clear();
    tags.clear();
    F.clear();
    fd = 0;
    initial_ = null_state;
    I.clear();
    I.insert(initial_);
  }

  template <typename BFirstCursor>
  void compress(BFirstCursor i, bool tag_compression = true) {
    compress(i, BFirstCursor(), tag_compression);
  }

  template <typename BFirstCursor>
  void compress(BFirstCursor i, BFirstCursor j, bool tag_compression = true) {
    clear();
    if (i == j) return;
    tags.reserve(8192);
    letters.reserve(8192);
    letters.push_back((char_type) 0);
    aims_tags.reserve(8192);
    aims_tags.push_back(0);
    F.reserve(8192);
    F.push_back(false);
#ifndef _MSC_VER
    typedef typename BFirstCursor::state_type bstate_type;
#else
    typedef BFirstCursor::state_type bstate_type;
#endif
    map<bstate_type, safe<state_type> > old2new;
    vector<bstate_type> new2old;
    new2old.push_back(bstate_type());
    bstate_type ii = i.src();
#ifndef _MSC_VER
    map<bstate_type, pair<typename BFirstCursor::tag_type, bool> > sinks;
#else
    map<bstate_type, pair<BFirstCursor::tag_type, bool> > sinks;
#endif

    while (i != j) {
      safe<state_type> &q = old2new[i.src()];
      if (q == null_state) {
	sinks.erase(i.src());
	q = letters.size();
	letters.push_back((char_type) 0);
	new2old.push_back(bstate_type());
	F.push_back(i.src_final());
	typename mapable_vector<tag_type>::iterator k = tags.end();
	tag_type tmp_tag;
	tmp_tag = i.src_tag();
	if (tag_compression) 
	  k = find(tags.begin(), tags.end(), tmp_tag);

	if (k == tags.end()) {
	  tags.push_back(tmp_tag);
	  k = tags.end() - 1;
	}

	aims_tags.push_back(k - tags.begin());
	
	set<pair<char_type, bstate_type> > tmp;
	do {
	  tmp.insert(make_pair(i.letter(), i.aim()));
	  safe<state_type> &p = old2new[i.aim()]; 
	  if (p == null_state)
	    // may be a transition-less state:
	    sinks.insert(make_pair(i.aim(), make_pair(i.aim_tag(), 
						      i.aim_final())));
	}
	while (i.next());
	
	for(typename set<pair<char_type, bstate_type> >::const_iterator jj
	      = tmp.begin(); jj != tmp.end(); ++jj) {
	  letters.push_back(jj->first);
	  new2old.push_back(jj->second);
	  F.push_back(false);
	  aims_tags.push_back(0);
	}
      }
      else // q != 0, dejavu
	while (i.next());
    }

    for(unsigned int k = 1; k < new2old.size(); ++k)
      if (letters[k] != (char_type) 0) {
	safe<state_type> &p = old2new[new2old[k]];
	if (p == null_state) {
	  // trans pointing to a transition-less state, add it
	  p = letters.size();
	  letters.push_back((char_type) 0);
	  new2old.push_back(bstate_type());
#ifndef _MSC_VER
	  pair<typename BFirstCursor::tag_type, bool> pp = sinks[new2old[k]];
#else
	  pair<BFirstCursor::tag_type, bool> pp = sinks[new2old[k]];
#endif
	  F.push_back(pp.second);
	  typename mapable_vector<tag_type>::iterator k = tags.end();
	  tag_type tmp;
	  tmp = pp.first;
	  if (tag_compression) 
	    k = find(tags.begin(), tags.end(), tmp);
	  if (k == tags.end()) {
	    tags.push_back(tmp);
	    k = tags.end() - 1;
	  }
	  aims_tags.push_back(k - tags.begin());
	}
	aims_tags[k] = p;
      }
	
    letters.push_back((char_type) 0);
    I.clear();
//      for(typename set<typename ForwardCursor::state_type>::const_iterator i 
//  	  = init.begin(); i != init.end(); ++i)
//        I.insert(old2new[*i]);
//      if (I.size() == 1) 
//        initial_ = *I.begin();
//      else
//        initial_ = null_state;
    initial_ = old2new[ii];
    I.insert(initial_);
    _state_count = old2new.size();
    _trans_count = letters.size() - _state_count - 2;
  }
};

#ifndef _MSC_VER
template <typename T, typename U>
const typename FA_compress<T, U>::state_type 
FA_compress<T, U>::null_state;
#else
template <typename T, typename U>
const typename FA_compress<T, U>::state_type 
FA_compress<T, U>::null_state = 0;
#endif

} // namespace astl

#endif   // ASTL_DFA_COMPACT_H

