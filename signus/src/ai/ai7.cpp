/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002
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
//           Mission 7
//
// Programmed by Marek Wunsch
//


#include "headers.h"
#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "fields.h"



// Base 6
#define B6X 202
#define B6Y 10

// Action Radius of Army 1
#define A1X1 36
#define A1Y1 65
#define A1X2 47
#define A1Y2 68

// Action Radius of Army 2
#define A2X1 49
#define A2Y1 18
#define A2X2 85
#define A2Y2 45

// Action Radius of Army 3
#define A3X1 105
#define A3Y1 55
#define A3X2 132
#define A3Y2 78

// Action Radius of Army 4
#define A4X1 139
#define A4Y1 25
#define A4X2 155
#define A4Y2 41

// Action Radius of Army 5
#define A5X1 185
#define A5Y1 24
#define A5X2 207
#define A5Y2 40

// Action Radius of Army 6
#define A6X1 202
#define A6Y1 16
#define A6X2 215
#define A6Y2 25

// Action Radius of Army 7
#define A7X1 104
#define A7Y1 79
#define A7X2 154
#define A7Y2 105

// Action Radius of Army 8
#define A8X1 148
#define A8Y1 52
#define A8X2 163
#define A8Y2 62 



// These units will be placed to Olymp1
#define TO1X1  46
#define TO1Y1  57
#define TO1X2  50
#define TO1Y2  58

// These units will be placed to Olymp2
#define TO2X1  46
#define TO2Y1  53
#define TO2X2  50
#define TO2Y2  54

// These units will be placed to Olymp3
#define TO3X1  46
#define TO3Y1  60
#define TO3X2  50
#define TO3Y2  61

// These units will be placed to Olymp4
#define TO4X1  46
#define TO4Y1  63
#define TO4X2  50
#define TO4Y2  64


// Base3
//#define B3X 50
//#define B3Y 40

// Factory building 2
#define F2X 75
#define F2Y 25

// Radar of Army2
#define R2X 64
#define R2Y 32

// Radar of Army3
#define R3X 105
#define R3Y 54

// Radar of Army4
#define R4X 193
#define R4Y 40

// Radar of Army5
#define R5X 178
#define R5Y 28

// Radar of Army6
#define R6X 210
#define R6Y 10


// Olymp1 
#define O1X 33
#define O1Y 64

// Olymp2 
#define O2X 35
#define O2Y 63

// Olymp3
#define O3X 37
#define O3Y 65

// Olymp4
#define O4X 39
#define O4Y 65

// Mohykan1 
#define M1X 31
#define M1Y 66
#define M1DX 23
#define M1DY 70

// Mohykan2 
#define M2X 32
#define M2Y 65
#define M2DX 25
#define M2DY 70

// Mohykan3 
#define M3X 34
#define M3Y 66
#define M3DX 27
#define M3DY 72

// Mohykan4 
#define M4X 35
#define M4Y 65
#define M4DX 29
#define M4DY 71

// Mesias1
#define Me1X 38
#define Me1Y 63

// Ganymedes
#define Ga1X 42
#define Ga1Y 65



// Olymp1 Destination
#define O1DX 27
#define O1DY 70

// Olymp2 Destination
#define O2DX 28
#define O2DY 69

// Olymp3 Destination
#define O3DX 31
#define O3DY 69

// Olymp4 Destination
#define O4DX 34
#define O4DY 66




// Pagoda1
#define P1X 72
#define P1Y 25
// Pagoda2
#define P2X 75
#define P2Y 31

#define WP1X -1
#define WP1Y -1


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define CMinDang1 0    // Dangerous limit for Army 1
#define CMinDang2 0    // Dangerous limit for Army 2
#define CMinDang3 0    // Dangerous limit for Army 1
#define CMinDang4 0    // Dangerous limit for Army 2
#define CMinDang5 0    // Dangerous limit for Army 2
#define CMinDang6 0    // Dangerous limit for Army 2
#define CMinDang7 0    // Dangerous limit for Army 2
#define CMinDang8 0    // Dangerous limit for Army 2

#define CSuicide1 4    // Suicide limit for Army 1
#define CSuicide2 4    // Suicide limit for Army 2
#define CSuicide3 4    // Suicide limit for Army 1
#define CSuicide4 50   // Suicide limit for Army 2
#define CSuicide5 4    // Suicide limit for Army 2
#define CSuicide6 4    // Suicide limit for Army 2
#define CSuicide7 4    // Suicide limit for Army 2
#define CSuicide8 4    // Suicide limit for Army 2

int Factory2List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unOlymp, unMohykan, unMesias, unXenon, unMedea, 0};

TPoint MinePlaces7 [] = { 62,63, 63,65, 67,63, 74,65,
                                                    98,66, 102,64, 100,61,
                                                    122,68, 139,63, 143,64, 140,65,
                                                    177,37, 186,38, 179,43,
                                                -1,-1};


void InitAI7 ()
{
    int i,j;
    TBase *Base;
    TField *f;
    TUnit *u;
    
    LockDraw();

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 600;
    MoneyBadlife = 7000;
    MBPlaces = NULL;
    DeniedPlaces = NULL;

    Base6 = GetField (B6X, B6Y) -> Unit;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    Olymp1 = GetField (O1X, O1Y) -> Unit;
    Olymp2 = GetField (O2X, O2Y) -> Unit;
    Olymp3 = GetField (O3X, O3Y) -> Unit;
    Olymp4 = GetField (O4X, O4Y) -> Unit;
    Mohykan1 = GetField (M1X, M1Y) -> Unit;
    Mohykan2 = GetField (M2X, M2Y) -> Unit;
    Mohykan3 = GetField (M3X, M3Y) -> Unit;
    Mohykan4 = GetField (M4X, M4Y) -> Unit;
    Mesias1 = GetField (Me1X, Me1Y) -> Unit;
    Ganymedes1 = GetField (Ga1X, Ga1Y) -> Unit;
    Radar2 = GetField (R2X, R2Y) -> Unit;
    Pagoda1 = GetField (P1X, P1Y) -> Unit;
    Pagoda2 = GetField (P2X, P2Y) -> Unit;
    
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
    Army1 -> Status = asActive;
//  Army1 -> WayPoint.x = WP1X; Army1 -> WayPoint.y = WP1Y;
    Army1 -> SetAIConst(2);


    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> SetRadar (R2X, R2Y);
    Army2 -> InsertUnits ();
    
    Army3 = new TGroundArmy();
    Army3 -> SetAR (A3X1, A3Y1, A3X2, A3Y2);
    Army3 -> SetRadar (R3X, R3Y);
    Army3 -> InsertUnits ();

    Army4 = new TGroundArmy();
    Army4 -> SetAR (A4X1, A4Y1, A4X2, A4Y2);
    Army4 -> SetRadar (R4X, R4Y);
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
    Army7 -> InsertUnits ();
    
    Army8 = new TGroundArmy();
    Army8 -> SetAR (A8X1, A8Y1, A8X2, A8Y2);
    Army8 -> InsertUnits ();
    
    // Jednotky do Olympu
    for (i = TO1X1; i <= TO1X2; i++) {
        for (j = TO1Y1; j <= TO1Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOlymp*)Units [Olymp1]) -> Inventory [((TOlymp*)Units [Olymp1]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TO2X1; i <= TO2X2; i++) {
        for (j = TO2Y1; j <= TO2Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOlymp*)Units [Olymp2]) -> Inventory [((TOlymp*)Units [Olymp2]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TO3X1; i <= TO3X2; i++) {
        for (j = TO3Y1; j <= TO3Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOlymp*)Units [Olymp3]) -> Inventory [((TOlymp*)Units [Olymp3]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TO4X1; i <= TO4X2; i++) {
        for (j = TO4Y1; j <= TO4Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOlymp*)Units [Olymp4]) -> Inventory [((TOlymp*)Units [Olymp4])
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    
    // Polozime miny
    
    for (i = 0; MinePlaces7 [i].x != -1; i++) {
        MinePlace (MinePlaces7 [i].x, MinePlaces7 [i].y, BADLIFE);
    }
    UnlockDraw ();          


}




void LoadArtificialIntelligence7 (FILE *f)
{

    DoneArtificialIntelligence7 ();

    MBPlaces = NULL;    
    DeniedPlaces = NULL;
    
    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);
    DUPos = 0;
    
    fread (&Olymp1, sizeof (int), 1, f);
    fread (&Olymp2, sizeof (int), 1, f);
    fread (&Olymp3, sizeof (int), 1, f);
    fread (&Olymp4, sizeof (int), 1, f);
    fread (&Mohykan1, sizeof (int), 1, f);
    fread (&Mohykan2, sizeof (int), 1, f);
    fread (&Mohykan3, sizeof (int), 1, f);
    fread (&Mohykan4, sizeof (int), 1, f);
    fread (&Mesias1, sizeof (int), 1, f);
    fread (&Ganymedes1, sizeof (int), 1, f);
    fread (&Pagoda1, sizeof (int), 1, f);
    fread (&Pagoda2, sizeof (int), 1, f);
    fread (&Factory2, sizeof (int), 1, f);
    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
    Army4 = new TGroundArmy (f);
    Army5 = new TGroundArmy (f);
    Army6 = new TGroundArmy (f);
    Army7 = new TGroundArmy (f);
    Army8 = new TGroundArmy (f);
}



void SaveArtificialIntelligence7 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);

    fwrite (&Olymp1, sizeof (int), 1, f);
    fwrite (&Olymp2, sizeof (int), 1, f);
    fwrite (&Olymp3, sizeof (int), 1, f);
    fwrite (&Olymp4, sizeof (int), 1, f);
    fwrite (&Mohykan1, sizeof (int), 1, f);
    fwrite (&Mohykan2, sizeof (int), 1, f);
    fwrite (&Mohykan3, sizeof (int), 1, f);
    fwrite (&Mohykan4, sizeof (int), 1, f);
    fwrite (&Mesias1, sizeof (int), 1, f);
    fwrite (&Ganymedes1, sizeof (int), 1, f);
    fwrite (&Pagoda1, sizeof (int), 1, f);
    fwrite (&Pagoda2, sizeof (int), 1, f);
    fwrite (&Factory2, sizeof (int), 1, f);
    Towers -> Save (f);
    Army1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);
    Army4 -> Save (f);
    Army5 -> Save (f);
    Army6 -> Save (f);
    Army7 -> Save (f);
    Army8 -> Save (f);
    DUPos = 0;
}



int ArtificialIntelligence7 ()
{
    int i, j, r, st, x, y, id;
    char buf [40];
    TBase *Base;
    TField *f;

    Message (SigText[TXT_AI_ANALYSE]);
    
    // Prepocitame viditelnost
    AllowBadlife (TRUE);
    ComputeVisib ();
    RedrawMap ();
    
    LockDraw ();
    
    // Aktualizujeme specialni objekty
    Base6 = GetField (B6X, B6Y) -> Unit;
    if (Base6 == NO_UNIT || Units [Base6] -> Type % BADLIFE != unBase) 
        Base6 = NO_UNIT;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    if (Factory2 == NO_UNIT || Units [Factory2] -> Type % BADLIFE != unFactory) 
        Factory2 = NO_UNIT;
    Radar2 = GetField (R2X, R2Y) -> Unit;
    if (Radar2 == NO_UNIT || Units [Radar2] -> Type % BADLIFE != unRadar) 
        Radar2 = NO_UNIT;
    Radar3 = GetField (R3X, R3Y) -> Unit;
    if (Radar2 == NO_UNIT || Units [Radar3] -> Type % BADLIFE != unRadar) 
        Radar3 = NO_UNIT;
    Radar4 = GetField (R4X, R4Y) -> Unit;
    if (Radar4 == NO_UNIT || Units [Radar4] -> Type % BADLIFE != unRadar) 
        Radar4 = NO_UNIT;
    Radar5 = GetField (R5X, R5Y) -> Unit;
    if (Radar5 == NO_UNIT || Units [Radar5] -> Type % BADLIFE != unRadar) 
        Radar5 = NO_UNIT;
    Radar6 = GetField (R6X, R6Y) -> Unit;
    if (Radar6 == NO_UNIT || Units [Radar6] -> Type % BADLIFE != unRadar) 
        Radar6 = NO_UNIT;

    // Vyradime znicene jednotky z armad        
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
    Army7 -> DeleteKilled2 (); Army7 -> DeleteKilled ();
    Army8 -> DeleteKilled2 (); Army8 -> DeleteKilled ();
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
    Army8 -> DoAttackedFields ();
    AttackFieldPos = 0;

    AnalyseLandscape ();    
    
    Message (SigText[TXT_AI_ARMY]);
  
    Army1 -> Status = asActive;
        

  // Likvidace FACTORY II
    if ((Factory2 != NO_UNIT) && (Units[Factory2] != NULL) && ( (GetField(P1X,P1Y) -> Visib == 1) || (GetField(P2X,P2Y) -> Visib == 2) ) ) { 
      UnlockDraw ();
    if ((Pagoda1 != NO_UNIT) && (Units[Pagoda1] != NULL)) {
        Units[Pagoda1] -> Attack(75, 25);
        Units[Pagoda1] -> Attack(75, 25);
    }
    if ((Pagoda2 != NO_UNIT) && (Units[Pagoda2] != NULL)) {
        Units[Pagoda2] -> Attack(75, 28);
        Units[Pagoda2] -> Attack(75, 28);
    }
    LockDraw ();
  }


    // Test zaktivovani armady 2
    if (Army2 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army2 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army2 -> Status = asActive;
                Army2 -> SetAIConst (1);
            }
        }
    }

    // Test zaktivovani armady 3
    if (Army3 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army3 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army3 -> Status = asActive;
                Army3 -> SetAIConst (1);
            }
        }
    }

    // Test zaktivovani armady 4
    if (Army4 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army4 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army4 -> Status = asActive;
                Army4 -> SetAIConst (1);
            }
        }
    }

    // Test zaktivovani armady 5
    if (Army5 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army5 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army5 -> Status = asActive;
                Army5 -> SetAIConst (2);
            }
        }
    }

    // Test zaktivovani armady 6
    if (Army6 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army6 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army6 -> Status = asActive;
                Army6 -> SetAIConst (1);
            }
        }
    }

    // Test zaktivovani armady 7
    if (Army7 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army7 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army7 -> Status = asActive;
                Army7 -> SetAIConst (4);
            }
        }
    }

    // Test zaktivovani armady 8
    if (Army8 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army8 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army8 -> Status = asActive;
                Army8 -> SetAIConst (1);
            }
        }
    }


    
    // Presun Olympu a Mohykanu
    if (ActualTurn == 1) {
    UnlockDraw();
    if ((Mohykan1 != NO_UNIT) && (Units[Mohykan1] != NULL)) {
      Units[Mohykan1] -> Select ();
      Units[Mohykan1] -> Move(M1DX, M1DY);
      if (Units [Mohykan1] != NULL) Army1 -> Insert (Mohykan1);
    }
    if ((Mohykan2 != NO_UNIT) && (Units[Mohykan2] != NULL)) {
      Units[Mohykan2] -> Select ();
      Units[Mohykan2] -> Move(M2DX, M2DY);
      if (Units [Mohykan2] != NULL) Army1 -> Insert (Mohykan2);
    }
    if ((Mohykan3 != NO_UNIT) && (Units[Mohykan3] != NULL)) {
      Units[Mohykan3] -> Select ();
      Units[Mohykan3] -> Move(M3DX, M3DY);
      if (Units [Mohykan3] != NULL) Army1 -> Insert (Mohykan3);
    }
    if ((Mohykan4 != NO_UNIT) && (Units[Mohykan4] != NULL)) {
      Units[Mohykan4] -> Select ();
      Units[Mohykan4] -> Move(M4DX, M4DY);
      if (Units [Mohykan4] != NULL) Army1 -> Insert (Mohykan4);
    }
    if ((Olymp1 != NO_UNIT) && (Units[Olymp1] != NULL)) {
      Units[Olymp1] -> Select ();
      Units[Olymp1] -> Move(O1DX, O1DY);
      if (Units [Olymp1] != NULL) Army1 -> Insert (Olymp1);
    }
    if ((Olymp2 != NO_UNIT) && (Units[Olymp2] != NULL)) {
      Units[Olymp2] -> Select ();
      Units[Olymp2] -> Move(O2DX, O2DY);
      if (Units [Olymp2] != NULL) Army1 -> Insert (Olymp2);
    }
    if ((Olymp3 != NO_UNIT) && (Units[Olymp3] != NULL)) {
      Units[Olymp3] -> Select ();
      Units[Olymp3] -> Move(O3DX, O3DY);
      if (Units [Olymp3] != NULL) Army1 -> Insert (Olymp3);
    }
    if ((Olymp4 != NO_UNIT) && (Units[Olymp4] != NULL)) {
      Units[Olymp4] -> Select ();
      Units[Olymp4] -> Move(O4DX, O4DY);
      if (Units [Olymp4] != NULL) Army1 -> Insert (Olymp4);
    }
    if ((Mesias1 != NO_UNIT) && (Units [Mesias1] != NULL)) Army1 -> Insert (Mesias1);
    if ((Ganymedes1 != NO_UNIT) && (Units [Ganymedes1] != NULL)) Army1 -> Insert (Ganymedes1);
    LockDraw();
  }
    

    // Dojel Olymp1 - vyklada se...
    if ((Olymp1 != NO_UNIT) && (Units [Olymp1] != NULL) && 
        IsInRect (Units [Olymp1] -> X, Units [Olymp1] -> Y, 
      O1DX, O1DY, O1DX, O1DY)) {
        UnlockDraw ();
        do {
            if (((TOlymp *) Units [Olymp1]) -> LoadedUnits > 0) {
                id = ((TOlymp *) Units [Olymp1]) -> Inventory [0];
                st = ((TOlymp *) Units [Olymp1]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army1 -> Insert (id);
        }
        while (st == TRUE); 
        LockDraw ();
    }
    
    // Dojel Olymp2 - vyklada se...
    if ((Olymp2 != NO_UNIT) && (Units [Olymp2] != NULL) && 
        IsInRect (Units [Olymp2] -> X, Units [Olymp2] -> Y, 
        O2DX, O2DY, O2DX, O2DY)) {
        UnlockDraw ();
        do {
            if (((TOlymp *) Units [Olymp2]) -> LoadedUnits > 0) {
                id = ((TOlymp *) Units [Olymp2]) -> Inventory [0];
                st = ((TOlymp *) Units [Olymp2]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army1 -> Insert (id);
        }
        while (st == TRUE); 
        LockDraw ();
        UnlockDraw();
    RedrawMap();
    LockDraw();
    }
    // Dojel Olymp3 - vyklada se...
    if ((Olymp3 != NO_UNIT) && (Units [Olymp3] != NULL) && 
        IsInRect (Units [Olymp3] -> X, Units [Olymp3] -> Y, 
        O3DX, O3DY, O3DX, O3DY)) {
        UnlockDraw ();
        do {
            if (((TOlymp *) Units [Olymp3]) -> LoadedUnits > 0) {
                id = ((TOlymp *) Units [Olymp3]) -> Inventory [0];
                st = ((TOlymp *) Units [Olymp3]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army1 -> Insert (id);
        }
        while (st == TRUE); 
        LockDraw ();
        UnlockDraw();
    RedrawMap();
    LockDraw();
    }
    // Dojel Olymp4 - vyklada se...
    if ((Olymp4 != NO_UNIT) && (Units [Olymp4] != NULL) && 
        IsInRect (Units [Olymp4] -> X, Units [Olymp4] -> Y, 
        O4DX, O4DY, O4DX, O4DY)) {
        UnlockDraw ();
        do {
            if (((TOlymp *) Units [Olymp4]) -> LoadedUnits > 0) {
                id = ((TOlymp *) Units [Olymp4]) -> Inventory [0];
                st = ((TOlymp *) Units [Olymp4]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army1 -> Insert (id);
        }
        while (st == TRUE); 
        LockDraw ();
        UnlockDraw();
    RedrawMap();
    LockDraw();
    }
    
    ComputeVisib ();


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
    if (Army8 -> Status == asActive || Army8 -> Status == asSuicide) st += Army8 -> nofBadLife;
    if (st == 0) st = 5; // Magic number
    ProgressNull ();
    ProgressLimit (st);     

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
        if (Army5 -> Dangerous <= CMinDang4) {
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
        if (Army7 -> Dangerous <= CMinDang6) {
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
        if (Army8 -> Dangerous <= CMinDang6) {
            Army8 -> Status = asDestroyed;
        }
    }

    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
//  LockDraw();
    return AssignResult ();

    
}





int *GetManufacturableUnits7 (int FactoryID)
{
        return Factory2List;
}




int AssignResult7 ()
{
    int i, t = 0;

    Base6 = GetField (B6X, B6Y) -> Unit;
    if (Base6 == NO_UNIT || Units [Base6] -> Type % BADLIFE != unBase) 
        Base6 = NO_UNIT;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL && Units [i] -> Type < unRadar) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if (Base6 < BADLIFE || Base6 == NO_UNIT) {
        return 1; 
    }
        // GoodLife vyhral - obsazena nepratelska zakladna
    
    return 0; // Zatim se jeste hraje
}




void DoneArtificialIntelligence7 ()
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
    if (Army5 != NULL) {delete Army5; Army5 = NULL; }
    if (Army6 != NULL) {delete Army6; Army6 = NULL; }
    if (Army7 != NULL) {delete Army7; Army7 = NULL; }
    if (Army8 != NULL) {delete Army8; Army8 = NULL; }
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
// OK
