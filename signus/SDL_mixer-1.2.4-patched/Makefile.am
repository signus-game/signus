# Makefile.am for the SDL sample mixer library and players

noinst_LIBRARIES = libsignusSDL_mixer.a

SUBDIRS = @MUSIC_SUBDIRS@
DIST_SUBDIRS = mikmod

libsignusSDL_mixer_a_SOURCES =	\
	load_aiff.c		\
	load_aiff.h		\
	load_voc.c		\
	load_voc.h		\
	load_ogg.c		\
	load_ogg.h		\
	mixer.c			\
	music.c			\
	music_cmd.c		\
	music_cmd.h		\
	music_ogg.c		\
	music_ogg.h		\
	wavestream.c		\
	wavestream.h		\
	effect_position.c	\
	effect_stereoreverse.c	\
	effects_internal.c	\
	effects_internal.h

if USE_MIKMOD
MIKMOD_LIB = mikmod/libmikmod.a
else
MIKMOD_LIB =
endif

EXTRA_DIST =			\
	CHANGES			\
	autogen.sh README.Signus
