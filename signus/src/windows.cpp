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

// Windows-specific functions

#include <windows.h>
#include <direct.h>
#include <SDL_syswm.h>
#include "global.h"

int window_info(SDL_SysWMinfo *info);

static char *data_basepath = NULL;

int create_dir(const char *path) {
	return mkdir(path);
}

void print_error(const char *fmt, ...) {
	SDL_SysWMinfo winfo;
	HWND wnd = NULL;
	char *message;
	va_list args;

	va_start(args, fmt);
	message = format_string(fmt, args);

	if (!message) {
		return;
	}

	va_end(args);

	if (window_info(&winfo) && winfo.subsystem == SDL_SYSWM_WINDOWS) {
		wnd = winfo.info.win.window;
	}

	MessageBox(wnd, message, "Error", MB_ICONERROR | MB_OK);
	memfree(message);
}

char *get_locale(void) {
	char *ret;
	size_t tmp, len, size = 32;

	ret = (char*)memalloc(size);

	if (!ret) {
		return NULL;
	}

	*ret = '\0';
	len = GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SISO639LANGNAME, ret,
		size - 1);

	if (!len) {
		memfree(ret);
		return NULL;
	}

	len = strlen(ret);
	ret[len++] = '_';
	tmp = GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME,
		ret + len, size - len);

	if (!tmp) {
		ret[--len] = '\0';
	}

	return ret;
}

char *concat_path(const char *root, const char *path) {
	size_t i, pathlen, rootlen;
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

	if (path && *path) {
		if (rootlen && ret[rootlen - 1] != '/' &&
			ret[rootlen - 1] != '\\') {
			ret[rootlen++] = '\\';
		}

		strcpy(ret + rootlen, path);
	}

	for (i = 0; ret[i]; i++) {
		if (ret[i] == '/') {
			ret[i] = '\\';
		}
	}

	return ret;
}

char *signus_data_path(const char *path) {
	return concat_path(data_basepath, path);
}

char *signus_config_path(const char *path) {
	const char *basedir;
	char *tmp, *ret;

	basedir = getenv("APPDATA");

	if (!basedir || !*basedir) {
		return signus_data_path(path);
	}

	tmp = concat_path(basedir, "signus");
	ret = concat_path(tmp, path);
	memfree(tmp);
	return ret;
}

int init_datadir(const char *exepath) {
	size_t i;
	const char *basepath = ".";
	char *tmp;

	for (i = 0; exepath[i]; i++) {
		if (exepath[i] == '\\') {
			basepath = exepath;
			break;
		}
	}

	data_basepath = (char*)memalloc(strlen(basepath) + 1);

	if (!data_basepath) {
		print_error("Cannot allocate memory");
		return 0;
	}

	strcpy(data_basepath, basepath);
	tmp = strrchr(data_basepath, '\\');

	if (tmp) {
		*tmp = '\0';
	}

	return 1;
}

void cleanup_datadir(void) {
	memfree(data_basepath);
}
