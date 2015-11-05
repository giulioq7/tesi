#include <astl.h>
#include <minimize.h>
#include <string>
#include <iostream>
#include <stream.h>

using namespace std;
using namespace astl;


// send the minimized input dfa to standard output

const string USAGE = "[-a|-c]";
const string DESCRIPTION = "minimize the DFA read from stdin\n\
   -a\tacyclic minimization (Revuz)\n   -c\tcyclic minimization (Brzozowski, default)";
bool cyclic = true;

template <class DFA>
void minimize(DFA &A, const config& c)
{
  A.initial(clone(A, istream_cursor<DFA>(cin)));
  if (c.verbose_mode)
    cerr << c.argv[0] << ": before: states " << A.state_count() 
	 << ", transitions " << A.trans_count() << endl;
  
  DFA B;		       
  brzozowski(A, B);
  
  if (c.verbose_mode) 
    cerr << c.argv[0] << ": after: states " << B.state_count() 
	 << ", transitions " << B.trans_count() << endl;

  if (c.state_mark)
    dump(cout, dfirst_markc(B));
  else
    dump(cout, dfirstc(B));
}

template <class DFA>
void acyclic_minimize(DFA &A, const config& c)
{
  A.initial(clone(A, istream_cursor<DFA_map<typename DFA::char_traits> >(cin)));
  if (c.verbose_mode)
    cerr << c.argv[0] << ": before: states " << A.state_count() 
	 << ", transitions " << A.trans_count() << endl;
  
  acyclic_minimization(A);
  
  if (c.verbose_mode) 
    cerr << c.argv[0] << ": after: states " << A.state_count() 
	 << ", transitions " << A.trans_count() << endl;
  
  if (c.state_mark) 
    dump(cout, dfirst_markc(A));
  else
    dump(cout, dfirstc(A));
}

int main(int argc, char **argv)
{
  config c(argc, argv, 1, "map bin matrix", USAGE, DESCRIPTION);

  for(config::const_iterator i = c.begin(); i != c.end(); ++i)
    if (*i == "-a") cyclic = false;
    else if (*i == "-c") cyclic = true;
    else c.usage();

  if (c.representation == "map") 
    if (cyclic) {
      DFA_map<plain> A;
      minimize(A, c);
    }
    else {
      DFA_map<plain, minimization_tag> A;
      acyclic_minimize(A, c);
    }

  if (c.representation == "matrix") 
    if (cyclic) {
      DFA_matrix<plain> A;
      minimize(A, c);
    }
    else {
      DFA_matrix<plain, minimization_tag> A;
      acyclic_minimize(A, c);
    }
 
  if (c.representation == "bin") 
    if (cyclic) {
      DFA_bin<plain> A;
      minimize(A, c);
    }
    else {
      DFA_bin<plain, minimization_tag> A;
      acyclic_minimize(A, c);
    }

  return 0;
}
