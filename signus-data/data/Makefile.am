
SUBDIRS = nolang en cs

datdir = $(datadir)/signus
dat_DATA = version-data

install-data-local:
	rm -f $(datdir)/default
	ln -sf en $(datdir)/default

