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
//           Mission 6
//
// Programmed by Richard Wunsch
//

#include "headers.h"
#include "mouse.h"
#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "aiair.h"

// Action Radius of Army 1
#define A1X1 33
#define A1Y1 70
#define A1X2 45
#define A1Y2 82

// Action Radius of Army 2
#define A2X1 62
#define A2Y1 69
#define A2X2 74
#define A2Y2 81

// Action Radius of Army 3
#define A3X1 69
#define A3Y1 41
#define A3X2 82
#define A3Y2 51

// Action Radius of Army 4
#define A4X1 46
#define A4Y1 26
#define A4X2 59
#define A4Y2 36

// Action Radius of Army 5
#define A5X1 24
#define A5Y1 38
#define A5X2 35
#define A5Y2 50

// Action Radius of Army 6
#define A6X1 39
#define A6Y1 40
#define A6X2 65
#define A6Y2 66

// Saturn 1
#define S1X  4
#define S1Y 49

// Saturn 2
#define S2X  4
#define S2Y 57

// Caesar 1
#define C1X 37
#define C1Y 76

// Caesar 2
#define C2X 69
#define C2Y 76

// Caesar 3
#define C3X 77
#define C3Y 44

// Caesar 4
#define C4X 52
#define C4Y 29

// Caesar 5
#define C5X 28
#define C5Y 43

// WayPoint of Armies
#define WPX 61
#define WPY 52



#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6

#define CSuicide1  3    // Suicide limit for Army 1
#define CSuicide2  3    // Suicide limit for Army 2
#define CSuicide3  3    // Suicide limit for Army 3
#define CSuicide4  3    // Suicide limit for Army 4
#define CSuicide5  3    // Suicide limit for Army 5
#define CSuicide6  3    // Suicide limit for Army 6

#define SaturnStart   MaxInt    // Saturns starting...

/// animace invaze:


#include "loadsave.h"
#include "anims.h"

void InvasionAnimation()
{
    FILE *f = fopen("mission6.tmp", "wb");

    if (f == NULL) return;
    SaveGameState(f);
    fclose(f);
    MouseShow();
    ShowHelpers(); 
    DoneArtificialIntelligence();
    DoneEngine();
    
#if 0  // FIXME
    PlayAnimation("mis6x");          
#endif
    
    f = fopen("mission6.tmp", "rb");
    if (f == NULL) return;
    LoadGameState(f);
    fclose(f);
    remove("mission6.tmp");

    MouseHide();
    HideHelpers(); 
    SaySpeech ("mis6a", 2005);
}



TPoint MinePlaces6 [] = {65,52, 60,53, 55,48, 53,51, 48,53,
                                                -1,-1};

int SaturnState = 0;
int StartInvasion = MaxInt;
TPoint S1Start, S2Start;

void InitAI6 ()
{
    int i,j;
    TBase *Base;
    TAirport *Airport;
    TField *f;
    TUnit *u;
    TPoint p;

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 0;
    MoneyBadlife = 100;
    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    for (i = BADLIFE; i < UNITS_TOP; i++) {
        if (Units [i] != NULL) {
            if (Units [i] -> X == S1X && Units [i] -> Y == S1Y) {
                Saturn1 = i;
            }
            if (Units [i] -> X == S2X && Units [i] -> Y == S2Y) {
                Saturn2 = i;        
            }
            if (Units [i] -> X == C1X && Units [i] -> Y == C1Y) {
                Caesar1 = i;
            }
            if (Units [i] -> X == C2X && Units [i] -> Y == C2Y) {
                Caesar2 = i;
            }
            if (Units [i] -> X == C3X && Units [i] -> Y == C3Y) {
                Caesar3 = i;
            }
            if (Units [i] -> X == C4X && Units [i] -> Y == C4Y) {
                Caesar4 = i;
            }
            if (Units [i] -> X == C5X && Units [i] -> Y == C5Y) {
                Caesar5 = i;
            }   
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
    

    LockDraw ();        
    Army1 = new TGroundArmy();
    Army1 -> SetAR (A1X1, A1Y1, A1X2, A1Y2);
    Army1 -> SetRadar (-1, -1);
    Army1 -> InsertUnits ();
    Army1 -> Displace ();

    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> SetRadar (-1, -1);
    Army2 -> InsertUnits ();
    Army2 -> Displace ();
    
    Army3 = new TGroundArmy();
    Army3 -> SetAR (A3X1, A3Y1, A3X2, A3Y2);
    Army3 -> SetRadar (-1, -1);
    Army3 -> InsertUnits ();
    Army3 -> Displace ();

    Army4 = new TGroundArmy();
    Army4 -> SetAR (A4X1, A4Y1, A4X2, A4Y2);
    Army4 -> SetRadar (-1, -1);
    Army4 -> InsertUnits ();
    Army4 -> Displace ();

    Army5 = new TGroundArmy();
    Army5 -> SetAR (A5X1, A5Y1, A5X2, A5Y2);
    Army5 -> SetRadar (-1, -1);
    Army5 -> InsertUnits ();
    Army5 -> Displace ();

    Army6 = new TGroundArmy();
    Army6 -> SetAR (A6X1, A6Y1, A6X2, A6Y2);
    Army6 -> SetRadar (-1, -1);
    Army6 -> InsertUnits ();
    Army6 -> WayPoint.x = WPX; Army6 -> WayPoint.y = WPY; 
    Army6 -> Status = asSuicide;
    
    ((TCaesar *)Units [Caesar1]) -> Land ();
    Units [Caesar1] -> AI_Info.point.x = Units [Caesar1] -> X;
    Units [Caesar1] -> AI_Info.point.y = Units [Caesar1] -> Y;
    Units [Caesar1] -> PlaceGround (FALSE);
    Units [Caesar1] -> X = -1;
    Units [Caesar1] -> Y = -1;
    
    ((TCaesar *)Units [Caesar2]) -> Land ();
    Units [Caesar2] -> AI_Info.point.x = Units [Caesar2] -> X;
    Units [Caesar2] -> AI_Info.point.y = Units [Caesar2] -> Y;
    Units [Caesar2] -> PlaceGround (FALSE);
    Units [Caesar2] -> X = -1;
    Units [Caesar2] -> Y = -1;
    
    ((TCaesar *)Units [Caesar3]) -> Land ();
    Units [Caesar3] -> AI_Info.point.x = Units [Caesar3] -> X;
    Units [Caesar3] -> AI_Info.point.y = Units [Caesar3] -> Y;
    Units [Caesar3] -> PlaceGround (FALSE);
    Units [Caesar3] -> X = -1;
    Units [Caesar3] -> Y = -1;
    
    ((TCaesar *)Units [Caesar4]) -> Land ();
    Units [Caesar4] -> AI_Info.point.x = Units [Caesar4] -> X;
    Units [Caesar4] -> AI_Info.point.y = Units [Caesar4] -> Y;
    Units [Caesar4] -> PlaceGround (FALSE);
    Units [Caesar4] -> X = -1;
    Units [Caesar4] -> Y = -1;
    
    ((TCaesar *)Units [Caesar5]) -> Land ();
    Units [Caesar5] -> AI_Info.point.x = Units [Caesar5] -> X;
    Units [Caesar5] -> AI_Info.point.y = Units [Caesar5] -> Y;
    Units [Caesar5] -> PlaceGround (FALSE);
    Units [Caesar5] -> X = -1;
    Units [Caesar5] -> Y = -1;
    


    
    // Polozime miny
    
    for (i = 0; MinePlaces6 [i].x != -1; i++) {
        MinePlace (MinePlaces6 [i].x, MinePlaces6 [i].y, BADLIFE);
    }
    UnlockDraw ();          

}




void LoadArtificialIntelligence6 (FILE *f)
{

    DoneArtificialIntelligence6 ();

    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    fread (&AttackFieldPos, sizeof (int), 1, f);
    fread (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);
    DUPos = 0;

    fread (&Saturn1, sizeof (int), 1, f);
    fread (&Saturn2, sizeof (int), 1, f);
    fread (&Caesar1, sizeof (int), 1, f);
    fread (&Caesar2, sizeof (int), 1, f);
    fread (&Caesar3, sizeof (int), 1, f);
    fread (&Caesar4, sizeof (int), 1, f);
    fread (&Caesar5, sizeof (int), 1, f);
    
    fread (&SaturnState, sizeof (int), 1, f);
    fread (&S1Start, sizeof (TPoint), 1, f);
    fread (&S2Start, sizeof (TPoint), 1, f);
    fread (&StartInvasion, sizeof (int), 1, f);
    
    Towers = new TTowers (f);
    Army1 = new TGroundArmy (f);
    Army2 = new TGroundArmy (f);
    Army3 = new TGroundArmy (f);
    Army4 = new TGroundArmy (f);
    Army5 = new TGroundArmy (f);
    Army6 = new TGroundArmy (f);
}



void SaveArtificialIntelligence6 (FILE *f)
{
    fwrite (&AttackFieldPos, sizeof (int), 1, f);
    fwrite (AttackedField, (AttackFieldPos + 1)*sizeof (TAttackedField), 1, f);

    fwrite (&Saturn1, sizeof (int), 1, f);
    fwrite (&Saturn2, sizeof (int), 1, f);
    fwrite (&Caesar1, sizeof (int), 1, f);
    fwrite (&Caesar2, sizeof (int), 1, f);
    fwrite (&Caesar3, sizeof (int), 1, f);
    fwrite (&Caesar4, sizeof (int), 1, f);
    fwrite (&Caesar5, sizeof (int), 1, f);

    fwrite (&SaturnState, sizeof (int), 1, f);
    fwrite (&S1Start, sizeof (TPoint), 1, f);
    fwrite (&S2Start, sizeof (TPoint), 1, f);
    fwrite (&StartInvasion, sizeof (int), 1, f);
    
    Towers -> Save (f);
    Army1 -> Save (f);
    Army2 -> Save (f);
    Army3 -> Save (f);      
    Army4 -> Save (f);
    Army5 -> Save (f);
    Army6 -> Save (f);
    DUPos = 0;
}


int ArtificialIntelligence6 ()
{
    int i, j, r, st, x, y, id, h, hmax;
    char buf [40];
    TBase *Base;
    TField *f;
    TPoint p;

    // Vylodeni
    if (StartInvasion == ActualTurn) {
        Army1 -> Place ();      
        Army1 -> Status = asActive;
        Army1 -> WayPoint.x = WPX; Army1 -> WayPoint.y = WPY; 
        Army1 -> SetAIConst (2);
        Army2 -> Place ();
        Army2 -> Status = asActive;
        Army2 -> WayPoint.x = WPX; Army2 -> WayPoint.y = WPY; 
        Army2 -> SetAIConst (2);
        Army3 -> Place ();
        Army3 -> Status = asActive;
        Army3 -> WayPoint.x = WPX; Army3 -> WayPoint.y = WPY; 
        Army3 -> SetAIConst (2);
        Army4 -> Place ();
        Army4 -> Status = asActive;
        Army4 -> WayPoint.x = WPX; Army4 -> WayPoint.y = WPY; 
        Army4 -> SetAIConst (2);
        Army5 -> Place ();
        Army5 -> Status = asActive;
        Army5 -> WayPoint.x = WPX; Army5 -> WayPoint.y = WPY; 
        Army5 -> SetAIConst (2);

        for (j = 0; j < 120; j++) {
            p = CartezianSnail (j);
            p.x += Units [Caesar1] -> AI_Info.point.x;
            p.y += Units [Caesar1] -> AI_Info.point.y;
            if (p.x < 0 || p.x >= MapSizeX || p.y < 0 || p.y >= MapSizeY) continue;
            f = GetField (p.x, p.y);
            if (f -> Unit == NO_UNIT && GetAircraftAt (p.x, p.y) == NULL) {
                Units [Caesar1] -> X = p.x;
                Units [Caesar1] -> Y = p.y;
                Units [Caesar1] -> PlaceGround (TRUE);
                Units [Caesar1] -> AI_Info.point.x = 0;
                Units [Caesar1] -> AI_Info.point.y = 0;
                break;
            }
        }
        for (j = 0; j < 120; j++) {
            p = CartezianSnail (j);
            p.x += Units [Caesar2] -> AI_Info.point.x;
            p.y += Units [Caesar2] -> AI_Info.point.y;
            if (p.x < 0 || p.x >= MapSizeX || p.y < 0 || p.y >= MapSizeY) continue;
            f = GetField (p.x, p.y);
            if (f -> Unit == NO_UNIT && GetAircraftAt (p.x, p.y) == NULL) {
                Units [Caesar2] -> X = p.x;
                Units [Caesar2] -> Y = p.y;
                Units [Caesar2] -> PlaceGround (TRUE);
                Units [Caesar2] -> AI_Info.point.x = 0;
                Units [Caesar2] -> AI_Info.point.y = 0;
                break;
            }
        }
        for (j = 0; j < 120; j++) {
            p = CartezianSnail (j);
            p.x += Units [Caesar3] -> AI_Info.point.x;
            p.y += Units [Caesar3] -> AI_Info.point.y;
            if (p.x < 0 || p.x >= MapSizeX || p.y < 0 || p.y >= MapSizeY) continue;
            f = GetField (p.x, p.y);
            if (f -> Unit == NO_UNIT && GetAircraftAt (p.x, p.y) == NULL) {
                Units [Caesar3] -> X = p.x;
                Units [Caesar3] -> Y = p.y;
                Units [Caesar3] -> PlaceGround (TRUE);
                Units [Caesar3] -> AI_Info.point.x = 0;
                Units [Caesar3] -> AI_Info.point.y = 0;
                break;
            }
        }
        for (j = 0; j < 120; j++) {
            p = CartezianSnail (j);
            p.x += Units [Caesar4] -> AI_Info.point.x;
            p.y += Units [Caesar4] -> AI_Info.point.y;
            if (p.x < 0 || p.x >= MapSizeX || p.y < 0 || p.y >= MapSizeY) continue;
            f = GetField (p.x, p.y);
            if (f -> Unit == NO_UNIT && GetAircraftAt (p.x, p.y) == NULL) {
                Units [Caesar4] -> X = p.x;
                Units [Caesar4] -> Y = p.y;
                Units [Caesar4] -> PlaceGround (TRUE);
                Units [Caesar4] -> AI_Info.point.x = 0;
                Units [Caesar4] -> AI_Info.point.y = 0;
                break;
            }
        }
        for (j = 0; j < 120; j++) {
            p = CartezianSnail (j);
            p.x += Units [Caesar5] -> AI_Info.point.x;
            p.y += Units [Caesar5] -> AI_Info.point.y;
            if (p.x < 0 || p.x >= MapSizeX || p.y < 0 || p.y >= MapSizeY) continue;
            f = GetField (p.x, p.y);
            if (f -> Unit == NO_UNIT && GetAircraftAt (p.x, p.y) == NULL) {
                Units [Caesar5] -> X = p.x;
                Units [Caesar5] -> Y = p.y;
                Units [Caesar5] -> PlaceGround (TRUE);
                Units [Caesar5] -> AI_Info.point.x = 0;
                Units [Caesar5] -> AI_Info.point.y = 0;
                break;
            }
        }
        InvasionAnimation ();
    }

    Message (SigText[TXT_AI_ANALYSE]);

    // Prepocitame viditelnost
    AllowBadlife (TRUE);
    ComputeVisib ();
    RedrawMap ();
    
    LockDraw ();
    
    // Aktualizujeme specialni objekty

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



//////////////////////////////// AKTIVACE ARMAD
    
    // S A T U R N I
    // CHEAT !
    if (SaturnState == 0) {
        if (Units [Saturn1] != NULL) 
            ((TSaturn *)Units [Saturn1]) -> Fuel = ((TSaturn *)Units [Saturn1]) -> MaxFuel;
        if (Units [Saturn2] != NULL) 
            ((TSaturn *)Units [Saturn2]) -> Fuel = ((TSaturn *)Units [Saturn2]) -> MaxFuel;     
    }
    
    // Aktivace
    if (SaturnState == 0 
    && (Army6 -> Status == asDestroyed || ActualTurn >= SaturnStart)) {
        SaturnState = 1;
    }

    // Vybereme nejlepsi misto na bombeni
    if (SaturnState == 1) {
        hmax = 0;
        for (i = 0; i < BADLIFE; i++) {
            h = 0;
            if (Units [i] != NULL) {
                for (x = Units [i] -> X - 5; x <= Units [i] -> X + 5; x++) {
                    if (x < 0 || x > MapSizeX) continue;
                    for (y = Units [i] -> Y - 4; y <= Units [i] -> Y + 5; y++) {
                        if (y < 0 || y > MapSizeY) continue;
                        f = GetField (x, y);
                        if (f -> Unit != NO_UNIT && f -> Unit < BADLIFE) {
                            h += TabDanger [Units [f -> Unit] -> ID];
                        }
                    }
                }
            }
            if (h > hmax) {
                hmax = h;
                S1Start.x = Units [i] -> X - 5;
                S1Start.y = Units [i] -> Y - 2;
                S2Start.x = Units [i] -> X - 5;
                S2Start.y = Units [i] -> Y + 3;
            }
        }
    }
    // Bombi se
    i = j = FALSE; 
    if (SaturnState == 2) {
        if (Units [Saturn1] != NULL) {
            UnlockDraw ();
            Units [Saturn1] -> Select ();
            if (Units [Saturn1] -> MoveFar (S1Start.x, S1Start.y) > 0) {
                if (((TSaturn *)Units [Saturn1]) -> EndBombing () > 0) i = TRUE;
            }
            LockDraw ();
        }
        if (Units [Saturn2] != NULL) {
            UnlockDraw ();
            Units [Saturn2] -> Select ();
            if (Units [Saturn2] -> MoveFar (S2Start.x, S2Start.y) > 0) {
                if (((TSaturn *)Units [Saturn2]) -> EndBombing () > 0) j = TRUE;
            }
            LockDraw ();
        }
        if (i && j) {
            SaturnState = 3;    
            StartInvasion = ActualTurn + 1;
        }
    }
    // De se bombit
    i = j = FALSE; 
    if (SaturnState == 1) {
        if (Units [Saturn1] != NULL) {
            UnlockDraw ();
            Units [Saturn1] -> Select ();
            if (Units [Saturn1] -> MoveFar (S1Start.x, S1Start.y) > 0) {
                if (((TSaturn *)Units [Saturn1]) -> StartBombing () > 0) i = TRUE;
            }
            LockDraw ();
        }
        if (Units [Saturn2] != NULL) {
            UnlockDraw ();
            Units [Saturn2] -> Select ();
            if (Units [Saturn2] -> MoveFar (S2Start.x, S2Start.y) > 0) {
                if (((TSaturn *)Units [Saturn2]) -> StartBombing () > 0) j = TRUE;
            }
            LockDraw ();
        }
        if (i && j) {
            SaturnState = 2;    
            S1Start.x += 8;
            S2Start.x += 8;
        }
    }
    // Leti se domuuu
    i = j = FALSE;
    if (SaturnState == 3) { 
        if (Units [Saturn1] != NULL) {
            UnlockDraw ();
            Units [Saturn1] -> Select ();
            if (Units [Saturn1] -> MoveFar (MapSizeX - 1, S1Start.y) > 0) {
                ((TSaturn *)Units [Saturn1]) -> PlaceGround (FALSE);
                Units [Saturn1] -> X = Units [Saturn1] -> Y = -1;
                i = TRUE;
            }
            LockDraw ();
        }
        if (Units [Saturn2] != NULL) {
            UnlockDraw ();
            Units [Saturn2] -> Select ();
            if (Units [Saturn2] -> MoveFar (MapSizeX - 1, S2Start.y) > 0) {
                ((TSaturn *)Units [Saturn2]) -> PlaceGround (FALSE);
                Units [Saturn2] -> X = Units [Saturn2] -> Y = -1;
                j = TRUE;
            }
            LockDraw ();
        }           
        if (i && j) {
            SaturnState = 4; // Konec akce  
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

    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult6 ()
{
    int i, t = 0;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL && Units [i] -> Type == unGolias) t++;
    if (t == 0) return -1; // GoodLife prohral  

    if (Army1 -> Status == asDestroyed && Army2 -> Status == asDestroyed 
    && Army3 -> Status == asDestroyed && Army4 -> Status == asDestroyed 
    && Army5 -> Status == asDestroyed && Army6 -> Status == asDestroyed) 
        return 1; // BadLife prohral
    
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits6 (int FactoryID)
{
    return GFactoryList;
}



void DoneArtificialIntelligence6 ()
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
    
