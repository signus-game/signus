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
//           Mission 12
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
#define A1X1 71
#define A1Y1  8
#define A1X2 91
#define A1Y2 29

// Action Radius of Marine 1
#define M1X1 31
#define M1Y1 154
#define M1X2 50
#define M1Y2 165

// Action Radius of Air Army 1
#define AA1X1  0
#define AA1Y1 46
#define AA1X2  8
#define AA1Y2 72

// Action Radius of Army 2
#define A2X1  6
#define A2Y1 52
#define A2X2 24
#define A2Y2 71

// Action Radius of Army 3
#define A3X1 87
#define A3Y1  0
#define A3X2 105
#define A3Y2 12

// Action Radius of Army 4
#define A4X1  0
#define A4Y1 72
#define A4X2 16
#define A4Y2 88

// Action Radius of Army 5
#define A5X1 21
#define A5Y1 90
#define A5X2 51
#define A5Y2 117

// Action Radius of Army 6
#define A6X1 63
#define A6Y1 105
#define A6X2 79
#define A6Y2 119

// Action Radius of Army 7
#define A7X1 56
#define A7Y1 133
#define A7X2 71
#define A7Y2 146

// Action Radius of Army 8
#define A8X1 19
#define A8Y1 133
#define A8X2 51
#define A8Y2 151

// Action Radius of Army 9
#define A9X1  0
#define A9Y1 121
#define A9X2 18
#define A9Y2 141

// Posily
#define PX1  8
#define PY1 151
#define PX2 29
#define PY2 165

#define F1X 64
#define F1Y 56

#define B1X 56
#define B1Y 48

#define A1X 57
#define A1Y 58
#define TA1X1 85
#define TA1Y1 52
#define TA1X2 88
#define TA1Y2 56


// 1st selected unit
#define SelX 64
#define SelY 56


#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDangM1 0    // Dangerous limit for Army 1
#define CMinDangA1 0    // Dangerous limit for AirArmy 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6
#define CMinDang7  0    // Dangerous limit for Army 7
#define CMinDang8  0    // Dangerous limit for Army 8
#define CMinDang9  0    // Dangerous limit for Army 9

#define CSuicide1  5    // Suicide limit for Army 1
#define CSuicideM1 8    // Suicide limit for Army 1
#define CSuicideA1 5    // Suicide limit for AirArmy 4
#define CSuicide2  5    // Suicide limit for Army 2
#define CSuicide3  5    // Suicide limit for Army 3
#define CSuicide4  5    // Suicide limit for Army 4
#define CSuicide5  5    // Suicide limit for Army 5
#define CSuicide6  5    // Suicide limit for Army 6
#define CSuicide7  5    // Suicide limit for Army 7
#define CSuicide8  5    // Suicide limit for Army 8
#define CSuicide9  5    // Suicide limit for Army 9

#define Message1Turn 3
#define Message2Turn 5

int Factory12List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama,
                                             unHerkules, unSkorpion, unPatriot, unGolias,
                                             unXenon, unFalanga, unRex, unCeres, unGnom, 0};

TPoint MinePlaces12 [] = {91,119, 83,119, 81,134, 81,124, 35,150, 36,149,
                                                    38,149, 40,149, 42,150,
                                                    -1,-1};

static void Message1 () 
{
    SaySpeech ("mis12a", 2005);
}

static void Message2 () 
{
    SaySpeech ("mis12b", 2005);
}

void InitAI12 ()
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

    MoneyGoodlife = 5000;
    MoneyBadlife = 10000;
    MBPlaces = NULL;    
    DeniedPlaces = NULL;
        
    Factory1 = GetField (F1X, F1Y) -> Unit;
    Airport1 = GetField (A1X, A1Y) -> Unit;
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
    Army1 -> Status = asActive;
    Army1 -> WayPoint.x = F1X; Army1 -> WayPoint.y = F1Y;
    Army1 -> Insert (Factory1);
    Army1 -> Insert (Airport1);
    Army1 -> Insert (Base1);        
    Army1 -> SetAIConst (2);
    Army1 -> WayPoint.x = F1X; Army1 -> WayPoint.y = F1Y;

    Marine1 = new TMarine();
    Marine1 -> SetAR (M1X1, M1Y1, M1X2, M1Y2);
    Marine1 -> SetRadar (-1, -1);
    Marine1 -> InsertUnits ();

    AirArmy1 = new TAirArmy();
    AirArmy1 -> SetAR (AA1X1, AA1Y1, AA1X2, AA1Y2);
    AirArmy1 -> SetRadar (-1, -1);
    AirArmy1 -> InsertUnits ();
    AirArmy1 -> Status = asActive;
    AirArmy1 -> Insert (Factory1);
    AirArmy1 -> Insert (Airport1);
    AirArmy1 -> Insert (Base1);     
    AirArmy1 -> WayPoint.x = F1X; AirArmy1 -> WayPoint.y = F1Y;
    
    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> SetRadar (-1, -1);
    Army2 -> InsertUnits ();
    Army2 -> Status = asActive;
    Army2 -> WayPoint.x = F1X; Army2 -> WayPoint.y = F1Y;
    Army2 -> Insert (Factory1);
    Army2 -> Insert (Airport1);
    Army2 -> Insert (Base1);        
    Army2 -> SetAIConst (2);
    
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
    Army9 -> SetRadar (-1, -1);
    Army9 -> InsertUnits ();
    
    // schovame posily - vedeme je jako marine5
    Marine5 = new TMarine ();
    Marine5 -> SetAR (PX1, PY1, PX2, PY2);
    Marine5 -> SetRadar (-1, -1);
    Marine5 -> InsertUnits ();
    Marine5 -> DisplaceGL ();
    
    // Letadla do Airportu
    Airport = (TAirport *)Units [Airport1];
    for (i = TA1X1; i <= TA1X2; i++) {
        for (j = TA1Y1; j <= TA1Y2; j++) {
            u = GetAircraftAt (i, j);
            if (u != NULL) {
                u -> PlaceGround (FALSE);               
                Airport -> Inventory [Airport -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
                
            }
        }
    }

    for (i = 0; i < BADLIFE; i++) {
        if (Units [i] != NULL && Units [i] -> Type == unGargantua) {
            u = (TUnit *)Units [i];
            u -> PlaceGround (FALSE);               
            ((TBase*)Units [Base1]) -> Inventory [((TBase*)Units [Base1]) -> LoadedUnits++] = u -> ID;
            u -> X = u -> Y = -1;
        }
    }
    
    // Polozime miny    
    for (i = 0; MinePlaces12 [i].x != -1; i++) {
        MinePlace (MinePlaces12 [i].x, MinePlaces12 [i].y, BADLIFE);
    }

    // zamerime jednotku
    i = GetField (SelX, SelY) -> Unit;  
    Units [i] -> Select ();
    Units [i] -> Center ();

    UnlockDraw ();          

    RedrawMap ();

}




void LoadArtificialIntelligence12 (FILE *f)
{

    DoneArtificialIntelligence12 ();

    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);
    DUPos = 0;

    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Marine1 = new TMarine (f);
    AirArmy1 = new TAirArmy (f);        
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
    Army4 = new TGroundArmy (f);
    Army5 = new TGroundArmy (f);
    Army6 = new TGroundArmy (f);
    Army7 = new TGroundArmy (f);
    Army8 = new TGroundArmy (f);
    Army9 = new TGroundArmy (f);
    Marine5 = new TMarine (f);
}



void SaveArtificialIntelligence12 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);

    Towers -> Save (f);
    Army1 -> Save (f);
    Marine1 -> Save (f);
    AirArmy1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);      
    Army4 -> Save (f);
    Army5 -> Save (f);
    Army6 -> Save (f);
    Army7 -> Save (f);
    Army8 -> Save (f);
    Army9 -> Save (f);
    Marine5 -> Save (f);
    DUPos = 0;
}


int ArtificialIntelligence12 ()
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
    if (ActualTurn == Message2Turn) {
        Message2 ();
        Marine5 -> PlaceGL ();
        
    }
    
    
    // Aktualizujeme specialni objekty
    Factory1 = GetField (F1X, F1Y) -> Unit;
    if (Factory1 == NO_UNIT || Units [Factory1] -> Type % BADLIFE != unFactory) 
        Factory1 = NO_UNIT;
    Airport1 = GetField (A1X, A1Y) -> Unit;
    if (Airport1 == NO_UNIT || Units [Airport1] -> Type % BADLIFE != unAirport) 
        Airport1 = NO_UNIT;
    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;


    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Marine1 -> DeleteKilled2 (); Marine1 -> DeleteKilled ();
    AirArmy1 -> DeleteKilled2 (); AirArmy1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
    Army7 -> DeleteKilled2 (); Army7 -> DeleteKilled ();
    Army8 -> DeleteKilled2 (); Army8 -> DeleteKilled ();
    Army9 -> DeleteKilled2 (); Army9 -> DeleteKilled ();
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
    AirArmy1 -> DoAttackedFields ();
    Army2 -> DoAttackedFields ();
    Army3 -> DoAttackedFields ();
    Army4 -> DoAttackedFields ();
    Army5 -> DoAttackedFields ();
    Army6 -> DoAttackedFields ();
    Army7 -> DoAttackedFields ();
    Army8 -> DoAttackedFields ();
    Army9 -> DoAttackedFields ();
    AttackFieldPos = 0;
    
    AnalyseLandscape ();    
    
    Message (SigText[TXT_AI_ARMY]);



//////////////////////////////// AKTIVACE ARMAD

    // Test zaktivovani marine 1
    if (Marine1 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Marine1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Marine1 -> Status = asActive;
            }
        }
    }
    // Test zaktivovani air armady 1
    if (AirArmy1 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (AirArmy1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy1 -> Status = asActive;
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

    // ARMY 3 & 4 Special activation
    if (Army3 -> Status == asSleeping
    && (Army1 -> Status == asDestroyed || Army2 -> Status == asDestroyed)) {
        Army3 -> Status = asActive;
        Army3 -> WayPoint.x = F1X; Army3 -> WayPoint.y = F1Y;
        Army3 -> Insert (Factory1);
        Army3 -> Insert (Airport1);
        Army3 -> Insert (Base1);        
        Army3 -> SetAIConst (2);
    }

    if (Army4 -> Status == asSleeping
    && (Army1 -> Status == asDestroyed || Army2 -> Status == asDestroyed)) {
        Army4 -> Status = asActive;
        Army4 -> WayPoint.x = F1X; Army4 -> WayPoint.y = F1Y;
        Army4 -> Insert (Factory1);     
        Army4 -> Insert (Airport1);
        Army4 -> Insert (Base1);        
        Army4 -> SetAIConst (2);
    }
        

///////////////////////////// AKCE ARMAD

    // Nastavime ProgressBar
    st = 0;
    st += Towers -> nofBadLife;
    if (Army1 -> Status == asActive || Army1 -> Status == asSuicide) st += Army1 -> nofBadLife;
    if (Marine1 -> Status == asActive || Marine1 -> Status == asSuicide) st += Marine1 -> nofBadLife;
    if (AirArmy1 -> Status == asActive || AirArmy1 -> Status == asSuicide) st += AirArmy1 -> nofBadLife;
    if (Army2 -> Status == asActive || Army2 -> Status == asSuicide) st += Army2 -> nofBadLife;
    if (Army3 -> Status == asActive || Army3 -> Status == asSuicide) st += Army3 -> nofBadLife;
    if (Army4 -> Status == asActive || Army4 -> Status == asSuicide) st += Army4 -> nofBadLife;
    if (Army5 -> Status == asActive || Army5 -> Status == asSuicide) st += Army5 -> nofBadLife;
    if (Army6 -> Status == asActive || Army6 -> Status == asSuicide) st += Army6 -> nofBadLife;
    if (Army7 -> Status == asActive || Army7 -> Status == asSuicide) st += Army7 -> nofBadLife;
    if (Army8 -> Status == asActive || Army8 -> Status == asSuicide) st += Army8 -> nofBadLife;
    if (Army9 -> Status == asActive || Army9 -> Status == asSuicide) st += Army9 -> nofBadLife;
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


    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult12 ()
{
    int i, t = 0;
    int j, u = 0;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL && Units [i] -> Type == unGargantua) t++;
    if (t < 3) return -1; // GoodLife prohral   
    
    // Vsechny Garagntuy jsou v oasach
    for (i = 0; i < BADLIFE; i++) {
        if (Units [i] != NULL && Units [i] -> Type == unOasa) {
            for (j = 0; j < ((TOasa *)Units [i]) -> LoadedUnits; j++) {
                if (Units [((TOasa *)Units [i]) -> Inventory [j]] -> Type == unGargantua) {
                    u++;
                }
            }
        }
    }
    if (t == u) return 1; // Goodlife vyhral

    
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits12 (int FactoryID)
{
    return Factory12List;
}



void DoneArtificialIntelligence12 ()
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
    if (AirArmy1 != NULL) {delete AirArmy1; AirArmy1 = NULL; }
    if (Army2 != NULL) {delete Army2; Army2 = NULL; }
    if (Army3 != NULL) {delete Army3; Army3 = NULL; }
    if (Army4 != NULL) {delete Army4; Army4 = NULL; }
    if (Army5 != NULL) {delete Army5; Army5 = NULL; }
    if (Army6 != NULL) {delete Army6; Army6 = NULL; }
    if (Army7 != NULL) {delete Army7; Army7 = NULL; }
    if (Army8 != NULL) {delete Army8; Army8 = NULL; }
    if (Army9 != NULL) {delete Army9; Army9 = NULL; }
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
// OK   
