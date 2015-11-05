#include <astl.h>
#include <minimize.h>
#include <string>
#include <iostream>
#include <stream.h>
#include <determinize.h>

using namespace std;
using namespace astl;


const string USAGE = "";
const string DESCRIPTION = "reverse the DFA read from stdin";

template <class DFA>
void run(DFA &A, const config& c)
{
  A.initial(clone(A, istream_cursor<DFA>(cin)));
  NFA_mmap<plain, typename DFA::state_type> B;
  reverse(A, B);

  if (c.state_mark)
    dump(cout, dfirst_markc(forwarddc(B)));
  else
    dump(cout, dfirstc(forwarddc(B)));
}

int main(int argc, char **argv)
{
  config c(argc, argv, 1, "all", USAGE, DESCRIPTION);

  if (c.representation == "map") {
    DFA_map<plain> dfa;
    run(dfa, c);
  }
  if (c.representation == "matrix") {
    DFA_matrix<plain> dfa;
    run(dfa, c);
  }
  if (c.representation == "bin") {
    DFA_bin<plain> dfa;
    run(dfa, c);
  }
  if (c.representation == "mtf") {
    DFA_mtf<plain> dfa;
    run(dfa, c);
  }
  if (c.representation == "tr") {
    DFA_tr<plain> dfa;
    run(dfa, c);
  }

  return 0;
}
