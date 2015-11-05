# ASTL - the Automaton Standard Template Library.
# C++ generic components for Finite State Automata handling.
# Copyright (C) 2000-2003 Vincent Le Maout (vincent.lemaout@chello.fr).
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# usage: make help
#
# Note: - this makefile needs a Makefile.d storing all targets prerequisits, that is,
#         object files, source files and include files the target depends on.
#         This file MUST be generated prior to compiling and must be updated manually
#         each time dependencies change (use 'make dep' or 'make clean').
#       - target names are automatically generated from *.cc files. Adding a .cc file
#         in the src directory adds a new target. Do not forget to 'make dep' afterwards
#         to rebuild the dependencies file (or 'make clean').

CC=g++
INC=../include
BIN=../bin
SRC=.
INCLUDE=-I$(INC)
CCFLAGS=-fno-rtti -fno-exceptions -pipe -Wall
ifeq ($(PROFILE),1)
DEBUG=1
FLAGS=-pg
endif
ifeq ($(RELEASE),1)
ifeq ($(DEBUG),1)
FLAGS:=$(FLAGS) -O2
else
FLAGS:=$(FLAGS) -O3
endif
endif
ifeq ($(DEBUG),1)
FLAGS:=$(FLAGS) -g -fno-inline
endif

ALLFLAGS=$(INCLUDE) $(CCFLAGS) $(FLAGS) $(EXTRA)
COMPILE=$(CC) $(ALLFLAGS)

SOURCES=$(shell echo *.cc)
ALL=$(SOURCES:.cc=)

all: $(ALL)

help what usage howto:
	@echo "usage: make [all]       builds all binaries"
	@echo "            [clean]     suppress binaries and rebuild dependencies"
	@echo "            [dep]       rebuild dependencies file"
	@echo "            [target]    build any binary file whose name is 'target'"
	@echo "            [DEBUG=1]   add debugging info (default)"
	@echo "            [PROFILE=1] add profiling and debugging informations"
	@echo "            [RELEASE=1] turn on code optimization"
	@echo "            [EXTRA=...] pass any extra options to compiler"
	@echo "            [help]      this help"

clean: dep
	rm -f *.o
	rm -f *~
	cd $(BIN); rm -f $(ALL)

# ';' prevents gmake from using implicit commands:
$(ALL): %: $(BIN)/% ;    

$(BIN)/%: %.o
	$(COMPILE) $< -o $@

%.o: %.cc
	$(COMPILE) -c $< -o $@

dep:
	$(COMPILE) -MM *.cc >Makefile.d  

$(shell touch Makefile.d)
include Makefile.d

# This makefile works with the following make and g++ versions:

# $ make -v
# GNU Make 3.80
# Copyright (C) 2002  Free Software Foundation, Inc.

# $ g++ -v
# Reading specs from /usr/lib/gcc-lib/i586-mandrake-linux-gnu/3.2.2/specs
# Configured with: ../configure --prefix=/usr --libdir=/usr/lib --with-slibdir=/lib 
# --mandir=/usr/share/man --infodir=/usr/share/info --enable-shared --enable-threads=posix 
# --disable-checking --enable-long-long --enable-__cxa_atexit 
# --enable-languages=c,c++,ada,f77,objc,java --host=i586-mandrake-linux-gnu --with-system-zlib
# Thread model: posix
# gcc version 3.2.2 (Mandrake Linux 9.1 3.2.2-3mdk)


