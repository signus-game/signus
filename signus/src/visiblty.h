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


// 
// Hlavickovy soubor pro modul vypoctu viditelnosti
//


#ifndef _VISIBLTY_H
#define _VISIBLTY_H


#include "global.h"
#include "engine.h"
#include "etables.h"




///////// FCe volane z TObject::DoVisib() podle typu videni:


extern int ProcessRadarVisib(int X, int Y, int Visib/*range*/, int ID);
		// "radarova" viditelnost - bez ohledu na prekazky vidi vsechno
extern int ProcessEyeVisib(int X, int Y, int Visib/*range*/, int ID);
		// bere ohled na teren a prekazky



extern int VisibEnemies;
    // pocet zjistenych nepratel behem vykreslovani
    

///////// Reseni viditelnosti Goodlife a Badlife:

extern int AllowedBadlife;
		// kdyz je to aktivni, generuje se visibmap i pro Badlife, jinak nic
		
extern byte *BadlifeVisib;

extern void AllowBadlife(int enable);
		// Zapne/vypne gener. pro Badlife, vola se v AI na zac. a konci		
		// alokuje potrebne promnene
		
extern byte GetBadlifeVisib(int x, int y);
		// Zjisti, zda je na donem policku b-visib. Vraci 0,1,2 (2-full,0-none)
extern void SetBadlifeVisib(int x, int y, byte value);

extern byte GetGoodlifeVisib(int x, int y);
extern void SetGoodlifeVisib(int x, int y, byte value);


extern void ComputeVisib();
		// Spocita viditelnost vsech jednotek. Goodlife se ulozi do popisek
		// TField.Visib, Badlife do zvlastniho pole, pristupneho pomoci




#endif
