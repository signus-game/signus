
noinst_LIBRARIES = libsignusdata.a

libsignusdata_a_SOURCES = datafile.cpp datafile.h fonts.cpp fonts.h \
	stream.cpp stream.h system.h

noinst_PROGRAMS = makedat mk_rgb2pal fntmng

makedat_SOURCES = makedat.cpp
makedat_LDADD = libsignusdata.a -lpng -lz

mk_rgb2pal_SOURCES = mk_rgb2pal.c

fntmng_SOURCES = fntmng.cpp
fntmng_LDADD = libsignusdata.a -lpng -lz
