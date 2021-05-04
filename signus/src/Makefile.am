
AM_CPPFLAGS = -ansi -pedantic -Wall -I. -I$(srcdir)/ai -I$(top_srcdir)/iniparser/src


ENGINE_FILES = \
           etables.cpp \
           etab_ai.cpp \
           etab_drw.cpp \
           etab_mov.cpp \
           memmng.cpp \
           anims.cpp \
           autofire.cpp \
           briefing.cpp \
           building.cpp \
           showrngs.cpp \
           datafile.cpp \
           engine.cpp \
           eng800.cpp \
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
	   stream.cpp \
           graydark_g.c

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

header_files = ai/aiair.h ai/aiglobal.h ai/aiground.h ai/ai.h ai/aimarine.h \
	ai/aitowers.h anims.h autofire.h briefing.h building.h consts.h \
	datafile.h eng800.h engine.h engtimer.h etables.h events.h explode.h \
	fields.h fonts.h global.h graphio.h iounreg.h jukebox.h keycodes.h \
	loadsave.h mainmenu.h memmng.h mission_screen.h mouse.h moveunit.h \
	resource.h samples.h shortcut.h showrngs.h sound.h stream.h system.h \
	transbox.h uaircraf.h uamphi.h uground.h ui_toolkit.h unit_id.h \
	units.h unmodif.h untables.h uships.h utowers.h utrains.h utrans.h \
	visiblty.h weapons.h

if SYSTEM_UNIX
ENGINE_FILES += unix.cpp
endif

if SYSTEM_WINDOWS
ENGINE_FILES += windows.cpp
endif


bin_PROGRAMS = signus
signus_SOURCES = $(ENGINE_FILES) $(AI_FILES) $(header_files)
signus_LDADD = $(SDL2_LIBS) $(libvorbis_LIBS) $(libjpeg_LIBS) \
               -L../iniparser -liniparser
