
gradir = $(datadir)/signus/nolang

gra_DATA = graphics-common.dat

graphics-common.dat: scr_rgb.tab pal_rgb.tab
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

pal_rgb.tab: $(srcdir)/palette.col
	$(MK_RGB2PAL) $< $@

clean-local:
	rm -f graphics-common.dat scr_rgb.tab pal_rgb.tab


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

unitdir = $(datadir)/signus/nolang/unit-images

unit_DATA = \
			  unit-images/unit1.jpg \
			  unit-images/unit2.jpg \
			  unit-images/unit3.jpg \
			  unit-images/unit4.jpg \
			  unit-images/unit5.jpg \
			  unit-images/unit6.jpg \
			  unit-images/unit7.jpg \
			  unit-images/unit8.jpg \
			  unit-images/unit9.jpg \
			  unit-images/unit10.jpg \
			  unit-images/unit11.jpg \
			  unit-images/unit12.jpg \
			  unit-images/unit13.jpg \
			  unit-images/unit14.jpg \
			  unit-images/unit15.jpg \
			  unit-images/unit16.jpg \
			  unit-images/unit17.jpg \
			  unit-images/unit18.jpg \
			  unit-images/unit19.jpg \
			  unit-images/unit20.jpg \
			  unit-images/unit21.jpg \
			  unit-images/unit22.jpg \
			  unit-images/unit23.jpg \
			  unit-images/unit24.jpg \
			  unit-images/unit25.jpg \
			  unit-images/unit26.jpg \
			  unit-images/unit27.jpg \
			  unit-images/unit28.jpg \
			  unit-images/unit29.jpg \
			  unit-images/unit30.jpg \
			  unit-images/unit31.jpg \
			  unit-images/unit32.jpg \
			  unit-images/unit33.jpg \
			  unit-images/unit34.jpg \
			  unit-images/unit35.jpg \
			  unit-images/unit36.jpg \
			  unit-images/unit37.jpg \
			  unit-images/unit38.jpg \
			  unit-images/unit39.jpg \
			  unit-images/unit40.jpg \
			  unit-images/unit41.jpg \
			  unit-images/unit42.jpg \
			  unit-images/unit43.jpg \
			  unit-images/unit44.jpg \
			  unit-images/unit45.jpg \
			  unit-images/unit50.jpg \
			  unit-images/unit51.jpg \
			  unit-images/unit52.jpg \
			  unit-images/unit53.jpg \
			  unit-images/unit54.jpg \
			  unit-images/unit55.jpg \
			  unit-images/unit56.jpg \
			  unit-images/unit57.jpg \
			  unit-images/unit58.jpg
