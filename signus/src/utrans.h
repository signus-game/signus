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

Transportni jednotky - ve vsech prostredich 
(v jednom souboru jsou pro Poradek...)

*/


#ifndef _UTRANS_H
#define _UTRANS_H


#include "units.h"


// ruzne kapacity transporteru:
#define TCAPACITY_BIG      24
#define TCAPACITY_MEDIUM   12
#define TCAPACITY_SMALL     6
#define MAX_TCAPACITY      TCAPACITY_BIG


// Obecny predek trasporteru:

class TTransporter : public TUnit {
		public:
			int Inventory[MAX_TCAPACITY];
					// seznam nalozenych jednotek - idecka
			int LoadedUnits;
					// pocet vlozenych jednotek 
			int Capacity;
					// celkova hmotnost pojmutelnych jednotek

			TTransporter(int capac) : TUnit() {Capacity = capac;};
			void Init(int x, int y, int party, FILE *f);
			void Setup();
			unsigned GetAvailableActions();
			void Read(FILE *f);
			void Write(FILE *f);
			void RemoveFromWorld();
			int InfoEvent(TEvent *e);
			void Action(int x, int y);
			void GetUnitInfo();
			void ChangeParty();
			
			int LoadUnit(TUnit *u);
					// vezme dovnitr jednotku. return viz hlp
			int UnloadUnit(TUnit *u);
					// vylozi jednotku ven
			void DoInventory();
					// provadi operace nad obsahem zakladny - uziv. rozhrani pro glife
			int GetTotalWeight();
					// vraci celkovou hmotnost jednotek vevnitr
			virtual int GetLoadingPoint(int *x, int *y, TUnit *u);
					// vraci misto, odkud se muze jednotka u nalodit viz hlp
			virtual int WantToLoad(TUnit *u);
					// vraci, jestli chce/umi nalozit tuhle jednotku
	};





// Pozemni transportery:

class TGroundTransporter : public TTransporter {
		public:
			TGroundTransporter(int capac) : TTransporter(capac) {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveGround; *terr2 = TabTerr2MoveGround;};
			void MakeTrack();
	};



// Uran:

class TUran : public TGroundTransporter {
		public:
			TUran() : TGroundTransporter(TCAPACITY_MEDIUM) {};
			int GetType() {return unUran;}
			int GetWeight() {return WGT_ULTRAHEAVY + GetTotalWeight();}
			void Setup();
			void IncLevel(int alevel);
	};




// Vznasedlo-transporter:

class TNeptun : public TTransporter {
		public:
			TNeptun() : TTransporter(TCAPACITY_SMALL) {};
			int GetType() {return unNeptun;}
			int GetWeight() {return WGT_ULTRAHEAVY + GetTotalWeight();}
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveAmphi; *terr2 = TabTerr2MoveAmphi;};
			void MakeTrack() {};
			void Setup();
			void IncLevel(int alevel);
	};






// Vlakovy transporter:

class TOlymp : public TTransporter {
		public:
			int SpriteOrient;
		
			TOlymp() : TTransporter(TCAPACITY_MEDIUM) {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveTrain; *terr2 = TabTerr2MoveTrain;};
			void MakeTrack() {};
			int GetType() {return unOlymp;}
			int GetWeight() {return WGT_ULTRAHEAVY + GetTotalWeight();}
			void Setup();
			void IncLevel(int alevel);
			void PrepareFieldTime(int x, int y);
			void Write(FILE *f);
			void Read(FILE *f);
			void Rotate(int angle);
			TSprite *GetSprite();
	};




#endif
