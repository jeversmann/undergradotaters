# use this to generate the report, especially
# if you change the bibliography info at all

pdflatex report.tex
bibtex report
pdflatex report.tex
pdflatex report.tex
