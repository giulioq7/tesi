#include <astl.h>
#include <combinatory.h>
#include <stream.h>
#include <iostream>

using namespace astl;

int main(int argc, char** argv)
{
  config c(argc, argv, 0, "", "n p", "generate a DFA recognizing combinations");

  int n = 0, p = 0;
  if (c.arg_pos < argc) n = atoi(argv[c.arg_pos]);
  else c.usage();
  if (++c.arg_pos < argc) p = atoi(argv[c.arg_pos]);
  if (p < 1 || p > n) {
    cerr << argv[0] << ": p must be greater than n" << endl;
    c.usage();
  }

  if (c.state_mark)
    dump(cout, dfirst_markc(combinationc(n, p)));
  else
    dump(cout, dfirstc(combinationc(n, p)));
  
  return 0;
}
