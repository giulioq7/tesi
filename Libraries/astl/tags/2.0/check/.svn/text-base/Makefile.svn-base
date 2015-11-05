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

D1=words
D2=words2
DS1=$(D1).sorted
DS2=$(D2).sorted
ALL=union intersection ddiff symdiff match concat seed \
isomorph reverse minimize compact compress ahocorasick \
strcursor
BIN=../bin
DFAREF=$(BIN)/dfaref.dfa

all: check $(ALL)

$(ALL): 1.dfa 2.dfa 1m.dfa 2m.dfa

.PHONY: clean check $(ALL)

clean:
	rm -f *.dfa
	rm -f *.ref
	rm -f *.sorted

check:
	check_dfa
	check_nfa
	check_cursor

union: union.ref
	@echo "Checking union..."; echo -n "  running (A | B) : "
	union <1.dfa 2.dfa | language | sort | diff -q - $<
	@echo -n "  running (AM | BM) : "
	union <1m.dfa 2m.dfa | language | sort | diff -q - $<
	@echo -n "  running (B | A) : "
	union <2.dfa 1.dfa | language | sort | diff -q - $<
	@echo -n "  running (BM | AM) : "
	union <2m.dfa 1m.dfa | language | sort | diff -q - $<
	@echo -n "  running (AM | B) : "
	union <1m.dfa 2.dfa | language | sort | diff -q - $<
	@echo -n "  running (A | BM) : "
	union <1.dfa 2m.dfa | language | sort | diff -q - $<
	@echo -n "  running (BM | A) : "
	union <2m.dfa 1.dfa | language | sort | diff -q - $<
	@echo -n "  running (B | AM) : "
	union <2.dfa 1m.dfa | language | sort | diff -q - $<

intersection: intersection.ref
	@echo "Checking intersection..."; echo -n "  running (A & B) : "
	intersection <1.dfa 2.dfa | language | sort | diff -q - $<
	@echo -n "  running (AM & BM) : "
	intersection <1m.dfa 2m.dfa | language | sort | diff -q - $<
	@echo -n "  running (B & A) : "
	intersection <2.dfa 1.dfa | language | sort | diff -q - $<
	@echo -n "  running (BM & AM) : "
	intersection <2m.dfa 1m.dfa | language | sort | diff -q - $<
	@echo -n "  running (B & AM) : "
	intersection <2.dfa 1m.dfa | language | sort | diff -q - $<
	@echo -n "  running (BM & A) : "
	intersection <2m.dfa 1.dfa | language | sort | diff -q - $<
	@echo -n "  running (A & BM) : "
	intersection <1.dfa 2m.dfa | language | sort | diff -q - $<
	@echo -n "  running (AM & B) : "
	intersection <1m.dfa 2.dfa | language | sort | diff -q - $<

ddiff: ddiff1.ref ddiff2.ref
	@echo "Checking ddiff..."; echo -n "  running (A \ B) : "
	ddiff <1.dfa 2.dfa | language | sort | diff -q - ddiff1.ref
	@echo -n "  running (AM \ BM) : "
	ddiff <1m.dfa 2m.dfa | language | sort | diff -q - ddiff1.ref
	@echo -n "  running (B \ A) : "
	ddiff <2.dfa 1.dfa | language | sort | diff -q - ddiff2.ref
	@echo -n "  running (BM \ AM) : "
	ddiff <2m.dfa 1m.dfa | language | sort | diff -q - ddiff2.ref
	@echo -n "  running (B \ AM) : "
	ddiff <2.dfa 1m.dfa | language | sort | diff -q - ddiff2.ref
	@echo -n "  running (BM \ A) : "
	ddiff <2m.dfa 1.dfa | language | sort | diff -q - ddiff2.ref
	@echo -n "  running (AM \ B) : "
	ddiff <1m.dfa 2.dfa | language | sort | diff -q - ddiff1.ref
	@echo -n "  running (A \ BM) : "
	ddiff <1.dfa 2m.dfa | language | sort | diff -q - ddiff1.ref

symdiff: symdiff.ref
	@echo "Checking symdiff..."; echo -n "  running (A U B) \ (A & B) : "
	symdiff <1.dfa 2.dfa | language | sort | diff -q - symdiff.ref
	@echo -n "  running (AM U BM) \ (AM & BM): "
	symdiff <1m.dfa 2m.dfa | language | sort | diff -q - symdiff.ref
	@echo -n "  running (B U A) \ (B & A) : "
	symdiff <2.dfa 1.dfa | language | sort | diff -q - symdiff.ref
	@echo -n "  running (BM U AM) \ (BM & AM) : "
	symdiff <2m.dfa 1m.dfa | language | sort | diff -q - symdiff.ref
	@echo -n "  running (B U AM) \ (B & AM) : "
	symdiff <2.dfa 1m.dfa | language | sort | diff -q - symdiff.ref
	@echo -n "  running (BM U A) \ (BM & A) : "
	symdiff <2m.dfa 1.dfa | language | sort | diff -q - symdiff.ref
	@echo -n "  running (AM U B) \ (AM & B) : "
	symdiff <1m.dfa 2.dfa | language | sort | diff -q - symdiff.ref
	@echo -n "  running (A U BM) \ (A & BM) : "
	symdiff <1.dfa 2m.dfa | language | sort | diff -q - symdiff.ref

concat: concat.ref
	@echo "Checking concat..."
	concat <11.dfa 2.dfa | language | sort | diff -q - concat.ref
	concat <11m.dfa 2.dfa | language | sort | diff -q - concat.ref
	concat <11.dfa 2m.dfa | language | sort | diff -q - concat.ref
	concat <11m.dfa 2m.dfa | language | sort | diff -q - concat.ref

match:
	@echo "Checking match..."
	match -l <1.dfa $(DS1) | diff -q - $(DS1)
	match -l <2.dfa $(DS2) | diff -q - $(DS2)
	match -l <1m.dfa $(DS1) | diff -q - $(DS1)
	match -l <2m.dfa $(DS2) | diff -q - $(DS2)

minimize: dfarefm.dfa
	@echo "Checking minimize..."
	@echo -n "checking Revuz's algorithm : "
	tree <$(DS1) | minimize -a | language | sort | diff -q - $(DS1)
	tree <$(DS2) | minimize -a | language | sort | diff -q - $(DS2)
	tree <$(DS1) | minimize -a | minimize -a | language | sort | diff -q - $(DS1)
	@echo -n "checking Brzozowski's algorithm : "
	tree <$(DS1) | minimize -c | language | sort | diff -q - $(DS1)
	tree <$(DS2) | minimize -c | language | sort | diff -q - $(DS2)
	tree <$(DS1) | minimize -c | minimize -c | language | sort | diff -q - $(DS1)
	reverse -c -m yes <../bin/dfaref.dfa | reverse -c -m yes | diff -q - dfarefm.dfa

reverse:
	@echo "Checking reverse..."
	tree <$(DS1) | reverse -m yes | reverse | language | sort | diff -q - $(DS1)
	rm -f /tmp/rev.txt
	echo DCBA >/tmp/rev.txt
	echo "1 65 2 2 66 3 3 67 4 4 68 -5" | reverse | language | diff -q - /tmp/rev.txt
	rm -f /tmp/rev.txt

isomorph: 
	@echo "Checking isomorph..."
	@tree <$(DS1) | minimize -c | isomorph 1m.dfa; if [ $$? -ne 0 ]; then echo "  error test1"; exit 1; fi
	@tree <$(DS2) | minimize -c | isomorph 2m.dfa; if [ $$? -ne 0 ]; then echo "  error test2"; exit 1; fi

compress:
	@echo "Checking compress.."
	rm -f dfaref.cpr; $(BIN)/compress -m yes -b dfaref.cpr <$(DFAREF)
	@$(BIN)/compress -m yes -d dfaref.cpr | isomorph $(DFAREF); if [ $$? -ne 0 ]; then echo "  error"; exit 1; fi
	rm -f dfaref.cpr
	tree <$(DS1) | compress -b tree.cpr; compress -d tree.cpr | language | sort | diff -q - $(DS1)
	tree <$(DS1) | minimize -a | compress -b tree.cpr; compress -d tree.cpr | language | sort | diff -q - $(DS1)

compact:
	@echo "Checking compact.."
	rm -f dfaref.cmp; compact -b dfaref.cmp <$(DFAREF)
	@compact -m yes -d dfaref.cmp | isomorph $(DFAREF); if [ $$? -ne 0 ]; then echo "  error"; exit 1; fi
	rm -f dfaref.cmp
	tree <$(DS1) | compact -b tree.cmp; compact -d tree.cmp | language | sort | diff -q - $(DS1)
	tree <$(DS1) | minimize -a | compact -b tree.cmp; compact -d tree.cmp | language | sort | diff -q - $(DS1)

ahocorasick:
	@echo "Checking aho-corasick algorithm..."
	ahocorasick $(DS2) <$(DS1)

strcursor:
	@echo "Checking string cursor..."
	@if [ `strcursor strcursor | language` = strcursor ]; then echo "  ok"; else echo "  error"; exit 1; fi


# how to build reference files
ddiff1.ref: $(DS1) $(DS2)
	comm -2 -3 $(DS1) $(DS2) >ddiff1.ref

ddiff2.ref: $(DS1) $(DS2)
	comm -1 -3 $(DS1) $(DS2) >ddiff2.ref

symdiff.ref: intersection.ref union.ref
	comm -2 -3 union.ref intersection.ref >symdiff.ref

intersection.ref: $(DS1) $(DS2)
	comm -1 -2 $(DS1) $(DS2) >intersection.ref

union.ref: $(DS1) $(DS2)
	 sort -u $(DS1) $(DS2) >$@

concat.ref: $(DS1) $(DS2) 11.dfa 11m.dfa
	rm -f concat.ref
	for i in `head -6 $(DS1)`; do for j in `cat $(DS2)`; do echo $$i$$j; done; done | sort >concat.ref

# how to build DFAs from dictionaries
1.dfa: $(DS1)
	tree <$(DS1) >1.dfa

2.dfa: $(DS2)
	tree <$(DS2) >2.dfa

dfarefm.dfa: ../bin/dfaref.dfa
	minimize -c -m yes <$< >$@

../bin/dfaref.dfa: ../bin/dfaref.txt
	build -m yes <$< >$@

../bin/dfaref.dot: ../bin/dfaref.dfa
	dotout <$< >$@

../bin/dfaref.ps: ../bin/dfaref.dot
	dot -Tps $< >$@

1m.dfa: 1.dfa
	minimize -a <1.dfa >1m.dfa

2m.dfa: 2.dfa
	minimize -a <2.dfa >2m.dfa

11.dfa: $(DS1)
	head -6 $(DS1) | tree >11.dfa

11m.dfa: 11.dfa
	minimize -a <11.dfa >11m.dfa

$(DS1): $(D1)
	sort -u $(D1) >$(DS1)

$(DS2): $(D2)
	sort -u $(D2) >$(DS2)

# build a reference file to check for any change in the
# DFA random generator:
seed.check:
	for i in 1 2 3 4 5 10 97 149 2148; do generator -d 9 -s $$i cyclic; done >seed.check

seed: seed.check
	@echo "Checking random generator..."
	for i in 1 2 3 4 5 10 97 149 2148; do generator -d 9 -s $$i cyclic; done | diff -q - seed.check







