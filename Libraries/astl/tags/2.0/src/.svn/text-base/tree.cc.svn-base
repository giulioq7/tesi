#include <astl.h>
#include <language.h>
#include <dfa.h>
#include <stream.h>
#include <tools.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <ccopy.h>
#include <string>
#include <iterator>

using namespace std;
using namespace astl;


template <typename DFA>
void run(DFA& dfa, const config& c)
{
  int wc;
  string word;

  for(wc = 0, getline(cin, word); !cin.eof(); ++wc, getline(cin, word)) 
    add_word(dfa, word.begin(), word.end());
 

  if (c.verbose_mode) 
    cerr << c.argv[0] << ": word count " << wc << endl;
  dump(cout, dfirstc(dfa));
}

int main(int argc, char** argv)
{
  config c(argc, argv, 1, "map bin tr mtf");

  if (c.representation == "map") {
    DFA_map<plain> dfa;
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
