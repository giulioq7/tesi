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

#ifndef ASTL_DIR_CURSOR_H
#define ASTL_DIR_CURSOR_H

// not implemented for Visual C++
#ifndef _MSC_VER

#include <astl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <cstring>
#include <climits>
#include <functional>

using namespace std;

namespace astl {

// A directory cursor simulates an automaton from a directory tree:
// transitions are labeled with files names and a predicate provided
// at instanciation time dictates if a state is a terminal state. By
// default, a state is considered non-terminal if it is a directory. 
struct default_predicate 
  : public binary_function<string, struct stat *, bool>
{
  bool operator()(const string &, const struct stat *s) const {
    return !S_ISDIR(s->st_mode);
  }
};

template <class Predicate = default_predicate>
class directory_cursor : public forward_cursor_concept
{
protected:
  string state, transition;   // current state and letter
  DIR *dir;

public:
  typedef directory_cursor self;
  typedef time_t           tag_type;    // time of last modification
  typedef string           state_type;
  typedef string           char_type;

  typedef std::char_traits<string> char_traits;

  directory_cursor() 
	: dir(NULL)
  { }
  
  directory_cursor(const string &dir_)
    : state(dir_) {
    dir = opendir(state.c_str());
  }

  directory_cursor(const self &x)
    : state(x.state), transition(x.transition), dir(x.dir) {
    if (x.dir) {
      dir = opendir(state.c_str());
      if (!transition.empty())
	seekdir(dir, telldir(x.dir));
    }
  }

  ~directory_cursor() { if (dir) closedir(dir); }

  state_type src() const { return state; }

  self& operator=(const state_type& p) 
  { 
    state = p;
    if (dir) closedir(dir);
    dir = opendir(state.c_str());
    return *this;
  }

  bool sink() const { return state.empty(); }

  bool forward(const char_type &a)   // linear time method
  { 
    struct dirent *entry;
    for(rewinddir(dir); (entry = readdir(dir)) != NULL; )
      if (strcmp(a.c_str(), entry->d_name)) {
	state += '/';
	state += entry->d_name;
	closedir(dir);
	dir = opendir(state.c_str());
	return true;
      }
    closedir(dir);
    dir = NULL;
    state.clear();
    return false;
  }

  bool src_final() const
  { 
    struct stat s;
    stat(state.c_str(), &s);
    return Predicate()(state, &s);
  }

  tag_type src_tag() const { 
    struct stat s;
    stat(state.c_str(), &s);
    return s.st_mtime;
  }
    
  bool exists(const char_type &a) const {
    struct dirent *tmp_entry = NULL;
    DIR *d = opendir(state.c_str());
    if (d) {
      for(rewinddir(d); (tmp_entry = readdir(d)) != NULL; )
	if (strcmp(a.c_str(), tmp_entry->d_name)) break;
      
      closedir(d);
    }
    return tmp_entry != NULL;
  }

  self& operator=(const self &x) { 
    if (dir) closedir(dir);
    state = x.state;
    transition = x.transition;
    dir = opendir(state.c_str());
    if (dir && !transition.empty()) seekdir(dir, telldir(x.dir));
    return *this;
  }  

  bool operator==(const self &x) const { 
	  return state == x.state && transition == x.transition; 
  }

  char_type letter() const { return transition; }

  bool first() { 
    if (dir == NULL) return false;
    struct dirent *entry;
    for(rewinddir(dir); (entry = readdir(dir)) != NULL; )
      if (strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name) != 0) {
	transition = entry->d_name;
	return true;
      }
    return false;
  }
  
  bool next() { 
    struct dirent *entry;
    for(; (entry = readdir(dir)) != NULL; )
      if (strcmp(".", entry->d_name) != 0 || strcmp("..", entry->d_name) != 0) {
	transition = entry->d_name;
	return true;
      }
    return false;
  }

  void forward() { 
    state += '/';
    state += transition;
    closedir(dir);
    dir = opendir(state.c_str());
  }

  bool find(const char_type &a) { 
    if (dir == NULL) return false;
    struct dirent *entry;
    for(rewinddir(dir); (entry = readdir(dir)) != NULL; )
      if (strcmp(a.c_str(), entry->d_name)) {
	transition = entry->d_name;
	return true;
      }
    return false;
  }

  state_type aim() const { 
    return state + "/" + transition;
  }

  bool aim_final() const { 
    struct stat s;
    string path = aim();
    stat(path.c_str(), &s);
    return Predicate()(path, &s);
  }

  tag_type aim_tag() const {
    struct stat s;
    string path = aim();
    stat(path.c_str(), &s);
    return s.st_mtime; 
  }
};

inline
directory_cursor<> directoryc(const string &d = ".") {
  return directory_cursor<>(d);
}

template <typename Predicate>
inline
directory_cursor<Predicate> 
directoryc(const Predicate&, const string &d = ".") {
  return directory_cursor<Predicate>(d);
}

#endif

} // namespace astl

#endif 
