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
//           Mission 2
//
// Programmed by Richard Wunsch
//

#include "headers.h"
#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"

// Action Radius of Army 1
#define A1X1  8
#define A1Y1 61
#define A1X2 26
#define A1Y2 76

// Action Radius of Army 2
#define A2X1  0
#define A2Y1 38
#define A2X2 23
#define A2Y2 51


// Action Radius of Army 3
#define A3X1 35
#define A3Y1 41
#define A3X2 64
#define A3Y2 58

// Action Radius of Army 4
#define A4X1 22
#define A4Y1 23
#define A4X2 51
#define A4Y2 33

// Action Radius of Army 5
#define A5X1  9
#define A5Y1 17
#define A5X2 31
#define A5Y2 37

// These units will be placed to Base
#define TB1X1 41
#define TB1Y1 87
#define TB1X2 43
#define TB1Y2 88

// Base1
#define B1X 31
#define B1Y 83

// Base2
#define B2X 32
#define B2Y  6

// Radar of Army1
#define R1X 13
#define R1Y 61

// Radar of Army2
#define R2X 11
#define R2Y 41

// Radar of Army3
#define R3X 47
#define R3Y 47

// Radar of Army4
#define R4X 31
#define R4Y 28

// Radar of Army4
#define R5X 0
#define R5Y 0


// GL Radar at Base 1
#define R6X 38
#define R6Y 81

// GL Radar at Base 2
#define R7X 21
#define R7Y  2

// GL Tower1
#define T1X 28
#define T1Y 79

// GL Tower2
#define T2X 39
#define T2Y 79

// Warehouse 1
#define W1X 35
#define W1Y 85

// Warehouse2
#define W2X 37
#define W2Y 83

// Warehouse3
#define W3X 39
#define W3Y  5



// Artefact Transporter
#define ATX 43
#define ATY 87


// Attack Activation Zone for Army 2 of ArtefactTrans
#define AAZ2X1 26
#define AAZ2Y1 38
#define AAZ2X2 64
#define AAZ2Y2 70

// Attack Activation Zone for Army 5 of ArtefactTrans
#define AAZ5X1  0
#define AAZ5Y1  0
#define AAZ5X2 33
#define AAZ5Y2 34

// Escape point for Army 1 
#define CEscPoint1X 13    
#define CEscPoint1Y 46

// Army 2 WayPoint 1 - Pres most
#define A2WP1X 12
#define A2WP1Y 71

// Army 2 WayPoint 2 - Base1
#define A2WP2X 32
#define A2WP2Y 84

// Army 2 WayPoint 3 - Pred mosty
#define A2WP3X 50
#define A2WP3Y 62

// Army 2 WayPoint 4 - Za mosty
#define A2WP4X 51
#define A2WP4Y 31

// Army 2 WayPoint 5 - Base2
#define A2WP5X 32
#define A2WP5Y  6



#define CMinDang1 5    // Dangerous limit for Army 1
#define CMinDang2 0    // Dangerous limit for Army 2
#define CMinDang3 0    // Dangerous limit for Army 3
#define CMinDang4 0    // Dangerous limit for Army 4
#define CMinDang5 0    // Dangerous limit for Army 5

#define CSuicide2 4    // Suicide limit for Army 2
#define CSuicide3 3    // Suicide limit for Army 3
#define CSuicide4 3    // Suicide limit for Army 4
#define CSuicide5 3    // Suicide limit for Army 5




void InitAI2 ()
{
    int i,j;
    TBase *Base;
    TField *f;
    TUnit *u;

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 500;
    MoneyBadlife = 0;
    MBPlaces = NULL;    
    DeniedPlaces = NULL;
        
    Base1 = GetField (B1X, B1Y) -> Unit;
    Base2 = GetField (B2X, B2Y) -> Unit;
    Tower1 = GetField (T1X, T1Y) -> Unit;
    Tower2 = GetField (T2X, T2Y) -> Unit;
    Warehouse1 = GetField (W1X, W1Y) -> Unit;   
    Warehouse2 = GetField (W2X, W2Y) -> Unit;   
    Warehouse3 = GetField (W3X, W3Y) -> Unit;   

    ArtefactTrans = GetField (ATX, ATY) -> Unit;
                        
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
    Army1 -> SetAIConst (2); // AGRESIVE ARMY
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
    Army4 -> SetRadar (R4X, R4Y);
    Army4 -> SetAIConst (4); // SCATTERED ARMY
    Army4 -> InsertUnits ();
    
    Army5 = new TGroundArmy();
    Army5 -> SetAR (A5X1, A5Y1, A5X2, A5Y2);
    Army5 -> SetRadar (R5X, R5Y);
    Army5 -> InsertUnits ();


    // Jednotky do zakladny
    LockDraw ();    
    Base = (TBase*)Units[Base1];
    for (i = TB1X1; i <= TB1X2; i++) {
        for (j = TB1Y1; j <= TB1Y2; j++) {
            f = GetField (i, j);
            if ((f -> Unit != NO_UNIT) && (f -> Unit != Base -> ID)) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                Base -> Inventory [Base -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    UnlockDraw ();          
    
    Units [Base1] -> Select ();

}




void LoadArtificialIntelligence2 (FILE *f)
{

    DoneArtificialIntelligence2 ();

    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);  
    DUPos = 0;

    fread (&ArtefactTrans, sizeof (int), 1, f);
    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
    Army4 = new TGroundArmy (f);
    Army5 = new TGroundArmy (f);
}



void SaveArtificialIntelligence2 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);

    fwrite (&ArtefactTrans, sizeof (int), 1, f);
    Towers -> Save (f);
    Army1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);      
    Army4 -> Save (f);
    Army5 -> Save (f);
    DUPos = 0;
}



int ArtificialIntelligence2 ()
{
    int i, j, r, st, x, y;
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
    Base1 = GetField (B1X, B1Y) -> Unit;
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    Base2 = GetField (B2X, B2Y) -> Unit;
    if (Base2 == NO_UNIT || Units [Base2] -> Type % BADLIFE != unBase) 
        Base2 = NO_UNIT;    
    Tower1 = GetField (T1X, T1Y) -> Unit;
    if (Tower1 == NO_UNIT || Units [Tower1] -> Type % BADLIFE != unPagoda) 
        Tower1 = NO_UNIT;
    Tower2 = GetField (T2X, T2Y) -> Unit;
    if (Tower2 == NO_UNIT || Units [Tower2] -> Type % BADLIFE != unPagoda) 
        Tower2 = NO_UNIT;
    Warehouse1 = GetField (W1X, W1Y) -> Unit;   
    if (Warehouse1 == NO_UNIT || Units [Warehouse1] -> Type % BADLIFE != unWarehouse) 
        Warehouse1 = NO_UNIT;
    Warehouse2 = GetField (W2X, W2Y) -> Unit;   
    if (Warehouse2 == NO_UNIT || Units [Warehouse2] -> Type % BADLIFE != unWarehouse) 
        Warehouse2 = NO_UNIT;
    Warehouse3 = GetField (W3X, W3Y) -> Unit;   
    if (Warehouse3 == NO_UNIT || Units [Warehouse3] -> Type % BADLIFE != unWarehouse) 
        Warehouse3 = NO_UNIT;
    

    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
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


    // Zaktivujeme Army2, kdyz je Transporter na ceste a posleme ji na WP1
    if (IsInRect (Units [ArtefactTrans] -> X, Units [ArtefactTrans] -> Y, 
    AAZ2X1, AAZ2Y1, AAZ2X2, AAZ2Y2)) {
        Army2 -> WayPoint.x = A2WP1X;
        Army2 -> WayPoint.y = A2WP1Y;
        Army2 -> Status = asActive;
        Army2 -> SetAIConst (3); // MOVING ARMY
    }

    // WP1 dosazen - posleme ARMY 2 do utoku na Base1 a WP2
    for (i = 0; i < Army2 -> nofBadLife; i++) {
        if (IsInRect (Units [Army2 -> BL [i]] -> X, Units [Army2 -> BL [i]] -> Y, 
        A2WP1X - 5, A2WP1Y - 5, A2WP1X + 5, A2WP1Y + 5)) {
            Army2 -> WayPoint.x = A2WP2X;
            Army2 -> WayPoint.y = A2WP2Y;
            Army2 -> SetAIConst (1); // STANDART ARMY
            if (Base1 < BADLIFE) Army2 -> Insert (Base1);
            if (Tower1 < BADLIFE) Army2 -> Insert (Tower1);
            if (Tower2 < BADLIFE) Army2 -> Insert (Tower2);
            if (Warehouse1 < BADLIFE) Army2 -> Insert (Warehouse1);
            if (Warehouse2 < BADLIFE) Army2 -> Insert (Warehouse2);
        }
    }

    // Posleme Army 2 na WP3
    if (Base1 >= BADLIFE && Tower1 == NO_UNIT && Tower2 == NO_UNIT 
    && Warehouse1 >= BADLIFE && Warehouse2 >= BADLIFE) { 
    // Base1 zajata nebo znicena && veze zniceny
        Army2 -> Status = asActive;
        Army2 -> WayPoint.x = A2WP3X;
        Army2 -> WayPoint.y = A2WP3Y;
        Army2 -> SetAIConst (3); // MOVING ARMY     
    }

    // WP3 dosazen - posleme ARMY 2 na WP4
    for (i = 0; i < Army2 -> nofBadLife; i++) {
        if (IsInRect (Units [Army2 -> BL [i]] -> X, Units [Army2 -> BL [i]] -> Y, 
        A2WP3X - 5, A2WP3Y - 5, A2WP3X + 5, A2WP3Y + 5)) {
            Army2 -> WayPoint.x = A2WP4X;
            Army2 -> WayPoint.y = A2WP4Y;
            Army2 -> SetAIConst (3); // MOVING ARMY
        }
    }


    // WP4 dosazen - posleme ARMY 2 do utoku na Base2 a AT a WP5
    for (i = 0; i < Army2 -> nofBadLife; i++) {
        if (IsInRect (Units [Army2 -> BL [i]] -> X, Units [Army2 -> BL [i]] -> Y, 
        A2WP4X - 5, A2WP4Y - 5, A2WP4X + 5, A2WP4Y + 5)) {
            Army2 -> WayPoint.x = A2WP5X;
            Army2 -> WayPoint.y = A2WP5Y;
            Army2 -> SetAIConst (2); // AGRESIVE ARMY
            Army2 -> Insert (Base2);
            Army2 -> Insert (ArtefactTrans);
        }
    }

    // Zaktivujeme Army5, kdyz je Transporter pred cilem
    if (IsInRect (Units [ArtefactTrans] -> X, Units [ArtefactTrans] -> Y, 
    AAZ5X1, AAZ5Y1, AAZ5X2, AAZ5Y2)) {
        Army5 -> WayPoint.x = A2WP5X;
        Army5 -> WayPoint.y = A2WP5Y;
        Army5 -> Status = asActive;
        Army5 -> Insert (ArtefactTrans);
        Army5 -> SetAIConst (2); // AGRESIVE ARMY
    }

        
    // Nastavime ProgressBar
    st = 0;
    st += Towers -> nofBadLife;
    if (Army1 -> Status == asActive || Army1 -> Status == asSuicide) st += Army1 -> nofBadLife;
    if (Army2 -> Status == asActive || Army2 -> Status == asSuicide) st += Army2 -> nofBadLife;
    if (Army3 -> Status == asActive || Army3 -> Status == asSuicide) st += Army3 -> nofBadLife;
    if (Army4 -> Status == asActive || Army4 -> Status == asSuicide) st += Army4 -> nofBadLife;
    if (Army5 -> Status == asActive || Army5 -> Status == asSuicide) st += Army5 -> nofBadLife;
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
            for (i = 0; i < Army1 -> nofBadLife; i++) {
                Units [Army1 -> BL [i]] -> AI_Info.State = aistMoving;
                Units [Army1 -> BL [i]] -> AI_Info.point.x = CEscPoint1X;
                Units [Army1 -> BL [i]] -> AI_Info.point.y = CEscPoint1Y;
            }
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
            if ((Units [i] != NULL) && (IsInRect (Units [i] -> AI_Info.point.x, Units [i] 
            -> AI_Info.point.y, Army2 -> ActionRadius)) && (st == TRUE)) {
                Units [i] -> AI_Info.State = aistNought;
                Army2 -> Insert (i);
            }
        }
    }
    LockDraw ();
    ProgressNull ();

    
    // Zruseni transportu jednotek po aktivaci Army 2
    if (Army2 -> Status == asActive) {
        for (i = BADLIFE; i < UNITS_TOP; i++) {
            if (Units [i] == NULL) continue;
            if (Units [i] -> AI_Info.State == aistMoving && 
            IsInRect (Units [i] -> AI_Info.point.x, Units [i] -> AI_Info.point.y
            , Army2 -> ActionRadius)) {  // je-li cilem moving unit Army2
                Units [i] -> AI_Info.State = aistNought;
                Army2 -> Insert (i);
            }
        }
    }
    
    AttackFieldPos = 0; 
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}



int *GetManufacturableUnits2 (int FactoryID)
{
    return GFactoryList;
}



int AssignResult2 ()
{
    int i, t = 0;

    Base2 = GetField (B2X, B2Y) -> Unit;
    if (Base2 == NO_UNIT || Units [Base2] -> Type % BADLIFE != unBase) 
        Base2 = NO_UNIT;    
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    if (Units [ArtefactTrans] == NULL || Units [ArtefactTrans] -> Type != unGargantua) return -1; // GL prohral
    
    if (Base2 == NO_UNIT || Units [Base2] == NULL) return -1; // GL prohral
 
    if (Base2 < BADLIFE) {
        for (i = 0; i < ((TBase *)Units [Base2]) -> LoadedUnits; i++) {
            if (((TBase *)Units [Base2]) -> Inventory [i] == ArtefactTrans) {
                return 1; // GL vyhral
            }
        }
    }
    
    return 0; // Zatim se jeste hraje
}



void DoneArtificialIntelligence2 ()
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
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
    
// OK
