dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision$)
AC_PREREQ([2.71])

AC_INIT([signus], [1.96.0], [next_ghost@quick.cz])

AC_CONFIG_SRCDIR([signus.spec])
AC_CONFIG_AUX_DIR([admin])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

case "${host_os}" in
linux*|darwin*|freebsd*|netbsd*|openbsd*)
	signus_system=unix ;;
cygwin*|mingw*)
	signus_system=windows ;;
*)
	AC_MSG_ERROR(["$host_os is not supported (yet)"])
esac

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

AM_CONDITIONAL([SYSTEM_UNIX], [test "x$signus_system" = "xunix"])
AM_CONDITIONAL([SYSTEM_WINDOWS], [test "x$signus_system" = "xwindows"])

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

PKG_CHECK_MODULES([SDL2], [sdl2 >= 2.0.5 SDL2_mixer SDL2_ttf SDL2_image])
PKG_CHECK_MODULES([libvorbis], [vorbis])
PKG_CHECK_MODULES([libjpeg], [libjpeg])

dnl FIXME - check for version-data file and contained version

case "$USE_DEBUG" in
yes)	DEBUG_FLAGS="-g -O0 -Wall"
    ;;    
*)	DEBUG_FLAGS="-O3"
    ;;
esac

GCC_FLAGS="-fno-rtti -fno-exceptions"


DATA_VERSION=`echo $VERSION | sed -e 's,\([[0-9]]*\.[[0-9]]*\)\.[[0-9]]*,\1,g'`
echo xxx $DATA_VERSION xxx

CXXFLAGS="$SDL2_CFLAGS $libvorbis_CFLAGS $libjpeg_CFLAGS $CXXFLAGS $GCC_FLAGS $DEBUG_FLAGS \"-DSIGNUS_DATA_DIR=\\\"\$(pkgdatadir)/$DATA_VERSION\\\"\""


AC_SUBST(LIBS)
AC_SUBST(LDFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(RESCOMP)
AC_SUBST(DATA_VERSION)

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([
         Makefile
         src/Makefile
         iniparser/Makefile
         ])
AC_OUTPUT

