#include <astl.h>
#include <vector>
#include <string>
#include <lazy.h>
#include <iostream>
#include "capture.h"
#include "experiment2.h"

using namespace std;
using namespace astl;

const bool USE_HORSPOOL = true;

// typedef lazy_cursor<capture_cursor, DFT_matrix_mini> capturer_type;
typedef capture_cursor capturer_type;
typedef astl::non_deterministic_regexp_capture_cursor non_deterministic_capturer_type;

capturer_type build_capturer(const string &exp, bool use_horspool) {
  return capturer_type(capture_cursor(exp, use_horspool));
}

non_deterministic_capturer_type build_non_deterministic_capturer(const string &exp, bool use_horspool) {
  return non_deterministic_capturer_type(exp, use_horspool);
}

pair<string::const_iterator, string::const_iterator> trim_range(const string &s) {
  pair<string::const_iterator, string::const_iterator> p(s.begin(), s.end());
  if (!s.empty()) {
    for(; p.first < p.second && (*p.first == ' ' || *p.first == '\t' || *p.first == '\n' || *p.first == '\r'); ++p.first);
    for(--p.second; p.second > p.first && (*p.second == ' ' || *p.second == '\t' || *p.first == '\n' || *p.first == '\r'); --p.second);
    ++p.second;
  }
  return p;
}

string trim(const string &s) {
  pair<string::const_iterator, string::const_iterator> p = trim_range(s);
  return string(p.first, p.second);
}

template <typename RandomAccessI>
bool capture(RandomAccessI &first, RandomAccessI last, 
	     non_deterministic_capturer_type &c, 
	     std::vector<RandomAccessI> &captures) {
  const RandomAccessI start = first;
  while(first != last && c.forward(*first)) {
    ++first;
  }
  if (first == last && c.src_final()) {
    c.forward('\0'); // finalize capturing
    ++first; // offsets will be given from this position
    const non_deterministic_capturer_type::configuration &conf = c.src_tag();
    if (!conf.empty()) {
      for(non_deterministic_capturer_type::configuration::const_iterator i = conf.begin(); i != conf.end(); ++i) {
	cout << "captures[" << i->first.id() << "] displacement " << i->second << " (" << (first - start + i->second) << ')' << endl;
	captures[i->first.id()] = first + i->second; // add capture offset
      }
    }
    --first;
    return true;
  }
  return false;
}

template <typename RandomAccessI>
bool capture(RandomAccessI &first, RandomAccessI last, 
	     capturer_type &c, 
	     std::vector<RandomAccessI> &captures) {
  if (c.forward('\0')) {
    const capturer_type::configuration &conf = c.src_tag();
    if (!conf.empty()) {
      for(capturer_type::configuration::const_iterator i = conf.begin(); i != conf.end(); ++i) {
	cout << "captures[" << i->first.id() << "] displacement " << i->second << " (" << (i->second) << ')' << endl;
	captures[i->first.id()] = max(first, first + i->second);
      }
    }
    const RandomAccessI start = first;
    while(first != last && c.forward(*first)) {
      ++first;
      const capturer_type::configuration &conf = c.src_tag();
      if (!conf.empty()) {
	for(capturer_type::configuration::const_iterator i = conf.begin(); i != conf.end(); ++i) {
	  cout << "captures[" << i->first.id() << "] displacement " << i->second << " (" << (first - start + i->second) << ')' << endl;
	  captures[i->first.id()] = max(start, first + i->second);
	}
      }
    }
    if (first == last) {
      if (!c.src_final()) {
	if (c.forward('\0') && c.src_final()) {
	  ++first;
	  const capturer_type::configuration &conf = c.src_tag();
	  if (!conf.empty()) {
	    for(capturer_type::configuration::const_iterator i = conf.begin(); i != conf.end(); ++i) {
	      cout << "captures[" << i->first.id() << "] displacement " << i->second << " (" << (first - start + i->second) << ')' << endl;
	      captures[i->first.id()] = max(start, min(last, first + i->second));
	    }
	  }
	}
	else return false;
      }
      c.finalize();
      //c.forward('\0'); // finalize capturing
      ++first; // offsets will be given from this position
      const capturer_type::configuration &conf = c.src_tag();
      if (!conf.empty()) {
	for(capturer_type::configuration::const_iterator i = conf.begin(); i != conf.end(); ++i) {
	  cout << "captures[" << i->first.id() << "] displacement " << i->second << " (" << (first - start + i->second) << ')' << endl;
	  captures[i->first.id()] = max(start, min(last, first + i->second));
	}
      }
      --first;
      return true;
    }
  }
  return false;
}

void display(ostream &out, const vector<string::const_iterator> &captures, const std::string &text) {
  for(string::const_iterator c = text.begin(); c != text.end(); ++c) {
    for(vector<string::const_iterator>::const_iterator cap = captures.begin(); cap < captures.end(); ++cap)
      if (*cap == c) 
	out << ((cap - captures.begin()) % 2 == 0 ? '[' : ']');
    out << *c;
  }
  for(vector<string::const_iterator>::const_iterator cap = captures.begin(); cap < captures.end(); ++cap)
    if (*cap == text.end()) 	
      out << ((cap - captures.begin()) % 2 == 0 ? '[' : ']');
}

#if 0
bool lookup(string::const_iterator &first, string::const_iterator last, capturer_type &c) {
  capturer_type::state_type last_valid = c.src();
  if (c.adaptee().horspool->size() > 0) {
    const horspool_finder &hs = *c.adaptee().horspool;
    const capturer_type::state_type i = c.src();
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
#endif

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
    fields[0] = trim(fields[0]);
    capturer_type c = build_capturer(fields[0], USE_HORSPOOL);

    if (verbose) {
      cout << line_count << '\t' << fields[1] << endl << "\t" << bold << fields[0] << normal << endl;
      cout << '\t' << c /*.adaptee()*/ << endl;
    }
    if (c.sink()) {
      cout << line_count << '\t' << fields[1] << endl << "\t" << bold << fields[0] << normal << endl;
      cout << "\tPARSE ERROR\t" << c /*.adaptee()*/.error_message() << " at character " 
	   << c /*.adaptee()*/ .error() << " line " << line_count << endl;
      return 1;
    }

    non_deterministic_capturer_type::state_type i = c.src();
    // for each test:
    for(vector<string>::iterator test = fields.begin() + 2; test < fields.end(); test += 2) {
      const string bogus;
      const string text = trim(test[0]);
      const string reference = "[" + trim(test[1]) + ']';
      // build reference
      vector<string::const_iterator> reference_captures(c.capture_count(), bogus.end());
      unsigned long position = 0;
      int current_open_capture = 0;
      vector<int> open_capture_stack;
      for(string::const_iterator cc = reference.begin(); cc != reference.end(); ++cc) {
	switch (*cc) {
	case '[' :
	  reference_captures[current_open_capture] = text.begin() + position;
	  open_capture_stack.push_back(current_open_capture);
	  current_open_capture += 2;
	  break;
	case ']' :
	  reference_captures[open_capture_stack.back() + 1] = text.begin() + position;
	  open_capture_stack.pop_back();
	  break;
	default :
	  ++position;
	  break;
	}
      }

      ++counter;
      c = i;
      vector<string::const_iterator> captures(c.capture_count(), bogus.end());
      bool failed = false;
      string::const_iterator first = text.begin();
      if (capture(first, text.end(), c, captures)) {
	bool ok = captures == reference_captures;
	if (!(ok || (captures.size() > reference_captures.size() && 
	    equal(reference_captures.begin(), reference_captures.end(), captures.begin()) &&
	    count(captures.begin() + reference_captures.size(), captures.end(), bogus.end()) ==
		     captures.size() - reference_captures.size()))) {
	  
// 	  for(vector<string::const_iterator>::const_iterator cap = captures.begin(); cap != captures.end(); ++cap)
// 	    cout << (void*) &**cap << " ";
// 	  cout << endl;

// 	  for(vector<string::const_iterator>::const_iterator cap = configuration_reference.begin(); cap != configuration_reference.end(); ++cap)
// 	    cout << (void*) &**cap << " ";
// 	  cout << endl;

	  if (!verbose)
	    cout << line_count << '\t' << fields[1] << endl << "\t\"" << bold << fields[0] << normal << '"' << endl;
	  cout << '\t' << red << "WRONG CAPTURES (" << reference_captures.size() << ')' << normal;
	  cout << "\t" << green + bold << string(text.begin(), first) << normal << red 
	       << string(first, text.end()) << normal << '\t';
	  display(cout, captures, text);
	  cout << " SHOULD BE ";
	  display(cout, reference_captures, text);
	  cout << "\t" << c /*.adaptee()*/ << '"' << endl;
	  failed = true;
	}
	else {
	  ++success;
	  if (verbose) {
	    cout << '\t' << blue + bold << "SUCCESSFUL MATCH" << normal << '\t';
	    cout << '\t' << green + bold << string(text.begin(), text.end()) << normal << '\t';
	    display(cout, captures, text);
// 	    cout << "\t\tstates " << c.cache().state_count()
// 	    << "\ttransitions " << c.cache().trans_count();
	    cout << endl;
	  }
	}
      }
      else {
	if (!verbose)
	  cout << line_count << '\t' << fields[1] << endl << "\t\"" << bold << fields[0] << normal << '"' << endl;
	cout << '\t' << red << "FAILED TO MATCH" << normal;
	cout << "\t" << green + bold << string(text.begin(), first) << normal << red << string(first, text.end()) << normal;
	cout << "\t\"";
	cout << c /*.adaptee()*/ << '"' << endl;
	failed = true;
      }

      if (failed)
	cout << '\t' << string(70, '_') << endl << endl;
      //      else if (verbose)
      //	cout << endl << endl;
    }
  }
  cout << "success " << success << '/' << counter << " (" << ((double) success / counter) * 100 << "%)" << endl;
  return success == counter ? 0 : 1;
}

