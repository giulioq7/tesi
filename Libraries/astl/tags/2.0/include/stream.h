/*
 * ASTL - the Automaton Standard Template Library.
 * C++ generic components for Finite State Automata handling.
 * Copyright (C) 2000-2005 Vincent Le Maout (vincent.lemaout@chello.fr).
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

#ifndef ASTL_STREAM_H
#define ASTL_STREAM_H

#include <astl.h>
#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <utility>

using namespace std;

#if (__GNUG__ && __GNUG__ > 2)
using namespace rel_ops;
#endif

namespace astl {

template <typename Tag = empty>
class DFA_stream
{
public:
  typedef Tag        tag_type;
  typedef int        state_type;
  typedef state_type State;
  struct bool_reference;
  friend struct bool_reference;

protected:
  ostream &out;
  set<state_type> F;
  state_type count;
  map<state_type, tag_type> T;

public:
  static const state_type null_state = 0;

  DFA_stream(ostream &output)
    : out(output), count(0)
  { }

  state_type new_state() { return ++count;  }

  tag_type& tag(state_type q) { return tag_(q, tag_type()); }

  struct bool_reference
  {
    state_type q;
    DFA_stream &dfa;

    bool_reference(state_type p, DFA_stream &a)
      : q(p), dfa(a)
    { }

    bool_reference& operator=(bool b) {
      if (b == true) dfa.F.insert(q);
      else dfa.F.erase(q);
      return *this;
    }
  };

  bool_reference final(state_type q) {
    return bool_reference(q, *this);
  }

  template <typename Alphabet>
  void set_trans(state_type q, const Alphabet &a, state_type p) {
    write_state(q);
    out	<< '\t' << a << '\t';
    write_state(p);
    write_tag(q);
    write_tag(p);
    out << endl;
  }

  void set_trans(state_type q, const char& a, state_type p) {
    set_trans(q, (int) a, p);
  }

  void set_trans(state_type q, const unsigned char& a, state_type p) {
    set_trans(q, (int) a, p);
  }

  void set_trans(state_type q, const string& a, state_type p) {
    write_state(q);
    out	<< "\t\"" << a << "\"\t";
    write_state(p);
    write_tag(q);
    write_tag(p);
    out << endl;
  }

protected:
  template <typename TagType>
  tag_type& tag_(state_type q, const TagType&) {
    return T[q];
  }

  tag_type& tag_(state_type, const empty&) {
    static tag_type bogus;
    return bogus;
  }

  void write_state(state_type q) {
    // final states have negative ID:
    out << (F.find(q) != F.end() ? -q : q);
  }

  void write_tag(state_type q) {
    write_tag(q, tag_type());
  }
  
  template <typename TagType>
  void write_tag(state_type q, const TagType&)
  { 
    // output tag if q is in the tag mapping:
    typename map<state_type, tag_type>::iterator i = T.find(q);
    if (i != T.end()) {
      out << '\t' << i->second;
      T.erase(i);
    }    
  }

  void write_tag(state_type q, const string&)
  { 
    // output tag if q is in the tag mapping:
    typename map<state_type, tag_type>::iterator i = T.find(q);
    if (i != T.end()) {
      out << "\t\"" << i->second << '"';
      T.erase(i);
    }    
  }

  // nothing to do with empty tags:
  void write_tag(state_type, const empty&)
  { }
};

template <typename Tag>
const typename DFA_stream<Tag>::state_type DFA_stream<Tag>::null_state;

template <typename DFirstC>
inline
ostream& dump(ostream &out, DFirstC first, DFirstC last = DFirstC())
{
  DFA_stream<> output(out); // empty tag
  clone(output, first);
  return out;
}

template <typename DFirstCursor>
inline
ostream& full_dump(ostream &out, DFirstCursor first, DFirstCursor last = DFirstCursor())
{
  DFA_stream<typename DFirstCursor::tag_type> output(out);
  clone(output, first);
  return out;
}

template <typename CharType>
struct read_char : public binary_function<istream, CharType, void>
{
  void operator()(istream &in, CharType& x) const {
    in >> x;
  }
};

template < >
struct read_char<unsigned char> 
  : public binary_function<istream, unsigned char, void>
{
  void operator()(istream &in, unsigned char& x) const {
    int tmp;
    in >> tmp;
    x = (unsigned char) tmp;
  }
};

template < >
struct read_char<char> 
  : public binary_function<istream, char, void>
{
  void operator()(istream &in, char& x) const {
    int tmp;
    in >> tmp;
    x = (char) tmp;
  }
};

template < >
struct read_char<string> 
  : public binary_function<istream, string, void>
{
  void operator()(istream &in, string& x) const {
    x.erase(x.begin(), x.end());   // VC++ does not implement clear()
    char c;
    for(c = '\0'; c != '"'; in.get(c));
    for(in.get(c); c != '"'; in.get(c))
      x += c;
  }
};

template <typename FA>
class istream_cursor : public dfirst_cursor_concept
{
public:
  typedef typename FA::char_traits        char_traits;
  typedef typename char_traits::char_type char_type;
  typedef int                             state_type;
  typedef typename FA::tag_type           tag_type;
  typedef istream_cursor<FA>              self;

  istream_cursor()               // end of range
    : in(cin)
  { }

  istream_cursor(istream &input) // start of range
    : in(input), has_poped(false) { 
    forward();
  }

  state_type src() const { return s.top().q;  }
  state_type aim() const { return s.top().p;  }

  bool operator==(const self &x) const { // test for the end of range
    return s == x.s;
  }

  bool operator!= (const self &x) const { return !(*this == x); }

  bool src_final() const { return s.top().q < 0;  }
  bool aim_final() const { return s.top().p < 0;  }

  char_type letter() const { return s.top().a; }

  bool forward() 
  {
    if (in.eof()) {
      if (!s.empty()) s.pop();
      return s.empty();
    }

    if (has_poped) {
      if (s.top().q == t.q) {
	s.top() = t;
	has_poped = false;
      }
      else 
	s.pop();
    }
    else {
      in >> t.q;
#if defined(__GNUG__) && __GNUG__ >= 3
      if (in.eof())
	return false;
#endif
      read_char<char_type>()(in, t.a);
      in >> t.p;
      if (s.empty() || s.top().p == t.q) {
	read_tag(t.q, tag_type());
	read_tag(t.p, tag_type());
	s.push(t);
      }
      else 
	has_poped = true;
    }
    return !has_poped;
  }

  tag_type src_tag() { return T[s.top().q]; }
  tag_type aim_tag() { return T[s.top().p]; }

protected:
  struct transition 
  {
    state_type q, p;
    char_type a;
    bool operator==(const transition &x) const {
      return q == x.q && p == x.p && a == x.a;
    }
  };

  istream &in;
  stack<transition> s;
  transition t;
  bool has_poped;
  mutable map<state_type, tag_type> T;

  template <typename TagType>
  void read_tag(state_type q, const TagType&) { 
    if (T.find(q) == T.end()) {
      tag_type tmp;
      read_char<tag_type>()(in, tmp);
      T.insert(make_pair(q, tmp));
    }
  }

  void read_tag(state_type, const empty&)
  { }
};

template <typename FA>
inline
istream_cursor<FA> istreamc(istream &in, const FA&)
{
  return istream_cursor<FA>(in);
}

template <typename DFA>
inline
void restore(DFA &a, istream &in)
{
  a.initial(clone(a, istreamc(in, a)));
}

/** algorithm
name dump

prototype $
template <typename DFirstC>
ostream& 
dump(ostream &out, DFirstC first, DFirstC last = DFirstC());
$

description $
Writes the automaton defined by the range \verb+[first, last)+
to the output stream \verb+out+ in an ASCII representation (letters
shall have an operator \verb+<<+ defined). 
Such a representation can be read through the algorithm \verb+restore+
or the input stream cursor \verb+istream_cursor+. This version
of the algorithm does not add states tags to the representation,
see \verb+full_dump+ for a complete one.
$

requirement \verb+DFirstC+ is a model of depth-first cursor.
requirement \verb+ostream& operator<<(ostream&, DFirstC::char_type&)+. 

precondition \verb+[first, last)+ is a valid range.

complexity $O(n\log{n})$ where $n$ is \verb+last - first+.

example $
#include <astl.h>
#include <stream.h>
#include <minimize.h>
#include <iostream>

int main()
{
  DFA_bin<plain, minimization_tag> A;
  // construct with words from stdin:
  add_words(A, istream_iterator<string>(std::cin), 
               istream_iterator<string>());
  // minimize:
  acyclic_minimization(A);
  // dump to stdout getting rid of minimization_tag:
  dump(std::cout, dfirst_markc(A));
}  
$

note Since this algorithm does not take in account states tags, it is a good way to get rid of tags which were added temporarily to the automaton in order to apply an algorithm.

seealso full\_dump, restore, istream\_cursor

*/

/** algorithm
name full_dump

prototype $
template <typename DFirstC>
ostream& 
full_dump(ostream &out, DFirstC first, DFirstC last = DFirstC());
$

description $
Writes the automaton defined by the range \verb+[first, last)+
to the output stream \verb+out+ in an ASCII representation (letters
shall have an operator \verb+<<+ defined). 
Such a representation can be read through the algorithm \verb+restore+
or the input stream cursor \verb+istream_cursor+. This version
of the algorithm adds states tags to the representation imposing
that an operator \verb+<<+ be defined for tags, 
see \verb+dump+ for a tagless representation.
$

requirement \verb+DFirstC+ is a model of depth-first cursor.
requirement \verb+ostream& operator<<(ostream&, DFirstC::char_type&)+. 
requirement \verb+ostream& operator<<(ostream&, DFirstC::tag_type&)+. 

precondition \verb+[first, last)+ is a valid range.

complexity $O(n\log{n})$ where $n$ is \verb+last - first+.

example $
#include <astl.h>
#include <stream.h>
#include <hash.h>
#include <iostream>

int main()
{
  DFA_map<plain, hash_tag> A;
  // construct with words from stdin:
  add_words(A, istream_iterator<string>(std::cin), 
               istream_iterator<string>());
  // turn A into a hashing automaton:
  make_hash(A);
  // dump to stdout with hashing tags:
  full_dump(std::cout, dfirst_markc(A));
}  
$

seealso dump, restore, istream\_cursor

*/

/** algorithm
name restore

prototype $
template <typename FA>
void restore(FA &a, istream &in);
$

description $
Reads the automaton ASCII representation from the input stream 
\verb+in+ and construct \verb+a+ accordingly. \verb+FA+ tags and
alphabet types shall match those used to generate the representation
(through \verb+dump+ or \verb+full_dump+). The first state defined
by the representation is set as the automaton initial state.
$

requirement \verb+FA+ is a model of FA.

precondition \verb+FA::tag_type+ and \verb+FA::char_type+ must match those used to write the ASCII representation.

complexity $O(n\log{n})$ where $n$ is the transition count of the input representation

example $
#include <astl.h>
#include <stream.h>
#include <hash.h>
#include <iostream>

int main()
{
  // reconstruct the hashing automaton of dull_dump example:
  DFA_map<plain, hash_tag> A;
  restore(A, std::cin);
}  
$

seealso dump, full\_dump, istream\_cursor

*/

} // namespace astl

#endif // ASTL_STREAM_ALGORITHMS
