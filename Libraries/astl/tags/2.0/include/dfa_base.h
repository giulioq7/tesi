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

#ifndef ASTL_DFA_BASE_H
#define ASTL_DFA_BASE_H

// DFA_matrix, DFA_bin, DFA_mtf, DFA_tr, DFA_map derive from this base class
// This class manages state structures allocation/deallocation


#include <tools.h>
#include <state.h>
#include <vector>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

using std::vector;

namespace astl {
  
  /**
     Base class for ASTL deterministic automaton containers. It takes
     care of common functionalities dealing with states allocation/deallocation.
  */
  
  template <class CharTraits, class TagType, class StateData_, class StateType = unsigned int>
  class DFA_base : public DFA_concept
  {
  public:
    typedef CharTraits                     char_traits; /**< @chartraits */
    typedef TagType                        tag_type;    /**< @tagtype */
    typedef typename CharTraits::char_type char_type;   /**< @chartype */
    typedef StateType                      state_type;  /**< @statetype */

  private:
      friend class boost::serialization::access;
      template<class Archive>
      void serialize(Archive & ar, const unsigned int version)
      {
          ar & Q;
          ar & I;
          ar & F;
          ar & state_count_;
          ar & trans_count_;
      }

  protected:
    typedef vector<char> set_F;
    typedef state_data<tag_type, StateData_> StateData;

    vector<StateData*> Q;
    state_type         I;
    set_F              F;
    unsigned long      state_count_;
    unsigned long      trans_count_;

  public:
    typedef skip_blanks_iterator<StateData> const_iterator;


    static const state_type null_state;

    /** @return a forward iterator on the beginning of the sequence of
	the automaton states of type @c state_type arranged in an
	undefined order (probably in allocation order but that is not a
	requirement).
    */
    const_iterator begin() const {
      const_iterator x(&Q, 0);
      return ++x;
    }

    /** @return a past-the-end forward iterator on the sequence of
	the automaton states.
    */
    const_iterator  end() const { 
      return const_iterator(&Q, Q.size()); 
    }

    /** Sets the initial state of the automaton to @c s.
	@pre @c s has been returned by a previous call to @c new_state()
	@post @code initial() == s @endcode
    */
    void initial(state_type s) { 
      I = s; 
    }

    /** @return the initial state of the automaton. */
    state_type initial() const { 
      return I; 
    }

    /** @return @c true if @c s is a final state. 
	@pre @c s has been returned by a previous call to @c new_state()
    */
    bool final(state_type s) const { 
      return F[s] != '\0'; 
    }
  
    /** @return a reference to a boolean that, if assigned @c true sets
	the state @c s to be a final state and if assigned @c false sets @c
	s to be a non-final state. 
	@pre @c s has been returned by a previous call to @c new_state()
    */
    char& final(state_type s) { 
      return F[s]; 
    }

    /** Allocates a new state structure. The newly created state is
	non-final and has no transitions. The tag is
	default-constructed.
	@return the id of the freshly allocated state
    */
    state_type new_state() {
      Q.push_back(new StateData);
      F.push_back('\0');
      ++state_count_;
      return Q.size() - 1;
    }

    template <class OutputIterator>
    OutputIterator new_state(unsigned long how_many, OutputIterator x)
    {
      for(; how_many > 0; --how_many)
	*x++ = new_state();
      return x;
    }
      
    /** Destroyes the state @c s. The tag and the transitions are
	destroyed.
	@pre @c s has been returned by a previous call to @c new_state()
    */
    void del_state(state_type s) 
    {
      if (s == initial()) initial(null_state);
      trans_count_ -= Q[s]->edges().size();
      delete Q[s];
      Q[s] = NULL;
      --state_count_;
    }

    /** Makes the state @c to an exact copy of the state @c from. The tag
	and the transitions of @c to are destroyed and the new ones are
	copy-constructed from those of @c from.
	@pre @c from has been returned by a previous call to @c new_state()
	@pre @c to has been returned by a previous call to @c new_state()
    */
    void copy_state(state_type from, state_type to) {
      trans_count_ += Q[from]->edges().size() - Q[to]->edges().size();
      *Q[to] = *Q[from];
      F[to] = F[from];
    }

    /** Allocates an exact copy of the state @c q.
	@return the id of the freshly allocated state
	@pre @c q has been returned by a previous call to @c new_state()
    */
    state_type duplicate_state(state_type q)
    {
      Q.push_back(new StateData(*Q[q]));
      F.push_back(F[q]);
      ++state_count_;
      trans_count_ += Q[q]->edges().size();
      return Q.size() - 1;
    }

    /** @return the number of states in the automaton */
    unsigned long state_count() const { 
      return state_count_; 
    }
  
    /** @return the number of transitions in the automaton */
    unsigned long trans_count() const { 
      return trans_count_; 
    }

    /** @return the data attached to the state @c s
	@pre @c s has been returned by a previous call to @c new_state()
    */
    tag_type& tag(state_type s) { 
      return Q[s]->tag(); 
    }

    /** @return the data attached to the state @c s
	@pre @c s has been returned by a previous call to @c new_state()
    */
    const tag_type& tag(state_type s) const { 
      return Q[s]->tag(); 
    }

    DFA_base(unsigned long n) :
      Q(1, (StateData*) 0), I(0), F(1, '\0'), state_count_(0UL), 
      trans_count_(0UL) { 
      Q.reserve(n + 1);
    }

    ~DFA_base()
    {
      const_iterator start, finish = end();
      for(start = begin(); start != finish; ++start)
	del_state(*start);
    }
  };

  template <class CharTraits, class TagType, class StateData_, class StateType>
  const typename DFA_base<CharTraits, TagType, StateData_, StateType>::state_type 
  DFA_base<CharTraits, TagType, StateData_, StateType>::null_state = 0;

} // namespace astl

#endif  // ASTL_DFA_BASE_H
