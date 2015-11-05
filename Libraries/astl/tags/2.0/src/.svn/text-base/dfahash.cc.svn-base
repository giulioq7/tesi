#include <astl.h>
#include <stream.h>
#include <hash.h>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;
using namespace astl;


const string USAGE = "";
const string DESCRIPTION = "read a DFA from file and output hash\
 value for each word read from stdin";

template <typename DFA>
void run(DFA &A, config &conf)
{
  A.initial(clone(A, istream_cursor<DFA>(conf.aux_input)));
  make_hash(A);
  string w;
  hash_cursor<forward_cursor<DFA> > c(forwardc(A));
  string::const_iterator i;
  if (conf.verbose_mode) cerr << "Ready" << endl;
  for(getline(cin, w); !cin.eof(); getline(cin, w)) {
    for(c = A.initial(), i = w.begin(); i != w.end() && c.forward(*i); ++i);
    cout << w << '\t';
    if (i == w.end() && c.src_final()) 
      cout << c.hash_value();
    else cout << 0;
    cout << endl;
  }
}
    
int main(int argc, char** argv)
{
  config c(argc, argv, 2, "map bin matrix", USAGE, DESCRIPTION);
  
  if (c.representation == "map") {
    DFA_map<plain, hash_tag> A;
    run(A, c);
  }

  if (c.representation == "matrix") {
    DFA_matrix<plain, hash_tag> A;
    run(A,c );
  }
  
  if (c.representation == "bin") {
    DFA_bin<plain, hash_tag> A;
    run(A, c);
  }

  return 0;
}
