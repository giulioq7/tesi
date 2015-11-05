#include <astl.h>
#include <language.h>
#include <aho_corasick.h>
#include <lazy.h>
#include <default.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <iterator>
#include <algorithm>

using namespace std;
using namespace astl;

const string USAGE = "[-nl]";
const string DESCRIPTION = "search stdin for the set of words read from file\
\n   -nl\tno lazy construction optimization";

template <typename DFA>
void run(DFA &A, config &c, bool nl)
{
  add_words(A, istream_iterator<string>(c.aux_input), 
	    istream_iterator<string>());
  c.aux_input.close();
  aho_corasick(A, '\0');
  if (nl) {
    substitute_cursor<forward_cursor<DFA> > s = substitutec(forwardc(A), '\0');
    cout << match_count(s, finput_iterator<>(stdin), finput_iterator<>());
  }
  else {
    lazy_cursor<substitute_cursor<forward_cursor<DFA> > > l = substitutec(forwardc(A), '\0');
    cout << match_count(l, finput_iterator<>(stdin), finput_iterator<>());
  }
  cout << endl;
}

int main(int argc, char **argv)
{
  config c(argc, argv, 2, "matrix map bin mtf tr", USAGE, DESCRIPTION);

  bool nl = find(c.options.begin(), c.options.end(), string("-nl")) 
    != c.options.end();
    
  if (c.representation == "map") {
    DFA_map<> dfa;
    run(dfa, c, nl);
  }
  if (c.representation == "bin") {
    DFA_bin<> dfa;
    run(dfa, c, nl);
  }
  if (c.representation == "matrix") {
    DFA_matrix<> dfa;
    run(dfa, c, nl);
  }
  if (c.representation == "mtf") {
    DFA_mtf<> dfa;
    run(dfa, c, nl);
  }
  if (c.representation == "tr") {
    DFA_tr<> dfa;
    run(dfa, c, nl);
  }
  
  return 0;
}


    

  
