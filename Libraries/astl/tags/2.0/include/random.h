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

#ifndef ASTL_RANDOM_H
#define ASTL_RANDOM_H

#include <astl.h>
#include <cmath>       // modf
#include <ctime>       // time

namespace astl {

// TODO:
// 1. Les méthodes find et exists ne sont pas implémentées (tournent en temps linéaire de
//    toute façon, beurk).
// 2. On ne peut pas itérer deux fois de suite sur les transitions d'un état.

// Generates double floating point numbers in the range [0,1].
// Based on Lary Hudson's public domain function frandom
class randomizer
{
protected:
  double seed_, state;
  static const double TEN_PI;
  static const double E;

public:
  randomizer(unsigned int i = time(0)) {
    seed(i);
  }

  unsigned int seed() const {
    return (unsigned int) seed_;
  }

  void seed(unsigned int i) {
    seed_ = i;
    state = seed_ * E; 
  }

  double operator()() {
    static double bogus;
    state = modf(state * TEN_PI + E, &bogus);
    return state;
  }
};

const double randomizer::TEN_PI = 31.41592653589793;
const double randomizer::E = 2.718281828459045;

template <typename EnumerableCharTraits>
class generator_base : public forward_cursor_concept
{
public:
  typedef generator_base self;
  typedef unsigned int   state_type;
  typedef empty_tag      tag_type;
  typedef typename EnumerableCharTraits::char_type char_type;
  typedef EnumerableCharTraits char_traits;

protected:
  mutable randomizer gen1, gen2;    // 1 for transitions, 2 for final states
  int graine;
  state_type q;
  pair<typename EnumerableCharTraits::const_iterator, state_type> t;
  unsigned int depth;
  double t_prob, f_prob;
	
public:
  generator_base(unsigned int height, double t_probability, 
		 double f_probability, int seed = time(0))
    : graine(seed), depth(height), t_prob(t_probability), 
      f_prob(f_probability) 
  { }
		
  bool src_final() const {
    gen2.seed(graine + q);
    return gen2() < f_prob;
  }

  bool aim_final() const {
    gen2.seed(graine + t.second);
    return gen2() < f_prob;
  }

  self& operator=(state_type p) {
    q = p;
  }

  state_type src() const {
    return q;
  }

  state_type aim() const {
    return t.second;
  }

  tag_type src_tag() const {
    return empty_tag();
  }

  tag_type aim_tag() const {
    return empty_tag();
  }

  char_type letter() const {
    return *t.first;
  }

  state_type sink_state() const {
    return state_type(0);
  }

  bool sink() const {
    return q == sink_state();
  }

  void forward() {
    q = t.second;
    --depth;
  }

  bool operator==(const self& x) const {
    return q == x.q && t == x.t;
  }
};

template <typename EnumerableCharTraits>
class dag_generator : public generator_base<EnumerableCharTraits>
{
public:
  typedef dag_generator self;
  typedef generator_base<EnumerableCharTraits> super;
  typedef typename super::state_type state_type;
  typedef typename super::tag_type   tag_type;
  typedef typename super::char_type  char_type;
  typedef typename super::char_traits char_traits;

public:
  dag_generator(unsigned int height, double t_probability, 
		double f_probability, int seed = time(0))
    : super(height, t_probability, f_probability, seed) {
    q = t.second = 1;
  }
		
  bool first() {
    if (depth == 0) return false;
    gen1.seed(graine + q);
    for(t.first = EnumerableCharTraits::begin(); 
	t.first != EnumerableCharTraits::end() && gen1() >= t_prob; ++t.first);
    if (t.first != EnumerableCharTraits::end()) {
      ++t.second;
      return true;
    }
    return false;
  }
			
  bool next() {
    for(++t.first; t.first != EnumerableCharTraits::end() 
	  && gen1() >= t_prob; ++t.first);
    if (t.first != EnumerableCharTraits::end()) {
      ++t.second;
      return true;
    }
    return false;
  }

protected:
  using super::q;
  using super::t;
  using super::depth;
  using super::gen1;
  using super::graine;
  using super::t_prob;
};

template <typename EnumerableCharTraits>
class tree_generator : public generator_base<EnumerableCharTraits>
{
public:
  typedef tree_generator self;
  typedef generator_base<EnumerableCharTraits> super;
  typedef typename super::state_type state_type;
  typedef typename super::tag_type   tag_type;
  typedef typename super::char_type  char_type;
  typedef typename super::char_traits char_traits;

protected:
  static state_type count;
	
public:
  tree_generator(unsigned int height, float t_probability, 
		 float f_probability, int seed = time(0))
    : super(height, t_probability, f_probability, seed) {
    q = ++count;
  }
		
  bool first() {
    if (depth == 0) return false;
    gen1.seed(graine + q);
    for(t.first = EnumerableCharTraits::begin(); 
	t.first != EnumerableCharTraits::end() && gen1() >= t_prob; ++t.first);
    if (t.first != EnumerableCharTraits::end()) {
      t.second = ++count;
      return true;
    }
    return false;
  }
			
  bool next() {
    if (depth == 0) return false;
    for(++t.first; t.first != EnumerableCharTraits::end() 
	  && gen1() >= t_prob; ++t.first);
    if (t.first != EnumerableCharTraits::end()) {
      t.second = ++count;
      return true;
    }
    return false;
  }

protected:
  using super::q;
  using super::t;
  using super::depth;
  using super::gen1;
  using super::graine;
  using super::t_prob;
};

template <typename EnumerableCharTraits>
typename tree_generator<EnumerableCharTraits>::state_type 
tree_generator<EnumerableCharTraits>::count = 0;

template <typename EnumerableCharTraits>
class cyclic_generator : public generator_base<EnumerableCharTraits>
{
public:
  typedef cyclic_generator self;
  typedef generator_base<EnumerableCharTraits> super;
  typedef typename super::state_type state_type;
  typedef typename super::tag_type   tag_type;
  typedef typename super::char_type  char_type;
  typedef typename super::char_traits char_traits;

  cyclic_generator(unsigned int height, float t_probability, 
		   float f_probability, int seed = time(0))
    : super(height, t_probability, f_probability, seed) {
    q = t.second = 1;
  }
		
  bool first() 
  {
    if (depth == 0) return false;
    gen1.seed(graine + q);
    for(t.first = EnumerableCharTraits::begin(); 
	t.first != EnumerableCharTraits::end() && gen1() >= t_prob; ++t.first);
    if (t.first != EnumerableCharTraits::end()) {
      t.second = ((state_type) (gen1() * (++t.second * 2))) + 1;
      return true;
    }
    return false;
  }
			
  bool next() 
  {
    if (depth == 0) return false;
    for(++t.first; t.first != EnumerableCharTraits::end() 
	  && gen1() >= t_prob; ++t.first);
    if (t.first != EnumerableCharTraits::end()) {
      t.second = ((state_type) (gen1() * (++t.second * 2))) + 1;
      return true;
    }
    return false;
  }

protected:
  using super::q;
  using super::t;
  using super::depth;
  using super::gen1;
  using super::graine;
  using super::t_prob;
};

} // namespace astl


#endif // GENERATOR_CURSOR_H
