OBJ = tr.ps
SRC = 

all: ${OBJ}
src: ${SRC}

clean:
	rm -f *.dvi *.aux *.log *~ \#* *.bbl *.blg
	rm -f *.o *.s a.out 
	rm -f ${OBJ}

.dvi.ps:
	dvips $< -o $@

.tex.dvi:
	latex $< $@
	-bibtex `basename $< .tex`
	latex $<
	latex $<

.ps.pdf:
	ps2pdf $<

.c.s:
	gcc $(CFLAGS) -S $<

.SUFFIXES: .dvi .ps .pdf .tex .s
