#include <astl.h>
#include <stream.h>
#include <iostream>
#include <iterator>

using namespace std;
using namespace astl;


int main(int argc, char** argv)
{
  config c(argc, argv, 1);
    
  DFA_min<> dfa;
  string word;
  int wc;

  for(wc = 0, getline(cin,word); !cin.eof(); ++wc, getline(cin,word)) 
    dfa.insert(word.begin(), word.end());

  if (c.verbose_mode) {
    cerr << argv[0] << ": word count " << wc << ", states " << dfa.state_count();
    cerr << ", transitions " << dfa.trans_count() << endl;
  }

  if (c.state_mark)
    dump(cout, dfirst_markc(dfa));
  else
    dump(cout, dfirstc(dfa));
  return 0;
}
