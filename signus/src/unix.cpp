/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002, 2003
 *  Vaclav Slavik, Richard Wunsch, Marek Wunsch
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

// UNIX-specific functions

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <clocale>
#include <sys/stat.h>

#include "memmng.h"

int create_dir(const char *path) {
	return mkdir(path, 0755);
}

void print_error(const char *fmt, ...) {
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

char *get_locale(void) {
	const char *locale;
	char *ret;

	locale = setlocale(LC_MESSAGES, NULL);

	if (!locale) {
		return NULL;
	}

	ret = (char*)memalloc(strlen(locale) + 1);

	if (ret) {
		strcpy(ret, locale);
	}

	return ret;
}

char *concat_path(const char *root, const char *path) {
	size_t pathlen, rootlen;
	char *ret;

	if (!root) {
		return NULL;
	}

	rootlen = strlen(root);
	pathlen = (path && *path) ? strlen(path) : 0;
	ret = (char*)memalloc(rootlen + pathlen + 2);

	if (!ret) {
		return NULL;
	}

	strcpy(ret, root);

	if (!path || !*path) {
		return ret;
	}

	if (rootlen && ret[rootlen - 1] != '/') {
		ret[rootlen++] = '/';
	}

	strcpy(ret + rootlen, path);
	return ret;
}

char *signus_data_path(const char *path) {
	const char *basedir;

	basedir = getenv("SIGNUS_DATA_DIR");

	if (!basedir || !*basedir) {
		basedir = SIGNUS_DATA_DIR;
	}

	return concat_path(basedir, path);
}

char *signus_config_path(const char *path) {
	const char *basedir;
	char *tmp, *ret;

	basedir = getenv("HOME");

	if (!basedir || !*basedir) {
		basedir = ".";
	}

	tmp = concat_path(basedir, ".signus");
	ret = concat_path(tmp, path);
	memfree(tmp);
	return ret;
}

int init_datadir(const char *exepath) {
	return 1;
}

void cleanup_datadir(void) {

}
