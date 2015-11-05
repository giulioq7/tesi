#include <astl.h>
#include <stream.h>
#include <set_operation.h>

using namespace astl;

template <typename DFA>
int run(DFA &A, DFA &B, config &c)
{
  A.initial(clone(A, istream_cursor<DFA>(cin)));
  B.initial(clone(B, istream_cursor<DFA>(c.aux_input)));
  if (c.state_mark)
    dump(cout, dfirst_markc(sym_diffc(forwardc(A), 
				      forwardc(B))));
  else
    dump(cout, dfirstc(sym_diffc(forwardc(A), 
				 forwardc(B))));
  return 0;
}

int main(int argc, char **argv)
{
  config c(argc, argv, 2, "map matrix bin");
  
  if (c.representation == "map") {
    DFA_map<plain> A1, A2;
    return run(A1, A2, c);
  }

  if (c.representation == "matrix") {
    DFA_matrix<plain> A1, A2;
    return run(A1, A2, c);
  }
  
  if (c.representation == "bin") {
    DFA_bin<plain> A1, A2;
    return run(A1, A2, c);
  }
}; 
