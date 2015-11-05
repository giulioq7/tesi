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

#ifndef ASTL_TOOLS_H
#define ASTL_TOOLS_H

#ifdef _MSC_VER
// don't understand this WC++ warning, let's kill it:
#pragma warning( disable : 4800 ) // forcing value to bool true or false
#endif


#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <bitset>
#include <map>
#include <iterator>
#include <cstdio>
#include <ctime>
#include <cwchar>
#include <functional>

#ifndef _MSC_VER
#include <unistd.h>   // getopt
#include <sys/mman.h>
#include <fcntl.h>
#define POPEN popen
#define PCLOSE pclose
#define max__ max
#define min__ min
#else
#include <cstdio>
#define POPEN _popen
#define PCLOSE _pclose
#define max__ _MAX
#define min__ _MIN
#endif

using namespace std;

namespace astl {

#if 0
  class multiple_horspool_finder
  {
    multiple_horspool_finder()
    { }

    multiple_horspool_finder(const vector<string> &s) {
      init(s);
    }
    
    void init(const vector<string> &s) {
      finders.clear();
      finders.reserve(s.size());
      for(vector<string>::const_iterator i = s.begin(); i != s.end(); ++i)
	finders.push_back(horspool_finder(*i));
    }

    // returns beg of match
    pair<const char*, const char*> find(const char *first, const char *last) const {
      while (true) {
	int shift = numeric_limits<int>::max();
	int match = 0;
	for(vector<horspool_finder>::const_iterator finder = 0; finder != finders.size(); ++finder) {
	  const char *finish = first + finder->size() - 1;
	  if (finish < last) {
	    const int s = finder->step(finish, last);
	    if (s < 0) 
	      match = max(match, finder->size());
	    else shift = min(shift, s);
	  }
	}
	if (match != 0)
	  return make_pair(first, first + finder->size());
	if (shift == numeric_limits<int>::max())
	  break;
	first += shift;
      }
      return last; // no match
    }

    /*  
    fill(positions.begin(), positions.end(), first);
      int candidate = -1;
      for(vector<horspool_finder*>::size_type finder = 0; finder != finders.size(); ++finder) 
	if (positions[finder] < last - finders[finder]->size()) {
	  const int shift = step(positions[finder], last);
	  if (shift < 0) {
	    if (candidate == -1 || positions[finder] < positions[candidate] ||
		positions[finder] == positions[candidate] && finders[finder]->size() < finders[candidate]->size())
	      candidate = finder;
	  }
	  else positions[finder] += shift;
	}
      if (candidate != -1)
	return positions[candidate];
    }
    */

  protected:
    vector<horspool_finder> finders;
  };
#endif

  class horspool_finder
  {
  public:
    horspool_finder()
    { }


    horspool_finder(const std::string &s) {
      init(s);
    }


    void init(const std::string &s) {
      word = s;
      last_position = --word.end();
      shift.clear();
      shift.resize(256, s.size());
      shifts();
    }


#if 0
    horspool_finder(const std::vector<bitset<256> > &s) {
      init(s);
    }


    void init(const std::string &s) {
      word.resize(s.size());
      for(std::string::size_type i = 0; i < s.size(); ++i)
	word[i].set((unsigned char) s[i]);
      init(word);
    }

    void init(const std::vector<bitset<256> > &s) {
      word = s;
      last_position = --word.end();
      shift.clear();
      shift.resize(256, s.size());
      shifts();
    }
#endif
    unsigned int size() const { return word.size(); }

    std::string::const_iterator find(const std::string &s) const {
      return find(s.begin(), s.end());
    }

//     const char* find(const char *text, char end) const {
//       for(text = std::find(text, text + size() - 1, end); 
// 	  *text != end; 
// 	  text = std::find(text, text + shift[(unsigned char) *text], end)) {
// 	const char *x = text;
// 	for(std::string::const_iterator y = last_position; *x == *y; --x, --y)
// 	  if (y == word.begin())
// 	    return text - (size() - 1);
//       }
//       return text;
//     }


    // returns beg of match
    template <typename RandomAccessI>
    RandomAccessI find(RandomAccessI first, RandomAccessI last) const {
      for(first += size() - 1; first < last; first += shift[(unsigned char) *first]) {
	RandomAccessI x = first;
	for(std::string::const_iterator y = last_position; *x == *y; --x, --y)
	  if (y == word.begin())
	    return first - (size() - 1);
      }
      return last;
    }

#if 0
    // returns beg of match
    template <typename RandomAccessI>
    RandomAccessI find(RandomAccessI first, RandomAccessI last) const {
      for(first += size() - 1; first < last; first += shift[(unsigned char) *first]) {
	RandomAccessI x = first;
	for(std::vector<bitset<256> >::const_iterator y = last_position; y->test((unsigned char) *x); --x, --y)
	  if (y == word.begin())
	    return first - (size() - 1);
      }
      return last;
    }
#endif
//     template <typename RandomAccessI>
//     int step(RandomAccessI first, RandomAccessI last) const {
//       const int s = shift[(unsigned char) *first]; 
//       for(std::string::const_iterator y = last_position; *first == *y; --first, --y)
// 	if (y == word.begin())
// 	  return -s;
//       return s;
//     }

  protected:


    std::string word;
    std::vector<int> shift;
    std::string::const_iterator last_position;


#if 0
    std::vector<bitset<256> > word;
    std::vector<int> shift;
    std::vector<bitset<256> >::const_iterator last_position;
#endif

    void shifts() {
      for (std::string::size_type i = 0; i < word.size() - 1; ++i)
	shift[(unsigned char) word[i]] = word.size() - 1 - i;
    }

#if 0
    void shifts() {
      for(std::string::size_type i = 0; i < word.size() - 1; ++i)
	for(int j = 0; j < 256; ++j)
	  if (word[i].test(j))
	    shift[j] = word.size() - 1 - i;
    }
#endif

  };

  class boyer_moore_finder
  {
  public:
    boyer_moore_finder()
    { }

    boyer_moore_finder(const std::string &s) { 
      init(s);
    }

    void init(const std::string &s) {
      word = s;
      bmGs.resize(s.size());
      bmBc.clear();
      bmBc.resize(256, s.size());
      good_suffix_shifts();
      bad_character_shifts();
    }

    unsigned int size() const { return word.size(); }

    std::string::const_iterator find(const std::string &s) const {
      return find(s.begin(), s.end());
    }

    // returns beg of match
    template <typename RandomAccessI>
    RandomAccessI find(RandomAccessI first, RandomAccessI last) const {
      const int m = size() - 1;
      last -= m;
      int i;
      while (first < last) {
	for (i = m; i >= 0 && word[i] == first[i]; --i);
	if (i < 0)
	  return first;
	else 
	  first += std::max(bmGs[i], bmBc[(unsigned char) first[i]] - m + i);
      }
      return last + m;
    }

  protected:
    std::string word;
    std::vector<int> bmGs, bmBc;

    void bad_character_shifts() {
      for (std::string::size_type i = 0; i < word.size() - 1; ++i)
	bmBc[(unsigned char) word[i]] = word.size() - i - 1;
    }
 
    void suffixes(std::vector<int>& suff) {
      const int m = (int) word.size();
      int f = 0, g, i;    
      suff[m - 1] = m;
      g = m - 1;
      for (i = m - 2; i >= 0; --i) {
	if (i > g && suff[i + m - 1 - f] < i - g)
	  suff[i] = suff[i + m - 1 - f];
	else {
	  if (i < g)
	    g = i;
	  f = i;
	  while (g >= 0 && word[g] == word[g + m - 1 - f])
	    --g;
	  suff[i] = f - g;
	}
      }
    }

    void good_suffix_shifts() {
      const int m = (int) word.size();
      int i, j;
      std::vector<int> suff(m);
      suffixes(suff);
      for (i = 0; i < m; ++i)
	bmGs[i] = m;
      j = 0;
      for (i = m - 1; i >= 0; --i)
	if (suff[i] == i + 1)
	  for (; j < m - 1 - i; ++j)
	    if (bmGs[j] == m)
	      bmGs[j] = m - 1 - i;
      for (i = 0; i <= m - 2; ++i)
	bmGs[m - 1 - suff[i]] = m - 1 - i;
    }
  };

  // Split a line into a (possibly empty) vector of (possibly empty) strings
  inline
  int cut(const string &line, vector<string> &fields, char sep = '\t')
  {
    fields.clear();
    string::size_type j;
    for(string::size_type i = 0; i < line.size(); i = j + 1) {
      j = line.find(sep, i);
      if (j == string::npos) j = line.size();
      fields.push_back(line.substr(i, j - i));
    }
    return (int) fields.size();
  }

  inline
  int cut(const wstring &line, vector<wstring> &fields, wchar_t sep = L'\t')
  {
    fields.clear();
    wstring::size_type j;
    for(wstring::size_type i = 0; i < line.size(); i = j + 1) {
      j = line.find(sep, i);
      if (j == wstring::npos) j = line.size();
      fields.push_back(line.substr(i, j - i));
    }
    return (int) fields.size();
  }

  inline
  int cut(const char *line, vector<string> &fields, char sep = '\t')
  {
    fields.clear();
    const char *j = line;
    const char *i = line;
    do {
      if (*i == sep || (*i == 0 && i != line)) {
	fields.push_back(string(j, i));
	j = i + 1;
      }
    } while (*i++ != '\0');
    return (int) fields.size();
  }

  // Define class smart pointer with following properties:
  // 1. A smart pointer allocates automatically the pointed object
  // 2. A smart pointer shares the pointed object with its copies
  // 3. A smart pointer deallocates the pointed object when no other
  //    smart pointer points to the object
  // 4. operator== returns true if both pointers point to the same
  //    object

  template <typename T>
  class smart_ptr
  {
  protected:
    struct object
    {
      T data;
      unsigned int count;

      object()
	: count(1)
      { }
    };

    object *p;

  public:
    typedef smart_ptr self;

    smart_ptr()
      : p(new object)
    { }

    smart_ptr(const self &x)
      : p(x.p) {
      ++p->count;
    }

    ~smart_ptr() { if (--p->count == 0) delete p; }

    self& operator=(const self &x) {
      if (p != x.p) {
	if (--p->count == 0) delete p;
	p = x.p;
	++p->count;
      }
      return *this;
    }

    void reset() {
      this->~smart_ptr<T>();
      p = new object;
    }

    bool     operator==(const self &x) const { return p == x.p;  }
    T&       operator*() { return p->data; }
    const T& operator*() const { return p->data; }
    T*       operator->() { return &(p->data);  }
    const T* operator->() const { return &(p->data); }
  };


  // Make sure a C++ builtin data type is constructed with a default value:
  // #ifndef _MSC_VER
  template <class T, T value = (T) 0>
  // #else
  // template <class T>
  // #endif
  struct safe
  {
    T q;
    safe(T p)
      : q(p)
    { }

    // #ifndef _MSC_VER
    safe()
      : q(value)
    { }
    // #else
    //   safe()
    //     : q(0)
    //   { }
    // #endif

    operator T () const { return q; }
    operator T ()       { return q; }

    safe& operator=(T p) {
      q = p;
      return *this;
    }

    safe& operator+=(T p) {
      q += p;
      return *this;
    }

    safe& operator++() {
      ++q;
      return *this;
    }

    //   safe& operator+=(int i) {
    //     q += i;
    //     return *this;
    //   }

    safe operator+(safe &x) const {
      return safe(q + x.q);
    }

    bool operator<(safe &x) const {
      return q < x.q;
    }

    safe& operator--() {
      --q;
      return *this;
    }
  };

  // Equivalent to popen(command, "r") (RTFM)
  // remove the last \n if any
  static inline string execute(const string &command)
  {
    FILE *f = POPEN(command.c_str(), "r");
    if (f == NULL) return "";
    string r;
    size_t n;
    for(char buffer[1024];
	(n = fread(buffer, sizeof(char), 1023, f)) > 0;
	r += buffer)
      buffer[n] = '\0';
    PCLOSE(f);
    if (r[r.size() - 1] == '\n')
      r.erase(r.size() - 1);
    return r;
  }

  // template <typename T>
  // class mapable_vector
  // {
  // protected:
  //   off_t offset; // offset in file for mmap
  //   T   *first, *last, *eos;
  //   ostream *output;

  // public:
  //   typedef size_t   size_type;
  //   typedef T        value_type;
  //   typedef T*       iterator;
  //   typedef const T* const_iterator;
  //   typedef T&       reference;
  //   typedef const T& const_reference;
  //   typedef mapable_vector self;

  //   mapable_vector()
  //     : offset(0), first(NULL), last(NULL), eos(NULL), output(NULL) 
  //   { }

  //   mapable_vector(ostream &out) // push back direct to disk
  //     : offset(sizeof(size_type)), first(NULL), last(NULL), eos(NULL), output(&out) { 
  //     size_type dummy = ~0; // reserved for size
  //     output->write((const char*) &dummy, sizeof(size_type));
  //   }

  //   mapable_vector(const self &x)
  //     : offset(0), first(NULL), last(NULL), eos(NULL), output(x.output) {
  //     if (x.first != NULL) {
  //       first = new T[x.capacity()];
  //       last = copy(x.first, x.last, first);
  //       eos = first + x.capacity();
  //     }
  //   }

  //   mapable_vector(size_type s)
  //     : offset(0), output(NULL) {
  //     first = new T[s];
  //     last = eos = first + s;
  //     memset(first, 0, s * sizeof(T));
  //   }

  //   void clear() {
  // #ifndef _MSC_VER
  //     if (mapped())
  //       munmap();
  //     else
  // #endif
  //       {
  // 	delete [] first;
  // 	first = last = eos = NULL;
  //       }
  //   }

  //   self& operator=(const self &x) {
  //     if (&x != this) {
  //       clear();  // set first to NULL
  //       if (!x.empty()) first = new T[x.size()];
  //       last = eos = first + x.size();
  //       copy(x.first, x.last, first);
  //     }
  //     return *this;
  //   }

  //   size_type size() const { return last - first; }

  //   bool empty() const { return size() == 0; }

  //   T& back() { return last[-1]; }

  //   const T& back() const { return last[-1]; }

  //   T& front() { return *first; }

  //   const T& front() const { return *first; }

  //   T& operator[](size_type s) { return first[s]; }

  //   const T& operator[](size_type s) const { return first[s]; }

  //   bool write(ostream& out) const {
  //     size_type s = size();
  //     out.write((const char*) &s, sizeof(size_type));
  //     out.write((const char*) first, sizeof(T) * s);
  //     return (bool) out;
  //   }

  //   bool read(istream& in) {
  //     size_type s;
  //     in.read((char*) &s, sizeof(size_type));
  //     if (in.gcount() != sizeof(size_type)) return false;
  //     clear();
  //     if (s > 0)
  //       first = new T[s];
  //     else first = 0;
  //     last = eos = first + s;
  //     in.read((char*) first, sizeof(T) * s);
  //     return (size_type) in.gcount() == sizeof(T) * s;
  //   }

  // #ifndef _MSC_VER
  //   bool mmap(int file_desc) {
  //     off_t off = lseek(file_desc, 0, SEEK_CUR);
  //     delete [] first;
  //     size_type p;
  //     ::read(file_desc, &p, sizeof(size_type));
  //     off += sizeof(size_type);
  //     offset = off % getpagesize();
  //     char *ptr = (char*) ::mmap(0, p * sizeof(T),
  // 			       PROT_READ, MAP_PRIVATE, file_desc, off - offset);
  //     if (ptr == (char*) -1) return false;
  //     first = (T*) (ptr + offset);
  //     last = eos = first + p;
  //     lseek(file_desc, sizeof(T) * p, SEEK_CUR);
  //     return true;
  //   }

  //   void munmap() {
  //     if (mapped()) {
  //       ::munmap(((char*) first) - offset, size() * sizeof(T));
  //       first = last = eos = NULL;
  //       offset = -1;
  //     }
  //   }
  // #endif

  //   size_type capacity() const { return eos - first; }

  //   void resize(size_type s, const T& x) {
  //     if (s < size()) erase(begin() + s, end());
  //     else insert(end(), s - size(), x);
  //   }

  //   void erase(iterator i, iterator j) {
  //     copy(j, last, first);
  //     last -= j - i;
  //   }

  //   void insert(iterator i, size_type s, const T& x = T()) {
  //     if (s + size() > capacity()) {
  //       T *p = new T[s + size()];
  //       copy(first, i, p);
  //       copy_backward(i, last, p + (i - first) + s);
  //       i = p + (i - first);
  //       last = p + size() + s;
  //       delete [] first;
  //       first = p;
  //       eos = last;
  //     }
  //     else {
  //       copy_backward(i, last, i + s);
  //       last += s;
  //     }
  //     fill(i, i + s, x);
  //   }

  //   template <typename InputI>
  //   void push_back(InputI first, InputI last) {
  //     for(; first != last; ++first)
  //       push_back(*first);
  //   }

  //   void finalize() {
  //     // UGLY HACK:
  //     size_type s = offset;
  //     output->seekp(-offset, ios_base::cur);
  //     output->write((const char*)&s, sizeof(s));
  //     output->seekp(offset - sizeof(s), ios_base::cur);
  //   }

  //   iterator begin() { return first; }

  //   iterator end() { return last; }

  //   const_iterator begin() const { return first; }

  //   const_iterator end() const { return last; }

  //   void push_back(const T& x) {
  //     if (output != NULL) {
  //       output->write((const char*) &x, sizeof(x));
  //       offset += sizeof(x);
  //     }
  //     else {
  //       if (last == eos) {
  // 	T *p = new T[max__((size_type) 1, size()) * 2];
  // 	copy(first, last, p);
  // 	delete [] first;
  // 	eos = p + 2 * max__((size_type) 1, size());
  // 	last = p + (last - first);
  // 	first = p;
  //       }
  //       *last++ = x;
  //     }
  //   }

  //   void reserve(size_type i) {
  //     if (i > capacity()) {
  //       T *p = new T[i];
  //       copy(first, last, p);
  //       delete [] first;
  //       last = p + (last - first);
  //       first = p;
  //       eos = first + i;
  //     }
  //   }

  //   bool mapped() const { return offset != -1; }

  //   ~mapable_vector() { clear(); }
  // };

  // // A compact table maps integers to static arrays of element of type T
  // // To use the read/write functionalities, T must be a POD type
  // // For example, say t is a compact table, the sequence #4 is defined
  // // by the range t[4],t[5] or t.range(4)
  // // a mapped compact table is a static object
  // template <typename T, typename SizeType = unsigned int>
  // class compact_table
  // {
  // protected:
  //   mapable_vector<T>        data;
  //   mapable_vector<SizeType> index;
  //   int fd;          // file descriptor for memory mapping
  //   ostream *output; // push_back "direct to file"

  //   bool mapped() const { return fd > -1; }

  // public:
  //   typedef SizeType size_type;
  //   typedef compact_table<T> self;

  //   compact_table(ostream &out) 
  //     : data(out), fd(-1), output(&out) {
  //     index.push_back(0);
  //   }

  //   compact_table()
  //     : fd(-1), output(NULL) {
  //     index.push_back(0);
  //   }

  //   compact_table(const self &x)
  //     : data(x.data), index(x.index), fd(-1), output(x.output) {
  //   }

  //   void reserve(size_type s) { index.reserve(s); }

  //   ~compact_table() {
  // #ifndef _MSC_VER
  //     munmap();
  // #endif
  //   }

  //   void clear() {
  // #ifndef _MSC_VER
  //     munmap();
  // #endif
  //   }

  //   size_type size() const { return index.size() - 1; }

  //   bool empty() const { return size() == 0; }

  //   bool mmap(const string &path) {
  //     fd = open(path.c_str(), O_RDONLY);
  //     return fd != -1 && mmap(fd);
  //   }

  //   bool mmap(int f) {
  //     return data.mmap(f) && index.mmap(f);
  //   }

  // #ifndef _MSC_VER
  //   void munmap() {
  //     if (fd != -1) {
  //       data.munmap();
  //       index.munmap();
  //       close(fd);
  //       fd = -1;
  //     }
  //   }
  // #endif

  //   bool read(istream& i) {
  //     fd = -1;
  //     return data.read(i) && index.read(i);
  //   }

  //   bool write(ostream& o) const {
  //     return data.write(o) && index.write(o);
  //   }

  //   const T* operator[](size_type i) const { return &data[index[i]]; }

  //   pair<const T*, const T*> range(size_type i) const {
  //     return make_pair((*this)[i], (*this)[i + 1]);
  //   }

  //   template <typename OutputIterator>
  //   OutputIterator find_and_write(size_type i, OutputIterator x) const {
  //     return copy((*this)[i], (*this)[i + 1] - 1, x);
  //   }

  //   size_type capacity() const { return data.size(); }

  //   template <typename RandomAccessIterator>
  //   void push_back(RandomAccessIterator start, RandomAccessIterator finish)
  //   {
  //     data.push_back(start, finish);
  //     index.push_back(index.back() + (finish - start));
  //   }

  // public:
  //   bool finalize() {
  //     data.finalize();
  //     return index.write(*output);
  //   }

  //   class const_iterator : public iterator<random_access_iterator_tag,
  // 					 pair<const T*, const T*> >
  //   {
  //   public:
  //     const_iterator(typename mapable_vector<SizeType>::const_iterator x,
  // 		   const mapable_vector<T> *d)
  //       : i(x), data(d)
  //     { }

  //     pair<const T*, const T*> operator*() const {
  //       return make_pair(&(*data)[i[0]], &(*data)[i[1]]);
  //     }

  //     const_iterator& operator++() {
  //       ++i;
  //       return *this;
  //     }

  //     const_iterator operator++(int) {
  //       const_iterator tmp = *this;
  //       ++*this;
  //       return tmp;
  //     }

  //     bool operator==(const const_iterator &x) const { return i == x.i; }

  //   protected:
  //     typename mapable_vector<SizeType>::const_iterator i;
  //     const mapable_vector<T> *data;
  //   };

  //   const_iterator begin() const {
  //     return const_iterator(index.begin(), data);
  //   }

  //   const_iterator end() const {
  //     return const_iterator(index.end() - 1, data);
  //   }
  // };

  // EQUIVALENT WIN32 DE MMAP
  // typedef struct
  //  {
  //    // This receives a pointer within the current process at which the 
  //    // shared memory is located.
  //    // The same shared memory may reside at different addresses in other
  //    // processes which share it.
  //        void *	location;
  //        HANDLE	hFileMapping;
  // }mem_shared_struct, *mem_shared, *token;

  // mem_shared_struct   *token;

  // if ((*token = (mem_shared) malloc(sizeof(mem_shared_struct))) == NULL)     
  //      return RC_NOT_ENOUGH_MEMORY;
     
  // if (newmode == new_shared_create)
  //    (*token)->hFileMapping = CreateFileMapping((HANDLE) 0xFFFFFFFF, NULL,
  //                  PAGE_READWRITE,
  //                  0,
  //                 (DWORD) size,
  //                  (LPSTR) name);
  //  else
  //       (*token)->hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS,
  //                  FALSE,
  //                  (LPSTR) name);
  // if ((*token)->hFileMapping == NULL)
  // {
  //       free( *token );
  //       return RC_SHM_NOT_CREATED );
  // }

  // (*token)->location = MapViewOfFile((*token)->hFileMapping,
  //                                       FILE_MAP_READ | FILE_MAP_WRITE, 
  //                                       0, 0, 0);
  // if ((*token)->location == NULL)
  // {
  //       CloseHandle((*token)->hFileMapping);
  //             free(*token);
  //             return RC_OBJECT_NOT_CREATED;
  // }

  // A memory mappable vector used by DFA_compact. It does not cope with
  // the standard (minimal interface). Use with caution, once mapped, a
  // vector only supports read-only operations.
  // Type requirements: T shall be a POD type.

  template <typename T>
  class mapable_vector
  {
  protected:
    bool mapped;
    T   *first, *last, *eos;

  public:
    typedef unsigned int size_type;
    typedef T        value_type;
    typedef T*       iterator;
    typedef const T* const_iterator;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef mapable_vector self;

    mapable_vector()
      : mapped(false), first(NULL), last(NULL), eos(NULL)
    { }

    mapable_vector(const self &x)
      : mapped(false), first(NULL), last(NULL), eos(NULL) {
      if (!x.empty()) {
	first = new T[x.capacity()];
	last = copy(x.first, x.last, first);
	eos = first + x.capacity();
      }
    }

    mapable_vector(size_type s)
      : mapped(false), first(NULL), last(NULL), eos(NULL) {
      if (s > 0) {
	first = new T[s];
	last = eos = first + s;
	memset(first, 0, s * sizeof(T));
      }
    }

    template <typename RandomAccessI>
    mapable_vector(RandomAccessI start, RandomAccessI finish)
      : mapped(false), first(finish - start > 0 ? new T[finish - start] : (T*) 0), 
	last(first + (finish - start)), eos(last) {
      copy(start, finish, first);
    }

    void swap(self &x) {
      std::swap(mapped, x.mapped);
      std::swap(first, x.first);
      std::swap(last, x.last);
      std::swap(eos, x.eos);
    }

    void clear() {
#ifndef _MSC_VER
      if (mapped)
	munmap();
      else
#endif
	{
	  delete [] first;
	  first = last = eos = NULL;
	}
    }

    self& operator=(const self &x) {
      if (&x != this) {
	clear();  // set first to NULL
	if (!x.empty()) first = new T[x.size()];
	last = eos = first + x.size();
	copy(x.first, x.last, first);
      }
      return *this;
    }

    size_type size() const     { return last - first; }
    size_type capacity() const { return eos - first; }
    bool      empty() const    { return size() == 0; }

    T&        back()        { return last[-1]; }
    const T&  back() const  { return last[-1]; }

    T&        front()       { return *first; }
    const T&  front() const { return *first; }

    iterator       begin()       { return first; }
    const_iterator begin() const { return first; }

    iterator       end()       { return last; }
    const_iterator end() const { return last; }

    T&        operator[](size_type s)       { return first[s]; }
    const T&  operator[](size_type s) const { return first[s]; }

    bool write(const string &path) {
      ofstream output(path.c_str(), ios::out | ios::binary);
      return output && write(output);
    }

    bool write(ostream& out) const {
      size_type s = size();
      out.write((const char*) &s, sizeof(s));
      out.write((const char*) first, sizeof(T) * s);
      return (bool) out;
    }

    bool read(const string &path) {
      ifstream input(path.c_str(), ios::in | ios::binary);
      return input && read(input);
    }

    bool read(istream& in) {
      size_type s;
      in.read((char*) &s, sizeof(s));
      if (in.gcount() != sizeof(s)) return false;
      clear();
      if (s > 0)
	first = new T[s];
      else first = 0;
      last = eos = first + s;
      in.read((char*) first, sizeof(T) * s);
      return (bool) in;
    }

#ifndef _MSC_VER
    bool mmap(int file_desc) {
      delete [] first;
      size_type p;
      ::read(file_desc, &p, sizeof(size_type));
      first = (T*) ::mmap(0, p * sizeof(T),
			  PROT_READ, MAP_FILE | MAP_PRIVATE, file_desc, 0);
      if (first == (T*) -1) return false;
      last = eos = first + p;
      lseek(file_desc, p * sizeof(T), SEEK_CUR);
      mapped = true;
      return true;
    }

    void munmap() {
      if (mapped) {
	::munmap((char*) first, size() * sizeof(T));
	first = last = eos = NULL;
	mapped = false;
      }
    }
#endif

    void resize(size_type s, const T& x = T()) {
      if (s < size()) erase(begin() + s, end());
      else insert(end(), s - size(), x);
    }

    void erase(iterator i, iterator j) {
      copy(j, last, first);
      last -= j - i;
    }

    void insert(iterator i, size_type s, const T& x = T()) {
      if (s + size() > capacity()) {
	T *p = new T[(s + size()) * 2];
	copy(first, i, p);
	copy_backward(i, last, p + size() + s);
	i = p + (i - first);
	eos = p + (s + size()) * 2;
	last = p + s + size();
	delete [] first;
	first = p;
      }
      else {
	copy_backward(i, last, last + s);
	last += s;
      }
      fill(i, i + s, x);
    }

    void push_back(const T& x) {
      if (last == eos) {
	T *p = new T[max__((size_type) 1, size()) * 2];
	copy(first, last, p);
	delete [] first;
	eos = p + 2 * max__((size_type) 1, size());
	last = p + (last - first);
	first = p;
      }
      *last++ = x;
    }

    void reserve(size_type i) {
      if (i > capacity()) {
	T *p = new T[i];
	copy(first, last, p);
	delete [] first;
	last = p + (last - first);
	first = p;
	eos = first + i;
      }
    }

    ~mapable_vector() { clear(); }
  };

  // A compact table maps integers to static arrays of element of type T
  // To use the read/write functionalities, T must be a POD type
  // For example, say t is a compact table, the sequence #4 is defined
  // by the range t[4],t[5] or t.range(4)
  // a mapped compact table is a static object
  template <typename T, typename SizeType = unsigned int>
  class compact_table
  {
  protected:
    mapable_vector<SizeType> index;
    T *data, *eos;  // data and end-of-storage
    int fd;         // file descriptor for memory mapping

    bool mapped() const {
      return fd > -1;
    }

    void mapped(bool b) {
      if (b && fd < 0) fd = 0;
      else fd = -1;
    }

  public:
    typedef SizeType size_type;
    typedef compact_table<T> self;
    //typedef typename
    //  mapable_vector<typename mapable_vector<T>::size_type>::size_type size_type;

    compact_table()
      : index(1), data(NULL), eos(NULL), fd(-1)
    { }

    compact_table(const self &x)
      : index(x.index), data(NULL), eos(NULL), fd(-1) {
      if (x.eos - x.data > 0) {
	data = new T[x.eos - x.data];
	eos = copy(x.data, x.eos, data);
      }
    }

    void reserve(size_type s) {
      index.reserve(s);
    }

    ~compact_table() {
#ifndef _MSC_VER
      if (mapped()) munmap();
      else
#endif
	delete [] data;
    }

    void clear()
    {
#ifndef _MSC_VER
      if (mapped()) munmap();
      else
#endif
	{
	  delete [] data;
	  data = eos = NULL;
	  index.clear();
	  index.push_back(0);
	}
    }

    size_type size() const {
      return index.size() - 1;
    }

    bool empty() const {
      return size() == 0;
    }

    bool mmap(const string &path) {
      fd = open(path.c_str(), O_RDONLY);
      if (fd == -1) return false;
      return mmap(fd);
    }

    bool mmap(int f) {
      mapped(true);
      if (index.mmap(f)) {
	data = (T*) ::mmap(0, index.back() * sizeof(T),
			   PROT_READ, MAP_FILE | MAP_PRIVATE, f, 0);
	if (data == (T*) -1) return false;
	eos = data + index.back();
	return true;
      }
      return false;
    }

#ifndef _MSC_VER
    void munmap() {
      index.munmap();
      if (fd > 0) close(fd);
      mapped(false);
    }
#endif

    bool read(istream& i) {
      mapped(false);
      if (!index.read(i)) return false;
      data = new T[index.back()];
      i.read((char*) data, index.back() * sizeof(T));
      eos = data + index.back();
      return (size_type) i.gcount() == index.back() * sizeof(T);
    }

    bool write(ostream& o) const {
      index.write(o);
      o.write((char*) data, index.back() * sizeof(T));
      return (bool) o;
    }

    const T* operator[](size_t i) const {
      return data + index[i];
    }

    pair<const T*, const T*> range(size_t i) const {
      return make_pair((*this)[i], (*this)[i + 1]);
    }

    vector<T> find(size_t i) const {
      return vector<T>((*this)[i], (*this)[i + 1]);
    }

    template <typename OutputIterator>
    OutputIterator find_and_write(size_t i, OutputIterator x) const {
      return copy((*this)[i], (*this)[i + 1] - 1, x);
    }

    size_type capacity() const {
      return eos - data;
    }

    template <typename RandomAccessIterator>
    void push_back(RandomAccessIterator start, RandomAccessIterator finish)
    {
      if (capacity() - index.back() < (size_type) (finish - start)) {
	if (data == NULL) {
	  data = new T[finish - start];
	  eos = data + (finish - start);
	  copy(start, finish, data);
	}
	else {
	  size_type new_size;
	  for(new_size = 2 * capacity();
	      new_size - index.back() < (size_type) (finish - start);
	      new_size *= 2);

	  T *tmp = new T[new_size];
	  copy(start, finish, copy(data, data + index.back(), tmp));
	  delete [] data;
	  data = tmp;
	  eos = data + new_size;
	}
      }
      else copy(start, finish, data + index.back());
      index.push_back(index.back() + (finish - start));
    }

    class const_iterator : public iterator<random_access_iterator_tag,
					   pair<const int*, const int*> >
    {
    public:
      const_iterator(typename mapable_vector<typename
		     mapable_vector<T>::size_type>::const_iterator x,
		     const T *d)
	: i(x), data(d)
      { }

      pair<const int*, const int*> operator*() const {
	return make_pair(data + i[0], data + i[1]);
      }

      const_iterator& operator++() {
	++i;
	return *this;
      }

      const_iterator operator++(int) {
	const_iterator tmp = *this;
	++*this;
	return tmp;
      }

      bool operator==(const const_iterator &x) const {
	return i == x.i;
      }

    protected:
      typename mapable_vector<typename mapable_vector<T>::size_type>::const_iterator i;
      const T *data;
    };

    const_iterator begin() const {
      return const_iterator(index.begin(), data);
    }

    const_iterator end() const {
      return const_iterator(index.end() - 1, data);
    }
  };

  // Command line parser
  // User Options: -m {yes|no} /* state marks */
  //               -r {matrix|map|bin|mtf|tr}
  //               -v verbose mode
  // Parameters:   number of inputs
  //               allowed representations for automata
  struct config
  {
    bool state_mark;
    string representation;
    bool verbose_mode;
    string aux_file;
    ifstream aux_input;
    FILE *aux_handle;
    ofstream output;
    int argc;
    char** argv;
    int inputs;
    int arg_pos;
    vector<string> representations;
    list<string> options;
    string use, description;

    void usage()
    {
      cerr << "Usage: " << argv[0] << " [-v] [-m {yes|no}]";
      if (representations.size() > 1) {
	cerr << " [-r {";
	vector<string>::const_iterator i = representations.begin();
	while (1) {
	  cerr << *i;
	  if (++i != representations.end()) cerr << "|";
	  else break;
	}
	cerr << "}]";
      }
      cerr << " " << use;
      if (!use.empty()) cerr << " ";
      if (inputs == 2) cerr << "file";
      cerr << endl;
      cerr << "  " << description << endl;
      cerr << "   -v\tverbose output to stderr" << endl;
      cerr << "   -m\tmark states during traversal (disabled by default)" << endl;
      if (representations.size() > 1)
	cerr << "   -r\tchoose DFA representation, default is "
	     << representations.front() << endl;
      if (representation.empty() && !representations.empty())
	representation = representations.front();
      exit(1);
    }

    config(int argc_, char** argv_, int inputs_, const string representations_ = "",
	   const string &use_ = "", const string &desc = "")
      : state_mark(false), verbose_mode(false), argc(argc_), argv(argv_),
	inputs(inputs_), use(use_), description(desc) {

      aux_handle = NULL;
      string tmpr = representations_;
      if (tmpr == "all") tmpr = "map matrix bin tr mtf";
      string::iterator i, j = tmpr.begin();
      for(;;) {
	i = j;
	j = find(j, tmpr.end(), ' ');
	representations.push_back(string(i, j));
	if (j == tmpr.end()) break;
	++j;
      }

      copy(argv_ + 1, argv_ + argc, back_inserter(options));
      arg_pos = 1;
      for(list<string>::iterator x = options.begin(); x != options.end(); ++arg_pos)
	if (x->operator[](0) != '-') {
	  if (inputs > 1 && aux_file.empty()) {
	    aux_file = *x;
	    aux_input.open(aux_file.c_str());
	    if (!aux_input) {
	      string msg = argv_[0];
	      msg = msg + ": " + aux_file;
	      perror(msg.c_str());
	      exit(2);
	    }
	    aux_handle = fopen(aux_file.c_str(), "rb");
	    if (aux_handle == NULL) {
	      perror(aux_file.c_str());
	      exit(2);
	    }
	    options.erase(x++);
	  }
	  else break;
	}
	else if (*x == "-h")
	  usage();
	else if (*x == "-v") {
	  verbose_mode = true;
	  options.erase(x++);
	}
	else if (*x == "-m") {
	  options.erase(x++);
	  if (x == options.end()) usage();
	  ++arg_pos;
	  if (*x == "yes") state_mark = true;
	  else
	    if (*x == "no") state_mark = false;
	    else usage();
	  options.erase(x++);
	}
	else if (*x == "-r") {
	  if (representations.size() < 2) usage();
	  options.erase(x++);
	  if (x == options.end()) usage();
	  ++arg_pos;
	  if (find(representations.begin(), representations.end(), *x)
	      != representations.end())
	    representation = *x;
	  else
	    usage();
	  options.erase(x++);
	}
	else ++x;

      if (representation.empty() && !representations.empty())
	representation = representations.front();

      if (inputs > 1 && aux_file.empty()) usage();

      if (verbose_mode) {
	char buffer[1024];
	FILE *f = POPEN("date", "r");
	buffer[max__((int) fread(buffer, sizeof(char), 1023, f) - 1, 0)] = '\0';
	PCLOSE(f);
	cerr << buffer << " \"";
	copy(argv, argv + argc, ostream_iterator<char*>(cerr, " "));
	cerr << "\"" << endl;
      }
    }

    ~config()
    {
      if (verbose_mode)
	cerr << argv[0] << ": " << clock() / (double) CLOCKS_PER_SEC
	     << " s CPU time" << endl;

      if (!aux_file.empty())
	aux_input.close();
      if (aux_handle != NULL)
	fclose(aux_handle);
    }

    typedef list<string>::const_iterator const_iterator;
    const_iterator begin() const {
      return options.begin();
    }
    const_iterator end() const {
      return options.end();
    }
  };

  template <typename T1, typename T2 = T1, typename T3 = T2>
  struct triple : public std::pair<T1, T2>
  {
    T3 third;

    triple() : pair<T1, T2>(), third() { }
    triple(const T1 &t1, const T2 &t2, const T3 &t3)
      : std::pair<T1, T2>(t1, t2), third(t3) { }
  };

  template <typename T1, typename T2, typename T3>
  inline
  ostream& operator<<(ostream &out, const triple<T1, T2, T3> &x) {
    return out << '<' << x.first << ", " << x.second << ", "
	       << x.third << '>';
  }

  template <class T1, class T2, class T3>
  inline bool operator<(const triple<T1, T2, T3> &x,
			const triple<T1, T2, T3> &y) {
    if (x.first < y.first) return true;
    if (y.first < x.first) return false;
    if (x.second < y.second) return true;
    if (y.second < x.second) return false;
    return x.third < y.third;
  }

  template <class T1, class T2, class T3>
  inline bool operator==(const triple<T1, T2, T3> &x,
			 const triple<T1, T2, T3> &y) {
    return (x.first == y.first
	    && x.second == y.second && x.third == y.third);
  }

  template <class T1, class T2, class T3>
  inline triple<T1, T2, T3>
  make_triple(const T1 &x, const T2 &y,const T3 &z) {
    return triple<T1, T2, T3>(x, y, z);
  }

  template <class T1, class T2, class T3>
  inline triple<T1, T2, T3> make_triple(const T1 &x,
					const pair<T2, T3> &y) {
    return triple<T1, T2, T3>(x, y.first, y.second);
  }

  // File iterator classes
  //  Descrition: - input and random access iterators on a file accessed
  //                through the system call read. It is implemented with a reference
  //                counting buffer allowing efficient by-value calls to function
  //              - Define a specialized version of the standard STL copy algorithm
  //
  //  Template Parameters: 1. The buffer size in bytes (default 8192)
  //                       2. A unary object function from char to any type used as
  //                          output filter (default identity<char>)
  //  Constructor args:    1. A file descriptor. The default constructor is used to
  //                          build a past-the-end iterator
  //  Remarks: No checks are made on the value returned by read. If an error occurs,
  //  i.e. read returns -1, the behavior is undefined. Also, all operations regarding
  //  file opening and closing are left up to the user.
  //

  // This one should be used to read "once and only once" streams (i.e. pipes), it is
  // an input iterator. It is then forbidden to iterate through a sequence with two
  // iterators simultaneously. The reference counting functionnality is only provided
  // for the copy constructor to be efficient.

  template <int BUFFER_SIZE = 8192>
  class finput_iterator : public iterator<input_iterator_tag, char>
  {
  public:
    typedef char value_type;
    typedef finput_iterator<BUFFER_SIZE> self;


    finput_iterator(FILE* handle)
      : b(new shared_buffer(handle)), c(b->buffer) {
      eob = c + fread(c, sizeof(char), BUFFER_SIZE, b->file);
    }

    finput_iterator()
      : b(NULL), eob(NULL)
    { }

    ~finput_iterator() {
      if (b && --b->references == 0)
	delete b;
    }

    finput_iterator(const self &x)
      : b(x.b), c(x.c), eob(x.eob) {
      if (b) ++b->references;
    }

    self& operator++ () {
      if (++c == eob) {
	c = b->buffer;
	eob = c + fread(c, sizeof(char), BUFFER_SIZE, b->file);
	if (c == eob) eob = NULL;  // end of file
      }
      return *this;
    }

    self operator++ (int) {
      self tmp = *this;
      ++(*this);
      return tmp;
    }

    value_type operator* () const {
      return *c;
    }

    bool operator==(const self &x) const {
      return eob == x.eob;   // only for end of range equality testing
    }

    bool operator!=(const self &x) const {
      return !(*this == x);
    }

  protected:
    struct shared_buffer
    {
      FILE* file;
      unsigned int references;
      char buffer[BUFFER_SIZE];

      shared_buffer(FILE *handle)
	: file(handle), references(1)
      { }
    } *b;

    char *c, *eob;  // current char and end-of-buffer
  };

  template <int BUFFER_SIZE = 8192>
  class frandom_iterator : public iterator<random_access_iterator_tag, char>
  {
  protected:
    struct shared_buffer
    {
      FILE* file;
      unsigned int references;
      char buffer[BUFFER_SIZE];

      shared_buffer(FILE *handle)
	: file(handle), references(1)
      { }
    } *buff;

    long _offset;
    char *c, *eob;  // current char and end-of-buffer

  public:
    typedef frandom_iterator<BUFFER_SIZE> self;
    typedef char value_type;

    frandom_iterator()
      : buff(NULL), c(NULL)
    { }

    frandom_iterator(FILE *handle)
      : buff(new shared_buffer(handle)),
	_offset(ftell(handle)), c(buff->buffer) {
      eob = c + fread(c, sizeof(char), BUFFER_SIZE, buff->file);
      if (c == eob) c = NULL;
    }

    int offset() const {
      return _offset;
    }

    ~frandom_iterator() {
      if (buff != NULL && --buff->references == 0)
	delete buff;
    }

    frandom_iterator(const self &x)
      : buff(x.buff), _offset(x._offset),  c(x.c), eob(x.eob) {
      if (buff != NULL) ++buff->references;
    }

    self& operator++ () {
      ++_offset;
      if (++c == eob) {
	if (buff->references > 1) {
	  --buff->references;
	  buff = new shared_buffer(buff->file);
	}
	fseek(buff->file, _offset, SEEK_SET);
	eob = buff->buffer + fread(buff->buffer, sizeof(char), BUFFER_SIZE, buff->file);
	if (eob == buff->buffer) c = NULL;      // EOF
	else c = buff->buffer;

      }
      return *this;
    }

    self operator++ (int) {
      self tmp = *this;
      ++(*this);
      return tmp;
    }

    self& operator=(const self &x) {
      if (this != &x) {
	if (buff != NULL && --buff->references == 0) delete buff;
	buff = x.buff; c = x.c; _offset = x._offset; eob = x.eob;
	if (buff != NULL) ++buff->references;
      }
      return *this;
    }

    value_type operator* () const {
      return *c;
    }

    bool operator==(const self &x) const {
      if (c == NULL) return x.c == NULL;
      return (x.c != NULL) && (_offset == x._offset);
    }

    bool operator!=(const self &x) const {
      return !(*this == x);
    }

    self operator+(int x) const {
      if (c + x >= eob || c + x < buff->buffer) {
	fseek(buff->file, _offset + x, SEEK_SET);
	return frandom_iterator(buff->file);
      }
      frandom_iterator r(*this);
      r.c += x; r._offset += x;
      return r;
    }

    self& operator+=(int x) {
      c += x;
      _offset += x;
      if (c >= eob || c < buff->buffer) {
	fseek(buff->file, _offset, SEEK_SET);
	c = buff->buffer;
	eob = c + fread(c, sizeof(char), BUFFER_SIZE, buff->file);
      }
      return *this;
    }

    self operator-(int x) const {
      if (c == NULL || c - x < buff->buffer || c - x >= eob) {
	fseek(buff->file, _offset - x, SEEK_SET);
	return frandom_iterator(buff->file);
      }
      frandom_iterator r(*this);
      r.c -= x; r._offset -= x;
      return r;
    }

    int operator-(const self &x) const {
      return _offset - x._offset;
    }

    self& operator-=(int x) {
      _offset -= x;
      if (c == NULL || c - x < buff->buffer || c - x >= eob) {
	fseek(buff->file, _offset, SEEK_SET);
	c = buff->buffer;
	eob = c + fread(c, sizeof(char), BUFFER_SIZE, buff->file);
      }
      else c -= x;
      return *this;
    }

    self& operator--() {
      return *this -= 1;
    }

    self operator--(int) {
      self tmp = *this;
      *this -= 1;
      return tmp;
    }

    bool operator<(const self &x) const {
      return _offset < x._offset;
    }

    value_type operator[](int x) const {
      return *(*this + x);
    }
  };

  // Equivalent to UNIX command "tr" (RTFM)
  template <typename InputIterator>
  class translate
    : public iterator<input_iterator_tag, char>
  {
  protected:
#ifndef _MSC_VER
    InputIterator i;
#endif
    vector<char> tr;

  public:
#ifdef _MSC_VER
    InputIterator i;
#endif
    typedef translate self;

    translate()
    { }

    translate(const InputIterator &x)
      : i(x)
    { }

    translate(const InputIterator &x, const string& src, const string& trg)
      : i(x), tr(256)
    {
      mapping(src, trg);
    }

    void mapping(const string& src, const string& trg) {
      tr.resize(256);
      for(vector<char>::size_type j = 0; j < 256; ++j)  // identity
	tr[j] = (char) j;
      for(string::const_iterator jj = src.begin(), k = trg.begin();
	  jj != src.end(); ++jj, ++k)
	tr[(unsigned char) *jj] = *k;
    }

    value_type operator*() const {
      return tr[(unsigned char) *i];
    }

    self& operator=(const InputIterator &x) {
      i = x;
      return *this;
    }

    self& operator++() {
      ++i;
      return *this;
    }

    self operator++(int) {
      self tmp = *this;
      ++(*this);
      return tmp;
    }

#ifndef _MSC_VER
    friend bool operator==<>(const self&, const self&);
#endif
  };

  template <typename InputIterator>
  inline
  bool operator==(const translate<InputIterator> &x,
		  const translate<InputIterator> &y) {
    return x.i == y.i;
  }

  template <typename InputIterator>
  inline
  bool operator!=(const translate<InputIterator> &x,
		  const translate<InputIterator> &y) {
    return !(x == y);
  }

  // Equivalent to UNIX command "tr -s" (RTFM)
  template <typename InputIterator>
  class squeeze
    : public iterator<input_iterator_tag, char>
  {
  protected:
#ifndef _MSC_VER
    InputIterator first, last;
#endif
    vector<char>  squeeze_me;
    char          c;

  public:
#ifdef _MSC_VER
    InputIterator first, last;
#endif
    typedef squeeze self;

    squeeze(const InputIterator &eos = InputIterator())
      : first(eos)
    { }

    squeeze(const string& chars, const InputIterator &start,
	    const InputIterator &finish = InputIterator())
      : first(start), last(finish), squeeze_me(256, (char) 0)
    {
      for(string::const_iterator j = chars.begin();
	  j != chars.end(); ++j)
	squeeze_me[(unsigned char) *j] = (char) 1;
      if (!(first == last)) c = *first;
    }

    value_type operator*() const {
      return c;
    }

    self& operator++() {
      if (squeeze_me[(unsigned char) c] == (char) 0) {
	++first;
	c = *first;
      }
      else {
	char d;
	while(!(++first == last)) {
	  d = *first;
	  if (d != c) {
	    c = d;
	    break;
	  }
	}
      }
      return *this;
    }

    self operator++(int) {
      self tmp = *this;
      ++(*this);
      return tmp;
    }

#ifndef _MSC_VER
    friend bool operator==<>(const self&, const self&);
#endif
  };

  template <typename InputIterator>
  inline
  bool operator==(const squeeze<InputIterator> &x,
		  const squeeze<InputIterator> &y) {
    return x.first == y.first;
  }

  template <typename InputIterator>
  inline
  bool operator!=(const squeeze<InputIterator> &x,
		  const squeeze<InputIterator> &y) {
    return !(x == y);
  }

  // Equivalent to UNIX command "tr -d" (RTFM)
  template <typename InputIterator>
  class ignore
    : public iterator<input_iterator_tag, char>
  {
  protected:
#ifndef _MSC_VER
    InputIterator first, last;
#endif
    vector<char>  ignore_me;
    char          c;

  public:
#ifdef _MSC_VER
    InputIterator first, last;
#endif
    typedef ignore self;

    ignore(const InputIterator &eos = InputIterator())  // end of sequence
      : first(eos)
    { }

    ignore(const string& chars, const InputIterator &start,
	   const InputIterator &finish = InputIterator())
      : first(start), last(finish), ignore_me(256, (char) 0)
    {
      for(string::const_iterator j = chars.begin();
	  j != chars.end(); ++j)
	ignore_me[(unsigned char) *j] = (char) 1;
      if (!(first == last)) {
	c = *first;
	if (ignore_me[(unsigned char) c] == (char) 1)
	  ++(*this);
      }
    }

    value_type operator*() const {
      return c;
    }

    self& operator++() {
      while(!(++first == last)) {
	c = *first;
	if (ignore_me[(unsigned char) c] == (char) 0)
	  break;
      }
      return *this;
    }

    self operator++(int) {
      self tmp = *this;
      ++(*this);
      return tmp;
    }

#ifndef _MSC_VER
    friend bool operator==<>(const self&, const self&);
#endif
  };

  template <typename InputIterator>
  inline
  bool operator==(const ignore<InputIterator> &x,
		  const ignore<InputIterator> &y) {
    return x.first == y.first;
  }

  template <typename InputIterator>
  inline
  bool operator!=(const ignore<InputIterator> &x,
		  const ignore<InputIterator> &y) {
    return !(x == y);
  }

  //namespace std {
  template <typename T, typename U>
  inline
  ostream& operator<<(ostream &out, const pair<T, U> &p) {
    out << "<" << p.first << ", " << p.second << ">";
    return out;
  }
  //};

  template <class T, class Allocator>
  inline
  ostream& operator<<(ostream &out, const vector<T, Allocator> &v)
  {
    out << "(";
    if (v.empty()) return out << " )";
    typename vector<T, Allocator>::const_iterator i = v.begin();
    while (1) {
      out << *i;
      if (++i != v.end()) out << ",";
      else return out << ")";
    }
  }

  template <class T>
  inline
  ostream& operator<<(ostream &out, const set<T> &v)
  {
    out << "(";
    if (v.empty()) return out << " )";
    typename set<T>::const_iterator i = v.begin();
    while (1) {
      out << *i;
      if (++i != v.end()) out << ",";
      else return out << ")";
    }
  }

#ifdef _MSC_VER
  // Some missing POSIX functions or STL extensions for VC++
  static const char *optarg;
  static int optind;

  static
  int getopt(int argc, char* const* argv, const char *opt)
  {
    static char* const* prev_argv = NULL;

    optarg = NULL;
    int r = -1;

    if (argv != prev_argv) {
      prev_argv = argv;
      optind = 1;
    }

    if (optind < argc) {
      if (argv[optind][0] == '-') {
	const char* p = find(opt, opt + strlen(opt), argv[optind][1]);
	if (*p == '\0') return '?';
	if (strlen(argv[optind]) == 2) {
	  r = *p;
	  ++optind;
	  if (p[1] == ':')
	    if (optind == argc || argv[optind][0] == '-')
	      return ':'; // missing argument to option
	    else optarg = argv[optind++];
	}
      }
    }
    return r;
  }

#endif

  template <typename F, typename G>
  class unary_compose
    : public unary_function<typename G::argument_type,
			    typename F::result_type>
  {
  protected:
    F f;
    G g;
  public:
    unary_compose(const F &x, const G &y)
      : f(x), g(y) {}
    typename F::result_type
    operator()(const typename G::argument_type &x) const {
      return f(g(x));
    }
  };

  template <typename F, typename G>
  inline
  unary_compose<F, G>
  compose1(const F &x, const G &y) {
    return unary_compose<F, G>(x, y);
  }

  template <class Pair>
  struct select1st
    : public unary_function<Pair, typename Pair::first_type>
  {
    const typename Pair::first_type&
    operator()(const Pair& x) const { return x.first;  }
  };

  template <class Pair>
  struct select2nd
    : public unary_function<Pair, typename Pair::second_type>
  {
    const typename Pair::second_type&
    operator()(const Pair& x) const { return x.second;  }
  };

  template <typename T>
  struct identity : public unary_function<T, T>
  {
    const T& operator()(const T &x) const {
      return x;
    }
  };

  template <typename InputIterator1, typename InputIterator2>
  inline
  int lexicographical_compare_3way(InputIterator1 x1, InputIterator1 y1,
				   InputIterator2 x2, InputIterator2 y2)
  {
    for(; x1 != y1 && x2 != y2; ++x1, ++x2) {
      if (*x1 < *x2) return -1;
      if (*x2 < *x1) return 1;
    }
    if (x2 == y2) return !(x1 == y1);
    return -1;
  }

  template <typename T, typename U>
  inline
  bool operator==(const pair<const T, U> &x, const pair<T, U> &y) {
    return x.first == y.first && x.second == y.second;
  }

#ifndef _MSC_VER
  template <typename T, typename U>
  inline
  bool operator==(const pair<T, U> &x, const pair<const T, U> &y) {
    return x.first == y.first && x.second == y.second;
  }
#endif

  template <size_t n>
  inline
  bool operator<(const bitset<n> &x, const bitset<n> &y)
  {
    return lexicographical_compare(x.begin(), x.end(),
				   y.begin(), y.end());
  }

  // skip_blanks_iterator implements DFA_*::const_iterator
  // it skips holes in the pointers vector to internal states
  // left by del_state
  template <typename T>
  class skip_blanks_iterator
    : public iterator<forward_iterator_tag, typename vector<T*>::size_type>
  {
    typedef skip_blanks_iterator self;

  private:
    const vector<T*> *Q;
    typename vector<T*>::size_type pos;

  public:
    skip_blanks_iterator(const vector<T*> *_Q,
			 typename vector<T*>::size_type _pos)
      : Q(_Q), pos(_pos)
    { }

    skip_blanks_iterator() : Q(NULL), pos(0) { }
    skip_blanks_iterator(const self &x) : Q(x.Q), pos(x.pos) { }
    self& operator=(const self &x)
    {
      Q = x.Q;
      pos = x.pos;
      return *this;
    }

    bool operator==(const self &x) const { return x.pos == pos; }
    bool operator!=(const self &x) const { return x.pos != pos; }

    typename vector<T*>::size_type operator*() const {
      return pos;
    }

    self& operator++()  {
      for(++pos; pos < Q->size() && Q->operator[](pos) == NULL; ++pos);
      return *this;
    }

    self operator++(int) {
      self tmp = *this;
      ++(*this);
      return tmp;
    }
  };

#ifndef _MSC_VER
  template <typename Key, typename Data,
	    template <typename K, typename D> class Container = map>
  class cache : protected list<Data>
  {
  public:
    typedef Data data_type;
    typedef Key key_type;
    typedef pair<key_type, data_type> value_type;
    typedef list<data_type> super;
    typedef size_t size_type;

    cache(size_type capacity__)
      : capacity_(capacity__)
    { }

  protected:
    Container<Key, typename list<Data>::iterator> hasher;
    size_type capacity_;
  };
#endif

} // namespace astl

#endif

