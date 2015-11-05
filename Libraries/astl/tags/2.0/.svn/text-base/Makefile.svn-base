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

# ASTL 2.0 general makefile
# usage: make help

.PHONY: doc all

all:
	cd src && make all EXTRA=$(EXTRA)

doc:
	doxygen

clean:
	cd src && make clean
	cd check && make clean

dep:
	cd src && make dep

check: all
	cd check && ./check

help:
	@echo "ASTL 2.0 general makefile"
	@echo "usage: make [all]       build all bin/ executables from src/*.cc (default target)"
	@echo "            [check]     run check/ test scripts"
	@echo "            [clean]     suppress all binaries and garbage and rebuild dependencies"
	@echo "                        clean up check/ and doc/ directories"
	@echo "            [dep]       rebuild dependencies src/Makefile.d"
	@echo "            [doc]       make documentation from source with doxygen"
	@echo "            [help]      display makefile usage"
	@echo "            [DEBUG=1]   debug mode compilation (default)"
	@echo "            [RELEASE=1] optimize code for speed"
	@echo "            [PROFILE=1] profiling mode compilation"

