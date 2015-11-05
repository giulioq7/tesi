/*
 * ASTL - the Automaton Standard Template Library.
 * C++ generic components for Finite State Automata handling.
 * Copyright (C) 2000-2006 Vincent Le Maout (vincent.lemaout@chello.fr).
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

/** @file */

#ifndef ASTL_MATCH_H
#define ASTL_MATCH_H

namespace astl {

template <typename ForwardI, typename Cursor>
inline
ForwardI 
first_match(Cursor &c, ForwardI first, ForwardI last)
{
  ForwardI start = first;
  for(; first != last && c.forward(*first); ++first)
    if (c.src_final()) return ++first;
  return start;  // no match found
}

template <typename Cursor>
inline
const char*
first_match(Cursor &c, const char *text)
{
  const char *start = text;
  for(; *text != '\0' && c.forward(*text); ++text)
    if (c.src_final()) return ++text;
  return start;  // no match found
}

template <typename ForwardI, typename Cursor>
inline
ForwardI 
longest_match(Cursor &c, ForwardI first, ForwardI last)
{
  ForwardI last_match_position = first;
  while(first != last && c.forward(*first)) {
    ++first;
    if (c.src_final()) 
      last_match_position = first;
  }
  return last_match_position;
}

template <typename Cursor>
inline
const char*
longest_match(Cursor &c, const char *text)
{
  const char *last_match_position = text;
  while(*text != '\0' && c.forward(*text)) {
    ++text;
    if (c.src_final()) 
      last_match_position = text;
  }
  return last_match_position;
}

template <typename InputI, typename Cursor>
inline
int match_count(Cursor &c, InputI first, InputI last)
{
  int r = 0;
  for(; first != last && c.forward(*first); ++first)
    if (c.src_final()) ++r;
  return r;
}

/** Tries to move a cursor along the path defined by the range <tt>
    [first, last) </tt> and returns @c true if the reached state is
    final.
    @param c A plain cursor
    @param first start-of-range input iterator
    @param last An end-of-range input iterator
    @return @c true if for all @c i in <tt> [first, last) </tt> @c
    c.forward(*i) returns @c true and for <tt> i == last </tt>, @c
    c.src_final() is @c true
    @pre @c c.sink() is @c false
    @see #first_match(), #longest_match(), #match_count() */
   
template <typename InputI, typename Cursor>
inline
bool match(Cursor &c, InputI first, InputI last)
{
  for(; first != last && c.forward(*first); ++first);
  return first == last && c.src_final();
}

template <typename Cursor>
inline
bool match(Cursor& c, const char *w)
{
  for(; *w != '\0' && c.forward(*w); ++w);
  return *w == '\0' && c.src_final();
}

template <typename ForwardI, typename ExtractCursor>
inline
std::vector<std::pair<int, int> > 
split_match(ExtractCursor &c, ForwardI first, ForwardI last)
{
  std::vector<std::pair<int, int> > tmp;
  for(; first != last && c.forward(*first); ++first)
    if (c.src_final()) tmp = c.submatches();
  return tmp;
}

/** algorithm
name first_match

prototype $
template <typename ForwardI, typename Cursor>
ForwardI 
first_match(Cursor &c, ForwardI first, ForwardI last);

template <typename ForwardI, typename Cursor>
ForwardI 
first_match(const Cursor &c, ForwardI first, ForwardI last);
$

description $
Returns a past-the-end iterator on the shortest prefix of the word 
\verb+[first, last)+ recognized by the automaton pointed by
\verb+c+. If none is found, \verb+first+ is returned which means that
either no final state has been reached during the traversal or a
transition was undefined. \\
The first version of the algorithm does not copy the cursor for
efficiency reasons as this function is to be called intensively
during pattern recognition processings. The second version allows
the algorithm to be called with constant cursors like those
returned by the helper functions as \verb+forwardc()+.
$

requirement \verb+ForwardI+ is a model of forward iterator.
requirement \verb+Cursor+ is a model of plain cursor.
requirement \verb+ForwardI::value_type+ is convertible to \verb+Cursor::char_type+.

precondition \verb+[first, last)+ is a valid range.
precondition \verb+c.sink()+ is false. 

complexity At most \verb+last - first+ calls to \verb+Cursor::forward+.

seealso \verb+match+, \verb+longest_match+, \verb+match_count+

*/

/** algorithm
name longest_match

prototype $
template <typename ForwardI, typename Cursor>
ForwardI 
longest_match(Cursor &c, ForwardI first, ForwardI last);

template <typename ForwardI, typename Cursor>
ForwardI 
longest_match(const Cursor &c, ForwardI first, ForwardI last);
$

description $
Returns a past-the-end iterator on the longest prefix of the word 
\verb+[first, last)+ recognized by the automaton pointed by
\verb+c+. If none is found, \verb+first+ is returned which means that
either no final state has been reached during the traversal or a
transition was undefined. \\
The first version of the algorithm does not copy the cursor for
efficiency reasons as this function is to be called intensively
during pattern recognition processings. The second version allows
the algorithm to be called with constant cursors like those
returned by the helper functions as \verb+forwardc()+.
$

requirement \verb+ForwardI+ is a model of forward iterator.
requirement \verb+Cursor+ is a model of plain cursor.
requirement \verb+ForwardI::value_type+ is convertible to \verb+Cursor::char_type+.

precondition \verb+[first, last)+ is a valid range.
precondition \verb+c.sink()+ is false. 

complexity At most \verb+last - first+ calls to \verb+Cursor::forward+.

seealso \verb+match+, \verb+first_match+, \verb+match_count+

*/

/** algorithm
name match_count

prototype $
template <typename InputI, typename Cursor>
int match_count(Cursor &c, InputI first, InputI last);

template <typename InputI, typename Cursor>
int match_count(const Cursor &c, InputI first, InputI last);
$

description $
Returns a count of the prefixes of the sequence \verb+[first, last)+
recognized by the automaton pointed to by c, in other words, a count
of states where \verb+c.src_final()+ is true which were reached during 
the automaton traversal along the path \verb+[first, last)+. \\
The first version of the algorithm does not copy the cursor for
efficiency reasons as this function is to be called intensively
during pattern recognition processings. The second version allows
the algorithm to be called with constant cursors like those
returned by the helper functions as \verb+forwardc()+.
$

requirement \verb+InputI+ is a model of input iterator.
requirement \verb+Cursor+ is a model of plain cursor.
requirement \verb+InputI::value_type+ is convertible to \verb+Cursor::char_type+.

precondition \verb+[first, last)+ is a valid range.
precondition \verb+c.sink()+ is false. 

complexity At most \verb+last - first+ calls to \verb+Cursor::forward+.

seealso \verb+match+, \verb+first_match+, \verb+longest_match+

*/

} // namespace astl


#endif
