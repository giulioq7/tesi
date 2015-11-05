// This file tests pre/postconditions and invariants of NFA class.

#include <astl.h>
#include <nfa_mmap.h>

#include <cassert>
#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

using namespace astl;

template <typename NFA>
void check_state_creation(NFA &A, typename NFA::state_type q) {
  assert(q != A.null_state);
  typename NFA::edges_type e = A.delta2(q);
  assert(e.empty() == true);  // there should be no transitions at creation
  assert(e.size() == 0);
  assert(e.find('a') == e.end());
  assert(e.begin() == e.end());
  assert(e.count(' ') == 0);
  assert(e.lower_bound('z') == e.begin());
  assert(e.upper_bound('z') == e.end());
  pair<typename NFA::edges_type::const_iterator, 
    typename NFA::edges_type::const_iterator> p = e.equal_range('z');
  assert(p.first == p.second);
  assert(e == e);
  typedef typename NFA::edges_type::key_type dummy1;
  typedef typename NFA::edges_type::mapped_type dummy2;
  typedef typename NFA::edges_type::value_type dummy3;
  typedef typename NFA::edges_type::key_compare dummy4;
  typedef typename NFA::edges_type::value_compare dummy5;
  typedef typename NFA::edges_type::const_reference dummy6;
  typedef typename NFA::edges_type::size_type dummy7;
  typedef typename NFA::edges_type::difference_type dummy8;
  typedef typename NFA::edges_type::const_iterator dummy9;
  typedef typename NFA::edges_type::const_reverse_iterator dummy10;
  typename NFA::edges_type::key_compare kc = e.key_comp();
  assert(kc('b', 'a') == false);
  typename NFA::edges_type::value_compare vc = e.value_comp();
  pair<typename NFA::edges_type::key_type, typename NFA::edges_type::data_type> p1('a', A.null_state);
  typename NFA::edges_type::value_type p2('b', NFA::null_state);
  assert(vc(p2, p1) == false);
}	

template <typename NFA>
void check_transition_creation(NFA &A, typename NFA::state_type q, 
			       typename NFA::char_traits::char_type a, 
			       typename NFA::state_type p,
			       unsigned long &T) {
  typename NFA::state_type qq;
  assert(A.delta1(q, a, &qq) == &qq);
  A.set_trans(q, a, p);
  assert(A.trans_count() == ++T);
  assert(A.delta1(q, a, &qq) == &qq + 1);
  assert(qq == p);
  typename NFA::edges_type e = A.delta2(q);
  typedef typename NFA::edges_type edges_t;
  typename edges_t::size_type s = e.size();
  assert(e.size() > 0);
  assert(e.empty() == false);
  pair<typename NFA::char_type, typename NFA::state_type> tmp_pair(a, p);
  assert(*(e.find(a)) == tmp_pair);
  assert(e.count(a) == 1);
  for(typename NFA::edges_type::const_iterator i = e.begin(); i != e.end(); ++i, --s);
  assert(s == 0);
  assert(e == e);
}

template <typename NFA>
void check_state_deletion(NFA &A, typename NFA::state_type q, 
			  unsigned long &Q, unsigned long &T) {
  T -= A.delta2(q).size();
  A.del_state(q);
  assert(A.state_count() == --Q);
  assert(A.trans_count() == T);
}

template <typename NFA>
void check_transition_deletion(NFA &A, typename NFA::state_type q, 
			       typename NFA::char_type a, 
			       typename NFA::state_type p,
			       unsigned long &T) {
  typename NFA::edges_type::size_type s = A.delta2(q).size();
  typename NFA::state_type qq;
  assert(A.delta1(q, a, &qq) != &qq);
  A.del_trans(q, a);
  assert(A.trans_count() == --T);
  assert(A.delta1(q, a, &qq) == &qq);
  assert(A.delta2(q).size() == --s);
}

template <typename NFA>
void check(NFA &A)
{
  unsigned long Q = 0, T = 0;

  // Check the NFA initial state:
  assert(A.state_count() == 0);
  assert(A.trans_count() == 0);
  assert(A.initial().empty());

  // state_type creation:
  typename NFA::state_type q = A.new_state();
  check_state_creation(A, q);
  assert(A.state_count() == ++Q);
  A.initial().insert(q);
  assert(A.initial().find(q) != A.initial().end());

  // Transition creation
  typename NFA::state_type p = A.new_state();
  check_state_creation(A, p);
  assert(A.state_count() == ++Q);
  check_transition_creation(A, q, 'a', p, T);
  check_transition_creation(A, q, 'é', p, T);
  check_transition_creation(A, p, 'a', p, T);
  check_transition_creation(A, p, 'é', p, T);
  assert(A.delta2(q) == A.delta2(p));
	
  // state_type deletion and initial state constistency:
  check_state_deletion(A, q, Q, T);
  assert(A.initial().find(q) == A.initial().end());   // the initial state q has been removed

  // Multiple state creation:
  typename NFA::state_type t[25];
  typename NFA::state_type *ptr = A.new_state(10, t);
  assert(ptr == (t + 10));
  for(typename NFA::state_type *pt = t; pt != t + 10; ++pt)
    check_state_creation(A, *pt);
  Q += 10;
  assert(A.state_count() == Q);
  ptr = A.new_state(15, ptr);
  assert(ptr == (t + 25));
  for(typename NFA::state_type *pt = t + 10; pt != ptr; ++pt)
    check_state_creation(A, *pt);
  Q += 15;
  assert(A.state_count() == Q);	

  // Transition deletion:
  check_transition_deletion(A, p, 'a', p, T);
  check_transition_deletion(A, p, 'é', p, T);

  // Transition redefinition:
  q = A.new_state();
  check_state_creation(A, q);
  assert(A.state_count() == ++Q);
  check_transition_creation(A, p, '$', p, T);
  check_transition_creation(A, p, 't', q, T);
  check_transition_creation(A, p, '0', q, T);
  A.change_trans(p, '0', q, p);
  typename NFA::edges_type::size_type s = A.delta2(p).size();
  typename NFA::state_type qq;
  assert(A.delta1(p, '0', &qq) != &qq);
  assert(qq == p);
  assert(A.trans_count() == T);
  assert(A.delta2(p).size() == s);
  assert((*A.delta2(p).find('0')).first == '0');
  assert((*A.delta2(p).find('0')).second == p);
	
  // state_type duplication:
  check_transition_creation(A, p, ',', t[0], T);
  check_transition_creation(A, p, 'u', t[0], T);
  check_transition_creation(A, p, 'ç', t[1], T);
  check_transition_creation(A, p, 'o', t[2], T);
  typename NFA::state_type h = A.duplicate_state(p);
  assert(h != A.null_state);
  assert(A.state_count() == ++Q);
  T += A.delta2(p).size();
  assert(A.trans_count() == T);
  assert(A.delta2(h) == A.delta2(p));
  assert(A.delta2(h).size() == A.delta2(p).size());
  assert(A.delta2(h).empty() == A.delta2(p).empty());
  assert(equal(A.delta2(h).begin(), A.delta2(h).end(), A.delta2(p).begin()));
  assert(A.final(h) == A.final(p));

  // state_type copy:
  check_transition_creation(A, t[1], 'ù', t[4], T);
  check_transition_creation(A, t[1], '?', t[5], T);
  T += A.delta2(p).size() - A.delta2(t[1]).size();
  A.final(p) = true;
  assert(A.final(p) == true);
  A.copy_state(p, t[1]);
  assert(A.state_count() == Q);
  assert(A.trans_count() == T);
  assert(A.delta2(t[1]) == A.delta2(p));
  assert(A.delta2(t[1]).size() == A.delta2(p).size());
  assert(A.delta2(t[1]).empty() == A.delta2(p).empty());
  assert(equal(A.delta2(t[1]).begin(), A.delta2(t[1]).end(), A.delta2(p).begin()));
  assert(A.final(t[1]) == A.final(p));

  // Final:
  A.final(t[9]) = A.final(p);
  assert(A.final(t[9]) == true);
  assert(A.final(t[9]) == A.final(p));
  A.final(p) = false;
  assert(A.final(p) == false);
  assert(A.final(t[9]) != A.final(p));

  // Delta1 and Delta2 consistency:
  typename vector<pair<typename NFA::char_traits::char_type, typename NFA::state_type> >::const_iterator i;
  vector<pair<typename NFA::char_traits::char_type, typename NFA::state_type> > v;
  copy(A.delta2(p).begin(), A.delta2(p).end(), back_inserter(v));
  for(i = v.begin(); i != v.end(); ++i) {
    assert(A.delta1(p, (*i).first, &qq) == &qq + 1);
    assert(qq == (*i).second);
  }
}

int main()
{
  cout << "Testing NFA mmap" << endl;
  NFA_mmap<plain> A, AA(12);
  check(A);
}
	
