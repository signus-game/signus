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
// Hlavickovy soubor pro POZEMNI jednotky
//


#ifndef _UGROUND_H
#define _UGROUND_H

#include "units.h"


extern void MakeGroundTrack(int X, int Y, int Orient);
		// vytvari stopu za pozemni jednotkou


class TGroundUnit : public TUnit {
		public:
			TGroundUnit() : TUnit() {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveGround; *terr2 = TabTerr2MoveGround;};
			void MakeTrack();
	};

class TGroundToweredUnit : public TToweredUnit {
		public:
			TGroundToweredUnit() : TToweredUnit() {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveGround; *terr2 = TabTerr2MoveGround;};
			void MakeTrack();
	};	

class TGroundSupportUnit : public TSupportUnit {
		public:
			TGroundSupportUnit() : TSupportUnit() {};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveGround; *terr2 = TabTerr2MoveGround;};
			void MakeTrack();
	};	




// Zakladni tankova rada:

class TPerseusAlfa : public TGroundUnit {
		public:
			TPerseusAlfa() : TGroundUnit() {};
			int GetType() {return unPerseusAlfa;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_LIGHT;};
	};
	
	
class TPerseusBeta : public TGroundToweredUnit {
		public:
			TPerseusBeta() : TGroundToweredUnit() {};
			int GetType() {return unPerseusBeta;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_LIGHT;};
	};
	
	
class TPerseusGama : public TGroundToweredUnit {
		public:
			TPerseusGama() : TGroundToweredUnit() {};
			int GetType() {return unPerseusGama;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
			void ShowShootAt(int x, int y, int phase);
			void WpnRotate(int x, int y);
			unsigned GetSupportedActions();
	};
	
	



// artilerie:

class TGolias : public TGroundUnit {
		public:
			TGolias() : TGroundUnit() {};
			int GetType() {return unGolias;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
	};


class TPatriot : public TGroundUnit {
		public:
			TPatriot() : TGroundUnit() {};
			int GetType() {return unPatriot;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
	};


class THerkules : public TGroundUnit {
		public:
			THerkules() : TGroundUnit() {};
			int GetType() {return unHerkules;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
	};

class TSkorpion : public TGroundToweredUnit {
		public:
			TSkorpion() : TGroundToweredUnit() {};
			int GetType() {return unSkorpion;}
			void Setup();
			void IncLevel(int alevel);
			void ShowShootAt(int x, int y, int phase);
			int GetWeight() {return WGT_MEDIUM;};
	};





// podpurne jednotky:

// zasobnik paliva:
class TCeres : public TGroundSupportUnit {
		public:
			TCeres() : TGroundSupportUnit() {};
			int GetType() {return unCeres;}
			void Setup();
			int GetWeight() {return WGT_HEAVY;};
			void SupportUnit(TUnit *Unit);
			int WillSupport(TUnit *Unit);
	};

// zasobnik streliva:
class TGnom : public TGroundSupportUnit {
		public:
			int Ammo[4], MaxAmmo[4];
		
			TGnom() : TGroundSupportUnit() {};
			int GetType() {return unGnom;}
			void Setup();
			int GetWeight() {return WGT_HEAVY;};
			void AfterSetup();
			void Write(WriteStream &stream);
			void Read(ReadStream &stream);
			void SupportUnit(TUnit *Unit);
			int WillSupport(TUnit *Unit);
			void GetUnitInfo(bool alt_wpinfo = false);
	};

// opravna-servisman:
class TIris : public TGroundSupportUnit {
		public:
			TIris() : TGroundSupportUnit() {};
			int GetType() {return unIris;}
			void Setup();
			int GetWeight() {return WGT_HEAVY;};
			void SupportUnit(TUnit *Unit);
			int WillSupport(TUnit *Unit);
	};






// specialni jednotky:


// pruzkumnik:
class TBumerang : public TGroundUnit {
		public:
			TBumerang() : TGroundUnit() {};
			int GetType() {return unBumerang;}
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_LIGHT;};
			void ShowShootAt(int x, int y, int phase);
			unsigned GetSupportedActions();
			void Action(int x, int y);
			void GetCursor(int x, int y, int *cursor, int *selbold);
			int OccupyBuilding(TObject *b);
					//obsadi budovu a zmeni jeji barvu na svoji
					// b MUSI byt TBuilding nebo potomek!!
	};


// mina:
class TSatan : public TGroundUnit {
		public:
			int DestruAtN1, DestruBoN1, DestruAtN2, DestruBoN2; // utok autodestrukci
		
			TSatan() : TGroundUnit() {};
			void Init(int x, int y, int party, ReadStream *stream = NULL);
			int GetType() {return unSatan;};
			void GetTerrMove(int **terr, int **terr2) {*terr = TabTerrMoveAmphi; *terr2 = TabTerr2MoveAmphi;};
			void MakeTrack() {};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_LIGHT;};
			void GetUnitInfo(bool alt_wpinfo = false);
			int InfoEvent(TEvent *e);
			TSprite *GetSprite();
			void Explode();
			void Write(WriteStream &stream);
			void Read(ReadStream &stream);
			
			void GlobalDestruction();
					// tahle fce ho vybouchne				
	};
	



// artefaktovy transporter:
class TGargantua : public TGroundUnit {
		public:
			TGargantua() : TGroundUnit() {};
			int GetType() {return unGargantua;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
			void GetUnitInfo(bool alt_wpinfo = false);
			void Explode();
	};
	
	

// miner:    //////////////////
class TXenon : public TGroundUnit {
		public:
			int Mines;
		
			TXenon() : TGroundUnit() {};
			void Init(int x, int y, int party, ReadStream *stream = NULL);
			int GetType() {return unXenon;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
			void GetUnitInfo(bool alt_wpinfo = false);
			int InfoEvent(TEvent *e);
			void GoOnMine();
			void PrepareFieldTime(int x, int y);
			void Write(WriteStream &stream);
			void Read(ReadStream &stream);
			
			int PlaceMine();
					// polozi minu
			void UnplaceMine();
					// sebere minu
	};




// Falanga:

class TFalanga : public TGroundToweredUnit {
		public:
			TFalanga() : TGroundToweredUnit() {};
			int GetType() {return unFalanga;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
			unsigned GetSupportedActions();
	};





// zenijni jednotka:

class TMedea : public TGroundUnit {
		public:
			TMedea() : TGroundUnit() {};
			int GetType() {return unMedea;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_HEAVY;};
			unsigned GetFieldActions(int x, int y);
			void GetCursor(int x, int y, int *cursor, int *selbold);
			void Action(int x, int y);
			
			int BuildField(int x, int y);
					// postavi na danem policku, co se tam hodi (most, railway)
					// vraci FALSE, kdyz neni cas
	};




// radarove vozitko Skiatron:

class TSkiatron : public TGroundUnit {
		public:
			TSkiatron() : TGroundUnit() {};
			int GetType() {return unSkiatron;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_LIGHT;};
			int DoVisib();
	};




// specialni tanky:

class TDirrac : public TGroundToweredUnit {
		public:
			TDirrac() : TGroundToweredUnit() {};
			int GetType() {return unDirrac;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
	};



class TAgni : public TGroundToweredUnit {
		public:
			TAgni() : TGroundToweredUnit() {};
			int GetType() {return unAgni;};
			void Setup();
			void IncLevel(int alevel);
			int GetWeight() {return WGT_MEDIUM;};
	};





/////////////////////////////	


#endif


