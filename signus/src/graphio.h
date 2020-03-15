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
// Hlavickovy soubor pro GRAPHIO.CPP a VGAKIT.ASM
// Obsah: init, done videa, 
//        prepinani video bank, nastav. palety apod.
//


#ifndef _GRAPHIO_H
#define _GRAPHIO_H



#include "system.h"
#include <SDL/SDL.h>





// (in-game) screen resolution:
#define RES_X    800
#define RES_Y    600
//was: extern int ResX, ResY; 



extern int InitVideoCard(int mode);
extern int DoneVideoCard();



extern int InitVideo(int mode);
// inicializace videa
extern int DoneVideo();
// vypnuti videa a obnoveni puvodniho rezimu

extern void SetPalette(char *paldat);
// nastavi barevnou paletu z bufferu
extern void SetPalettePart(char *paldat, int palofs, int palsize);
// nastavi cast palety

extern SDL_Surface *GetScreenSurface();

// toggles fullscreen mode
extern void ToggleFullscreen();

///////////////////////////////////////////////////////
// Funkce vkladajici na obrazovky ruzne vyrezy: 
//


// Vykresli mapu:
extern void DrawMapBuf();

// a tyhle kresli vysec uplne v pohode:
extern void DrawMapBufP(int x, int y, int sx, int sy);
#define     DrawMapBufP32(x, y, sx, sy)                  DrawMapBufP(x, y, sx, sy)

extern void ClearScr();
// Vykresli na celou obrazovku celoobrazovkovou bitmapu:
extern void DrawPicture(void *src);
extern void DrawPictureNB(void *src);
// Vykresli mapicku vpravo nahore:
extern void DrawLitMap();

extern void DrawCursor(void *src, int x, int y, int sx, int sy, int fromx, int fromy);
    // Vykresli kurzor mysi s rozmery 24x24 pixelu:
extern void GetCurBack(void *src, int x, int y, int sx, int sy, int fromx, int fromy);
    // Sejme pozadi pod kurzorem:
extern void PutCurBack(void *src, int x, int y, int sx, int sy, int fromx, int fromy);
    // Zabrazi drive sejmute pozadi:





// Kopirovani bmp do bmp:
extern void CopyBmp(void *tar, int tarwidth, int x, int y, void *src, int w, int h);
extern void CopyBmpNZ(void *tar, int tarwidth, int x, int y, void *src, int w, int h);
extern void CopyFromBmp(void *src, int srcwidth, int x, int y, void *tar, int w, int h);
// Kresleni do bitmapy:
extern void BarBmp(void *tar, int tarwidth, int x, int y, int w, int h, byte clr);
extern void BarBmpSwap(void *tar, int tarwidth, int x, int y, int w, int h, byte clr1, byte clr2);
extern void PercentBar(void *tar, int width, int height, int x, int y, int w, int h, byte clr1, byte clr2, double value, const char *text);
extern void RectBmp(void *tar, int tarwidth, int x, int y, int w, int h, byte clr1, byte clr2);


// Vykresli obecnou bitmapu:
extern void PutBitmap(int x, int y, void *data, int w, int h);
extern void PutBitmapNZ(int x, int y, void *data, int w, int h);
#define     PutBitmap32(x, y, data, w, h)                           PutBitmap(x, y, data, w, h)
extern void GetBitmap(int x, int y, void *data, int w, int h);
#define     GetBitmap32(x, y, data, w, h)                           GetBitmap(x, y, data, w, h)



// Zatmeni a rozsviceni palety:
extern void FadeOut(char *paldat, int dlay);
extern void FadeIn(char *paldat, int dlay);


// converts RGB SDL surface into 8bit palettized image array
extern void paletizeSurface(byte *output, SDL_Surface *surf, const char *tableName);

#endif
