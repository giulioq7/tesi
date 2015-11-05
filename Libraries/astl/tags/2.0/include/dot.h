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

#ifndef ASTL_DOT_H
#define ASTL_DOT_H

#include <astl.h>
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

namespace astl {

template <typename Tag = empty>
class DFA_dot
{
public:
  ostream &out;
  typedef unsigned long state_type;
  typedef Tag           tag_type;
  typedef state_type State;
  unsigned long Q, _state_fontsize, _edge_fontsize;
  double _ratio, _ranksep;
  string _rankdir, _size;
  bool _initial;
  mutable map<state_type, tag_type> T;

#ifndef _MSC_VER
  static const state_type null_state = 0;
#else
  static const state_type null_state;
#endif

  DFA_dot(ostream &output)
    : out(output), Q(0), _state_fontsize(40), _edge_fontsize(20),
      _ratio(.5), _ranksep(.4), _rankdir("TB"), _initial(true)
  { }
  
  void graph_attributes() {
    out << "ratio=" << _ratio << ";" << endl;
    out << "rankdir=" << _rankdir << ";" << endl;
    out << "ranksep=" << _ranksep << ";" << endl;
    if (!_size.empty())
      out << "size=\"" << _size << "\";" << endl;
  }

  DFA_dot& ratio(float x) {
    _ratio = x;
    return *this; 
  }

  DFA_dot& state_fontsize(unsigned long x) {
    _state_fontsize = x;
    return *this;
  }

  DFA_dot& fontsize(unsigned long x) {
    _state_fontsize = x;
    _edge_fontsize = x;
    return *this;
  }

  DFA_dot& edge_fontsize(unsigned long x) {
    _edge_fontsize = x;
    return *this;
  }

  DFA_dot& size(const string &s) {
    _size = s;
    return *this;
  }

  DFA_dot& rankdir(const string &s) {
    _rankdir = s;
    return *this;
  }

  DFA_dot& initial(bool b) {
    _initial = b;
    return *this;
  }

  DFA_dot& ranksep(float f) {
    _ranksep = f;
    return *this;
  }

  bool initial() const { return _initial; }

  unsigned long new_state() { return ++Q; }
  
  template <typename char_type>
  void set_trans(State q, const char_type &a, State p) {
    write_state(q); // possibly write tag
    out << " -> ";
    write_state(p);
    out << " [label=\"" << a << "\",fontsize=" 
	<< _edge_fontsize << "];" << endl;
  }

  tag_type& tag(state_type q) { return tag_(q, tag_type()); }

  void set_trans(State q, const char &a, State p) {
    write_state(q); // possibly write tag
    out << " -> ";
    write_state(p);
    out << " [label=\"";
    if (isgraph(a)) {
      if (a == '"' || a == '\\') out << '\\';
      out << a;
    }
    else out << "\\" << (int) a;
    out << "\",fontsize=" << _edge_fontsize << "];" << endl;
  }

  struct bool_reference
  {
    State q;
    DFA_dot &dfa;

    bool_reference(State p, DFA_dot &a)
      : q(p), dfa(a)
    { }

    bool_reference& operator=(bool b) {
      dfa.out << q;
      if (b == true) dfa.out  << " [shape=doublecircle";
      else dfa.out << " [shape=circle"; 	
      dfa.out << ",fontsize=" << dfa._state_fontsize;
      write_label(q, typename DFA_dot<Tag>::tag_type());
      dfa.out << "];" << endl;
      return *this;
    }
    
  protected:
    template <typename TagType>
    void write_label(state_type q, const TagType&) {
      typename map<state_type, tag_type>::iterator i = dfa.T.find(q);
      if (i != dfa.T.end()) {
	dfa.out << ",label=\"" << i->first << "\n" << i->second << '"';
	dfa.T.erase(i);
      }
    }
    
    void write_label(state_type, const empty&) 
    { }
  };

  bool_reference final(State q) {
    return bool_reference(q, *this);
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

  void write_state(state_type q) { write_state(q, tag_type()); }

  template <typename TagType>
  void write_state(state_type q, const TagType&) {
    typename map<state_type, tag_type>::iterator i = T.find(q);
    out << q;
    if (i != T.end()) {
      out << " [label=\"" << i->second << "\"];" << endl;
      T.erase(i);
    }
  }

  void write_state(state_type q, const empty&) { out << q; }
};

#ifndef _MSC_VER
template <typename Tag>
const typename DFA_dot<Tag>::state_type DFA_dot<Tag>::null_state;
#else
template <typename Tag>
const typename DFA_dot<Tag>::state_type DFA_dot<Tag>::null_state = 0;
#endif

// Output the graph with default configuration:
template <typename DFirstCursor>
void dot(ostream &out, DFirstCursor x, DFirstCursor y = DFirstCursor())
{
  DFA_dot<> output(out); // empty tag
  dot(output, x, y);
}

// Output the graph with default configuration:
template <typename DFirstCursor>
void full_dot(ostream &out, DFirstCursor x, DFirstCursor y = DFirstCursor())
{
  DFA_dot<typename DFirstCursor::tag_type> output(out);
  dot(output, x, y);
}

// Customized output representation through the DFA_dot object: 
template <typename DFirstCursor, typename Tag>
inline
void dot(DFA_dot<Tag> &output, 
	 DFirstCursor x, DFirstCursor y = DFirstCursor())
{
  output.out << "digraph G {" << endl;
  output.graph_attributes();
  typename DFA_dot<Tag>::state_type i = clone(output, x, y);
  if (output.initial() && i != output.null_state)
  {
    output.out << "node [shape = point, color=white, fontcolor=white]; start;" << endl;
    output.out << "start -> " << i << ";" << endl;
  }
  output.out << "}" << endl;
}

// template <typename DFirstCursor, typename Tag>
// void dot_(DFA_dot<typename DFirstCursor::tag_type> &output, 
// 	  DFirstCursor x, DFirstCursor y, const Tag&)
// {
//   output.out << "digraph G {" << endl;
//   output.graph_attributes();
//   typename DFA_dot<typename DFirstCursor::tag_type>::State i = clone(output, x, y);
//   if (output.initial() && i != output.null_state)
//     output.out << i << " [shape=doublecircle];" << endl;
//   output.out << "}" << endl;
// }

// template <typename DFirstCursor>
// void dot_(DFA_dot<typename DFirstCursor::tag_type> &output, 
// 	  DFirstCursor x, DFirstCursor y, const empty&)
// {
//   //  output.tags(false);
//   output.out << "digraph G {" << endl;
//   output.graph_attributes();
//   typename DFA_dot<typename DFirstCursor::tag_type>::State i = clone(output, x, y);
//   if (output.initial() && i != output.null_state)
//     output.out << i << " [shape=doublecircle];" << endl;
//   output.out << "}" << endl;
// }



} // namespace astl
    
#endif // ASTL_DOT_ALGORITHM


