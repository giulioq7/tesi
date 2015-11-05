#include <tools.h>
#include <dfa.h>
#include <ccopy.h>
#include <cursor.h>
#include <match.h>
#include <iostream>
#include <iterator>
#include <stream.h>
#include <algorithm>

using namespace astl;

const string USAGE = "[-s | -l] [-o]";
const string DESCRIPTION = "match input file against the DFA read from stdin\n\
   -s\tshortest match\n   -l\tlongest match (default)\n   -o\treport \
overlapping matches";
bool shortest = false;
bool overlapping = false;

template <typename DFA>
void run(DFA& dfa, config &conf)
{
  dfa.initial(clone(dfa, istream_cursor<DFA>(cin)));
  frandom_iterator<> first(conf.aux_handle), last;
  cursor<DFA> c(dfa, dfa.initial());
  frandom_iterator<> end_of_match, tmp;

  for(; first != last;) {
    if (shortest) end_of_match = first_match(c, first, last);
    else end_of_match = longest_match(c, first, last);
    if (end_of_match != first) {
      if (overlapping) {
	for(tmp = first; first != end_of_match; ++first)
	  cout << *first;
	first = ++tmp;
      }
      else
	for(; first != end_of_match; ++first)
	  cout << *first;
      cout << endl;
    }
    else ++first;
    c = dfa.initial();
  }
} 

int main(int argc, char** argv)
{
  config c(argc, argv, 2, "all", USAGE, DESCRIPTION);

  for(config::const_iterator i = c.begin(); i != c.end(); ++i)
    if (*i == "-s") shortest = true;
    else if (*i == "-l") shortest = false;
    else if (*i == "-o") overlapping = true;
    else c.usage();

  if (c.representation == "matrix") {
    DFA_matrix<> dfa;
    run(dfa, c);
  }
  if (c.representation == "map") {
    DFA_map<> dfa;
    run(dfa, c);
  }
  if (c.representation == "bin") {
    DFA_bin<> dfa;
    run(dfa, c);
  }
  if (c.representation == "mtf") {
    DFA_mtf<> dfa;
    run(dfa, c);
  }
  if (c.representation == "tr") {
    DFA_tr<> dfa;
    run(dfa, c);
  }

  return 0;
}
