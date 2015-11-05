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

#ifndef ASTL_CONCEPT_H
#define ASTL_CONCEPT_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace astl {

// Defines a hierarchy of concepts needed for dispatching:

struct cursor_concept 
{ 
  typedef cursor_concept concept;
};

struct transition_cursor_concept 
{
  typedef transition_cursor_concept concept;
};

struct forward_cursor_concept 
  : public cursor_concept, public transition_cursor_concept 
{
  typedef forward_cursor_concept concept;
};

struct stack_cursor_concept : public transition_cursor_concept 
{
  typedef stack_cursor_concept concept;
};

struct queue_cursor_concept : public transition_cursor_concept 
{
  typedef queue_cursor_concept concept;
};

struct dfirst_cursor_concept 
{
  typedef dfirst_cursor_concept concept;
};

struct bfirst_cursor_concept 
{
  typedef bfirst_cursor_concept concept;
};

struct FA_concept {
    typedef FA_concept concept;
};

struct DFA_concept : public FA_concept {
  typedef DFA_concept concept;
};

struct NFA_concept : public FA_concept {
  typedef NFA_concept concept;
};

} // namespace astl

#endif // ASTL_CONCEPT_H
