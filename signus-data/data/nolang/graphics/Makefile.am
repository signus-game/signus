
gradir = $(datadir)/signus/nolang

gra_DATA = graphics-common.dat

graphics-common.dat: scr_rgb.tab
	rm -f $@
	$(MAKEDAT) $@ $(srcdir)/*.{col,tab} ./*.tab
	$(MAKEDAT) $@ /mark $(srcdir)/bitmaps/*
	$(MAKEDAT) $@ /mark $(srcdir)/tiles/*.tile.png
	$(MAKEDAT) $@ /mark $(srcdir)/sprites-terrain/*.spr.png
	$(MAKEDAT) $@ /mark $(srcdir)/sprites-misc/*.mem $(srcdir)/sprites-misc/*.spr.png
	$(MAKEDAT) $@ /mark $(srcdir)/sprites-units-goodlife/*.spr.png
	$(MAKEDAT) $@ /mark $(srcdir)/sprites-units-badlife/*.spr.png
    
scr_rgb.tab: $(srcdir)/scrpal.col
	$(MK_RGB2PAL) $< $@

clean-local:
	rm -f graphics-common.dat scr_rgb.tab


screendir = $(datadir)/signus/nolang/screens

screen_DATA = screens/mis1-image.jpg \
              screens/mis2-image.jpg \
              screens/mis3-image.jpg \
              screens/mis4-image.jpg \
              screens/mis5-image.jpg \
              screens/mis6-image.jpg \
              screens/mis7-image.jpg \
              screens/mis8-image.jpg \
              screens/mis9-image.jpg \
              screens/mis10-image.jpg \
              screens/mis11-image.jpg \
              screens/mis12-image.jpg \
              screens/mis13-image.jpg \
              screens/mis14-image.jpg \
              screens/mis15-image.jpg \
              screens/mis16-image.jpg \
              screens/mis17-image.jpg \
              screens/mis18-image.jpg \
              screens/mis19-image.jpg \
              screens/background.jpg
