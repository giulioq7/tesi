#include <astl.h>
#include <stream.h>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;
using namespace astl;


void usage(const char* exe)
{
  cerr << "Usage: " << exe << endl;
  cerr << "       trim the uneeded paths of an acyclic dfa read from stdin" << endl;
  exit(1);
}

int main(int argc, char** argv)
{
  if (argc != 1) usage(argv[0]);
  DFA_bin<plain> dfa;
  if (argc == 1) dfa.initial(trim(dfa, istream_cursor<DFA_bin<plain> >(cin)));
  else {
    ifstream in(argv[1]);
    if (!in) {
      perror(argv[1]);
      exit(2);
    }
    dfa.initial(trim(dfa, istream_cursor<DFA_bin<plain> >(in)));
    in.close();
  }
  dump(cout, dfirstc(dfa));
  return 0;
}
