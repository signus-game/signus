
misdir = $(datadir)/signus

mis_DATA = missions.dat

missions.dat: 
	$(MAKEDAT) $@ /nohdr $(srcdir)/*.mis

clean-local:
	rm -f missions.dat
