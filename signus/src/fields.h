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

FIELDS : specialni osetreni zvlastnich policek (barely, ohne, kratery...)

*/



#ifndef _FIELDS_H
#define _FIELDS_H


//////////////////////// konstanty jednotlivych zvlastnich L2 terenu:

#define terBarrel            262
#define terBarrels           263









/////////////////////// ruzne metody:


// priznaky typu krateru
#define cratNothing  0
#define cratCrater   1
#define cratCrash    2

extern int MegaDestruction;
		// priznak niceni vseho (i hradeb)

extern void MakeCrater(int x, int y, int crater_type);
		// vytvori na danem policku krater po vybuchu

extern void ProcessFieldAnim(int x, int y);
		// animuje to policko

extern void FieldTurnEnd(int x, int y);
		// provede zmeny na konci kola

extern int AttackField(int x, int y, int WpnType, int AtN, int BoN);
		// utoci na teren policka


extern void PlaceL2(int x, int y, int terrain2, int anim = 0);
		// na danem policku vytvori L2 teren...



/////////////////////// minova pole:

typedef struct {
			int x, y;      // pozice
			int party;     // bad, good
	} TMineRecord;
	
#define MAX_MINES_CNT      512
extern TMineRecord MinesList[MAX_MINES_CNT];
extern int MinesCnt;

extern int flagUnitOnMine;
		// tato vlajka se nastavi pri pohybu, pokud jednotka najede na minu
		// (tj. behem MoveRel().) V Move() se na to reaguje volanim
		// metody GoOnMine()

extern void MinePlace(int x, int y, int party/*GOODLIFE,BADLIFE*/);
		// umisti minu

extern void MineRemove(int x, int y);
		// odstrani minu

extern void MineExplode(int x, int y);
		// vybouchne minu

extern int GetMineAt(int x, int y);
		// zjisti, jestli tam je mina a vrati jaji prislusnost
		// jinak vraci -1

extern int MineIsSeen(int x, int y, int by_which_side);
		// zjisti, jestli je dana mina videt Xenonem

// Check for visible mines on given field
int field_has_visible_mine(int x, int y, int side);

#endif
