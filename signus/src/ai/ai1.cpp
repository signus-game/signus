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
//           Mission 1
//
// Programmed by Richard Wunsch
//

#include "headers.h"
#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"

// Action Radius of Army 1
#define A1X1 38
#define A1Y1  5
#define A1X2 59
#define A1Y2 30

// Action Radius of Army 2
#define A2X1  0
#define A2Y1  0
#define A2X2 33
#define A2Y2 12

// Action Radius of Army 3
#define A3X1 15
#define A3Y1 27
#define A3X2 29
#define A3Y2 45


// Radar 1
#define R1X 49
#define R1Y 24


// Base 1
#define B1X 31
#define B1Y 20

// Tower 1
#define T1X 48
#define T1Y 21

// Radar 2
#define R2X 5
#define R2Y 4

// Warehouse 2
#define W2X 4
#define W2Y 1

// Radar 3
#define R3X 25
#define R3Y 47

// Tower 3
#define T3X 25
#define T3Y 37

// Radar 4
#define R4X 11
#define R4Y 53

// Tower 4
#define T4X 10
#define T4Y 51

// Warehouse 4
#define W4X  8
#define W4Y 53

// To Base 1
#define TB1X1 20
#define TB1Y1  0
#define TB1X2 30
#define TB1Y2  1



#define CMinDang1 3    // Dangerous limit for Army 1
#define CMinDang2 0    // Dangerous limit for Army 2
#define CMinDang3 0    // Dangerous limit for Army 3

#define CSuicide2 4    // Suicide limit for Army 2
#define CSuicide3 4    // Suicide limit for Army 3



#define CEscPoint1X 17    // Escape point for Army 1 
#define CEscPoint1Y  3

#define START_TRANS_E 4 // Turn starting transport from Base 1 - EASY
#define START_TRANS_N 3 // Turn starting transport from Base 1 - NORMAL
#define START_TRANS_H 2 // Turn starting transport from Base 1 - HARD


int START_TRANS;

void InitAI1 ()
{
    int i,j;
    TBase *Base;
    TField *f;
    TUnit *u;

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 0;
    MoneyBadlife = 750; 
    MBPlaces = NULL;
    DeniedPlaces = NULL;

    Base1 = GetField (B1X, B1Y) -> Unit;
    Tower1 = GetField (T1X, T1Y) -> Unit;
    Tower3 = GetField (T3X, T3Y) -> Unit;
    Tower4 = GetField (T4X, T4Y) -> Unit;
    Warehouse2 = GetField (W2X, W2Y) -> Unit;
    Warehouse2 = GetField (W4X, W4Y) -> Unit;

    // Nastaveni parametru obtiznosti
    switch (ActualDifficulty) {
        case 'e' : {
            START_TRANS = START_TRANS_E;
            break;
        }
        case 'n' : {
            START_TRANS = START_TRANS_N;
            break;
        }
        case 'h' : {
            START_TRANS = START_TRANS_H;
            break;
        }
    }
        
        

    
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

    // Jednotky do zakladny
    LockDraw ();    
    Base = (TBase*)Units[GetField(B1X, B1Y) -> Unit];
    for (i = TB1X1; i <= TB1X2; i++) {
        for (j = TB1Y1; j <= TB1Y2; j++) {
            f = GetField (i, j);
            if ((f -> Unit != NO_UNIT) && (f -> Unit != Base ->ID) && (f -> Unit >= BADLIFE)) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                Base -> Inventory [Base -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
                
            }
        }
    }
    UnlockDraw ();          
}


void LoadArtificialIntelligence1 (FILE *f)
{

    DoneArtificialIntelligence1 ();

    MBPlaces = NULL;
    DeniedPlaces = NULL;
    
    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);  

    DUPos = 0;
    
    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
}



void SaveArtificialIntelligence1 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);

    Towers -> Save (f);
    Army1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);      
    DUPos = 0;
    
}



int ArtificialIntelligence1 ()
{
    int i, j, r, st, x, y;
    TField *f;
    char buf [40];
    TBase *Base;

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
    Tower1 = GetField (T1X, T1Y) -> Unit;
    if (Tower1 == NO_UNIT || Units [Tower1] -> Type % BADLIFE != unPagoda) 
        Tower1 = NO_UNIT;
    Tower3 = GetField (T3X, T3Y) -> Unit;
    if (Tower3 == NO_UNIT || Units [Tower3] -> Type % BADLIFE != unPagoda) 
        Tower3 = NO_UNIT;
    Tower4 = GetField (T4X, T4Y) -> Unit;
    if (Tower4 == NO_UNIT || Units [Tower4] -> Type % BADLIFE != unPagoda) 
        Tower4 = NO_UNIT;
    Warehouse2 = GetField (W2X, W2Y) -> Unit;
    if (Warehouse2 == NO_UNIT || Units [Warehouse2] -> Type % BADLIFE != unWarehouse) 
        Warehouse2 = NO_UNIT;
    Warehouse4 = GetField (W4X, W4Y) -> Unit;
    if (Warehouse4 == NO_UNIT || Units [Warehouse4] -> Type % BADLIFE != unWarehouse) 
        Warehouse4 = NO_UNIT;
        
    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
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

    // Nastavime ProgressBar
    st = 0;
    st += Towers -> nofBadLife;
    if (Army1 -> Status == asActive || Army1 -> Status == asSuicide) st += Army1 -> nofBadLife;
    if (Army2 -> Status == asActive || Army2 -> Status == asSuicide) st += Army2 -> nofBadLife;
    if (Army3 -> Status == asActive || Army3 -> Status == asSuicide) st += Army3 -> nofBadLife;
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
    if (Army1 -> Status == asActive) {
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
    if (Army2 -> Status == asActive) {
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
        Army2 -> CountDangerous ();
        if (Army2 -> Dangerous <= CMinDang2) {
            Army2 -> Status = asDestroyed;
        }
        
    }

    // ARMY 3
    if (Army3 -> Status == asActive) {
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
        sprintf (buf, SigText[TXT_AI_ARMY], 3);
        if (Army3 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 3
        Army3 -> CountDangerous ();
        if (Army3 -> Dangerous <= CMinDang3) {
            Army3 -> Status = asDestroyed;
        }
                
    }

                    
    
    
    
    // Presun jednotek z Base 1
    Base = (TBase*)Units[GetField(B1X, B1Y) -> Unit];   
    if (Base -> Type == BADLIFE + unBase) {
        // 1. Perseus Alfa
        if (ActualTurn == START_TRANS) {
            for (i = 0; i < Base -> LoadedUnits; i++) {
                if (Units [Base -> Inventory [i]] -> Type % BADLIFE == unPerseusAlfa) {
                    r = Base -> Inventory [i]; // uchovame ID
                    st = Base -> UnloadUnit ((TUnit*)Units [r]);
                    if (st == TRUE) {
                        Units [r] -> AI_Info.State = aistMoving;
                        Units [r] -> AI_Info.point.x = CEscPoint1X - 1;
                        Units [r] -> AI_Info.point.y = CEscPoint1Y - 1;
                    }
                    break;          
                }
            }
        }
        // 2. Perseus Alfa
        if (ActualTurn == START_TRANS + 1) {
            for (i = 0; i < Base -> LoadedUnits; i++) {
                if (Units [Base -> Inventory [i]] -> Type % BADLIFE == unPerseusAlfa) {
                    r = Base -> Inventory [i]; // uchovame ID
                    st = Base -> UnloadUnit ((TUnit*)Units [r]);
                    if (st == TRUE) {
                        Units [r] -> AI_Info.State = aistMoving;
                        Units [r] -> AI_Info.point.x = CEscPoint1X;
                        Units [r] -> AI_Info.point.y = CEscPoint1Y - 1;
                    }
                    break;          
                }
            }
        }
        // 3. Perseus Beta
        if (ActualTurn == START_TRANS + 2) {
            for (i = 0; i < Base -> LoadedUnits; i++) {
                if (Units [Base -> Inventory [i]] -> Type % BADLIFE == unPerseusBeta) {
                    r = Base -> Inventory [i]; // uchovame ID
                    st = Base -> UnloadUnit ((TUnit*)Units [r]);
                    if (st == TRUE) {
                        Units [r] -> AI_Info.State = aistMoving;
                        Units [r] -> AI_Info.point.x = CEscPoint1X + 1;
                        Units [r] -> AI_Info.point.y = CEscPoint1Y - 1;
                    }
                    break;          
                }
            }
        }
        // 4. Perseus Beta
        if (ActualTurn == START_TRANS + 3) {
            for (i = 0; i < Base -> LoadedUnits; i++) {
                if (Units [Base -> Inventory [i]] -> Type % BADLIFE == unPerseusBeta) {
                    r = Base -> Inventory [i]; // uchovame ID
                    st = Base -> UnloadUnit ((TUnit*)Units [r]);
                    if (st == TRUE) {
                        Units [r] -> AI_Info.State = aistMoving;
                        Units [r] -> AI_Info.point.x = CEscPoint1X - 1;
                        Units [r] -> AI_Info.point.y = CEscPoint1Y;
                    }
                    break;          
                }
            }
        }
        // 5. Perseus Beta
        if (ActualTurn == START_TRANS + 4) {
            for (i = 0; i < Base -> LoadedUnits; i++) {
                if (Units [Base -> Inventory [i]] -> Type % BADLIFE == unPerseusBeta) {
                    r = Base -> Inventory [i]; // uchovame ID
                    st = Base -> UnloadUnit ((TUnit*)Units [r]);
                    if (st == TRUE) {
                        Units [r] -> AI_Info.State = aistMoving;
                        Units [r] -> AI_Info.point.x = CEscPoint1X;
                        Units [r] -> AI_Info.point.y = CEscPoint1Y;
                    }
                    break;          
                }
            }
        }
        // 6. Ceres
        if (ActualTurn == START_TRANS + 5) {
            for (i = 0; i < Base -> LoadedUnits; i++) {
                if (Units [Base -> Inventory [i]] -> Type % BADLIFE == unCeres) {
                    r = Base -> Inventory [i]; // uchovame ID
                    st = Base -> UnloadUnit ((TUnit*)Units [r]);
                    if (st == TRUE) {
                        Units [r] -> AI_Info.State = aistMoving;
                        Units [r] -> AI_Info.point.x = CEscPoint1X + 1;
                        Units [r] -> AI_Info.point.y = CEscPoint1Y;
                    }
                    break;          
                }
            }
        }
        // 7. Gnom
        if (ActualTurn == START_TRANS + 6) {
            for (i = 0; i < Base -> LoadedUnits; i++) {
                if (Units [Base -> Inventory [i]] -> Type % BADLIFE == unGnom) {
                    r = Base -> Inventory [i]; // uchovame ID
                    st = Base -> UnloadUnit ((TUnit*)Units [r]);
                    if (st == TRUE) {
                        Units [r] -> AI_Info.State = aistMoving;
                        Units [r] -> AI_Info.point.x = CEscPoint1X - 1;
                        Units [r] -> AI_Info.point.y = CEscPoint1Y + 1;
                    }
                    break;          
                }
            }
        }
    }

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

    
    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}

int *GetManufacturableUnits1 (int FactoryID)
{
        return GFactoryList;
}


int AssignResult1 ()
{
    int i, t = 0;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) t++;
    if (t == 0) return -1; // GoodLife prohral - prisel o jednotky
    
    t = 0;
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL) 
            if (Units [i] -> Type == unBumerang) t++;
    if (t == 0 && (Units [Base1] == NULL || Base1 > BADLIFE)) {
        return -1;
        // GoodLife prohral - nema Bumerangy a nezajal base1
    }
    
        
    t = 0;
    for (i = BADLIFE; i < UNITS_TOP; i++)
        if (Units [i] != NULL && Units [i] -> Type - BADLIFE < unRadar) t++;
    if (t == 0) return 1; // BadLife prohral - prisel o vsechno
    
    if (Army1 -> Status == asDestroyed 
    && Army2 -> Status == asDestroyed 
    && Army3 -> Status == asDestroyed) return 1;
    
    
    return 0; // Zatim se jeste hraje
}



void DoneArtificialIntelligence1 ()
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
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

    
    
    
// OK
