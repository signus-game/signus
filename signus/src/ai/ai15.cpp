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
//           Mission 15
//
// Programmed by Richard Wunsch
//

#include "headers.h"
#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "aiair.h"
#include "aimarine.h"

// Action Radius of Army 1
#define A1X1 145
#define A1Y1 83
#define A1X2 173
#define A1Y2 104

// Action Radius of Army 2
#define A2X1 115
#define A2Y1 67
#define A2X2 146
#define A2Y2 86

// Action Radius of Army 3
#define A3X1 129
#define A3Y1 51
#define A3X2 150
#define A3Y2 168

// Action Radius of Army 4
#define A4X1 78
#define A4Y1 60
#define A4X2 109
#define A4Y2 93

// Action Radius of Marine 1
#define M1X1 128
#define M1Y1 33
#define M1X2 146
#define M1Y2 46

// Action Radius of Marine 2
#define M2X1  5
#define M2Y1 56
#define M2X2 51
#define M2Y2 109

// Action Radius of Air Army 1
#define AA1X1 66
#define AA1Y1 99
#define AA1X2 73
#define AA1Y2 105

// Action Radius of Air Army 2
#define AA2X1 66
#define AA2Y1 66
#define AA2X2 77
#define AA2Y2 77

// Oasa 1
#define O1X 48
#define O1Y 137
#define TO1X1 35
#define TO1Y1 136
#define TO1X2 42
#define TO1Y2 136

// Oasa 2
#define O2X 50
#define O2Y 136
#define TO2X1 35
#define TO2Y1 134
#define TO2X2 42
#define TO2Y2 134

// Oasa 3
#define O3X 48
#define O3Y 133
#define TO3X1 35
#define TO3Y1 132
#define TO3X2 42
#define TO3Y2 132

// Oasa 4
#define O4X 46
#define O4Y 131
#define TO4X1 35
#define TO4Y1 130
#define TO4X2 42
#define TO4Y2 130

// Oasa 5
#define O5X 52
#define O5Y 131
#define TO5X1 35
#define TO5Y1 128
#define TO5X2 42
#define TO5Y2 128

// Oasa 6
#define O6X 55
#define O6Y 134
#define TO6X1 35
#define TO6Y1 126
#define TO6X2 42
#define TO6Y2 126

// Neptun 1
#define N1X 43
#define N1Y 124
#define TN1X1 35
#define TN1Y1 124
#define TN1X2 42
#define TN1Y2 124

// Neptun 2
#define N2X 44
#define N2Y 138
#define TN2X1 35
#define TN2Y1 138
#define TN2X2 42
#define TN2Y2 138

// Base1 
#define B1X 88
#define B1Y 68

// Factory1 
#define F1X 80
#define F1Y 77

// Docks1 
#define D1X 134
#define D1Y 47

// 1st selected unit
#define SelX 57
#define SelY 132

#define Message1Turn 2

#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDangM1 0    // Dangerous limit for Marine 1
#define CMinDangM2 0    // Dangerous limit for Marine 2
#define CMinDangA1 0    // Dangerous limit for Air Army 1
#define CMinDangA2 0    // Dangerous limit for Air Army 2

#define CSuicide1  5    // Suicide limit for Army 1
#define CSuicide2  5    // Suicide limit for Army 2
#define CSuicide3  5    // Suicide limit for Army 3
#define CSuicide4  5    // Suicide limit for Army 4
#define CSuicideM1 8    // Suicide limit for Marine 1
#define CSuicideM2 8    // Suicide limit for Marine 2
#define CSuicideA1 8    // Suicide limit for Air Army 1
#define CSuicideA2 3    // Suicide limit for Air Army 2

TPoint MinePlaces15 [] = {148,90, 140,60, 134,61, 137,76, 100,75, 84,73, 
                                                    135,50,
                                                -1,-1};


int Factory15List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama, 
                                             unRex, unHerkules, unSkorpion, unPatriot, unGolias, unXenon,
                                             unCeres, unGnom, unIris, 0};
TPoint MBPlaces15 [] = {
                       -1,-1};

static void Message1 () 
{
    SaySpeech ("mis15a", 2005);
}


void InitAI15 ()
{
    int i,j;
    TBase *Base;
    TAirport *Airport;
    TField *f;
    TUnit *u;
    TPoint p;

    LockDraw ();        

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 0;
    MoneyBadlife = 10000;
    MBPlaces = MBPlaces15;  
    DeniedPlaces = NULL;

    Oasa1 = GetField (O1X, O1Y) -> Unit;    
    Oasa2 = GetField (O2X, O2Y) -> Unit;    
    Oasa3 = GetField (O3X, O3Y) -> Unit;    
    Oasa4 = GetField (O4X, O4Y) -> Unit;    
    Oasa5 = GetField (O5X, O5Y) -> Unit;        Oasa6 = GetField (O6X, O6Y) -> Unit;        Neptun1 = GetField (N1X, N1Y) -> Unit;      Neptun2 = GetField (N2X, N2Y) -> Unit;  
    Factory1 = GetField (F1X, F1Y) -> Unit; 
    Base1 = GetField (B1X, B1Y) -> Unit;    
    Docks1 = GetField (D1X, D1Y) -> Unit;   
    
    

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

    Marine1 = new TMarine ();
    Marine1 -> SetAR (M1X1, M1Y1, M1X2, M1Y2);
    Marine1 -> SetRadar (-1, -1);
    Marine1 -> InsertUnits ();

    Marine2 = new TMarine ();
    Marine2 -> SetAR (M2X1, M2Y1, M2X2, M2Y2);
    Marine2 -> SetRadar (-1, -1);
    Marine2 -> InsertUnits ();

    AirArmy1 = new TAirArmy();
    AirArmy1 -> SetAR (AA1X1, AA1Y1, AA1X2, AA1Y2);
    AirArmy1 -> SetRadar (-1, -1);
    AirArmy1 -> InsertUnits ();
    AirArmy1 -> Status = asActive;
    AirArmy1 -> Insert (Oasa1);

    AirArmy2 = new TAirArmy();
    AirArmy2 -> SetAR (AA2X1, AA2Y1, AA2X2, AA2Y2);
    AirArmy2 -> SetRadar (-1, -1);
    AirArmy2 -> InsertUnits ();
    AirArmy2 -> Status = asActive;

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

        
    // Polozime miny    
    for (i = 0; MinePlaces15 [i].x != -1; i++) {
        MinePlace (MinePlaces15 [i].x, MinePlaces15 [i].y, BADLIFE);
    }
    UnlockDraw ();          
    
    // zamerime jednotku
    i = GetField (SelX, SelY) -> Unit;  
    Units [i] -> Select ();

    RedrawMap ();

}




void LoadArtificialIntelligence15 (FILE *f)
{

    DoneArtificialIntelligence15 ();

    MBPlaces = MBPlaces15;  
    DeniedPlaces = NULL;

    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);
    DUPos = 0;

    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
    Army4 = new TGroundArmy (f);
    Marine1 = new TMarine (f);
    Marine2 = new TMarine (f);
    AirArmy1 = new TAirArmy (f);
    AirArmy2 = new TAirArmy (f);
}



void SaveArtificialIntelligence15 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);

    Towers -> Save (f);
    Army1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);      
    Army4 -> Save (f);
    Marine1 -> Save (f);
    Marine2 -> Save (f);
    AirArmy1 -> Save (f);
    AirArmy2 -> Save (f);
    DUPos = 0;
}


int ArtificialIntelligence15 ()
{
    int i, j, k, r, st, x, y, id, h, hmax;
    TPoint p;
    char buf [40];
    TBase *Base;
    TField *f;

    Message (SigText[TXT_AI_ANALYSE]);

    // Prepocitame viditelnost
    AllowBadlife (TRUE);
    ComputeVisib ();
    RedrawMap ();
    
    LockDraw ();

    if (ActualTurn == Message1Turn) Message1 ();
    
    // Aktualizujeme specialni objekty
    Factory1 = GetField (F1X, F1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;
    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    Docks1 = GetField (D1X, D1Y) -> Unit;
    if (Docks1 == NO_UNIT || Units [Docks1] -> Type % BADLIFE != unDocks) 
        Docks1 = NO_UNIT;

    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Marine1 -> DeleteKilled2 (); Marine1 -> DeleteKilled ();
    Marine2 -> DeleteKilled2 (); Marine2 -> DeleteKilled ();
    AirArmy1 -> DeleteKilled2 (); AirArmy1 -> DeleteKilled ();
    AirArmy2 -> DeleteKilled2 (); AirArmy2 -> DeleteKilled ();
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
    Marine1 -> DoAttackedFields ();
    Marine2 -> DoAttackedFields ();
    AirArmy1 -> DoAttackedFields ();
    AirArmy2 -> DoAttackedFields ();
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
    if (Marine1 -> Status == asActive || Marine1 -> Status == asSuicide) st += Marine1 -> nofBadLife;
    if (Marine2 -> Status == asActive || Marine2 -> Status == asSuicide) st += Marine2 -> nofBadLife;
    if (AirArmy1 -> Status == asActive || AirArmy1 -> Status == asSuicide) st += AirArmy1 -> nofBadLife;
    if (AirArmy2 -> Status == asActive || AirArmy2 -> Status == asSuicide) st += AirArmy2 -> nofBadLife;
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

    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult15 ()
{
    int i, t = 0;

    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    Docks1 = GetField (D1X, D1Y) -> Unit;
    if (Docks1 == NO_UNIT || Units [Docks1] -> Type % BADLIFE != unDocks) 
        Docks1 = NO_UNIT;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if ((Base1 == NO_UNIT || Base1 < BADLIFE)
    && (Docks1 == NO_UNIT || Docks1 < BADLIFE)) return 1;
    
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits15 (int FactoryID)
{
    return Factory15List;
}



void DoneArtificialIntelligence15 ()
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
    if (Army4 != NULL) {delete Army4; Army4 = NULL; }
    if (Marine1 != NULL) {delete Marine1; Marine1 = NULL; }
    if (Marine2 != NULL) {delete Marine2; Marine2 = NULL; }
    if (AirArmy1 != NULL) {delete AirArmy1; AirArmy1 = NULL; }
    if (AirArmy2 != NULL) {delete AirArmy2; AirArmy2 = NULL; }
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
// OK   
