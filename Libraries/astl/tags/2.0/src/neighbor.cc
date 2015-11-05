#include <astl.h>
#include <iostream>
#include <stream.h>
#include <language.h>
#include <neighbor.h>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#ifndef _MSC_VER
#include <unistd.h>
#endif

using namespace std;
using namespace astl;


void usage(const char* exe)
{
  cerr << "Usage: " << exe << " [-vwua] word distance" << endl;
  cerr << "       -v\tverbose mode" << endl;
  cerr << "       -w\toutput words" << endl;
  cerr << "       -a\toutput the DFA (default)" << endl;
  cerr << "       distance is a positive integer" << endl;
  exit(1);
}

int main(int argc, char** argv)
{
  typedef istream_cursor<DFA_map<plain> > input_cursor;
  typedef DFA_bin<plain> dfa_type;

  bool verbose = false;
  bool words = false;
  const char* exe = argv[0];

  for(int c; (c = getopt(argc, argv, "vwa")) != EOF;)
    switch(c) {
    case 'v' : verbose = true; break;
    case 'w' : words = true; break;
    case 'a' : words = false; break;
    default : usage(exe);
    }

  if (optind >= argc) usage(exe);
  const char* word = argv[optind];
  if (++optind == argc) usage(exe);
  int d = atoi(argv[optind]);
  if (d < 0) usage(exe);

// deprecated:  verbose_first(verbose, argc, argv);

  dfa_type dfa;
  dfa.initial(clone(dfa, input_cursor(cin)));

  if (words) 
    language(cout, dfirstc(neighborc(forwardc(dfa), word, d * 1000)));
  else 
    dump(cout, dfirstc(neighborc(forwardc(dfa), word, d * 1000)));


// deprecated:  verbose_last(verbose);
  return 0;
}
