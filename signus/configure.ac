dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision$)
AC_PREREQ(2.52)

AC_INIT([signus], [1.96.0], [zarevucky.jiri@gmail.com])

AC_CONFIG_SRCDIR([signus.spec])
AC_CONFIG_AUX_DIR([admin])

AC_CANONICAL_SYSTEM

dnl cross-compiling support: we're cross compiling if the build system is
dnl different from the target one (assume host and target be always the same)
if test "$build" != "$host" ; then
    CC=$host_alias-gcc
    CXX=$host_alias-c++
    AR=$host_alias-ar
    RANLIB=$host_alias-ranlib
    DLLTOOL=$host_alias-dlltool
    RESCOMP=$host_alias-windres
    LD=$host_alias-ld
    NM=$host_alias-nm
    STRIP=$host_alias-strip
fi

AM_INIT_AUTOMAKE([subdir-objects])

AM_MAINTAINER_MODE

AC_ARG_ENABLE(debug, [  --enable-debug          Enable debugging information],
              USE_DEBUG="$enableval", USE_DEBUG="no")

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CXXCPP

AM_PATH_SDL(1.2.0, [], [AC_MSG_ERROR([SDL is required by Signus.])])

XIPH_PATH_OGG([], [AC_MSG_ERROR([Ogg Vorbis is required by Signus.])])
AC_MSG_CHECKING(for Vorbis)
  AC_TRY_COMPILE([
  #include <vorbis/vorbisfile.h>
  ], [], 
  [AC_MSG_RESULT([yes])],
  [
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([Ogg Vorbis is required by Signus.])
  ])

AC_CHECK_LIB(jpeg, jpeg_set_colorspace, [], 
      [AC_MSG_ERROR([libjpeg is required by Signus.])])

AC_MSG_CHECKING([for SDL_mixer >= 1.2.5])
  AC_TRY_COMPILE([
  #include <SDL/SDL_mixer.h>
  #if !( \
       (MIX_MAJOR_VERSION > 1) || \
       (MIX_MAJOR_VERSION == 1 && MIX_MINOR_VERSION > 2) || \
       (MIX_MAJOR_VERSION == 1 && MIX_MINOR_VERSION == 2 && \
          MIX_PATCHLEVEL >= 5))
      #error "SDL_mixer too old!"
  #endif
  ], [], 
  [AC_MSG_RESULT([yes])],
  [
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([SDL_mixer >= 1.2.5 is required by Signus.])
  ])

AC_CHECK_HEADER([SDL/SDL_ttf.h],[],
                [AC_MSG_ERROR([SDL_ttf is required by Signus.])])

AC_CHECK_LIB(SDL_ttf, TTF_RenderUTF8_Shaded, [], 
      [AC_MSG_ERROR([SDL_ttf is required by Signus.])])


dnl FIXME - make it possible to compile signus w/o NASM
dnl FIXME - check for version-data file and contained version

case "$USE_DEBUG" in
yes)	DEBUG_FLAGS="-g -O0 -Wall"
        NASMFLAGS="$NASMFLAGS -g"
    ;;    
*)	DEBUG_FLAGS="-O3"
    ;;
esac

GCC_FLAGS="-fno-rtti -fno-exceptions"


data_version=`echo $VERSION | sed -e 's,\([[0-9]]*\.[[0-9]]*\)\.[[0-9]]*,\1,g'`
echo xxx $data_version xxx

CXXFLAGS="$SDL_CFLAGS $CXXFLAGS $SDL_CFLAGS $GCC_FLAGS $DEBUG_FLAGS \"-DSIGNUS_DATA_DIR=\\\"\$(pkgdatadir)/$data_version\\\"\""


AC_SUBST(LIBS)
AC_SUBST(LDFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(NASMFLAGS)
AC_SUBST(RESCOMP)

AM_CONFIG_HEADER([config.h])

AC_OUTPUT([
         Makefile
         src/Makefile
         iniparser/Makefile
         sdlimage/Makefile
         ])

