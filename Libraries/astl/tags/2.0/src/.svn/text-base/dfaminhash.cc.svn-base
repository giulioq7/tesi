#include <dfa.h>
#include <ccopy.h>
#include <stream.h>
#include <tools.h>
#include <iostream>
#include <iterator>

using namespace std;
using namespace astl;


// nmshld 8,5 
// ~332 Mo RAM (32 bits) == ~332 Mo RAM (32 bits, DFA_min<> == no hash)
// 8,5 Mmots triés :
// mer jun  4 21:01:22 CEST 2008 "../bin/dfaminhash -m yes -v "
// done 8500000
// done inserting 8507856 lines
// word count 8507856
// states 4837923
// transitions 11074359
// state creations 35714175
// state copies    8048048
// state merges    38924300
// ../bin/dfaminhash: 350.32 s CPU time (+ 100s sorting)
// 318s with a DFA_min<>

//  non triés
// mer jun  4 21:07:58 CEST 2008 "../bin/dfaminhash -m yes -v "
// done 8500000
// done inserting 8507856 lines
// word count 8507856
// states 4837923
// transitions 11074359
// state creations 35714175
// state copies    7015105
// state merges    37891357
// ../bin/dfaminhash: 1391.95 s CPU time

// Avec batch de 300000 (set<string>)
// mer jun  4 21:56:27 CEST 2008 "../bin/dfaminhash -m yes -v "
// done 8500000
// done inserting 8507856 lines
// word count 8400000
// states 4798261
// transitions 10969584
// state creations 35340972
// state copies    6933565
// state merges    37476276
// ../bin/dfaminhash: 1189.34 s CPU time

// Avec batch buffer 100 Mo
// jeu jun  5 01:58:35 CEST 2008 "../bin/dfaminhash -m yes -v "
// done 8500000
// done inserting 8507856 lines
// word count 7094826
// states 4288353
// transitions 9646042
// state creations 30741040
// state copies    5916649
// state merges    32369336
// ../bin/dfaminhash: 565.89 s CPU time

struct string_cmp : public binary_function<const char*, const char*, bool>
{
  bool operator()(const char *x, const char *y) const {
    return strcmp(x, y) < 0;
  }
};

int main(int argc, char** argv)
{
  config c(argc, argv, 1);
  DFA_min_hash<> dfa;
  string word;
  int wc;

#if BATCH

  if (c.verbose_mode)
    cerr << "batch mode size " << BATCH << " bytes" << endl;
  const int batch_size = BATCH;
  char *buffer = new char[batch_size / 2];
  char *end = buffer + batch_size / 2;
  vector<char*> index;
  char *p = buffer;

  for(wc = 0, getline(cin,word); !cin.eof(); getline(cin,word)) {
    if (p + word.size() + 1 > end) {
      std::sort(index.begin(), index.end(), string_cmp());
      for(vector<char*>::iterator i = index.begin(); i != index.end(); ++i, ++wc) {
	dfa.insert(*i);
	if (c.verbose_mode && wc % 100000 == 0)
	  cerr << "done " << wc << '\r' << flush;
      }
      index.clear();
      p = buffer;
    }
    strcpy(p, word.c_str());
    index.push_back(p);
    p += word.size() + 1;
  }

  // flush:
  std::sort(index.begin(), index.end(), string_cmp());
  for(vector<char*>::iterator i = index.begin(); i != index.end(); ++i, ++wc) {
    dfa.insert(*i);
    if (c.verbose_mode && wc % 100000 == 0)
      cerr << "done " << wc << '\r' << flush;
  }
 
  delete [] buffer;
  index.clear();

#else

  for(wc = 0, getline(cin,word); !cin.eof(); getline(cin,word)) {
    ++wc;
    dfa.insert(word);
    if (c.verbose_mode && wc % 100000 == 0)
      cerr << "done " << wc << '\r' << flush;
  }

#endif

  if (c.verbose_mode) {
    cerr << endl << "done inserting " << wc << " lines" << endl;
    dfa.stats(cerr);
  }

//   DFA_stream<hash_tag> out(cout);
//   if (c.state_mark)
//     clone(out, dfirst_markc(dfa));
//   else
//     clone(out, dfirstc(dfa));
  return 0;
}
