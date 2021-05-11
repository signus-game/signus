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
// Artificial Intelligence of Signus
//
//       Global Intelligence
//
//           Mission 13
//
// Programmed by Richard Wunsch
//

#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "aiair.h"
#include "aimarine.h"

// Action Radius of Army 1
#define A1X1 149
#define A1Y1 63
#define A1X2 179
#define A1Y2 85

// Action Radius of Army 2
#define A2X1 151
#define A2Y1 32
#define A2X2 184
#define A2Y2 62

// Action Radius of Army 3
#define A3X1 173
#define A3Y1 10
#define A3X2 189
#define A3Y2 34

// Action Radius of Army 4
#define A4X1 63
#define A4Y1 39
#define A4X2 98
#define A4Y2 68

// Action Radius of Army 5
#define A5X1 28
#define A5Y1 91
#define A5X2 68
#define A5Y2 125

// Action Radius of Army 6
#define A6X1  1
#define A6Y1 25
#define A6X2 38
#define A6Y2 74

// Action Radius of Marine 1
#define M1X1 147
#define M1Y1 13
#define M1X2 176
#define M1Y2 32

// Action Radius of Marine 2
#define M2X1 25
#define M2Y1 41
#define M2X2 51
#define M2Y2 67

// Action Radius of Marine 3
#define M3X1  0
#define M3Y1  9
#define M3X2 14
#define M3Y2 24

// Action Radius of Marine 4
#define M4X1  0
#define M4Y1 65
#define M4X2 14
#define M4Y2 81

// Action Radius of Air Army 5
#define AA5X1 55
#define AA5Y1 104
#define AA5X2 66
#define AA5Y2 116

// Action Radius of Air Army 6
#define AA6X1 14
#define AA6Y1 54
#define AA6X2 26
#define AA6Y2 67

// Action Radius of Air Army 7
#define AA7X1  8
#define AA7Y1 33
#define AA7X2 18
#define AA7Y2 41

// Radar of Marine 3 & 4
#define RX 26
#define RY 58

// Factory 1
#define F1X 77
#define F1Y 57

// Factory 2
#define F2X 16
#define F2Y 63

// Base 1
#define B1X 165
#define B1Y 47

// Base 2
#define B2X 75
#define B2Y 49

// Base 3
#define B3X 49
#define B3Y 100

// Base 4
#define B4X 16
#define B4Y 45

// Doky 1
#define D1X 164
#define D1Y  33
#define TD1X1 160
#define TD1Y1   2
#define TD1X2 165
#define TD1Y2   7


// 1st selected unit
#define SelX 161
#define SelY 118


#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6
#define CMinDangM1 0    // Dangerous limit for Army 1
#define CMinDangM2 0    // Dangerous limit for Army 2
#define CMinDangM3 0    // Dangerous limit for Army 3
#define CMinDangM4 0    // Dangerous limit for Army 4
#define CMinDangA5 0    // Dangerous limit for AirArmy 5
#define CMinDangA6 0    // Dangerous limit for AirArmy 6
#define CMinDangA7 0    // Dangerous limit for AirArmy 7

#define CSuicide1  5    // Suicide limit for Army 1
#define CSuicide2  5    // Suicide limit for Army 2
#define CSuicide3  5    // Suicide limit for Army 3
#define CSuicide4  5    // Suicide limit for Army 4
#define CSuicide5  5    // Suicide limit for Army 5
#define CSuicide6  5    // Suicide limit for Army 6
#define CSuicideM1 8    // Suicide limit for Army 1
#define CSuicideM2 8    // Suicide limit for Army 2
#define CSuicideM3 8    // Suicide limit for Army 3
#define CSuicideM4 8    // Suicide limit for Army 4
#define CSuicideA5 5    // Suicide limit for AirArmy 5
#define CSuicideA6 5    // Suicide limit for AirArmy 6
#define CSuicideA7 5    // Suicide limit for AirArmy 7

TPoint MinePlaces13 [] = {178,45, 163,74, 165,74, 80,58, 76,53, 17,48, 20,61,
                                                -1,-1};

int Factory13List1 [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama,
                                                 unXenon, unFalanga, unIris, unCeres, unGnom,
                                             unRex, unHerkules, unSkorpion, unPatriot, unGolias, 0};

int Factory13List2 [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama,
                                                 unXenon, unFalanga, unCeres, unGnom,
                                             unRex, unHerkules, unSkorpion, unPatriot, unGolias, 0};



void InitAI13 ()
{
    int i,j;
    TField *f;
    TUnit *u;

    LockDraw ();        

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 0;
    MoneyBadlife = 10000;
    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    Factory1 = GetField (F1X, F1Y) -> Unit; 
    Factory2 = GetField (F2X, F2Y) -> Unit; 
    Base1 = GetField (B1X, B1Y) -> Unit;
    Base2 = GetField (B2X, B2Y) -> Unit;
    Base3 = GetField (B3X, B3Y) -> Unit;
    Base4 = GetField (B4X, B4Y) -> Unit;
    

    Towers = new TTowers ();
    for (i = 0; i < UNITS_TOP; i++) {
        if (Units [i] == NULL) continue;
        // vsechny veze a GL
        if ((((TUnit *) Units [i]) -> Type >= (unHorizont + BADLIFE) 
        && ((TUnit *) Units [i]) -> Type  <= (unSpektrum + BADLIFE))
        || (((TUnit *) Units[i]) -> Type < BADLIFE)) 
            Towers -> Insert (i);
    }
    
    Army1 = new TGroundArmy();
    Army1 -> SetAR (A1X1, A1Y1, A1X2, A1Y2);
    Army1 -> SetRadar (-1, -1);
    Army1 -> InsertUnits ();

    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> SetRadar (-1, -1);
    Army2 -> InsertUnits ();
    
    Army3 = new TGroundArmy();
    Army3 -> SetAR (A3X1, A3Y1, A3X2, A3Y2);
    Army3 -> SetRadar (-1, -1);
    Army3 -> InsertUnits ();

    Army4 = new TGroundArmy();
    Army4 -> SetAR (A4X1, A4Y1, A4X2, A4Y2);
    Army4 -> SetRadar (-1, -1);
    Army4 -> InsertUnits ();

    Army5 = new TGroundArmy();
    Army5 -> SetAR (A5X1, A5Y1, A5X2, A5Y2);
    Army5 -> SetRadar (-1, -1);
    Army5 -> InsertUnits ();

    Army6 = new TGroundArmy();
    Army6 -> SetAR (A6X1, A6Y1, A6X2, A6Y2);
    Army6 -> SetRadar (-1, -1);
    Army6 -> InsertUnits ();

    Marine1 = new TMarine();
    Marine1 -> SetAR (M1X1, M1Y1, M1X2, M1Y2);
    Marine1 -> SetRadar (-1, -1);
    Marine1 -> InsertUnits ();

    Marine2 = new TMarine();
    Marine2 -> SetAR (M2X1, M2Y1, M2X2, M2Y2);
    Marine2 -> SetRadar (-1, -1);
    Marine2 -> InsertUnits ();

    Marine3 = new TMarine();
    Marine3 -> SetAR (M3X1, M3Y1, M3X2, M3Y2);
    Marine3 -> SetRadar (RX, RY);
    Marine3 -> InsertUnits ();

    Marine4 = new TMarine();
    Marine4 -> SetAR (M4X1, M4Y1, M4X2, M4Y2);
    Marine4 -> SetRadar (RX, RY);
    Marine4 -> InsertUnits ();

    AirArmy5 = new TAirArmy();
    AirArmy5 -> SetAR (AA5X1, AA5Y1, AA5X2, AA5Y2);
    AirArmy5 -> SetRadar (-1, -1);
    AirArmy5 -> InsertUnits ();
    AirArmy5 -> Status = asActive;
    
    AirArmy6 = new TAirArmy();
    AirArmy6 -> SetAR (AA6X1, AA6Y1, AA6X2, AA6Y2);
    AirArmy6 -> SetRadar (-1, -1);
    AirArmy6 -> InsertUnits ();
    AirArmy6 -> Status = asActive;
    
    AirArmy7 = new TAirArmy();
    AirArmy7 -> SetAR (AA7X1, AA7Y1, AA7X2, AA7Y2);
    AirArmy7 -> SetRadar (-1, -1);
    AirArmy7 -> InsertUnits ();
    AirArmy7 -> Status = asActive;

    Docks1 = GetField (D1X, D1Y) -> Unit;
    for (i = TD1X1; i <= TD1X2; i++) {
        for (j = TD1Y1; j <= TD1Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TDocks *)Units [Docks1]) -> Inventory [((TDocks*)Units [Docks1]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    
    // Polozime miny    
    for (i = 0; MinePlaces13 [i].x != -1; i++) {
        MinePlace (MinePlaces13 [i].x, MinePlaces13 [i].y, BADLIFE);
    }

    // zamerime jednotku
    i = GetField (SelX, SelY) -> Unit;  
    Units [i] -> Select ();
    Units [i] -> Center ();

    UnlockDraw ();          
    RedrawMap ();

}




void LoadArtificialIntelligence13(ReadStream &stream, int format) {
	DoneArtificialIntelligence13 ();

	MBPlaces = NULL;
	DeniedPlaces = NULL;
	DUPos = 0;
	loadAttackedFields(stream, format);

	Towers = new TTowers(stream);
	Army1 = new TGroundArmy(stream);
	Army2 = new TGroundArmy(stream);
	Army3 = new TGroundArmy(stream);
	Army4 = new TGroundArmy(stream);
	Army5 = new TGroundArmy(stream);
	Army6 = new TGroundArmy(stream);
	Marine1 = new TMarine(stream);
	Marine2 = new TMarine(stream);
	Marine3 = new TMarine(stream);
	Marine4 = new TMarine(stream);
	AirArmy5 = new TAirArmy(stream);
	AirArmy6 = new TAirArmy(stream);
	AirArmy7 = new TAirArmy(stream);
}

void SaveArtificialIntelligence13(WriteStream &stream) {
	saveAttackedFields(stream);

	Towers->Save(stream);
	Army1->Save(stream);
	Army2->Save(stream);
	Army3->Save(stream);
	Army4->Save(stream);
	Army5->Save(stream);
	Army6->Save(stream);
	Marine1->Save(stream);
	Marine2->Save(stream);
	Marine3->Save(stream);
	Marine4->Save(stream);
	AirArmy5->Save(stream);
	AirArmy6->Save(stream);
	AirArmy7->Save(stream);
	DUPos = 0;
}

int ArtificialIntelligence13 ()
{
    int i, j, k, st, id;
    TPoint p;

    Message (SigText[TXT_AI_ANALYSE]);

    // Prepocitame viditelnost
    AllowBadlife (TRUE);
    ComputeVisib ();
    RedrawMap ();
    
    LockDraw ();

    // Aktualizujeme specialni objekty
    Factory1 = GetField (F1X, F1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    if (Factory2 == NO_UNIT || Units [Factory2] -> Type % BADLIFE != unFactory) 
        Factory2 = NO_UNIT;
    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    Base2 = GetField (B2X, B2Y) -> Unit;
    if (Base2 == NO_UNIT || Units [Base2] -> Type % BADLIFE != unBase) 
        Base2 = NO_UNIT;
    Base3 = GetField (B3X, B3Y) -> Unit;
    if (Base3 == NO_UNIT || Units [Base3] -> Type % BADLIFE != unBase) 
        Base3 = NO_UNIT;
    Base4 = GetField (B4X, B4Y) -> Unit;
    if (Base4 == NO_UNIT || Units [Base4] -> Type % BADLIFE != unBase) 
        Base4 = NO_UNIT;

    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
    Marine1 -> DeleteKilled2 (); Marine1 -> DeleteKilled ();
    Marine2 -> DeleteKilled2 (); Marine2 -> DeleteKilled ();
    Marine3 -> DeleteKilled2 (); Marine3 -> DeleteKilled ();
    Marine4 -> DeleteKilled2 (); Marine4 -> DeleteKilled ();
    AirArmy5 -> DeleteKilled2 (); AirArmy5 -> DeleteKilled ();
    AirArmy6 -> DeleteKilled2 (); AirArmy6 -> DeleteKilled ();
    AirArmy7 -> DeleteKilled2 (); AirArmy7 -> DeleteKilled ();
    DUPos = 0;
    
    // Testneme konec hry   
    st = AssignResult ();
    if (st != 0) {
        UnlockDraw ();  
        AllowBadlife (FALSE);       
        return st;  
    }

    // Zpracujeme seznam poli, na ktere se utocilo
    Army1 -> DoAttackedFields ();
    Army2 -> DoAttackedFields ();
    Army3 -> DoAttackedFields ();
    Army4 -> DoAttackedFields ();
    Army5 -> DoAttackedFields ();
    Army6 -> DoAttackedFields ();
    Marine1 -> DoAttackedFields ();
    Marine2 -> DoAttackedFields ();
    Marine3 -> DoAttackedFields ();
    Marine4 -> DoAttackedFields ();
    AirArmy5 -> DoAttackedFields ();
    AirArmy6 -> DoAttackedFields ();
    AirArmy7 -> DoAttackedFields ();
    AttackFieldPos = 0;
    
    AnalyseLandscape ();    
    
    Message (SigText[TXT_AI_ARMY]);



//////////////////////////////// AKTIVACE ARMAD

    // Test zaktivovani armady 1
    if (Army1 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army1 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 2
    if (Army2 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army2 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army2 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 3
    if (Army3 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army3 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army3 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 4
    if (Army4 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army4 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army4 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 5
    if (Army5 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army5 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army5 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 6
    if (Army6 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army6 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army6 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani marine 1
    if (Marine1 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine1 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani marine 2
    if (Marine2 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine2 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine2 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani marine 3
    if (Marine3 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine3 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine3 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani marine 4
    if (Marine4 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine4 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine4 -> Status = asActive;
            }
        }
    }

///////////////////////////// AKCE ARMAD

    // Nastavime ProgressBar
    st = 0;
    st += Towers -> nofBadLife;
    if (Army1 -> Status == asActive || Army1 -> Status == asSuicide) st += Army1 -> nofBadLife;
    if (Army2 -> Status == asActive || Army2 -> Status == asSuicide) st += Army2 -> nofBadLife;
    if (Army3 -> Status == asActive || Army3 -> Status == asSuicide) st += Army3 -> nofBadLife;
    if (Army4 -> Status == asActive || Army4 -> Status == asSuicide) st += Army4 -> nofBadLife;
    if (Army5 -> Status == asActive || Army5 -> Status == asSuicide) st += Army5 -> nofBadLife;
    if (Marine1 -> Status == asActive || Marine1 -> Status == asSuicide) st += Marine1 -> nofBadLife;
    if (Marine2 -> Status == asActive || Marine2 -> Status == asSuicide) st += Marine2 -> nofBadLife;
    if (Marine3 -> Status == asActive || Marine3 -> Status == asSuicide) st += Marine3 -> nofBadLife;
    if (Marine4 -> Status == asActive || Marine4 -> Status == asSuicide) st += Marine4 -> nofBadLife;
    if (AirArmy5 -> Status == asActive || AirArmy5 -> Status == asSuicide) st += AirArmy5 -> nofBadLife;
    if (AirArmy6 -> Status == asActive || AirArmy6 -> Status == asSuicide) st += AirArmy6 -> nofBadLife;
    if (AirArmy7 -> Status == asActive || AirArmy7 -> Status == asSuicide) st += AirArmy7 -> nofBadLife;
    if (st == 0) st = 5; // Magic number
    ProgressNull ();
    ProgressLimit (st);     

    // Vylozime letiste
    UnlockDraw ();

    for (i = BADLIFE; i < UNITS_TOP; i++){
        // Airport
        if (Units [i] != NULL && Units [i] -> Type - BADLIFE == unAirport) {            
            for (j = 0; j < ((TAirport *) Units [i]) -> LoadedUnits; j++) {
                id = ((TAirport *)Units [i]) -> Inventory [j];
                if (Units [id] -> AI_Info.State == aistFuel) {
                    st = ((TAirport *)Units [i]) -> UnloadUnit ((TUnit *)Units [id]);
                    if (st == 1) {
                        j--;
                        Units [id] -> AI_Info.State = aistNought;
                        for (k = 120; k > 1; k--) {
                            p = CartezianSnail (k);
                            p.x += Units [id] -> X; p.y += Units [id] -> Y;
                            if (p.x >= 0 && p.x < MapSizeX && p.y >= 0 && p.y < MapSizeY
                            && GetAircraftAt (p.x, p.y) == NULL) {
                                Units [id] -> Select ();
                                st = Units [id] -> MoveFar (p.x, p.y);
                                if (st == 1) break;
                                
                            }
                        }
                    }
                }
            }
        }
        // Heliport
        if (Units [i] != NULL && Units [i] -> Type - BADLIFE == unHeliport) {           
            for (j = 0; j < ((THeliport *) Units [i]) -> LoadedUnits; j++) {
                id = ((THeliport *)Units [i]) -> Inventory [j];
                if (Units [id] -> AI_Info.State == aistFuel) {
                    st = ((THeliport *)Units [i]) -> UnloadUnit ((TUnit *)Units [id]);
                    if (st == 1) {
                        j--;
                        Units [id] -> AI_Info.State = aistNought;
                        for (k = 120; k > 1; k--) {
                            p = CartezianSnail (k);
                            p.x += Units [id] -> X; p.y += Units [id] -> Y;
                            if (p.x >= 0 && p.x < MapSizeX && p.y >= 0 && p.y < MapSizeY
                            && GetAircraftAt (p.x, p.y) == NULL) {
                                Units [id] -> Select ();
                                st = Units [id] -> MoveFar (p.x, p.y);
                                if (st == 1) break;
                            }
                        }
                    }
                }
            }
        }
        // Laguna
        if (Units [i] != NULL && Units [i] -> Type - BADLIFE == unLaguna) {         
            for (j = 0; j < ((TLaguna *) Units [i]) -> LoadedUnits; j++) {
                id = ((TLaguna *)Units [i]) -> Inventory [j];
                if (Units [id] -> AI_Info.State == aistFuel) {
                    st = ((TLaguna *)Units [i]) -> UnloadUnit ((TUnit *)Units [id]);
                    if (st == 1) {
                        j--;
                        Units [id] -> AI_Info.State = aistNought;
                        for (k = 120; k > 1; k--) {
                            p = CartezianSnail (k);
                            p.x += Units [id] -> X; p.y += Units [id] -> Y;
                            if (p.x >= 0 && p.x < MapSizeX && p.y >= 0 && p.y < MapSizeY
                            && GetAircraftAt (p.x, p.y) == NULL) {
                                Units [id] -> Select ();
                                st = Units [id] -> MoveFar (p.x, p.y);
                                if (st == 1) break;
                            }
                        }
                    }
                }
            }
        }
        
    }
    LockDraw ();

    // Veze
    if (Towers -> MakeTurn () == FALSE) {
        st = AssignResult ();
        if (st != 0) {
            UnlockDraw ();  
            AllowBadlife (FALSE);       
            return st;  
        }
    }

    
    // ARMY 1
    if (Army1 -> Status == asActive || Army1 -> Status == asSuicide) {
        Army1 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army1 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        Army1 -> CountDangerous ();
        if (Army1 -> Dangerous <= CSuicide1) {
            Army1 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army1 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }

        // Likvidace armady 1
        Army1 -> CountDangerous ();     
        if (Army1 -> Dangerous <= CMinDang1) {
            Army1 -> Status = asDestroyed;          
        }
        
        
    }

    // ARMY 2
    if (Army2 -> Status == asActive || Army2 -> Status == asSuicide) {
        Army2 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army2 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army2 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        Army2 -> CountDangerous ();
        if (Army2 -> Dangerous <= CSuicide2) {
            Army2 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army2 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }

        // Likvidace armady 2
        if (Army2 -> Dangerous <= CMinDang2) {
            Army2 -> Status = asDestroyed;
        }
        
    }

    // ARMY 3
    if (Army3 -> Status == asActive || Army3 -> Status == asSuicide) {
        Army3 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army3 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army3 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        Army3 -> CountDangerous ();
        if (Army3 -> Dangerous <= CSuicide3) {
            Army3 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army3 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 3
        if (Army3 -> Dangerous <= CMinDang3) {
            Army3 -> Status = asDestroyed;
        }
                
    }
    // ARMY 4
    if (Army4 -> Status == asActive || Army4 -> Status == asSuicide) {
        Army4 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army4 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army4 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army4 -> CountDangerous ();
        if (Army4 -> Dangerous <= CSuicide4) {
            Army4 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army4 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 4
        if (Army4 -> Dangerous <= CMinDang4) {
            Army4 -> Status = asDestroyed;
        }               
    }
    
    // ARMY 5
    if (Army5 -> Status == asActive || Army5 -> Status == asSuicide) {
        Army5 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army5 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army5 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army5 -> CountDangerous ();
        if (Army5 -> Dangerous <= CSuicide5) {
            Army5 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army5 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 5
        if (Army5 -> Dangerous <= CMinDang5) {
            Army5 -> Status = asDestroyed;
        }               
    }
    // ARMY 6
    if (Army6 -> Status == asActive || Army6 -> Status == asSuicide) {
        Army6 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army6 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army6 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army6 -> CountDangerous ();
        if (Army6 -> Dangerous <= CSuicide6) {
            Army6 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army6 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 6
        if (Army6 -> Dangerous <= CMinDang6) {
            Army6 -> Status = asDestroyed;
        }               
    }


    // MARINE 1
    if (Marine1 -> Status == asActive || Marine1 -> Status == asSuicide) {
        Marine1 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Marine1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine1 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Marine1 -> CountDangerous ();
        if (Marine1 -> Dangerous <= CSuicideM1) {
            Marine1 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Marine1 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 1
        if (Marine1 -> Dangerous <= CMinDangM1) {
            Marine1 -> Status = asDestroyed;
        }               
    }
    
    // MARINE 2
    if (Marine2 -> Status == asActive || Marine2 -> Status == asSuicide) {
        Marine2 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Marine2 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine2 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Marine2 -> CountDangerous ();
        if (Marine2 -> Dangerous <= CSuicideM2) {
            Marine2 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Marine2 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 2
        if (Marine2 -> Dangerous <= CMinDangM2) {
            Marine2 -> Status = asDestroyed;
        }               
    }
    
    // MARINE 3
    if (Marine3 -> Status == asActive || Marine3 -> Status == asSuicide) {
        Marine3 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Marine3 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine3 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Marine3 -> CountDangerous ();
        if (Marine3 -> Dangerous <= CSuicideM3) {
            Marine3 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Marine3 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 3
        if (Marine3 -> Dangerous <= CMinDangM3) {
            Marine3 -> Status = asDestroyed;
        }               
    }
    
    // MARINE 4
    if (Marine4 -> Status == asActive || Marine4 -> Status == asSuicide) {
        Marine4 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Marine4 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine4 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Marine4 -> CountDangerous ();
        if (Marine4 -> Dangerous <= CSuicideM4) {
            Marine4 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Marine4 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 4
        if (Marine4 -> Dangerous <= CMinDangM4) {
            Marine4 -> Status = asDestroyed;
        }               
    }
    
    // AIR ARMY 5
    if (AirArmy5 -> Status == asActive || AirArmy5 -> Status == asSuicide) {
        AirArmy5 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (AirArmy5 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy5 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        AirArmy5 -> CountDangerous ();
        if (AirArmy5 -> Dangerous <= CSuicideA5) {
            AirArmy5 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (AirArmy5 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 5
        if (AirArmy5 -> Dangerous <= CMinDangA5) {
            AirArmy5 -> Status = asDestroyed;
        }
                
    }

    // AIR ARMY 7
    if (AirArmy7 -> Status == asActive || AirArmy7 -> Status == asSuicide) {
        AirArmy7 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (AirArmy7 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy7 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        AirArmy7 -> CountDangerous ();
        if (AirArmy7 -> Dangerous <= CSuicideA7) {
            AirArmy7 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (AirArmy7 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 7
        if (AirArmy7 -> Dangerous <= CMinDangA7) {
            AirArmy7 -> Status = asDestroyed;
        }
                
    }


    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult13 ()
{
    int i, t = 0;

    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    Base2 = GetField (B2X, B2Y) -> Unit;
    if (Base2 == NO_UNIT || Units [Base2] -> Type % BADLIFE != unBase) 
        Base2 = NO_UNIT;
    Base3 = GetField (B3X, B3Y) -> Unit;
    if (Base3 == NO_UNIT || Units [Base3] -> Type % BADLIFE != unBase) 
        Base3 = NO_UNIT;
    Base4 = GetField (B4X, B4Y) -> Unit;
    if (Base4 == NO_UNIT || Units [Base4] -> Type % BADLIFE != unBase) 
        Base4 = NO_UNIT;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if ((Base1 == NO_UNIT || Base1 < BADLIFE)
    && (Base2 == NO_UNIT || Base2 < BADLIFE)
    && (Base3 == NO_UNIT || Base3 < BADLIFE)
    && (Base4 == NO_UNIT || Base4 < BADLIFE)) return 1; // GoodLife vyhral
        
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits13 (int FactoryID)
{
    Factory1 = GetField (F1X, F1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    if (Factory2 == NO_UNIT || Units [Factory2] -> Type % BADLIFE != unFactory) 
        Factory2 = NO_UNIT;
    
    if (FactoryID == Factory1) return Factory13List1;
    else return Factory13List2;
}



void DoneArtificialIntelligence13() {
	free_landscape_analysis();

	delete Army1;
	Army1 = NULL;
	delete Army2;
	Army2 = NULL;
	delete Army3;
	Army3 = NULL;
	delete Army4;
	Army4 = NULL;
	delete Army5;
	Army5 = NULL;
	delete Army6;
	Army6 = NULL;
	delete Marine1;
	Marine1 = NULL;
	delete Marine2;
	Marine2 = NULL;
	delete Marine3;
	Marine3 = NULL;
	delete Marine4;
	Marine4 = NULL;
	delete AirArmy5;
	AirArmy5 = NULL;
	delete AirArmy6;
	AirArmy6 = NULL;
	delete AirArmy7;
	AirArmy7 = NULL;
	delete Towers;
	Towers = NULL;
}

    
    
// OK   
