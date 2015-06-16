
AM_CPPFLAGS = -ansi -pedantic -Wall -Werror -I. -I$(srcdir)/ai -I$(top_srcdir)/iniparser/src \
		   -I$(top_srcdir)/sdlimage -fsanitize=address -fstack-protector -fsanitize=undefined

AM_LDFLAGS = -fsanitize=address -fstack-protector -fsanitize=undefined

ENGINE_FILES = \
           etables.cpp \
           memmng.cpp \
           anims.cpp \
           autofire.cpp \
           briefing.cpp \
           building.cpp \
           showrngs.cpp \
           datafile.cpp \
           engine.cpp \
           explode.cpp \
           fields.cpp \
           fonts.cpp \
           infobox.cpp \
           ui_toolkit.cpp \
           mainmenu.cpp \
           moveunit.cpp \
           transbox.cpp \
           uaircraf.cpp \
           uamphi.cpp \
           uground.cpp \
           units.cpp \
           unmodif.cpp \
           uships.cpp \
           utowers.cpp \
           utrains.cpp \
           utrans.cpp \
           visiblty.cpp \
           weapons.cpp \
           jukebox.cpp \
           signus.cpp \
           loadsave.cpp \
           global.cpp \
           engtimer.cpp \
           sound.cpp \
           mouse.cpp \
           events.cpp \
           graphio.cpp \
           main.cpp \
           mission_screen.cpp \
           graydark_g.c \
           engdraw_g_1.cpp \
           engdraw_g_2.cpp

AI_FILES = \
           ai/ai.cpp \
           ai/aiair.cpp \
           ai/aiglobal.cpp \
           ai/aiground.cpp \
           ai/aimarine.cpp \
           ai/aitowers.cpp \
           ai/ai1.cpp \
           ai/ai2.cpp \
           ai/ai3.cpp \
           ai/ai4.cpp \
           ai/ai5.cpp \
           ai/ai6.cpp \
           ai/ai7.cpp \
           ai/ai8.cpp \
           ai/ai9.cpp \
           ai/ai10.cpp \
           ai/ai11.cpp \
           ai/ai12.cpp \
           ai/ai13.cpp \
           ai/ai14.cpp \
           ai/ai15.cpp \
           ai/ai16.cpp \
           ai/ai17.cpp \
           ai/ai18.cpp \
           ai/ai19.cpp

bin_PROGRAMS = signus
signus_SOURCES = $(ENGINE_FILES) $(AI_FILES)
signus_LDADD = $(SDL_LIBS) \
               -lSDL_mixer \
               -L../iniparser -liniparser \
               -L../sdlimage -lminiSDLimage -ljpeg

.asm.o:
	$(NASM) $(NASMFLAGS) -I$(srcdir)/ -o $@ $<
