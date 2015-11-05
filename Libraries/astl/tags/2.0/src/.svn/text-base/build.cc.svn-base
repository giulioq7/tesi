#include <astl.h>
#include <build.h>
#include <stream.h>
#include <iostream>

using namespace std;
using namespace astl;


template <typename DFA>
void run(DFA& dfa, bool verbose, bool state_mark)
{
  dfa.initial(build(dfa, cin));
  if (verbose) 
    cerr << "DFA built has " << dfa.state_count() << " states and "
	 << dfa.trans_count() << " transitions" << endl;
  if (state_mark)
    dump(cout, dfirst_markc(dfa));
  else
    dump(cout, dfirstc(dfa));
}

int main(int argc, char** argv)
{
  config c(argc, argv, 1, "all");

  if (c.representation == "matrix") {
    DFA_matrix<> dfa;
    run(dfa, c.verbose_mode, c.state_mark);
  }
  if (c.representation == "map") {
    DFA_map<> dfa;
    run(dfa, c.verbose_mode, c.state_mark);
  }
  if (c.representation == "bin") {
    DFA_bin<> dfa;
    run(dfa, c.verbose_mode, c.state_mark);
  }
  if (c.representation == "mtf") {
    DFA_mtf<> dfa;
    run(dfa, c.verbose_mode, c.state_mark);
  }
  if (c.representation == "tr") {
    DFA_tr<> dfa;
    run(dfa, c.verbose_mode, c.state_mark);
  }
  return 0;
}  
