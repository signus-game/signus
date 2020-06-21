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
// Artificial Intelligence of Signus
//
//       Global Intelligence
//
//           Mission 17
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
#define A1X1 52
#define A1Y1 48
#define A1X2 83
#define A1Y2 75

// Action Radius of Army 2
#define A2X1 86
#define A2Y1 62
#define A2X2 105
#define A2Y2 85

// Action Radius of Army 3
#define A3X1 74
#define A3Y1 79
#define A3X2 102
#define A3Y2 101

// Action Radius of Army 4
#define A4X1 51
#define A4Y1 104
#define A4X2 105
#define A4Y2 136

// Action Radius of Army 5
#define A5X1 60
#define A5Y1 140
#define A5X2 91
#define A5Y2 178

// Action Radius of Army 6
#define A6X1 92
#define A6Y1 143
#define A6X2 105
#define A6Y2 179

// Action Radius of Army 7
#define A7X1 15
#define A7Y1 129
#define A7X2 56
#define A7Y2 164


// Action Radius of Marine 1
#define M1X1 53
#define M1Y1 22
#define M1X2 72
#define M1Y2 43

// Action Radius of Marine 2
#define M2X1 78
#define M2Y1 31
#define M2X2 103
#define M2Y2 59

// Action Radius of Air Army 1
#define AA1X1 95
#define AA1Y1 49
#define AA1X2 104
#define AA1Y2 59

// Action Radius of Air Army 2
#define AA2X1 100
#define AA2Y1 170
#define AA2X2 105
#define AA2Y2 179

// Action Radius of Air Army 3
#define AA3X1 93
#define AA3Y1 176
#define AA3X2 101
#define AA3Y2 179

// Radar 2
#define R2X 80
#define R2Y 109

// Radar 5
#define R5X 85
#define R5Y 158

// Radar 6
#define R6X 100
#define R6Y 158

// Radar 7
#define R7X 32
#define R7Y 141


// Oasa 1
#define O1X 93
#define O1Y  5
#define TO1X1 70
#define TO1Y1  8
#define TO1X2 80
#define TO1Y2  8

// Oasa 2
#define O2X 96
#define O2Y  5
#define TO2X1 70
#define TO2Y1 10
#define TO2X2 80
#define TO2Y2 10

// Oasa 3
#define O3X 96
#define O3Y  8
#define TO3X1 70
#define TO3Y1 12
#define TO3X2 80
#define TO3Y2 12

// Oasa 4
#define O4X 97
#define O4Y 10
#define TO4X1 70
#define TO4Y1 14
#define TO4X2 80
#define TO4Y2 14

// Oasa 5
#define O5X 94
#define O5Y 10
#define TO5X1 70
#define TO5Y1 16
#define TO5X2 80
#define TO5Y2 16

// Oasa 6
#define O6X 96
#define O6Y 12
#define TO6X1 70
#define TO6Y1 18
#define TO6X2 80
#define TO6Y2 18

// Neptun 1
#define N1X 99
#define N1Y  8
#define TN1X1 70
#define TN1Y1  4
#define TN1X2 80
#define TN1Y2  4

// Neptun 2
#define N2X 99
#define N2Y 12
#define TN2X1 70
#define TN2Y1  6
#define TN2X2 80
#define TN2Y2  6

// Laguna 1
#define L1X 86
#define L1Y  6
#define TL1X1 60
#define TL1Y1  4
#define TL1X2 70
#define TL1Y2  4

// Laguna 2
#define L2X 91
#define L2Y  9
#define TL2X1 60
#define TL2Y1  6
#define TL2X2 70
#define TL2Y2  6

// Base1 
#define B1X 91
#define B1Y 164

// Factory1 
#define F1X 32
#define F1Y 149

// 1st selected unit
#define SelX 94
#define SelY 17



#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6
#define CMinDang7  0    // Dangerous limit for Army 7
#define CMinDangM1 0    // Dangerous limit for Marine 1
#define CMinDangM2 0    // Dangerous limit for Marine 2
#define CMinDangA1 0    // Dangerous limit for Air Army 1
#define CMinDangA2 0    // Dangerous limit for Air Army 2
#define CMinDangA3 0    // Dangerous limit for Air Army 3

#define CSuicide1  5    // Suicide limit for Army 1
#define CSuicide2  5    // Suicide limit for Army 2
#define CSuicide3  5    // Suicide limit for Army 3
#define CSuicide4  5    // Suicide limit for Army 4
#define CSuicide5  5    // Suicide limit for Army 5
#define CSuicide6  5    // Suicide limit for Army 6
#define CSuicide7  5    // Suicide limit for Army 7
#define CSuicideM1 8    // Suicide limit for Marine 1
#define CSuicideM2 8    // Suicide limit for Marine 2
#define CSuicideA1 8    // Suicide limit for Air Army 1
#define CSuicideA2 8    // Suicide limit for Air Army 2
#define CSuicideA3 8    // Suicide limit for Air Army 3

TPoint MinePlaces17 [] = {101,72, 92,75, 87,86, 103,91, 67,58, 71,69, 92,146,
                                                    95,165, 83,168, 73,166, 43,159,
                                                -1,-1};


int Factory17List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama, 
                                             unRex, unHerkules, unSkorpion, unPatriot, unGolias, 
                                             unCeres, unGnom, unIris, 0};



void InitAI17 ()
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

    Oasa1 = GetField (O1X, O1Y) -> Unit;    
    Oasa2 = GetField (O2X, O2Y) -> Unit;    
    Oasa3 = GetField (O3X, O3Y) -> Unit;    
    Oasa4 = GetField (O4X, O4Y) -> Unit;    
    Oasa5 = GetField (O5X, O5Y) -> Unit;        Oasa6 = GetField (O6X, O6Y) -> Unit;        Neptun1 = GetField (N1X, N1Y) -> Unit;      Neptun2 = GetField (N2X, N2Y) -> Unit;      Laguna1 = GetField (L1X, L1Y) -> Unit;
    Laguna2 = GetField (L2X, L2Y) -> Unit;

    Factory1 = GetField (F1X, F1Y) -> Unit; 
    Base1 = GetField (B1X, B1Y) -> Unit;    


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
    Army5 -> SetRadar (R5X, R5Y);
    Army5 -> InsertUnits ();

    Army6 = new TGroundArmy();
    Army6 -> SetAR (A6X1, A6Y1, A6X2, A6Y2);
    Army6 -> SetRadar (R6X, R6Y);
    Army6 -> InsertUnits ();

    Army7 = new TGroundArmy();
    Army7 -> SetAR (A7X1, A7Y1, A7X2, A7Y2);
    Army7 -> SetRadar (R7X, R7Y);
    Army7 -> InsertUnits ();

    Marine1 = new TMarine ();
    Marine1 -> SetAR (M1X1, M1Y1, M1X2, M1Y2);
    Marine1 -> SetRadar (-1, -1);
    Marine1 -> InsertUnits ();
    Marine1 -> Status = asActive;
    Marine1 -> SetAIConst (2);
    Marine1 -> Insert (Oasa5);

    Marine2 = new TMarine ();
    Marine2 -> SetAR (M2X1, M2Y1, M2X2, M2Y2);
    Marine2 -> SetRadar (-1, -1);
    Marine2 -> InsertUnits ();
    Marine2 -> Status = asActive;
    Marine2 -> SetAIConst (2);
    Marine2 -> Insert (Oasa5);

    AirArmy1 = new TAirArmy();
    AirArmy1 -> SetAR (AA1X1, AA1Y1, AA1X2, AA1Y2);
    AirArmy1 -> SetRadar (-1, -1);
    AirArmy1 -> InsertUnits ();
    AirArmy1 -> Status = asActive;
    AirArmy1 -> Insert (Oasa5);
    

    AirArmy2 = new TAirArmy();
    AirArmy2 -> SetAR (AA2X1, AA2Y1, AA2X2, AA2Y2);
    AirArmy2 -> SetRadar (R2X, R2Y);
    AirArmy2 -> InsertUnits ();
    AirArmy2 -> Status = asActive;

    AirArmy3 = new TAirArmy();
    AirArmy3 -> SetAR (AA3X1, AA3Y1, AA3X2, AA3Y2);
    AirArmy3 -> SetRadar (-1, -1);
    AirArmy3 -> InsertUnits ();
    AirArmy3 -> Status = asActive;

    // Naplnime Oasy
    for (i = TO1X1; i <= TO1X2; i++) {
        for (j = TO1Y1; j <= TO1Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOasa*)Units [Oasa1]) -> Inventory [((TOasa*)Units [Oasa1]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TO2X1; i <= TO2X2; i++) {
        for (j = TO2Y1; j <= TO2Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOasa*)Units [Oasa2]) -> Inventory [((TOasa*)Units [Oasa2]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TO3X1; i <= TO3X2; i++) {
        for (j = TO3Y1; j <= TO3Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOasa*)Units [Oasa3]) -> Inventory [((TOasa*)Units [Oasa3]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TO4X1; i <= TO4X2; i++) {
        for (j = TO4Y1; j <= TO4Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOasa*)Units [Oasa4]) -> Inventory [((TOasa*)Units [Oasa4]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TO5X1; i <= TO5X2; i++) {
        for (j = TO5Y1; j <= TO5Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOasa*)Units [Oasa5]) -> Inventory [((TOasa*)Units [Oasa5]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TO6X1; i <= TO6X2; i++) {
        for (j = TO6Y1; j <= TO6Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOasa*)Units [Oasa6]) -> Inventory [((TOasa*)Units [Oasa6]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       

    // Naplnime Neptuny
    for (i = TN1X1; i <= TN1X2; i++) {
        for (j = TN1Y1; j <= TN1Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TNeptun*)Units [Neptun1]) -> Inventory [((TNeptun*)Units [Neptun1]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TN2X1; i <= TN2X2; i++) {
        for (j = TN2Y1; j <= TN2Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TNeptun*)Units [Neptun2]) -> Inventory [((TNeptun*)Units [Neptun2]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       

    // Naplnime Laguny
    for (i = TL1X1; i <= TL1X2; i++) {
        for (j = TL1Y1; j <= TL1Y2; j++) {
            u = GetAircraftAt (i, j);
            if (u != NULL) {
                u -> PlaceGround (FALSE);               
                ((TLaguna*)Units [Laguna1]) -> Inventory [((TLaguna*)Units [Laguna1]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TL2X1; i <= TL2X2; i++) {
        for (j = TL2Y1; j <= TL2Y2; j++) {
            u = GetAircraftAt (i, j);
            if (u != NULL) {
                u -> PlaceGround (FALSE);               
                ((TLaguna*)Units [Laguna2]) -> Inventory [((TLaguna*)Units [Laguna2]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
        
    // Polozime miny    
    for (i = 0; MinePlaces17 [i].x != -1; i++) {
        MinePlace (MinePlaces17 [i].x, MinePlaces17 [i].y, BADLIFE);
    }
    
    // zamerime jednotku
    i = GetField (SelX, SelY) -> Unit;  
    Units [i] -> Select ();

    UnlockDraw ();          
    RedrawMap ();

}




void LoadArtificialIntelligence17(ReadStream &stream, int format) {
	DoneArtificialIntelligence17();

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
	Army7 = new TGroundArmy(stream);
	Marine1 = new TMarine(stream);
	Marine2 = new TMarine(stream);
	AirArmy1 = new TAirArmy(stream);
	AirArmy2 = new TAirArmy(stream);
	AirArmy3 = new TAirArmy(stream);
}

void SaveArtificialIntelligence17(WriteStream &stream) {
	saveAttackedFields(stream);

	Towers->Save(stream);
	Army1->Save(stream);
	Army2->Save(stream);
	Army3->Save(stream);
	Army4->Save(stream);
	Army5->Save(stream);
	Army6->Save(stream);
	Army7->Save(stream);
	Marine1->Save(stream);
	Marine2->Save(stream);
	AirArmy1->Save(stream);
	AirArmy2->Save(stream);
	AirArmy3->Save(stream);
	DUPos = 0;
}

int ArtificialIntelligence17 ()
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
    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;

    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
    Army7 -> DeleteKilled2 (); Army7 -> DeleteKilled ();
    Marine1 -> DeleteKilled2 (); Marine1 -> DeleteKilled ();
    Marine2 -> DeleteKilled2 (); Marine2 -> DeleteKilled ();
    AirArmy1 -> DeleteKilled2 (); AirArmy1 -> DeleteKilled ();
    AirArmy2 -> DeleteKilled2 (); AirArmy2 -> DeleteKilled ();
    AirArmy3 -> DeleteKilled2 (); AirArmy3 -> DeleteKilled ();
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
    Army7 -> DoAttackedFields ();
    Marine1 -> DoAttackedFields ();
    Marine2 -> DoAttackedFields ();
    AirArmy1 -> DoAttackedFields ();
    AirArmy2 -> DoAttackedFields ();
    AirArmy3 -> DoAttackedFields ();
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
    // Test zaktivovani armady 7
    if (Army7 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army7 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army7 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani Marine 1
    if (Marine1 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine1 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani Marine 2
    if (Marine2 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine2 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine2 -> Status = asActive;
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
    if (Army6 -> Status == asActive || Army6 -> Status == asSuicide) st += Army6 -> nofBadLife;
    if (Army7 -> Status == asActive || Army7 -> Status == asSuicide) st += Army7 -> nofBadLife;
    if (AirArmy1 -> Status == asActive || AirArmy1 -> Status == asSuicide) st += AirArmy1 -> nofBadLife;
    if (AirArmy2 -> Status == asActive || AirArmy2 -> Status == asSuicide) st += AirArmy2 -> nofBadLife;
    if (Marine1 -> Status == asActive || Marine1 -> Status == asSuicide) st += Marine1 -> nofBadLife;
    if (Marine2 -> Status == asActive || Marine2 -> Status == asSuicide) st += Marine2 -> nofBadLife;
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
    // ARMY 7
    if (Army7 -> Status == asActive || Army7 -> Status == asSuicide) {
        Army7 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army7 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army7 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army7 -> CountDangerous ();
        if (Army7 -> Dangerous <= CSuicide7) {
            Army7 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army7 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 7
        if (Army7 -> Dangerous <= CMinDang7) {
            Army7 -> Status = asDestroyed;
        }               
    }

    // Marine 1
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
        
        // Likvidace Marine 1
        if (Marine1 -> Dangerous <= CMinDangM1) {
            Marine1 -> Status = asDestroyed;
        }               
    }
    // Marine 2
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
        
        // Likvidace Marine 2
        if (Marine2 -> Dangerous <= CMinDangM2) {
            Marine2 -> Status = asDestroyed;
        }               
    }

    // AIR ARMY 1
    if (AirArmy1 -> Status == asActive || AirArmy1 -> Status == asSuicide) {
        AirArmy1 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (AirArmy1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy1 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        AirArmy1 -> CountDangerous ();
        if (AirArmy1 -> Dangerous <= CSuicideA1) {
            AirArmy1 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (AirArmy1 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 1
        if (AirArmy1 -> Dangerous <= CMinDangA1) {
            AirArmy1 -> Status = asDestroyed;
        }
                
    }
    // AIR ARMY 2
    if (AirArmy2 -> Status == asActive || AirArmy2 -> Status == asSuicide) {
        AirArmy2 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (AirArmy2 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy2 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        AirArmy2 -> CountDangerous ();
        if (AirArmy2 -> Dangerous <= CSuicideA2) {
            AirArmy2 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (AirArmy2 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 2
        if (AirArmy2 -> Dangerous <= CMinDangA2) {
            AirArmy2 -> Status = asDestroyed;
        }
                
    }
    // AIR ARMY 3
    if (AirArmy3 -> Status == asActive || AirArmy3 -> Status == asSuicide) {
        AirArmy3 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (AirArmy3 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy3 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        AirArmy3 -> CountDangerous ();
        if (AirArmy3 -> Dangerous <= CSuicideA3) {
            AirArmy3 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (AirArmy3 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 3
        if (AirArmy3 -> Dangerous <= CMinDangA3) {
            AirArmy3 -> Status = asDestroyed;
        }
                
    }



    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult17 ()
{
    int i, t = 0;

    Factory1 = GetField (F1X, F1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;
    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) t++;
    if (t == 0) return -1; // GoodLife prohral  

    if ((Base1 == NO_UNIT || Base1 < BADLIFE)
    && (Factory1 == NO_UNIT || Factory1 < BADLIFE)) return 1;

    
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits17 (int FactoryID)
{
    return Factory17List;
}



void DoneArtificialIntelligence17() {
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
	delete Army7;
	Army7 = NULL;
	delete Marine1;
	Marine1 = NULL;
	delete Marine2;
	Marine2 = NULL;
	delete AirArmy1;
	AirArmy1 = NULL;
	delete AirArmy2;
	AirArmy2 = NULL;
	delete AirArmy3;
	AirArmy3 = NULL;
	delete Towers;
	Towers = NULL;
}

    
    
// OK   
