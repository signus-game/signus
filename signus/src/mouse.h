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
// Hlavickovy soubor pro MOUSE.CPP
// Obsah: handler pro mys
//


#ifndef _MOUSE_H
#define _MOUSE_H

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Funkce pro spolupraci s mysi:
//

#define mcurCnt        11

#define mcurArrow       0
#define mcurWait        1
#define mcurSelect      2
#define mcurSelectBig   4
#define mcurSelectSmall 5
#define mcurTarget      3
#define mcurTargetBig   6
#define mcurTargetSmall 7
#define mcurSupport     8
#define mcurLoadIn      9
#define mcurBuild      10


// Struktura prenasenych dat (i vsech ostatnich):
typedef struct {
        int ActCur;
        
        // Pocet zamku na kresleni/na volani kresleni:
        int Locks, SuperLocks;
        
        // Aktualni stav:
        unsigned short code;
        unsigned short buttons;
        unsigned short x;
        unsigned short y;
} TMouseCBD;

extern TMouseCBD Mouse;

extern int MouseInit();
extern void MouseDone();
extern void MouseSetCursor(int cnum);
extern void MouseHide();
extern void MouseShow();
extern void MouseFreeze(int x, int y, int w, int h);
extern void MouseUnfreeze();
extern void MouseSetRect(int x1, int y1, int x2, int y2);
extern void MouseSetPos(int x, int y);

extern void MousePaint();
#endif
