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

Vlaky a vlacky a masinky a ...

*/

#ifndef _UTRAINS_H
#define _UTRAINS_H


#include "units.h"



class TToweredTrainUnit : public TToweredUnit {
		public:
			int SpriteOrient;

			TToweredTrainUnit() : TToweredUnit() {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveTrain; *terr2 = TabTerr2MoveTrain;};
			void MakeTrack() {};
			void Setup();
			void PrepareFieldTime(int x, int y);
			void Write(WriteStream &stream);
			void Read(ReadStream &stream);
			void Rotate(int angle);
			TSprite *GetSprite();
			void Draw();
			void GetDrawRect(TRect *r);
	};

class TTrainSupportUnit : public TSupportUnit {
		public:
			int SpriteOrient;

			TTrainSupportUnit() : TSupportUnit() {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveTrain; *terr2 = TabTerr2MoveTrain;};
			void MakeTrack() {};
			void Setup();
			void PrepareFieldTime(int x, int y);
			void Write(WriteStream &stream);
			void Read(ReadStream &stream);
			void Rotate(int angle);
			TSprite *GetSprite();
	};	








// Mohykan:

class TMohykan : public TToweredTrainUnit {
		public:
			TMohykan() : TToweredTrainUnit() {};
			int GetType() {return unMohykan;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
	};



// Mesias:

class TMesias : public TToweredTrainUnit {
		public:
			TMesias() : TToweredTrainUnit() {};
			int GetType() {return unMesias;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
	};


// Gamymedes - zasobovac:

class TGanymedes : public TTrainSupportUnit {
		public:
			int Ammo[4], MaxAmmo[4];
		
			TGanymedes() : TTrainSupportUnit() {};
			int GetType() {return unGanymedes;}
			void Setup();
			int GetWeight() {return WGT_HEAVY;};
			void AfterSetup();
			void Write(WriteStream &stream);
			void Read(ReadStream &stream);
			void SupportUnit(TUnit *Unit);
			int WillSupport(TUnit *Unit);
			void GetUnitInfo(bool alt_wpinfo = false);
	};


#endif
