#!/bin/sh

aclocal-1.6 -I admin
autoconf
autoheader
automake-1.6 -a -c --foreign


#libtoolize -c -f
#cat m4/*.m4 >acinclude.m4
#aclocal
#automake --foreign -a -c
#autoconf
#autoheader

