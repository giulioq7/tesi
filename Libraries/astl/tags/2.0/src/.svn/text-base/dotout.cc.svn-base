#include <astl.h>
#include <stream.h>
#include <dot.h>
#include <iostream>
#include <string>

using namespace std;
using namespace astl;


void usage(const string& exe)
{
  cerr << "Usage: " << exe << " [-s]" << endl;
  cerr << "      output a representation of the DFA read from stdin"
       << " suitable for dot" << endl; 
  cerr << "      -s\texpect string as the DFA alphabet" << endl;
  exit(1);
}

int main(int argc, const char** argv)
{
  if (argc > 2) usage(argv[0]);
  if (argc == 2)
    if (string(argv[1]) == "-s") { // string mode ?
      istream_cursor<DFA_map<strings> > c(cin);
      dot(cout, c);
    }
    else usage(argv[0]);
  else {  // normal, character mode
    istream_cursor<DFA_map<plain> > c(cin);
    dot(cout, c);
  }
  return 0;
}


