#include <astl.h>
#include <stream.h>
#include <set_operation.h>
#include <iostream>

using namespace astl;

template <typename DFA>
int run(DFA &A, config &c)
{
  A.initial(clone(A, istream_cursor<DFA>(cin)));
  if (c.state_mark)
    dump(cout, dfirst_markc(notc(forwardc(A))));
  else
    dump(cout, dfirstc(notc(forwardc(A))));
  return 0;
}

int main(int argc, char **argv)
{
  config c(argc, argv, 1, "map matrix bin");
  
  if (c.representation == "map") {
    DFA_map<plain> A;
    return run(A, c);
  }

  if (c.representation == "matrix") {
    DFA_matrix<plain> A;
    return run(A, c);
  }
  
  if (c.representation == "bin") {
    DFA_bin<plain> A;
    return run(A, c);
  }
}; 
