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

#ifndef ASTL_REGEXP_H
#define ASTL_REGEXP_H

#include <astl.h>
#include <lazy.h>
#include <vector>
#include <iostream>
#include <set>
#include <iterator>
#include <cctype>  
#include <bitset>
#include <cstring>
#include <sstream>

#define USE_HASH_TABLE

// #define UTF8

// guess what? crappy C++ compiler from sparc/solaris can't compile hash sets...
#if defined(USE_HASH_TABLE) && defined(__sparc)
#undef USE_HASH_TABLE
#endif

#if defined(USE_HASH_TABLE)
// Use g++'s tr1::unordered_set if possible as it looks more efficient than boost's
#if defined(__GNUG__) && (__GNUC__ > 3)
#define USE_TR1_UNORDERED_SET
#include <tr1/unordered_set>
#else
#define USE_BOOST_UNORDERED_SET
#include <boost/unordered_set.hpp>
#endif

#endif // USE_HASH_TABLE

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

  class regexp_cursor : public cursor_concept
  {
#ifdef _MSC_VER
  public:
#else
  protected:
#endif
    struct node;
#ifdef USE_TR1_UNORDERED_SET
    typedef tr1::unordered_set<node*> S;
#else
#ifdef USE_BOOST_UNORDERED_SET
    typedef boost::unordered_set<node*> S;
#else
    typedef set<node*> S;
#endif
#endif

    typedef enum { STAR, CONCAT, OR, REPEAT, EPSILON, 
		   LETTER, OPEN, CLOSE, QUESTION, PLUS,
		   ANY, FINAL } token_type;
  
    struct node {
      bitset<256> letter;
      token_type type;
      short n, m; // repeat operator
      bool annulable;
      S premiere_pos, derniere_pos, pos_suivante;
    
      node(token_type t, short n_ = 0, short m_ = 0)
	: type(t), n(n_), m(m_) { 
	if (type == ANY) {
	  type = LETTER;
	  letter.set();
	}
      }

      node(char t)
	: type(LETTER), annulable(false) { 
	letter.set((unsigned char) t);
      }

      node(const bitset<256> &t)
	: letter(t), type(LETTER), annulable(false)
      { }

      bool final() const {
	return type == FINAL;
      }

      char to_char() const {   // does not work with ranges
	size_t i;
	for(i = 0; i != letter.size() && letter.test(i) == false; ++i);
	return (char) (-128 + i);
      }
    };

    S q;  // current state
    smart_ptr<vector<node> > e;  // node "allocator"
    const char *errmsg;
    smart_ptr<S> I;

  public:
    smart_ptr<horspool_finder> horspool;

  protected:
    friend ostream& operator<<(ostream&, const regexp_cursor&);

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

    typedef regexp_cursor    self;
    typedef S                state_type;
    typedef set<node*>       ordered_state_type;
    typedef plain            char_traits;
    typedef plain::char_type char_type;
    typedef empty_tag        tag_type;

    regexp_cursor(const string &expression = "", bool horspool_optimized = false)
      : errmsg(NULL), errpos(-1)
    {
      if (!expression.empty()) {
	e->reserve(128);
	errpos = tokenize(expression.c_str(), 
			  expression.c_str() + expression.size(), 
			  back_inserter(*e));

	if (errpos == -1) {
	  preprocess_repeats();
	  vector<node>::iterator first = e->begin();
	  node *root = exp(first, e->end());
	  // managed non-anchored alternatives
	  if (root != NULL) {
	    // if there is only one position for the root and this position
	    // is not ^ nor ANY, then there may be a literal prefix that can
	    // be matched more efficiently with a boyer-moore search
	    if (horspool_optimized && root->premiere_pos.size() == 1) {
	      // look for a litteral prefix of size > 1
	      string prefix;
	      node *n = *root->premiere_pos.begin();
	      for(; n->letter.count() == 1 && !n->annulable && n->letter.test(0) == false; n = *(n->pos_suivante.begin())) {
		for(int i = 0; i < 256; ++i)
		  if (n->letter.test(i) == true) {
		    prefix += (char) i;
		    break;
		  }
		if (n->pos_suivante.size() != 1 || (*n->pos_suivante.begin())->type == FINAL) {
		  if (prefix.size() > 1) {
		    // initialize boyer-moore searcher and remove prefix from expression
		    horspool->init(prefix);
		    q = n->pos_suivante;
		    return;
		  }
		  break;
		}
	      }
	      if (prefix.size() > 1) {
		// initialize boyer-moore searcher and remove prefix from expression
		horspool->init(prefix);
		q.insert(n);
		return;
	      }
	    }

	    q = root->premiere_pos;
	    for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	      // if not beginning of line (anchor ^)
	      if ((*i)->letter.count() == 256 || (*i)->letter.test(0) == false)
		I->insert(*i);
	  }
	  else errpos = expression.size() - errpos;
	}
      }
    }

    ptrdiff_t error() const { // possible error position (-1 if expression is ok)
      return errpos;
    }
    
    const char* error_message() const {
      return errmsg;
    }

    self& operator=(const state_type &s) {
      q = s;
      return *this;
    }

#ifdef USE_HASH_TABLE
    self& operator=(const ordered_state_type &s) {
      q.clear();
      q.insert(s.begin(), s.end());
      return *this;
    }
#endif
    // #ifdef USE_HASH_TABLE
    //    state_type src() const {
    //      return state_type(q.begin(), q.end());
    //#else
    const state_type& src() const {
      return q;
      //#endif
    }

    bool exists(char a) const {
      for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	if ((*i)->letter.test((unsigned char) a) == true)
	  return true;
      return false;
    }

    bool forward(char letter) {
      state_type p = *I; // for non-anchored expressions
      for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	if ((*i)->letter.test((unsigned char) letter) == true)
	  p.insert((*i)->pos_suivante.begin(),
		   (*i)->pos_suivante.end());
      q.swap(p);   // optimized equivalent to q = p;
      return !q.empty();
    }
    
    bool src_final() const {
      for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	if ((*i)->final()) return true;
      return false;
    }
     
    bool sink() const {
      return q.empty();
    }
 
    tag_type src_tag() const {
      return tag_type();
    }

    void clear() {
      q.clear();
      e->clear();
      I->clear();
    }

    void determinize(DFA_matrix<plain> &dfa) const {
      typedef DFA_matrix<plain> dfa_type;
      map<ordered_state_type, dfa_type::state_type> old2new;
      dfa_type::state_type i = dfa.new_state();
      dfa.initial(i);
      determinize_(dfa, old2new, old2new.insert(make_pair(ordered_state_type(q.begin(), q.end()), i)).first);
    }

    void determinize(DFA_matrix_mini<plain> &dfa) const {
      typedef DFA_matrix_mini<plain> dfa_type;
      map<ordered_state_type, dfa_type::state_type> old2new;
      dfa_type::state_type i = dfa.new_state();
      dfa.initial(i);
      determinize_(dfa, old2new, old2new.insert(make_pair(ordered_state_type(q.begin(), q.end()), i)).first);
    }

    template <typename DFA>
    void determinize_(DFA &dfa, 
		      map<ordered_state_type, typename DFA::state_type> &old2new,
		      typename map<ordered_state_type, typename DFA::state_type>::iterator p) const {
      for(int i = -128; i < 128; ++i) {
	ordered_state_type aim(I->begin(), I->end());    
	for(typename ordered_state_type::iterator s = p->first.begin(); s != p->first.end(); ++s) {
	  if ((*s)->letter.test((unsigned char) i)) {
	    aim.insert((*s)->pos_suivante.begin(), (*s)->pos_suivante.end());
	  }
	}
	if (!aim.empty()) {
	  pair<typename map<ordered_state_type, typename DFA::state_type>::iterator, bool> j = 
	    old2new.insert(make_pair(aim, DFA::null_state));
	  if (j.second == false) {
	    dfa.set_trans(p->second, (char) (unsigned char) i, j.first->second);
	  }
	  else {
	    j.first->second = dfa.new_state();
	    dfa.set_trans(p->second, (char) (unsigned char) i, j.first->second);
	    for(typename ordered_state_type::const_iterator ii = aim.begin(); ii != aim.end(); ++ii)
	      if ((*ii)->final()) {
		dfa.final(j.first->second) = true;
		break;
	      }
	    aim.clear();
	    determinize_(dfa, old2new, j.first);
	  }
	}
      }
    }

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
  void regexp_cursor::preprocess_repeats() {
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
  ostream& operator<<(ostream &out, const regexp_cursor::node &x) 
  {
    switch(x.type) {
    case regexp_cursor::LETTER :
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
		cout << (unsigned char) lower;
	      }
	      else if (i - lower == 2) {
		cout << (unsigned char) lower << (unsigned char) (lower + 1);
	      }
	      else cout << (unsigned char) lower << '-' 
			<< (unsigned char) (i - 1);
	      lower = 0;
	    }
	  }
	}
	if (x.letter.count() > 1)
	  out << ']';
      }
      break;
	
    case regexp_cursor::OR : 
      out << "|"; 
      break;
	
    case regexp_cursor::FINAL : 
      out << "[EoE]";
      break;
	
    case regexp_cursor::OPEN : 
      out << "("; 
      break;
	
    case regexp_cursor::CLOSE : 
      out << ")"; 
      break;
	
    case regexp_cursor::CONCAT : 
      out << " "; 
      break;
	
    case regexp_cursor::STAR : 
      out << "*"; 
      break;
	
    case regexp_cursor::QUESTION : 
      out << "?"; 
      break;
	
    case regexp_cursor::PLUS : 
      out << "+"; 
      break;
	
    case regexp_cursor::REPEAT : 
      out << "{" << x.n << "," << x.m << "}";
      break;
	
    default :
      break;
    } 
    return out;
  }

  inline
  void regexp_cursor::build_char_sets(map<string, bitset<256> >& char_sets) {
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
  int regexp_cursor::next_char(bitset<256>& letter, const char *begin, const char*& first, const char *last, 
			       bool& literal, const map<string, bitset<256> >& char_sets, bool icase) {
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
      else
	letter.set();
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
  int regexp_cursor::tokenize(const char *first, const char *last, OutputIterator x)
  {
    static map<string, bitset<256> > char_sets;
    if (char_sets.empty())
      build_char_sets(char_sets);

    *x++ = node(OPEN);
    node previous = node(OPEN);
  
    bool literal = false;
    std::vector<char> parenthesis; // contains '(' or 'i' for (?i:
    bool icase = false;
  
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
	*x = previous = node(OPEN);
	if (first + 2 < last && first[1] == '?' && first[2] == ':') // non capturing parenthesis (?:
	  first += 2;
	else
	  if (first + 3 < last && first[1] == '?' && 
	      first[2] == 'i' && first[3] == ':') { // case insensitive (?i:
	    first += 3;
	    if (icase == false) {
	      parenthesis.push_back('i');
	      icase = true;
	      break;
	    }
	  }
	parenthesis.push_back('(');
	break;

      case ')' : 
	if (parenthesis.empty())
	  return token_error(first - begin, "unbalanced parenthesis");
	if (parenthesis.back() == 'i')
	  icase = false;
	parenthesis.pop_back();
	*x = previous = node(CLOSE);
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
	  if (previous.type != OPEN && previous.type != OR) 
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
    return -1;
  }

  // exp -> term exp2    
  inline
  regexp_cursor::node*
  regexp_cursor::exp(vector<regexp_cursor::node>::iterator &first, 
		     vector<regexp_cursor::node>::iterator last)
  {
    if (first == last)
      return syntax_error(last - first, "empty expression");
    node *t = term(first, last);
    if (t == 0) return 0;
    return exp2(first, last, t);
  }

  // exp2 -> + term exp2 | epsilon
  inline
  regexp_cursor::node*
  regexp_cursor::exp2(vector<regexp_cursor::node>::iterator &first, 
		      vector<regexp_cursor::node>::iterator last, node *left)
  {
    if (first != last && first->type == OR) {
      node &root = *first;
      node *right = term(++first, last);
      if (right == 0) return 0;
      root.annulable = right->annulable || left->annulable;
      // premiere_pos(here) = premiere_pos(left) U premiere_pos(right): 
      root.premiere_pos.insert(left->premiere_pos.begin(),
			       left->premiere_pos.end()); 
      root.premiere_pos.insert(right->premiere_pos.begin(),
			       right->premiere_pos.end()); 
      root.derniere_pos.insert(left->derniere_pos.begin(),
			       left->derniere_pos.end()); 
      root.derniere_pos.insert(right->derniere_pos.begin(),
			       right->derniere_pos.end());
      return exp2(first, last, &root);
    }
    return left;
  }

  // term -> form repeat term2
  inline
  regexp_cursor::node*
  regexp_cursor::term(vector<regexp_cursor::node>::iterator &first, 
		      vector<regexp_cursor::node>::iterator last)
  {
    if (first == last) 
      return syntax_error(last - first, "unterminated expression");
    node *f = form(first, last);
    if (f == 0) return 0;
    node *s = repeat(first, last, f);  // return f if no repeat token is found
    return term2(first, last, s);
  }

  // repeat -> * repeat | ? repeat | + repeat | epsilon
  inline
  regexp_cursor::node*
  regexp_cursor::repeat(vector<regexp_cursor::node>::iterator &first, 
			vector<regexp_cursor::node>::iterator last, node *left)
  {
    if (first != last)
      switch (first->type) {
      case STAR :
	{
	  node &root = *first;
	  root.annulable = true;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	
	  // pos_suivante(derniere_pos(here)) += premiere_pos(here):
	  for(S::const_iterator i = root.derniere_pos.begin(); 
	      i != root.derniere_pos.end(); ++i)
	    (*i)->pos_suivante.insert(root.premiere_pos.begin(),
				      root.premiere_pos.end());
	  return repeat(++first, last, &root);
	}
      case QUESTION : 
	{
	  node &root = *first;
	  root.annulable = true;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	  return repeat(++first, last, &root);
	}
      case PLUS :
	{
	  node &root = *first;
	  root.annulable = false;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	
	  // pos_suivante(derniere_pos(here)) += premiere_pos(here):
	  for(S::const_iterator i = root.derniere_pos.begin(); 
	      i != root.derniere_pos.end(); ++i)
	    (*i)->pos_suivante.insert(root.premiere_pos.begin(),
				      root.premiere_pos.end());
	  return repeat(++first, last, &root);
	}
      default : 
	break;
      }
    return left;
  }
    

  // term2 -> . form repeat term2 | epsilon
  inline
  regexp_cursor::node*
  regexp_cursor::term2(vector<regexp_cursor::node>::iterator &first, 
		       vector<regexp_cursor::node>::iterator last, node *left)
  {
    if (first != last && first->type == CONCAT) {
      node &root = *first;
      node *f = form(++first, last);
      if (f == 0) return 0; 
      node *right = repeat(first, last, f);
      root.annulable = left->annulable && right->annulable;
      if (left->annulable) {
	root.premiere_pos.insert(left->premiere_pos.begin(),
				 left->premiere_pos.end()); 
	root.premiere_pos.insert(right->premiere_pos.begin(),
				 right->premiere_pos.end()); 
      }
      else
	root.premiere_pos = left->premiere_pos;
    
      if (right->annulable) {
	root.derniere_pos.insert(right->derniere_pos.begin(),
				 right->derniere_pos.end()); 
	root.derniere_pos.insert(left->derniere_pos.begin(),
				 left->derniere_pos.end());
      }
      else
	root.derniere_pos = right->derniere_pos;
      // pos_suivante(derniere_pos(left)) += premiere_pos(right):
      for(S::const_iterator i = left->derniere_pos.begin(); 
	  i != left->derniere_pos.end(); ++i)
	(*i)->pos_suivante.insert(right->premiere_pos.begin(),
				  right->premiere_pos.end());
      return term2(first, last, &root);
    }
    return left;
  }


  // form -> ( exp ) | letter
  inline
  regexp_cursor::node*
  regexp_cursor::form(vector<regexp_cursor::node>::iterator &first, 
		      vector<regexp_cursor::node>::iterator last)
  {
    if (first == last) 
      return syntax_error(last - first, "unterminated expression");
    if (first->type == OPEN) {
      node *e = exp(++first, last);
      if (e == 0) return 0;
      if (first->type == CLOSE) ++first;
      else return syntax_error(last - first, "unbalanced parenthesis");
      return e;
    }
    first->annulable = false;
    first->premiere_pos.insert(&*first);
    first->derniere_pos.insert(&*first);
    ++first;
    return &(first[-1]);
  }

  inline
  regexp_cursor regexpc(const string &exp) {
    return regexp_cursor(exp.c_str());
  }

  inline
  regexp_cursor regexpc() {
    return regexp_cursor();
  }


  template <template <typename T, typename U> class DFA>
  class lazy_cursor<regexp_cursor, DFA> : public cursor_concept
  {
  public:
    typedef regexp_cursor Cursor;
#ifdef USE_HASH_TABLE
    typedef DFA<Cursor::char_traits, 
		const regexp_cursor::ordered_state_type*> DFA_type;
#else
    typedef DFA<Cursor::char_traits, 
		const regexp_cursor::state_type*> DFA_type;
#endif

  protected:
#ifdef USE_HASH_TABLE
    typedef std::map<regexp_cursor::ordered_state_type, typename DFA_type::state_type> mapper;
#else
    typedef std::map<regexp_cursor::state_type, typename DFA_type::state_type> mapper;
#endif

    Cursor               c;
    smart_ptr<DFA_type>  dfa;
    smart_ptr<mapper>    mapping;  
    typename DFA_type::state_type my_sink;
    typename DFA_type::state_type q;
    
  public:
    typedef lazy_cursor                   self;
    typedef typename DFA_type::state_type state_type;
    typedef empty_tag                     tag_type;
    typedef Cursor::char_type    char_type;
    typedef Cursor::char_traits  char_traits;

    lazy_cursor(const Cursor &x)
      : c(x), my_sink(dfa->new_state()), q(DFA_type::null_state)
    { 
      if (!x.sink()) {
	q = dfa->new_state();
#ifdef USE_HASH_TABLE
	dfa->tag(q) = &(mapping->insert(make_pair(regexp_cursor::ordered_state_type(c.src().begin(), c.src().end()), q)).first->first);
#else
	dfa->tag(q) = &(mapping->insert(make_pair(c.src(), q)).first->first);
#endif
	dfa->final(q) = c.src_final();
	dfa->initial(q);
      }
    }

    // deprecated:
    //   const Cursor& cursor() {
    //     c = dfa->tag(q);
    //     return c;
    //   }

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
      return *this;
    }
    
    bool src_final() const {
      return dfa->final(q);
    }
    
    empty_tag src_tag() const {
      return empty_tag();
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
	  std::pair<typename mapper::iterator, bool> p = 
	    mapping->insert(std::make_pair(regexp_cursor::ordered_state_type(c.src().begin(), c.src().end()), DFA_type::null_state));
#else
	  std::pair<typename mapper::iterator, bool> p = 
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
	}
	else {
	  dfa->set_trans(q, a, my_sink);
	  q = DFA_type::null_state;
	  return false;
	}
      }
      q = tmp;
      return true;
    }
  };

  // match algorithm specializations for regexp

  inline
  bool match(lazy_cursor<regexp_cursor>& c, const char *w)
  {
    if (!c.forward('\0')) // ^ anchoring
      return false;
    for(; *w != '\0' && c.forward(*w); ++w)
      if (c.src_final()) return true;
    return *w == '\0' && c.forward('\0') && c.src_final(); // $ anchoring
  }

  template <typename InputI>
  inline
  bool match(lazy_cursor<regexp_cursor>& c, InputI first, InputI last)
  {
    if (!c.forward('\0'))  // ^ anchoring
      return false;
    for(; first != last && c.forward(*first); ++first)
      if (c.src_final()) return true;
    return first == last && c.forward('\0') && c.src_final(); // $ anchoring
  }

  template <typename ForwardI>
  inline
  ForwardI first_match(lazy_cursor<regexp_cursor> &c, ForwardI first, ForwardI last)
  {
    if (!c.forward('\0')) return first;
    const ForwardI start = first;
    for(; first != last && c.forward(*first); ++first)
      if (c.src_final()) return ++first;
    return first == last && c.forward('\0') && c.src_final() ? first : start;
  }

  inline
  const char*
  first_match(lazy_cursor<regexp_cursor> &c, const char *text)
  {
    if (!c.forward('\0')) return text;
    const char *start = text;
    for(; *text != '\0' && c.forward(*text); ++text)
      if (c.src_final()) return ++text;
    return *text == '\0' && c.forward('\0') && c.src_final() ? text : start;
  }


  inline
  ostream& operator<<(ostream& out, const regexp_cursor& c) {
    for(vector<regexp_cursor::node>::const_iterator n = c.e->begin(); n != c.e->end(); ++n)
      if (c.q.find(const_cast<astl::regexp_cursor::node*>(&*n)) != c.q.end())
	out << "\033[1m" << *n << "\033[0m";
      else
	out << *n;
    return out;
  }

  static string regexp_fcursor_errmsg;

  // le regexp forward cursor ne gère pas les ensembles de lettres du
  // genre [A-Z] (trop cher pour le parcours des transitions sortantes) 
  class regexp_fcursor : public forward_cursor_concept
  {
  protected:
#ifdef _MSC_VER
  public:
#endif
    struct node;
    typedef set<node*> S;

    typedef enum { STAR, CONCAT, OR, REPEAT, EPSILON, 
		   LETTER, OPEN, CLOSE, QUESTION, PLUS,
		   ANY, FINAL } token_type;
  
    struct node {
      token_type type;
      short letter;
      bool annulable;
      const char *pos;
      S premiere_pos, derniere_pos, pos_suivante;
    
      node(token_type t, const char *p = 0)
	: type(t), letter(128),pos(p)
      { }

      node(token_type type_, short letter_, const char *p = 0)
	: type(type_), letter(letter_), annulable(false), pos(p) 
      { }

      bool final() const {
	return type == FINAL;
      }
    };

    S q;  // current state
    smart_ptr<vector<node> > e;  // node "allocator"
    short c; // letter

    node* exp(vector<node>::iterator &first, vector<node>::iterator last);
    node* form(vector<node>::iterator &first, vector<node>::iterator last);
    node* term(vector<node>::iterator &first, vector<node>::iterator last);
    node* exp2(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    node* term2(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    node* repeat(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    template <typename OutputIterator>
    int tokenize(const char *first, const char *last, OutputIterator x);

  public:
    ptrdiff_t errpos;

    friend ostream& print_node(ostream&, const node&);
    typedef regexp_fcursor   self;
    typedef regexp_cursor    super;
    typedef S                state_type;
    typedef plain            char_traits;
    typedef plain::char_type char_type;
    typedef empty_tag        tag_type;
    typedef forward_cursor_concept concept;

    regexp_fcursor(const char *expression = "")
      : errpos(-1)
    {
      if (strlen(expression) > 0) {
	e->reserve(8);
	tokenize(expression, expression + strlen(expression),
		 back_inserter(*e));
	vector<node>::iterator first = e->begin();
	node *root = exp(first, e->end());
	if (root == 0) 
	  errpos = first->pos - expression;
	else 
	  q = root->premiere_pos;
      }
    }

    bool sink() const {
      return q.empty();
    }

    const string& error_message() const {
      return regexp_fcursor_errmsg;
    }

    self& operator=(const state_type &s) {
      q = s;
      return *this;
    }
  
    char_type letter() const {
      return (char_type) c;
    }

    bool first() {
      c = 128;
      for(S::const_iterator i = q.begin(); i != q.end(); ++i) 
	if ((*i)->type == ANY) {
	  c = -128;
	  return true;
	}
	else
	  c = min__(c, (*i)->letter);
      return c != 128;
    }
    
    bool next() {
      short tmp = c;
      c = 128;
      for(S::const_iterator i = q.begin(); i != q.end(); ++i) {
	if ((*i)->type == ANY) {
	  c = tmp + 1;
	  break;
	}
	if ((*i)->letter > tmp)
	  c = min__(c, (*i)->letter);
      }
      return c != 128;
    }

    state_type src() const {
      return q;
    }

    bool src_final() const {
      for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	if ((*i)->type == FINAL) return true;
      return false;
    }   
    
    // Brute force and massive ignorance:
    state_type aim() const { 
      regexp_fcursor tmp = *this;
      tmp.forward();
      return tmp.src();
    }

    // Brute force and massive ignorance:
    bool aim_final() const
    { 
      self tmp = *this;
      tmp.forward();
      return tmp.src_final();
    }

    void forward() {
      forward(c);
    }

    bool exists(char a) const {
      for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	if ((*i)->type == ANY || (*i)->letter == a)
	  return true;
      return false;
    }

    bool forward(char a) {
      state_type p;
      for(S::const_iterator i = q.begin(); i != q.end(); ++i)
	if ((*i)->type == ANY || (*i)->letter == a)
	  p.insert((*i)->pos_suivante.begin(),
		   (*i)->pos_suivante.end());
      q.swap(p);   // optimized equivalent to q = p;
      return !q.empty();
    }

    tag_type aim_tag() const {
      return tag_type();
    }

    bool operator==(const regexp_fcursor &y) const {
      return true;
    }

    tag_type src_tag() const {
      return tag_type();
    }
  };

  // Return error position or -1 if ok
  template <typename OutputIterator>
  int regexp_fcursor::tokenize(const char *first, const char *last, 
			       OutputIterator x)
  {
    *x++ = node(OPEN);
    for(const char *begin = first; first != last; ++first, ++x)
      switch (*first) {
      case '|' : 
	*x = node(OR, first); 
	break;

      case '(' : 
	if (first != begin && first[-1] != '(' && first[-1] != '|') 
	  *x++ = node(CONCAT, *first);
	*x = node(OPEN, first); 
	break;

      case ')' : 
	*x = node(CLOSE, first);
	break;

      case '*' :
	*x = node(STAR, first);
	break;

      case '?' :
	*x = node(QUESTION, first);
	break;

      case '+' :
	*x = node(PLUS, first);
	break;
	// Escaped character:
      case '\\' : 
	if (first != begin && first[-1] != '|' && first[-1] != '(') 
	  *x++ = node(CONCAT, first);
	if (++first == last) return first - begin;
	switch (*first) {
	case 'n' : *x = node(LETTER, '\n', first); break;
	case 't' : *x = node(LETTER, '\t', first); break;
	case 'r' : *x = node(LETTER, '\r', first); break;
	default : *x = node(LETTER, *first, first); 
	}
	break;
      
      default :
	if (first != begin && first[-1] != '|' && first[-1] != '(') 
	  *x++ = node(CONCAT, first);
	if (*first == '.') *x = node(ANY, first);
	else *x = node(LETTER, *first, first); 
	break;
      }
    *x++ = node(CLOSE);
    *x++ = node(CONCAT);
    *x++ = node(FINAL);
    return 0;
  }

  // exp -> term exp2    
  inline
  regexp_fcursor::node*
  regexp_fcursor::exp(vector<regexp_fcursor::node>::iterator &first, 
		      vector<regexp_fcursor::node>::iterator last)
  {
    if (first == last) {
      regexp_fcursor_errmsg = "empty expression";
      return 0;
    }
    node *t = term(first, last);
    if (t == 0) return 0;
    return exp2(first, last, t);
  }

  // exp2 -> + term exp2 | epsilon
  inline
  regexp_fcursor::node*
  regexp_fcursor::exp2(vector<regexp_fcursor::node>::iterator &first, 
		       vector<regexp_fcursor::node>::iterator last, node *left)
  {
    if (first != last && first->type == OR) {
      node &root = *first;
      node *right = term(++first, last);
      if (right == 0) return 0;
      root.annulable = right->annulable || left->annulable;
      // premiere_pos(here) = premiere_pos(left) U premiere_pos(right): 
      root.premiere_pos.insert(left->premiere_pos.begin(),
			       left->premiere_pos.end()); 
      root.premiere_pos.insert(right->premiere_pos.begin(),
			       right->premiere_pos.end()); 
      root.derniere_pos.insert(left->derniere_pos.begin(),
			       left->derniere_pos.end()); 
      root.derniere_pos.insert(right->derniere_pos.begin(),
			       right->derniere_pos.end());
      return exp2(first, last, &root);
    }
    return left;
  }

  // term -> form repeat term2
  inline
  regexp_fcursor::node*
  regexp_fcursor::term(vector<regexp_fcursor::node>::iterator &first, 
		       vector<regexp_fcursor::node>::iterator last)
  {
    if (first == last) return 0;
    node *f = form(first, last);
    if (f == 0) return 0;
    node *s = repeat(first, last, f);  // return f if no repeat token is found
    return term2(first, last, s);
  }

  // repeat -> * repeat | ? repeat | + repeat | {n,m} repeat | epsilon
  inline
  regexp_fcursor::node*
  regexp_fcursor::repeat(vector<regexp_fcursor::node>::iterator &first, 
			 vector<regexp_fcursor::node>::iterator last, node *left)
  {
    if (first != last)
      switch (first->type) {
      case STAR :
	{
	  node &root = *first;
	  root.annulable = true;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	
	  // pos_suivante(derniere_pos(here)) += premiere_pos(here):
	  for(S::const_iterator i = root.derniere_pos.begin(); 
	      i != root.derniere_pos.end(); ++i)
	    (*i)->pos_suivante.insert(root.premiere_pos.begin(),
				      root.premiere_pos.end());
	  return repeat(++first, last, &root);
	}
      case QUESTION : 
	{
	  node &root = *first;
	  root.annulable = true;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	  return repeat(++first, last, &root);
	}
      case PLUS :
	{
	  node &root = *first;
	  root.annulable = false;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	
	  // pos_suivante(derniere_pos(here)) += premiere_pos(here):
	  for(S::const_iterator i = root.derniere_pos.begin(); 
	      i != root.derniere_pos.end(); ++i)
	    (*i)->pos_suivante.insert(root.premiere_pos.begin(),
				      root.premiere_pos.end());
	  return repeat(++first, last, &root);
	}
      default : 
	return left;
      }
    return left;
  }
    
  // term2 -> . form repeat term2 | epsilon
  inline
  regexp_fcursor::node*
  regexp_fcursor::term2(vector<regexp_fcursor::node>::iterator &first, 
			vector<regexp_fcursor::node>::iterator last, node *left)
  {
    if (first != last && first->type == CONCAT) {
      node &root = *first;
      node *f = form(++first, last);
      if (f == 0) return 0;
      node *right = repeat(first, last, f);
      root.annulable = left->annulable && right->annulable;
      if (left->annulable) {
	root.premiere_pos.insert(left->premiere_pos.begin(),
				 left->premiere_pos.end()); 
	root.premiere_pos.insert(right->premiere_pos.begin(),
				 right->premiere_pos.end()); 
      }
      else
	root.premiere_pos = left->premiere_pos;
    
      if (right->annulable) {
	root.derniere_pos.insert(right->derniere_pos.begin(),
				 right->derniere_pos.end()); 
	root.derniere_pos.insert(left->derniere_pos.begin(),
				 left->derniere_pos.end());
      }
      else
	root.derniere_pos = right->derniere_pos;
      // pos_suivante(derniere_pos(left)) += premiere_pos(right):
      for(S::const_iterator i = left->derniere_pos.begin(); 
	  i != left->derniere_pos.end(); ++i)
	(*i)->pos_suivante.insert(right->premiere_pos.begin(),
				  right->premiere_pos.end());
      return term2(first, last, &root);
    }
    return left;
  }


  // form -> ( exp ) | letter
  inline
  regexp_fcursor::node*
  regexp_fcursor::form(vector<regexp_fcursor::node>::iterator &first, 
		       vector<regexp_fcursor::node>::iterator last)
  {
    if (first == last) return 0;
    if (first->type == OPEN) {
      node *e = exp(++first, last);
      if (e == 0) return 0;
      if (first->type == CLOSE) ++first;
      else {
	regexp_fcursor_errmsg = "unbalanced parenthesis";
	return 0;
      }
      return e;
    }
    first->annulable = false;
    first->premiere_pos.insert(&*first);
    first->derniere_pos.insert(&*first);
    ++first;
    return &(first[-1]);
  }

  inline
  regexp_fcursor regexpfc(const string &exp) {
    return regexp_fcursor(exp.c_str());
  }

  inline
  std::ostream& print_node(std::ostream &out, const regexp_fcursor::node &x) 
  {
    switch(x.type) {
    case regexp_fcursor::LETTER :
      out << "[" << x.letter << "]";
      break;

    case regexp_fcursor::OR : 
      out << "|"; 
      break;

    case regexp_fcursor::FINAL : 
      out << "[EoE]";
      break;

    case regexp_fcursor::OPEN : 
      out << "("; 
      break;

    case regexp_fcursor::CLOSE : 
      out << ")"; 
      break;

    case regexp_fcursor::CONCAT : 
      out << " "; 
      break;

    case regexp_fcursor::STAR : 
      out << "*"; 
      break;

    case regexp_fcursor::QUESTION : 
      out << "?"; 
      break;

    case regexp_fcursor::PLUS : 
      out << "+"; 
      break;

    default :
      break;
    } 
    return out;
  }

  static const int EXTRACT_CURSOR_MAX_SUB_EXPRESSION = 16;

  class extract_cursor : public cursor_concept
  {
  public:
    struct node;
    typedef set<node*> S;
    typedef pair<int, int> match;
    struct token
    {
      node *state;
      vector<match> submatches;
      int ccount;  // char count along the way
      token(node *state_ = NULL)
	: state(state_), ccount(0)
      { }
      token(node *state_, const vector<match> &v, int count)
	: state(state_), submatches(v), ccount(count)
      { }
      bool operator<(const token &x) const {
	return state < x.state ||
	  (state == x.state && ccount < x.ccount) ||
	  (ccount == x.ccount && submatches < x.submatches);
      }
    };

    typedef enum { STAR, CONCAT, OR, REPEAT, EPSILON, 
		   LETTER, OPEN, CLOSE, QUESTION, PLUS,
		   ANY, FINAL } token_type;
  
    struct node {
      token_type type;
      bitset<256> letter;
      bool annulable;
      const char *pos;
      S premiere_pos, derniere_pos, pos_suivante;
      bitset<EXTRACT_CURSOR_MAX_SUB_EXPRESSION> begin, end;  // starting & ending subexpressions
      int subexpression; // subexpression number if token == '('
    
      node(token_type t, const char *p = 0, int sub = 0) //, int n_ = 0, int m_ = 0)
	: type(t), pos(p), subexpression(sub) { 
	if (type == ANY) {
	  type = LETTER;
	  letter.set();
	}
      }

      node(char t, const char *p = 0)
	: type(LETTER), annulable(false), pos(p) {
	letter.set((unsigned char) t);
      }

      node(const bitset<256> &t, const char *p = 0)
	: type(LETTER), letter(t), annulable(false), pos(p)
      { }

      bool final() const {
	return type == FINAL;
      }

      char to_char() const {   // does not work with ranges
	size_t i;
	for(i = 0; i != letter.size() && letter.test(i) == false; ++i);
	return (char) (-128 + i);
      }
    };

  public:
    typedef set<token> state_type;

  protected:
    set<token>                 q;  // current state
    smart_ptr<vector<node> >   e;  // node "allocator"
    int                        sub_count; // subexpression count
    bitset<EXTRACT_CURSOR_MAX_SUB_EXPRESSION> statics;
    // sub with static start, i.e. different from (A)*

    node* exp(vector<node>::iterator &first, vector<node>::iterator last);
    node* form(vector<node>::iterator &first, vector<node>::iterator last);
    node* term(vector<node>::iterator &first, vector<node>::iterator last);
    node* exp2(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    node* term2(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    node* repeat(vector<node>::iterator &first, vector<node>::iterator last, node *left);
    template <typename OutputIterator>
    int tokenize(const char *first, const char *last, OutputIterator x, 
		 bool kmp_style = false);

  public:
    friend std::ostream& operator<<(std::ostream &, const
				    extract_cursor::node &);
    ptrdiff_t errpos;

    typedef extract_cursor    self;
    typedef plain            char_traits;
    typedef plain::char_type char_type;
    typedef vector<match>    tag_type;

    extract_cursor(const string &expression = ".", bool kmp_style = false, 
		   bool verbose = false)
      : sub_count(0), errpos(-1)
    {
      statics.set();
      e->reserve(128);
      tokenize(expression.c_str(), expression.c_str() + expression.size(),
	       back_inserter(*e), kmp_style);
      if (verbose) {
	cout << "tokenized expression:" << endl;
	copy(e->begin(), e->end(), ostream_iterator<node>(cout));
	cout << endl;
      }
      vector<node>::iterator first = e->begin();
      node *root = exp(first, e->end());
      if (root == NULL) 
	errpos = first->pos - expression.c_str();
      else {
	for(set<node*>::iterator i = root->premiere_pos.begin(); i != root->premiere_pos.end(); ++i)
	  q.insert(token(*i));
      }
    }

    ptrdiff_t error() const { // possible error position (-1 if ok)
      return errpos;
    }

    self& operator=(const state_type &s) {
      q = s;
      return *this;
    }
  
    state_type src() const {
      return q;
    }

    bool exists(char a) const {
      for(set<token>::const_iterator i = q.begin(); i != q.end(); ++i)
	if (i->state->letter[(unsigned char) a] == true)
	  return true;
      return false;
    }

    bool forward(char letter) {
      static vector<match> tmp(EXTRACT_CURSOR_MAX_SUB_EXPRESSION);
      set<token> p;
      for(set<token>::iterator i = q.begin(); i != q.end(); ++i) {
	if (i->state->letter[(unsigned char) letter] == true) {
	  tmp = i->submatches;
	  if (i->state->begin.any() || i->state->end.any())
	    for(int j = 0; j < subexp_count(); ++j) {
	      if (i->state->begin[j]) { // sub-expression j starts here ?
		if ((unsigned int) j >= tmp.size()) {
		  tmp.resize(j + 1, match(-1, -1));
		  tmp[j].first = i->ccount;
		}
		else
		  if (tmp[j].first == -1 || statics[j] == false)
		    tmp[j].first = i->ccount;  // update start position
	      }
	      if (i->state->end[j])   // sub-expression j ends here ?
		tmp[j].second = i->ccount + 1;
	    }
	  for(S::const_iterator k = i->state->pos_suivante.begin();
	      k != i->state->pos_suivante.end(); ++k)
	    p.insert(token(*k, tmp, i->ccount + 1));
	}
      }
      q.swap(p);
      return !q.empty();
    }

    int subexp_count() const {
      return sub_count;  // always at least one subexpression
    }

    vector<pair<int, int> > submatches() const {
      for(set<token>::const_iterator i = q.begin(); i != q.end(); ++i)
	if (i->state->final()) return i->submatches;
      return vector<match>();
    }

    bool src_final() const {
      for(set<token>::const_iterator i = q.begin(); i != q.end(); ++i)
	if (i->state->final()) return true;
      return false;
    }
     
    bool sink() const {
      return q.empty();
    }
 
    tag_type src_tag() const {
      return submatches();
    }
  };

  inline
  std::ostream& operator<<(std::ostream &out, const extract_cursor::node &x) 
  {
    switch(x.type) {
    case extract_cursor::LETTER :
      out << "[";
      if (x.letter.count() == 256)   // Sigma
	out << "Sigma";
      else
	for(int i = 32; i < 256; ++i)
	  if (x.letter.test(i) == true) out << (char) i;
      out << "]";
      break;

    case extract_cursor::OR : 
      out << "|"; 
      break;

    case extract_cursor::FINAL : 
      out << "[EoE]";
      break;

    case extract_cursor::OPEN : 
      out << "("; 
      break;

    case extract_cursor::CLOSE : 
      out << ")"; 
      break;

    case extract_cursor::CONCAT : 
      out << " "; 
      break;

    case extract_cursor::STAR : 
      out << "*"; 
      break;

    case extract_cursor::QUESTION : 
      out << "?"; 
      break;

    case extract_cursor::PLUS : 
      out << "+"; 
      break;

      //    case extract_cursor::REPEAT : 
      //      out << "{" << x.n << "," << x.m << "}";
      //      break;

    default :
      break;
    } 
    return out;
  }

  // Return error position or -1 if ok
  template <typename OutputIterator>
  int extract_cursor::tokenize(const char *first, const char *last, 
			       OutputIterator x, bool kmp_style)
  {
    int subexpression = 0;

    if (kmp_style) {  // prepend Sigma*
      *x++ = node(ANY);
      *x++ = node(STAR);
      *x++ = node(CONCAT); 
    }
    *x++ = node(OPEN, NULL, subexpression++); // the overall subexpression
    for(const char *begin = first; first != last; ++first, ++x)
      switch (*first) {
      case '|' : 
	*x = node(OR, first); 
	break;

      case '(' : 
	if (first != begin && first[-1] != '(' && first[-1] != '|') 
	  *x++ = node(CONCAT, first);
	*x = node(OPEN, first, subexpression++); 
	break;

      case ')' : 
	*x = node(CLOSE, first);
	break;

      case '*' :
	*x = node(STAR, first);
	break;

      case '?' :
	*x = node(QUESTION, first);
	break;

      case '+' :
	*x = node(PLUS, first);
	break;

	/* not yet implemented 
	   case '{' :
	   if (++first == last) return first - begin;
	   {
	   int n = 0;
	   const char *tmp = first;
	   while (isdigit(*first)) {
	   n = n * 10 + *first - '0';
	   if (++first == last) return first - begin;
	   }
	   if (tmp == first) return first - begin;
	   int m = n;
	   if (*first == ',') {
	   if (++first == last) return first - begin;
	   tmp = first;
	   for(m = 0; isdigit(*first);) {
	   m = m * 10 + *first - '0';
	   if (++first == last) return first - begin;
	   }
	   if (tmp == first) return first - begin;
	   }
	   if (*first != '}') return first - begin;
	   if (n > m) return first - begin;
	   *x = node(REPEAT, first, n, m);
	   }
	   break;
	*/

      case '[' :   // characters set
	{
	  if (first != begin && first[-1] != '|' && first[-1] != '(') 
	    *x++ = node(CONCAT, first);
	  bitset<256> a;
	  bool v = true;
	  if (++first == last) return first - begin;
	  if (*first == '^') {
	    a.set();
	    v = false;
	    if (++first == last) return first - begin;
	  }
	  while (first != last && *first != ']') {
	    const unsigned char b = (unsigned char) *first;
	    if (++first == last) break;
	    if (*first != '-') {
	      a.set(b, v);
	      continue;
	    }
	    if (++first == last || *first == ']' || *first < b) return first - begin;
	    for(unsigned int i = b; i <= (unsigned char) *first; ++i)
	      a.set(i, v);
	    ++first;
	  }
	  if (first == last) return first - begin; // first is supposed to point to ']'
	  *x++ = node(a);
	  break;
	}
	  

	// Escaped character:
      case '\\' : 
	if (first != begin && first[-1] != '|' && first[-1] != '(') 
	  *x++ = node(CONCAT, first);
	if (++first == last) return first - begin;
	switch (*first) {
	case 'n' : *x = node('\n', first); break;
	case 't' : *x = node('\t', first); break;
	case 'r' : *x = node('\r', first); break;
	default : *x = node(*first, first); 
	}
	break;
      
      default :
	if (first != begin && first[-1] != '|' && first[-1] != '(') 
	  *x++ = node(CONCAT, first);
	if (*first == '.') *x = node(ANY, first);
	else
	  *x = node(*first, first); 
	break;
      }
    *x++ = node(CLOSE);
    *x++ = node(CONCAT);
    *x++ = node(FINAL);
    return 0;
  }

  // exp -> term exp2    
  inline
  extract_cursor::node*
  extract_cursor::exp(vector<extract_cursor::node>::iterator &first, 
		      vector<extract_cursor::node>::iterator last)
  {
    if (first == last) return NULL;
    node *t = term(first, last);
    if (t == 0) return 0;
    return exp2(first, last, t);
  }

  // exp2 -> + term exp2 | epsilon
  inline
  extract_cursor::node*
  extract_cursor::exp2(vector<extract_cursor::node>::iterator &first, 
		       vector<extract_cursor::node>::iterator last, node *left)
  {
    if (first != last && first->type == OR) {
      node &root = *first;
      node *right = term(++first, last);
      if (right == 0) return 0;
      root.annulable = right->annulable || left->annulable;
      // premiere_pos(here) = premiere_pos(left) U premiere_pos(right): 
      root.premiere_pos.insert(left->premiere_pos.begin(),
			       left->premiere_pos.end()); 
      root.premiere_pos.insert(right->premiere_pos.begin(),
			       right->premiere_pos.end()); 
      root.derniere_pos.insert(left->derniere_pos.begin(),
			       left->derniere_pos.end()); 
      root.derniere_pos.insert(right->derniere_pos.begin(),
			       right->derniere_pos.end());
      return exp2(first, last, &root);
    }
    return left;
  }

  // term -> form repeat term2
  inline
  extract_cursor::node*
  extract_cursor::term(vector<extract_cursor::node>::iterator &first, 
		       vector<extract_cursor::node>::iterator last)
  {
    if (first == last) return 0;
    node *f = form(first, last);
    if (f == 0) return 0;
    node *s = repeat(first, last, f);  // return f if no repeat token is found
    return term2(first, last, s);
  }

  // repeat -> * repeat | ? repeat | + repeat | epsilon
  inline
  extract_cursor::node*
  extract_cursor::repeat(vector<extract_cursor::node>::iterator &first, 
			 vector<extract_cursor::node>::iterator last, node *left)
  {
    if (first != last)
      switch (first->type) {
      case STAR :
	{
	  if (left->type == OPEN) statics &= ~left->begin;

	  node &root = *first;
	  root.annulable = true;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	
	  // pos_suivante(derniere_pos(here)) += premiere_pos(here):
	  for(S::const_iterator i = root.derniere_pos.begin(); 
	      i != root.derniere_pos.end(); ++i)
	    (*i)->pos_suivante.insert(root.premiere_pos.begin(),
				      root.premiere_pos.end());
	  return repeat(++first, last, &root);
	}
      case QUESTION : 
	{
	  node &root = *first;
	  root.annulable = true;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	  return repeat(++first, last, &root);
	}
      case PLUS :
	{
	  if (left->type == OPEN) statics &= ~left->begin;

	  node &root = *first;
	  root.annulable = false;
	  root.premiere_pos = left->premiere_pos;
	  root.derniere_pos = left->derniere_pos;
	
	  // pos_suivante(derniere_pos(here)) += premiere_pos(here):
	  for(S::const_iterator i = root.derniere_pos.begin(); 
	      i != root.derniere_pos.end(); ++i)
	    (*i)->pos_suivante.insert(root.premiere_pos.begin(),
				      root.premiere_pos.end());
	  return repeat(++first, last, &root);
	}
      default : 
	return left;
      }
    return left;
  }
    

  // term2 -> . form repeat term2 | epsilon
  inline
  extract_cursor::node*
  extract_cursor::term2(vector<extract_cursor::node>::iterator &first, 
			vector<extract_cursor::node>::iterator last, node *left)
  {
    if (first != last && first->type == CONCAT) {
      node &root = *first;
      node *f = form(++first, last);
      if (f == 0) return 0;
      node *right = repeat(first, last, f);
      root.annulable = left->annulable && right->annulable;
      if (left->annulable) {
	root.premiere_pos.insert(left->premiere_pos.begin(),
				 left->premiere_pos.end()); 
	root.premiere_pos.insert(right->premiere_pos.begin(),
				 right->premiere_pos.end()); 
      }
      else
	root.premiere_pos = left->premiere_pos;
    
      if (right->annulable) {
	root.derniere_pos.insert(right->derniere_pos.begin(),
				 right->derniere_pos.end()); 
	root.derniere_pos.insert(left->derniere_pos.begin(),
				 left->derniere_pos.end());
      }
      else
	root.derniere_pos = right->derniere_pos;
      // pos_suivante(derniere_pos(left)) += premiere_pos(right):
      for(S::const_iterator i = left->derniere_pos.begin(); 
	  i != left->derniere_pos.end(); ++i)
	(*i)->pos_suivante.insert(right->premiere_pos.begin(),
				  right->premiere_pos.end());
      return term2(first, last, &root);
    }
    return left;
  }


  // form -> ( exp ) | letter
  inline
  extract_cursor::node*
  extract_cursor::form(vector<extract_cursor::node>::iterator &first, 
		       vector<extract_cursor::node>::iterator last)
  {
    if (first == last) return 0;
    if (first->type == OPEN) {
      int tmp = first->subexpression;
      vector<extract_cursor::node>::iterator tmp_first = first;
      node *e = exp(++first, last);
      if (e == 0) return 0;
      if (first->type == CLOSE) ++first;
      else return NULL; // unbalanced parenthesis

      if (tmp < EXTRACT_CURSOR_MAX_SUB_EXPRESSION) {
	for(S::iterator i = e->premiere_pos.begin(); 
	    i != e->premiere_pos.end(); ++i)
	  (*i)->begin[tmp] = true;
    
	for(S::iterator ii = e->derniere_pos.begin(); 
	    ii != e->derniere_pos.end(); ++ii)
	  (*ii)->end[tmp] = true;
	sub_count = max__(sub_count, tmp + 1);
      }    
      *tmp_first = *e;
      tmp_first->type = OPEN;
      return &*tmp_first;
    }
    first->annulable = false;
    first->premiere_pos.insert(&*first);
    first->derniere_pos.insert(&*first);
    ++first;
    return &(first[-1]);
  }

  inline
  extract_cursor extractc(const string &exp, bool kmp_style) {
    return extract_cursor(exp.c_str(), kmp_style);
  }

  inline
  extract_cursor extractc(const string &exp) {
    return extract_cursor(exp.c_str());
  }

  inline
  extract_cursor extractc() {
    return extract_cursor();
  }

  template <typename RandomAccessIterator>
  inline 
  vector<pair<RandomAccessIterator, RandomAccessIterator> > 
  extract(extract_cursor &c, 
	  RandomAccessIterator i, RandomAccessIterator j) 
  {
    vector<pair<RandomAccessIterator, RandomAccessIterator> > r;
    return r;
  }

  inline
  ostream& operator<<(ostream &out, const extract_cursor::token &x) 
  {
    return out << x.state;
  }

}

#endif
