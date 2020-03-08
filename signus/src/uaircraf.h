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

Letadla - osetreni zakonitosti + vlastni objekty


*/

#ifndef _UAIRCRAF_H
#define _UAIRCRAF_H


#include "global.h"
#include "engine.h"
#include "units.h"




// predek vsech letadel:

class TAircraft : public TUnit {
		public:
			int FlyLevel; 
					// uroven, v niz se leti (1 - nad terenem, 2 - vysoko)
			int LittleAlt;
					// pro animace sestupu a startu apod.
		
			TAircraft() : TUnit() {};
			void Init(int x, int y, int party, ReadStream *stream);
			void GetTerrMove(int **terr, int **terr2);
			void TurnReset();
			void PlaceGround(int place);
			int DoVisib();
			void Read(ReadStream &stream);
			void Write(WriteStream &stream);
			void Draw();
			int PaintUnitInMove(int bx, int by);
			void GetDrawRect(TRect *r);
			void PrepareFieldTime(int x, int y); //kod v units.cpp!!!
			int MoveRel(int rx, int ry);
			unsigned GetSupportedActions();
			unsigned GetAvailableActions();
			void GetCursor(int x, int y, int *cursor, int *selbold);
			void Action(int x, int y);
			void ShowShootAt(int x, int y, int phase);
			unsigned GetFieldActions(int x, int y);
			void Explode();
			int CanGoOnField(int x, int y);
			
			virtual TSprite *GetShadowSprite();
	};
	
	
	
	

// glob. prom:

extern TAircraft *Aircrafts[256];
		// seznam leticich letadel
extern int AircraftsCnt;
		// pocty letadel v tom seznamu (celkem, goodlife, badlife)

extern TAircraft *GetAircraftAt(int x, int y);
		// vraci letadlo nad tim polickem nebo NULL, kdyz je tam pusto
	




///// potomci: jednotliva letadla:



// AT stihac Mystik:

class TMystik : public TAircraft {
		public:
			TMystik() : TAircraft() {};
			int GetType() {return unMystik;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
			unsigned GetSupportedActions();
	};




// vrtulnik Rex:

class TRex : public TAircraft {
		public:
			int APhase;
		
			TRex() : TAircraft() {};
			void Init(int x, int y, int party, ReadStream *stream);
			int GetType() {return unRex;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_LIGHT;};
			int IsAnimated() {return (FlyLevel != 0);}
			void DoAnim();
			TSprite *GetSprite();
			TSprite *GetShadowSprite();
			unsigned GetSupportedActions();
			unsigned GetAvailableActions();
			void GetUnitInfo();
			int InfoEvent(TEvent *e);
			
			int Land();
					// pristane na zemi; vraci uspech (muze jen na A-ter)
			int CanLand(int x, int y);
					// vraci jestli lze pristat na tom poli
			int TakeOff();
					// vzletne
	};




// transporter Caesar:

class TCaesar : public TAircraft {
		public:
			int Inventory[6];
					// seznam nalozenych jednotek - idecka
			int LoadedUnits;
					// pocet vlozenych jednotek 
			int Capacity;
					// celkova hmotnost pojmutelnych jednotek

			TCaesar() : TAircraft() {Capacity = 6;};
			void Init(int x, int y, int party, ReadStream *stream);
			int GetType() {return unCaesar;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM + GetTotalWeight();};
			unsigned GetSupportedActions();
			unsigned GetAvailableActions();
			void Read(ReadStream &stream);
			void Write(WriteStream &stream);
			void RemoveFromWorld();
			void GetUnitInfo();
			int InfoEvent(TEvent *e);
			void Action(int x, int y);
			void ChangeParty();
			
			int Land();
					// pristane na zemi; vraci uspech (muze jen na A-ter)
			int CanLand(int x, int y);
					// vraci jestli lze pristat na tom poli
			int TakeOff();
					// vzletne
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
	};




// stihac Ikaros:

class TIkaros : public TAircraft {
		public:
			TIkaros() : TAircraft() {};
			int GetType() {return unIkaros;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_LIGHT;};
			unsigned GetSupportedActions();
	};




// light-bomber Kobra:
class TKobra : public TAircraft {
		public:
			TKobra() : TAircraft() {};
			int GetType() {return unKobra;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
			unsigned GetSupportedActions();
			void Select();
			int Attack(int x, int y);
	};



// supertezky bombarder Saturn:

class TSaturn : public TAircraft {
		public:
			int IsBombing;
			int Bombs, BombAN;
		
			TSaturn() : TAircraft() {};
			int GetType() {return unSaturn;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
			unsigned GetSupportedActions();
			void GoOnField();
			int MoveSUB(int x, int y);
			int MoveFarSUB(int x, int y);
			void GetUnitInfo();
			int InfoEvent(TEvent *e);
			void Read(ReadStream &stream);
			void Write(WriteStream &stream);

			int StartBombing();
			int EndBombing();
					// zahaji/skonci bombardovani - vraci FALSE/TRUE (
					// podle toho jestli jsou bomby anebo cas)
	};










// doplnovac paliva:

class TProton : public TAircraft {
		public:
			TProton() : TAircraft() {};
			int GetType() {return unProton;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
			unsigned GetSupportedActions();
			void GetCursor(int x, int y, int *cursor, int *selbold);
			void Action(int x, int y);

			virtual void DoSupport(int x, int y);
				// provede zasobovani/opravu jednotky na policku x,y
			virtual void SupportUnit(TUnit *Unit);
				// vlastni operace "nalozeni zasob"
			virtual int WillSupport(TUnit *Unit);
				// vraci, jestli ma smysl zasobovat tu jednotku
	};


#endif
