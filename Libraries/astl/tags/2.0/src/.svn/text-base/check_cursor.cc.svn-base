#include <astl.h>
#include <default.h>
#include <check.h>
#include <debug.h>
#include <nfa_mmap.h>
#include <determinize.h>
#include <language.h>
#include <combinatory.h>

using namespace astl;

int main()
{
  DFA_matrix<> A;
  A.initial(A.new_state());
  check(plainc(A), "cursor");
  A.set_trans(A.initial(), 'a', A.new_state());
  check(transitionc(A), "transition_cursor");
  check(forwardc(A), "forward_cursor");
  check(debugplainc(A, cout), "cursor_debug");
  check(debugc(A, cout), "forward_cursor_debug");
  cursor_trace<cursor<DFA_matrix<> > > trace(plainc(A), cout);
  check(trace, "cursor_trace");
  check(tracec(forwardc(A), cout), "forward_cursor_trace");
  NFA_mmap<> nfa;
  NFA_mmap<>::state_type q = nfa.new_state(), p = nfa.new_state();
  nfa.initial().insert(q);
  check(dcursor<NFA_mmap<> >(nfa, nfa.initial()), "dcursor");
  nfa.set_trans(q, 'a', p);
  check(forward_dcursor<NFA_mmap<> >(nfa, nfa.initial()), "forward_dcursor");
  check(closurec(transitionc(nfa, q), 'a'), "closure_cursor");
  check(async_forward_dcursor<NFA_mmap<> >(nfa, 'a', nfa.initial()), "async_forward_dcursor");
  check(buildc(A), "build_cursor");
  check(def_transc(plainc(A), 'x'), "def_trans_cursor");
  check(def_statec(forwardc(A), A.initial()), "def_state_cursor");
  check(substitutec(forwardc(A), 'x'), "substitute_cursor");
  check(permutationc(5), "permutation_cursor", 1);
  check(combinationc(5, 3), "combination_cursor", 1);
  return 0;
}
