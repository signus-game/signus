#!/bin/sh

aclocal-1.6 -I admin
autoconf
automake-1.6 -a -c --foreign
