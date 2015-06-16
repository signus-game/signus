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
//           Mission 19
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
#define A1X1 40
#define A1Y1 46
#define A1X2 65
#define A1Y2 63

// Action Radius of Army 2
#define A2X1 136
#define A2Y1 43
#define A2X2 167
#define A2Y2 67

// Action Radius of Army 3
#define A3X1  0
#define A3Y1 81
#define A3X2 19
#define A3Y2 103

// Action Radius of Army 4
#define A4X1 174
#define A4Y1 83
#define A4X2 194
#define A4Y2 99

// Action Radius of Army 5
#define A5X1 81
#define A5Y1 57
#define A5X2 117
#define A5Y2 82

// Action Radius of Army 6
#define A6X1 40
#define A6Y1 87
#define A6X2 53
#define A6Y2 98

// Action Radius of Army 7
#define A7X1 138
#define A7Y1 81
#define A7X2 154
#define A7Y2 96

// Action Radius of Army 8
#define A8X1  7
#define A8Y1 156
#define A8X2 29
#define A8Y2 175

// Action Radius of Army 9
#define A9X1 171
#define A9Y1 164
#define A9X2 193
#define A9Y2 183

// Action Radius of Army 10
#define A10X1 52
#define A10Y1 103
#define A10X2 72
#define A10Y2 117

// Action Radius of Army 11
#define A11X1 133
#define A11Y1 100
#define A11X2 154
#define A11Y2 117

// Action Radius of Army 12
#define A12X1 149
#define A12Y1 152
#define A12X2 164
#define A12Y2 175

// Action Radius of Army 13
#define A13X1 49
#define A13Y1 159
#define A13X2 69
#define A13Y2 180

// Action Radius of Army 14
#define A14X1 80
#define A14Y1 114
#define A14X2 100
#define A14Y2 131

// Action Radius of Army 15
#define A15X1 116
#define A15Y1 118
#define A15X2 138
#define A15Y2 138

// Action Radius of Army 16
#define A16X1 92
#define A16Y1 153
#define A16X2 128
#define A16Y2 181


// Action Radius of Air Army 1
#define AA1X1 95
#define AA1Y1 140
#define AA1X2 110
#define AA1Y2 151

// Uran 1
#define U1X 38
#define U1Y 19
#define TU1X1 38
#define TU1Y1 14
#define TU1X2 38
#define TU1Y2 18

// Uran 1
#define U1X 38
#define U1Y 19
#define TU1X1 38
#define TU1Y1 14
#define TU1X2 38
#define TU1Y2 18

// Uran 2
#define U2X 39
#define U2Y 17
#define TU2X1 39
#define TU2Y1 12
#define TU2X2 39
#define TU2Y2 16

// Uran 3
#define U3X 41
#define U3Y 18
#define TU3X1 41
#define TU3Y1 13
#define TU3X2 41
#define TU3Y2 17

// Uran 4
#define U4X 145
#define U4Y 19
#define TU4X1 145
#define TU4Y1 14
#define TU4X2 145
#define TU4Y2 18

// Uran 5
#define U5X 148
#define U5Y 18
#define TU5X1 148
#define TU5Y1 13
#define TU5X2 148
#define TU5Y2 17


// Radar 1 (of Armies 1 & 3)
#define R1X 49
#define R1Y 48

// Radar 2 (of Armies 2 & 4)
#define R2X 156
#define R2Y 49

// Radar 6
#define R6X 97
#define R6Y 91

// Radar 7
#define R7X 113
#define R7Y 97

// Radar 8
#define R8X 44
#define R8Y 148

// Radar 9
#define R9X 165
#define R9Y 154

// Base 1 
#define B1X 113
#define B1Y 152

// Factory 1 
#define F1X 149
#define F1Y 168

// Factory 2 
#define F2X 132
#define F2Y 119

// Factory 3 
#define F3X  73
#define F3Y 143

// Factory 4 
#define F4X  94
#define F4Y 159

// 1st selected unit
#define SelX 46
#define SelY 33



#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6
#define CMinDang7  0    // Dangerous limit for Army 7
#define CMinDang8  0    // Dangerous limit for Army 8
#define CMinDang9  0    // Dangerous limit for Army 9
#define CMinDang10 0    // Dangerous limit for Army 10
#define CMinDang11 0    // Dangerous limit for Army 11
#define CMinDang12 0    // Dangerous limit for Army 12
#define CMinDang13 0    // Dangerous limit for Army 13
#define CMinDang14 0    // Dangerous limit for Army 14
#define CMinDang15 0    // Dangerous limit for Army 15
#define CMinDang16 0    // Dangerous limit for Army 16
#define CMinDangA1 0    // Dangerous limit for Air Army 1

#define CSuicide1  5    // Suicide limit for Army 1
#define CSuicide2  5    // Suicide limit for Army 2
#define CSuicide3  5    // Suicide limit for Army 3
#define CSuicide4  5    // Suicide limit for Army 4
#define CSuicide5  5    // Suicide limit for Army 5
#define CSuicide6  5    // Suicide limit for Army 6
#define CSuicide7  5    // Suicide limit for Army 7
#define CSuicide8  5    // Suicide limit for Army 8
#define CSuicide9  5    // Suicide limit for Army 9
#define CSuicide10 5    // Suicide limit for Army 10
#define CSuicide11 5    // Suicide limit for Army 11
#define CSuicide12 5    // Suicide limit for Army 12
#define CSuicide13 5    // Suicide limit for Army 13
#define CSuicide14 5    // Suicide limit for Army 14
#define CSuicide15 5    // Suicide limit for Army 15
#define CSuicide16 5    // Suicide limit for Army 16
#define CSuicideA1 8    // Suicide limit for Air Army 1

TPoint MinePlaces19 [] = {78,58, 87,63, 96,64, 91,72, 95,72, 98,72, 102,72,
                                                    95,83, 98,82, 68,83, 67,93, 36,133, 41,136, 41,139, 
                                                    41,143, 50,148, 127,83, 166,148, 165,142, 165,137,
                                                    105,111, 109,111, 108,120, 77,130, 71,142, 67,140,
                                                    97,159, 104,160, 114,162, 117,159, 135,146, 132,170,
                                                    130,168, 122,173, 96,150,
                                                -1,-1};


int Factory19List1 [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama, 
                                             unAgni, unRex, unHerkules, unSkorpion, unPatriot, 
                                             unGolias, unXenon, unCeres, unGnom, 0};

int Factory19List2 [] = {unPerseusAlfa, unPerseusBeta, unPerseusGama, unDirrac, 
                                             unRex, unHerkules, unSkorpion, unPatriot, unGolias, 
                                             unFalanga, unXenon, unMedea, 0};

int Factory19List3 [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama, 
                                             unMohykan, unOlymp, unMesias, unGanymedes, 
                                             unDirrac, unAgni, unRex, unHerkules, unSkorpion, unPatriot, 
                                             unGolias, unFalanga, unXenon, unCeres, unGnom, unMedea, 0};

int Factory19List4 [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama, 
                                             unDirrac, unAgni, unRex, unHerkules, unSkorpion, unPatriot, unGolias, 
                                             unFalanga, unXenon, unCeres, unGnom, unMedea, 0};


void InitAI19 ()
{
    int i,j;
    TField *f;
    TUnit *u;

    LockDraw ();        
    
    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 0;
    MoneyBadlife = 15000;
    MBPlaces = NULL;
    DeniedPlaces = NULL;
    
    Uran1 = GetField (U1X, U1Y) -> Unit;    
    Uran2 = GetField (U2X, U2Y) -> Unit;    
    Uran3 = GetField (U3X, U3Y) -> Unit;    
    Uran4 = GetField (U4X, U4Y) -> Unit;    
    Uran5 = GetField (U5X, U5Y) -> Unit;    
    
    Base1 = GetField (B1X, B1Y) -> Unit;
    Factory1 = GetField (F1X, F1Y) -> Unit;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    Factory3 = GetField (F3X, F3Y) -> Unit;
    Factory4 = GetField (F4X, F4Y) -> Unit;
    
    
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
    Army1 -> SetRadar (R1X, R1Y);
    Army1 -> InsertUnits ();

    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> SetRadar (R2X, R2Y);
    Army2 -> InsertUnits ();
    
    Army3 = new TGroundArmy();
    Army3 -> SetAR (A3X1, A3Y1, A3X2, A3Y2);
    Army3 -> SetRadar (R1X, R1Y);
    Army3 -> InsertUnits ();

    Army4 = new TGroundArmy();
    Army4 -> SetAR (A4X1, A4Y1, A4X2, A4Y2);
    Army4 -> SetRadar (R2X, R2Y);
    Army4 -> InsertUnits ();

    Army5 = new TGroundArmy();
    Army5 -> SetAR (A5X1, A5Y1, A5X2, A5Y2);
    Army5 -> SetRadar (-1, -1);
    Army5 -> InsertUnits ();

    Army6 = new TGroundArmy();
    Army6 -> SetAR (A6X1, A6Y1, A6X2, A6Y2);
    Army6 -> SetRadar (R6X, R6Y);
    Army6 -> InsertUnits ();

    Army7 = new TGroundArmy();
    Army7 -> SetAR (A7X1, A7Y1, A7X2, A7Y2);
    Army7 -> SetRadar (R7X, R7Y);
    Army7 -> InsertUnits ();

    Army8 = new TGroundArmy();
    Army8 -> SetAR (A8X1, A8Y1, A8X2, A8Y2);
    Army8 -> SetRadar (R8X, R8Y);
    Army8 -> InsertUnits ();

    Army9 = new TGroundArmy();
    Army9 -> SetAR (A9X1, A9Y1, A9X2, A9Y2);
    Army9 -> SetRadar (R9X, R9Y);
    Army9 -> InsertUnits ();

    Army10 = new TGroundArmy();
    Army10 -> SetAR (A10X1, A10Y1, A10X2, A10Y2);
    Army10 -> SetRadar (-1, -1);
    Army10 -> InsertUnits ();

    Army11 = new TGroundArmy();
    Army11 -> SetAR (A11X1, A11Y1, A11X2, A11Y2);
    Army11 -> SetRadar (-1, -1);
    Army11 -> InsertUnits ();

    Army12 = new TGroundArmy();
    Army12 -> SetAR (A12X1, A12Y1, A12X2, A12Y2);
    Army12 -> SetRadar (-1, -1);
    Army12 -> InsertUnits ();

    Army13 = new TGroundArmy();
    Army13 -> SetAR (A13X1, A13Y1, A13X2, A13Y2);
    Army13 -> SetRadar (-1, -1);
    Army13 -> InsertUnits ();

    Army14 = new TGroundArmy();
    Army14 -> SetAR (A14X1, A14Y1, A14X2, A14Y2);
    Army14 -> SetRadar (-1, -1);
    Army14 -> InsertUnits ();

    Army15 = new TGroundArmy();
    Army15 -> SetAR (A15X1, A15Y1, A15X2, A15Y2);
    Army15 -> SetRadar (-1, -1);
    Army15 -> InsertUnits ();

    Army16 = new TGroundArmy();
    Army16 -> SetAR (A16X1, A16Y1, A16X2, A16Y2);
    Army16 -> SetRadar (-1, -1);
    Army16 -> InsertUnits ();

    AirArmy1 = new TAirArmy();
    AirArmy1 -> SetAR (AA1X1, AA1Y1, AA1X2, AA1Y2);
    AirArmy1 -> SetRadar (-1, -1);
    AirArmy1 -> InsertUnits ();
    AirArmy1 -> Status = asActive;
    

    // Naplnime Urany
    for (i = TU1X1; i <= TU1X2; i++) {
        for (j = TU1Y1; j <= TU1Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran1]) -> Inventory [((TUran*)Units [Uran1]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TU2X1; i <= TU2X2; i++) {
        for (j = TU2Y1; j <= TU2Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran2]) -> Inventory [((TUran*)Units [Uran2]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TU3X1; i <= TU3X2; i++) {
        for (j = TU3Y1; j <= TU3Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran3]) -> Inventory [((TUran*)Units [Uran3]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TU4X1; i <= TU4X2; i++) {
        for (j = TU4Y1; j <= TU4Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran4]) -> Inventory [((TUran*)Units [Uran4]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    for (i = TU5X1; i <= TU5X2; i++) {
        for (j = TU5Y1; j <= TU5Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT ) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran5]) -> Inventory [((TUran*)Units [Uran5]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
        
    // Polozime miny    
    for (i = 0; MinePlaces19 [i].x != -1; i++) {
        MinePlace (MinePlaces19 [i].x, MinePlaces19 [i].y, BADLIFE);
    }
    
    // zamerime jednotku
    i = GetField (SelX, SelY) -> Unit;  
    Units [i] -> Select ();

    UnlockDraw ();          
    RedrawMap ();

}




void LoadArtificialIntelligence19 (FILE *f)
{

    DoneArtificialIntelligence19 ();

    MBPlaces = NULL;
    DeniedPlaces = NULL;

    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);
    DUPos = 0;

    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
    Army4 = new TGroundArmy (f);
    Army5 = new TGroundArmy (f);
    Army6 = new TGroundArmy (f);
    Army7 = new TGroundArmy (f);
    Army8 = new TGroundArmy (f);
    Army9 = new TGroundArmy (f);
    Army10 = new TGroundArmy (f);
    Army11 = new TGroundArmy (f);
    Army12 = new TGroundArmy (f);
    Army13 = new TGroundArmy (f);
    Army14 = new TGroundArmy (f);
    Army15 = new TGroundArmy (f);
    Army16 = new TGroundArmy (f);
    AirArmy1 = new TAirArmy (f);
}



void SaveArtificialIntelligence19 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);

    Towers -> Save (f);
    Army1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);      
    Army4 -> Save (f);
    Army5 -> Save (f);
    Army6 -> Save (f);
    Army7 -> Save (f);
    Army8 -> Save (f);
    Army9 -> Save (f);
    Army10 -> Save (f);
    Army11 -> Save (f);
    Army12 -> Save (f);
    Army13 -> Save (f);
    Army14 -> Save (f);
    Army15 -> Save (f);
    Army16 -> Save (f);
    AirArmy1 -> Save (f);
    DUPos = 0;
}


int ArtificialIntelligence19 ()
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
    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unArtefactHouse) 
        Base1 = NO_UNIT;
    Factory1 = GetField (F1X, F1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;
    Factory2 = GetField (F2X, F2Y) -> Unit;
    if (Factory2 == NO_UNIT || Units [Factory2] -> Type % BADLIFE != unFactory) 
        Factory2 = NO_UNIT;
    Factory3 = GetField (F3X, F3Y) -> Unit;
    if (Factory3 == NO_UNIT || Units [Factory3] -> Type % BADLIFE != unFactory) 
        Factory3 = NO_UNIT;
    Factory4 = GetField (F4X, F4Y) -> Unit;
    if (Factory4 == NO_UNIT || Units [Factory4] -> Type % BADLIFE != unFactory) 
        Factory4 = NO_UNIT;

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
    Army9 -> DeleteKilled2 (); Army9 -> DeleteKilled ();
    Army10 -> DeleteKilled2 (); Army10 -> DeleteKilled ();
    Army11 -> DeleteKilled2 (); Army11 -> DeleteKilled ();
    Army12 -> DeleteKilled2 (); Army12 -> DeleteKilled ();
    Army13 -> DeleteKilled2 (); Army13 -> DeleteKilled ();
    Army14 -> DeleteKilled2 (); Army14 -> DeleteKilled ();
    Army15 -> DeleteKilled2 (); Army15 -> DeleteKilled ();
    Army16 -> DeleteKilled2 (); Army16 -> DeleteKilled ();
    AirArmy1 -> DeleteKilled2 (); AirArmy1 -> DeleteKilled ();
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
    Army9 -> DoAttackedFields ();
    Army10 -> DoAttackedFields ();
    Army11 -> DoAttackedFields ();
    Army12 -> DoAttackedFields ();
    Army13 -> DoAttackedFields ();
    Army14 -> DoAttackedFields ();
    Army15 -> DoAttackedFields ();
    Army16 -> DoAttackedFields ();
    AirArmy1 -> DoAttackedFields ();
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
    // Test zaktivovani armady 11
    if (Army11 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army11 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army11 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 12
    if (Army12 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army12 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army12 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 13
    if (Army13 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army13 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army13 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 14
    if (Army14 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army14 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army14 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 15
    if (Army15 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army15 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army15 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani armady 16
    if (Army16 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army16 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army16 -> Status = asActive;
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
    if (Army8 -> Status == asActive || Army8 -> Status == asSuicide) st += Army8 -> nofBadLife;
    if (Army9 -> Status == asActive || Army9 -> Status == asSuicide) st += Army9 -> nofBadLife;
    if (Army10 -> Status == asActive || Army10 -> Status == asSuicide) st += Army10 -> nofBadLife;
    if (Army11 -> Status == asActive || Army11 -> Status == asSuicide) st += Army11 -> nofBadLife;
    if (Army12 -> Status == asActive || Army12 -> Status == asSuicide) st += Army12 -> nofBadLife;
    if (Army13 -> Status == asActive || Army13 -> Status == asSuicide) st += Army13 -> nofBadLife;
    if (Army14 -> Status == asActive || Army14 -> Status == asSuicide) st += Army14 -> nofBadLife;
    if (Army15 -> Status == asActive || Army15 -> Status == asSuicide) st += Army15 -> nofBadLife;
    if (Army16 -> Status == asActive || Army16 -> Status == asSuicide) st += Army16 -> nofBadLife;
    if (AirArmy1 -> Status == asActive || AirArmy1 -> Status == asSuicide) st += AirArmy1 -> nofBadLife;
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
    // ARMY 11
    if (Army11 -> Status == asActive || Army11 -> Status == asSuicide) {
        Army11 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army11 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army11 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army11 -> CountDangerous ();
        if (Army11 -> Dangerous <= CSuicide11) {
            Army11 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army11 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 11
        if (Army11 -> Dangerous <= CMinDang11) {
            Army11 -> Status = asDestroyed;
        }               
    }
    // ARMY 12
    if (Army12 -> Status == asActive || Army12 -> Status == asSuicide) {
        Army12 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army12 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army12 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army12 -> CountDangerous ();
        if (Army12 -> Dangerous <= CSuicide12) {
            Army12 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army12 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 12
        if (Army12 -> Dangerous <= CMinDang12) {
            Army12 -> Status = asDestroyed;
        }               
    }
    // ARMY 13
    if (Army13 -> Status == asActive || Army13 -> Status == asSuicide) {
        Army13 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army13 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army13 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army13 -> CountDangerous ();
        if (Army13 -> Dangerous <= CSuicide13) {
            Army13 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army13 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 13
        if (Army13 -> Dangerous <= CMinDang13) {
            Army13 -> Status = asDestroyed;
        }               
    }
    // ARMY 14
    if (Army14 -> Status == asActive || Army14 -> Status == asSuicide) {
        Army14 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army14 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army14 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army14 -> CountDangerous ();
        if (Army14 -> Dangerous <= CSuicide14) {
            Army14 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army14 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 14
        if (Army14 -> Dangerous <= CMinDang14) {
            Army14 -> Status = asDestroyed;
        }               
    }
    // ARMY 15
    if (Army15 -> Status == asActive || Army15 -> Status == asSuicide) {
        Army15 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army15 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army15 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army15 -> CountDangerous ();
        if (Army15 -> Dangerous <= CSuicide15) {
            Army15 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army15 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 15
        if (Army15 -> Dangerous <= CMinDang15) {
            Army15 -> Status = asDestroyed;
        }               
    }
    // ARMY 16
    if (Army16 -> Status == asActive || Army16 -> Status == asSuicide) {
        Army16 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army16 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army16 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        Army16 -> CountDangerous ();
        if (Army16 -> Dangerous <= CSuicide16) {
            Army16 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (Army16 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 16
        if (Army16 -> Dangerous <= CMinDang16) {
            Army16 -> Status = asDestroyed;
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



    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult19 ()
{
    int i, t = 0;

    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unArtefactHouse) 
        Base1 = NO_UNIT;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if (Base1 == NO_UNIT || Base1 < BADLIFE) return 1;

    
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits19 (int FactoryID)
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
    Factory4 = GetField (F4X, F4Y) -> Unit;
    if (Factory4 == NO_UNIT || Units [Factory4] -> Type % BADLIFE != unFactory) 
        Factory4 = NO_UNIT;
    
    if (FactoryID == Factory1) return Factory19List1;
    if (FactoryID == Factory2) return Factory19List2;
    if (FactoryID == Factory3) return Factory19List3;
    if (FactoryID == Factory4) return Factory19List4;
    return Factory19List1;
}



void DoneArtificialIntelligence19 ()
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
    if (Army9 != NULL) {delete Army9; Army9 = NULL; }
    if (Army10 != NULL) {delete Army10; Army10 = NULL; }
    if (Army11 != NULL) {delete Army11; Army11 = NULL; }
    if (Army12 != NULL) {delete Army12; Army12 = NULL; }
    if (Army13 != NULL) {delete Army13; Army13 = NULL; }
    if (Army14 != NULL) {delete Army14; Army14 = NULL; }
    if (Army15 != NULL) {delete Army15; Army15 = NULL; }
    if (Army16 != NULL) {delete Army16; Army16 = NULL; }
    if (AirArmy1 != NULL) {delete AirArmy1; AirArmy1 = NULL; }
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
    
