#include <astl.h>
#include <vector>
#include <string>
// #include <regexp.h>
#include <lazy.h>
#include <iostream>

#include <capture.h>

using namespace std;
using namespace astl;

// typedef lazy_cursor<regexp_cursor, DFA_matrix_mini> cursor_type;
typedef lazy_cursor<regexp_capture_cursor, DFT_matrix_mini> cursor_type;
const bool USE_HORSPOOL = true;

cursor_type build_regexp(const string &exp, bool use_horspool) {
  //  return lazy_cursor<regexp_cursor, DFA_matrix_mini>(regexp_cursor(exp, use_horspool));
  return lazy_cursor<regexp_capture_cursor, DFT_matrix_mini>(regexp_capture_cursor(exp, use_horspool));
}

pair<string::const_iterator, string::const_iterator> trim(const string &s) {
  pair<string::const_iterator, string::const_iterator> p(s.begin(), s.end());
  if (!s.empty()) {
    for(; p.first < p.second && (*p.first == ' ' || *p.first == '\t' || *p.first == '\n' || *p.first == '\r'); ++p.first);
    for(--p.second; p.second > p.first && (*p.second == ' ' || *p.second == '\t' || *p.first == '\n' || *p.first == '\r'); --p.second);
    ++p.second;
  }
  return p;
}

bool lookup(string::const_iterator &first, string::const_iterator last, cursor_type &c) {

  cursor_type::state_type last_valid = c.src();
  if (c.adaptee().horspool->size() > 0) {
    const horspool_finder &hs = *c.adaptee().horspool;
    const cursor_type::state_type i = c.src();
    for(first = hs.find(first, last); first < last; first = hs.find(first, last)) {
      c = i;
      first += hs.size();
      string::const_iterator start;
      for(start = first; start < last && !c.src_final() && c.forward(*start); ++start)
	last_valid = c.src();
      if (!c.sink() && (c.src_final() || (start == last && c.forward('\0') && c.src_final()))) 
	return true;
    }
  }
  else {
    if (c.forward('\0')) { // ^ anchoring
      last_valid = c.src();
      for(const string::const_iterator start = first; first != last && c.forward(*first); ++first) {
	last_valid = c.src();
	if (c.src_final()) {
	  c = last_valid;
	  return true;
	}
      }
      if (first == last && c.forward('\0'))
	return c.src_final();
    }
  }
  c = last_valid;
  return false;
}

const string bold = "\033[1m";
const string normal = "\033[0m";
const string red = "\033[47;31m";
const string underline = "\033[4m";
const string green = "\033[47;32m";
const string blue = "\033[47;34m";

int main(int argc, const char **argv) {
  const bool verbose = argc == 2 && argv[1] == string("-v");
  vector<string> fields, text;
  int counter = 0, success = 0, line_count = 0;
  for(string line; getline(cin, line); ) {
    ++line_count;
    cut(line, fields);
    // trim expression
    pair<string::const_iterator, string::const_iterator> p = trim(fields[1]);
    fields[1] = string(p.first, p.second);
    cursor_type c = build_regexp(fields[0], USE_HORSPOOL);
    if (verbose) {
      cout << line_count << '\t' << fields[1] << endl << "\t" << bold << fields[0] << normal << endl;
      cout << '\t' << c.adaptee() << endl;
    }
    if (c.sink()) {
      cout << line_count << '\t' << fields[1] << endl << "\t" << bold << fields[0] << normal << endl;
      cout << "\tPARSE ERROR\t" << c.adaptee().error_message() << " at character " 
	   << c.adaptee().error() << " line " << line_count << endl;
      return 1;
    }

    cursor_type::state_type i = c.src();

    // test matching
    cut(fields[2], text, '|');
    bool failed = false;
    for(vector<string>::iterator t = text.begin(); t != text.end(); ++t) {
      ++counter;
      pair<string::const_iterator, string::const_iterator> p = trim(*t);
      c = i;
      string::const_iterator start = p.first, last = p.second;
      if (lookup(p.first, p.second, c)) {
	++success;
	if (verbose) {
	  unsigned long Q = 0, T = 0;
// 	  if (t == text.begin()) {
// 	    DFA_matrix_mini<> dfa;
// 	    regexp_cursor(fields[0]).determinize(dfa);
// 	    Q = dfa.state_count();
// 	    T = dfa.trans_count();
// 	  }
	  cout << '\t' << blue + bold << "SUCCESSFUL MATCH" << normal << '\t';
	  cout << '\t' << green + bold << std::string(start, last) << normal
	       << "\t\tcache states " << c.cache().state_count();
// 	  if (t == text.begin())
// 	    cout << '/' << Q;
	  cout << "\tcache transitions " << c.cache().trans_count();
// 	  if (t == text.begin())
// 	    cout << '/' << T;
	  cout << endl;
	}
      }
      else {
	if (!verbose)
	  cout << line_count << '\t' << fields[1] << endl << "\t\"" << bold << fields[0] << normal << '"' << endl;
	cout << '\t' << red << "FAIL TO MATCH" << normal;
	cout << "\t" << green + bold << std::string(start, p.first) << normal << red << std::string(p.first, last) << normal;
	cout << "\t\"";
	cout << c.adaptee() << '"' << endl;
	failed = true;
      }
    }

    // test non-matching
    if (fields.size() > 3 && !fields[3].empty()) {
      cut(fields[3], text, '|');
      for(vector<string>::iterator t = text.begin(); t != text.end(); ++t) {
	++counter;
	pair<string::const_iterator, string::const_iterator> p = trim(*t);
	c = i;
	string::const_iterator start = p.first, last = p.second;
	if (lookup(p.first, p.second, c)) {
	  cout << line_count << '\t' << fields[1] << endl << "\t\"" << bold << fields[0] << normal << '"' << endl;
	  cout << '\t' << red << "SHOULD NOT MATCH" << normal;
	  cout << "\t" << std::string(start, last);
	  cout << "\t\"";
	  c = i;
	  cout << c.adaptee() << '"' << endl;
	  failed = true;
	}
	else {
	  ++success;
	  if (verbose) {
	    cout << '\t' << blue + bold << "SUCCESSFUL NON-MATCH" << normal << '\t';
	    cout << '\t' << green + bold << std::string(start, last) << normal
		 << "\t\tstates " << c.cache().state_count()
		 << "\ttransitions " << c.cache().trans_count() << endl;
	  }
	}
      }
    }

    if (verbose || failed)
      cout << '\t' << string(70, '_') << endl << endl;
  }
  cout << "success " << success << '/' << counter << " (" << ((double) success / counter) * 100 << "%)" << endl;
  return success == counter ? 0 : 1;
}
