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
//           Mission 10
//
// Programmed by Marek Wunsch
//


#include "headers.h"
#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "fields.h"
#include "aiair.h"



// Base 8
#define B8X 147
#define B8Y 38

// Action Radius of Army 1
#define A1X1 32
#define A1Y1 57
#define A1X2 47
#define A1Y2 73

// Action Radius of Army 2
#define A2X1 37
#define A2Y1 29
#define A2X2 46
#define A2Y2 43

// Action Radius of Army 3
#define A3X1 58
#define A3Y1 34
#define A3X2 81
#define A3Y2 63

// Action Radius of Army 4
#define A4X1 81
#define A4Y1 35
#define A4X2 96
#define A4Y2 68

// Action Radius of Army 5
#define A5X1 107
#define A5Y1 39
#define A5X2 129
#define A5Y2 63

// Action Radius of Army 6
#define A6X1 113
#define A6Y1 11
#define A6X2 128
#define A6Y2 25

// Action Radius of Army 7
#define A7X1 116
#define A7Y1 58
#define A7X2 136
#define A7Y2 82

// Action Radius of Army 8
#define A8X1 127
#define A8Y1 25
#define A8X2 159
#define A8Y2 68

// Action radius of AirArmy1
#define AA1X1 145
#define AA1Y1 55
#define AA1X2 148
#define AA1Y2 60


// Uran 1
#define U1X 8 
#define U1Y 66

#define TU1X1 7 
#define TU1Y1 67
#define TU1X2 9 
#define TU1Y2 67
// Uran 2
#define U2X 5 
#define U2Y 61

#define TU2X1 4 
#define TU2Y1 60
#define TU2X2 6 
#define TU2Y2 60
// Uran 3
#define U3X 8 
#define U3Y 56

#define TU3X1 7 
#define TU3Y1 55
#define TU3X2 9 
#define TU3Y2 55


// Radar of Army3
#define R3X 67
#define R3Y 37


// Radar of Army8
#define R8X 141
#define R8Y 48



//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define CMinDang1 0    // Dangerous limit for Army 1
#define CMinDang2 0    // Dangerous limit for Army 2
#define CMinDang3 0    // Dangerous limit for Army 1
#define CMinDang4 0    // Dangerous limit for Army 2
#define CMinDang5 0    // Dangerous limit for Army 2
#define CMinDang6 0    // Dangerous limit for Army 2
#define CMinDang7 0    // Dangerous limit for Army 2
#define CMinDang8 0    // Dangerous limit for Army 2
#define CMinDangA1 0    // Dangerous limit for AirArmy 1

#define CSuicide1 5    // Suicide limit for Army 1
#define CSuicide2 5    // Suicide limit for Army 2
#define CSuicide3 5    // Suicide limit for Army 1
#define CSuicide4 5    // Suicide limit for Army 2
#define CSuicide5 5    // Suicide limit for Army 2
#define CSuicide6 5    // Suicide limit for Army 2
#define CSuicide7 5    // Suicide limit for Army 2
#define CSuicide8 5    // Suicide limit for Army 2
#define CSuicideA1 5    // Suicide limit for Army 1



TPoint MinePlaces10 [] = { 41,47,  50,48,  48,61,  56,58,  58,52, 
                           67,58,  65,47,  96,60,  94,45, 102,53,
                          113,55, 119,65, 130,53, 124,41, 138,49,
                           50,54,  56,43, 80,51, 81,37, 86,52, 
                          103,44, 112,40, 131,43, 
                                                -1,-1};


void InitAI10 ()
{
    int i,j;
    TBase *Base;
    TField *f;
    TUnit *u;
    
    LockDraw();

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 500;
    MoneyBadlife = 2000;
    MBPlaces = NULL;
    DeniedPlaces = NULL;
    

    Base8 = GetField (B8X, B8Y) -> Unit;
    Uran1 = GetField (U1X, U1Y) -> Unit;
    Uran2 = GetField (U2X, U2Y) -> Unit;
    Uran3 = GetField (U3X, U3Y) -> Unit;
    
    
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
    Army1 -> InsertUnits ();


    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> InsertUnits ();
    
    Army3 = new TGroundArmy();
    Army3 -> SetAR (A3X1, A3Y1, A3X2, A3Y2);
    Army3 -> SetRadar (R3X, R3Y);
    Army3 -> InsertUnits ();

    Army4 = new TGroundArmy();
    Army4 -> SetAR (A4X1, A4Y1, A4X2, A4Y2);
    Army4 -> InsertUnits ();

    Army5 = new TGroundArmy();
    Army5 -> SetAR (A5X1, A5Y1, A5X2, A5Y2);
    Army5 -> InsertUnits ();

    Army6 = new TGroundArmy();
    Army6 -> SetAR (A6X1, A6Y1, A6X2, A6Y2);
    Army6 -> InsertUnits ();

    Army7 = new TGroundArmy();
    Army7 -> SetAR (A7X1, A7Y1, A7X2, A7Y2);
    Army7 -> InsertUnits ();
    
    Army8 = new TGroundArmy();
    Army8 -> SetAR (A8X1, A8Y1, A8X2, A8Y2);
    Army8 -> SetRadar (R8X, R8Y);
    Army8 -> InsertUnits ();

    AirArmy1 = new TAirArmy ();
    AirArmy1 -> SetAR (AA1X1, AA1Y1, AA1X2, AA1Y2); 
    AirArmy1 -> Status = asActive;
    AirArmy1 -> InsertUnits ();


    // Jednotky do Uranu 1
    for (i = TU1X1; i <= TU1X2; i++) {
        for (j = TU1Y1; j <= TU1Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran1]) -> Inventory [((TUran*)Units [Uran1]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    
    // Jednotky do Uranu 2 
    for (i = TU2X1; i <= TU2X2; i++) {
        for (j = TU2Y1; j <= TU2Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran2]) -> Inventory [((TUran*)Units [Uran2]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       

    // Jednotky do Uranu 3
    for (i = TU3X1; i <= TU3X2; i++) {
        for (j = TU3Y1; j <= TU3Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran3]) -> Inventory [((TUran*)Units [Uran3]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       


    // Polozime miny
    
    for (i = 0; MinePlaces10 [i].x != -1; i++) {
        MinePlace (MinePlaces10 [i].x, MinePlaces10 [i].y, BADLIFE);
    }
    UnlockDraw ();          
    
}




void LoadArtificialIntelligence10 (FILE *f)
{
    DoneArtificialIntelligence10 ();

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
    AirArmy1 = new TAirArmy (f);
}



void SaveArtificialIntelligence10 (FILE *f)
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
    AirArmy1 -> Save (f);
    DUPos = 0;
}



int ArtificialIntelligence10 ()
{
    int i, j, k, r, st, x, y, id;
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
    
    // Aktualizujeme specialni objekty
    Base8 = GetField (B8X, B8Y) -> Unit;
    if (Base8 == NO_UNIT || Units [Base8] -> Type % BADLIFE != unBase) 
        Base8 = NO_UNIT;
        
    Radar3 = GetField (R3X, R3Y) -> Unit;
    if (Radar2 == NO_UNIT || Units [Radar3] -> Type % BADLIFE != unRadar) 
        Radar3 = NO_UNIT;

    Radar8 = GetField (R8X, R8Y) -> Unit;
    if (Radar8 == NO_UNIT || Units [Radar8] -> Type % BADLIFE != unRadar) 
        Radar8 = NO_UNIT;
        
    // Vyradime znicene jednotky z armad        
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2(); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
    Army7 -> DeleteKilled2 (); Army7 -> DeleteKilled ();
    Army8 -> DeleteKilled2 (); Army8 -> DeleteKilled ();
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
    AirArmy1 -> DoAttackedFields ();
    AttackFieldPos = 0;

    AnalyseLandscape ();    
    
    Message (SigText[TXT_AI_ARMY]); 
    
    
    // Test zaktivovani armady 1
    if (Army1 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army1 -> Status = asActive;
                Army1 -> SetAIConst (2);
            }
        }
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
                Army3 -> SetAIConst (2);
            }
        }
    }

    // Test zaktivovani armady 4
    if (Army4 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army4 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army4 -> Status = asActive;
                Army4 -> SetAIConst (2);
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
                Army7 -> SetAIConst (1);
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
        AirArmy1 -> CountDangerous ();      
        if (AirArmy1 -> Dangerous <= CMinDangA1) {
            AirArmy1 -> Status = asDestroyed;           
        }
        
        
    }



    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();
    
}



int *GetManufacturableUnits10 (int FactoryID)
{
    return GFactoryList;
}




int AssignResult10 ()
{
    int i, t = 0;

    Base8 = GetField (B8X, B8Y) -> Unit;
    if (Base8 == NO_UNIT || Units [Base8] -> Type % BADLIFE != unBase) 
        Base8 = NO_UNIT;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL && Units [i] -> Type < unRadar) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if (Base8 < BADLIFE || Base8 == NO_UNIT) {
        return 1; 
    }
        // GoodLife vyhral - obsazena nepratelska zakladna
    
    return 0; // Zatim se jeste hraje
}



void DoneArtificialIntelligence10 ()
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
    if (AirArmy1 != NULL) {delete AirArmy1; AirArmy1 = NULL; }  
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
// OK   
