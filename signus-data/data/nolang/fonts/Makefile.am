
fntdir = $(datadir)/signus/nolang

fnt_DATA = fonts.dat

fonts.dat: 
	$(FNTMNG) $@ normal c $(srcdir)/normal
	$(FNTMNG) $@ huge   c $(srcdir)/huge
	$(FNTMNG) $@ tiny   c $(srcdir)/tiny

clean-local:
	rm -f fonts.dat
