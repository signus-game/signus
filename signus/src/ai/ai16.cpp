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
//           Mission 16
//
// Programmed by Marek Wunsch
//



#include "headers.h"
#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "fields.h"



// Action Radius of Army 1
#define A1X1 24
#define A1Y1 112
#define A1X2 52
#define A1Y2 142

// Action Radius of Army 2
#define A2X1 52
#define A2Y1 104
#define A2X2 71
#define A2Y2 119

// Action Radius of Army 3
#define A3X1 59
#define A3Y1 66
#define A3X2 76
#define A3Y2 83

// Action Radius of Army 4
#define A4X1 76
#define A4Y1 89
#define A4X2 94
#define A4Y2 106

// Action Radius of Army 5
#define A5X1 84
#define A5Y1 67
#define A5X2 101
#define A5Y2 83

// Action Radius of Army 6
#define A6X1 82
#define A6Y1 39
#define A6X2 98
#define A6Y2 55

// Action Radius of Army 7
#define A7X1 104
#define A7Y1 53
#define A7X2 122
#define A7Y2 69

// Action Radius of Army 8
#define A8X1 117
#define A8Y1 80
#define A8X2 137
#define A8Y2 104

// Action Radius of Army 9
#define A9X1 109
#define A9Y1 102
#define A9X2 130
#define A9Y2 122

// Action Radius of Army 10
#define A10X1 84
#define A10Y1 144
#define A10X2 968
#define A10Y2 155

// Radar of Army10
#define R10X 70
#define R10Y 86



// Neptun 1
#define N1X 9 
#define N1Y 131
#define TN1X1 7 
#define TN1Y1 130
#define TN1X2 9 
#define TN1Y2 130

// Neptun 2
#define N2X 11
#define N2Y 135
#define TN2X1 9 
#define TN2Y1 134
#define TN2X2 11
#define TN2Y2 134

// Neptun 3
#define N3X 17
#define N3Y 150
#define TN3X1 15
#define TN3Y1 149
#define TN3X2 17
#define TN3Y2 149

// Neptun 4
#define N4X 20
#define N4Y 156
#define TN4X1 18
#define TN4Y1 155
#define TN4X2 20
#define TN4Y2 155



//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define CMinDang1  0    // Dangerous limit for Army  
#define CMinDang2  0    // Dangerous limit for Army  
#define CMinDang3  0    // Dangerous limit for Army   
#define CMinDang4  0    // Dangerous limit for Army  
#define CMinDang5  0    // Dangerous limit for Army  
#define CMinDang6  0    // Dangerous limit for Army  
#define CMinDang7  0    // Dangerous limit for Army  
#define CMinDang8  0    // Dangerous limit for Army  
#define CMinDang9  0    // Dangerous limit for Army  
#define CMinDang10 0    // Dangerous limit for Army  

#define CSuicide1  4    // Suicide limit for Army  
#define CSuicide2  4    // Suicide limit for Army  
#define CSuicide3  4    // Suicide limit for Army  
#define CSuicide4  4    // Suicide limit for Army  
#define CSuicide5  4    // Suicide limit for Army  
#define CSuicide6  4    // Suicide limit for Army  
#define CSuicide7  4    // Suicide limit for Army  
#define CSuicide8  4    // Suicide limit for Army  
#define CSuicide9  4    // Suicide limit for Army  
#define CSuicide10 4    // Suicide limit for Army  


//int Factory2List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, unOlymp, unMohykan, unMesias, unXenon, 0};

TPoint MinePlaces16 [] = { 36,114, 43,117, 50,116, 52,122, 62,121, 51,106,
                                                     55,100, 65,92, 70,94, 67,95, 72,101, 69,104,
                                                     76,104, 81,77, 84,87, 96,92, 110,98, 110,106,
                                                     111,70, 117,66, 103,60, 84,74, 77,76, 67,87,
                                                     70,87, 65,89, -1,-1};


void InitAI16 ()
{
    int i,j;
    TBase *Base;
    TField *f;
    TUnit *u;
    
    LockDraw();

    AttackFieldPos = 0;
    DUPos = 0;
    MBPlaces = NULL;
    DeniedPlaces = NULL;
    MoneyGoodlife = 0;
    MoneyBadlife = 0;


    Neptun1 = GetField (N1X, N1Y) -> Unit;
    Neptun2 = GetField (N2X, N2Y) -> Unit;
    Neptun3 = GetField (N3X, N3Y) -> Unit;
    Neptun4 = GetField (N4X, N4Y) -> Unit;

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
    Army8 -> InsertUnits ();
    
    Army9 = new TGroundArmy();
    Army9 -> SetAR (A9X1, A9Y1, A9X2, A9Y2);
    Army9 -> InsertUnits ();
    
    Army10 = new TGroundArmy();
    Army10 -> SetAR (A10X1, A10Y1, A10X2, A10Y2);
    Army10 -> SetRadar (R10X, R10Y);
    Army10 -> InsertUnits ();


    // Jednotky do Neptuna 1
    for (i = TN1X1; i <= TN1X2; i++) {
        for (j = TN1Y1; j <= TN1Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TNeptun*)Units [Neptun1]) -> Inventory [((TNeptun*)Units [Neptun1]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       

    // Jednotky do Neptuna 2
    for (i = TN2X1; i <= TN2X2; i++) {
        for (j = TN2Y1; j <= TN2Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TNeptun*)Units [Neptun2]) -> Inventory [((TNeptun*)Units [Neptun2]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       

    // Jednotky do Neptuna 3
    for (i = TN3X1; i <= TN3X2; i++) {
        for (j = TN3Y1; j <= TN3Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TNeptun*)Units [Neptun3]) -> Inventory [((TNeptun*)Units [Neptun3]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       

    // Jednotky do Neptuna 4
    for (i = TN4X1; i <= TN4X2; i++) {
        for (j = TN4Y1; j <= TN4Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TNeptun*)Units [Neptun4]) -> Inventory [((TNeptun*)Units [Neptun4]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       


    // Polozime miny
    
    for (i = 0; MinePlaces16 [i].x != -1; i++) {
        MinePlace (MinePlaces16 [i].x, MinePlaces16 [i].y, BADLIFE);
    }
    UnlockDraw ();          
    
}




void LoadArtificialIntelligence16 (FILE *f)
{
    DoneArtificialIntelligence16 ();

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
    Army10= new TGroundArmy (f);
}



void SaveArtificialIntelligence16 (FILE *f)
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
    Army10-> Save (f);
    DUPos = 0;
}



int ArtificialIntelligence16 ()
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
    Radar10= GetField (R10X, R10Y) -> Unit;
    if (Radar10== NO_UNIT || Units [Radar10] -> Type % BADLIFE != unRadar) 
        Radar10= NO_UNIT;
        
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
    Army10-> DeleteKilled2 (); Army10-> DeleteKilled ();
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
    Army10-> DoAttackedFields ();
    AttackFieldPos = 0;

    AnalyseLandscape ();    
    
    Message (SigText[TXT_AI_ARMY]);
    
    
    
    // Test zaktivovani armady 1
    if (Army1 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army1 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army1 -> Status = asActive;
                Army1 -> SetAIConst (1);
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
                Army5 -> SetAIConst (1);
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
    
    // Test zaktivovani armady 9
    if (Army9 -> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army9 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army9 -> Status = asActive;
                Army9 -> SetAIConst (1);
            }
        }
    }
    // Test zaktivovani armady 10
    if (Army10-> Status == asSleeping) {
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;
            if (Army10-> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                Army10-> Status = asActive;
                Army10-> SetAIConst (2);
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
    if (Army9 -> Status == asActive || Army9 -> Status == asSuicide) st += Army9 -> nofBadLife;
    if (Army10 -> Status == asActive || Army10 -> Status == asSuicide) st += Army10 -> nofBadLife;
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
//  LockDraw();
    return AssignResult ();

    
}



int *GetManufacturableUnits16 (int FactoryID)
{
    return GFactoryList;
}




int AssignResult16 ()
{
    int i, t = 0;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL && Units [i] -> Type < unRadar) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    t = 0;
    for (i = BADLIFE; i < UNITS_TOP; i++)
        if ((Units [i] != NULL) && (Units [i] -> Type % BADLIFE  < unRadar)) t++;
    if (t == 0) return 1; // BadLife prohral - prisel o vsechno
    
    return 0; // Zatim se jeste hraje
}



void DoneArtificialIntelligence16 ()
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
    if (Army9 != NULL) {delete Army8; Army9 = NULL; }
    if (Army10 != NULL) {delete Army8; Army10 = NULL; }
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
    
// OK
