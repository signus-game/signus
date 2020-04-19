mission_src = mis1e.mis mis1h.mis mis1n.mis mis10e.mis mis10h.mis mis10n.mis \
	mis11e.mis mis11h.mis mis11n.mis mis12e.mis mis12h.mis mis12n.mis \
	mis13e.mis mis13h.mis mis13n.mis mis14e.mis mis14h.mis mis14n.mis \
	mis15e.mis mis15h.mis mis15n.mis mis16e.mis mis16h.mis mis16n.mis \
	mis17e.mis mis17h.mis mis17n.mis mis18e.mis mis18h.mis mis18n.mis \
	mis19e.mis mis19h.mis mis19n.mis mis2e.mis mis2h.mis mis2n.mis \
	mis3e.mis mis3h.mis mis3n.mis mis4e.mis mis4h.mis mis4n.mis mis5e.mis \
	mis5h.mis mis5n.mis mis6e.mis mis6h.mis mis6n.mis mis7e.mis mis7h.mis \
	mis7n.mis mis8e.mis mis8h.mis mis8n.mis mis9e.mis mis9h.mis mis9n.mis

dist_noinst_DATA = $(mission_src)

misdir = $(datadir)/signus/$(VERSION)/nolang

mis_DATA = missions.dat

missions.dat: $(mission_src)
	$(MAKEDAT) $@ /nohdr $^

clean-local:
	rm -f missions.dat
