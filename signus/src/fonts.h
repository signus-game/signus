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
// Hlavickovy soubor pro FONTS.CPP a FONTSASM.ASM
//
// Obsah: nacitani, ukladani a zobrazovani text. fontu
//


#ifndef _FONTS_H
#define _FONTS_H

#include <stdio.h>
#include "system.h"

#include <SDL/SDL_ttf.h>
typedef TTF_Font TFont;

// Vykresli do TAR text dvema barvami. Funguji i znaky CR, LF
extern void PutStr(void *tar, int width, int height, int xpoz, int ypoz,
	const char *s, TFont *f, byte clr1, byte clr2);

// Get text dimensions:
#define GetStrDimensions(f,s,w,h) TTF_SizeUTF8(f,s,w,h)

// Get text width and height:
inline int GetStrWidth(const char *s, TFont *f)
{
    int w, h;
    GetStrDimensions(f, s, &w, &h);
    return w;
}

inline int GetStrHeight(const char *s, TFont *f)
{
    int w, h;
    GetStrDimensions(f, s, &w, &h);
    return h;
}

#endif
