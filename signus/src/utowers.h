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
// Hlavickovy soubor pro VEZE
//


#ifndef _UTOWERS_H
#define _UTOWERS_H

#include "units.h"


class TTower : public TUnit {
		public:
			TTower() : TUnit() {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveGround; *terr2 = TabTerr2MoveGround;};
			void GetUnitInfo(bool alt_wpinfo = false);
			unsigned GetSupportedActions();
			void CreateRangeHelpers();
			void ShowShootAt(int x, int y, int phase);
			int TimeToMove(int x, int y) {if ((x == X) && (y == Y)) return 0; else return -1;};
			void FieldSelected(int x, int y);
			int GetWeight() {return WGT_ULTRAHEAVY;};
	};
	





// Raketova vez Thor:

class TThor : public TTower {
		public:
			int IsOverground;
			int TotalRockets;
		
			TThor() : TTower() {};
			void Init(int x, int y, int party, ReadStream *stream = NULL);
			int GetType() {return unThor;};
			void Setup();
			void IncLevel(int alevel);
			void GetUnitInfo(bool alt_wpinfo = false);
			unsigned GetSupportedActions();
			void GoOverground(int over);
				// vysune/zasune vez
			int InfoEvent(TEvent *e);
			void Read(ReadStream &stream);
			void Write(WriteStream &stream);
			void ShowShootAt(int x, int y, int phase);
	};




// Kanonova vez Pagoda:

class TPagoda : public TTower {
		public:
			TPagoda() : TTower() {};
			int GetType() {return unPagoda;};
			void Setup();
			void IncLevel(int alevel);
	};
		
	


// Dalekonosny kanon Minotaurus:

class TMinotaurus : public TTower {
		public:
			TMinotaurus() : TTower() {};
			int GetType() {return unMinotaurus;};
			void Setup();
			void IncLevel(int alevel);
	};
		


// Protiletadlova vez:

class THorizont : public TTower {
		public:
			THorizont() : TTower() {};
			int GetType() {return unHorizont;};
			void Setup();
			void IncLevel(int alevel);
			unsigned GetSupportedActions();
	};




// laserova vez:

class TSpektrum : public TTower {
		public:
			TSpektrum() : TTower() {};
			int GetType() {return unSpektrum;};
			void Setup();
			void IncLevel(int alevel);
	};
	


#endif


