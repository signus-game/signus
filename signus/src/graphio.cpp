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
// Modul grafickeho rozhrani
//

#include "global.h"
#include "graphio.h"
#include "system.h"
#include "events.h"
#include "mouse.h"
#include "engine.h"

#include <assert.h>
#include <time.h>
#include <SDL_timer.h>
#include <SDL.h>



void ClearScr()
{
    // pameti je na to dost, protoze Signus je NENAHOZENEJ tak proc ne...
    void *me = memalloc(RES_X * RES_Y);
    memset(me, 0, RES_X * RES_Y);
    DrawPicture(me);
    memfree(me);
}



///////////////////////////////////////////////////////////////////////////////////////////////////

//                               DirectX 5.0 interface (DirectDraw)                              //

///////////////////////////////////////////////////////////////////////////////////////////////////

byte *VideoLFB = NULL;
unsigned LFB_Pitch = 0;

SDL_Surface *screen;
SDL_Color    palette[256];


////////////////////////////// rozhrani k frame-bufferu:

inline int SomeoneDrawing() {return (VideoLFB != NULL);}

void *LFB_Lock()
{
    SDL_LockSurface(screen);
    VideoLFB = (byte*)screen->pixels;
    LFB_Pitch = screen->pitch;

    return VideoLFB;
}



void LFB_Unlock()
{
    SDL_UnlockSurface(screen);
    VideoLFB = NULL;
}



SDL_Surface *GetScreenSurface()
{
    return screen;
}


//////////////////// funkce Signusu:

#include "system.h"

bool SwitchDisplayMode(int mode)
{
    assert(mode == SVGA_800x600 || mode == SVGA_640x480);
    
    if (mode == SVGA_800x600)
        screen = SDL_SetVideoMode(800, 600, 8, SDL_SWSURFACE);
    else if (mode == SVGA_640x480)
        screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
    if (screen == NULL) return FALSE;
    
    memset(palette, 0, sizeof(palette));
    SDL_SetColors(screen, palette, 0, 256);
    
    ClearScr();
    return TRUE;
}



int InitVideo(int mode)
{
    if (SwitchDisplayMode(mode))
        return TRUE;

    char buf[512];
    sprintf(buf, "SDL initialization failed: %s\n", SDL_GetError());
#ifdef __unix__
    fprintf(stderr, buf);
#else
    MessageBox(hWindow, buf, "ERROR", MB_OK | MB_ICONERROR);
#endif
    return FALSE;
}




int DoneVideo()
{
    SDL_FreeSurface(screen);
    screen = NULL;
    return TRUE;
}



void ToggleFullscreen()
{
    SDL_WM_ToggleFullScreen(screen);
}


///////// DrawPicture

void DrawPicture(void *src)
{
    register unsigned y;
    register byte *ptr_s = (byte *)src;
    register byte *ptr_t;

    if (SomeoneDrawing()) return;
    EnterTCS();
    ptr_t = (byte *)LFB_Lock();
    for (y = RES_Y; y != 0; y--) {
        memcpy(ptr_t, ptr_s, RES_X);
        ptr_t += LFB_Pitch, ptr_s += RES_X;
    }
    LFB_Unlock();
    LeaveTCS();
    SDL_UpdateRect(screen, 0, 0, RES_X, RES_Y);
}



void DrawPictureNB(void *src)
{
    register unsigned y, x;
    register byte *ptr_s = (byte *)src;
    register byte *ptr_t;

    if (SomeoneDrawing()) return;
    EnterTCS();
    ptr_t = (byte *)LFB_Lock();
    for (y = RES_Y; y != 0; y--) {
        for (x = RES_X; x != 0; x--) {
            if (*ptr_s) *ptr_t = *ptr_s;
            ptr_t++; ptr_s++;
        }
        ptr_t += LFB_Pitch - RES_X;
    }
    LFB_Unlock();
    LeaveTCS();
    SDL_UpdateRect(screen, 0, 0, RES_X, RES_Y);
}




/////////DRAWMAPBUFP

void DrawMapBuf()
{
    register byte *src, *vid;
    register int y;

    if (SomeoneDrawing()) return;
    src = ((byte*)FullBuf) + VIEW_OFS_X + VIEW_OFS_Y * VIEW_PIXSZ_X;
    EnterTCS();
    vid = (byte*) LFB_Lock(); vid += VIEW_X_POS + LFB_Pitch * VIEW_Y_POS;
    for (y = VIEW_SY; y != 0; y--) {
        memcpy(vid, src, VIEW_SX);
        vid += LFB_Pitch, src += VIEW_PIXSZ_X;
    }
    LFB_Unlock();
    LeaveTCS();
    SDL_UpdateRect(screen, VIEW_X_POS, VIEW_Y_POS, VIEW_SX, VIEW_SY);
}



void DrawMapBufP(int x, int y, int sx, int sy)
{
    register byte *src, *vid;
    register int ay;

    if (SomeoneDrawing()) return;
    x -= VIEW_OFS_X, y -= VIEW_OFS_Y;
    src = ((byte*)FullBuf) + (VIEW_OFS_X + x) + (VIEW_OFS_Y + y) * VIEW_PIXSZ_X;
    EnterTCS();
    vid = (byte*) LFB_Lock(); vid += (VIEW_X_POS + x) + LFB_Pitch * (VIEW_Y_POS + y);
    for (ay = sy; ay != 0; ay--) {
        memcpy(vid, src, sx);
        vid += LFB_Pitch, src += VIEW_PIXSZ_X;
    }
    LFB_Unlock();
    LeaveTCS();
    SDL_UpdateRect(screen, VIEW_X_POS+x, VIEW_Y_POS+y, sx, sy);
}



void DrawLitMap()
{
    register byte *src, *vid;
    register int ay;

    if (SomeoneDrawing()) return;
    src = ((byte*)LitMapTrans) + LitMapOfs.x + LitMapOfs.y * LMapSizeX;
    EnterTCS();
    vid = (byte*) LFB_Lock(); vid += LITMAP_X + LFB_Pitch * LITMAP_Y;
    for (ay = LITMAP_SIZE; ay != 0; ay--) {
        memcpy(vid, src, LITMAP_SIZE);
        vid += LFB_Pitch, src += LMapSizeX;
    }
    LFB_Unlock();
    LeaveTCS();
    SDL_UpdateRect(screen, LITMAP_X, LITMAP_Y, LITMAP_SIZE, LITMAP_SIZE);
}


///////// Putbitmap:

void PutBitmap(int x, int y, const void *data, int w, int h)
{
    register int ay;
    register const byte *aptr = (const byte*) data;
    register byte *vid;

    if (SomeoneDrawing()) return;
    EnterTCS();
    MouseFreeze(x, y, w, h);
    vid = (byte*)LFB_Lock(); vid += x + LFB_Pitch * y;
    for (ay = h; ay != 0; ay--) {
        memcpy(vid, aptr, w);
        aptr += w, vid += LFB_Pitch;
    }
    LFB_Unlock();
    MouseUnfreeze();
    SDL_UpdateRect(screen, x, y, w, h);
    LeaveTCS();
}


void PutBitmapNZ(int x, int y, void *data, int w, int h)
{
    register int ay, ax;
    register byte *aptr = (byte*) data;
    register byte *vid;

    if (SomeoneDrawing()) return;
    EnterTCS();
    MouseFreeze(x, y, w, h);
    vid = (byte*)LFB_Lock(); vid += x + LFB_Pitch * y;
    for (ay = h; ay != 0; ay--) {
        for (ax = w; ax != 0; ax--, vid++, aptr++)
            if (*aptr) *vid = *aptr;
        vid += LFB_Pitch-w;
    }
    LFB_Unlock();
    MouseUnfreeze();
    SDL_UpdateRect(screen, x, y, w, h);
    LeaveTCS();
}



void GetBitmap(int x, int y, void *data, int w, int h)
{
    register int ay;
    register byte *aptr = (byte*) data;
    register byte *vid;

    if (SomeoneDrawing()) return;
    EnterTCS();
    MouseFreeze(x, y, w, h);
    vid = (byte*) LFB_Lock();
    vid += x + LFB_Pitch * y;
    for (ay = h; ay != 0; ay--) {
        memcpy(aptr, vid, w);
        aptr += w;
        vid += LFB_Pitch;
    }
    LFB_Unlock();
    MouseUnfreeze();
    LeaveTCS();
}




///////// palette:

void SetPalette(char *paldat)
{
    SetPalettePart(paldat, 0, 256);
}



void SetPalettePart(char *paldat, int palofs, int palsize)
{
    int i;
    char *px = paldat;

    for (i = 0; i < palsize; i++) {
        palette[palofs+i].r = (*(px++)) << 2;
        palette[palofs+i].g = (*(px++)) << 2;
        palette[palofs+i].b = (*(px++)) << 2;
    }
    SDL_SetColors(screen, palette+palofs, palofs, palsize);
}

void SetRawPalette(const uint8_t *pal) {
	int i;

	for (i = 0; i < 256; i++) {
		palette[i].r = *pal++;
		palette[i].g = *pal++;
		palette[i].b = *pal++;
	}

	SDL_SetColors(screen, palette, 0, 256);
}



//////// mouse cursor:

void DrawCursor(void *src, int x, int y, int sx, int sy, int fromx, int fromy)
{
    register byte *vid, *s;
    register int ay, ax;

    if (SomeoneDrawing()) return;
    LFB_Lock();
    vid = ((byte*)VideoLFB) + x + y * LFB_Pitch;
    s = ((byte*)src) + fromx + 32 * fromy;
    for (ay = sy; ay != 0; ay--, vid += (LFB_Pitch - sx), s += (32-sx)) {
        for (ax = sx; ax != 0; ax--, vid++, s++)
            if (*s) *vid = *s;
    }
    LFB_Unlock();
    SDL_UpdateRect(screen, x, y, sx, sy);
}



void GetCurBack(void *src, int x, int y, int sx, int sy, int fromx, int fromy)
{
    register byte *vid, *s;
    register int ay;

    if (SomeoneDrawing()) return;
    LFB_Lock();
    vid = ((byte*)VideoLFB) + x + y * LFB_Pitch;
    s = ((byte*)src);// + fromx + 32 * fromy;
    for (ay = sy; ay != 0; ay--) {
        memcpy(s, vid, sx);
        vid += LFB_Pitch, s += 32;
    }
    LFB_Unlock();
}



void PutCurBack(void *src, int x, int y, int sx, int sy, int fromx, int fromy)
{
    register byte *vid, *s;
    register int ay;

    if (SomeoneDrawing()) return;
    LFB_Lock();
    vid = ((byte*)VideoLFB) + x + y * LFB_Pitch;
    s = ((byte*)src);// + fromx + 32 * fromy;
    for (ay = sy; ay != 0; ay--) {
        memcpy(vid, s, sx);
        vid += LFB_Pitch, s += 32;
    }
    LFB_Unlock();
    SDL_UpdateRect(screen, x, y, sx, sy);
}

void DrawVideoFrame(const uint8_t *frame, unsigned width, unsigned height) {
	uint8_t *screenbuf, *dst;
	const uint8_t *src;
	unsigned x, y, pitch = width;
	int basex, basey;

	if (SomeoneDrawing()) {
		return;
	}

	EnterTCS();
	screenbuf = (uint8_t*)LFB_Lock();
	basex = (RES_X - 2 * (int)width) / 2;
	basey = (RES_Y - 2 * (int)height) / 2;
	width = (RES_X < 2 * width) ? RES_X : 2 * width;
	height = (RES_Y < 2 * height) ? RES_Y : 2 * height;

	if (basex < 0) {
		frame -= basex >> 1;
		basex = 0;
	}

	if (basey < 0) {
		frame -= pitch * (basey >> 1);
		basey = 0;
	}

	for (y = 0; y < height; y++) {
		src = frame + pitch * (y >> 1);
		dst = screenbuf + LFB_Pitch * (basey + y);

		for (x = 0; x < width; x++) {
			dst[basex + x] = src[x >> 1];
		}
	}

	LFB_Unlock();
	LeaveTCS();
	SDL_UpdateRect(screen, basex, basey, width, height);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////// BITMAPs /////////////////////////////////

#include "events.h"
#include "global.h"
#include "fonts.h"







// KoPirovani mezi bitmapami - to ani nema cenu psat v ASM, stejne se to
// skoro nepouziva...

void CopyBmp(void *tar, int tarwidth, int x, int y, const void *src, int w, int h)
{
    int i, j;
    byte *tp = ((byte *)tar) + x + y * tarwidth;
    const byte *sp = (const byte *)src;
    int dif = tarwidth - w;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) *(tp++) = *(sp++);
        tp += dif;
    }
}



void CopyBmpNZ(void *tar, int tarwidth, int x, int y, void *src, int w, int h)
{
    int i, j;
    byte *tp = ((byte *)tar) + x + y * tarwidth;
    byte *sp = (byte *)src;
    int dif = tarwidth - w;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++, tp++, sp++) if (*sp != 0) *tp = *sp;
        tp += dif;
    }
}



void CopyFromBmp(void *src, int srcwidth, int x, int y, void *tar, int w, int h)
{
    int i, j;
    byte *sp = ((byte *)src) + x + y * srcwidth;
    byte *tp = (byte *)tar;
    int dif = srcwidth - w;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) *(tp++) = *(sp++);
        sp += dif;
    }
}





void BarBmp(void *tar, int tarwidth, int x, int y, int w, int h, byte clr)
{
    int ax, ay;
    byte *tp = (byte*) tar + (x + y * tarwidth);
    int dif = tarwidth - w;

    for (ay = 0; ay < h; ay++) {
        for (ax = 0; ax < w; ax++) *(tp++) = clr;
        tp += dif;
    }
}



void BarBmpSwap(void *tar, int tarwidth, int x, int y, int w, int h, byte clr1, byte clr2)
{
    int ax, ay;
    byte *tp = (byte*) tar + (x + y * tarwidth);
    byte b;
    int dif = tarwidth - w;

    for (ay = 0; ay < h; ay++) {
        for (ax = 0; ax < w; ax++) {
            b = *tp;
            if (b == clr1) b = clr2;
            else if (b == clr2) b = clr1;
            else if (b == clr2 - 5) b = clr1 - 5;
            *(tp++) = b;
        }
        tp += dif;
    }
}



void RectBmp(void *tar, int tarwidth, int x, int y, int w, int h, byte clr1, byte clr2)
{
    int i;
    byte *bf = ((byte*)tar) + y * tarwidth + x;

    for (i = 0; i < w; i++) bf[i] = clr1;
    for (i = 0; i < h; i++) bf[i * tarwidth] = clr1;
    for (i = 1; i < w; i++) bf[i + tarwidth * (h-1)] = clr2;
    for (i = 1; i < h; i++) bf[(w-1) + tarwidth * i] = clr2;
}



void PercentBar(void *tar, int width, int height, int x, int y, int w, int h, byte clr1, byte clr2, double value, const char *text) {
	int part = (int)((double)w * value);

	BarBmp(tar, width, x, y, w, h, clr2);
	BarBmp(tar, width, x+1, y+1, part-2, h-2, clr1);
	PutStr(tar, width, height, x+1 + (w - GetStrWidth(text, TinyFont))/2,
		y + (h - GetStrHeight(text, TinyFont))/2, text, TinyFont,
		clrWhite, clrBlack);
}









///////////////////// zatmivani a rozsveceni ----------

extern volatile int signus_suspended;

void FadeIn(char *paldat, int dlay)
{
    byte p2[768];
    int i, j;

    signus_suspended = TRUE;
    for (i = 0; i < 100; i+=2) {
        for (j = 0; j < 768; j++) p2[j] = paldat[j] * i / 100;
        SetPalette((char*)p2);
        SDL_Delay(1);
    }
    SetPalette(paldat);
    signus_suspended = FALSE;
}


void FadeOut(char *paldat, int dlay)
{
    byte p2[768];
    int i, j;

    signus_suspended = TRUE;
    for (i = 0; i < 100; i+=2) {
        for (j = 0; j < 768; j++) p2[j] = paldat[j] * (100-i) / 100;
        SetPalette((char*)p2);
        SDL_Delay(1);
    }
    memset(p2, 0, 768);
    SetPalette((char*)p2);
    signus_suspended = FALSE;
}




#define PRECISION      5
#define VALUES         (1<<PRECISION)
#define STEP           (1<<(8-PRECISION))


void paletizeSurface(byte *output, SDL_Surface *surf, const char *tableName)
{
    byte *table = (byte*) GraphicsDF->get(tableName);
    byte *Pout, *Pin;
    int x, y;
    int delta = surf->pitch - surf->w*3;
    byte r,g,b;
  
    for (Pout = output, Pin = (byte*)surf->pixels, y = surf->h; y > 0; 
         y--, Pin += delta)
    {
        for (x = surf->w; x > 0; x--)
        {
            r = *(Pin++)/STEP;
            g = *(Pin++)/STEP;
            b = *(Pin++)/STEP;
            *(Pout++) = table[r*VALUES*VALUES + g*VALUES + b];
        }
    }
    
    free(table);
}
