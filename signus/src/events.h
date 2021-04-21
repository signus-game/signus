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


//
// Hlavickovy soubor pro EVENTS.CPP
// Obsah: Kompletni funkce pro zpracovani udalosti z mysi & klavesnice
//


#ifndef _EVENTS_H
#define _EVENTS_H

#include "global.h"
#include "system.h"
#include "graphio.h"

#include <SDL.h>
#include <SDL_mouse.h>


// Scan kody spousty klaves:
#include "keycodes.h"


//////////////////////////////////////////
// Globalni fce a veci kolem udalosti:
//


// ------------ Kody udalosti:
#define evMouseDown   0x0001
#define evMouseUp     0x0002
#define evMouseMove   0x0004
#define evMouseScroll 0x0008
#define evKeyDown     0x0010
#define evTimer       0x0100

// ------------ Masky udalosti:
#define evNothing     0x0000
#define evMouse       0x000F
#define evKeyboard    0x0010
#define evBroadcast   0x0100

// ----------- Masky tlacitek mysi:
#define mbLeftButton   SDL_BUTTON_LEFT
#define mbRightButton  SDL_BUTTON_RIGHT
#define mbBottomButton SDL_BUTTON_MIDDLE



typedef struct {
            byte   Buttons;
            TPoint Where;
	    int    Scroll;
} TMouseEvent;

typedef struct {
            SDL_Keycode KeyCode;
            char CharCode;
} TKeyEvent;

typedef struct {
            int What;
            TMouseEvent Mouse;
            TKeyEvent Key;
} TEvent;



extern void PutEvent(TEvent *e);
extern void GetEvent(TEvent *e);
extern void ClearEvent();

// Stav shift klaves:
inline byte GetShiftState() {return (SDL_GetModState() & KMOD_SHIFT);}


#endif
