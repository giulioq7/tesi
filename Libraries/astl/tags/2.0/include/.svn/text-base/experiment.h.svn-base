/*
 * ASTL - the Automaton Standard Template Library.
 * C++ generic components for Finite State Automata handling.
 * Copyright (C) 2000-2003 Vincent Le Maout (vincent.lemaout@chello.fr).
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef ASTL_EXPERIMENT_H
#define ASTL_EXPERIMENT_H

//
// ** STILL UNDER CONSTRUCTION **
// USE AT YOUR OWN RISKS (THAT IS, DON'T USE THIS :-)
//

#include "astl.h"
#include "lazy.h"
#include <vector>
#include <iostream>
#include <set>
#include <iterator>
#include <cctype>  
#include <bitset>
#include <cstring>
#include <sstream>



#include <cassert>

#include <tr1/unordered_map>


// TODO/FIXME
// - determinization of sets of capture along the path. The method src_tag()
//   will return something useful for the upper layer to handle the offsets
//   of captures (an adapter capture_cursor could encapsulate either a 
//   regexp_capture_cursor or a lazy_cursor<regexp_capture_cursor> ?)
// - horspool optimization mixed with captures (?)
// - better implementation for the DFT_matrix_mini ?

// REFACTORING/GENERALIZATION
// - externalize tokenizer
// - externalize parser
// - externalize the node processing so that it can process a tree directly
// - templatize nodes so that any alphabet can be used
// => make node an abstract type and use visitors

// - make the parser be able to parse a regexp on any alphabet type (?)
// - merge regexp_cursor and capture_cursor since we need a capture for general
//   case when the expression is not left-anchored (need to know where the
//   match begins). Must not pay the price of the captures if none are present
//   in the expression.

// - can we manage greediness with this kind of processing ?
// - can we manage {n,m} repetitions with this kind of processing ?
// - can we manage approximative regexp matching ?
// - can we replace the neighbor cursor with this ? (more efficient ?)
// - debug trace at each point where there is a tag in the regexp ?
// => generalize to markers in expression corresponding to an action to be taken
//    (say, increment repetition counter, choose between greedy/non greedy,
//    update a edit cost or whatever is needed). Trouble: geneiric determinization.

// OPTIMIZATIONS
// - generalize boyer-moore-horspool to a set of words (?) (Tom|Sawyer|Huckleberry|Finn)
// - generalize boyer-moore-horspool to sets of characters (?) ([Tt]wain)
// - look for an optimization for the case ^[^ ]*?Twain (xpressive is 3 times faster)

#ifdef USE_HASH_TABLE
#undef USE_HASH_TABLE
#endif

using namespace std;

namespace astl {

  // regular expressions grammar:
  // exp    -> term exp2
  // exp2   -> + term exp2 | epsilon
  // term   -> form repeat term2
  // term2  -> . form repeat term2 | epsilon
  // repeat -> * repeat | ? repeat | + repeat | {n,m} repeat | epsilon
  // form   -> ( exp ) | letter 
  //
  // + is union, . is concatenation, epsilon is the empty word
  // a letter is a set of chars

  class non_deterministic_regexp_capture_cursor : public cursor_concept
  {
  public:
    typedef enum { STAR, CONCAT, OR, REPEAT, EPSILON, 
		   LETTER, OPEN, CLOSE, QUESTION, PLUS,
		   ANY, OPENCAPTURE, CLOSECAPTURE, FINAL, GRANDFINAL } token_type;
  protected:  
    class node;

  public:
    class capture
    {
    public:
      capture(const node *n)
	: n(n)
      { }
      
      int        id()    const { return n->n; }
      int        index() const { return n->m; }
      token_type type()  const { return n->type; }

      bool operator<(const capture &c) const {
	return id() < c.id();
      }

      bool operator==(const capture &c) const {
	return n == c.n;
      }

    protected:
      const node *n;
    };

    class configuration;
    friend ostream& operator<<(ostream&, const configuration&);

    class configuration : public map<capture, long>
    {
    public:
      configuration& operator--() {
	for(iterator i = begin(); i != end(); ++i)
	  --(i->second);
	return *this;
      }


      bool operator<(const configuration &y) const {

	cout << "comparing " << *this << " and " << y << " == ";

	if (&y == this || empty()) 
	  {
	    cout << "false" << endl;
	    return false;
	  }

	const_iterator i, j;
	for(i = begin(), j = y.begin(); 
	    i != end() && j != y.end() && i->first.id() == j->first.id() && i->second == j->second; 
	    ++i, ++j);
	if (i == end() && j == y.end())
	  {
	    cout << "false" << endl;
	    return false;
	  }

	// check validity of this:
	if (i == end())
	  {
	    cout << "false" << endl;
	    return false;
	  }

	// check validity of this:
	if (j == y.end())
	  {
	    cout << "true" << endl;
	    return true;
	  }

	// check validity of this:
	if (i->first.id() != j->first.id())
	  {
	    cout << (i->first.id() < j->first.id() ? "true" : "false") << endl;
	    return i->first.id() < j->first.id();
	  }

	// left-most longest: 
	// * left-most OPENCAPTURE are preferred,
	// * right-most CLOSECAPTURE are preferred 
	// * empty match is better than no match
	if (i->first.type() == OPENCAPTURE) // OPEN ?
	  {
	    cout << (i->second < j->second ? "true" : "false") << endl;
	    return i->second < j->second;
	  }
	cout << (j->second < i->second ? "true" : "false") << endl;
	return j->second < i->second;
      }


      configuration intersection(const configuration &x) const {
	cout << "intersection(" << *this << ", " << x << ") == ";
	if (empty()) {
	  cout << *this << endl;
	  return *this;
	}
	if (x.empty()) {
	  cout << x << endl;
	  return x;
	}
	configuration r;
	configuration::const_iterator i = begin(), j = x.begin();
	while (i != end() && j != x.end()) {
	  if (i->first.id() < j->first.id()) {
	    ++i;
	  }
	  else if (j->first.id() < i->first.id()) {
	    ++j;
	  }
	  else {
	    if (i->second == j->second) {
	      r.insert(*j);
	    }
	    ++j;
	    ++i;
	  }
	}
	cout << r << endl;
	return r;
      }

    };

  protected:
    typedef node* position;
    typedef map<position, configuration> S;

    struct node {
      bitset<256> letter;
      token_type type;
      short n, m; // repeat operator
      bool nullable;
      S firstpos, lastpos, nextpos;
      configuration emptymatch;
      // configuration conf;
    
      node(token_type t, short n_ = 0, short m_ = 0)
	: type(t), n(n_), m(m_), nullable(false) { 
	switch (type) {
	case FINAL :
	  letter.set(0);
	  break;
	case ANY :
	  type = LETTER;
	  letter.set();



	  // letter.set(0, false); // don't match '\0'



	  break;
	case OPENCAPTURE :
	case CLOSECAPTURE :
	  nullable = true;
	  break;
	default :;
	}
      }

      node(char t)
	: type(LETTER), nullable(false) { 
	letter.set((unsigned char) t);
      }

      node(const bitset<256> &t)
	: letter(t), type(LETTER), nullable(false)
      { }

      bool final() const {
	return type == FINAL;
      }

      char to_char() const {   // does not work with ranges
	size_t i;
	for(i = 0; i != letter.size() && letter.test(i) == false; ++i);
	return (char) (-128 + i);
      }

      string pretty() const {
	ostringstream r;
	r << "[ " << *this;
	for(S::const_iterator i = nextpos.begin(); i != nextpos.end(); ++i) {
	  r << ", {";
	  for(configuration::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
	    r << ' ' << j->first.id();
	  r << " }";
	}
	r << " ]";
	return r.str();
      }
    };

  protected:
    S q;  // current state
    smart_ptr<vector<node> > e;  // node "allocator"
    const char *errmsg;
    smart_ptr<S> I;
    unsigned char capture_count_;
    configuration intersection;

  public:
    smart_ptr<horspool_finder> horspool;

  protected:
    friend ostream& operator<<(ostream&, const non_deterministic_regexp_capture_cursor&);

    node* exp(vector<node>::iterator &first, vector<node>::iterator last);
    node* form(vector<node>::iterator &first, vector<node>::iterator last);
    node* term(vector<node>::iterator &first, vector<node>::iterator last);
    node* exp2(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    node* term2(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    node* repeat(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    void preprocess_repeats();
    void build_char_sets(map<string, bitset<256> >&);
    template <typename OutputIterator>
    int tokenize(const char *first, const char *last, OutputIterator x);

  public:
    friend ostream& operator<<(ostream &, const node&);
    ptrdiff_t errpos;

    typedef non_deterministic_regexp_capture_cursor    self;
    typedef S                state_type;
    typedef set<node*>       ordered_state_type;
    typedef plain            char_traits;
    typedef char_traits::char_type char_type;
    typedef configuration    tag_type;

    non_deterministic_regexp_capture_cursor(const string &expression = "", bool horspool_optimized = false)
      : errmsg(NULL), capture_count_(0), errpos(-1)
    {
      if (!expression.empty()) {

	// bourinnage a l'ancienne:
	const string enhanced = string(".*(") + expression + ')';

	e->reserve(128);
// 	errpos = tokenize(expression.c_str(), 
// 			  expression.c_str() + expression.size(), 
// 			  back_inserter(*e));
	errpos = tokenize(enhanced.c_str(), 
			  enhanced.c_str() + enhanced.size(), 
			  back_inserter(*e));
	if (errpos == -1) {
	  preprocess_repeats();
	  vector<node>::iterator first = e->begin();
	  node *root = exp(first, e->end());

	  // managed non-anchored alternatives
	  if (root != NULL) {
	    q = root->firstpos;
#if 0
	    for(S::iterator i = q.begin(); i != q.end(); ++i)
	      // if not beginning of line (anchor ^)
	      if ((*i).first->letter.count() == 256 || (*i).first->letter.test(0) == false)
		I->insert(make_pair(i->first, i->second));
#endif
	  }
	  else errpos = expression.size() - errpos;
	}
      }
    }

    ptrdiff_t error() const { // possible error position (-1 if expression is ok)
      return errpos;
    }
    
    const char* error_message() const { return errmsg; }

    self& operator=(const state_type &s) {
      q = s;
      return *this;
    }

    const state_type& src() const { return q; }

    bool exists(char a) const {
      for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	if ((*i).first->letter.test((unsigned char) a) == true)
	  return true;
      return false;
    }

    int capture_count() const { return capture_count_; }

    bool forward(char letter) {
      state_type p; // = *I; // for non-anchored expressions
      cout << string(80, '_') << endl << "forward(" << (int) (unsigned char) letter << ")\tI == ";
      for(S::const_iterator i = I->begin(); i != I->end(); ++i) {
	cout << '\t' << i->first << ':' << i->first->pretty() << ":=" << i->second;
      }
      cout << endl;
      // q.insert(I->begin(), I->end());
      for(S::const_iterator i = q.begin(); i != q.end(); ++i) { // for each (position, configuration)
	cout <<  "\033[1m" << "position: " << i->first << ':' << i->first->pretty() 
	     << ":=" << i->second << "\033[0m" << endl;
	if (i->first->letter.test((unsigned char) letter) == true) {
	  state_type::const_iterator target = i->first->nextpos.begin();
	  const state_type::const_iterator last = i->first->nextpos.end();
	  for(; target != last; ++target) { // for each target (position, configuration)
	    configuration tmp = i->second;
	    --tmp; // shift configuration offsets
	    // union of configurations:
	    configuration::const_iterator cfirst = target->second.begin(), clast = target->second.end();
	    for(; cfirst != clast; ++cfirst) {
// 	      configuration::const_iterator ci = tmp.find(cfirst->first);
// 	      if (ci == tmp.end() || cfirst->first.type() == CLOSECAPTURE)
		tmp[cfirst->first] = 0; // update offset
	    }
	    configuration &conf = p[target->first];
            cout << "configurations dedup (one/position): ";
	    if (tmp < conf) conf = tmp; // dedup of configurations (only one per position)
	  }
	}
      }
      q.swap(p);
      //      q.insert(I->begin(), I->end());
      return !q.empty();
    }

    bool src_final() const {
      for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	if (i->first->final()) return true;
      return false;
    }
     
  bool sink() const { return q.empty(); }
 
  const tag_type& src_tag() const { 
    // return "minimal" configuration
    S::const_iterator m = q.end();
    for(S::const_iterator i = q.begin(); i != q.end(); ++i)
      if (m == q.end() || i->second < m->second)
	m = i;
    return m->second;
  }

    void clear() {
      q.clear();
      e->clear();
      I->clear();
    }

//     void determinize(DFA_matrix<plain> &dfa) const {
//       typedef DFA_matrix<plain> dfa_type;
//       map<ordered_state_type, dfa_type::state_type> old2new;
//       dfa_type::state_type i = dfa.new_state();
//       dfa.initial(i);
//       determinize_(dfa, old2new, old2new.insert(make_pair(ordered_state_type(q.begin(), q.end()), i)).first);
//     }

//     void determinize_(DFA_matrix<plain> &dfa, 
// 		      map<ordered_state_type, DFA_matrix<plain>::state_type> &old2new,
// 		      map<ordered_state_type, DFA_matrix<plain>::state_type>::iterator p) const {
//       for(int i = -128; i < 128; ++i) {
// 	ordered_state_type aim(I->begin(), I->end());    
// 	for(ordered_state_type::iterator s = p->first.begin(); s != p->first.end(); ++s) {
// 	  if ((*s)->letter.test((unsigned char) i)) {
// 	    aim.insert((*s)->nextpos.begin(), (*s)->nextpos.end());
// 	  }
// 	}
// 	if (!aim.empty()) {
// 	  pair<map<ordered_state_type, DFA_matrix<plain>::state_type>::iterator, bool> j = 
// 	    old2new.insert(make_pair(aim, DFA_matrix<plain>::null_state));
// 	  if (j.second == false) {
// 	    dfa.set_trans(p->second, (char) (unsigned char) i, j.first->second);
// 	  }
// 	  else {
// 	    j.first->second = dfa.new_state();
// 	    dfa.set_trans(p->second, (char) (unsigned char) i, j.first->second);
// 	    for(ordered_state_type::const_iterator ii = aim.begin(); ii != aim.end(); ++ii)
// 	      if ((*ii)->final()) {
// 		dfa.final(j.first->second) = true;
// 		break;
// 	      }
// 	    aim.clear();
// 	    determinize_(dfa, old2new, j.first);
// 	  }
// 	}
//       }
//     }

  protected:
    ptrdiff_t token_error(ptrdiff_t pos, const char *message) {
      errmsg = message;
      errpos = pos;
      return pos;
    }

    node* syntax_error(ptrdiff_t pos, const char *message = NULL) {
      errmsg = message;
      errpos = pos;
      return NULL;
    }

    int next_char(bitset<256>& letter, const char *begin, const char*& first, const char *last, 
		  bool& literal, const map<string, bitset<256> >& char_sets, bool icase);

  };

  inline
  ostream& operator<<(ostream &out, const non_deterministic_regexp_capture_cursor::configuration &c) {
    for(non_deterministic_regexp_capture_cursor::configuration::const_iterator i = c.begin();
	i != c.end(); ++i)
      out << i->first.id() << ':' << i->second << ' ';
    return out;
  }

  inline
  void non_deterministic_regexp_capture_cursor::preprocess_repeats() {
    vector<node> &v = *e;
    for(vector<node>::size_type p = 0; p < v.size();) {
      if (v[p].type != REPEAT) ++p;
      else {
	// look for what is repeated: letter, (exp), ?, +, *, ANY 
	vector<node>::size_type first = p - 1, last = p;
	for(; first > 0; --first) {
	  if (v[first].type == LETTER || v[first].type == CLOSE || v[first].type == ANY)
	    break;
	}
	if (v[first].type == CLOSE) {
	  int count = 1;
	  while(count != 0 && --first > 0)
	    if (v[first].type == CLOSE) ++count;
	    else if (v[first].type == OPEN) --count;
	}
	// the range [first, last) contains the pattern to be repeated
	// enclose it in parenthesis because of such things: a{0,2}{3}
	// which would otherwise be rewritten: a?a?{3} but shoud be written: (a?a?){3}
	// limiting to 128
	short n = (v[p].n > 128 ? 128 : v[p].n), 
	  m = (v[p].m > 128 ? 128 : v[p].m);
	v.insert(v.begin() + first, node(OPEN));
	++p;
	++last;
	++first;
	// remove repeat node
	if (n == 0)
	  v[p] = node(QUESTION);
	else
	  v.erase(v.begin() + p);
	vector<node> tmp;
	if (m == -1)
	  tmp.reserve((last + 1 - first) * n + 1);
	else
	  tmp.reserve((last + 1 - first) * m + m - n + 1);

	while(n > 0 || m > 0) {
	  --n; 
	  --m;
	  if (n <= 0) { // end of mandatory repeats ?
	    if (m < n) {  // case {n,}
	      tmp.push_back(node(PLUS));
	      continue; // end of processing
	    }
	    if (m == 0)
	      continue; // end of processing
	  }
	  tmp.push_back(node(CONCAT));
	  for(vector<node>::size_type tmp_p = first; tmp_p < last; ++tmp_p)
	    tmp.push_back(v[tmp_p]);
	  if (n <= 0)
	    tmp.push_back(node(QUESTION));
	}
	tmp.push_back(node(CLOSE));
	v.insert(v.begin() + p, tmp.begin(), tmp.end());
	p += tmp.size();
      }
    }
  }

  inline
  ostream& operator<<(ostream &out, const non_deterministic_regexp_capture_cursor::node &x) 
  {
    switch(x.type) {
    case non_deterministic_regexp_capture_cursor::LETTER :
//      if (x.letter.count() == 256)




      if (x.letter.count() == 256)




	out << ".";
      else {
	if (x.letter.count() > 1)
	  out << '[';
	int lower = 0;
	// rebuild approximatively the character set
	for(int i = 32; i < 257; ++i) {
	  if (i < 256 && x.letter.test(i) == true) {
	    if (lower == 0)
	      lower = i;
	  }
	  else {
	    if (lower > 0) {
	      if (i - lower == 1) {
		out << (unsigned char) lower;
	      }
	      else if (i - lower == 2) {
		out << (unsigned char) lower << (unsigned char) (lower + 1);
	      }
	      else out << (unsigned char) lower << '-' 
			<< (unsigned char) (i - 1);
	      lower = 0;
	    }
	  }
	}
	if (x.letter.count() > 1)
	  out << ']';
      }
      break;
	
    case non_deterministic_regexp_capture_cursor::OR : 
      out << "|"; 
      break;
	
    case non_deterministic_regexp_capture_cursor::FINAL : 
      out << "[EoE]";
      break;

    case non_deterministic_regexp_capture_cursor::GRANDFINAL : 
      out << "[GF]";
      break;
	
    case non_deterministic_regexp_capture_cursor::OPEN : 
      out << "("; 
      break;
	
    case non_deterministic_regexp_capture_cursor::CLOSE : 
      out << ")"; 
      break;
	
    case non_deterministic_regexp_capture_cursor::CONCAT : 
      out << " "; 
      break;
	
    case non_deterministic_regexp_capture_cursor::STAR : 
      out << "*"; 
      break;
	
    case non_deterministic_regexp_capture_cursor::QUESTION : 
      out << "?"; 
      break;
	
    case non_deterministic_regexp_capture_cursor::PLUS : 
      out << "+"; 
      break;
	
    case non_deterministic_regexp_capture_cursor::REPEAT : 
      out << "{" << x.n << "," << x.m << "}";
      break;
	
    case non_deterministic_regexp_capture_cursor::OPENCAPTURE :
      out << "_(" << x.n << "_";
      break;

    case non_deterministic_regexp_capture_cursor::CLOSECAPTURE :
      out << "_)" << x.n << "_";
      break;

    default :
      break;
    } 
    return out;
  }

  inline
  ostream& operator<<(ostream& out, const non_deterministic_regexp_capture_cursor& c) {
    for(vector<non_deterministic_regexp_capture_cursor::node>::const_iterator n = c.e->begin(); n != c.e->end(); ++n)
      if (c.q.find(const_cast<astl::non_deterministic_regexp_capture_cursor::node*>(&*n)) != c.q.end())
	out << "\033[1m" << *n << "\033[0m";
      else
	out << *n;
    return out;
  }

  inline
  void non_deterministic_regexp_capture_cursor::build_char_sets(map<string, bitset<256> >& char_sets) {
    bitset<256> alpha, alnum, ascii, blank, cntrl, digit, word;
    bitset<256> graph, lower, print, punct, space, upper, xdigit;
    for(int c = 0; c < 256; ++c) {
      alnum.set(c, isalnum(c) != 0);
      alpha.set(c, isalpha(c) != 0);
      ascii.set(c, isascii(c) != 0);
      cntrl.set(c, iscntrl(c) != 0);
      digit.set(c, isdigit(c) != 0);
      graph.set(c, isgraph(c) != 0);
      lower.set(c, islower(c) != 0);
      print.set(c, isprint(c) != 0);
      punct.set(c, ispunct(c) != 0);
      space.set(c, isspace(c) != 0);
      upper.set(c, isupper(c) != 0);
      xdigit.set(c, isxdigit(c) != 0);
    }
    char_sets["[:alnum:]"] = alnum;
    char_sets["[:^alnum:]"] = ~ char_sets["[:alnum:]"];
    char_sets["[:alpha:]"] = alpha;
    char_sets["[:^alpha:]"] = ~ char_sets["[:alpha:]"];
    char_sets["[:ascii:]"] = ascii;
    char_sets["[:^ascii:]"] = ~ char_sets["[:ascii:]"];
    blank.set((unsigned char) ' ');
    blank.set((unsigned char) '\t');
    char_sets["[:blank:]"] = blank;
    char_sets["[:^blank:]"] = ~ char_sets["[:blank:"];
    char_sets["[:cntrl:]"] = cntrl;
    char_sets["[:^cntrl:]"] = ~ char_sets["[:cntrl:]"];
    char_sets["[:digit:]"] = digit;
    char_sets["[:^digit:]"] = ~ char_sets["[:digit:]"];
    char_sets["[:graph:]"] = graph;
    char_sets["[:^graph:]"] = ~ char_sets["[:graph:]"];
    char_sets["[:lower:]"] = lower;
    char_sets["[:^lower:]"] = ~ char_sets["[:lower:]"];
    char_sets["[:print:]"] = print;
    char_sets["[:^print:]"] = ~ char_sets["[:print:]"];
    char_sets["[:punct:]"] = punct;
    char_sets["[:^punct:]"] = ~ char_sets["[:punct:]"];
    char_sets["[:space:]"] = space;
    char_sets["[:^space:]"] = ~ char_sets["[:space:]"];
    char_sets["[:upper:]"] = upper;
    char_sets["[:^upper:]"] = ~ char_sets["[:upper:]"];
    char_sets["[:xdigit:]"] = xdigit;
    char_sets["[:^xdigit:]"] = ~ char_sets["[:xdigit:]"];
    alnum.set((unsigned char) '_');
    char_sets["[:word:]"] = alnum;
    char_sets["[:^word:]"] = ~ char_sets["[:word:]"];
    char_sets["\\d"] = digit;
    char_sets["\\D"] = ~ char_sets["\\d"];
    char_sets["\\s"] = space;
    char_sets["\\S"] = ~ char_sets["\\s"];
    char_sets["\\w"] = alnum;
    char_sets["\\W"] = ~alnum;
    char_sets["\\t"] = bitset<256>().set((unsigned char) '\t');
    char_sets["\\n"] = bitset<256>().set((unsigned char) '\n');
    char_sets["\\r"] = bitset<256>().set((unsigned char) '\r');
    char_sets["\\f"] = bitset<256>().set((unsigned char) '\f');
    char_sets["\\a"] = bitset<256>().set((unsigned char) '\a');
    char_sets["\\e"] = bitset<256>().set((unsigned char) '\x1B');
    char_sets["\\v"] = bitset<256>().set((unsigned char) '\v');
    char_sets["\\V"] = ~ char_sets["\\v"];
    char_sets["\\h"] = bitset<256>().set((unsigned char) ' ');
    char_sets["\\H"] = ~ char_sets["\\h"];
  }

  // read char at position first and advance first
  // may return an empty char when encountering \E
  inline
  int non_deterministic_regexp_capture_cursor::next_char(bitset<256>& letter, const char *begin, const char*& first, 
				       const char *last, bool& literal, 
				       const map<string, bitset<256> >& char_sets, bool icase) {
    letter.reset();

    // anchores
    if ((*first == '$' || *first == '^') && !literal) {
      letter.set((unsigned char) 0); // beginning/end of line is '\0'
      ++first;
      return -1;
    }

    if (*first == '\\') {
      if (++first == last) 
	return token_error(first - begin, "unterminated escaped sequence");

      // quote sequence \Q ... \E
      if (*first == 'Q') {
	if (literal) {
	  letter.set((unsigned char) '\\');
	  return -1;
	}
	else {
	  literal = true;
	  if (++first == last) 
	    return token_error(first - begin, "unterminated quote sequence");
	  return next_char(letter, begin, first, last, literal, char_sets, icase);
	}
      }
      if (*first == 'E') {
	if (!literal) {
	  letter.set((unsigned char) 'E');
	  if (icase)
	    letter.set((unsigned char) 'e');
	}
	else literal = false;
	++first;
	return -1;
      }

      if (literal) {
	letter.set((unsigned char) '\\');
	return -1;
      }
      
      if (*first == 'x') { // hexadecimal character \x01 through \xFF
	if (++first == last)
	  return token_error(first - begin, "\\x used with no following hex digits");
	int n = 0;
	if (*first >= '0' && *first <= '9') {
	  n = 16 * (*first - '0'); 
	}
	else 
	  if (*first >= 'A' && *first <= 'F') {
	    n = 16 * (10 + *first - 'A'); 
	  }
	  else 
	    if (*first >= 'a' && *first <= 'f') {
	      n = 16 * (10 + *first - 'a'); 
	    }
	    else return token_error(first - begin, "\\x used with no following hex digits");
	
	if (++first == last)
	  return token_error(first - begin, "\\x used with only one following hex digit");
	
	if (*first >= '0' && *first <= '9') {
	  n += *first - '0'; 
	}
	else 
	  if (*first >= 'A' && *first <= 'F') {
	    n += 10 + (*first - 'A'); 
	  }
	  else
	    if (*first >= 'a' && *first <= 'f') {
	      n += 10 + (*first - 'a'); 
	    }
	    else return token_error(first - begin, "\\x used with only one following hex digit");
	letter.set(n);
	++first;
	return -1;
      }
      
      if (*first == 'c') { // control character \cA through \cZ maps to \x01 \x1A
	if (++first == last || *first < 'A' || *first > 'Z')
	  return token_error(first - begin, "\\c used with no following upper-case letter");
	letter.set(*first - 'A' + 1);
	++first;
	return -1;
      }

      // look for \s, \S, \w, \W, \d, \D, \n, \t, \v, \V, \r, \a, \e, \f, \h, \H
      std::map<string, bitset<256> >::const_iterator c = char_sets.find(std::string("\\") + *first);
      if (c != char_sets.end()) {
	letter = c->second;
	++first;
	return -1;
      }

      if (*first == '.') {
	letter.set((unsigned char) '.');
	++first;
	return -1;
      }
    }

    if (*first == '.') {
      if (literal)
	letter.set((unsigned char) '.');
      else {
	letter.set();





	// letter.set(0, false);





      }
    }
    else
      if (icase && isalpha(*first)) {
	letter.set((unsigned char) tolower((unsigned char) *first));
	letter.set((unsigned char) toupper((unsigned char) *first));
      }
      else letter.set((unsigned char) *first);
    ++first;
    return -1;
  }

  // Return error position or -1 if ok
  template <typename OutputIterator>
  int non_deterministic_regexp_capture_cursor::tokenize(const char *first, const char *last, OutputIterator x)
  {
    static map<string, bitset<256> > char_sets;
    if (char_sets.empty())
      build_char_sets(char_sets);

    *x++ = node(OPEN);
    node previous = node(OPEN);
  
    bool literal = false;
    std::vector<char> parenthesis; // contains '(' or 'i' for (?i:
    bool icase = false;
    std::vector<int> open_captures;
    int current_open_capture = 0;

    for(const char *begin = first; first != last; ++first, ++x) {
      if (literal && *first != '\\') {
	if (previous.type != OPEN && previous.type != OR) 
	  *x++ = node(CONCAT);
	if (icase && isalpha(*first)) {
	  bitset<256> c;
	  c.set((unsigned char) tolower(*first));
	  c.set((unsigned char) toupper(*first));
	  *x = previous = node(c);
	}
	else
	  *x = previous = node(*first);
	continue;
      }

      switch (*first) {
      case '|' :
	if (first == begin)
	  return token_error(0, "missing left part of alternative");
	*x = previous = node(OR);
	break;

      case '(' : 
	if (previous.type != OPEN && previous.type != OR) 
	  *x++ = node(CONCAT);
	if (first + 2 < last && first[1] == '?' && first[2] == ':') { // non capturing parenthesis (?:
	  first += 2;
	  parenthesis.push_back('(');
	}
	else {
	  if (first + 3 < last && first[1] == '?' && 
	      first[2] == 'i' && first[3] == ':') { // case insensitive (?i:
	    first += 3;
	    if (icase == false) {
	      parenthesis.push_back('i');
	      icase = true;
	    }
	    else parenthesis.push_back('(');
	  }
	  else { // capture
	    ++capture_count_;
	    *x++ = node(OPEN);
	    *x++ = node(OPENCAPTURE, current_open_capture, current_open_capture / 2);
	    open_captures.push_back(current_open_capture);
	    current_open_capture += 2;
	    *x++ = node(CONCAT);
	    parenthesis.push_back('c');
	  }
	}
	*x = previous = node(OPEN);
	break;

      case ')' : 
	if (parenthesis.empty())
	  return token_error(first - begin, "unbalanced parenthesis");
	if (parenthesis.back() == 'i')
	  icase = false;
	else 
	  if (parenthesis.back() == 'c') { // end of capture ?
	    ++capture_count_;
	    *x++ = node(CLOSE);
	    *x++ = node(CONCAT);
	    *x++ = node(CLOSECAPTURE, open_captures.back() + 1, open_captures.back() / 2);
	    open_captures.pop_back();
	  }
	*x = previous = node(CLOSE);
	parenthesis.pop_back();
	break;

      case '*' :
	if (first == begin)
	  return token_error(0, "missing expression before operator *");
	*x = previous = node(STAR);
	break;

      case '?' :
	if (first == begin)
	  return token_error(0, "missing expression before operator ?");
	*x = previous = node(QUESTION);
	break;

      case '+' :
	if (first == begin)
	  return token_error(0, "missing expression before operator +");
	*x = previous = node(PLUS);
	break;

      case '{' : // {n,m}
	if (first == begin)
	  return token_error(0, "missing expression before repeat operator");
	if (++first == last) 
	  return token_error(first - begin, "unterminated expression");
	{
	  int n = 0;
	  const char *tmp = first;
	  while (isdigit(*first)) {
	    n = n * 10 + *first - '0';
	    if (++first == last) 
	      return token_error(first - begin, "unterminated repeat operator");
	  }
	  int m = n;
	  if (*first == ',') {
	    if (++first == last) 
	      return token_error(first - begin, "unterminated repeat operator");
	    tmp = first;
	    for(m = 0; isdigit(*first);) {
	      m = m * 10 + *first - '0';
	      if (++first == last) 
		return token_error(first - begin, "unterminated repeat operator");
	    }
	    if (tmp == first) m = -1; // comma but no upper bound: {n,}
	  }
	  if (*first != '}') 
	    return token_error(first - begin, "invalid repeat operator");

	  // errors: {0} {0,0} {,0}
	  if (n > 32768 || m > 65535 || (n > m && m > -1) || (n == m && n == 0)) 
	    return token_error(first - begin, "wrong range for repeat operator");

	  // special cases: {0,1} {,1} {0,} {1,} {1,1}
	  if (n == 0 && m == 1)
	    *x = node(QUESTION);
	  else if (n == 0 && m == -1)
	    *x = node(STAR);
	  else if (n == 1 && m == -1)
	    *x = node(PLUS);
	  else if (!(n == 1 &&  m == 1))
	    *x = node(REPEAT, n, m);
	}
	break;

      case '[' :   // characters set
	{
	  if (previous.type != OPEN && previous.type != OPENCAPTURE && previous.type != OR) 
	    *x++ = node(CONCAT);
	  bitset<256> a;
	  bool v = true;
	  if (++first == last) 
	    return token_error(first - begin, "unterminated expression");
	  if (*first == '^') {  // a negation ?
	    a.set();
	    a.set(0, false);
	    v = false;
	    if (++first == last) 
	      return token_error(first - begin, "unterminated expression");
	  }
	  if (*first == '-') {  // "escaped" hyphen at the beginning ?
	    a.set((unsigned char) '-', v);
	    ++first;
	  }
	  else
	    if (*first == ']') {  // "escaped" closing square bracket at the beginning ?
	      a.set((unsigned char) ']', v);
	      ++first;
	    }
	  for(; first != last && *first != ']'; ++first) {
	    if (*first == '[') {
	      if (++first == last) break;
	      if (*first != ':') {
		a.set((unsigned char) '[', v);
	      }
	      else { // a predefined set as [:digits:]
		const char *start = first - 1;
		first = std::find(++first, last, ']');
		if (first == last) break;
		std::map<string, bitset<256> >::const_iterator c = char_sets.find(std::string(start, first + 1));
		if (c == char_sets.end())
		  return token_error(start - begin, "unknown character set");
		if (v == true)
		  a |= c->second;
		else a &= ~c->second;
		continue;
	      }
	    }
	    // TODO: next_char manages ranges and return a character set (pass a boolean for 
	    // saying this is a character set context and '-' as a special meaning, good for
	    // . also, see below the ugly hack)
	    bitset<256> tmp; // will possibly be used as a range lower bound
	    if (*first != '-') {  // not a potential character range ?
	      switch (*first) {
	      case '^' :
	      case '$' :
		tmp.set((unsigned char) *first++);
		break;
	      default :
		{
		  int r = next_char(tmp, begin, first, last, literal, char_sets, icase);
		  if (r != -1)
		    return r;
		  // ugly hack for dot
		  if (tmp.count() == 256) {
		    tmp.reset();
		    tmp.set((unsigned char) '.'); // . is a literal in a character set
		  }
		}
	      }
	      if (v == true)
		a |= tmp;
	      else a &= ~ tmp; // negative definition
	      if (*first != '-') {
		--first;
		continue;
	      }
	    }
	    // now *first == '-'
	    if (++first == last) 
	      return token_error(first - begin, "unterminated expression");
	    if (*first == ']') {  // unterminated character range ?
	      a.set((unsigned char) '-', v);
	      break; // end of character set
	    }
	    else { // character range [a-z]
	      if (tmp.count() != 1) { // a bound must not be a character set
		return token_error(first - 2 - begin, "wrong character range: the lower bound cannot be a character set");
	      }
	      bitset<256> upper_bound;
	      switch (*first) {
	      case '^' :
	      case '$' :
		upper_bound.set((unsigned char) *first);
		break;
	      default :
		{
		  int r = next_char(upper_bound, begin, first, last, literal, char_sets, icase);
		  --first;
		  if (r != -1)
		    return token_error(first - begin, "wrong character range");
		  // ugly hack for dot
		  if (upper_bound.count() == 256) {
		    upper_bound.reset();
		    upper_bound.set((unsigned char) '.'); // . is a literal in a character set
		  }
		}
	      }
	      if (upper_bound.count() != 1) // a bound must not be a character set
		return token_error(first - begin, "wrong character range: the upper bound cannot be a character set");

	      unsigned int int_lower_bound = 0, int_upper_bound = 0;
	      while (tmp.test(int_lower_bound) == false) 
		++int_lower_bound;
	      while (upper_bound.test(int_upper_bound) == false)
		++int_upper_bound;

	      // guess what, lower bound should be lower or equal to upper bound
	      if (int_upper_bound < int_lower_bound)
		return token_error(first - begin, "wrong character range: lower bound cannot be greater than upper bound");
	      else
		for(unsigned int i = int_lower_bound; i <= int_upper_bound; ++i)
		  a.set(i, v);
	    }
	  }
	  // first is supposed to point to ']'
	  if (first == last) 
	    return token_error(first - begin, "unterminated expression");
	  *x = previous = node(a);
	  break;
	}

      default :
	bitset<256> next_letter;
	int r = next_char(next_letter, begin, first, last, literal, char_sets, icase);
	if (r != -1)
	  return r;
	if (next_letter.any()) {
	  if (previous.type != OPEN && previous.type != OR) 
	    *x++ = node(CONCAT);



	  if (next_letter.count() == 256) // catch-all dot 



#ifndef UTF8
	    *x = previous = node(ANY);
#else
	  {
	    *x++ = node(OPEN);
	    next_letter.reset();
	    for(int i = 1; i < 0x7F; ++i)
	      next_letter.set(i);
	    *x++ = node(next_letter);
	    *x++ = node(OR);
	    next_letter.reset();
	    for(int i = 0; i < 32; ++i)
	      next_letter.set(i | 192);
	    *x++ = node(next_letter);
	    *x++ = node(CONCAT);
	    *x++ = node(ANY);
	    *x = previous = node(CLOSE);
	  }
#endif
	  else
	    *x = previous = node(next_letter);
	}
	--first;
	break;
      }
    }
    *x++ = node(CLOSE);
    *x++ = node(CONCAT);
    *x++ = node(FINAL);
    *x++ = node(CONCAT);
    *x++ = node(GRANDFINAL);
    return -1;
  }

  // exp -> term exp2    
  inline
  non_deterministic_regexp_capture_cursor::node*
  non_deterministic_regexp_capture_cursor::exp(vector<non_deterministic_regexp_capture_cursor::node>::iterator &first, 
		     vector<non_deterministic_regexp_capture_cursor::node>::iterator last)
  {
    if (first == last)
      return syntax_error(last - first, "empty expression");
    node *t = term(first, last);
    assert(t != 0);
    if (t == 0) return 0;
    return exp2(first, last, t);
  }

  // exp2 -> + term exp2 | epsilon
  inline
  non_deterministic_regexp_capture_cursor::node*
  non_deterministic_regexp_capture_cursor::exp2(vector<non_deterministic_regexp_capture_cursor::node>::iterator &first, 
		      vector<non_deterministic_regexp_capture_cursor::node>::iterator last, node *left)
  {
    assert(left != 0);
    if (first != last && first->type == OR) {
      node &root = *first;
      node *right = term(++first, last);
      if (right == 0) return 0;
      root.nullable = right->nullable || left->nullable;
      // firstpos(here) = firstpos(left) U firstpos(right): 
      root.firstpos = left->firstpos;
      root.firstpos.insert(right->firstpos.begin(), right->firstpos.end()); 
      root.lastpos = left->lastpos;
      root.lastpos.insert(right->lastpos.begin(), right->lastpos.end());
      if (left->nullable)
	root.emptymatch = left->emptymatch;
      else
	root.emptymatch = right->emptymatch;
      return exp2(first, last, &root);
    }
    return left;
  }

  // term -> form repeat term2
  inline
  non_deterministic_regexp_capture_cursor::node*
  non_deterministic_regexp_capture_cursor::term(vector<non_deterministic_regexp_capture_cursor::node>::iterator &first, 
		      vector<non_deterministic_regexp_capture_cursor::node>::iterator last)
  {
    if (first == last) 
      return syntax_error(last - first, "unterminated expression");
    node *f = form(first, last);
    assert(f != 0);
    if (f == 0) return 0;
    node *s = repeat(first, last, f);  // return f if no repeat token is found
    return term2(first, last, s);
  }

  // repeat -> * repeat | ? repeat | + repeat | epsilon
  inline
  non_deterministic_regexp_capture_cursor::node*
  non_deterministic_regexp_capture_cursor::repeat(vector<non_deterministic_regexp_capture_cursor::node>::iterator &first, 
			vector<non_deterministic_regexp_capture_cursor::node>::iterator last, node *left)
  {
    assert(left != 0);
    if (first != last)
      switch (first->type) {
      case STAR :
      case PLUS :
	{
	  node &root = *first;
	  root.nullable = first->type == STAR;
	  root.firstpos = left->firstpos;
	  root.lastpos = left->lastpos;
//	  if (first->type == STAR)
	    if (left->nullable)
	      root.emptymatch = left->emptymatch;

	  // nextpos(lastpos(here)) += firstpos(here):
	  for(S::const_iterator i = root.lastpos.begin(); i != root.lastpos.end(); ++i)
	    for(S::const_iterator j = root.firstpos.begin(); j != root.firstpos.end(); ++j) {
	      configuration tmp = i->second;
	      tmp.insert(j->second.begin(), j->second.end());
	      (*i).first->nextpos.insert(make_pair(j->first, tmp));
	    }
	  return repeat(++first, last, &root);
	}
      case QUESTION : 
	{
	  node &root = *first;
	  root.nullable = true;
	  root.firstpos = left->firstpos;
	  root.lastpos = left->lastpos;
	  if (left->nullable)
	    root.emptymatch = left->emptymatch;
	  return repeat(++first, last, &root);
	}
      default : 
	break;
      }
    return left;
  }
    

  // term2 -> . form repeat term2 | epsilon
  inline
  non_deterministic_regexp_capture_cursor::node*
  non_deterministic_regexp_capture_cursor::term2(vector<non_deterministic_regexp_capture_cursor::node>::iterator &first, 
		       vector<non_deterministic_regexp_capture_cursor::node>::iterator last, node *left)
  {
    if (first != last && first->type == CONCAT) {
      node &root = *first;
      node *f = form(++first, last);
      assert(f != 0);
      if (f == 0) return 0; 
      node *right = repeat(first, last, f);
      root.nullable = left->nullable && right->nullable;

      root.firstpos = left->firstpos;
      if (left->nullable) {
	for(S::const_iterator s = right->firstpos.begin(); s != right->firstpos.end(); ++s) {
	  configuration tmp = s->second;
	  tmp.insert(left->emptymatch.begin(), left->emptymatch.end());
	  root.firstpos.insert(make_pair(s->first, tmp));
	}
      }

      root.lastpos = right->lastpos;    
      if (right->nullable) {
	for(S::const_iterator s = left->lastpos.begin(); s != left->lastpos.end(); ++s) {
	  configuration tmp = s->second;
	  tmp.insert(right->emptymatch.begin(), right->emptymatch.end());
	  root.lastpos.insert(make_pair(s->first, tmp));
	}
      }

      root.emptymatch = left->emptymatch;
      root.emptymatch.insert(right->emptymatch.begin(), right->emptymatch.end());

      // nextpos(lastpos(left)) += firstpos(right):
      for(S::const_iterator i = left->lastpos.begin(); i != left->lastpos.end(); ++i)
	for(S::const_iterator j = right->firstpos.begin(); j != right->firstpos.end(); ++j) {
	  configuration tmp = j->second;
	  tmp.insert(i->second.begin(), i->second.end());
	  i->first->nextpos.insert(make_pair(j->first, tmp));	  
	}
      return term2(first, last, &root);
    }
    return left;
  }


  // form -> ( exp ) | letter
  inline
  non_deterministic_regexp_capture_cursor::node*
  non_deterministic_regexp_capture_cursor::form(vector<non_deterministic_regexp_capture_cursor::node>::iterator &first, 
		      vector<non_deterministic_regexp_capture_cursor::node>::iterator last)
  {
    if (first == last) 
      return syntax_error(last - first, "unterminated expression");
    if (first->type == OPEN) {
      node *e = exp(++first, last);
      assert(e != 0);
      if (e == 0) return 0;
      if (first->type == CLOSE) ++first;
      else return syntax_error(last - first, "unbalanced parenthesis");
      return e;
    }
    if (first->type == OPENCAPTURE || first->type == CLOSECAPTURE) {
      first->emptymatch[&*first] = 0;
      ++first;
      return &(first[-1]);
    }
    first->nullable = false;
    first->firstpos.insert(make_pair(&*first, configuration()));
    first->lastpos.insert(make_pair(&*first, configuration()));
    ++first;
    return &(first[-1]);
  }

//   inline
//   non_deterministic_regexp_capture_cursor regexpc(const string &exp) {
//     return non_deterministic_regexp_capture_cursor(exp.c_str());
//   }

//   inline
//   non_deterministic_regexp_capture_cursor regexpc() {
//     return non_deterministic_regexp_capture_cursor();
//   }


class capture_cursor : public non_deterministic_regexp_capture_cursor
{
public:
  typedef capture_cursor                          self;
  typedef non_deterministic_regexp_capture_cursor super;
  typedef configuration                           tag_type;

  capture_cursor(const string &exp, bool use_horspool)
    : super(exp, use_horspool)
  { }

  bool forward(char_type c) {
    const bool r = super::forward(c);
    intersection.clear();

    if (q.size() == 1) {
      // FIXME:optimize set_difference in this case
      intersection.swap(q.begin()->second);
      cout << "INTERSECTION " << "\033[47;31m" << intersection << "\033[0m" << endl;
      return r;
    }
    else {
      //      state_type::const_iterator best = q.end();
      for(state_type::const_iterator i = q.begin(); i != q.end(); ++i) {
// 	if (i->first->type == GRANDFINAL && 
// 	    (best == q.end() || i->second < best->second))
// 	  best = i;
// 	else
	  if (i == q.begin())
	    intersection = i->second;
	  else
	    intersection = intersection.intersection(i->second);
      }
//       if (c == '\0' && best != q.end()) {
// 	intersection = best->second; // '\0' + GRANDFINAL == finalizing
// 	cout << "INTERSECTION " << "\033[47;31m" << intersection << "\033[0m" << endl;
// 	return r;
//       }
    }
    cout << "INTERSECTION " << "\033[47;31m" << intersection << "\033[0m" << endl;
    for(state_type::iterator i = q.begin(); i != q.end(); ++i) {
      configuration diff;
      set_difference(i->second.begin(), i->second.end(), intersection.begin(), intersection.end(), inserter(diff, diff.end()));
      i->second = diff;
      cout <<  "\033[1m" << "remain: " << i->first << ':' << i->first->pretty() 
	   << ":=" << i->second << "\033[0m" << endl;
    }
    return r;
  }

  void finalize() {
    super::forward('\0');
    state_type::const_iterator best = q.end();
    for(state_type::const_iterator i = q.begin(); i != q.end(); ++i) {
      if (i->first->type == GRANDFINAL && 
	  (best == q.end() || i->second < best->second))
	best = i;
    }
    intersection = best->second;
  }

  self& operator=(const state_type &s) {
    super::operator=(s);
    return *this;
  }

  const tag_type& src_tag() const {
    return intersection;
  }

protected:
  configuration intersection;
};


  template <>
  class lazy_cursor<capture_cursor, DFT_matrix_mini> : public cursor_concept
  {
  public:
    typedef capture_cursor Cursor;
#ifdef USE_HASH_TABLE
    typedef DFT_matrix_mini<const capture_cursor::ordered_state_type*,
 			    capture_cursor::tag_type> DFA_type;
#else
    typedef DFT_matrix_mini<const capture_cursor::state_type*,
			    capture_cursor::tag_type> DFA_type;
#endif

  protected:
#ifdef USE_HASH_TABLE
    typedef std::map<capture_cursor::ordered_state_type, DFA_type::state_type> mapper;
#else
    typedef std::map<capture_cursor::state_type, DFA_type::state_type> mapper;
#endif

    typedef unsigned long offset_type;

    Cursor               c;
    smart_ptr<DFA_type>  dfa;
    smart_ptr<mapper>    mapping;  
    DFA_type::state_type my_sink;
    DFA_type::state_type q;

  public:
    typedef lazy_cursor           self;
    typedef DFA_type::state_type  state_type;
    typedef Cursor::char_type     char_type;
    typedef Cursor::char_traits   char_traits;
    typedef Cursor::configuration configuration;
    typedef configuration         tag_type;

  protected:
    const configuration *conf;

  public:
    lazy_cursor(const Cursor &x)
      : c(x), my_sink(dfa->new_state()), q(DFA_type::null_state), conf(NULL)
    { 
      if (!x.sink()) {
	q = dfa->new_state();
#ifdef USE_HASH_TABLE
	dfa->tag(q) = &(mapping->insert(make_pair(capture_cursor::ordered_state_type(c.src().begin(), c.src().end()), q)).first->first);
#else
	dfa->tag(q) = &(mapping->insert(make_pair(c.src(), q)).first->first);
#endif
	dfa->final(q) = c.src_final();
	dfa->initial(q);
      }
    }

    int capture_count() const { return c.capture_count(); }

    const Cursor& adaptee() {
      if (q != DFA_type::null_state)
	c = capture_cursor::state_type(dfa->tag(q)->begin(), dfa->tag(q)->end());
      return c;
    }
    
    const DFA_type& cache() const {
      return *dfa;
    }
    
    state_type src() const {
      return q;
    }
    
    self& operator=(state_type p) {
      q = p;
      return *this;
    }
    
    bool src_final() const {
      return dfa->final(q);
    }
    
    // return relative offsets
    const tag_type& src_tag() const {
      static const configuration empty;
      if (conf != NULL)
	return *conf;
      return empty;
    }
    
    bool sink() const {
      return q == DFA_type::null_state;
    }
    
    state_type sink_state() const {
      return DFA_type::null_state;
    }

    bool exists(const char_type &a) const {
      state_type aim = dfa->delta1(q, a);
      if (aim == DFA_type::null_state) {
	Cursor tmp = c;
#ifdef USE_HASH_TABLE
	tmp = capture_cursor::state_type(dfa->tag(q)->begin(), dfa->tag(q)->end());
#else
	tmp = *dfa->tag(q);
#endif
	return tmp.exists(a);
      }
      return aim != my_sink;
    }

    bool forward(const char_type &a) {
      state_type tmp = dfa->delta1(q, a);
      if (tmp == my_sink) {
	q = DFA_type::null_state; // we already know there is no transition with 'a'
	return false;
      }
      if (tmp == DFA_type::null_state) {    // transition not in the cache ?
	c = capture_cursor::state_type(dfa->tag(q)->begin(), dfa->tag(q)->end());
	if (c.forward(a)) {   // any transition labelled with 'a' ?
#ifdef USE_HASH_TABLE
	  std::pair<mapper::iterator, bool> p = 
	    mapping->insert(std::make_pair(capture_cursor::ordered_state_type(c.src().begin(), c.src().end()), DFA_type::null_state));
#else
	  std::pair<mapper::iterator, bool> p = 
	    mapping->insert(std::make_pair(c.src(), DFA_type::null_state));
#endif
	  if (p.second == true) { // target state is not dejavu ?
	    tmp = dfa->new_state();
	    dfa->final(tmp) = c.src_final();
	    dfa->tag(tmp) = &(p.first->first);
	    p.first->second = tmp;
	  }
	  else tmp = p.first->second;
	  dfa->set_trans(q, a, tmp);
	  if (!c.src_tag().empty()) {
	    // cout << "dfa->set_output(" << q << ", " << a << ", " << c.src_tag() << ")" << endl;
	    dfa->set_output(q, a, c.src_tag());
	  }
	}
	else {
	  dfa->set_trans(q, a, my_sink);
	  q = DFA_type::null_state;
	  return false;
	}
      }

//       conf = dfa->output(q, a);
//       if (conf != NULL) {
// 	for(non_deterministic_regexp_capture_cursor::configuration::const_iterator ic = conf->begin(); ic != conf->end(); ++ic) {
// 	  cout << "captures[" << ic->first.id() << "] displacement " << ic->second << endl;
// 	  //	  (*captures)[ic->first.id()] = offset + ic->second;
// 	}
//       }
//       //      ++offset;

      conf = dfa->output(q, a); // how to avoid this if we don't need
				// capture and don't need to pay the price
      q = tmp;
      return true;
    }
  };

#if 0
  template <>
  class lazy_cursor<non_deterministic_regexp_capture_cursor, DFT_matrix_mini> : public cursor_concept
  {
  public:
    typedef non_deterministic_regexp_capture_cursor Cursor;
#ifdef USE_HASH_TABLE
    typedef DFT_matrix_mini<const non_deterministic_regexp_capture_cursor::ordered_state_type*,
			    non_deterministic_regexp_capture_cursor::tag_type> DFA_type;
#else
    typedef DFT_matrix_mini<const non_deterministic_regexp_capture_cursor::state_type*,
			    non_deterministic_regexp_capture_cursor::tag_type> DFA_type;
#endif

  protected:
#ifdef USE_HASH_TABLE
    typedef std::map<non_deterministic_regexp_capture_cursor::ordered_state_type, DFA_type::state_type> mapper;
#else
    typedef std::map<non_deterministic_regexp_capture_cursor::state_type, DFA_type::state_type> mapper;
#endif

    typedef unsigned long offset_type;

    Cursor               c;
    smart_ptr<DFA_type>  dfa;
    smart_ptr<mapper>    mapping;  
    smart_ptr<vector<offset_type> > captures;
    DFA_type::state_type my_sink;
    DFA_type::state_type q;
    offset_type          offset;

  public:
    typedef lazy_cursor          self;
    typedef DFA_type::state_type state_type;
    typedef vector<offset_type>  tag_type;
    typedef Cursor::char_type    char_type;
    typedef Cursor::char_traits  char_traits;

    lazy_cursor(const Cursor &x)
      : c(x), my_sink(dfa->new_state()), q(DFA_type::null_state), offset(0)
    { 
      if (!x.sink()) {
	q = dfa->new_state();
#ifdef USE_HASH_TABLE
	dfa->tag(q) = &(mapping->insert(make_pair(non_deterministic_regexp_capture_cursor::ordered_state_type(c.src().begin(), c.src().end()), q)).first->first);
#else
	dfa->tag(q) = &(mapping->insert(make_pair(c.src(), q)).first->first);
#endif
	dfa->final(q) = c.src_final();
	dfa->initial(q);
	captures->resize(x.capture_count(), 0); 
      }
    }

    const Cursor& adaptee() {
      if (q != DFA_type::null_state)
	c = *dfa->tag(q);
      return c;
    }
    
    const DFA_type& cache() const {
      return *dfa;
    }
    
    state_type src() const {
      return q;
    }
    
    self& operator=(state_type p) {
      q = p;
      offset = 0;
      fill(captures->begin(), captures->end(), 0);
      return *this;
    }
    
    bool src_final() const {
      return dfa->final(q);
    }
    
    const tag_type& src_tag() const {
      return *captures;
    }
    
    bool sink() const {
      return q == DFA_type::null_state;
    }
    
    state_type sink_state() const {
      return DFA_type::null_state;
    }

    //   bool exists(const char_type &a) const {
    //     state_type aim = dfa->delta1(q, a);
    //     if (aim == my_sink) return false;
    //     Cursor tmp = c;
    //     tmp = dfa->tag(q);
    //     return tmp.exists(a);
    //   }

    bool forward(const char_type &a) {
      state_type tmp = dfa->delta1(q, a);
      if (tmp == my_sink) {
	q = DFA_type::null_state; // we already know there is no transition with 'a'
	return false;
      }
      if (tmp == DFA_type::null_state) {    // transition not in the cache ?
	c = *dfa->tag(q);
	if (c.forward(a)) {   // any transition labelled with 'a' ?
#ifdef USE_HASH_TABLE
	  std::pair<mapper::iterator, bool> p = 
	    mapping->insert(std::make_pair(non_deterministic_regexp_capture_cursor::ordered_state_type(c.src().begin(), c.src().end()), DFA_type::null_state));
#else
	  std::pair<mapper::iterator, bool> p = 
	    mapping->insert(std::make_pair(c.src(), DFA_type::null_state));
#endif
	  if (p.second == true) { // target state is not dejavu ?
	    tmp = dfa->new_state();
	    dfa->final(tmp) = c.src_final();
	    dfa->tag(tmp) = &(p.first->first);
	    p.first->second = tmp;
	  }
	  else tmp = p.first->second;
	  dfa->set_trans(q, a, tmp);
	  if (!c.src_tag().empty()) {
	    // cout << "dfa->set_output(" << q << ", " << a << ", " << c.src_tag() << ")" << endl;
	    dfa->set_output(q, a, c.src_tag());
	  }
	}
	else {
	  dfa->set_trans(q, a, my_sink);
	  q = DFA_type::null_state;
	  return false;
	}
      }
      non_deterministic_regexp_capture_cursor::configuration *conf = dfa->output(q, a);
      if (conf != NULL) {
	for(non_deterministic_regexp_capture_cursor::configuration::const_iterator ic = conf->begin(); ic != conf->end(); ++ic) {
	  // cout << "captures[" << ic->first.id() << "] = offset + displacement = " << offset << " + " << ic->second << " = " << offset + ic->second << endl;
	  (*captures)[ic->first.id()] = offset + ic->second;
	}
      }
      q = tmp;
      ++offset;
      return true;
    }
  };

  // match algorithm specializations for regexp

  inline
  bool match(lazy_cursor<non_deterministic_regexp_capture_cursor>& c, const char *w)
  {
    if (!c.forward('\0')) // ^ anchoring
      return false;
    for(; *w != '\0' && c.forward(*w); ++w)
      if (c.src_final()) return true;
    return *w == '\0' && c.forward('\0') && c.src_final(); // $ anchoring
  }

  template <typename InputI>
  inline
  bool match(lazy_cursor<non_deterministic_regexp_capture_cursor>& c, InputI first, InputI last)
  {
    if (!c.forward('\0'))  // ^ anchoring
      return false;
    for(; first != last && c.forward(*first); ++first)
      if (c.src_final()) return true;
    return first == last && c.forward('\0') && c.src_final(); // $ anchoring
  }

  template <typename ForwardI>
  inline
  ForwardI first_match(lazy_cursor<non_deterministic_regexp_capture_cursor> &c, ForwardI first, ForwardI last)
  {
    if (!c.forward('\0')) return first;
    const ForwardI start = first;
    for(; first != last && c.forward(*first); ++first)
      if (c.src_final()) return ++first;
    return first == last && c.forward('\0') && c.src_final() ? first : start;
  }

  inline
  const char*
  first_match(lazy_cursor<non_deterministic_regexp_capture_cursor> &c, const char *text)
  {
    if (!c.forward('\0')) return text;
    const char *start = text;
    for(; *text != '\0' && c.forward(*text); ++text)
      if (c.src_final()) return ++text;
    return *text == '\0' && c.forward('\0') && c.src_final() ? text : start;
  }

  template <typename ForwardI>
  inline 
  bool capture(lazy_cursor<non_deterministic_regexp_capture_cursor, DFT_matrix_mini> &c, ForwardI first, ForwardI last, vector<unsigned long> &offsets) {
    for(; first != last && c.forward(*first); ++first)
    if (first == last && c.forward('\0') && c.src_final()) {
      offsets = c.src_tag();
      return true;
    }
    return false;
  }

//   inline
//   ostream& operator<<(ostream& out, const non_deterministic_regexp_capture_cursor& c) {
//     for(vector<non_deterministic_regexp_capture_cursor::node>::const_iterator n = c.e->begin(); n != c.e->end(); ++n) {
//       if (c.q.find(const_cast<astl::non_deterministic_regexp_capture_cursor::node*>(&*n)) != c.q.end())
// 	out << "\033[1m" << *n << "\033[0m";
//       else
// 	out << *n;
//     }
//     return out;
//   }


  template <typename ForwardI>
  inline
  ForwardI first_match(non_deterministic_regexp_capture_cursor &c, ForwardI first, ForwardI last)
  {
    //    if (!c.forward('\0')) return first;
    const ForwardI start = first;
    for(non_deterministic_regexp_capture_cursor::state_type::const_iterator i = c.src().begin(); i != c.src().end(); ++i) {
      cout << "< " << i->first << ':' << *i->first << ", { ";
      for(non_deterministic_regexp_capture_cursor::configuration::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
	cout << (int) *j << " ";
      cout << "} > ";
    }
    cout << endl;
    for(; first != last && c.forward(*first); ++first) {
      for(non_deterministic_regexp_capture_cursor::state_type::const_iterator i = c.src().begin(); i != c.src().end(); ++i) {
	cout << "< " << i->first << ':' << *i->first << ", { ";
	for(non_deterministic_regexp_capture_cursor::configuration::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
	  cout << (int) *j << " ";
	cout << "} > ";
      }
      cout << endl;
      if (c.src_final()) return ++first;
    }
    return first == last && c.forward('\0') && c.src_final() ? first : start;
  }
#endif

}

#endif
