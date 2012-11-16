# set latexfile to the name of the main file without the .tex
latexfile = report

# put the names of figure files here.  include the .eps
figures = 

# if no references, run "touch latexfile.bbl; touch referencefile.bib"
# or delete dependency in .dvi and tarball sections
referencefile = sample

TEX = latex
BIBTEX = bibtex

# support subdirectories
VPATH = Figs

help:
	@echo "Help for this makefile"
	@echo "----------------------"
	@echo "make pdf  : Generate PDF file"
	@echo "make ps   : Generate PS file"
	@echo "make help : Show this help"

# reruns latex if needed.  to get rid of this capability, delete the
# three lines after the rule.  Delete .bbl dependency if not using
# BibTeX references.
# idea from http://ctan.unsw.edu.au/help/uk-tex-faq/Makefile
$(latexfile).dvi : $(figures) $(latexfile).tex $(latexfile).bbl
	$(TEX) $(latexfile).tex

$(latexfile).bbl: $(referencefile).bib
	$(TEX) $(latexfile).tex
	$(BIBTEX) $(latexfile).aux
	$(TEX) $(latexfile).tex

# keep .eps files in the same directory as the .fig files
%.eps : %.fig
	fig2dev -L eps $< > $(dir $< )$@

$(latexfile).pdf : $(latexfile).ps
	ps2pdf $(latexfile).ps $(latexfile).pdf

pdf : $(latexfile).pdf

$(latexfile).ps : $(latexfile).dvi
	dvips $(latexfile)

ps : $(latexfile).ps 

# make can't know all the sourcefiles.  some file may not have
# sourcefiles, e.g. eps's taken from other documents. 
$(latexfile).tar.gz : $(figures) $(latexfile).tex $(referencefile).bib
	tar -czvf $(latexfile).tar.gz $^

tarball: $(latexfile).tar.gz

