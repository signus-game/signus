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
