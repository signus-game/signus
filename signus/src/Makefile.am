
INCLUDES = -I. -I$(srcdir)/ai -I$(top_srcdir)/iniparser/src \
           -I$(top_srcdir)/SDL_mixer-1.2.4-patched

X86_FILES = \
           engdraw_x86.asm \
           graydark_x86.asm

GENERIC_FILES = \
           graydark_g.c

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
           $(X86_FILES)

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
               -L../SDL_mixer-1.2.4-patched -lsignusSDL_mixer \
               -L../SDL_mixer-1.2.4-patched/mikmod -lsignusmikmod \
               -lvorbisfile -lvorbis -logg \
               -L../iniparser -liniparser \
               -L../sdlimage -lminiSDLimage -ljpeg

.asm.o:
	$(NASM) $(NASMFLAGS) -o $@ $<
