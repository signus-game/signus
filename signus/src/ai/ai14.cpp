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
//           Mission 14
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
#define A1X1  0
#define A1Y1 141
#define A1X2 18
#define A1Y2 159

// Action Radius of Army 2
#define A2X1 34
#define A2Y1 76
#define A2X2 48
#define A2Y2 90

// Action Radius of Army 3
#define A3X1 16
#define A3Y1 76
#define A3X2 34
#define A3Y2 91

// Action Radius of Army 4
#define A4X1 75
#define A4Y1 78
#define A4X2 94
#define A4Y2 101

// Action Radius of Army 5
#define A5X1 33
#define A5Y1 50
#define A5X2 46
#define A5Y2 62

// Action Radius of Army 6
#define A6X1  0
#define A6Y1 12
#define A6X2 33
#define A6Y2 35

// Action Radius of Army 7
#define A7X1 40
#define A7Y1 91
#define A7X2 66
#define A7Y2 104

// Action Radius of Air Army 2
#define AA2X1 38
#define AA2Y1 66
#define AA2X2 49
#define AA2Y2 76

// Action Radius of Air Army 4
#define AA4X1 86
#define AA4Y1 71
#define AA4X2 96
#define AA4Y2 80

// Posily
#define PX1 40
#define PY1 135
#define PX2 54
#define PY2 159

// WayPoint 1 - pro horni armady
#define WP1X 54
#define WP1Y 132

// WayPoint 1 - pro army 1 
#define WP2X 46
#define WP2Y 159

// Factory 1
#define F1X 77
#define F1Y 57

// Base 1
#define B1X 15
#define B1Y 14

// 1st selected unit
#define SelX 51
#define SelY 115



#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6
#define CMinDang7  0    // Dangerous limit for Army 7
#define CMinDangA2 0    // Dangerous limit for AirArmy 2
#define CMinDangA4 0    // Dangerous limit for AirArmy 4

#define CSuicide1  5    // Suicide limit for Army 1
#define CSuicide2  5    // Suicide limit for Army 2
#define CSuicide3  5    // Suicide limit for Army 3
#define CSuicide4  5    // Suicide limit for Army 4
#define CSuicide5  5    // Suicide limit for Army 5
#define CSuicide6  5    // Suicide limit for Army 6
#define CSuicide7  5    // Suicide limit for Army 7
#define CSuicideA2 5    // Suicide limit for AirArmy 2
#define CSuicideA4 5    // Suicide limit for AirArmy 4

TPoint MinePlaces14 [] = {44,89, 39,75, 29,80, 33,72, 80,110, 85,81, 91,86,
                                                    34,50, 44,48, 17,24, 14,20, 15,20, 16,20, 15,19,
                                                    -1,-1};


int Factory14List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unPerseusGama, 
                                             unRex, unHerkules, unSkorpion, unPatriot, unGolias, 
                                             unCeres, unGnom, unIris, 0};
TPoint MBPlaces14 [] = {23,154, 22,154, 21,154, 20,154, 19,154,
                                                23,155, 22,155, 21,155, 20,155, 19,155,
                                                24,156, 23,156, 22,156, 21,156, 20,156, 19,156,
                                                24,157, 23,157, 22,157, 21,157, 20,157, 19,157,
                                                24,158, 23,158, 22,158, 21,158, 20,158,
                                                24,159, 23,159, 22,159, 21,159, 20,159,
                                                31,146, 30,146, 29,146, 28,146, 
                                                27,146, 26,146, 25,146, 24,146,
                                                30,147, 29,147, 28,147, 27,147, 
                                                26,147, 25,147, 24,147, 23,147, 22,147, 

                       -1,-1};



void InitAI14 ()
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
    MBPlaces = MBPlaces14;  
    DeniedPlaces = NULL;

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
    Army1 -> Status = asActive;
    Army1 -> WayPoint.x = WP2X; Army1 -> WayPoint.y = WP2Y;
    Army1 -> SetAIConst (3);

    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> SetRadar (-1, -1);
    Army2 -> InsertUnits ();
    Army2 -> Status = asActive;
    Army2 -> WayPoint.x = WP1X; Army2 -> WayPoint.y = WP1Y;
    
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
    Army7 -> Status = asActive;

    AirArmy2 = new TAirArmy();
    AirArmy2 -> SetAR (AA2X1, AA2Y1, AA2X2, AA2Y2);
    AirArmy2 -> SetRadar (-1, -1);
    AirArmy2 -> InsertUnits ();
    AirArmy2 -> Status = asActive;
    
    AirArmy4 = new TAirArmy();
    AirArmy4 -> SetAR (AA4X1, AA4Y1, AA4X2, AA4Y2);
    AirArmy4 -> SetRadar (-1, -1);
    AirArmy4 -> InsertUnits ();
    AirArmy4 -> Status = asActive;
    
    // Posily
    Army10 = new TGroundArmy ();
    Army10 -> SetAR (PX1, PY1, PX2, PY2);
    Army10 -> SetRadar (-1, -1);
    Army10 -> InsertUnits ();
    Army10 -> DisplaceGL ();
        
    // Polozime miny    
    for (i = 0; MinePlaces14 [i].x != -1; i++) {
        MinePlace (MinePlaces14 [i].x, MinePlaces14 [i].y, BADLIFE);
    }
    UnlockDraw ();          
    
    // zamerime jednotku
    i = GetField (SelX, SelY) -> Unit;  
    Units [i] -> Select ();

    RedrawMap ();

}




void LoadArtificialIntelligence14 (FILE *f)
{

    DoneArtificialIntelligence14 ();

    MBPlaces = MBPlaces14;  
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
    Army10 = new TGroundArmy (f);
    AirArmy2 = new TAirArmy (f);        
    AirArmy4 = new TAirArmy (f);        
}



void SaveArtificialIntelligence14 (FILE *f)
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
    Army10 -> Save (f);
    AirArmy2 -> Save (f);
    AirArmy4 -> Save (f);
    DUPos = 0;
}


int ArtificialIntelligence14 ()
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
    AirArmy2 -> DeleteKilled2 (); AirArmy2 -> DeleteKilled ();
    AirArmy4 -> DeleteKilled2 (); AirArmy4 -> DeleteKilled ();
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
    AirArmy2 -> DoAttackedFields ();
    AirArmy4 -> DoAttackedFields ();
    AttackFieldPos = 0;
    
    AnalyseLandscape ();    
    
    Message (SigText[TXT_AI_ARMY]);



//////////////////////////////// AKTIVACE ARMAD

    // Army1 & AirArmy2 special Activation
    if ((Army7 -> Status == asDestroyed || Army2 -> Status == asSuicide)) {
        AirArmy2 -> WayPoint.x = WP1X; AirArmy2 -> WayPoint.y = WP1Y;
    }
    if (Army1 -> WayPoint.x == WP2X && Army1 -> IsAtWP ()) {
        Army1 -> SetAIConst (2);
        Army1 -> WayPoint.x = -1; Army1 -> WayPoint.y = -1;
    }
        
    // prijdou posily
    if (Army10 -> Status == asSleeping && (Army1 -> Status == asDestroyed || Army1 -> Status == asSuicide)) {
        Army10 -> PlaceGL ();
        Army10 -> Status = asDestroyed;
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
    if (AirArmy2 -> Status == asActive || AirArmy2 -> Status == asSuicide) st += AirArmy2 -> nofBadLife;
    if (AirArmy4 -> Status == asActive || AirArmy4 -> Status == asSuicide) st += AirArmy4 -> nofBadLife;
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


    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult14 ()
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



int *GetManufacturableUnits14 (int FactoryID)
{
    return Factory14List;
}



void DoneArtificialIntelligence14 ()
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
    if (AirArmy2 != NULL) {delete AirArmy2; AirArmy2 = NULL; }
    if (AirArmy4 != NULL) {delete AirArmy4; AirArmy4 = NULL; }
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
    
// OK
