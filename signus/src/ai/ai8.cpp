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
//           Mission 8
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
#define A1X1 68
#define A1Y1 151
#define A1X2 92
#define A1Y2 174

// Action Radius of Army 2
#define A2X1 57
#define A2Y1 138
#define A2X2 85
#define A2Y2 150

// Action Radius of Army 3
#define A3X1 75
#define A3Y1 82
#define A3X2 96
#define A3Y2 95

// Action Radius of Air Army 3
#define AA3X1 73
#define AA3Y1 61
#define AA3X2 84
#define AA3Y2 70


// Action Radius of Army 4
#define A4X1 82
#define A4Y1 67
#define A4X2 91
#define A4Y2 76

// Action Radius of Army 5
#define A5X1 86
#define A5Y1 29
#define A5X2 104
#define A5Y2 46

// Action Radius of Army 6
#define A6X1 14
#define A6Y1 78
#define A6X2 33
#define A6Y2 108

// Action Radius of Army 7
#define A7X1 13
#define A7Y1 40
#define A7X2 35
#define A7Y2 64

// Action Radius of Army 8
#define A8X1 10
#define A8Y1  0
#define A8X2 37
#define A8Y2 39

// Action Radius of Air Army 8
#define AA8X1 38
#define AA8Y1 19
#define AA8X2 48
#define AA8Y2 28

// Action Radius of Marine 1
#define M1X1  3
#define M1Y1 80
#define M1X2  9
#define M1Y2 88

// Action Radius of Marine 8
#define M8X1  0
#define M8Y1  0
#define M8X2 11
#define M8Y2 25

// Docks 1
#define D1X 12
#define D1Y 84
#define TD1X1  7
#define TD1Y1 91
#define TD1X2 14
#define TD1Y2 98

// Docks 2
#define D2X 12
#define D2Y 19

// Factory 1
#define F1X 98
#define F1Y 187

// Factory 2
#define F2X 23
#define F2Y 20

// Factory 3
#define F3X 21
#define F3Y 77

// Radar 2
#define R2X 67
#define R2Y 147

// Radar 3
#define R3X 91
#define R3Y 90

// Radar 4
#define R4X 87
#define R4Y 68

// Radar 6
#define R6X 24
#define R6Y 99

// Radar 7
#define R7X 18
#define R7Y 55

// 1st Selected unit
#define SelX 94
#define SelY 186





#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDangA3 0    // Dangerous limit for AirArmy 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6
#define CMinDang7  0    // Dangerous limit for Army 7
#define CMinDang8  0    // Dangerous limit for Army 8
#define CMinDangA8 0    // Dangerous limit for AirArmy 8
#define CMinDangM1 0    // Dangerous limit for Marine 1
#define CMinDangM8 0    // Dangerous limit for Marine 8

#define CSuicide1  5    // Suicide limit for Army 1
#define CSuicide2  5    // Suicide limit for Army 2
#define CSuicide3  5    // Suicide limit for Army 3
#define CSuicideA3 5    // Suicide limit for AirArmy 3
#define CSuicide4  5    // Suicide limit for Army 4
#define CSuicide5  5    // Suicide limit for Army 5
#define CSuicide6  5    // Suicide limit for Army 6
#define CSuicide7  5    // Suicide limit for Army 7
#define CSuicide8  5    // Suicide limit for Army 8
#define CSuicideA8 5    // Suicide limit for AirArmy 8
#define CSuicideM1 8    // Suicide limit for Marine 1
#define CSuicideM8 8    // Suicide limit for Marine 8

TPoint MinePlaces8 [] = {39,178, 52,175, 78,162, 79,166, 73,140, 76,137,
                                                 61,130, 76,109, 81,102, 79,99, 85,86, 89,86, 82,83,
                                                 86,69, 97,40, 36,118, 28,112, 33,113, 22,89, 18,89,
                                                 18,87, 18,85, 18,83, 20,84, 20,86, 20,88, 25,47,
                                                 25,44, 21,46, 19,24, 28,30, 22,16,
                                                -1,-1};

int Factory8List1 [] = {unBumerang, unPerseusAlfa, unPerseusBeta, 
                                             unRex, unHerkules, unSkorpion, unPatriot, 
                                             unGolias, unXenon, unCeres, unGnom, 0};

int Factory8List2 [] = {unPerseusAlfa, unPerseusBeta, 
                                             unRex, unHerkules, unSkorpion, unPatriot, 
                                             unFalanga, unXenon, unMedea, 0};

int Factory8List3 [] = {unBumerang, unPerseusAlfa, unPerseusBeta, 
                                             unRex, unHerkules, unSkorpion, unPatriot, 
                                             unFalanga, unXenon, unCeres, unGnom, unMedea, 0};


void InitAI8 ()
{
    int i,j;
    TField *f;
    TUnit *u;

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 0;
    MoneyBadlife = 10000;
    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    Docks1 = GetField (D1X, D1Y) -> Unit;   
    Docks2 = GetField (D2X, D2Y) -> Unit;   
    Factory1 = GetField (F1X, F1Y) -> Unit;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    Factory3 = GetField (F3X, F3Y) -> Unit;
        
    Towers = new TTowers ();
    for (i = 0; i < UNITS_TOP; i++) {
        if (Units [i] == NULL) continue;
        // vsechny veze a GL
        if ((((TUnit *) Units [i]) -> Type >= (unHorizont + BADLIFE) 
        && ((TUnit *) Units [i]) -> Type  <= (unSpektrum + BADLIFE))
        || (((TUnit *) Units[i]) -> Type < BADLIFE)) 
            Towers -> Insert (i);
    }
    

    LockDraw ();        
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

    AirArmy3 = new TAirArmy();
    AirArmy3 -> SetAR (AA3X1, AA3Y1, AA3X2, AA3Y2);
    AirArmy3 -> SetRadar (-1, -1);
    AirArmy3 -> InsertUnits ();
    AirArmy3 -> Status = asActive;

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

    Army7 = new TGroundArmy();
    Army7 -> SetAR (A7X1, A7Y1, A7X2, A7Y2);
    Army7 -> SetRadar (-1, -1);
    Army7 -> InsertUnits ();

    Army8 = new TGroundArmy();
    Army8 -> SetAR (A8X1, A8Y1, A8X2, A8Y2);
    Army8 -> SetRadar (-1, -1);
    Army8 -> InsertUnits ();

    AirArmy8 = new TAirArmy();
    AirArmy8 -> SetAR (AA8X1, AA8Y1, AA8X2, AA8Y2);
    AirArmy8 -> SetRadar (-1, -1);
    AirArmy8 -> InsertUnits ();
    AirArmy8 -> Status = asActive;
    
    Marine1 = new TMarine();
    Marine1 -> SetAR (M1X1, M1Y1, M1X2, M1Y2);
    Marine1 -> SetRadar (-1, -1);
    Marine1 -> InsertUnits ();

    Marine8 = new TMarine();
    Marine8 -> SetAR (M8X1, M8Y1, M8X2, M8Y2);
    Marine8 -> SetRadar (-1, -1);
    Marine8 -> InsertUnits ();


    // Naplnime Docks 1
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
    for (i = 0; MinePlaces8 [i].x != -1; i++) {
        MinePlace (MinePlaces8 [i].x, MinePlaces8 [i].y, BADLIFE);
    }
    UnlockDraw ();          

    // zamerime jednotku
    i = GetField (SelX, SelY) -> Unit;  
    Units [i] -> Select ();

    RedrawMap ();
    
}




void LoadArtificialIntelligence8 (FILE *f)
{

    DoneArtificialIntelligence8 ();

    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);
    DUPos = 0;

    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
    AirArmy3 = new TAirArmy (f);
    Army4 = new TGroundArmy (f);
    Army5 = new TGroundArmy (f);
    Army6 = new TGroundArmy (f);
    Army7 = new TGroundArmy (f);
    Army8 = new TGroundArmy (f);
    AirArmy8 = new TAirArmy (f);
    Marine1 = new TMarine (f);
    Marine8 = new TMarine (f);
}



void SaveArtificialIntelligence8 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);
    
    Towers -> Save (f);
    Army1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);      
    AirArmy3 -> Save (f);
    Army4 -> Save (f);
    Army5 -> Save (f);
    Army6 -> Save (f);
    Army7 -> Save (f);
    Army8 -> Save (f);
    AirArmy8 -> Save (f);
    Marine1 -> Save (f);
    Marine8 -> Save (f);
    DUPos = 0;
}


int ArtificialIntelligence8 ()
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
    Docks1 = GetField (D1X, D1Y) -> Unit;
    if (Docks1 == NO_UNIT || Units [Docks1] -> Type % BADLIFE != unDocks) 
        Docks1 = NO_UNIT;
    Docks2 = GetField (D2X, D2Y) -> Unit;
    if (Docks2 == NO_UNIT || Units [Docks2] -> Type % BADLIFE != unDocks) 
        Docks2 = NO_UNIT;
    Factory1 = GetField (F1X, F1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    if (Factory2 == NO_UNIT || Units [Factory2] -> Type % BADLIFE != unFactory) 
        Factory2 = NO_UNIT;
    Factory3 = GetField (F3X, F3Y) -> Unit;
    if (Factory3 == NO_UNIT || Units [Factory3] -> Type % BADLIFE != unFactory) 
        Factory3 = NO_UNIT;

    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    AirArmy3 -> DeleteKilled2 (); AirArmy3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
    Army7 -> DeleteKilled2 (); Army7 -> DeleteKilled ();
    Army8 -> DeleteKilled2 (); Army8 -> DeleteKilled ();
    AirArmy8 -> DeleteKilled2 (); AirArmy8 -> DeleteKilled ();
    Marine1 -> DeleteKilled2 (); Marine1 -> DeleteKilled ();
    Marine8 -> DeleteKilled2 (); Marine8 -> DeleteKilled ();
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
    AirArmy3 -> DoAttackedFields ();
    Army4 -> DoAttackedFields ();
    Army5 -> DoAttackedFields ();
    Army6 -> DoAttackedFields ();
    Army7 -> DoAttackedFields ();
    Army8 -> DoAttackedFields ();
    AirArmy8 -> DoAttackedFields ();
    Marine1 -> DoAttackedFields ();
    Marine8 -> DoAttackedFields ();
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
    // Test zaktivovani armady 8
    if (Army8 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army8 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army8 -> Status = asActive;
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
    // Test zaktivovani marine 8
    if (Marine8 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine8 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine8 -> Status = asActive;
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
    if (AirArmy3 -> Status == asActive || AirArmy3 -> Status == asSuicide) st += AirArmy3 -> nofBadLife;
    if (Army4 -> Status == asActive || Army4 -> Status == asSuicide) st += Army4 -> nofBadLife;
    if (Army5 -> Status == asActive || Army5 -> Status == asSuicide) st += Army5 -> nofBadLife;
    if (Army6 -> Status == asActive || Army6 -> Status == asSuicide) st += Army6 -> nofBadLife;
    if (Army7 -> Status == asActive || Army7 -> Status == asSuicide) st += Army7 -> nofBadLife;
    if (Army8 -> Status == asActive || Army8 -> Status == asSuicide) st += Army8 -> nofBadLife;
    if (AirArmy8 -> Status == asActive || AirArmy8 -> Status == asSuicide) st += AirArmy8 -> nofBadLife;
    if (Marine1 -> Status == asActive || Marine1 -> Status == asSuicide) st += Marine1 -> nofBadLife;
    if (Marine8 -> Status == asActive || Marine8 -> Status == asSuicide) st += Marine8 -> nofBadLife;
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

    // ARMY 8
    if (Army8 -> Status == asActive || Army8 -> Status == asSuicide) {
        Army8 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army8 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army8 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army8 -> CountDangerous ();
        if (Army8 -> Dangerous <= CSuicide8) {
            Army8 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army8 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 6
        if (Army8 -> Dangerous <= CMinDang8) {
            Army8 -> Status = asDestroyed;
        }               
    }

    // AIR ARMY 8
    if (AirArmy8 -> Status == asActive || AirArmy8 -> Status == asSuicide) {
        AirArmy8 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (AirArmy8 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy8 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        AirArmy8 -> CountDangerous ();
        if (AirArmy8 -> Dangerous <= CSuicideA8) {
            AirArmy8 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (AirArmy8 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 6
        if (AirArmy8 -> Dangerous <= CMinDangA8) {
            AirArmy8 -> Status = asDestroyed;
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

    // MARINE 8
    if (Marine8 -> Status == asActive || Marine8 -> Status == asSuicide) {
        Marine8 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Marine8 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine8 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Marine8 -> CountDangerous ();
        if (Marine8 -> Dangerous <= CSuicideM8) {
            Marine8 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Marine8 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 8
        if (Marine8 -> Dangerous <= CMinDangM8) {
            Marine8 -> Status = asDestroyed;
        }               
    }



    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult8 ()
{
    int i, t = 0;

    Docks2 = GetField (D2X, D2Y) -> Unit;
    if (Docks2 == NO_UNIT || Units [Docks2] -> Type % BADLIFE != unDocks) 
        Docks2 = NO_UNIT;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if (Docks2 == NO_UNIT || Docks2 < BADLIFE) return 1; // BadLife prohral

    
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits8 (int FactoryID)
{
    Factory1 = GetField (F1X, F1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    if (Factory2 == NO_UNIT || Units [Factory2] -> Type % BADLIFE != unFactory) 
        Factory2 = NO_UNIT;
    Factory3 = GetField (F3X, F3Y) -> Unit;
    if (Factory3 == NO_UNIT || Units [Factory3] -> Type % BADLIFE != unFactory) 
        Factory3 = NO_UNIT;
    
    if (FactoryID == Factory1) return Factory8List1;
    if (FactoryID == Factory2) return Factory8List2;
    if (FactoryID == Factory3) return Factory8List3;
    return Factory8List1;
    
}



void DoneArtificialIntelligence8 ()
{
    // Likvidace existujicich tabulek
    if (UnAnalyse != NULL) {
        for (int i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (UnAnalyse [i].w0 != NULL) memfree (UnAnalyse [i].w0);
            if (UnAnalyse [i].w1 != NULL) memfree (UnAnalyse [i].w1);
            if (UnAnalyse [i].w2 != NULL) memfree (UnAnalyse [i].w2);
            if (UnAnalyse [i].w3 != NULL) memfree (UnAnalyse [i].w3);
        }
        memfree (UnAnalyse);
        UnAnalyse = NULL;
    }
    
    if (Army1 != NULL) {delete Army1; Army1 = NULL; }
    if (Army2 != NULL) {delete Army2; Army2 = NULL; }
    if (Army3 != NULL) {delete Army3; Army3 = NULL; }
    if (AirArmy3 != NULL) {delete AirArmy3; AirArmy3 = NULL; }
    if (Army4 != NULL) {delete Army4; Army4 = NULL; }
    if (Army5 != NULL) {delete Army5; Army5 = NULL; }
    if (Army6 != NULL) {delete Army6; Army6 = NULL; }
    if (Army7 != NULL) {delete Army7; Army7 = NULL; }
    if (Army8 != NULL) {delete Army8; Army8 = NULL; }
    if (AirArmy8 != NULL) {delete AirArmy8; AirArmy8 = NULL; }
    if (Marine1 != NULL) {delete Marine1; Marine1 = NULL; }
    if (Marine8 != NULL) {delete Marine8; Marine8 = NULL; }
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
    
// OK
