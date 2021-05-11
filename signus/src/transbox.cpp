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


/*

TRASPORT_BOX     - panel, ktery zobrazuje obsah trasporteru

*/

#include <SDL_timer.h>

#include "transbox.h"
#include "units.h"
#include "mouse.h"

#define OPEN_STEP   12


TTransBox::TTransBox(int aw, int ah, int *aunits, int numun, byte *atempl)
{
	w = aw, h = ah, lunits = aunits, Templ = atempl;
	x = RES_X-119 - w + 1, y = RES_Y-23 - h + 1;
	if (iniResolution > SVGA_640x480) y -= 8;
	Cnt = numun;
}



void TTransBox::Show(int fast)
{
	Buf = (byte*) memalloc(w * h);
	Bkg = (byte*) memalloc(w * h);
	lockmem(Buf, w * h); lockmem(Bkg, w * h);
	GetBitmap32(x, y, Bkg, w, h);

	MouseHide();
	if (iniEnhancedGuiOn && (!fast))
		for (int i = w; i > 0; i -= w/OPEN_STEP) {Draw(i); SDL_Delay(15);}
	memcpy(Buf, Templ, w * h);
	Draw(0);
	MouseShow();
}



void TTransBox::Draw(int phase)
{
	if (phase != 0) {
		for (int i = 0; i < h; i++) {
			memcpy(Buf + i * w, Bkg + i * w, phase);
			memcpy(Buf + i * w + phase, Templ + i * w, w - phase);
		}
	}
	PutBitmap32(x, y, Buf, w, h);
	UpdateScreen();
}



void TTransBox::Hide(int fast)
{
	MouseHide();
	if (iniEnhancedGuiOn && (!fast))
		for (int i = 0; i < w; i += w/OPEN_STEP) {Draw(i); SDL_Delay(15);}
	PutBitmap32(x, y, Bkg, w, h);
	MouseShow();
	UpdateScreen();
	unlockmem(Buf, w * h); unlockmem(Bkg, w * h);
	memfree(Bkg);
	memfree(Buf);
}



void TTransBox::DrawUnits() {
	int i, ux, uy, clr;

	for (i = 0; i < Cnt; i++) {
		ux = 8 + 57 * (i % (w / 57));
		uy = 8 + 57 * (i / (w / 57));
		CopyBmp(Buf, w, ux, uy, UnitsTransIcons[Units[lunits[i]]->Type],
			56, 56);

		if (Units[lunits[i]]->HitPoints < 0.3 * Units[lunits[i]]->MaxHitPoints) {
			clr = 10; /*red*/
		} else {
			clr = 59; /*light green*/
		}

		PercentBar(Buf, w, h, ux+1, uy+51, 54, 4, clr, 72,
			((double)Units[lunits[i]]->HitPoints)/Units[lunits[i]]->MaxHitPoints, "");
	}

	Draw(0);
}



int TTransBox::Handle() {
	TEvent e;
	int lastun = -1, un;

	DrawUnits();

	while (TRUE) {
		GetEvent(&e);

		if (e.What == evMouseDown) {
			if (e.Mouse.Buttons == mbRightButton) {
				return -1;
			}

			if (!IsInRect(e.Mouse.Where.x-x, e.Mouse.Where.y-y, 0, 0, w-1, h-1)) {
				return -1;
			}

			e.Mouse.Where.x -= x+8;
			e.Mouse.Where.y -= y+8;
			un = (e.Mouse.Where.x / 57) + (e.Mouse.Where.y / 57) * ((w-8) / 57);
			if ((un >= Cnt) || (un < 0)) {
				un = -1;
			}

			return un;
		} else if (e.What == evMouseMove) {
			e.Mouse.Where.x -= x+8;
			e.Mouse.Where.y -= y+8;
			un = (e.Mouse.Where.x / 57) + (e.Mouse.Where.y / 57) * (w-8) / 57;

			if (!IsInRect(e.Mouse.Where.x+8, e.Mouse.Where.y+8, 0, 0, w-1, h-1) || (un >= Cnt) || (un < 0)) {
				memcpy(Buf + 180*w, Templ+180*w, 18*w);
				PutBitmap32(x, y+180, Buf+180*w, w, 18);
				lastun = -1;
				continue;
			}

			if (un == lastun) {
				continue;
			}

			lastun = un;

			// vykresli info o jednotce:
			memcpy(Buf + 180*w, Templ+180*w, 18*w);
			PutStr(Buf, w, h, 30+8, 181,
				Units[lunits[un]]->GetName(), NormalFont,
				clrWhite, clrBlack);
			CopyBmpNZ(Buf, w, 1+8, 181,
				LevelBmps[((TUnit*)Units[lunits[un]])->Level],
				29, 16);
			PutBitmap32(x, y+180, Buf+180*w, w, 18);
		} else if (e.What == evKeyDown) {
			if (e.Key.KeyCode == kbEnter) {
				return lastun;
			} else {
				return -1;
			}
		}
	}
}

