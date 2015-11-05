#ifndef ASTL_DFA_MIN_H
#define ASTL_DFA_MIN_H

// Descrition:	
//   ASTL 2.0 Determinisic Finite Automaton Class Template DFA_min
//   Dynamic minimal acyclic DFA
//   This class has a limited interface: words can be added with the
//   insert method but no direct state/transition creations are
//   supported.
//   Sorry, words removal is not implemented yet!

#include <astl.h>
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

typedef bitset<32> bitset_type; 

// ** CharType shall be a POD type **
template <typename CharType>
class transition
{
protected:
  pair<CharType, unsigned int> letter_aim;

public:
  typedef CharType char_type;

  transition(CharType a = 0, unsigned int aim = 0)
    : letter_aim(a, aim)
  { }

  char_type letter() const { return letter_aim.first; }

  unsigned int aim() const { return letter_aim.second; }

  void aim(unsigned int new_aim) { letter_aim.second = new_aim; }

  bool operator<(const transition<char_type> &x) const {// lexicographical compare states
    return letter_aim < x.letter_aim;
  }

  const pair<char_type, unsigned int>& make_pair() const {
    return letter_aim;
  }
};

// Specialization for char alphabet
// 32 bits for a transition: 8 bits for the letter, 24 bits for the aim state
// Hence the following limitations: 256 characters max, about 16
// million states max

const bitset_type letter_mask(0xFF000000);
const bitset_type aim_mask(0x00FFFFFF);
const unsigned long LETTER_SHIFT = 24;

template <>
class transition<char>
{
protected:
  bitset_type data;  

public:
  typedef char char_type;

  transition()
    : data(0UL) 
  { }

  transition(char a, unsigned int aim = 0)
    : data((bitset_type(a) << LETTER_SHIFT) | bitset_type(aim))
  { }

  char letter() const {
    return (char) (((data & letter_mask) >> LETTER_SHIFT).to_ulong());
  }

  unsigned int aim() const { return (data & aim_mask).to_ulong(); }

  void aim(unsigned int new_aim) {
    data &= letter_mask;  
    data |= bitset_type(new_aim); 
  }

  bool operator<(const transition<char> &x) const {// lexicographical compare states
    return data.to_ulong() < x.data.to_ulong();
  }

  pair<char, unsigned int> make_pair() const { return std::make_pair(letter(), aim()); }
};

template <>
class transition<unsigned char>
{
protected:
  bitset_type data;  

public:
  typedef unsigned char char_type;

  transition()
    : data(0UL) 
  { }

  transition(unsigned char a, unsigned int aim = 0)
    : data((bitset_type(a) << LETTER_SHIFT) | bitset_type(aim))
  { }

  unsigned char letter() const {
    return (unsigned char) (((data & letter_mask) >> LETTER_SHIFT).to_ulong());
  }

  unsigned int aim() const { return (data & aim_mask).to_ulong(); }

  void aim(unsigned int new_aim) {
    data &= letter_mask;  
    data |= bitset_type(new_aim); 
  }

  bool operator<(const transition<unsigned char> &x) const {// lexicographical compare states
    return data.to_ulong() < x.data.to_ulong();
  }

  pair<unsigned char, unsigned int> make_pair() const { 
    return std::make_pair(letter(), aim()); 
  }
};

// This order relation is used to access transitions by binary search,
// it does not take in account the aim state:
template <typename Transition>
struct lower_transition : public binary_function<Transition, Transition, bool>
{
  bool operator()(const Transition &x, const Transition &y) const {
    return x.letter() < y.letter();
  }
};

// Implementation notes
// A state has the following attributes:
// - A in-degree (incoming transitions count)
// - A boolean (final or not)
// - A height (length of the longest word recognized by the state)
// - A out-degree (outgoing transitions count)
// 
// The in-degree and the final flag are stored in a 32 bits array: the
// first 31 bits are reserved for the in-degree. The height and
// out-degree are stored in a 32 bits array: the first 12 bits are
// reserved for the height and the last 20 bits for the out-degree.
// Hence the following limitations: word length max is 4096 
// and alphabet's max size is 1048576
// Optimization: when out-degree is 1, the outgoing transition is stored in the
// array pointer.

static const bitset_type   final_mask(1);
static const bitset_type   degree_in_mask(0xFFFFFFFF << 1);
static const bitset_type   card_mask(0x000FFFFF);
static const bitset_type   height_mask(0xFFF00000);
static const unsigned long DEGREE_IN_SHIFT = 1;
static const unsigned long HEIGHT_SHIFT = 20;

template <typename Transition, typename Tag>
class state_base
{
public:
  typedef Tag tag_type;
  tag_type& tag() { return data; }
  const tag_type& tag() const { return data; }

protected:
  tag_type data;
};

template <typename Transition>
class state_base<Transition, empty_tag> // empty object for optimizing
{
public:
  typedef empty_tag tag_type;
  empty_tag& tag() { return data; }
  const empty_tag& tag() const { return data; }

protected:
  static empty_tag data;
};

template <typename Transition>
empty_tag state_base<Transition, empty_tag>::data;

template <typename Transition, typename Tag = empty_tag>
class state  : public state_base<Transition, Tag>
{
public:
  class bool_reference;
  friend class bool_reference;

protected:
  bitset_type degree_in_final;
  bitset_type height_card; 
  union dummy_name {   // VC++ does not support methods in unnamed union
  private:
    // the sorted transitions array (order relation is given by
    // lower_than_transition):
    Transition *_t;
    char _tr[sizeof(Transition)]; // when there is only one transition
  public:
    Transition*& t() { return _t; }
    Transition& tr() { return *((Transition*) _tr); }
    Transition* const & t() const { return _t; }
    const Transition& tr() const { return *((Transition*) _tr); }
  } tunion;

public:
  typedef Transition*       iterator;
  typedef const Transition* const_iterator;

  state()
    : degree_in_final(0UL), height_card(0UL) { 
    tunion.t() = NULL;
  }

  state(const state &x)
    : degree_in_final(x.degree_in_final), height_card(x.height_card) 
  {
    state_base<Transition, Tag>::tag() = x.tag();
    degree_in_final &= final_mask; // in-degree is 0
    unsigned int n = x.card();
    switch (n) {
    case	0 : tunion.t() = NULL; break;
    case	1 : tunion.tr() = x.tunion.tr(); break;
    default :
      tunion.t() = new Transition[n];
      memcpy(tunion.t(), x.tunion.t(), sizeof(Transition) * n); 
      // Warning: DFA_min is responsible for in-degree updating
    }
  }

  ~state() {
    if (card() > 1) delete [] tunion.t();
  }

  unsigned int delta1(typename Transition::char_type a) const {
    const_iterator where = lower_bound(begin(), end(), Transition(a), 
				       lower_transition<Transition>());
    return (where == end() || (*where).letter() != a) ? 0 : (*where).aim();
  }

  const_iterator begin() const {
    return card() == 1 ? &(tunion.tr()) : tunion.t();
  }

  const_iterator end() const {
    return card() == 1 ? &(tunion.tr()) + 1 : (tunion.t() + card());
  }

  iterator begin() {
    return card() == 1 ? &(tunion.tr()) : tunion.t();
  }

  iterator end() {
    return card() == 1 ? &(tunion.tr()) + 1 : (tunion.t() + card());
  }

  unsigned int size() const {
    return card();
  }

  const_iterator find(typename Transition::char_type a) const {
    const_iterator where = lower_bound(begin(), end(), Transition(a), 
				       lower_transition<Transition>());
    return (where == end() || (*where).letter() != a) ? end() : where;
  }

  iterator find(typename Transition::char_type a) {
    iterator where = lower_bound(begin(), end(), Transition(a), 
				 lower_transition<Transition>());
    return (where == end() || (*where).letter() != a) ? end() : where;
  }

  void insert(const Transition &x) {  // Precond: transition does not exist
    unsigned int n = card();
    if (n == 0) 
      tunion.tr() = x; 
    else {
      iterator where = lower_bound(begin(), end(), x, lower_transition<Transition>());
      unsigned int i = where - begin();
      Transition *tmp = new Transition[n + 1];
      memcpy(tmp, begin(), i * sizeof(Transition));
      tmp[i] = x;
      memcpy(tmp + i + 1, where, (end() - where) * sizeof(Transition));
      if (n > 1) delete [] tunion.t();
      tunion.t() = tmp;
    }
    inc_card();
  }

  void change_trans(const Transition &x) {   // Precond: transition is defined
    (*(lower_bound(begin(), end(), x, lower_transition<Transition>()))) = x;
  }

  void erase(const Transition &x) {  // Precond: the transition is defined
    unsigned int n = card();
    switch (n) {
    case 1 : tunion.t() = NULL; break;
    case 2 : 
      {
	Transition *tmp = tunion.t();
	tunion.tr() = x.letter() < tmp[1].letter() ? tmp[1] : tmp[0];
	delete [] tmp;
	break;
      }
    default :
      {
	iterator where = lower_bound(begin(), end(), x, 
				     lower_transition<Transition>());
	Transition *tmp = new Transition[n - 1];
	memcpy(tmp, begin(), (where - begin()) * sizeof(Transition));
	memcpy(tmp + (where - begin()), where + 1, 
	       (end() - (where + 1)) * sizeof(Transition));
	delete [] tunion.t();
	tunion.t() = tmp;
      }
    }
    dec_card();
  }

  unsigned int degree_in() const {
    return ((degree_in_final & degree_in_mask) >> DEGREE_IN_SHIFT).to_ulong();
  }

  void inc_degree_in() {
    unsigned int d = degree_in() + 1;
    degree_in_final &= final_mask; 
    degree_in_final |= bitset_type(d) << DEGREE_IN_SHIFT;
  }

  void dec_degree_in() {
    unsigned int d = degree_in() - 1;
    degree_in_final &= final_mask;
    degree_in_final |= bitset_type(d) << DEGREE_IN_SHIFT;
  }

  unsigned int card() const {
    return (height_card & card_mask).to_ulong();
  }

  void inc_card() {
    unsigned int c = card() + 1;
    height_card &= height_mask;
    height_card |= bitset_type(c);
  }

  void dec_card() {
    unsigned int c = card() - 1;
    height_card &= height_mask;
    height_card |= bitset_type(c);
  }
		
  unsigned int height() const {
    return ((height_card & height_mask) >> HEIGHT_SHIFT).to_ulong();
  }

  void height(unsigned int h) {
    height_card &= card_mask;
    height_card |= bitset_type(h) << HEIGHT_SHIFT;
  }

  bool final() const {
    return (degree_in_final & final_mask).to_ulong() == 1;
  }

  class bool_reference
  {
    bitset_type &r;
  public:
    bool_reference(bitset_type &ref)
      : r(ref)
    { }
    
    operator bool() const {
      return (r & final_mask).to_ulong() == 1;
    }

    bool_reference& operator=(const bool_reference &x) {
      if (x == true) r |= final_mask;
      else r &= degree_in_mask;
      return *this;
    }

    bool_reference& operator=(bool b) {
      if (b == true) r |= final_mask;
      else r &= degree_in_mask;
      return *this;
    }

    bool operator==(const bool_reference &x) const {
      return (r & final_mask) == (x.r & final_mask);
    }

    bool operator==(bool b) const {
      return ((r & final_mask).to_ulong() != 0) == b;
    }
  };

  bool_reference final() {
    return bool_reference(degree_in_final);
  }

  // This strict weak order relation defines equivalent states:
  // Let q, p be two states. We have (q == p) <=> !(q < p) && !(p < q)
  // It compares two states lexicographicaly
//   bool operator< (const state<Transition, Tag> &x) const {
//     if (final() < x.final()) return true;
//     if (final() == x.final()) {
//       if (size() < x.size()) return true;
//       else
// 	if (size() == x.size()) { 
// 	  const_iterator i = begin(), j = end(), k = x.begin();
// 	  while (i != j)
// 	    if (*i < *k) return true;
// 	    else if (*k < *i) return false;
// 	    else { ++i; ++k; }
// 	  return false;
// 	}
//     }
//     return false;
//   }
};

  // This strict weak order relation defines equivalent states:
  // Let q, p be two states. We have (q == p) <=> !(q < p) && !(p < q)
  // It compares two states lexicographicaly
template <typename Transition, typename Tag>
inline
bool operator<(const state<Transition, Tag> &x, const state<Transition, Tag> &y) {
  if (x.final() < y.final()) return true;
  if (x.final() == y.final()) {
    if (x.size() < y.size()) return true;
    else
      if (x.size() == y.size()) { 
	typename state<Transition, Tag>::const_iterator i = x.begin(), j = x.end(), k = y.begin();
	while (i != j)
	  if (*i < *k) return true;
	  else if (*k < *i) return false;
	  else { ++i; ++k; }
	return false; // equivalent, not lower than
      }
  }
  return false;
}

// Compare two states given their ID (map ID -> state structure)
// Needed to use sets of state ID
template <typename Transition, typename Tag>
struct compare_state 
  : public binary_function<typename vector<state<Transition, Tag>*>::size_type, 
			   typename vector<state<Transition, Tag>*>::size_type, bool> 
{
  const vector<state<Transition, Tag>*> &Q;
  compare_state(const vector<state<Transition, Tag>*> &r)
    : Q(r)
  { }
  bool operator() (typename vector<state<Transition, Tag>*>::size_type x, 
		   typename vector<state<Transition, Tag>*>::size_type y) const {
    return *Q[x] < *Q[y];
  }
};

// ** CharType shall be a POD type **

/**
   A dynamic minimal acyclic DFA container class. Words can be added
   keeping the automaton minimal. It does not define the standard DFA
   interface but a restricted one that allows only word insertions and
   read-only operations thus enforcing the invariants for the
   structure integrity to be guaranteed.

   @tparams <tr><td>@c CharTraits</td><td>Character traits describing
    the alphabet that the words are built with.</td><td>@c plain</td><td>@c
    CharTraits is a model of #Alphabet and @c CharTraits::char_type is
    a POD type</td></tr>
    <tr><td>@c Tag</td><td>@tagtype.</td><td>@c empty_tag</td> 
    <td></td></tr></table>
 */

template <typename CharTraits = plain, typename Tag = empty_tag>
class DFA_min : public DFA_concept
{
public:
  typedef transition<typename CharTraits::char_type> transition_type;
  typedef CharTraits                                 char_traits;
  typedef Tag                                        tag_type;
  typedef unsigned int                               state_type;
  typedef typename CharTraits::char_type             char_type;
  typedef DFA_min                                    self;


#ifndef _MSC_VER
  static const state_type null_state = 0;
#else
  static const state_type null_state;
#endif

protected:
  state_type          i;
  bool                ready;
  unsigned int       _state_count, _trans_count;

  typedef set<state_type, compare_state<transition_type, tag_type> >   container;
  vector<container*>                       _pool;  // each height has its set
  vector<state<transition_type, tag_type>*> Q;
  compare_state<transition_type, tag_type>                            cmp_state;
  state_type                               hole;  // 1st free in state vector
  unsigned int                            wc_; // word count

  // statistics:
  unsigned long state_creation, state_copy, state_merge;

  container& pool(unsigned int h) { 
    for(; _pool.size() < h + 1; _pool.push_back(new container(cmp_state))); 
    return *(_pool[h]);
  }

  void prepare();

  typename state<transition_type, tag_type>::bool_reference final(state_type q) {
    return Q[q]->final();
  }

  void initial(state_type q) {
    i = q;
  }

  state_type new_state() { return new_state_(); }

  // for duplicating states:
  state_type new_state(const state<transition_type, tag_type> &q) { return new_state_(&q); }

  state_type new_state_(const state<transition_type, tag_type> *s = NULL) {
    if (s == NULL) ++state_creation;
    else ++state_copy;

    ++_state_count;
    while (hole < Q.size())
      if (Q[hole] == NULL) {
	Q[hole] = s == NULL ? new state<transition_type, tag_type> : new state<transition_type, tag_type>(*s);
	return hole++;
      }
      else ++hole;
    Q.push_back(s == NULL ? new state<transition_type, tag_type> : new state<transition_type, tag_type>(*s));
    ++hole;
    return Q.size() - 1;
  }

public:
  tag_type& tag(state_type q) { return Q[q]->tag(); }

  const tag_type& tag(state_type q) const { return Q[q]->tag(); }

protected:
  void reset() {
    freeze();
    typename vector<state<transition_type, tag_type>*>::iterator ii = 
      Q.begin(), j = Q.end();
    for(; ii != j; ++ii)
      delete *ii;
    
    i = 0;
    _state_count = 0;
    _trans_count = 0;
    Q.resize(1);
    hole = 2;
    wc_ = 0;
  }      

  void del_state(state_type q) {
    ++state_merge;

    --_state_count;
    _trans_count -= Q[q]->size();
    typename state<transition_type, tag_type>::iterator i = Q[q]->begin(), 
      j = Q[q]->end();
    for(; i != j; ++i)
      Q[(*i).aim()]->dec_degree_in();
    delete Q[q];
    Q[q] = NULL;
    hole = min__(hole, q);
  }

  void set_trans(state_type q, char_type a, state_type p) {
    ++_trans_count;
    Q[q]->insert(transition_type(a, p));
    Q[p]->inc_degree_in();
  }

  state_type duplicate_state(state_type q) {
    _trans_count += Q[q]->size();
    state_type p = new_state(*Q[q]);
    typename state<transition_type, tag_type>::iterator i = Q[p]->begin(), j = Q[p]->end();
    for(; i != j; ++i)
      Q[(*i).aim()]->inc_degree_in();
    return p;
  }
	
  void del_trans(state_type q, char_type a) {
    --_trans_count;
    Q[Q[q]->delta1(a)]->dec_degree_in();
    Q[q]->erase(transition_type(a));
  }

  void change_trans(state_type q, char_type a, state_type p) {
    Q[Q[q]->delta1(a)]->dec_degree_in();
    Q[q]->change_trans(transition_type(a,p));
    Q[p]->inc_degree_in();
  }

public: // TMP
  void height(state_type q, unsigned int h) {
    Q[q]->height(h);
  }

  unsigned int height(state_type q) const {
    return Q[q]->height();
  }

  unsigned int degree_in(state_type q) const {
    return Q[q]->degree_in();
  }

  void degree_in(state_type q, unsigned int d) {
    Q[q]->degree_in(d);
  }

  /** word count.
      @return a count of the words in the automaton language
  */
  unsigned int wc() const { return wc_; }

public:
  typedef skip_blanks_iterator<state<transition_type, tag_type> > const_iterator;

  DFA_min()
    : i(0),  ready(false),
      _state_count(0), _trans_count(0), Q(1, (state<transition_type, tag_type>*) 0), 
      cmp_state(Q), hole(2), wc_(0),
      state_creation(0), state_copy(0), state_merge(0)
  { 
    i = new_state();
  }

  ostream& stats(ostream &out) const {
    out << "word count " << wc() << endl << "states " << state_count();
    out << endl << "transitions " << trans_count() << endl;
    out << "state creations " << state_creation << endl;
    out << "state copies    " << state_copy << endl;
    return out << "state merges    " << state_merge << endl;
  }

  const_iterator begin() const {
    return const_iterator(&Q, initial());
  }

  const_iterator end() const {
    return const_iterator(&Q, Q.size());
  }

  ~DFA_min() { reset(); }

  /**
     @post @code wc() == 0 @endcode
  */
  void clear() {
    reset();
    i = new_state();
  }

  bool final(state_type q) const {
    return Q[q]->final();
  }
	
  /** Get rid of the extra data used for maintaining the automaton
      minimality. Once frozen, words can still be added but the next
      insertion will take more time to rebuild the necessary data
      structure that had been disposed of.
  */
  void freeze() { 
    for(typename vector<container*>::iterator i = _pool.begin(); 
	i != _pool.end(); ++i)
      delete *i;
    _pool.clear();
    ready = false;
  }

 /** Add a word defined by a range on a sequence of characters of
      type @c char_type.
      @return @c true if the word has been actually added, @c false if
      the word already exists and that the operation left the
      automaton unchanged.
      @post the automaton is minimal.
  */
  template <class ForwardI>
  bool insert(ForwardI, ForwardI);

  /** Add a word defined by a character string.
      @return @c true if the word has been actually added, @c false if
      the word already exists and that the operation left the
      automaton unchanged.
      @post the automaton is minimal.
  */	
  bool insert(const basic_string<char_type> &s) {
    return insert(s.begin(), s.end());
  }
	
  state_type initial() const { return i; }

  state_type delta1(state_type q, char_type a) const {
    return Q[q]->delta1(a);
  }

  class edges_type
  {
  protected:
    state<transition_type, tag_type> *q;

  public:
    typedef int                               difference_type;
    typedef char_type                         key_type;
    typedef state_type                        data_type;
    typedef pair<const char_type, state_type> value_type;

    edges_type(state<transition_type, tag_type> *p)
      : q(p)
    { }

    edges_type()
    {}

    bool empty() const {
      return q->size() == 0;
    }

    unsigned int size() const {
      return q->size();
    }

    class const_iterator 
      : public iterator<bidirectional_iterator_tag, 
			pair<char_type, state_type> >
    {
    protected:
      typename state<transition_type, tag_type>::const_iterator i;

    public:
      typedef const_iterator self;

      const_iterator()
      { }
      const_iterator(typename state<transition_type, tag_type>::const_iterator j)
	: i(j)
      { }
      pair<char_type, unsigned int> operator* () const {
	return i->make_pair(); // make_pair((*i).letter(), (*i).aim());
      }
      self& operator++ () {
	++i;
	return *this;
      }
      self operator++ (int) {
	self tmp = *this;
	++*this;
	return tmp;
      }
      bool operator== (const self &x) const {
	return i == x.i;
      }
      self& operator--() {
	--i;
	return *this;
      }
      self operator-- (int) {
	self tmp = *this;
	--*this;
	return tmp;
      }
    };
		
    const_iterator begin() const {
      return const_iterator(q->begin());
    }

    const_iterator end() const {
      return const_iterator(q->end());
    }

    const_iterator find(char_type a) const {
      return const_iterator(q->find(a));
    }
  };

  edges_type delta2(state_type q) const { return edges_type(Q[q]); }

  unsigned int state_count() const { return _state_count;  }
  unsigned int trans_count() const { return _trans_count; }

  /** @return @c wc()
   */
  unsigned int size() const { return wc_; }
};

#ifndef _MSC_VER
template <typename C, typename T>
const typename DFA_min<C, T>::state_type DFA_min<C, T>::null_state;
#else
template <typename C, typename T>
const typename DFA_min<C, T>::state_type DFA_min<C, T>::null_state = 0;
#endif

template <typename CharTraits, typename Tag>
void DFA_min<CharTraits, Tag>::prepare() {
  const_iterator first = begin(), last = end();
  for(; first != last; ++first)
    pool(height(*first)).insert(*first);
  ready = true;
}

template <class CharTraits, typename Tag> 
template <class InputIterator>
bool DFA_min<CharTraits, Tag>::insert(InputIterator first, 
				      InputIterator last)
{
  {
    cursor<DFA_min<CharTraits, Tag> > c(*this);
    if (match(c, first, last)) return false;  // word exists ?
  }
  if (!ready) prepare();  // build the structure if necessary
  stack_cursor<forward_cursor<self> > c(forwardc(*this));
  bool duplicating;

  for(duplicating = false; first != last; ++first) {
    if (!duplicating)
      pool(height(c.src())).erase(c.src());

    if (!c.find(*first)) {
      set_trans(c.src(), *first, new_state());
      duplicating = true;
      c.find(*first);
    }
    else 
      if (Q[c.aim()]->degree_in() > 1) {
	change_trans(c.src(), *first, duplicate_state(c.aim())); 
	duplicating = true;
	c.find(*first);
      }
    c.forward();
  }

  if (!duplicating) 
    pool(height(c.src())).erase(c.src());

  final(c.src()) = true;
  unsigned int h = height(c.src());

  for(; c.backward(); ) {
    // try to insert the current state in its height pool
    // if an equivalent state is found its ID is returned, otherwise
    // it is inserted 
    pair<typename container::iterator, bool> i = 
      pool(height(c.aim())).insert(c.aim());
    if (i.second == false) {  // found an equivalent state ?
      state_type tmp = c.aim(); 
      // redirect the transition towards equivalent state
      change_trans(c.src(), c.letter(), *i.first);
      del_state(tmp);
    }
    h = max__(height(c.src()), h+1); //Precond: states height initialized with 0
    height(c.src(), h);
  }
  pool(height(initial())).insert(initial());
  ++wc_;
  return true;
}

} // namespace astl

#endif // ASTL_CLASS_DFA_MIN

  
