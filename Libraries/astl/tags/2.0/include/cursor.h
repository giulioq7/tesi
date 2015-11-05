/*
 * ASTL - the Automaton Standard Template Library.
 * C++ generic components for Finite State Automata handling.
 * Copyright (C) 2000-2006 Vincent Le Maout (vincent.lemaout@chello.fr).
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

/** @file */

#ifndef ASTL_CURSOR_H
#define ASTL_CURSOR_H

#include <vector>
#include <functional>
#include <list>
#include <deque>
#include <queue>
#include <set>
#include <iostream>
#include <utility>

using namespace std;
#if !defined(__GNUG__) || __GNUG__ >= 3
using namespace rel_ops;
#endif

namespace astl {

/** A plain cursor is a pointer to an automaton state that is able to move along
defined transitions. Its purpose is to implement simple traversals
checking if a word is in the language defined by an automaton.
@tparams <tr><td>@c DFA</td><td>The type of the DFA that this cursor
will point to</td><td></td><td>@c DFA is a model of
#DFA</td></tr></table>
@modelof #cursor
@helpers #plainc()
*/

template <typename DFA>
class cursor : public cursor_concept
{
public:
  typedef cursor                    self;
  typedef cursor_concept            super;
  typedef typename DFA::state_type  state_type; /**< @statetype */
  typedef typename DFA::char_type   char_type;  /**< @chartype */
  typedef typename DFA::tag_type    tag_type;   /**< @tagtype */
  typedef typename DFA::char_traits char_traits; /**< @chartraits */
  typedef typename super::concept   concept;

  /** Creates a singular cursor */
  cursor() 
  { }

  /** Creates a cursor pointing to an undefined state of the automaton
      @c a */
  cursor(const DFA &a) 
    : dfa(&a)
  { }

  /** Creates a cursor pointing to state @c p of the automaton @c
      a. @post @code src() == p @endcode */
  cursor(const DFA &a, state_type p) 
    : dfa(&a), q(p)
  { }

  /** Returns the identifier of the state that this cursor points to */
  state_type src() const { return q; } 

  /** Sets this cursor to point to the state @c p. @post @code src() == p
      @endcode */
  self& operator=(state_type p) {
    q = p;
    return *this;
  }

  /** Returns @c true if @c x points to the same state as this cursor.
      @pre This cursor and @c x point to the same automaton */
  bool operator==(const self &x) const { return q == x.q; }

  /** Returns @c true if this cursor points to the sink state @c
      DFA::null_state */ 
  bool sink() const { return q == DFA::null_state; }

  /** Returns the identifier of the sink state @c DFA::null_state */
  state_type sink_state() const { return DFA::null_state; }

  /** Moves along the outgoing transition labeled with @c a if defined,
      otherwise moves to sink state and returns @c false */
  bool forward(const char_type &a) {
    q = dfa->delta1(q, a);
    return !sink();
  }

  /** Returns @c true if the state that this cursor points to is final */
  bool src_final() const { return dfa->final(q); }

  /** Returns the data attached to the state that this cursor points to */
  const tag_type& src_tag() const  { return dfa->tag(q); }

  /** Returns @c true if any outgoing transition labeled with @c a
      exists */
  bool exists(const char_type &a) const {
    return dfa->delta1(q, a) != DFA::null_state;
  }

protected:
  const DFA *dfa;
  state_type q;
};

/** A pointer to an automaton transition (@e source
    @e state, @e letter, @e aim @e state). It implements traversals of
    the sequence of transitions going out of the source state. 
    @tparams <tr><td>@c FA</td><td>The type of the automaton that this
    cursor will point to</td><td></td><td>@c FA is a model of
    #FA</td></tr></table>
    @modelof #transition_cursor
    @helpers #transitionc() */

template <typename FA>
class transition_cursor : public transition_cursor_concept
{
public:
  typedef transition_cursor         self;
  typedef transition_cursor_concept super;
  typedef typename FA::state_type  state_type; /**< @statetype */
  typedef typename FA::char_type   char_type;  /**< @chartype */
  typedef typename FA::tag_type    tag_type;   /**< @tagtype */
  typedef typename FA::char_traits char_traits; /**< @chartraits */
  typedef typename super::concept   concept;

  /** Creates a cursor pointing to the state @c p of the automaton @c
      a. 
      @post @code src() == p @endcode 
  */
  transition_cursor(const FA &a, state_type p)
    : fa(&a), q(p)
  { }

  /** Creates a cursor pointing to an undefined state of the automaton @c a */
  transition_cursor(const FA &a)
    : fa(&a)
  { }

  /** Creates a singular cursor */
  transition_cursor()
  { }

  /** Returns the id of the state that this cursor points to */
  state_type src() const { return q; }

  /** Sets this cursor to point to state @c p. 
      @post @code src() == p @endcode 
  */
  self& operator=(state_type p) {
    q = p;
    return *this;
  }

  /** Returns @c true if this cursor points to the sink state @c
      FA::null_state */ 
  bool sink() const { return q == FA::null_state; }

  /** Returns the identifier of the sink state @c FA::null_state */
  state_type sink_state() const { return FA::null_state;  }

  /** Returns @c true if the state this cursor points to is final */
  bool src_final() const { return fa->final(q);  }

  /** Returns the data attached to the state this cursor points to */
  const tag_type& src_tag() const { return fa->tag(q);  }

 /** Returns @c true if @c x points to the same transition as this cursor */
  bool operator==(const self &x) const {
    return (q == x.q && transition == x.transition);
  }

 /** Returns @c true if @c x points to another transition than this cursor */
  bool operator!=(const self &x) const { return !(*this == x); }

  /** Returns the letter on the pointed transition. @pre The cursor shall
      have been set to point to a defined transition beforehand by
      successfully calling #first or #next */
  char_type letter() const { return (*transition).first; }

  /** Makes this cursor point to the first element of the transitions
      sequence of the source state. Returns @c true if such an element
      exists (if any transition is defined), otherwise the pointed
      transition is undefined */
  bool first() {
    return !((transition = fa->delta2(q).begin()) == fa->delta2(q).end());
  }

  /** Moves this cursor to the next element of the transitions
      sequence of the source state. Returns @c true if such an element
      exists (the cursor is not at the end of the sequence), otherwise the
      pointed transition is undefined. @pre The cursor shall
      have been set to point to a defined transition beforehand by
      successfully calling #first or #next */ 
  bool next() { return !(++transition == fa->delta2(q).end()); }

  /** Moves forward along the pointed transition. @pre The cursor shall
      have been set to point to a defined transition beforehand by
      successfully calling #first or #next */
  void forward() { q = (*transition).second; }

  /** Returns the aim state of the pointed transition. @pre The cursor shall
      have been set to point to a defined transition beforehand by
      successfully calling #first or #next */
  state_type aim() const { return (*transition).second; }

  /** Returns @c true if the aim state of the transition that this cursor
      points to is final */ 
  bool aim_final() const { return fa->final((*transition).second); }

  /** Returns the data attached to the aim state of the pointed
      transition */
  const tag_type& aim_tag() const { return fa->tag((*transition).second); }

  // Not a standard requirement:
  bool operator<(const self &x) const {
    return q < x.q || (q == x.q && transition < x.transition);
  }

protected:
  const FA *fa;
  state_type q;
  typename FA::edges_type::const_iterator transition;
};

/** A pointer to a deterministic-automaton transition (@e source @e
    state, @e letter, @e aim @e state). It provides all the functionnalities
    of the plain #cursor and the #transition_cursor 
    @tparams <tr><td>@c DFA</td><td>The type of the automaton that this cursor
    will point to</td><td></td><td>@c DFA is a model of #DFA</td></tr></table>
    @modelof #cursor, #transition_cursor, #forward_cursor
    @helpers #forwardc() */

template <typename DFA>
class forward_cursor
  : public transition_cursor<DFA>, public forward_cursor_concept
{
public:
  typedef forward_cursor<DFA>         self;
  typedef transition_cursor<DFA>      super;
  typedef typename DFA::state_type  state_type; /**< @statetype */
  typedef typename DFA::char_type   char_type;  /**< @chartype */
  typedef typename DFA::tag_type    tag_type;   /**< @tagtype */
  typedef typename DFA::char_traits char_traits; /**< @chartraits */
  typedef forward_cursor_concept      concept;

  /** Creates a cursor pointing to a state @c p of the automaton @c a */
  forward_cursor(const DFA &a, state_type p)
    : super(a, p)
  { }

  /** Creates a cursor pointing to an undefined state of the
      automaton @c a */
  forward_cursor(const DFA &a)
    : super(a)
  { }

  /** Creates a singular cursor */
  forward_cursor()
    : super()
  { }

  /** Sets this cursor to point to the state @c p. 
      @post @code src() == p @endcode */
  self& operator=(state_type p) {
    super::operator=(p);
    return *this;
  }

  /** Moves along the outgoing transition labeled with @c.
      @return @c true if such a transition is defined,
      otherwise move to sink state and return @c false */  
  bool forward(const char_type &a) {
    q = fa->delta1(q, a);
    return !this->sink();
  }

  /** Moves forward along the pointed transition. 
      @pre The cursor shall have been set to point to a defined
      transition beforehand by successfully calling #first, #next or #find */
  void forward() { q = (*transition).second; }

  /** Returns @c true if an outgoing transition labeled with @c a
      exists */
  bool exists(const char_type &a) const {
    return fa->delta1(q, a) != DFA::null_state;
  }

  /** Makes this cursor point to the transition labeled with @c
      a. 
      @return @c true if such a transition exists, otherwise return @c
      false and the pointed transition is undefined */
  bool find(const char_type &a)  {
    return !((transition = fa->delta2(q).find(a)) == fa->delta2(q).end());
  }

protected:
  typedef typename DFA::edges_type::const_iterator edges_iterator;
  using super::q;
  using super::fa;
  using super::transition;
};

/** A #forward_cursor storing its path in a stack
of cursors. Each forward move along a transition pushes a new #forward_cursor
onto the stack top and an extra method #backward() allows
to pop. The depth-first traversal cursor #dfirst_cursor relies on the
#stack_cursor 
@tparams <tr><td>@c ForwardCursor</td><td>The type of the cursors that are
stored on the stack</td><td></td><td>@c ForwardCursor is a model of forward
cursor</td></tr>
<tr><td>@c StackContainer</td><td>The type of the sequential container
managing the stack</td><td>@c vector<ForwardCursor></td><td>@c StackContainer is
a model of back insertion sequence and @c StackContainer::value_type
is @c ForwardCursor</td></tr></table>
@modelof cursor, transition cursor, forward cursor, stack cursor
@helpers #stackc()
*/

template <typename ForwardCursor, typename StackContainer = vector<ForwardCursor> >
class stack_cursor
  : public StackContainer, public stack_cursor_concept
{
  // the interface publicly inherited from stack is not a standard requirement
public:
  typedef stack_cursor                        self;
  typedef StackContainer                      super;
  typedef typename ForwardCursor::state_type  state_type; /**< @statetype */
  typedef typename ForwardCursor::char_type   char_type;  /**< @chartype */
  typedef typename ForwardCursor::tag_type    tag_type;   /**< @tagtype */
  typedef typename ForwardCursor::char_traits char_traits; /**< @chartraits */
  typedef stack_cursor_concept                concept;

  /** Creates a cursor with a stack containing @c x */
  stack_cursor(const ForwardCursor &x)
    : super() {
    this->push_back(x);
  }

  /** Creates a cursor with an empty stack */
  stack_cursor()  // Empty stack
    : super()
  { }

  /** Returns the identifier of the state that this cursor points
      to. @pre The stack is not empty. @pre The stack is not empty.
      */ 
  state_type src() const { return super::back().src(); }

  /** Returns the aim state of the pointed transition. @pre The stack
  is not empty. @pre The cursor shall
  have been set to point to a defined transition beforehand by
  successfully calling #first, #next or #find */
  state_type aim() const { return super::back().aim(); }

  /** Returns the letter on the pointed transition. @pre The stack
  is not empty. @pre The cursor shall have been set to point to a
  defined transition beforehand by successfully calling #first, #next
  or #find */ 
  char_type letter() const { return super::back().letter(); }

  /** Returns @c true if this cursor points to the sink state. @pre
      The stack is not empty  */
  bool sink() const { return super::back().sink(); }

  /** Returns the sink state identifier. @pre The stack
  is not empty. */
  state_type sink_state() const { return super::back().sink_state(); }

  /** Returns @c true if an outgoing transition labeled with @c a
      exists. @pre The stack is not empty */
  bool exists(const char_type &a) const { return super::back().exists(a); }

  /** Makes this cursor point to the transition labeled with @c
  a. Returns @c true if such a transition exists, otherwise the
  pointed transition is undefined. @pre The stack is not empty */
  bool find(const char_type &a) { return super::back().find(a); }

  /** Makes this cursor point to the first element of the transitions
  sequence of the source state. Returns @c true if such an element
  exists (if any transition is defined), otherwise the pointed
  transition is undefined. @pre The stack is not empty */
  bool first() { return super::back().first(); }

  /** Moves this cursor to the next element of the transitions
  sequence of the source state. Returns @c true if such an element
  exists (the cursor is not at the end of the sequence), otherwise the
  pointed transition is undefined. @pre The stack
  is not empty. @pre The cursor shall
  have been set to point to a defined transition beforehand by
  successfully calling #first, #next or #find */ 
  bool next() { return super::back().next(); }

  /** Moves forward along the pointed transition and the resulting
  cursor is pushed onto the stack. @pre The stack
  is not empty. @pre The cursor shall
  have been set to point to a defined transition beforehand by
  successfully calling #first, #next or #find */
  void forward() {
    this->push_back(super::back());
    super::back().forward();
  }

  /** Moves along the outgoing transition labeled with @c a if defined,
      otherwise moves to sink state and returns @c false. The resulting
      cursor is pushed onto the stack. @pre The stack is not empty  */  
  bool forward(const char_type &a) {
    this->push_back(super::back());
    return super::back().forward(a);
  }

  /** Pops the stack top and return @c false if the resulting stack is
      empty. @pre The stack is not empty */
  bool backward() {
    super::pop_back();
    return !super::empty();
  }

  /** Returns @c true if the aim state of the transition that this
      cursor points to is final. @pre The stack is not empty. */ 
  bool aim_final() const { return super::back().aim_final(); }

  /** Returns @c true if the state that this cursor points to is
      final. @pre The stack is not empty. */
  bool src_final() const { return super::back().src_final(); }

  /** Returns the data attached to the state that this cursor points
      to. @pre The stack is not empty. */
  tag_type src_tag() const { return super::back().src_tag(); }

  /** Returns the data attached to the aim state of the pointed
      transition. @pre The stack is not empty. */
  tag_type aim_tag() const { return super::back().aim_tag(); }

  /** Returns the stack top. @pre The stack is not empty */
  const ForwardCursor& top() const { return super::back(); }
};

struct none
{
  template <class T>
  bool operator()(const T&) const {
    return false;
  }
};

/** Implements the depth-first traversal on deterministic automata. It
is in 
some sense an iterator on a sequence of transitions ordered according
to the depth-first traversal algorithm. The method #forward
allows to increment the cursor, making it point to the next transition
in the sequence. This methods returns @c true if the transition
reached has been pushed onto the stack (forward move) and @c false
otherwise (pop and backward move). 

The #dfirst_cursor is fundamental because it is used much in the
same way as the iterators on sequence to define ranges for
algorithms. 
@tparams <tr><td>@c StackCursor</td><td>The type of the stack
cursor</td><td></td><td>@c StackCursor is a model of stack
cursor</td></tr> 
         <tr><td>@c MarkerFunction</td><td>The type of state
         marker used for preventing the cursor from visiting twice
         the same transition on cyclic automata and DAGs</td><td>@c
         none</td><td>@c MarkerFunction is either @c none, @c
         set_marker<ForwardCursor::state_type> or a model 
         of state marker</td></tr></table>   
@modelof #dfirst_cursor
@helpers #dfirstc(), #dfirst_markc() */

template <typename StackCursor, typename MarkerFunction = none>
class dfirst_cursor : public dfirst_cursor_concept
{
protected:
  StackCursor c;
  bool has_poped;
  MarkerFunction visited;

public:
  typedef dfirst_cursor                     self;
  typedef dfirst_cursor_concept             super;
  typedef typename StackCursor::state_type  state_type; /**< @statetype */
  typedef typename StackCursor::char_type   char_type;  /**< @chartype */
  typedef typename StackCursor::tag_type    tag_type;   /**< @tagtype */
  typedef typename StackCursor::char_traits char_traits; /**< @chartraits */
  typedef typename super::concept           concept;

  /** Creates a cursor with @c x as stack */
  dfirst_cursor(const StackCursor &x,
		const MarkerFunction &f = MarkerFunction())
    : c(x), has_poped(false), visited(f) {
    visited(c.src());   // set c source to 'visited'
  }

  /** Creates a cursor with an empty stack used as an end-of-range
      iterator */
  dfirst_cursor()
    : c(), has_poped(false)
  { }

  /** Increments the cursor making it point to the next transition in
  the sequence. Returns @c true if the transition reached has been
  pushed on the stack. @pre The stack is not empty */
  bool forward() {
    if (has_poped) { 
      // move on to the next outgoing transition if any otherwise pop
      if (!c.next()) 
	return !c.backward();  // at edges end
      else {
	has_poped = false;
	return true;
      }
    }

    c.forward();
    if (visited(c.src()) || !c.first()) {
      c.backward(); // cannot forward so backward => pop
      has_poped = true;
      return false;
    }
    return true;
  }

  /** Forces the cursor to act as if there was no more transitions to
      push */
  void backward() { has_poped = true; } // force pop

  /** Returns the letter on the pointed transition. @pre The stack is
      not empty */
  char_type letter() const { return c.letter(); }

  /** Returns @c true if the aim state of the transition that this
      cursor points to is final. @pre The stack is
      not empty */ 
  bool aim_final() const { return c.aim_final(); }

  /** Returns @c true if the state that this cursor points to is
      final. @pre The stack is not empty */ 
  bool src_final() const { return c.src_final(); }

  /** Returns the identifier of the state that this cursor points
      to. @pre The stack is not empty */ 
  state_type src() const { return c.src(); }

  /** Returns the aim state of the pointed transition. @pre The stack is
      not empty */
  state_type aim() const { return c.aim(); }

  /** Returns @c true if both stacks are equal */
  bool operator==(const self &x) const { return c == x.c; }

  /** Returns @c true if stacks are different */
  bool operator!=(const self &x) const { return !(*this == x); }

  /** Returns the data attached to the state that this cursor points
      to. @pre The stack is not empty */ 
  tag_type src_tag() const { return c.src_tag(); }

  /** Returns the data attached to the aim state of the pointed
      transition. @pre The stack is not empty */
  tag_type aim_tag() const { return c.aim_tag(); }

  /** Returns the stack container */
  const StackCursor& stack() const { return c; }
};

template <class state_type, class LessThan = less<state_type> >
struct set_marker : unary_function<state_type, bool>
{
  set<state_type, LessThan> pool;
  set_marker()
  { }
  bool operator() (const state_type &q) {
    return !(pool.insert(q).second);  // insert returns pair<iterator,bool>
  }
};

/** Constructs a plain #cursor that points to a specified state of a
    deterministic automaton.
    @param a A deterministic automaton 
    @param q A state of the automaton @c a
    @return a plain #cursor pointing to the state @c q of the automaton
    @c a */
template <typename DFA>
inline
cursor<DFA> plainc(const DFA &a, typename DFA::state_type q) {
  return cursor<DFA>(a, q);
}

/** Constructs a plain #cursor that points to the initial state of a
    deterministic automaton.
    @param a A deterministic automaton 
    @return a plain #cursor pointing to @c a.initial()
*/
template <typename DFA>
inline
cursor<DFA> plainc(const DFA &a) {
  return cursor<DFA>(a, a.initial());
}

/** Constructs a #transition_cursor that points to a specified state of an
    automaton.
    @param a An automaton 
    @param q A state of the automaton @c a
    @return a #transition_cursor pointing to the state @c q of the automaton
    @c a */
template <typename FA>
inline
transition_cursor<FA> transitionc(const FA &a, typename FA::state_type q) {
  return transition_cursor<FA>(a, q);
}

/** Constructs a #transition_cursor that points to the initial state
    of an automaton.
    @param a An automaton 
    @return a #transition_cursor pointing to @c a.initial() */
template <typename FA>
inline
transition_cursor<FA> transitionc(const FA &a) {
  return transition_cursor<FA>(a, a.initial());
}

/** Constructs a #forward_cursor that points to a specified state of a
    deterministic automaton.
    @param a A deterministic automaton 
    @param q A state of the automaton @c a
    @return a #forward_cursor pointing to the state @c q of the automaton
    @c a */
template <typename DFA>
inline
forward_cursor<DFA> forwardc(const DFA &a, typename DFA::state_type q) {
  return forward_cursor<DFA>(a, q);
}

/** Constructs a #forward_cursor that points to the initial state of a
    deterministic automaton.
    @param a A deterministic automaton 
    @return a #forward_cursor pointing to @c a.initial() */
template <typename DFA>
inline
forward_cursor<DFA> forwardc(const DFA &a) {
  return forward_cursor<DFA>(a, a.initial());
}

/** Constructs a #stack_cursor containing a specified forward cursor.
    @param c A forward cursor
    @return a #stack_cursor whose stack contains @c c
*/
template <typename ForwardCursor>
inline
stack_cursor<ForwardCursor> stackc(const ForwardCursor &c) {
  return stack_cursor<ForwardCursor>(c);
}

// default behavior (catch-all):
template <typename Model, typename Concept>
struct dfirst_cursor_type
{
  // Trick: this template does not define a public 'model'
  // type. Instanciation of this template will occur if no
  // specialization exists and will lead to a compiler error
  // which means that Model is not either a FA, DFA, forward_cursor,
  // transition_cursor nor a stack_cursor and therefore cannot be used
  // to instanciate the function template dfirstc
};

template <class FA>
struct dfirst_cursor_type<FA, FA_concept>
{
  typedef dfirst_cursor<stack_cursor<transition_cursor<FA> > > model;
};

template <class DFA>
struct dfirst_cursor_type<DFA, DFA_concept>
{
  typedef dfirst_cursor<stack_cursor<forward_cursor<DFA> > > model;
};

template <class ForwardCursor>
struct dfirst_cursor_type<ForwardCursor, forward_cursor_concept>
{
  typedef dfirst_cursor<stack_cursor<ForwardCursor> > model;
};

template <class TransitionCursor>
struct dfirst_cursor_type<TransitionCursor, transition_cursor_concept>
{
  typedef dfirst_cursor<stack_cursor<TransitionCursor> > model;
};

template <class StackCursor>
struct dfirst_cursor_type<StackCursor, stack_cursor_concept>
{
  typedef dfirst_cursor<StackCursor> model;
};

/** Constructs a #dfirst_cursor from either an automaton, a forward
    cursor or a stack cursor.
    @param x An automaton, a forward cursor or a stack cursor
    @return A #dfirst_cursor pointing to the first outgoing transition
    of @c x.initial() if @c x is an automaton or a #dfirst_cursor
    initialized with the provided forward/stack cursor
    @pre @c x is not a cyclic automaton nor a cursor on a cyclic
    structure */
template <typename T>
inline
typename dfirst_cursor_type<T, typename T::concept>::model
dfirstc(const T &x)
{
  return dfirstc_(x, x);
}

// Builds a depth first cursor from a DFA pointing to the first
// outgoing transition of the initial state if any by constructing the
// underlying forward and stack cursors. If the DFA has no initial
// state or if it has no outgoing transitions, returns an empty cursor
template <typename DFA>
typename dfirst_cursor_type<DFA, DFA_concept>::model
dfirstc_(const DFA &x, DFA_concept)
{
  forward_cursor<DFA> fc(x, x.initial());
  if (!fc.sink() && fc.first())
    return dfirst_cursor<stack_cursor<forward_cursor<DFA> > >(stackc(fc));
  else
    return dfirst_cursor<stack_cursor<forward_cursor<DFA> > >();
}

template <typename FA>
typename dfirst_cursor_type<FA, FA_concept>::model
dfirstc_(const FA &x, FA_concept)
{
  transition_cursor<FA> fc(x, x.initial());
  if (!fc.sink() && fc.first())
    return dfirst_cursor<stack_cursor<transition_cursor<FA> > >(stackc(fc));
  else
    return dfirst_cursor<stack_cursor<transition_cursor<FA> > >();
}

// Builds a depth first cursor from a forward cursor by constructing the
// underlying stack cursor:
template <class ForwardCursor>
typename dfirst_cursor_type<ForwardCursor, forward_cursor_concept>::model
dfirstc_(const ForwardCursor &x, forward_cursor_concept)
{
  if (!x.sink()) {
    stack_cursor<ForwardCursor> s(x);
    if (s.first())
      return dfirst_cursor<stack_cursor<ForwardCursor> >(s);
  }
  return dfirst_cursor<stack_cursor<ForwardCursor> >();
}

// Builds a depth first cursor from a transition cursor by constructing the
// underlying stack cursor:
template <class TransitionCursor>
typename dfirst_cursor_type<TransitionCursor, transition_cursor_concept>::model
dfirstc_(const TransitionCursor &x, transition_cursor_concept)
{
  if (!x.sink()) {
    stack_cursor<TransitionCursor> s(x);
    if (s.first())
      return dfirst_cursor<stack_cursor<TransitionCursor> >(s);
  }
  return dfirst_cursor<stack_cursor<TransitionCursor> >();
}

// Builds a depth first cursor from a stack cursor:
template <class StackCursor>
typename dfirst_cursor_type<StackCursor, stack_cursor_concept>::model
dfirstc_(const StackCursor &x, stack_cursor_concept)
{
  if (!x.sink()) {
    StackCursor s(x);
    if (s.first()) return dfirst_cursor<StackCursor>(s);
  }
  return dfirst_cursor<StackCursor>();
}

// Depth first mark cursor helper functions:
template <typename Model, typename Concept, typename MarkerFunction>
struct dfirst_mark_cursor_type
{ };

template <typename DFA, typename MarkerFunction>
struct dfirst_mark_cursor_type<DFA, DFA_concept, MarkerFunction>
{
  typedef dfirst_cursor<stack_cursor<forward_cursor<DFA> >,
			MarkerFunction> model;
};

template <typename FA, typename MarkerFunction>
struct dfirst_mark_cursor_type<FA, FA_concept, MarkerFunction>
{
  typedef dfirst_cursor<stack_cursor<transition_cursor<FA> >,
			MarkerFunction> model;
};

template <typename TransitionCursor, typename MarkerFunction>
struct dfirst_mark_cursor_type<TransitionCursor, transition_cursor_concept,
			       MarkerFunction>
{
  typedef dfirst_cursor<stack_cursor<TransitionCursor>, MarkerFunction> model;
};

template <typename ForwardCursor, typename MarkerFunction>
struct dfirst_mark_cursor_type<ForwardCursor, forward_cursor_concept,
			       MarkerFunction>
{
  typedef dfirst_cursor<stack_cursor<ForwardCursor>, MarkerFunction> model;
};

/** Construct a #dfirst_cursor from either a cyclic automaton, a forward
    or stack cursor on cyclic structures. This cursor makes use of the
    default state marker to guarantee that no transition is visited more than
    twice: once in "descending" stage (pushing transitions on the
    stack) and once in "ascending" stage (poping), therby avoiding
    infinite loops.
    @param x An automaton, a forward cursor or a stack cursor
    @return A #dfirst_cursor pointing to the first transition of
    @c x.initial() if @c x is an automaton or a #dfirst_cursor
    initialized with the provided forward/stack cursor */

template <typename T>
inline
typename dfirst_mark_cursor_type<T, typename T::concept,
				 set_marker<typename T::state_type> >::model
dfirst_markc(const T &x)
{
  return dfirst_markc_(x, x, set_marker<typename T::state_type>());
}

template <typename T, typename MarkerFunction>
inline
typename dfirst_mark_cursor_type<T, typename T::concept, MarkerFunction>::model
dfirst_markc(const T &x, const MarkerFunction& m)
{
  return dfirst_markc_(x, x, m);
}

// Build a depth first cursor from a DFA pointing to the first
// outgoing transition of the initial state (if any) by constructing
// the underlying forward and stack cursors. If the DFA has no initial
// state or if it has no outgoing transition, return an empty cursor
template <typename DFA, typename MarkerFunction>
typename dfirst_mark_cursor_type<DFA, DFA_concept, MarkerFunction>::model
dfirst_markc_(const DFA &x, DFA_concept, const MarkerFunction& m)
{
  forward_cursor<DFA> fc(x, x.initial());
  if (!fc.sink() && fc.first())
    return dfirst_cursor<stack_cursor<forward_cursor<DFA> >,
      MarkerFunction>(stackc(fc), m);
  else
    return dfirst_cursor<stack_cursor<forward_cursor<DFA> >,
      MarkerFunction>();
}

template <typename FA, typename MarkerFunction>
typename dfirst_mark_cursor_type<FA, FA_concept, MarkerFunction>::model
dfirst_markc_(const FA &x, FA_concept, const MarkerFunction& m)
{
  transition_cursor<FA> fc(x, x.initial());
  if (!fc.sink() && fc.first())
    return dfirst_cursor<stack_cursor<transition_cursor<FA> >,
      MarkerFunction>(stackc(fc), m);
  else
    return dfirst_cursor<stack_cursor<transition_cursor<FA> >,
      MarkerFunction>();
}

// Builds a depth first cursor from a forward cursor by constructing
// the underlying stack cursor:
template <typename ForwardCursor, typename MarkerFunction>
typename dfirst_mark_cursor_type<ForwardCursor, forward_cursor_concept,
				 MarkerFunction>::model
dfirst_markc_(const ForwardCursor &x, forward_cursor_concept,
	      const MarkerFunction& m)
{
  if (!x.sink()) {
    stack_cursor<ForwardCursor> s(x);
    if (s.first())
      return dfirst_cursor<stack_cursor<ForwardCursor>, MarkerFunction>(s, m);
  }
  return dfirst_cursor<stack_cursor<ForwardCursor>, MarkerFunction>();
}

// Builds a depth first cursor from a transition cursor by
// constructing the underlying stack cursor:
template <typename TransitionCursor, typename MarkerFunction>
typename dfirst_mark_cursor_type<TransitionCursor, transition_cursor_concept,
				 MarkerFunction>::model
dfirst_markc_(const TransitionCursor &x, transition_cursor_concept,
	      const MarkerFunction& m)
{
  if (!x.sink()) {
    stack_cursor<TransitionCursor> s(x);
    if (s.first())
      return dfirst_cursor<stack_cursor<TransitionCursor>,
	MarkerFunction>(s, m);
  }
  return dfirst_cursor<stack_cursor<TransitionCursor>, MarkerFunction>();
}

/** A #forward_cursor storing its path in a queue
of cursors. Each move through the sequence of the outgoing transitions
of the source state (#next()) enqueues a forward cursor. An extra
method #dequeue() allows to dequeue and to implement the breadth-first
traversal.
@tparams <tr><td>@c ForwardCursor</td><td>The type of the cursors that are
stored in the queue</td><td></td><td>@c ForwardCursor is a model of forward
cursor</td></tr>
<tr><td>@c QueueContainer</td><td>The type of the sequential container
managing the queue</td><td>@c deque<ForwardCursor></td><td>@c QueueContainer is
a model of front and back insertion sequence and @c QueueContainer::value_type
is @c ForwardCursor</td></tr></table>
@modelof cursor, transition cursor, forward cursor, queue cursor
@helpers #queuec() */

template <typename ForwardCursor, typename QueueContainer = deque<ForwardCursor> >
class queue_cursor
  : public QueueContainer, public queue_cursor_concept
{
public:
  typedef queue_cursor                        self;
  typedef QueueContainer                      super;
  typedef typename ForwardCursor::state_type  state_type; /**< @statetype */
  typedef typename ForwardCursor::char_type   char_type;  /**< @chartype */
  typedef typename ForwardCursor::tag_type    tag_type;   /**< @tagtype */
  typedef typename ForwardCursor::char_traits char_traits; /**< @chartraits */

  /** Creates a cursor with a queue containing @c x */
  queue_cursor(const ForwardCursor &x)
    : super() {
    super::push_back(x);
  }

  /** Creates a cursor with an empty queue */
  queue_cursor()
    : super()
  { }

  /** Returns the identifier of the state that this cursor points
      to. @pre The queue is not empty  */
  state_type src() const { return super::back().src(); }

  /** Returns the aim state of the pointed transition. @pre The queue
  is not empty. @pre The cursor shall have been set to point to a
  defined transition beforehand by successfully calling #first, #next
  or #find */ 
  state_type aim() const { return super::back().aim(); }

  /** Returns @c true if the state that this cursor points to is
  final. @pre The queue is not empty */
  bool src_final() const { return super::back().src_final();  }

  /** Returns @c true if the aim state of the transition that this
      cursor points to is final. @pre The queue is not empty */ 
  bool aim_final() const { return super::back().aim_final();  }

  /** Returns the letter on the pointed transition. . @pre The queue
  is not empty. @pre The cursor shall have been set to point to a
  defined transition beforehand by successfully calling #first, #next
  or #find */ 
  char_type letter() const { return super::back().letter();  }

  /** Returns @c true if this cursor points to the sink state. @pre
      The queue is not empty  */
  bool sink() const { return super::back().sink();  }

  state_type sink_state() const { return super::back().sink_state(); }

  /** Returns the sink state identifier */
  bool find(const char_type &a) { return super::back().find(a); }

  /** Makes this cursor point to the first element of the transitions
  sequence of the source state and enqueues the resulting forward
  cursor. Returns @c true if such an element exists (if any transition
  is defined), otherwise the pointed transition is undefined. @pre The
  queue is not empty */ 
  bool first() { return super::back().first(); }

  /** Moves this cursor to the next element of the transitions
  sequence of the source state and enqueues the resulting
  cursor. Returns @c true if such an element exists (the cursor is not
  at the end of the sequence), otherwise dequeues a forward cursor and
  returns @c false. @pre The queue is not empty. @pre The cursor shall
  have been set to point to a defined transition beforehand by
  successfully calling #first, #next or #find */ 
  bool next() { 
    super::push_back(super::back());
    return super::back().next();
  }

  /** Moves forward along the pointed transition. @pre The queue is
  not empty. @pre The cursor shall have been set to point to a defined
  transition beforehand by successfully calling #first, #next or #find
  */  
  void forward() { super::back().forward(); }

  /** Moves along the outgoing transition labeled with @c a if defined,
      otherwise moves to sink state and returns @c false. @pre The
      queue is not empty */   
  bool forward(const char_type &a) { return super::back().forward(a); }

  /** Returns @c true if an outgoing transition labeled with @c a
      exists. @pre The queue is not empty */
  bool exists(const char_type &a) const { return super::back().exists(a); }

  /** Dequeues a forward cursor and return @c false if the resulting
      queue is empty. @pre The queue is not empty */ 
  bool dequeue() {
    super::back() = super::front();
    super::pop_back();
    return !super::empty();
  }

  /** Returns the data attached to the state that this cursor points
      to. @pre The queue is not empty. */
  tag_type src_tag() const { return super::back().src_tag(); }

  /** Returns the data attached to the aim state of the pointed
      transition. @pre The queue is not empty. */
  tag_type aim_tag() const { return super::back().aim_tag(); }
};


/** A #bfirst_cursor implements the breadth-first traversal on 
deterministic automata. It is an iterator on a sequence of transitions
ordered according to the breadth-first traversal algorithm. The method
#next() allows to increment the cursor, making it point to the next
transition in the sequence. This methods returns @c true if the
transition reached has been enqueued and @c false otherwise (dequeue).
The bfirst_cursor is used in the same way as the iterators on sequence
to define ranges foralgorithms. 
@tparams <tr><td>@c QueueCursor</td><td>The type of the queue
cursor</td><td></td><td>@c QueueCursor is a model of queue
cursor</td></tr> 
         <tr><td>@c MarkerFunction</td><td>The type of state
         marker used for preventing the cursor from visiting twice
         the same transition on cyclic automata and DAGs</td><td>@c
         none</td><td>@c MarkerFunction is either @c none, @c
         set_marker<ForwardCursor::state_type> or a model 
         of state marker</td></tr></table>   
@modelof #bfirst_cursor
@helpers #bfirstc(), #bfirst_markc() */

template <typename QueueCursor, typename MarkerFunction = none>
class bfirst_cursor : public bfirst_cursor_concept
{
protected:
  QueueCursor c;
  MarkerFunction visited;

public:
  typedef bfirst_cursor                     self;
  typedef typename QueueCursor::state_type  state_type; /**< @statetype */
  typedef typename QueueCursor::char_type   char_type;  /**< @chartype */
  typedef typename QueueCursor::tag_type    tag_type;   /**< @tagtype */
  typedef typename QueueCursor::char_traits char_traits; /**< @chartraits */
  typedef bfirst_cursor_concept             concept;

  /** Creates a cursor with @c x as queue */
  bfirst_cursor(const QueueCursor &x,
		const MarkerFunction &f = MarkerFunction())
    : c(x), visited(f) {
    visited(c.src());
  }

  /** Creates a cursor with an empty queue used as an end-of-range
      iterator */
  bfirst_cursor()
    : c()
  { }

  /** Increments the cursor making it point to the next transition in
  the sequence. Returns @c true if the transition reached has been
  enqueued. @pre The queue is not empty */
  bool next() {
    if (!c.next()) {
      while (c.dequeue()) {
	c.forward();
	if (!visited(c.src()) && c.first()) break;
      }
      return false;
    }
    return true;
  }

  /** Returns the identifier of the state that this cursor points
      to. @pre The queue is not empty */ 
  state_type src() const { return c.src(); }

  /** Returns the aim state of the pointed transition. @pre The queue is
      not empty */
  state_type aim() const { return c.aim(); }

  /** Returns @c true if the state that this cursor points to is
      final. @pre The queue is not empty */ 
  bool src_final() const { return c.src_final(); }

  /** Returns @c true if the aim state of the transition that this
      cursor points to is final. @pre The queue is
      not empty */ 
  bool aim_final() const { return c.aim_final(); }

  /** Returns the letter on the pointed transition. @pre The queue is
      not empty */
  char_type letter() const { return c.letter(); }

  /** Returns @c true if both queues are equal compared element by
      element. */
  bool operator==(const self &x) const { return c == x.c; }

  /** Returns @c true if queues are different compared element by
      element. */
  bool operator!=(const self &x) const { return !(*this == x); }

  /** Returns the data attached to the state that this cursor points
      to. @pre The queue is not empty */ 
  tag_type src_tag() const { return c.src_tag(); }

  /** Returns the data attached to the aim state of the pointed
      transition. @pre The queue is not empty */
  tag_type aim_tag() const { return c.aim_tag(); }
};

/** Constructs a #queue_cursor from a #forward_cursor
    @param x A forward cursor
    @return A #queue_cursor containing @c x */
template <class ForwardCursor>
queue_cursor<ForwardCursor> queuec(const ForwardCursor &x) {
  return queue_cursor<ForwardCursor>(x);
}

template <class Model, class Concept>
struct bfirst_cursor_type
{ };

template <class DFA>
struct bfirst_cursor_type<DFA, DFA_concept>
{
  typedef bfirst_cursor<queue_cursor<forward_cursor<DFA> > > model;
};

template <class TransitionCursor>
struct bfirst_cursor_type<TransitionCursor, transition_cursor_concept>
{
  typedef bfirst_cursor<queue_cursor<TransitionCursor> > model;
};

template <class ForwardCursor>
struct bfirst_cursor_type<ForwardCursor, forward_cursor_concept>
{
  typedef bfirst_cursor<queue_cursor<ForwardCursor> > model;
};

template <class QueueCursor>
struct bfirst_cursor_type<QueueCursor, queue_cursor_concept>
{
  typedef bfirst_cursor<QueueCursor> model;
};

/** Constructs a #bfirst_cursor from either an automaton, a forward
    cursor or a queue cursor.
    @param x An automaton, a forward cursor or a stack cursor
    @return A #bfirst_cursor pointing to the first outgoing transition
    of @c x.initial() if @c x is an automaton or a #bfirst_cursor
    initialized with the provided forward/queue cursor
    @pre @c x is not a cyclic automaton nor a cursor on a cyclic
    structure */
template <typename T>
inline
typename bfirst_cursor_type<T, typename T::concept>::model
bfirstc(const T &x) {
  return _bfirstc(x, x);
}

// Builds a breadth-first cursor from a DFA pointing to the first
// outgoing transition of the initial state if any by constructing the
// underlying forward and stack cursors. If the DFA has no initial
// state or if it has no outgoing transitions, returns an empty cursor
template <class DFA>
inline
typename bfirst_cursor_type<DFA, DFA_concept>::model
_bfirstc(const DFA &x, DFA_concept)
{
  forward_cursor<DFA> fc(x, x.initial());
  if (!fc.sink() && fc.first())
    return bfirst_cursor<queue_cursor<forward_cursor<DFA> > >(queuec(fc));
  else
    return bfirst_cursor<queue_cursor<forward_cursor<DFA> > >();
}

// Builds a breadth-first cursor from a transition cursor by constructing
// the underlying stack cursor:
template <class TransitionCursor>
inline
typename bfirst_cursor_type<TransitionCursor, transition_cursor_concept>::model
_bfirstc(const TransitionCursor &x, transition_cursor_concept)
{
  if (!x.sink()) {
    queue_cursor<TransitionCursor> s(x);
    if (s.first())
      return bfirst_cursor<queue_cursor<TransitionCursor> >(s);
  }
  return bfirst_cursor<queue_cursor<TransitionCursor> >();
}

// Builds a breadth-first cursor from a forward cursor by constructing
// the underlying stack cursor:
template <class ForwardCursor>
inline
typename bfirst_cursor_type<ForwardCursor, forward_cursor_concept>::model
_bfirstc(const ForwardCursor &x, forward_cursor_concept)
{
  if (!x.sink()) {
    queue_cursor<ForwardCursor> s(x);
    if (s.first())
      return bfirst_cursor<queue_cursor<ForwardCursor> >(s);
  }
  return bfirst_cursor<queue_cursor<ForwardCursor> >();
}

// Builds a breadth-first cursor from a stack cursor:
template <class QueueCursor>
inline
typename bfirst_cursor_type<QueueCursor, queue_cursor_concept>::model
_bfirstc(const QueueCursor &x, queue_cursor_concept)
{
  if (!x.sink()) {
    QueueCursor s(x);
    if (s.first())
      return bfirst_cursor<QueueCursor>(s);
  }
  return bfirst_cursor<QueueCursor>();
}

template <class Model, class Concept, typename MarkerFunction>
struct bfirst_mark_cursor_type
{ };

template <class DFA, typename MarkerFunction>
struct bfirst_mark_cursor_type<DFA, DFA_concept, MarkerFunction>
{
  typedef bfirst_cursor<queue_cursor<forward_cursor<DFA> >,
			MarkerFunction> model;
};

template <class ForwardCursor, typename MarkerFunction>
struct bfirst_mark_cursor_type<ForwardCursor, forward_cursor_concept,
			       MarkerFunction>
{
  typedef bfirst_cursor<queue_cursor<ForwardCursor>, MarkerFunction > model;
};

template <class QueueCursor, typename MarkerFunction>
struct bfirst_mark_cursor_type<QueueCursor, queue_cursor_concept,
			       MarkerFunction>
{
  typedef bfirst_cursor<QueueCursor, MarkerFunction> model;
};

/** Construct a #bfirst_cursor from either a cyclic automaton, a forward
    or queue cursor on cyclic structures. This cursor makes use of the
    default state marker to guarantee that no transition is visited more than
    once, therby avoiding infinite loops.
    @param x An automaton, a forward cursor or a queue cursor
    @return A #bfirst_cursor pointing to the first transition of
    @c x.initial() if @c x is an automaton or a #bfirst_cursor
    initialized with the provided forward/queue cursor */

template <class T>
inline
typename bfirst_mark_cursor_type<T, typename T::concept,
				 set_marker<typename T::state_type> >::model
bfirst_markc(const T &x) {
  return bfirst_markc_(x, x, set_marker<typename T::state_type>());
}

template <class T, typename MarkerFunction>
inline
typename bfirst_mark_cursor_type<T, typename T::concept,
				 MarkerFunction>::model
bfirst_markc(const T &x, const MarkerFunction& m) {
  return bfirst_markc_(x, x, m);
}

// Builds a breadth-first mark cursor from a DFA pointing to the first
// outgoing transition of the initial state if any by constructing the
// underlying forward and stack cursors. If the DFA has no initial
// state or if it has no outgoing transitions, returns an empty cursor
template <class DFA, typename MarkerFunction>
inline
typename bfirst_mark_cursor_type<DFA, DFA_concept, MarkerFunction>::model
bfirst_markc_(const DFA &x, DFA_concept, const MarkerFunction& m)
{
  forward_cursor<DFA> fc(x, x.initial());
  if (!fc.sink() && fc.first())
    return bfirst_cursor<queue_cursor<forward_cursor<DFA> >,
      MarkerFunction >(queuec(fc),m);
  else
    return bfirst_cursor<queue_cursor<forward_cursor<DFA> >,
      MarkerFunction >();
}

// Builds a breadth-first mark cursor from a forward cursor by constructing
// the underlying stack cursor:
template <class ForwardCursor, typename MarkerFunction>
inline
typename bfirst_mark_cursor_type<ForwardCursor, forward_cursor_concept,
				 MarkerFunction>::model
bfirst_markc_(const ForwardCursor &x, forward_cursor_concept,
	      const MarkerFunction& m)
{
  if (!x.sink()) {
    queue_cursor<ForwardCursor> s(x);
    if (s.first())
      return bfirst_cursor<queue_cursor<ForwardCursor>,
	MarkerFunction>(s, m);
  }
  return bfirst_cursor<queue_cursor<ForwardCursor>, MarkerFunction>();
}

// Builds a breadth-first cursor from a stack cursor:
template <class QueueCursor, typename MarkerFunction>
inline
typename bfirst_mark_cursor_type<QueueCursor, queue_cursor_concept,
				 MarkerFunction>::model
bfirst_markc_(const QueueCursor &x, queue_cursor_concept,
	      const MarkerFunction& m)
{
  if (!x.sink()) {
    QueueCursor s(x);
    if (s.first())
      return bfirst_cursor<QueueCursor, MarkerFunction>(s, m);
  }
  return bfirst_cursor<QueueCursor, MarkerFunction>();
}

/** Tries to move the cursor @c c along the path defined by the range
    <tt> [first, last) </tt>.
    @param c A plain cursor
    @param i A start-of-range input iterator
    @param j An end-of-range input iterator
    @return @c true if for all @c i in <tt> [first, last)
    </tt> @c c.forward(*i) returns @c true 
    @see #match(), #first_match(), #longest_match() */
template <typename Cursor, typename InputIterator>
inline
bool forward(Cursor &c, InputIterator i, InputIterator j) {
  for(; !(i == j) && c.forward(*i); ++i);
  return i == j;
}

} // namespace astl

#endif  // ASTL_CURSOR_H















