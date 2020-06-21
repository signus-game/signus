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
//           Mission 5
//
// Programmed by Richard Wunsch
//

#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "aiair.h"

// Action Radius of Army 1
#define A1X1 38
#define A1Y1 20
#define A1X2 70
#define A1Y2 40

// Army1 WayPoint
#define A1WPX 52
#define A1WPY 23

// Army2 WayPoint1
#define A2WP1X 33
#define A2WP1Y 70

// Army2 WayPoint2
#define A2WP2X 33
#define A2WP2Y 92

// Ikarosy - AirArmy1
#define AA1X1 47
#define AA1Y1 40
#define AA1X2 49
#define AA1Y2 42


// Action Radius of Army 2
#define A2X1 54
#define A2Y1 40
#define A2X2 72
#define A2Y2 51


// Action Radius of Army 3
#define A3X1 27
#define A3Y1 77
#define A3X2 40
#define A3Y2 90

// Action Radius of Army 4
#define A4X1 11
#define A4Y1 99
#define A4X2 28
#define A4Y2 118

// Action Radius of Army 5
#define A5X1 67
#define A5Y1 81
#define A5X2 86
#define A5Y2 104

// Action Radius of Army 6
#define A6X1 50
#define A6Y1 102
#define A6X2 80
#define A6Y2 130

// Action Radius of Army 7
#define A7X1 43
#define A7Y1 117
#define A7X2 50
#define A7Y2 124

// Action Radius of Army 8
#define A8X1 42
#define A8Y1 128
#define A8X2 68
#define A8Y2 155

// Action Radius of Air Army 8
#define AA8X1 50
#define AA8Y1 142
#define AA8X2 54
#define AA8Y2 145


// Radar of Army1
#define R1X 51
#define R1Y 30

// Radar of Army2
#define R2X 68
#define R2Y 48

// Radar of Army4
#define R4X 21
#define R4Y 110

// Radar of Army5
#define R5X 80
#define R5Y 91

// Radar of Army8
#define R8X 60
#define R8Y 134

// Base 1
#define B1X 62
#define B1Y 46

// To Base 1
#define TB1X1 67
#define TB1Y1 31
#define TB1X2 73
#define TB1Y2 34

// Base 8
#define B8X 62
#define B8Y 144

// Airport 1
#define A1X 48
#define A1Y 45

// Heliport 4
#define H4X 14
#define H4Y 106

// To Heliport 4
#define TH4X1 11
#define TH4Y1 96
#define TH4X2 13
#define TH4Y2 98

// Factory 8
#define F8X 56
#define F8Y 148

// Airport 8
#define A8X 48
#define A8Y 147

// Xenon 3
#define X3X 36
#define X3Y 82

// Kobra 1 
#define K1X 51
#define K1Y 45





#define CMinDang1  0    // Dangerous limit for Army 1
#define CMinDangA1 0    // Dangerous limit for AirArmy 1
#define CMinDang2  0    // Dangerous limit for Army 2
#define CMinDang3  0    // Dangerous limit for Army 3
#define CMinDang4  0    // Dangerous limit for Army 4
#define CMinDang5  0    // Dangerous limit for Army 5
#define CMinDang6  0    // Dangerous limit for Army 6
#define CMinDang7  0    // Dangerous limit for Army 7
#define CMinDang8  0    // Dangerous limit for Army 8

#define CSuicide1  3    // Suicide limit for Army 1
#define CSuicideA1 3    // Suicide limit for Army 1
#define CSuicide2  3    // Suicide limit for Army 2
#define CSuicide3  3    // Suicide limit for Army 3
#define CSuicide4  3    // Suicide limit for Army 4
#define CSuicide5  3    // Suicide limit for Army 5
#define CSuicide6  3    // Suicide limit for Army 6
#define CSuicide7  3    // Suicide limit for Army 7
#define CSuicide8  3    // Suicide limit for Army 8

#define A2Escape   5    // Army 2 Escape Dangerous Limit

int Factory5List [] = {unBumerang, unPerseusAlfa, unPerseusBeta, 
                                             unRex, unHerkules, unSkorpion, unPatriot, 0};

TPoint MinePlaces5 [] = {51,32, 54,39, 55,43, 49,51, 76,80, 76,82, 76,84, 
                                                76,86, 74,106, 68,112, 67,114, 31,92, 31,95, 16,103,
                                                20,104, 16,111,
                                                -1,-1};

TPoint Xenon3Mines [] = {35,77, 35,79, 35,81,
                                                 -1,-1};

int Xenon3ActivationTurn, X3MineCnt;

void InitAI5 ()
{
    int i,j;
    TBase *Base;
    THeliport *Heliport;
    TAirport *Airport;
    TField *f;
    TUnit *u;
    
    LockDraw ();
    
    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 0;
    MoneyBadlife = 5000;
    MBPlaces = NULL;    
    DeniedPlaces = NULL;

    ActivationTurn1 = MaxInt;
    ActivationTurn2 = MaxInt;
    ActivationTurn3 = MaxInt;
    ActivationTurn4 = MaxInt;
    ActivationTurn5 = MaxInt;
    ActivationTurn6 = MaxInt;
    ActivationTurn7 = MaxInt;
    ActivationTurn8 = MaxInt;
    Xenon3ActivationTurn = MaxInt;
    X3MineCnt = 0;

    Base1 = GetField (B1X, B1Y) -> Unit;
    Base8 = GetField (B8X, B8Y) -> Unit;
    Radar1 = GetField (R1X, R1Y) -> Unit;
    Radar2 = GetField (R2X, R2Y) -> Unit;
    Radar4 = GetField (R4X, R4Y) -> Unit;
    Radar5 = GetField (R5X, R5Y) -> Unit;
    Radar8 = GetField (R8X, R8Y) -> Unit;
    Factory8 = GetField (F8X, F8Y) -> Unit;
    Airport1 = GetField (A1X, A1Y) -> Unit;
    Airport8 = GetField (A8X, A8Y) -> Unit;
    Heliport4 = GetField (H4X, H4Y) -> Unit;
    
    Xenon3 = GetField (X3X, X3Y) -> Unit;
    
    for (i = BADLIFE; i < UNITS_TOP; i++) {
        if (Units [i] != NULL && Units [i] -> X == K1X && Units [i] -> Y == K1Y) {
            Kobra1 = i;
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
    Army1 -> WayPoint.x = A1WPX; Army1 -> WayPoint.y = A1WPY; 
    Army1 -> InsertUnits ();

    AirArmy1 = new TAirArmy ();
    AirArmy1 -> SetAR (AA1X1, AA1Y1, AA1X2, AA1Y2); 
    AirArmy1 -> WayPoint.x = A1WPX; AirArmy1 -> WayPoint.y = A1WPY;     
    AirArmy1 -> SetAIConst (2); // AGRESIVE ARMY
    AirArmy1 -> Status = asActive;
    AirArmy1 -> InsertUnits ();
        

    Army2 = new TGroundArmy();
    Army2 -> SetAR (A2X1, A2Y1, A2X2, A2Y2);
    Army2 -> SetRadar (R2X, R2Y);
    Army2 -> InsertUnits ();
    
    Army3 = new TGroundArmy();
    Army3 -> SetAR (A3X1, A3Y1, A3X2, A3Y2);
    Army3 -> SetRadar (-1, -1);
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
    Army6 -> SetRadar (-1, -1);
    Army6 -> InsertUnits ();

    Army7 = new TGroundArmy();
    Army7 -> SetAR (A7X1, A7Y1, A7X2, A7Y2);
    Army7 -> SetRadar (-1, -1);
    Army7 -> InsertUnits ();

    Army8 = new TGroundArmy();
    Army8 -> SetAR (A8X1, A8Y1, A8X2, A8Y2);
    Army8 -> SetRadar (R8X, R8Y);
    Army8 -> InsertUnits ();
    
    AirArmy8 = new TAirArmy ();
    AirArmy8 -> SetAR (AA8X1, AA8Y1, AA8X2, AA8Y2);
    AirArmy8 -> SetRadar (R8X, R8Y);
    AirArmy8 -> Status = asActive;
    AirArmy8 -> InsertUnits ();

    
    // Jednotky do zakladny
    Base = (TBase*)Units [Base1];
    for (i = TB1X1; i <= TB1X2; i++) {
        for (j = TB1Y1; j <= TB1Y2; j++) {
            f = GetField (i, j);
            if ((f -> Unit != NO_UNIT) && (f -> Unit >= BADLIFE)) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                Base -> Inventory [Base -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
                
            }
        }
    }
    // Kobra 1 do Airportu
    Airport = (TAirport*)Units [Airport1];
    Units [Kobra1] -> PlaceGround (FALSE);
    Airport -> Inventory [Airport -> LoadedUnits++] = Kobra1;
    Units [Kobra1] -> X = Units [Kobra1] -> Y = -1;
    
    // Rexove do Heliportu 4
    Heliport = (THeliport *)Units [Heliport4];
    for (i = TH4X1; i <= TH4X2; i++) {
        for (j = TH4Y1; j <= TH4Y2; j++) {
            u = GetAircraftAt (i, j);
            if ((u != NULL) && (u -> Type >= BADLIFE)) {
                u -> PlaceGround (FALSE);               
                Heliport -> Inventory [Heliport -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
                
            }
        }
    }
    
    
    // Polozime miny
    for (i = 0; MinePlaces5 [i].x != -1; i++) {
        MinePlace (MinePlaces5 [i].x, MinePlaces5 [i].y, BADLIFE);
    }
    
    UnlockDraw ();          


///////////////////////////////////////////////////////////////////



    

}




void LoadArtificialIntelligence5(ReadStream &stream, int format) {
	DoneArtificialIntelligence5();

	MBPlaces = NULL;
	DeniedPlaces = NULL;
	DUPos = 0;
	loadAttackedFields(stream, format);

	Xenon3 = stream.readSint32LE();
	Kobra1 = stream.readSint32LE();
	ActivationTurn1 = stream.readSint32LE();
	ActivationTurn2 = stream.readSint32LE();
	ActivationTurn3 = stream.readSint32LE();
	ActivationTurn4 = stream.readSint32LE();
	ActivationTurn5 = stream.readSint32LE();
	ActivationTurn6 = stream.readSint32LE();
	ActivationTurn7 = stream.readSint32LE();
	ActivationTurn8 = stream.readSint32LE();
	Xenon3ActivationTurn = stream.readSint32LE();
	X3MineCnt = stream.readSint32LE();

	Towers = new TTowers(stream);
	Army1 = new TGroundArmy(stream);
	AirArmy1 = new TAirArmy(stream);
	Army2 = new TGroundArmy(stream);
	Army3 = new TGroundArmy(stream);
	Army4 = new TGroundArmy(stream);
	Army5 = new TGroundArmy(stream);
	Army6 = new TGroundArmy(stream);
	Army7 = new TGroundArmy(stream);
	Army8 = new TGroundArmy(stream);
	AirArmy8 = new TAirArmy(stream);
}

void SaveArtificialIntelligence5(WriteStream &stream) {
	saveAttackedFields(stream);

	stream.writeSint32LE(Xenon3);
	stream.writeSint32LE(Kobra1);
	stream.writeSint32LE(ActivationTurn1);
	stream.writeSint32LE(ActivationTurn2);
	stream.writeSint32LE(ActivationTurn3);
	stream.writeSint32LE(ActivationTurn4);
	stream.writeSint32LE(ActivationTurn5);
	stream.writeSint32LE(ActivationTurn6);
	stream.writeSint32LE(ActivationTurn7);
	stream.writeSint32LE(ActivationTurn8);
	stream.writeSint32LE(Xenon3ActivationTurn);
	stream.writeSint32LE(X3MineCnt);

	Towers->Save(stream);
	Army1->Save(stream);
	AirArmy1->Save(stream);
	Army2->Save(stream);
	Army3->Save(stream);
	Army4->Save(stream);
	Army5->Save(stream);
	Army6->Save(stream);
	Army7->Save(stream);
	Army8->Save(stream);
	AirArmy8->Save(stream);
	DUPos = 0;
}

int ArtificialIntelligence5 ()
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
    if (Base1 == NO_UNIT || Units [Base1] -> Type % BADLIFE != unBase) 
        Base1 = NO_UNIT;
    Base8 = GetField (B8X, B8Y) -> Unit;
    if (Base8 == NO_UNIT || Units [Base8] -> Type % BADLIFE != unBase) 
        Base8 = NO_UNIT;
    Radar1 = GetField (R1X, R1Y) -> Unit;
    if (Radar1 == NO_UNIT || Units [Radar1] -> Type % BADLIFE != unRadar) 
        Radar1 = NO_UNIT;
    Radar2 = GetField (R2X, R2Y) -> Unit;
    if (Radar2 == NO_UNIT || Units [Radar2] -> Type % BADLIFE != unRadar) 
        Radar2 = NO_UNIT;
    Radar4 = GetField (R4X, R4Y) -> Unit;
    if (Radar4 == NO_UNIT || Units [Radar4] -> Type % BADLIFE != unRadar) 
        Radar4 = NO_UNIT;
    Radar5 = GetField (R5X, R5Y) -> Unit;
    if (Radar5 == NO_UNIT || Units [Radar5] -> Type % BADLIFE != unRadar) 
        Radar5 = NO_UNIT;
    Radar8 = GetField (R8X, R8Y) -> Unit;
    if (Radar8 == NO_UNIT || Units [Radar8] -> Type % BADLIFE != unRadar) 
        Radar8 = NO_UNIT;       
    Factory8 = GetField (F8X, F8Y) -> Unit;
    if (Factory8 == NO_UNIT || Units [Factory8] -> Type % BADLIFE != unFactory) 
        Factory8 = NO_UNIT;
    Airport1 = GetField (A1X, A1Y) -> Unit;
    if (Airport1 == NO_UNIT || Units [Airport1] -> Type % BADLIFE != unAirport) 
        Airport1 = NO_UNIT;
    Airport8 = GetField (A8X, A8Y) -> Unit;
    if (Airport8 == NO_UNIT || Units [Airport8] -> Type % BADLIFE != unAirport) 
        Airport8 = NO_UNIT;

    // Vyradime znicene jednotky z armad    
    Towers -> DeleteKilled2 (); Towers -> DeleteKilled ();
    Army1 -> DeleteKilled2 (); Army1 -> DeleteKilled ();
    AirArmy1 -> DeleteKilled2 (); AirArmy1 -> DeleteKilled ();
    Army2 -> DeleteKilled2 (); Army2 -> DeleteKilled ();
    Army3 -> DeleteKilled2 (); Army3 -> DeleteKilled ();
    Army4 -> DeleteKilled2 (); Army4 -> DeleteKilled ();
    Army5 -> DeleteKilled2 (); Army5 -> DeleteKilled ();
    Army6 -> DeleteKilled2 (); Army6 -> DeleteKilled ();
    Army7 -> DeleteKilled2 (); Army7 -> DeleteKilled ();
    Army8 -> DeleteKilled2 (); Army8 -> DeleteKilled ();
    AirArmy8 -> DeleteKilled2 (); AirArmy8 -> DeleteKilled ();
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
    AirArmy1 -> DoAttackedFields ();
    Army2 -> DoAttackedFields ();
    Army3 -> DoAttackedFields ();
    Army4 -> DoAttackedFields ();
    Army5 -> DoAttackedFields ();
    Army6 -> DoAttackedFields ();
    Army7 -> DoAttackedFields ();
    Army8 -> DoAttackedFields ();
    AirArmy8 -> DoAttackedFields ();
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
                Army2 -> SetAIConst (2); // AGRESIVE ARMY
            }
        }
    }
    if (Army2 -> Status == asActive) {
        if (Army2 -> Dangerous <= A2Escape && Army2 -> WayPoint.x == -1) {
            Army2 -> SetAIConst (3); // MOVING ARMY
            Army2 -> WayPoint.x = A2WP1X; Army2 -> WayPoint.y = A2WP1Y;         
        }
        if (Army2 -> IsAtWP () 
        && Army2 -> WayPoint.x == A2WP1X && Army2 -> WayPoint.y == A2WP1Y) {
            Army2 -> WayPoint.x = A2WP2X; Army2 -> WayPoint.y = A2WP2Y;
        }
        if (Army2 -> IsAtWP () 
        && Army2 -> WayPoint.x == A2WP2X && Army2 -> WayPoint.y == A2WP2Y) {
            Army2 -> SetAIConst (1);
            Xenon3ActivationTurn = ActualTurn;
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
    

///////////////////////////// AKCE ARMAD

    // Nastavime ProgressBar
    st = 0;
    st += Towers -> nofBadLife;
    if (Army1 -> Status == asActive || Army1 -> Status == asSuicide) st += Army1 -> nofBadLife;
    if (AirArmy1 -> Status == asActive || AirArmy1 -> Status == asSuicide) st += AirArmy1 -> nofBadLife;
    if (Army2 -> Status == asActive || Army2 -> Status == asSuicide) st += Army2 -> nofBadLife;
    if (Army3 -> Status == asActive || Army3 -> Status == asSuicide) st += Army3 -> nofBadLife;
    if (Army4 -> Status == asActive || Army4 -> Status == asSuicide) st += Army4 -> nofBadLife;
    if (Army5 -> Status == asActive || Army5 -> Status == asSuicide) st += Army5 -> nofBadLife;
    if (Army6 -> Status == asActive || Army6 -> Status == asSuicide) st += Army6 -> nofBadLife;
    if (Army7 -> Status == asActive || Army7 -> Status == asSuicide) st += Army7 -> nofBadLife;
    if (Army8 -> Status == asActive || Army8 -> Status == asSuicide) st += Army8 -> nofBadLife;
    if (AirArmy8 -> Status == asActive || AirArmy8 -> Status == asSuicide) st += AirArmy8 -> nofBadLife;
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

    // AIR ARMY 8
    if (AirArmy8 -> Status == asActive || AirArmy8 -> Status == asSuicide) {
        AirArmy8 -> DeleteKilled ();

        // Zarazeni novych nepratel (GL)
        for (i = 0; i < BADLIFE; i++) {
            if (Units [i] == NULL) continue;            
            if (Army8 -> IsInVisualRange (Units [i] -> X, Units [i] -> Y)) {
                AirArmy8 -> Insert (i);
            }
        }
        
        // Pokyn k sebevrazde       
        AirArmy8 -> CountDangerous ();
        if (AirArmy8 -> Dangerous <= CSuicide8) {
            AirArmy8 -> Status = asSuicide;
        }       
        
        // Provedeme akce
        if (AirArmy8 -> MakeTurn () == FALSE) {
            st = AssignResult ();
            if (st != 0) {
                UnlockDraw ();  
                AllowBadlife (FALSE);       
                return st;  
            }
        }
        
        // Likvidace armady 8
        if (AirArmy8 -> Dangerous <= CMinDang8) {
            AirArmy8 -> Status = asDestroyed;
        }               
    }


    // Xenon 3
    if (Xenon3 != NO_UNIT && Units [Xenon3] != NULL && Units [Xenon3] -> Type - BADLIFE == unXenon
    && Xenon3ActivationTurn <= ActualTurn && X3MineCnt >= 0) {
        for (i = X3MineCnt; Xenon3Mines [i].x != -1; i++) {
            UnlockDraw ();
            st = ((TXenon *)Units [Xenon3]) 
            -> MoveFar (Xenon3Mines [i].x, Xenon3Mines [i].y);
            if (Units [Xenon3] == NULL) {
                LockDraw (); // Jednotka znicena
                break;
            }
            if (st) {
                st = ((TXenon *)Units [Xenon3]) -> PlaceMine ();
                if (st) {
                    X3MineCnt++;
                    if (X3MineCnt == 3) ((TXenon *)Units [Xenon3]) 
                        -> MoveFar (A2WP2X, A2WP2Y);
                }
            }
            LockDraw ();            
        }
    }




    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult (); 
}




int AssignResult5 ()
{
    int i, t = 0;
    
    Base8 = GetField (B8X, B8Y) -> Unit;
    if (Base8 == NO_UNIT || Units [Base8] -> Type % BADLIFE != unBase) 
        Base8 = NO_UNIT;

    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL && Units [i] -> Type < unRadar) t++;
    if (t == 0) return -1; // GoodLife prohral  

    if (Base8 == NO_UNIT || Base8 < BADLIFE) return 1; // BadLife prohral
    
    return 0; // Zatim se jeste hraje
}



int *GetManufacturableUnits5 (int FactoryID)
{
    return Factory5List;
}



void DoneArtificialIntelligence5() {
	free_landscape_analysis();

	delete Army1;
	Army1 = NULL;
	delete AirArmy1;
	AirArmy1 = NULL;
	delete Army2;
	Army2 = NULL;
	delete Army3;
	Army3 = NULL;
	delete Army4;
	Army4 = NULL;
	delete Army5;
	Army5 = NULL;
	delete Army6;
	Army6 = NULL;
	delete Army7;
	Army7 = NULL;
	delete Army8;
	Army8 = NULL;
	delete AirArmy8;
	AirArmy8 = NULL;
	delete Towers;
	Towers = NULL;
}

    
    
// OK   
