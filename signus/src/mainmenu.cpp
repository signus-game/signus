/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002
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



#include "headers.h"

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


extern int SwitchDisplayMode(int mode);


static void DrawMN(byte *p1, byte *p2, int sel)
{
//    PutBitmapNZ(0, 0, p1, 640, 150);
    PutBitmapNZ(0, 150, ((sel == 0) ? p2 : p1) + 640 * 150, 640, 74);
    PutBitmapNZ(0, 224, ((sel == 1) ? p2 : p1) + 640 * 224, 640, 80);
    PutBitmapNZ(0, 304, ((sel == 2) ? p2 : p1) + 640 * 304, 640, 76);
    PutBitmapNZ(0, 380, ((sel == 3) ? p2 : p1) + 640 * 380, 640, 100);
}



static int FlashPos = 320;
static int Flash1st = FALSE;

#if 0 // FIXME
static void DrawFlash(TMutationHeader *Mut, void *Buffer, int FromY, int ToY)
{
    if (Flash1st) { PlaySample(FlashSnd, 8, 32, 128); Flash1st = FALSE; }
    PutBitmap(0, FlashPos, Buffer, Mut->ResolutionX, Mut->ResolutionY);
}
#endif


static void Flash()
{
#if 0               // FIXME
    FILE *animf;
    TEvent e;

    VVF_DrawFrame = DrawFlash;
    animf = GetAnimFile("flash");
    do {GetEvent(&e);} while (e.What != evNothing);
    DoneTimer();
    PlayVVF(animf);
    InitTimer();
    do {GetEvent(&e);} while (e.What != evNothing);
    fclose(animf);
#endif
}


static int ProcessMenu(char *mask1, char *mask2)
{
    byte *pt1 = (byte*) GraphicsI18nDF->get(mask1);
    byte *pt2 = (byte*) GraphicsI18nDF->get(mask2);
    int oldsel, sel = 0;
    TEvent e;

    MouseSetRect(0, 0, 640-1, 480/2-1);
    MouseSetPos(0, 0);

    DrawMN(pt1, pt2, sel);
    while (TRUE) {
        GetEvent(&e);
        if (e.What == evMouseMove) {
            oldsel = sel;
            sel = e.Mouse.Where.y / 60;
            if (sel != oldsel) {
                PlaySample(MenuSnd, 8, 32, 128);
                DrawMN(pt1, pt2, sel);
            }
        }
        if (e.What == evMouseDown) {
            FlashPos = 130 + 80 * sel;
            Flash1st = TRUE;
            Flash();
            break;
        }
        if ((e.What == evKeyDown) && (e.Key.KeyCode == kbEsc)) {sel = 3; break;}
    }
    
    memfree(pt1);
    memfree(pt2);
    ClearScr();
    return sel;
}



extern char ActualDifficulty;

int DoMainMenu()
{
    int rtval = -1;
    char *pa = (char*) GraphicsDF->get("menupal");

    PlayMusic("solution.s3m");
    
    MouseHide();
    DoneInteract();
    ClearScr();
    SetPalette(pa);
    MenuSnd = LoadSample("menu", FALSE);
    FlashSnd = LoadSample("flash", FALSE);

    void *bg = GraphicsI18nDF->get("mmnulogo");
    PutBitmap(0, 0, bg, 1024, 140);
    memfree(bg);

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
    MouseShow();
    memfree(pa);
    return rtval;
}











//////// credits:


void ShowCredits()
{
    // FIXME --- !!! use C-Lone approach !!!
    char b[30];
    void *ptr;
    char *pal2, pal[768] = {0};
    int i, j;
    
    TDataFile *dscr = new TDataFile("screens.dat", dfOpenRead);
    TEvent e;

    MouseHide();
    DoneInteract();
    StopMusic();
    CloseChannels();
    ClearScr();

    e.What = evNothing;
    for (i = -1; i < 7; i++) {
        if (i == -1) sprintf(b, "cred_7");
        else sprintf(b, "cred_%i", i);            
        FadeOut(Palette, 0);
        MouseHide();
        pal2 = (char*) dscr->get("credpal");
        ptr = dscr->get(b);
        DrawPicture(ptr);
        FadeIn(pal2, 0);
        for (j = 0; j < 100; j++) {
            SDL_Delay(40);
            GetEvent(&e);
            if ((e.What == evKeyDown) || (e.What == evMouseDown)) break;
        }
        FadeOut(pal2, 0);
        memset(ptr, 0, RES_X * RES_Y);
        DrawPicture(ptr);
        MouseShow();
        memfree(ptr);
        memfree(pal2);
        if ((e.What == evKeyDown) || (e.What == evMouseDown)) break;
    }
    delete dscr;

    ClearScr();
    SetPalette(Palette);
    OpenChannels(MUSIC_CHANNELS, EFFECT_CHANNELS);
    InitInteract();
    MouseShow();
}

