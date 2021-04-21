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


/*

Vyber v hlavnim menu hry

*/



#include <SDL_timer.h>

#include "mainmenu.h"
#include "global.h"
#include "graphio.h"
#include "sound.h"
#include "events.h"
#include "ui_toolkit.h"
#include "engtimer.h"
#include "anims.h"
#include "mouse.h"



static MIDASsample MenuSnd, FlashSnd;


static void DrawMN(byte *p1, byte *p2, void *bg, int sel)
{
    PutBitmap(0, 0, bg, 800, 140);
    PutBitmapNZ(80, 100+0, p1, 640, 150);
    PutBitmap(80, 250, ((sel == 0) ? p2 : p1) + 640 * 150, 640, 74);
    PutBitmap(80, 324, ((sel == 1) ? p2 : p1) + 640 * 224, 640, 80);
    PutBitmap(80, 404, ((sel == 2) ? p2 : p1) + 640 * 304, 640, 76);
    PutBitmap(80, 480, ((sel == 3) ? p2 : p1) + 640 * 380, 640, 100);
}



static void Flash(unsigned x, unsigned y) {
	File animfile;
	unsigned width, height, timer, curtime;
	TEvent e;

	// FIXME: The menu now uses different palette than the flash animation
	return;

	if (!open_anim_file(animfile, "flash")) {
		return;
	}

	DoneTimer();
	VVFStream anim(&animfile);
	width = anim.video_width();
	height = anim.video_height();
	PlaySample(FlashSnd, 8, 32, 128);
	timer = SDL_GetTicks();

	while (anim.next_frame()) {
		PutBitmap(x, y, anim.videobuf(), width, height);
		timer += anim.frame_time();
		curtime = SDL_GetTicks();

		if (timer > curtime) {
			SDL_Delay(timer - curtime);
		}
	}

	do {
		GetEvent(&e);
	} while (e.What != evNothing);

	InitTimer();
}


static int ProcessMenu(const char *mask1, const char *mask2)
{
    void *bg = GraphicsDF->get("mmnulogo");
    byte *pt1 = (byte*) GraphicsI18nDF->get(mask1);
    byte *pt2 = (byte*) GraphicsI18nDF->get(mask2);
    int oldsel, sel = 0;
    TEvent e;


    DrawMN(pt1, pt2, bg, sel);
    while (TRUE) {
        GetEvent(&e);
        if (e.What == evMouseMove) {
            oldsel = sel;
            sel = (e.Mouse.Where.y - 290) / 60;
            if (sel < 0) sel = 0;
            if (sel > 3) sel = 3;
            if (sel != oldsel) {
                PlaySample(MenuSnd, 8, 32, 128);
                DrawMN(pt1, pt2, bg, sel);
            }
        }
        if (e.What == evMouseDown) {
            Flash(80, 250 + 77 * sel);
            break;
        }
        if ((e.What == evKeyDown) && (e.Key.KeyCode == kbEsc)) {sel = 3; break;}
    }
    
    memfree(pt1);
    memfree(pt2);
    memfree(bg);
    ClearScr();
    return sel;
}



extern char ActualDifficulty;

int DoMainMenu()
{
    int rtval = -1;
    //char *pa = (char*) GraphicsDF->get("menupal");
    uint8_t *pa = (uint8_t*) GraphicsDF->get("palette");

    PlayMusic("solution.s3m");
    
    DoneInteract();
    ClearScr();
    SetPalette(pa);
    MenuSnd = LoadSample("menu", FALSE);
    FlashSnd = LoadSample("flash", FALSE);

    while (rtval == -1) {
        rtval = ProcessMenu("mmnu0", "mmnu1");
        if (rtval == 0) /*new game*/ {
            rtval = ProcessMenu("mmnu2", "mmnu3"); // dificulty
            if (rtval == 3) {rtval = -1; continue;}
            switch (rtval) {
                case 0 : ActualDifficulty = 'e'; break;
                case 1 : ActualDifficulty = 'n'; break;
                case 2 : ActualDifficulty = 'h'; break;                
            }
            rtval = 0;
            break;
        }
    }

    FreeSample(MenuSnd);
    FreeSample(FlashSnd);
    ClearScr();
    SetPalette(Palette);
    InitInteract();
    memfree(pa);
    return rtval;
}











//////// credits:


void ShowCredits() {
	char *buf;
	char line[200];
	void *ptr, *ptr2;
	char *c;
	int i, curln, lastchar;
	int oldvol = MusicVolume;

	TEvent e;

	SetVolume(EffectsVolume, SpeechVolume, 64);
	MouseHide();
	ClearScr();
	PlayMusic("solution.s3m");

	do {
		GetEvent(&e);
	} while (e.What != evNothing);

	ptr = memalloc(RES_X * RES_Y);
	memset(ptr, 0, 800*600);
	ptr2 = GraphicsDF->get("mmnulogo");
	memcpy(ptr, ptr2, 800 * 140);
	memfree(ptr2);

	buf = (char*) TextsDF->get("credits");
	ptr2 = memalloc(RES_X * RES_Y);
	memcpy(ptr2, ptr, RES_X * RES_Y);
	DrawPicture(ptr);

	for (curln = 520; TRUE; curln -= 1) {
		i = 0;
		line[0] = 0;
		lastchar = 0;

		for (c = buf; *c != 0; c++) {
			if (*c != '\n') {
				line[lastchar++] = *c;
				continue;
			}

			line[lastchar] = 0;

			if (strcmp(line, "END") == 0) {
				break;
			}

			i++;

			if (strlen(line) == 0) {
				continue;
			}

			if ((curln + 20 * i > 60) && (curln + 20 * i < 540)) {
				PutStr(ptr, RES_X, RES_Y,
					400 - GetStrWidth(line+1, NormalFont)/2,
					curln + 20 * i, line+1, NormalFont,
					(line[0] == '*') ? 9/*red*/:2/*white*/,
					0);
			}

			line[0] = 0; lastchar = 0;
		}

		PutBitmap(0, 220, ((char*)ptr) + 220 * 800, 800, 300);
		memcpy(ptr, ptr2, 800*600);
		SDL_Delay(20);

		GetEvent(&e);

		if ((e.What == evKeyDown) || (e.What == evMouseDown)) {
			break;
		}
	}

	memfree(ptr);
	memfree(ptr2);
	memfree(buf);
	MouseShow();
	ClearScr();
	SetVolume(EffectsVolume, SpeechVolume, oldvol);
}
