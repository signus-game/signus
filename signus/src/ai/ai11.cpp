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
//           Mission 11
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
#define A1X1 21
#define A1Y1 111
#define A1X2 70
#define A1Y2 137

// Action Radius of Marine 1
#define M1X1 54
#define M1Y1 130
#define M1X2 70
#define M1Y2 151

// Action Radius of Army 2
#define A2X1  5
#define A2Y1 103
#define A2X2 18
#define A2Y2 115

// Action Radius of Army 3
#define A3X1 69
#define A3Y1 108
#define A3X2 82
#define A3Y2 122

// Action Radius of Army 4
#define A4X1 18
#define A4Y1 87
#define A4X2 49
#define A4Y2 102

// Action Radius of Air Army 4
#define AA4X1 25
#define AA4Y1 79
#define AA4X2 42
#define AA4Y2 86

// Action Radius of Army 5
#define A5X1 69
#define A5Y1 74
#define A5X2 95
#define A5Y2 93

// Action Radius of Army 6
#define A6X1 68
#define A6Y1 59
#define A6X2 85
#define A6Y2 73

// Action Radius of Army 7
#define A7X1 44
#define A7Y1 47
#define A7X2 63
#define A7Y2 63

// Action Radius of Army 8
#define A8X1 13
#define A8Y1 35
#define A8X2 41
#define A8Y2 50

// Action Radius of Army 9
#define A9X1  0
#define A9Y1 41
#define A9X2 16
#define A9Y2 63

// Action Radius of Army 10
#define A10X1 30
#define A10Y1  0
#define A10X2 74
#define A10Y2 38

// Radar 2
#define R2X 17
#define R2Y 114

// Radar 5
#define R5X 71
#define R5Y 71

// Radar 9
#define R9X  8
#define R9Y 59

// Radar 10
#define R10X 56
#define R10Y 28

// Airport 1
#define A1X 28
#define A1Y 86

// Airport 2
#define A2X 34
#define A2Y 82

// Airport 3
#define A3X 39
#define A3Y 89

// Base 1
#define B1X 52
#define B1Y 12

// Factory 1
#define F1X 88
#define F1Y 58

// Laguna 1
#define L1X 23
#define L1Y 154
#define TL1X1 20
#define TL1Y1 151
#define TL1X2 22
#define TL1Y2 162

// Laguna 2
#define L2X 30
#define L2Y 153
#define TL2X1 31
#define TL2Y1 152
#define TL2X2 33
#define TL2Y2 159

// Laguna 3
#define L3X 37
#define L3Y 157
#define TL3X1 38
#define TL3Y1 155
#define TL3X2 40
#define TL3Y2 164

// Oasa 1
#define O1X 20
#define O1Y 158
#define TO1X1  0
#define TO1Y1 122
#define TO1X2  2
#define TO1Y2 126

// Oasa 2
#define O2X 27
#define O2Y 157
#define TO2X1  4
#define TO2Y1 122
#define TO2X2  6
#define TO2Y2 126

// Oasa 3
#define O3X 34
#define O3Y 160
#define TO3X1  8
#define TO3Y1 122
#define TO3X2 10
#define TO3Y2 126

// Oasa 4
#define O4X 24
#define O4Y 161
#define TO4X1 12
#define TO4Y1 122
#define TO4X2 14
#define TO4Y2 126

// Oasa 5
#define O5X 29
#define O5Y 163
#define TO5X1 19
#define TO5Y1 122
#define TO5X2 21
#define TO5Y2 126

// Oasa 6
#define O6X 30
#define O6Y 160
#define TO6X1  0
#define TO6Y1 130
#define TO6X2  2
#define TO6Y2 132

// Oasa 7
#define O7X 37
#define O7Y 164
#define TO7X1  4
#define TO7Y1 130 
#define TO7X2  6
#define TO7Y2 132

// 1st selected unit
#define SelX 30
#define SelY 153


#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDangM1 0    // Dangerous limit for Army 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDangA4 0    // Dangerous limit for AirArmy 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6
#define CMinDang7  0    // Dangerous limit for Army 7
#define CMinDang8  0    // Dangerous limit for Army 8
#define CMinDang9  0    // Dangerous limit for Army 9
#define CMinDang10 0    // Dangerous limit for Army 10

#define CSuicide1  5    // Suicide limit for Army 1
#define CSuicideM1 8    // Suicide limit for Army 1
#define CSuicide2  5    // Suicide limit for Army 2
#define CSuicide3  5    // Suicide limit for Army 3
#define CSuicide4  5    // Suicide limit for Army 4
#define CSuicideA4 5    // Suicide limit for AirArmy 4
#define CSuicide5  5    // Suicide limit for Army 5
#define CSuicide6  5    // Suicide limit for Army 6
#define CSuicide7  5    // Suicide limit for Army 7
#define CSuicide8  5    // Suicide limit for Army 8
#define CSuicide9  5    // Suicide limit for Army 9
#define CSuicide10 5    // Suicide limit for Army 10

TPoint MinePlaces11 [] = {10,122, 22,125, 40,119, 33,91, 61,84, 84,65,
                                                    89,61, 6,48, 5,54, 56,23, 47,20, 54,15, 43,25,
                                                    -1,-1};

int Factory11List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama,
                                             unHerkules, unSkorpion, unPatriot, unGolias,
                                             unXenon, unFalanga, unRex, unCeres, unGnom, 0};

static int MS1Done = FALSE;

static void Message1 () 
{
    SaySpeech ("mis11a", 2005);
}


void InitAI11 ()
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

    Airport1 = GetField (A1X, A1Y) -> Unit; 
    Airport2 = GetField (A2X, A2Y) -> Unit; 
    Airport3 = GetField (A3X, A3Y) -> Unit; 
    Base1 = GetField (B1X, B1Y) -> Unit;    
    Factory1 = GetField (F1X, F1Y) -> Unit; 
    
    Laguna1 = GetField (L1X, L1Y) -> Unit;
    Laguna2 = GetField (L2X, L2Y) -> Unit;
    Laguna3 = GetField (L3X, L3Y) -> Unit;
    Oasa1 = GetField (O1X, O1Y) -> Unit;    
    Oasa2 = GetField (O2X, O2Y) -> Unit;    
    Oasa3 = GetField (O3X, O3Y) -> Unit;    
    Oasa4 = GetField (O4X, O4Y) -> Unit;    
    Oasa5 = GetField (O5X, O5Y) -> Unit;        Oasa6 = GetField (O6X, O6Y) -> Unit;        Oasa7 = GetField (O7X, O7Y) -> Unit;    
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
    Army1 -> SetAIConst (2);

    Marine1 = new TMarine();
    Marine1 -> SetAR (M1X1, M1Y1, M1X2, M1Y2);
    Marine1 -> SetRadar (-1, -1);
    Marine1 -> InsertUnits ();

    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> SetRadar (R2X, R2Y);
    Army2 -> InsertUnits ();
    Army2 -> SetAIConst (2);
    
    Army3 = new TGroundArmy();
    Army3 -> SetAR (A3X1, A3Y1, A3X2, A3Y2);
    Army3 -> SetRadar (-1, -1);
    Army3 -> InsertUnits ();
    Army3 -> SetAIConst (2);

    Army4 = new TGroundArmy();
    Army4 -> SetAR (A4X1, A4Y1, A4X2, A4Y2);
    Army4 -> SetRadar (-1, -1);
    Army4 -> InsertUnits ();

    AirArmy4 = new TAirArmy();
    AirArmy4 -> SetAR (AA4X1, AA4Y1, AA4X2, AA4Y2);
    AirArmy4 -> SetRadar (-1, -1);
    AirArmy4 -> InsertUnits ();
    AirArmy4 -> Status = asActive;
    
    Army5 = new TGroundArmy();
    Army5 -> SetAR (A5X1, A5Y1, A5X2, A5Y2);
    Army5 -> SetRadar (R5X, R5Y);
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

    Army9 = new TGroundArmy();
    Army9 -> SetAR (A9X1, A9Y1, A9X2, A9Y2);
    Army9 -> SetRadar (R9X, R9Y);
    Army9 -> InsertUnits ();

    Army10 = new TGroundArmy();
    Army10 -> SetAR (A10X1, A10Y1, A10X2, A10Y2);
    Army10 -> SetRadar (R10X, R10Y);
    Army10 -> InsertUnits ();

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
    for (i = TL3X1; i <= TL3X2; i++) {
        for (j = TL3Y1; j <= TL3Y2; j++) {
            u = GetAircraftAt (i, j);
            if (u != NULL) {
                u -> PlaceGround (FALSE);               
                ((TLaguna*)Units [Laguna3]) -> Inventory [((TLaguna*)Units [Laguna3]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    
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
    for (i = TO7X1; i <= TO7X2; i++) {
        for (j = TO7Y1; j <= TO7Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOasa*)Units [Oasa7]) -> Inventory [((TOasa*)Units [Oasa7]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    
    // Polozime miny
    
    for (i = 0; MinePlaces11 [i].x != -1; i++) {
        MinePlace (MinePlaces11 [i].x, MinePlaces11 [i].y, BADLIFE);
    }

    // zamerime jednotku
    i = GetField (SelX, SelY) -> Unit;  
    Units [i] -> Select ();
    Units [i] -> Center ();


    UnlockDraw ();          
    
    RedrawMap ();

}




void LoadArtificialIntelligence11 (FILE *f)
{

    DoneArtificialIntelligence11 ();

    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);
    DUPos = 0;
    
    fread (&Laguna1, sizeof (int), 1, f);
    fread (&Laguna2, sizeof (int), 1, f);
    fread (&Laguna3, sizeof (int), 1, f);
    fread (&Oasa1, sizeof (int), 1, f);
    fread (&Oasa2, sizeof (int), 1, f);
    fread (&Oasa3, sizeof (int), 1, f);
    fread (&Oasa4, sizeof (int), 1, f);
    fread (&Oasa5, sizeof (int), 1, f);
    fread (&Oasa6, sizeof (int), 1, f);
    fread (&Oasa7, sizeof (int), 1, f);

    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Marine1 = new TMarine (f);
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
    Army4 = new TGroundArmy (f);
    AirArmy4 = new TAirArmy (f);    
    Army5 = new TGroundArmy (f);
    Army6 = new TGroundArmy (f);
    Army7 = new TGroundArmy (f);
    Army8 = new TGroundArmy (f);
    Army9 = new TGroundArmy (f);
    Army10 = new TGroundArmy (f);
    fread (&MS1Done, sizeof (int), 1, f);
}



void SaveArtificialIntelligence11 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);

    fwrite (&Laguna1, sizeof (int), 1, f);
    fwrite (&Laguna2, sizeof (int), 1, f);
    fwrite (&Laguna3, sizeof (int), 1, f);
    fwrite (&Oasa1, sizeof (int), 1, f);
    fwrite (&Oasa2, sizeof (int), 1, f);
    fwrite (&Oasa3, sizeof (int), 1, f);
    fwrite (&Oasa4, sizeof (int), 1, f);
    fwrite (&Oasa5, sizeof (int), 1, f);
    fwrite (&Oasa6, sizeof (int), 1, f);
    fwrite (&Oasa7, sizeof (int), 1, f);
    
    Towers -> Save (f);
    Army1 -> Save (f);
    Marine1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);      
    Army4 -> Save (f);
    AirArmy4 -> Save (f);
    Army5 -> Save (f);
    Army6 -> Save (f);
    Army7 -> Save (f);
    Army8 -> Save (f);
    Army9 -> Save (f);
    Army10 -> Save (f);
    DUPos = 0;
    fwrite (&MS1Done, sizeof (int), 1, f);
}


int ArtificialIntelligence11 ()
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
    Airport1 = GetField (A1X, A1Y) -> Unit;
    if (Airport1 == NO_UNIT || Units [Airport1] -> Type % BADLIFE != unAirport) 
        Airport1 = NO_UNIT;
    Airport2 = GetField (A2X, A2Y) -> Unit;
    if (Airport2 == NO_UNIT || Units [Airport2] -> Type % BADLIFE != unAirport) 
        Airport2 = NO_UNIT;
    Airport3 = GetField (A3X, A3Y) -> Unit;
    if (Airport3 == NO_UNIT || Units [Airport3] -> Type % BADLIFE != unAirport) 
        Airport3 = NO_UNIT;
    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    Factory1 = GetField (B1X, B1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;

    if ((!MS1Done) && ((Airport1 == NO_UNIT || Airport1 < BADLIFE) 
    || (Airport2 == NO_UNIT || Airport2 < BADLIFE) 
    || (Airport3 == NO_UNIT || Airport3 < BADLIFE))) {
        Message1 ();
        MS1Done = TRUE;
    }


    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Marine1 -> DeleteKilled2 (); Marine1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    AirArmy4 -> DeleteKilled2 (); AirArmy4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
    Army7 -> DeleteKilled2 (); Army7 -> DeleteKilled ();
    Army8 -> DeleteKilled2 (); Army8 -> DeleteKilled ();
    Army9 -> DeleteKilled2 (); Army9 -> DeleteKilled ();
    Army10 -> DeleteKilled2 (); Army10 -> DeleteKilled ();
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
    Marine1 -> DoAttackedFields ();
    Army2 -> DoAttackedFields ();
    Army3 -> DoAttackedFields ();
    Army4 -> DoAttackedFields ();
    AirArmy4 -> DoAttackedFields ();
    Army5 -> DoAttackedFields ();
    Army6 -> DoAttackedFields ();
    Army7 -> DoAttackedFields ();
    Army8 -> DoAttackedFields ();
    Army9 -> DoAttackedFields ();
    Army10 -> DoAttackedFields ();
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
    // Test zaktivovani marine 1
    if (Marine1 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine1 -> Status = asActive;
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
    // Test zaktivovani air armady 4
    if (AirArmy4 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (AirArmy4 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy4 -> Status = asActive;
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
    // Test zaktivovani armady 9
    if (Army9 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army9 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army9 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 10
    if (Army10 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army10 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army10 -> Status = asActive;
            }
        }
    }
    

///////////////////////////// AKCE ARMAD

    // Nastavime ProgressBar
    st = 0;
    st += Towers -> nofBadLife;
    if (Army1 -> Status == asActive || Army1 -> Status == asSuicide) st += Army1 -> nofBadLife;
    if (Marine1 -> Status == asActive || Marine1 -> Status == asSuicide) st += Marine1 -> nofBadLife;
    if (Army2 -> Status == asActive || Army2 -> Status == asSuicide) st += Army2 -> nofBadLife;
    if (Army3 -> Status == asActive || Army3 -> Status == asSuicide) st += Army3 -> nofBadLife;
    if (Army4 -> Status == asActive || Army4 -> Status == asSuicide) st += Army4 -> nofBadLife;
    if (AirArmy4 -> Status == asActive || AirArmy4 -> Status == asSuicide) st += AirArmy4 -> nofBadLife;
    if (Army5 -> Status == asActive || Army5 -> Status == asSuicide) st += Army5 -> nofBadLife;
    if (Army6 -> Status == asActive || Army6 -> Status == asSuicide) st += Army6 -> nofBadLife;
    if (Army7 -> Status == asActive || Army7 -> Status == asSuicide) st += Army7 -> nofBadLife;
    if (Army8 -> Status == asActive || Army8 -> Status == asSuicide) st += Army8 -> nofBadLife;
    if (Army9 -> Status == asActive || Army9 -> Status == asSuicide) st += Army9 -> nofBadLife;
    if (Army10 -> Status == asActive || Army10 -> Status == asSuicide) st += Army10 -> nofBadLife;
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
    
    // AIR ARMY 4
    if (AirArmy4 -> Status == asActive || AirArmy4 -> Status == asSuicide) {
        AirArmy4 -> DeleteKilled ();
        
        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (AirArmy4 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy4 -> Insert (i);
            }
        }

        // Pokyn k sebevrazde       
        AirArmy4 -> CountDangerous ();
        if (AirArmy4 -> Dangerous <= CSuicideA4) {
            AirArmy4 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (AirArmy4 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 4
        if (AirArmy4 -> Dangerous <= CMinDangA4) {
            AirArmy4 -> Status = asDestroyed;
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
        
        // Likvidace armady 8
        if (Army8 -> Dangerous <= CMinDang8) {
            Army8 -> Status = asDestroyed;
        }               
    }

    // ARMY 9
    if (Army9 -> Status == asActive || Army9 -> Status == asSuicide) {
        Army9 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army9 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army9 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army9 -> CountDangerous ();
        if (Army9 -> Dangerous <= CSuicide9) {
            Army9 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army9 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 9
        if (Army9 -> Dangerous <= CMinDang9) {
            Army9 -> Status = asDestroyed;
        }               
    }

    // ARMY 10
    if (Army10 -> Status == asActive || Army10 -> Status == asSuicide) {
        Army10 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army10 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army10 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army10 -> CountDangerous ();
        if (Army10 -> Dangerous <= CSuicide10) {
            Army10 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army10 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 10
        if (Army10 -> Dangerous <= CMinDang10) {
            Army10 -> Status = asDestroyed;
        }               
    }


    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult11 ()
{
    int i, t = 0;

    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if (Base1 == NO_UNIT || Base1 < BADLIFE) return 1;
    
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits11 (int FactoryID)
{
    return Factory11List;
}



void DoneArtificialIntelligence11 ()
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
    if (Marine1 != NULL) {delete Marine1; Marine1 = NULL; }
    if (Army2 != NULL) {delete Army2; Army2 = NULL; }
    if (Army3 != NULL) {delete Army3; Army3 = NULL; }
    if (Army4 != NULL) {delete Army4; Army4 = NULL; }
    if (AirArmy4 != NULL) {delete AirArmy4; AirArmy4 = NULL; }
    if (Army5 != NULL) {delete Army5; Army5 = NULL; }
    if (Army6 != NULL) {delete Army6; Army6 = NULL; }
    if (Army7 != NULL) {delete Army7; Army7 = NULL; }
    if (Army8 != NULL) {delete Army8; Army8 = NULL; }
    if (Army9 != NULL) {delete Army9; Army9 = NULL; }
    if (Army10 != NULL) {delete Army10; Army10 = NULL; }
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
// OK   
