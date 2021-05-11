/*
 *  This file is part of Signus: The Artefact Wars
 *  https://github.com/signus-game
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


//
// Modul pro klavesnici a mys
//


#include <SDL.h>
#include <SDL_mouse.h>
#include "mouse.h"
#include "graphio.h"
#include "system.h"
#include "global.h"
#include <time.h>


//////////////////////////////// MYS /////////////////////////////////////

TMouseCBD Mouse = {0};
// pomocna promnena - obsahuje vsechny data pouzivana behem INT handleru

static SDL_Cursor *MouseCurs[mcurCnt] = {NULL};

SDL_Cursor *load_cursor(const char *name, int hot_x, int hot_y) {
	SDL_Surface *surf;
	SDL_Cursor *ret;
	void *bitmap;

	bitmap = GraphicsDF->get(name);

	if (!bitmap) {
		return NULL;
	}

	surf = SDL_CreateRGBSurfaceWithFormatFrom(bitmap, 32, 32, 8, 32,
		SDL_PIXELFORMAT_INDEX8);

	if (!surf) {
		memfree(bitmap);
		return NULL;
	}

	SDL_SetPaletteColors(surf->format->palette, PaletteSDL, 0, 256);
	SDL_SetColorKey(surf, SDL_TRUE, 0);
	ret = SDL_CreateColorCursor(surf, hot_x, hot_y);
	SDL_FreeSurface(surf);
	memfree(bitmap);
	return ret;
}

int MouseInit()
{
	MouseCurs[mcurArrow] = load_cursor("curarrow", 0, 0);

	if (!MouseCurs[mcurArrow]) {
		return 0;
	}

	MouseCurs[mcurWait] = load_cursor("curwait", 16, 16);
	MouseCurs[mcurSelect] = load_cursor("cursel", 16, 16);
	MouseCurs[mcurSelectBig] = load_cursor("curselb", 16, 16);
	MouseCurs[mcurSelectSmall] = load_cursor("cursels", 16, 16);
	MouseCurs[mcurTarget] = load_cursor("curtar", 16, 16);
	MouseCurs[mcurTargetBig] = load_cursor("curtarb", 16, 16);
	MouseCurs[mcurTargetSmall] = load_cursor("curtars", 16, 16);
	MouseCurs[mcurSupport] = load_cursor("cursupp", 0, 0);
	MouseCurs[mcurLoadIn] = load_cursor("curload", 16, 16);
	MouseCurs[mcurBuild] = load_cursor("curbuild", 16, 16);
	
	Mouse.Locks = 1;
	MouseSetCursor(mcurArrow);
	MouseSetRect(0, 0, RES_X-1, RES_Y-1);
	MouseSetPos(320, 240);
	
	MouseShow();
	return 1;
}



void MouseDone() {
	int i;
	SDL_Cursor *cur = SDL_GetDefaultCursor();

	if (cur) {
		SDL_SetCursor(cur);
	}

	for (i = 0; i < mcurCnt; i++) {
		if (MouseCurs[i]) {
			SDL_FreeCursor(MouseCurs[i]);
		}
	}
}




void MouseSetCursor(int cnum) {
	if (Mouse.ActCur == cnum) {
		return;
	}

	if (cnum < 0 || cnum >= mcurCnt || !MouseCurs[cnum]) {
		return;
	}

	Mouse.SuperLocks++;
	SDL_SetCursor(MouseCurs[cnum]);
	Mouse.ActCur = cnum;
	Mouse.SuperLocks--;
}



void MouseHide() {
	Mouse.SuperLocks++;
	Mouse.Locks++;
	SDL_ShowCursor(SDL_DISABLE);
	Mouse.SuperLocks--;
}



void MouseShow()
{    
	Mouse.SuperLocks++;

	if (!--Mouse.Locks) {
		SDL_ShowCursor(SDL_ENABLE);
	}

	Mouse.SuperLocks--;
}



// Zakaze kurzor v dane oblasti:
void MouseFreeze(int x, int y, int w, int h) {
}



void MouseUnfreeze() {
}



void MouseSetRect(int x1, int y1, int x2, int y2)
{
	#if 0
	TRect r;
	
	r.x1 = x1, r.y1 = y1, r.x2 = x2, r.y2 = y2;
	ClipCursor(&r); /// FIXME -- SDL cannot clip mouse cursor, do it ourselves!
SDL_WM_GrabInput(SDL_GRAB_ON);
	#endif
}

int IsIntersect(int ax, int ay, int bx, int by, int rax, int ray, int rbx, int rby)
{
	return ((max(ax, rax) <= min(bx, rbx)) && 
	(max(ay, ray) <= min(by, rby)));    
}

void MousePaint() {
}




