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
// Hlavickovy soubor pro budovy
//


#ifndef _BUILDING_H
#define _BUILDING_H

#include "units.h"


// obecny prapredek baraku:

class TBuilding : public TObject {
		public:
			int SizeX, SizeY;
			int MoneyIndex;
			int RepairingNow;
					// priznak urcuje, jestli je prave ted budova
					// opravovana (v kazdem kole 50 HP)
			
			TBuilding() : TObject() {};
			void Init(int x, int y, int party, FILE *f = NULL);
			void WriteInitReq(FILE *f);
			void Read(FILE *f);
			void Write(FILE *f);
		  void PlaceGround(int place);
			void Draw();
			TSprite *GetStatusBar();
			void GetUnitInfo();
			int InfoEvent(TEvent *e);
			TSprite *GetSprite();
			int GetWeight() {return WGT_ULTRAHEAVY;}
			unsigned GetAvailableActions();
			void Explode();
			void TurnReset();
			int IsAnimated() {return FALSE;}

			int GetAllVisib();
					// vraci viditelnost baraku. odpovida nejvice visib policku
			void RepairBuilding(int repair) {RepairingNow = repair;}
					// zapne/vypne opravovani baraku
	};





// barak, ktery muze mit az 8 verzi v ruznych smerech

class TMutableBuilding : public TBuilding {
		public:
			int Mutation;
			
			TMutableBuilding() : TBuilding() {};
			void Init(int x, int y, int party, FILE *f = NULL);
			void WriteInitReq(FILE *f);			
			TSprite *GetSprite();	
	};






// elektrarna:

class TPowerplant : public TMutableBuilding {
		public:
			TPowerplant() : TMutableBuilding() {};
			void Setup();
			int GetType() {return unPowerplant;}
			int IsAnimated() {return TRUE;}
			void DoAnim();
			unsigned GetAvailableActions();
	};



// cecenci:

class THouse : public TMutableBuilding {
		public:
			THouse() : TMutableBuilding() {};
			void Setup();
			int GetType() {return unHouse;}
			unsigned GetAvailableActions();
	};



// artefakty:

class TArtefactHouse : public TMutableBuilding {
		public:
			TArtefactHouse() : TMutableBuilding() {};
			void Setup();
			int GetType() {return unArtefactHouse;}
			unsigned GetAvailableActions();
			void Draw();
	};




// radar:

class TRadar : public TBuilding {
		public:
			int AnimPhase;
		
			TRadar() : TBuilding() {};
			void Setup();
			int GetType() {return unRadar;}
			int DoVisib();
			int IsAnimated() {return TRUE;}
			void DoAnim();
			TSprite *GetSprite();	
	};


// zakladna:

#define BASE_CAPACITY  24

class TBase : public TBuilding {
		public:
			int Inventory[BASE_CAPACITY];
			int LoadedUnits;
			int Capacity;

			virtual int LoadUnit(TUnit *u);
					// vezme dovnitr jednotku. return viz hlp
			virtual int UnloadUnit(TUnit *u);
					// vylozi jednotku ven
			void DoInventory();
					// provadi operace nad obsahem zakladny
			virtual int WantToLoad(TUnit *u);
					// vraci, jestli chce/umi nalozit tuhle jednotku
		
			TBase() : TBuilding() {};
			void Init(int x, int y, int party, FILE *f);
			int GetType() {return unBase;}
			void Setup();
			unsigned GetAvailableActions();
			void Read(FILE *f);
			void Write(FILE *f);
			void RemoveFromWorld();
			int InfoEvent(TEvent *e);
			void Action(int x, int y);
			void GetUnitInfo();
			void ChangeParty();
	};





// letiste:

class TAirport : public TBase {
		public:
			TAirport() : TBase() {};
			int GetType() {return unAirport;}
			void Setup();
			unsigned GetAvailableActions();
			int LoadUnit(TUnit *u);
			int UnloadUnit(TUnit *u);
			void TurnReset();
	};





// heliport:

class THeliport : public TAirport {
		public:
			THeliport() : TAirport() {};
			int GetType() {return unHeliport;}
			void Setup();
			unsigned GetAvailableActions();
			int LoadUnit(TUnit *u);
			int UnloadUnit(TUnit *u);
	};





// skladiste:

class TWarehouse : public TMutableBuilding {
		public:
			TWarehouse() : TMutableBuilding() {};
			int GetType() {return unWarehouse;}
			void Setup();
			unsigned GetAvailableActions();
			
			int RefullUnit(TUnit *u);
					// doplni transporter na max. zasoby
					// vraci true-false
	};


// vyrobna jednotek:

class TFactory : public TBuilding {
		public:
			int CurrentJob;
					// typ jednotky, ktera se prave vyrabi (0 kdyz nic)
			int CurrentPhase, CurrentNeed;
					// pocet kol, stravenych nad jobem a potrebna kola
		
			TFactory() : TBuilding() {};
			int GetType() {return unFactory;}
			void Setup();
			int InfoEvent(TEvent *e);
			void GetUnitInfo();
			void TurnReset();
			void Action(int x, int y);
			void Select();
			void Read(FILE *f);
			void Write(FILE *f);
			
			void DoProducing(); // dialog...
			int ProduceUnit(int untype);
					// vytvori jednotku daneho typu. Vraci FALSE, pokud to nejde
					// (duvod: napr. out of money)
	};






// opravna jednotek:

class TRepairBay : public TBase {
		public:
			TRepairBay() : TBase() {};
			int GetType() {return unRepairBay;}
			void Setup();
			void TurnReset();
			int LoadUnit(TUnit *u);
			int UnloadUnit(TUnit *u);
			int GetLoadingPoint(int *x, int *y, TUnit *u);
	};





// doky:

class TDocks : public TBase {
		public:
			int Orient;
		
			TDocks() : TBase() {};
			void Init(int x, int y, int party, FILE *f = NULL);
			void WriteInitReq(FILE *f);			
			int GetType() {return unDocks;}
			void Setup();
			int WantToLoad(TUnit *u);
			void TurnReset();
			int LoadUnit(TUnit *u);
			int UnloadUnit(TUnit *u);
			
			TPoint GetLoadingPoint(TUnit *u);
					// vraci pozici, kde ma pristat nebo x=-1 kdyz to nejde
			TSprite *GetSprite();					
	};





extern int MoneyGoodlife, MoneyBadlife;
		// penize...
inline int GetMoney(int unitID) {return ((unitID  < BADLIFE) ? MoneyGoodlife : MoneyBadlife);};
		// vraci pocet penez strany, kam patri ta dana jednotka
inline void SetMoney(int unitID, int value) {((unitID  < BADLIFE) ? MoneyGoodlife : MoneyBadlife) = value;};
		// nastavi prasule

#endif
