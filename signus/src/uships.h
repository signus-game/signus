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

USHIPS - lode

*/


#ifndef _USHIPS_H
#define _USHIPS_H

#include "utrans.h"
#include "units.h"





extern void MakeWaterTrack(int X, int Y, int Orient);
		// vytvari stopu za lodi


class TShip : public TUnit {
		public:
			TShip() : TUnit() {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveWater; *terr2 = TabTerr2MoveWater;};
			void MakeTrack();
			void ShowShootAt(int x, int y, int phase);
			void WpnRotate(int x, int y) {};
	};


#ifndef CROSS_REFERENCE_TO_SHIPS

class TShipTransporter : public TTransporter {
		public:
			TShipTransporter(int capac) : TTransporter(capac) {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveWater; *terr2 = TabTerr2MoveWater;};
			void MakeTrack();
			void ShowShootAt(int x, int y, int phase);
			void WpnRotate(int x, int y) {};
	};

#endif



// male lodicky (no problemove):

class TFlint : public TShip {
		public:
			TFlint() : TShip() {};
			int GetType() {return unFlint;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
	};



class TViking : public TShip {
		public:
			TViking() : TShip() {};
			int GetType() {return unViking;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
			unsigned GetSupportedActions();
	};





class TBarracuda : public TShip {
		public:
			TBarracuda() : TShip() {};
			int GetType() {return unBarracuda;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_LIGHT;};
			void Draw();
			unsigned GetSupportedActions();
			unsigned GetAvailableActions();

			int IsVisib();
					// vraci, jestli ji vidi torpedak
	};


extern void RemoveSubmarines();
extern void PlaceSubmarines(int all = FALSE);
		// osetreni neviditelnosti ponorek




///////////// lode-kravy:


// kriznik:
class TPoseidon : public TShip {
		public:
			TPoseidon() : TShip() {};
			int GetType() {return unPoseidon;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
			void PrepareFieldTime(int x, int y);
			void PlaceGround(int place);
			void Rotate(int angle);
			void RotateRel(int ra);
			unsigned GetSupportedActions();
	};




// bitevnik:
class TKraken : public TShip {
		public:
			TKraken() : TShip() {};
			int GetType() {return unKraken;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
			void PrepareFieldTime(int x, int y);
			void PlaceGround(int place);
			void Rotate(int angle);
			void RotateRel(int ra);
			unsigned GetSupportedActions();
	};




#ifndef CROSS_REFERENCE_TO_SHIPS

// letadlovka:
class TLaguna : public TShipTransporter {
		public:
			TLaguna() : TShipTransporter(TCAPACITY_MEDIUM) {};
			int GetType() {return unLaguna;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_ULTRAHEAVY + GetTotalWeight();}
			void PrepareFieldTime(int x, int y);
			void PlaceGround(int place);
			void Rotate(int angle);
			void RotateRel(int ra);
			int LoadUnit(TUnit *u);
			unsigned GetAvailableActions();
			void TurnReset();
	};













// transporter:
class TOasa : public TShipTransporter {
		public:
			int Ammo[4], MaxAmmo[4];

			TOasa() : TShipTransporter(TCAPACITY_MEDIUM) {};
			int GetType() {return unOasa;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_ULTRAHEAVY + GetTotalWeight();}
			void PlaceGround(int place);
	};


#endif






#endif
