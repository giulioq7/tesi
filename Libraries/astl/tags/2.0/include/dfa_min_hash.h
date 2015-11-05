#ifndef ASTL_DFA_MIN_HASH_H
#define ASTL_DFA_MIN_HASH_H

// Descrition:	
//   ASTL 2.0 Determinisic Finite Automaton Class Template DFA_min
//   Dynamic minimal acyclic DFA implementing a perfect hash function
//   from words (sequences of chars) to integers and from integers to
//   words.
//   This class has a limited interface: words can be added with the
//   insert method but no direct state/transition creations are
//   supported. Moreover, this container does not support tags.
//   Sorry, words removal is not implemented yet!

#include <astl.h>
#include <hash.h>
#include <match.h>
#include <iostream>
#include <set>
#include <vector>
#include <functional>
#include <string>
#include <cstring>    // memcpy
#include <bitset>
#include <iterator>
#include <utility>    // pair<>

using namespace std;

namespace astl {

/** A perfect hashing function from words (sequences of characters) to
    integers and from integers to words.
    The hashing is implemented with a dynamic minimal acyclic DFA and
    maps words to strictly positive integers in lexicographical
    order. This order is determined by the ordering defined on
    characters in the static method @c CharTraits::lt. Therefore,
    the mapping is not stable with respect to the insertion: a new
    word will be associated to its position in the
    lexicographically-sorted word list, thus shifting by an offset of
    1 all mappings beyond the insertion position. The set of words
    must then be fixed for this hashing function to be usable.

    This class does not define the standard DFA interface but a
    restricted one that allows only word insertions and read-only
    operations thus enforcing the invariants for the structure
    integrity to be guaranteed.
    
    @tparams <tr><td>@c CharTraits</td><td>Character traits describing
    the alphabet that the words are built with.</td><td>@c plain</td><td>@c
    CharTraits is a model of #Alphabet</td></tr></table>
*/

template <typename CharTraits = plain>
class DFA_min_hash : public DFA_min<CharTraits, hash_tag>
{
public:
  typedef DFA_min<CharTraits, hash_tag> super;
  typedef DFA_min_hash                  self;
  typedef typename super::state_type    state_type;
  typedef typename super::char_type     char_type;
  typedef typename super::char_traits   char_traits;

  /** Get the hash value for the word passed as a range of characters
      of type @c char_type.
      @return the integer associated with the word if found, 0
      otherwise.
  */
  template <typename InputI>
  int hash_value(InputI first, InputI last) const {
    return astl::hash_value(forwardc(*this), first, last);
  }
  
  /** Get the hash value for the word passed as a character string.
      @return the integer associated with the word if found, 0
      otherwise.
  */
  int hash_value(const basic_string<char_type> &s) const {
    return hash_value(s.begin(), s.end());
  }

  /** Get the word for the hash value @c w as a sequence of characters
      of type @c char_type written out through the output iterator @c out.
      @return the length of the output sequence.
  */
  template <typename OutputI>
  int value_hash(int w, OutputI out) const {
    return value_hash(forwardc(*this), w, out);
  }

  /** Get the word for the hash value @c w as a character string.
      @return the word that @c w is associated to.
  */
  basic_string<char_type> value_hash(int w) const {
    basic_string<char_type> r;
    r.reserve(this->height(super::initial()));
    return value_hash(w, back_inserter(r));
  }

  /** Add a word defined by a range on a sequence of characters of
      type @c char_type.
      @return @c true if the word has been actually added, @c false if
      the word already exists and that the operation left the
      automaton unchanged.
  */
  template <class ForwardI>
  bool insert(ForwardI, ForwardI);
  
  /** Add a word defined by a character string.
      @return @c true if the word has been actually added, @c false if
      the word already exists and that the operation left the
      automaton unchanged.
  */
  bool insert(const basic_string<char_type> &s) {
    return insert(s.begin(), s.end());
  }
};

  template <class CharTraits> 
  template <class ForwardI>
  bool DFA_min_hash<CharTraits>::insert(ForwardI first, ForwardI last)
  {
    {
      cursor<DFA_min_hash<CharTraits> > c(*this);
      if (match(c, first, last)) return false;  // word exists ?
    }
    if (!super::ready) super::prepare();  // build the structure if necessary
    stack_cursor<forward_cursor<self> > c(forwardc(*this));
    bool duplicating;

    for(duplicating = false; first != last; ++first) {
      if (!duplicating)
	this->pool(this->height(c.src())).erase(c.src());

      if (!c.find(*first)) {
	this->set_trans(c.src(), *first, super::new_state());
	duplicating = true;
	c.find(*first);
      }
      else 
	if (super::Q[c.aim()]->degree_in() > 1) {
	  this->change_trans(c.src(), *first, this->duplicate_state(c.aim())); 
	  duplicating = true;
	  c.find(*first);
	}
      c.forward();
    }

    if (!duplicating) 
      this->pool(this->height(c.src())).erase(c.src());

    if (!this->final(c.src())) {
      this->tag(c.src()).wc() += 1;
      this->final(c.src()) = true;
    }

    unsigned int h = this->height(c.src());

    for(; c.backward(); ) {
      // try to insert the current state in its height pool
      // if an equivalent state is found its ID is returned, otherwise
      // it is inserted 
      pair<typename super::container::iterator, bool> i = 
	this->pool(this->height(c.aim())).insert(c.aim());
      if (i.second == false) {  // found an equivalent state ?
	state_type tmp = c.aim(); 
	// redirect the transition towards equivalent state
	this->change_trans(c.src(), c.letter(), *i.first);
	this->tag(*i.first).wc() = this->tag(tmp).wc();
	this->del_state(tmp);
      }
      this->tag(c.src()).wc() += 1; // update hashing
      h = max(this->height(c.src()), h+1); //Precond: states height initialized with 0
      this->height(c.src(), h);
    }
    this->pool(this->height(super::initial())).insert(super::initial());
    ++super::wc_;
    return true;
  }

// This specialization of the ordering of states takes advantage of hash tags:
template <typename Transition>
inline
bool operator<(const state<Transition, hash_tag> &x, const state<Transition, hash_tag> &y) {
  if (x.tag().wc() < y.tag().wc()) return true;
  if (x.tag().wc() == y.tag().wc()) {
    if (x.final() < y.final()) return true;
    if (x.final() == y.final()) {
      if (x.size() < y.size()) return true;
      else
	if (x.size() == y.size()) { 
	  typename state<Transition, hash_tag>::const_iterator i = x.begin(), j = x.end(), k = y.begin();
	  while (i != j)
	    if (*i < *k) return true;
	    else if (*k < *i) return false;
	    else { ++i; ++k; }
	  return false; // equivalent, not lower than
	}
    }
  }
  return false;
}

// statistics:

// word count 2000000 (unsorted)
// states 1735092
// transitions 3470388
// ../bin/dfaminhash: 144.54 s CPU time

// word count 2000000 (sorted)
// states 1735092
// transitions 3470388
// ../bin/dfaminhash: 73.46 s CPU time

// word count 5000000 (sorted)
// states 3365677
// transitions 7328087
// ../bin/dfaminhash: 202.44 s CPU time

// word count 8000000 (sorted)
// states 4648267
// transitions 10575345
// ../bin/dfaminhash: 334.99 s CPU time

} // namespace astl

#endif

  
