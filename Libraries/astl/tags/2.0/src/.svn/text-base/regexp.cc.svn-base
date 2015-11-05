#include <astl.h>
#include <regexp.h>
#include <stream.h>
#include <ccopy.h>
#include <string>
#include <iostream>
#include <tools.h>
#include <match.h>
#include <ctime>
#include <set_operation.h>

#define TEMPLATE_TEMPLATE_IMPLEMENTATION
#include <lazy.h>

using namespace std;
using namespace astl;

#if !defined(__GNUG__) || __GNUG__ >= 3
using namespace rel_ops;
#endif

const string USAGE = "[-s] [-v] [-c] [-nl] [-g] [-dfa] expression file";

void usage(const char* exe)
{
  cerr << "Usage: " << exe << " " << USAGE << endl;
  cerr << "       -v\tverbose mode" << endl;
  cerr << "       -c\tmatch count" << endl;
  cerr << "       -nl\tno lazy-construction optimization" << endl;
  cerr << "       -g\tgrep style (line mode)" << endl;
  cerr << "       -dfa\tdump dfa" << endl;
  cerr << "       -s\tsplit match according to subexpressions" << endl;
  exit(1);
}

int main(int argc, char** argv)
{
  if (argc < 3 || argc > 6) usage(argv[0]);
  bool verbose = false, no_lazy = false;
  bool grep = false, dump_dfa = false;
  bool match_count = false, split = false;
  string tag = "";
  int i;
  int c = argc; 

  for(i = 1; c > 3; --c, ++i)
    if (string("-s") == argv[i]) split = true;
    else
      if (string("-v") == argv[i]) verbose = true;
      else
	if (string("-nl") == argv[i]) no_lazy = true;
	else
	  if (string("-g") == argv[i]) grep = true;
	  else
	    if (string("-dfa") == argv[i]) dump_dfa = true;
	    else
	      if (string("-c") == argv[i]) match_count = true;
	      else
		usage(argv[0]);
    
  if (verbose) {
    char buffer[1024];
    FILE *f = POPEN("date", "r");
    buffer[max((int) fread(buffer, sizeof(char), 1023, f) - 1, 0)] = '\0';
    PCLOSE(f);
    cerr << buffer << " \"";
    copy(argv, argv + argc, ostream_iterator<char*>(cerr, " "));
    cerr << "\"" << endl;
  }

  FILE *f = fopen(argv[i + 1], "rb");
  if (f == NULL) {
    string msg = string(argv[0]) + ": " + argv[i + 1];
    perror(msg.c_str());
    exit(2);
  }
  frandom_iterator<> first(f), last, input;
  ostream_iterator<char> out(cout);

  if (no_lazy) {
    regexp_cursor rc(argv[i], false);
    if (rc.sink()) {
      cerr << argv[0] << ": error in expression at char " << rc.errpos << endl;
      cerr << argv[i] << endl;
      for(int k = 0; k++ < rc.errpos; cerr << " ");
      cerr << "^" << endl;
      exit(3);
    }
    regexp_cursor::state_type initial = rc.src();
    int count = 0;

    while (first != last) {
      input = first_match(rc, first, last);
      if (input == first) ++input;
      else
      if (match_count) ++count;
      else {
	cout << tag;
	copy(first, input, out);
	cout << tag << endl;
      }
      first = input;
      rc = initial;
    }

    if (match_count)
      cout << count << endl;

  }
  else
    if (grep) {
      regexp_cursor rg(argv[i]);   // KMP FA recognizing Sigma*expression
      lazy_cursor<regexp_cursor> rc(rg);
      lazy_cursor<regexp_cursor>::state_type initial = rc.src();
      int count = 0;

      for(input = first; first != last; )
	if (*first == '\n') { 
	  input = ++first;
	  rc = initial;
	}
	else {
	  rc.forward(*first);
	  if (rc.src_final())
	    if (match_count) {
	      ++count;
	      for(; first != last && *first != '\n'; ++first);
	      for(; first != last && *first == '\n'; ++first);
	      rc = initial;
	    }
	    else {
	      copy(input, ++first, out);
	      for(; first != last; ++first)
		if (*first == '\n') {
		  cout << endl;
		  input = ++first;
		  rc = initial;
		  break;
		}
		else cout << *first;
	    }
	  else ++first;
	}

      if (match_count)
	cout << count << endl;

      if (verbose) 
	cerr << "DFA has " << rc.cache().state_count() << " states and " 
	     << rc.cache().trans_count() << " transitions" << endl;
      
      if (dump_dfa) {
	dump(cerr, dfirst_markc(rc.cache()));
      }
    }
    else  // not grep mode
    {
      int count = 0;
      if (!split) {
	regexp_cursor rg(argv[i]);
	lazy_cursor<regexp_cursor> rc(rg);
	lazy_cursor<regexp_cursor>::state_type initial = rc.src();
	while (first != last) {
	  input = longest_match(rc, first, last);
	  if (input == first) ++input;
	  else
	    if (match_count) ++count;
	    else {
	      cout << tag;
	      copy(first, input, out);
	      cout << tag << endl;
	    }
	  first = input;
	  rc = initial;
	}

	if (match_count)
	  cout << count << endl;

	if (dump_dfa) {
	  dump(cerr, dfirst_markc(rc.cache()));
	}

	if (verbose) 
	  cerr << "DFA has " << rc.cache().state_count() << " states and " 
	       << rc.cache().trans_count() << " transitions" << endl;
      }
      else { // split mode
	extract_cursor rg(argv[i], false, verbose);
	lazy_cursor_tag<extract_cursor> rc(rg);
	lazy_cursor_tag<extract_cursor>::state_type initial = rc.src();
	vector<pair<int, int> > submatch;
	submatch.reserve(10);
	int subexp_count = rg.subexp_count();

	for(input = first; first != last; input = first, rc = initial) {
	  submatch.clear();
	  for(rc = initial; input != last && rc.forward(*input); ++input)
	    if (rc.src_final()) submatch = rc.src_tag();
	  if (!submatch.empty()) { // any match ?
	    if (match_count) ++count;
	    else {
	      ostream_iterator<char> out(cout);
	      cout << "match: '";
	      copy(first + submatch[0].first, 
		   first + submatch[0].second, out);
	      cout << '\'' << endl;
	      for(int i = 1; i < subexp_count; ++i) 
		if (submatch[i].first != -1) {
		  cout << "     " << i << ": ";
		  copy(first + submatch[i].first, 
		       first + submatch[i].second, out);
		  cout << endl;
		}
	    }
	    first += submatch[0].second;
	  }
	  else 
	    ++first;
	}
	if (match_count)
	  cout << count << endl; 

	if (dump_dfa) {
	  dump(cerr, dfirst_markc(rc.cache()));
	}

     	if (verbose) 
	  cerr << "DFA has " << rc.cache().state_count() << " states and " 
	       << rc.cache().trans_count() << " transitions" << endl;
      }
    }
      
  if (verbose)
    cerr << argv[0] << ": " 
	 << clock() / (double) CLOCKS_PER_SEC << " s CPU time" << endl;

  fclose(f);
  exit(0);
}
  
  
