
SUBDIRS = nolang en cs

datdir = $(datadir)/signus/$(VERSION)

install-data-local:
	rm -f $(DESTDIR)$(datdir)/default
	ln -sf en $(DESTDIR)$(datdir)/default

