ASTL=astl2

all: postscript PDF

postscript ps: $(ASTL).ps

dvi: $(ASTL).dvi

PDF: $(ASTL).pdf

$(ASTL).pdf: $(ASTL).dvi
	dvipdf $(ASTL).dvi $(ASTL).pdf

$(ASTL).ps: $(ASTL).dvi
	dvips $(ASTL).dvi -o $(ASTL).ps

$(ASTL).dvi: $(ASTL).tex algorithms.tex cursor.tex forward_cursor.tex
	latex $(ASTL)

.DELETE_ON_ERROR: algorithms.tex

algorithms.tex: algorithm.tex makedoc.pl
	./makedoc.pl ../include/*.h >$@

clean:
	rm -f $(ASTL).aux
	rm -f $(ASTL).dvi
	rm -f $(ASTL).log
	rm -f $(ASTL).pdf
	rm -f $(ASTL).ps
	rm -f $(ASTL).toc
	rm -f algorithms.tex
	rm -f *~



