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
//           Mission 3
//
// Programmed by Richard Wunsch
//

#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"

// Action Radius of Army 1
#define A1X1 11
#define A1Y1 67
#define A1X2 30
#define A1Y2 85

// Action Radius of Army 2
#define A2X1 50
#define A2Y1 65
#define A2X2 70
#define A2Y2 85


// Action Radius of Army 3
#define A3X1 30
#define A3Y1 35
#define A3X2 55
#define A3Y2 55

// Action Radius of Army 4
#define A4X1 76
#define A4Y1 20
#define A4X2 88
#define A4Y2 29

// Action Radius of Army 5
#define A5X1 68
#define A5Y1  0
#define A5X2 95
#define A5Y2 19

// Action Radius of Army 5
#define A6X1 65
#define A6Y1 37
#define A6X2 75
#define A6Y2 45

// These units will be placed to Olymp1
#define TOX1  98
#define TOY1  23
#define TOX2 104
#define TOY2  25

// Base3
#define B3X 50
#define B3Y 40

// Factory building 3
#define F3X 42
#define F3Y 40

// Base5
#define B5X 88
#define B5Y 11

// Radar of Army1
#define R1X 21
#define R1Y 74

// Radar of Army2
#define R2X 61
#define R2Y 72

// Radar of Army3
#define R3X 44
#define R3Y 50

// Radar of Army4
#define R4X 56
#define R4Y 40

// Radar of Army5
#define R5X 82
#define R5Y 16

// Olymp1
#define O1X 84
#define O1Y 28

// Olymp1 Destination
#define O1DX 72
#define O1DY 40

// Mohykan1
#define M1X 80
#define M1Y 32

// Mohykan1 Destination
#define M1DX 67
#define M1DY 43

// Attack Activation Zone for Army 2
#define AAZ2X1 35
#define AAZ2Y1 53
#define AAZ2X2 43
#define AAZ2Y2 59

// Attack Activation Zone for Army 6
#define AAZ6X1 53
#define AAZ6Y1 23
#define AAZ6X2 70
#define AAZ6Y2 53



//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define CMinDang1 0    // Dangerous limit for Army 1
#define CMinDang2 0    // Dangerous limit for Army 2
#define CMinDang3 0    // Dangerous limit for Army 3
#define CMinDang4 0    // Dangerous limit for Army 4
#define CMinDang5 0    // Dangerous limit for Army 5
#define CMinDang6 0    // Dangerous limit for Army 6

#define CSuicide1 4    // Suicide limit for Army 1
#define CSuicide2 4    // Suicide limit for Army 2
#define CSuicide3 4    // Suicide limit for Army 3
#define CSuicide4 4    // Suicide limit for Army 4
#define CSuicide5 4    // Suicide limit for Army 5
#define CSuicide6 4    // Suicide limit for Army 6

int Factory3List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unOlymp, 
                                             unMohykan, unCeres, 0};

void InitAI3 ()
{
    int i,j;
    TField *f;
    TUnit *u;

    AttackFieldPos = 0;
    DUPos = 0;
    
    ActivationTurn6 = MaxInt;       
    MoneyGoodlife = 0;
    MoneyBadlife = 2500;
    MBPlaces = NULL;
    DeniedPlaces = NULL;

    Base3 = GetField (B3X, B3Y) -> Unit;
    Factory3 = GetField (F3X, F3Y) -> Unit;
    Base5 = GetField (B5X, B5Y) -> Unit;
    Olymp1 = GetField (O1X, O1Y) -> Unit;
    Mohykan1 = GetField (M1X, M1Y) -> Unit;

    
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
    Army3 -> SetRadar (R3X, R3Y);
    Army3 -> InsertUnits ();

    Army4 = new TGroundArmy();
    Army4 -> SetAR (A4X1, A4Y1, A4X2, A4Y2);
    Army4 -> SetRadar (-1, -1);
    Army4 -> InsertUnits ();
    
    Army5 = new TGroundArmy();
    Army5 -> SetAR (A5X1, A5Y1, A5X2, A5Y2);
    Army5 -> SetRadar (R5X, R5Y);
    Army5 -> InsertUnits ();

    Army6 = new TGroundArmy ();
    Army6 -> SetRadar (-1, -1);
    Army6 -> SetAR (A6X1, A6Y1, A6X2, A6Y2);
    for (i = TOX1; i <= TOX2; i++) {
        for (j = TOY1; j <= TOY2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                Army6 -> Insert (f -> Unit);
            }
        }
    }
    Army6 -> Insert (Olymp1);
    Army6 -> Insert (Mohykan1);
    
    // Jednotky do Olympu
    LockDraw ();    
    for (i = TOX1; i <= TOX2; i++) {
        for (j = TOY1; j <= TOY2; j++) {
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
    UnlockDraw ();          


}




void LoadArtificialIntelligence3(ReadStream &stream, int format) {
	DoneArtificialIntelligence3();

	MBPlaces = NULL;
	DeniedPlaces = NULL;
	DUPos = 0;

	loadAttackedFields(stream, format);
	Olymp1 = stream.readSint32LE();
	Mohykan1 = stream.readSint32LE();
	ActivationTurn6 = stream.readSint32LE();

	Towers = new TTowers(stream);
	Army1 = new TGroundArmy(stream);
	Army2 = new TGroundArmy(stream);
	Army3 = new TGroundArmy(stream);
	Army4 = new TGroundArmy(stream);
	Army5 = new TGroundArmy(stream);
	Army6 = new TGroundArmy(stream);
}

void SaveArtificialIntelligence3(WriteStream &stream) {
	saveAttackedFields(stream);

	stream.writeSint32LE(Olymp1);
	stream.writeSint32LE(Mohykan1);
	stream.writeSint32LE(ActivationTurn6);

	Towers->Save(stream);
	Army1->Save(stream);
	Army2->Save(stream);
	Army3->Save(stream);
	Army4->Save(stream);
	Army5->Save(stream);
	Army6->Save(stream);
	DUPos = 0;
}

int ArtificialIntelligence3 ()
{
    int i, j, r, st;
    
    Message (SigText[TXT_AI_ANALYSE]);

    // Prepocitame viditelnost
    AllowBadlife (TRUE);
    ComputeVisib ();
    RedrawMap ();
    
    LockDraw ();
    
    // Aktualizujeme specialni objekty
    Base3 = GetField (B3X, B3Y) -> Unit;
    if (Base3 == NO_UNIT || Units [Base3] -> Type % BADLIFE != unBase) 
        Base3 = NO_UNIT;
    Factory3 = GetField (F3X, F3Y) -> Unit;
    if (Factory3 == NO_UNIT || Units [Factory3] -> Type % BADLIFE != unFactory) 
        Factory3 = NO_UNIT;
    Base5 = GetField (B5X, B5Y) -> Unit;
    if (Base5 == NO_UNIT || Units [Base5] -> Type % BADLIFE != unBase) 
        Base5 = NO_UNIT;

    // Vyradime znicene jednotky z armad
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
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
    AttackFieldPos = 0;

    
    AnalyseLandscape ();
    
    Message (SigText[TXT_AI_ARMY]);
    
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

    // Aktivace Army2
    for (i = 0; i < BADLIFE; i++) {
        if (Units [i] == NULL) continue;
        if (IsInRect (Units [i] -> X, Units [i] -> Y, 
        AAZ2X1, AAZ2Y1, AAZ2X2, AAZ2Y2)) {
            Army2 -> Status = asActive;
            Army2 -> SetAIConst (2); // AGRESIVE ARMY
            Army2 -> Insert (i);
        }
    }
    

    // ARMY 6 - SPECIAL ACTIVATION
    for (i = 0; i < BADLIFE; i++) {
        if (Units [i] == NULL) continue;
        if (IsInRect (Units [i] -> X, Units [i] -> Y, 
        AAZ6X1, AAZ6Y1, AAZ6X2, AAZ6Y2)) {
            ActivationTurn6 = ActualTurn;
        }
    }
    if (ActivationTurn6 == ActualTurn) {
        if (Units [Mohykan1] != NULL 
        && Units [Mohykan1] -> Type - BADLIFE == unMohykan) {
            Units [Mohykan1] -> AI_Info.State = aistMoving;
            Units [Mohykan1] -> AI_Info.point.x = M1DX;
            Units [Mohykan1] -> AI_Info.point.y = M1DY;
        }
        if (Units [Olymp1] != NULL 
        && Units [Olymp1] -> Type - BADLIFE == unOlymp) {       
            Units [Olymp1] -> AI_Info.State = aistMoving;
            Units [Olymp1] -> AI_Info.point.x = O1DX;
            Units [Olymp1] -> AI_Info.point.y = O1DY;
        }
    }
        
    
    // Nastavime ProgressBar
    st = 0;
    st += Towers -> nofBadLife;
    if (Army1 -> Status == asActive || Army1 -> Status == asSuicide) st += Army1 -> nofBadLife;
    if (Army2 -> Status == asActive || Army2 -> Status == asSuicide) st += Army2 -> nofBadLife;
    if (Army3 -> Status == asActive || Army3 -> Status == asSuicide) st += Army3 -> nofBadLife;
    if (Army4 -> Status == asActive || Army4 -> Status == asSuicide) st += Army4 -> nofBadLife;
    if (Army5 -> Status == asActive || Army5 -> Status == asSuicide) st += Army5 -> nofBadLife;
    if (Army6 -> Status == asActive || Army6 -> Status == asSuicide) st += Army6 -> nofBadLife;
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
        if (Army5 -> Dangerous <= CMinDang5) {
            Army5 -> Status = asDestroyed;
        }               
    }



    // Moving Units
    r = 0;
    for (i = BADLIFE; i < UNITS_TOP; i++) {
        if (Units [i] == NULL) continue;
        if (Units [i] -> AI_Info.State == aistMoving) r++;
    }
    if (r > 0) {
    }
    UnlockDraw ();  
    for (i = BADLIFE; i < UNITS_TOP; i++) {
        if (Units [i] == NULL) continue;
        if (Units [i] -> AI_Info.State == aistMoving) {
            ProgressInc ();
            Units [i] -> Select ();
            r = 0;
            do {
                for (j = -r; j <= r; j++) {
                    if (Units [i] == NULL) break;
                    if (Units [i] -> AI_Info.point.x + j < 0 || Units [i] -> AI_Info.point.x + j >= MapSizeX
                    || Units [i] -> AI_Info.point.y - r < 0 || Units [i] -> AI_Info.point.y - r >= MapSizeY) continue;
                    
                    st = Units [i] -> MoveFar (Units [i] -> AI_Info.point.x + j
                    , Units [i] -> AI_Info.point.y - r);
                    if (st == TRUE || st == -1) goto label;
                }
                for (j = -r; j <= r; j++) {
                    if (Units [i] == NULL) break;
                    if (Units [i] -> AI_Info.point.x + j < 0 || Units [i] -> AI_Info.point.x + j >= MapSizeX
                    || Units [i] -> AI_Info.point.y + r < 0 || Units [i] -> AI_Info.point.y + r >= MapSizeY) continue;
                    
                    st = Units [i] -> MoveFar (Units [i] -> AI_Info.point.x + j
                    , Units [i] -> AI_Info.point.y + r);
                    if (st == TRUE || st == -1) goto label;
                }
                for (j = -r + 1; j <= r - 1; j++) {
                    if (Units [i] == NULL) break;
                    if (Units [i] -> AI_Info.point.x + r < 0 || Units [i] -> AI_Info.point.x + r >= MapSizeX
                    || Units [i] -> AI_Info.point.y + j < 0 || Units [i] -> AI_Info.point.y + j >= MapSizeY) continue;
                    
                    st = Units [i] -> MoveFar (Units [i] -> AI_Info.point.x + r
                    , Units [i] -> AI_Info.point.y + j);
                    if (st == TRUE || st == -1) goto label;
                }
                for (j = -r + 1; j <= r - 1; j++) {
                    if (Units [i] == NULL) break;
                    if (Units [i] -> AI_Info.point.x - r < 0 || Units [i] -> AI_Info.point.x - r >= MapSizeX
                    || Units [i] -> AI_Info.point.y + j < 0 || Units [i] -> AI_Info.point.y + j >= MapSizeY) continue;
                    
                    st = Units [i] -> MoveFar (Units [i] -> AI_Info.point.x - r
                    , Units [i] -> AI_Info.point.y + j);
                    if (st == TRUE || st == -1) goto label;
                }
                r++;
                
            } while (r < 10);
            label:
            // POINT AT ARMY 2
            if ((Units [i] != NULL) && (Units [i] -> X == Units [i] -> AI_Info.point.x)
                && (Units [i]  -> Y == Units [i] -> AI_Info.point.y) && (st == TRUE)) {
                Units [i] -> AI_Info.State = aistNought;
            }
        }
    }
    LockDraw ();
    ProgressNull ();


    // Dojel Olymp - zaktivujeme Army6
    if (Army6 -> Status == asSleeping && Units [Olymp1] != NULL 
    && IsInRect (Units [Olymp1] -> X, Units [Olymp1] -> Y, Army6 -> ActionRadius)) {
        Army6 -> Status = asActive;
        Army6 -> SetAIConst (2); // AGRESIVE ARMY
        UnlockDraw ();
        do {
            if (((TOlymp *) Units [Olymp1]) -> LoadedUnits > 0) {
                st = ((TOlymp *) Units [Olymp1]) -> UnloadUnit ((TUnit *)Units 
                [((TOlymp *) Units [Olymp1]) -> Inventory [0]]);
            }
            else st = FALSE;
        }
        while (st == TRUE);
        LockDraw ();
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

    // Vykladame Olymp - zbytek
    if (Units [Olymp1] != NULL && Army6 -> Status == asActive) {
        UnlockDraw ();
        do {
            if (((TOlymp *) Units [Olymp1]) -> LoadedUnits > 0) {
                st = ((TOlymp *) Units [Olymp1]) -> UnloadUnit ((TUnit *)Units 
                [((TOlymp *) Units [Olymp1]) -> Inventory [0]]);
            }
            else st = FALSE;
        }
        while (st == TRUE);
        LockDraw ();
        
    }

    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}



int *GetManufacturableUnits3 (int FactoryID)
{
        return Factory3List;
}




int AssignResult3 ()
{
    int i, t = 0;
    
    Base5 = GetField (B5X, B5Y) -> Unit;
    if (Base5 == NO_UNIT || Units [Base5] -> Type % BADLIFE != unBase) 
        Base5 = NO_UNIT;

    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL && Units [i] -> Type < unRadar) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if (Base5 < BADLIFE || Base5 == NO_UNIT) return 1; 
        // GoodLife vyhral - obsazena nepratelska zakladna
    
    return 0; // Zatim se jeste hraje
}



void DoneArtificialIntelligence3 ()
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
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
// OK
