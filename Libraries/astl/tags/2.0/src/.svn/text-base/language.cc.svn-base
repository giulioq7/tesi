#include <astl.h>
#include <language.h>
#include <stream.h>
#include <iostream>
#include <string>

using namespace std;
using namespace astl;


void usage(const string &exe)
{
  cerr << "Usage: " << exe << " [type]" << endl;
  cerr << "  dump the language recognized by the DFA read from stdin" << endl;
  cerr << "   type\talphabet type, char (default) or string" << endl;
  exit(1);
}

int main(int argc, char** argv)
{
  if (argc > 2) usage(argv[0]);
  if (argc == 2)
    if (string("string") == argv[1]) {
      istream_cursor<DFA_map<strings> > input(cin);
      language(cout, input);
      return 0;
    }
    else usage(argv[0]);

  istream_cursor<DFA_map<plain> > input(cin);
  language(cout, input);
  return 0;
}
