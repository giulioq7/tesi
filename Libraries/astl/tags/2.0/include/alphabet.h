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

#ifndef ASTL_ALPHABET_H
#define ASTL_ALPHABET_H

// This file implements four alphabet traits:
// 1. template <class T, T lower_bound, T upper_bound> range
// 2. plain (8 bits signed)
// 3. ASCII (7 bits)
// 4. french
// 5. Unicode (wchar_t)
// 6. strings (std::string)
// 7. uplain (8 bits unsigned)


#include <iterator>
#include <string>

namespace astl {

using namespace std;

#if (__GNUG__ && __GNUG__ < 3)
#define CHAR_TRAITS string_char_traits
#else
#define CHAR_TRAITS std::char_traits
#endif

template <typename T, T lower_bound, T upper_bound>
class range : public CHAR_TRAITS<T>
{
public:
  typedef typename CHAR_TRAITS<T>::char_type char_type;
  typedef long                               int_type;
  static const size_t                        size;
  static char_type to_char_type(int_type e) {
    return (char_type) (e + lower_bound);
  }
  static int_type to_int_type(char_type c) {
    return (int_type) c - lower_bound;
  }
  static bool eq_int_type(int_type x, int_type y) {
    return x == y;
  }

#if (__GNUG__ && __GNUG__ < 3)
  class const_iterator : public bidirectional_iterator<T, ptrdiff_t>
#else
  class const_iterator 
    : public std::iterator<std::bidirectional_iterator_tag, T>
#endif
  {
  private:
    int_type c;

  public:
    const_iterator()
    { }

    const_iterator(int_type x)
      : c(x)
    { }

    const_iterator& operator++ () {
      ++c;
      return (*this);
    }

    const_iterator operator++ (int) {
      const_iterator tmp = *this;
      ++(*this);
      return (tmp);
    }

    char_type operator* () const {
      return (char_type) c;
    }

    bool operator== (const const_iterator& i) const {
      return c == i.c;
    }

    bool operator!= (const const_iterator& i) const {
      return !(*this == i);
    }
  };

  static const_iterator begin() { return const_iterator(lower_bound); }
  static const_iterator end() { return const_iterator(upper_bound + 1); }

  static bool lt(const char_type &x, const char_type &y) {
    return x < y;
  }

  static bool eq(const char_type &x, const char_type &y) {
    return x == y;
  }

  // These are not standard requirements (backward compatibility purpose):
  typedef const_iterator iterator;
  typedef char_type Alphabet;
  static unsigned long map(const char_type &x) {
    return (unsigned long) to_int_type(x);
  }
  static char_type unmap(unsigned long x) {
    return to_char_type((int_type) x);
  }
};

template <typename T, T lower_bound, T upper_bound> const size_t
range<T, lower_bound, upper_bound>::size = (size_t) upper_bound - lower_bound + 1;

typedef range<char, (char) -128, (char) 127> plain;
typedef range<unsigned char, (unsigned char) 0, (unsigned char) 255> uplain;
typedef range<char, (char) 0, (char) 127>    ASCII;
typedef std::char_traits<wchar_t> unicode;

class strings : public CHAR_TRAITS<string>
{
public:
  typedef string      char_type;
  typedef long        int_type;
  static const size_t size;

  static bool eq(const char_type &x, const char_type &y) {
    return x == y;
  }

  static bool lt(const char_type &x, const char_type &y) {
    return x < y;
  }
};

class ucs2
{
public:
  typedef unsigned short char_type;
  typedef unsigned short int_type;
  static const size_t size = 65536;

  static bool eq(const char_type &x, const char_type &y) {
    return x == y;
  }

  static bool lt(const char_type &x, const char_type &y) {
    return x < y;
  }
};

#ifdef _MSC_VER
// VC++ 6.0 does not compile the previous generic 'size' initialization:
//  const size_t plain::size   = 256;
//  const size_t ASCII::size   = 128;
//  const size_t range<char, (char) -128, (char) 127>::size   = 256;
//  const size_t range<char, (char) 0, (char) 127>::size   = 128;
#endif

} // namespace astl

#endif // ASTL_ALPHABET_H







