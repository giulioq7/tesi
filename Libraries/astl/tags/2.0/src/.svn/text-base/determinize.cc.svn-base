#include <astl.h>
#include <determinize.h>
#include <nfa_mmap.h>
#include <iostream>

using namespace astl;

int main()
{
  NFA_mmap<plain> nfa;
  NFA_mmap<plain>::state_type Q[10];
  nfa.new_state(10, Q);
  nfa.initial().insert(Q, Q + 2);
  nfa.set_trans(Q[0], 'a', Q[1]);
  dcursor<NFA_mmap<plain> > dc(nfa, nfa.initial());
  dc.forward('a');
  forward_dcursor<NFA_mmap<plain> > df(nfa, nfa.initial());
  cout << df.find('a') << " '";
  cout <<  df.letter() << "'" << endl;
}

