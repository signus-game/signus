graphics_src = bitmaps/load.png bitmaps/menu0.png bitmaps/menu1.png \
	bitmaps/menu2.png bitmaps/mmnu0.png bitmaps/mmnu1.png \
	bitmaps/mmnu2.png bitmaps/mmnu3.png bitmaps/repair0.png \
	bitmaps/repair1.png bitmaps/vsettr0.png bitmaps/vsettr1.png \
	bitmaps/vsettr2.png

dist_noinst_DATA = $(graphics_src)

gradir = $(datadir)/signus/$(VERSION)/cs

gra_DATA = graphics.dat

graphics.dat: $(graphics_src)
	rm -f $@
	$(MAKEDAT) $@ $^

clean-local:
	rm -f graphics.dat



screendir = $(datadir)/signus/$(VERSION)/cs/screens

dist_screen_DATA =   screens/mis1-name.jpg \
                     screens/mis2-name.jpg \
                     screens/mis3-name.jpg \
                     screens/mis4-name.jpg \
                     screens/mis5-name.jpg \
                     screens/mis6-name.jpg \
                     screens/mis7-name.jpg \
                     screens/mis8-name.jpg \
                     screens/mis9-name.jpg \
                     screens/mis10-name.jpg \
                     screens/mis11-name.jpg \
                     screens/mis12-name.jpg \
                     screens/mis13-name.jpg \
                     screens/mis14-name.jpg \
                     screens/mis15-name.jpg \
                     screens/mis16-name.jpg \
                     screens/mis17-name.jpg \
                     screens/mis18-name.jpg \
                     screens/mis19-name.jpg
