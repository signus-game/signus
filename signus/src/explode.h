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
// Hlavickovy soubor pro exploze
// Obsah: - fce Explode...(x,y)
//


#ifndef _EXPLODE_H
#define _EXPLODE_H


#include "global.h"
#include "engine.h"
#include "sound.h"



/////////////// ini-fce:

extern int InitExplode();
extern int DoneExplode();


////////////// vlastni vykonne fce:

extern void AddExplode1x1(int x, int y, int typ = 0, int xofs = 0, int yofs = 0);
		// prida explozi malou
extern void AddExplode3x3(int x, int y, int typ = 0, int xofs = 0, int yofs = 0, int mega = FALSE);
		// prida explozi velkou
		
extern void IncExplodeTime(int delta);
		// posune pozici, na kterou se zapise dalsi vybuch

extern void DoExplosion();
		// zobrazi exploze

extern int ProcessingExplosion;
		// priznak probihajici exploze
extern void UpdateExplodes();
		// behem kresleni anim. mapy se vola toto, aby se nesitil obraz

#endif
