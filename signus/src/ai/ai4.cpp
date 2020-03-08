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

#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"

// Action Radius of Armies
#define A1X1 10
#define A1Y1 68
#define A1X2 18
#define A1Y2 73

#define A2X1 12
#define A2Y1 81
#define A2X2 16
#define A2Y2 85

#define A3X1 20
#define A3Y1 85
#define A3X2 23
#define A3Y2 92

#define A4X1 11
#define A4Y1 90
#define A4X2 16
#define A4Y2 96

#define A5X1 20
#define A5Y1 94
#define A5X2 26
#define A5Y2 99

#define A6X1 28
#define A6Y1 94
#define A6X2 33
#define A6Y2 98

#define A7X1 12
#define A7Y1 99
#define A7X2 16
#define A7Y2 105

#define A8X1 22
#define A8Y1 101
#define A8X2 28
#define A8Y2 105

#define A9X1 85
#define A9Y1 87
#define A9X2 91
#define A9Y2 89

#define A10X1 83
#define A10Y1 92
#define A10X2 88
#define A10Y2 96

#define A11X1 95
#define A11Y1 94
#define A11X2 100
#define A11Y2 98

#define A12X1 83
#define A12Y1 98
#define A12X2 88
#define A12Y2 102

#define A13X1 95
#define A13Y1 100
#define A13X2 99
#define A13Y2 104

#define A14X1 102
#define A14Y1 100
#define A14X2 107
#define A14Y2 105


// Uran 4
#define U4X 15
#define U4Y 95

#define TU4X1 11
#define TU4Y1 94
#define TU4X2 12
#define TU4Y2 95

#define U5X 25
#define U5Y 98

#define TU5X1 20
#define TU5Y1 98
#define TU5X2 24
#define TU5Y2 98

#define U6X 33
#define U6Y 98

#define TU6X1 28
#define TU6Y1 97
#define TU6X2 29
#define TU6Y2 98

#define U8X 27
#define U8Y 105

#define TU8X1 22
#define TU8Y1 105
#define TU8X2 24
#define TU8Y2 105

#define U11X 99
#define U11Y 97

#define TU11X1 95
#define TU11Y1 96
#define TU11X2 96
#define TU11Y2 96

#define U12X 87
#define U12Y 101

#define TU12X1 83
#define TU12Y1 100
#define TU12X2 85
#define TU12Y2 100

#define U13X 96
#define U13Y 103

#define TU13X1 95
#define TU13Y1 103
#define TU13X2 95
#define TU13Y2 103

#define U14X 106
#define U14Y 103

#define TU14X1 102
#define TU14Y1 103
#define TU14X2 103
#define TU14Y2 105

#define O9X 91
#define O9Y 87

#define TO9X1 85
#define TO9Y1 87
#define TO9X2 88
#define TO9Y2 89

#define O9DX 91
#define O9DY 79

#define M1X 91
#define M1Y 85

#define M1DX 91
#define M1DY 73

#define M2X 91
#define M2Y 86

#define M2DX 91
#define M2DY 75

#define WP1X 14
#define WP1Y 71

#define WP2X 91
#define WP2Y 79

#define WP3X 84
#define WP3Y 32

#define WP4X 41
#define WP4Y 10

#define WP5X 16
#define WP5Y 33

#define B1X 5
#define B1Y 7

#define B2X 17
#define B2Y 7

#define W1X 12
#define W1Y 55

#define W2X 89
#define W2Y 59

#define W3X 92
#define W3Y 59

#define W4X 77
#define W4Y 16

#define T4X 84
#define T4Y 14



#define CMinDang1  0
#define CMinDang2  0
#define CMinDang3  0
#define CMinDang4  0
#define CMinDang5  0
#define CMinDang6  0
#define CMinDang7  0
#define CMinDang8  0
#define CMinDang9  0
#define CMinDang10 0
#define CMinDang11 0
#define CMinDang12 0
#define CMinDang13 0
#define CMinDang14 0

#define CSuicide1  4
#define CSuicide2  5
#define CSuicide3  5
#define CSuicide4  5
#define CSuicide5  5
#define CSuicide6  5
#define CSuicide7  5
#define CSuicide8  5
#define CSuicide9  4
#define CSuicide10 5
#define CSuicide11 5
#define CSuicide12 5
#define CSuicide13 5
#define CSuicide14 5

#define MaxDist 2       // Maximalni doba mezi aktivaci nasledujicich armad

static int UnloadUran4, UnloadUran5, UnloadUran6, UnloadUran8;
static int UnloadUran11, UnloadUran12, UnloadUran13, UnloadUran14;

TPoint MBPlaces4 [] = {11,46, 11,47, 11,48, 12,46, 12,47, 12,48, // LO 1
                                             13,46, 13,47, 13,48, 14,46, 14,47, 14,48,
                                             15,46, 15,47, 15,48, 19,45, 19,46, 19,47,
                                             20,45, 20,46, 20,47, 25,45, 25,46, 25,47,
                                             26,45, 26,46, 26,47, 27,45, 27,46, 27,47,
                                             
                                             83,44, 83,45, 83,46, 84,44, 84,45, 84,46, // RO 1
                                             85,44, 85,45, 85,46, 86,44, 86,45, 86,46,
                                             75,43, 75,44, 75,45, 75,46, 75,47,
                                             76,43, 76,44, 76,45, 76,46, 76,47,
                                             77,43, 77,44, 77,45, 77,46, 77,47,
                                             87,44, 87,45, 87,46, 87,47,
                                             88,44, 88,45, 88,46, 88,47, 88,48,
                                             
                                             64,12, 65,12, 66,12, 67,12,               // RO 2
                                             64,13, 65,13, 66,13, 67,13,
                                             64,14, 65,14, 66,14, 67,14,
                                             65,15, 66,15, 67,15, 65,16, 66,16, 67,16,
                                             65,17, 66,17, 67,17, 65,18, 66,18, 67,18,
                                             64,19, 65,19, 66,19, 67,19, 
                                             64,20, 65,20, 66,20, 67,20,
                                             64,21, 65,21, 66,21, 67,21,
                                             
                                             
                                             
                       -1,-1};
                       
TPoint DeniedPlaces4 [] = {11,49, 12,49, 13,49, 14,49, 13,50, 13,51,
                                                     83,47, 84,47, 85,47, 86,47,
                                                     68,15, 68,16, 68,17, 68,18, 68,19,
                                                     
                                                     -1,-1};                      


void InitAI4 ()
{
    int i,j;
    TField *f;
    TUnit *u;

    AttackFieldPos = 0;
    DUPos = 0;

    MoneyGoodlife = 2500;
    MoneyBadlife = 0;
    MBPlaces = MBPlaces4;
    DeniedPlaces = DeniedPlaces4;

    ActivationTurn1 = 1;
    ActivationTurn2 = 1;
    ActivationTurn3 = 2;
    ActivationTurn4 = MaxInt;
    ActivationTurn5 = MaxInt;
    ActivationTurn6 = MaxInt;
    ActivationTurn7 = MaxInt;
    ActivationTurn8 = MaxInt;
    ActivationTurn9 = 1;
    ActivationTurn10 = 2;
    ActivationTurn11 = MaxInt;
    ActivationTurn12 = MaxInt;
    ActivationTurn13 = MaxInt;
    ActivationTurn14 = MaxInt;
    
    UnloadUran4 = MaxInt;
    UnloadUran5 = MaxInt;
    UnloadUran6 = MaxInt;
    UnloadUran8 = MaxInt;
    UnloadUran11 = MaxInt;
    UnloadUran12 = MaxInt;
    UnloadUran13 = MaxInt;
    UnloadUran14 = MaxInt;

    Base1 = GetField (B1X, B1Y) -> Unit;
    Base2 = GetField (B2X, B2Y) -> Unit;
    Warehouse1 = GetField (W1X, W1Y) -> Unit;
    Warehouse2 = GetField (W2X, W2Y) -> Unit;
    Warehouse3 = GetField (W3X, W3Y) -> Unit;
    Warehouse4 = GetField (W4X, W4Y) -> Unit;
    Tower4 = GetField (T4X, T4Y) -> Unit;
    Mohykan1 = GetField (M1X, M1Y) -> Unit;
    Mohykan2 = GetField (M2X, M2Y) -> Unit;
    Uran4 = GetField (U4X, U4Y) -> Unit;
    Uran5 = GetField (U5X, U5Y) -> Unit;
    Uran6 = GetField (U6X, U6Y) -> Unit;
    Uran8 = GetField (U8X, U8Y) -> Unit;
    Uran11 = GetField (U11X, U11Y) -> Unit;
    Uran12 = GetField (U12X, U12Y) -> Unit;
    Uran13 = GetField (U13X, U13Y) -> Unit;
    Uran14 = GetField (U14X, U14Y) -> Unit;
    Olymp9 = GetField (O9X, O9Y) -> Unit;



    LockDraw ();    
    // Jednotky do Olympu   9
    for (i = TO9X1; i <= TO9X2; i++) {
        for (j = TO9Y1; j <= TO9Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TOlymp*)Units [Olymp9]) -> Inventory [((TOlymp*)Units [Olymp9]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    // Jednotky do Uranu 4
    for (i = TU4X1; i <= TU4X2; i++) {
        for (j = TU4Y1; j <= TU4Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran4]) -> Inventory [((TUran*)Units [Uran4]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    // Jednotky do Uranu 5
    for (i = TU5X1; i <= TU5X2; i++) {
        for (j = TU5Y1; j <= TU5Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran5]) -> Inventory [((TUran*)Units [Uran5]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    // Jednotky do Uranu 6
    for (i = TU6X1; i <= TU6X2; i++) {
        for (j = TU6Y1; j <= TU6Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran6]) -> Inventory [((TUran*)Units [Uran6]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    // Jednotky do Uranu 8
    for (i = TU8X1; i <= TU8X2; i++) {
        for (j = TU8Y1; j <= TU8Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran8]) -> Inventory [((TUran*)Units [Uran8]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    // Jednotky do Uranu 11
    for (i = TU11X1; i <= TU11X2; i++) {
        for (j = TU11Y1; j <= TU11Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran11]) -> Inventory [((TUran*)Units [Uran11]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    // Jednotky do Uranu 12
    for (i = TU12X1; i <= TU12X2; i++) {
        for (j = TU12Y1; j <= TU12Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran12]) -> Inventory [((TUran*)Units [Uran12]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    // Jednotky do Uranu 13
    for (i = TU13X1; i <= TU13X2; i++) {
        for (j = TU13Y1; j <= TU13Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran13]) -> Inventory [((TUran*)Units [Uran13]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }       
    // Jednotky do Uranu 14
    for (i = TU14X1; i <= TU14X2; i++) {
        for (j = TU14Y1; j <= TU14Y2; j++) {
            f = GetField (i, j);
            if (f -> Unit != NO_UNIT) {
                u = (TUnit *)Units [f -> Unit];
                u -> PlaceGround (FALSE);               
                ((TUran*)Units [Uran14]) -> Inventory [((TUran*)Units [Uran14]) 
                -> LoadedUnits++] = u -> ID;
                u -> X = u -> Y = -1;
            }
        }
    }           
    UnlockDraw ();          


        
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
    Army1 -> SetAIConst (2); // AGRESIVE ARMY
    Army1 -> WayPoint.x = WP1X; Army1 -> WayPoint.y = WP1Y;
    
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
    Army6 -> Displace ();

    Army7 = new TGroundArmy();
    Army7 -> SetAR (A7X1, A7Y1, A7X2, A7Y2);
    Army7 -> SetRadar (-1, -1);
    Army7 -> InsertUnits ();
    Army7 -> Displace ();

    Army8 = new TGroundArmy();
    Army8 -> SetAR (A8X1, A8Y1, A8X2, A8Y2);
    Army8 -> SetRadar (-1, -1);
    Army8 -> InsertUnits ();
    Army8 -> Displace ();

    Army9 = new TGroundArmy();
    Army9 -> SetAR (A9X1, A9Y1, A9X2, A9Y2);
    Army9 -> SetRadar (-1, -1);
    Army9 -> InsertUnits ();

    Army10 = new TGroundArmy();
    Army10 -> SetAR (A10X1, A10Y1, A10X2, A10Y2);
    Army10 -> SetRadar (-1, -1);
    Army10 -> InsertUnits ();
    Army10 -> Displace ();

    Army11 = new TGroundArmy();
    Army11 -> SetAR (A11X1, A11Y1, A11X2, A11Y2);
    Army11 -> SetRadar (-1, -1);
    Army11 -> InsertUnits ();
    Army11 -> Displace ();
    
    Army12 = new TGroundArmy();
    Army12 -> SetAR (A12X1, A12Y1, A12X2, A12Y2);
    Army12 -> SetRadar (-1, -1);
    Army12 -> InsertUnits ();
    Army12 -> Displace ();

    Army13 = new TGroundArmy();
    Army13 -> SetAR (A13X1, A13Y1, A13X2, A13Y2);
    Army13 -> SetRadar (-1, -1);
    Army13 -> InsertUnits ();
    Army13 -> Displace ();

    Army14 = new TGroundArmy();
    Army14 -> SetAR (A14X1, A14Y1, A14X2, A14Y2);
    Army14 -> SetRadar (-1, -1);
    Army14 -> InsertUnits ();
    Army14 -> Displace ();

///////////////////////////////////////////////////////////////////
    

}



void LoadArtificialIntelligence4(ReadStream &stream, int format) {
	DoneArtificialIntelligence4();

	MBPlaces = MBPlaces4;
	DeniedPlaces = DeniedPlaces4;
	DUPos = 0;
	loadAttackedFields(stream, format);

	Uran4 = stream.readSint32LE();
	Uran5 = stream.readSint32LE();
	Uran6 = stream.readSint32LE();
	Uran8 = stream.readSint32LE();
	Uran11 = stream.readSint32LE();
	Uran12 = stream.readSint32LE();
	Uran13 = stream.readSint32LE();
	Uran14 = stream.readSint32LE();
	Olymp9 = stream.readSint32LE();
	Mohykan1 = stream.readSint32LE();
	Mohykan2 = stream.readSint32LE();
	ActivationTurn1 = stream.readSint32LE();
	ActivationTurn2 = stream.readSint32LE();
	ActivationTurn3 = stream.readSint32LE();
	ActivationTurn4 = stream.readSint32LE();
	ActivationTurn5 = stream.readSint32LE();
	ActivationTurn6 = stream.readSint32LE();
	ActivationTurn7 = stream.readSint32LE();
	ActivationTurn8 = stream.readSint32LE();
	ActivationTurn9 = stream.readSint32LE();
	ActivationTurn10 = stream.readSint32LE();
	ActivationTurn11 = stream.readSint32LE();
	ActivationTurn12 = stream.readSint32LE();
	ActivationTurn13 = stream.readSint32LE();
	ActivationTurn14 = stream.readSint32LE();
	UnloadUran4 = stream.readSint32LE();
	UnloadUran5 = stream.readSint32LE();
	UnloadUran6 = stream.readSint32LE();
	UnloadUran8 = stream.readSint32LE();
	UnloadUran11 = stream.readSint32LE();
	UnloadUran12 = stream.readSint32LE();
	UnloadUran13 = stream.readSint32LE();
	UnloadUran14 = stream.readSint32LE();

	Towers = new TTowers(stream);
	Army1 = new TGroundArmy(stream);
	Army2 = new TGroundArmy(stream);
	Army3 = new TGroundArmy(stream);
	Army4 = new TGroundArmy(stream);
	Army5 = new TGroundArmy(stream);
	Army6 = new TGroundArmy(stream);
	Army7 = new TGroundArmy(stream);
	Army8 = new TGroundArmy(stream);
	Army9 = new TGroundArmy(stream);
	Army10 = new TGroundArmy(stream);
	Army11 = new TGroundArmy(stream);
	Army12 = new TGroundArmy(stream);
	Army13 = new TGroundArmy(stream);
	Army14 = new TGroundArmy(stream);
}

void SaveArtificialIntelligence4(WriteStream &stream) {
	saveAttackedFields(stream);

	stream.writeSint32LE(Uran4);
	stream.writeSint32LE(Uran5);
	stream.writeSint32LE(Uran6);
	stream.writeSint32LE(Uran8);
	stream.writeSint32LE(Uran11);
	stream.writeSint32LE(Uran12);
	stream.writeSint32LE(Uran13);
	stream.writeSint32LE(Uran14);
	stream.writeSint32LE(Olymp9);
	stream.writeSint32LE(Mohykan1);
	stream.writeSint32LE(Mohykan2);
	stream.writeSint32LE(ActivationTurn1);
	stream.writeSint32LE(ActivationTurn2);
	stream.writeSint32LE(ActivationTurn3);
	stream.writeSint32LE(ActivationTurn4);
	stream.writeSint32LE(ActivationTurn5);
	stream.writeSint32LE(ActivationTurn6);
	stream.writeSint32LE(ActivationTurn7);
	stream.writeSint32LE(ActivationTurn8);
	stream.writeSint32LE(ActivationTurn9);
	stream.writeSint32LE(ActivationTurn10);
	stream.writeSint32LE(ActivationTurn11);
	stream.writeSint32LE(ActivationTurn12);
	stream.writeSint32LE(ActivationTurn13);
	stream.writeSint32LE(ActivationTurn14);
	stream.writeSint32LE(UnloadUran4);
	stream.writeSint32LE(UnloadUran5);
	stream.writeSint32LE(UnloadUran6);
	stream.writeSint32LE(UnloadUran8);
	stream.writeSint32LE(UnloadUran11);
	stream.writeSint32LE(UnloadUran12);
	stream.writeSint32LE(UnloadUran13);
	stream.writeSint32LE(UnloadUran14);

	Towers->Save(stream);
	Army1->Save(stream);
	Army2->Save(stream);
	Army3->Save(stream);
	Army4->Save(stream);
	Army5->Save(stream);
	Army6->Save(stream);
	Army7->Save(stream);
	Army8->Save(stream);
	Army9->Save(stream);
	Army10->Save(stream);
	Army11->Save(stream);
	Army12->Save(stream);
	Army13->Save(stream);
	Army14->Save(stream);
	DUPos = 0;
}

int ArtificialIntelligence4 ()
{
    int i, st, id;

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
    Warehouse1 = GetField (W1X, W1Y) -> Unit;
    if (Warehouse1 == NO_UNIT || Units [Warehouse1] -> Type % BADLIFE != unWarehouse) 
        Warehouse1 = NO_UNIT;
    Warehouse2 = GetField (W2X, W2Y) -> Unit;
    if (Warehouse2 == NO_UNIT || Units [Warehouse2] -> Type % BADLIFE != unWarehouse) 
        Warehouse2 = NO_UNIT;
    Warehouse3 = GetField (W3X, W3Y) -> Unit;
    if (Warehouse3 == NO_UNIT || Units [Warehouse3] -> Type % BADLIFE != unWarehouse) 
        Warehouse3 = NO_UNIT;
    Warehouse4 = GetField (W4X, W4Y) -> Unit;
    if (Warehouse4 == NO_UNIT || Units [Warehouse4] -> Type % BADLIFE != unWarehouse) 
        Warehouse4 = NO_UNIT;
    Tower4 = GetField (T4X, T4Y) -> Unit;
    if (Tower4 == NO_UNIT || Units [Tower4] -> Type % BADLIFE != unPagoda) 
        Tower4 = NO_UNIT;
    
    

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
    AttackFieldPos = 0;


    AnalyseLandscape ();    
    
    Message (SigText[TXT_AI_ARMY]);
    

//////////////////////////////// AKTIVACE ARMAD
    
    

    // ARMY 1 - SPECIAL ACTIVATION  
    if (ActualTurn >= ActivationTurn1 && Army1 -> Status == asSleeping) {
        Army1 -> Status = asActive;
        Army1 -> WayPoint.x = WP1X; Army1 -> WayPoint.y = WP1Y;
        Army1 -> SetAIConst (3); // Moving Army     
    }
    if (Army1 -> Status == asActive) {
        // WayPoint 1 - in front of Left Outpost
        if (Army1 -> IsAtWP () && Army1 -> WayPoint.x == WP1X 
        && Army1 -> WayPoint.y == WP1Y) {
            if (Warehouse1 >= BADLIFE) { // Left OutPost lost   
                Army1 -> WayPoint.x = WP5X; Army1 -> WayPoint.y = WP5Y;
                Army1 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army1 -> WayPoint.x = W1X; Army1 -> WayPoint.y = W1Y;
                Army1 -> SetAIConst (2); // AGRESIVE ARMY               
            }
        }
        // At Warehouse 1 
        if (Army1 -> IsAtWP () && Army1 -> WayPoint.x == W1X 
        && Army1 -> WayPoint.y == W1Y && Warehouse1 >= BADLIFE) {
            Army1 -> WayPoint.x = WP5X; Army1 -> WayPoint.y = WP5Y;
            Army1 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
        }       
        // WayPoint 5 - At the back of Bridge
        if (Army1 -> IsAtWP () && Army1 -> WayPoint.x == WP5X 
        && Army1 -> WayPoint.y == WP5Y) { 
            Army1 -> WayPoint.x = B1X; Army1 -> WayPoint.y = B1Y;
            Army1 -> SetAIConst (2); // AGRESIVE ARMY
            Army1 -> Insert (Base1);
        }               
    }

    
    // ARMY 2 - SPECIAL ACTIVATION  
    if (ActualTurn >= ActivationTurn2 && Army2 -> Status == asSleeping) {
        Army2 -> Place ();      
        Army2 -> Status = asActive;
        Army2 -> WayPoint.x = WP1X; Army2 -> WayPoint.y = WP1Y;
        Army2 -> SetAIConst (3); // Moving Army     
    }
    if (Army2 -> Status == asActive) {
        // WayPoint 1 - in front of Left Outpost
        if (Army2 -> IsAtWP () && Army2 -> WayPoint.x == WP1X 
        && Army2 -> WayPoint.y == WP1Y) {
            if (Warehouse1 >= BADLIFE) { // Left OutPost lost   
                Army2 -> WayPoint.x = WP5X; Army2 -> WayPoint.y = WP5Y;
                Army2 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army2 -> WayPoint.x = W1X; Army2 -> WayPoint.y = W1Y;
                Army2 -> SetAIConst (2); // AGRESIVE ARMY               
            }
        }
        // At Warehouse 1 
        if (Army2 -> IsAtWP () && Army2 -> WayPoint.x == W1X 
        && Army2 -> WayPoint.y == W1Y && Warehouse1 >= BADLIFE) {
            Army2 -> WayPoint.x = WP5X; Army2 -> WayPoint.y = WP5Y;
            Army2 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
        }       
        // WayPoint 5 - At the back of Bridge
        if (Army2 -> IsAtWP () && Army2 -> WayPoint.x == WP5X 
        && Army2 -> WayPoint.y == WP5Y) { 
            Army2 -> WayPoint.x = B1X; Army2 -> WayPoint.y = B1Y;
            Army2 -> SetAIConst (2); // AGRESIVE ARMY
            Army2 -> Insert (Base1);
        }               
    }

    // ARMY 3 - SPECIAL ACTIVATION  
    if (ActualTurn >= ActivationTurn3 && Army3 -> Status == asSleeping) {
        Army3 -> Place ();      
        Army3 -> Status = asActive;
        Army3 -> WayPoint.x = WP1X; Army3 -> WayPoint.y = WP1Y;
        Army3 -> SetAIConst (3); // Moving Army     
    }
    if (Army3 -> Status == asActive) {
        // WayPoint 1 - in front of Left Outpost
        if (Army3 -> IsAtWP () && Army3 -> WayPoint.x == WP1X 
        && Army3 -> WayPoint.y == WP1Y) {
            if (Warehouse1 >= BADLIFE) { // Left OutPost lost   
                Army3 -> WayPoint.x = WP5X; Army3 -> WayPoint.y = WP5Y;
                Army3 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army3 -> WayPoint.x = W1X; Army3 -> WayPoint.y = W1Y;
                Army3 -> SetAIConst (2); // AGRESIVE ARMY               
            }
        }
        // At Warehouse 1 
        if (Army3 -> IsAtWP () && Army3 -> WayPoint.x == W1X 
        && Army3 -> WayPoint.y == W1Y && Warehouse1 >= BADLIFE) {
            Army3 -> WayPoint.x = WP5X; Army3 -> WayPoint.y = WP5Y;
            Army3 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
        }       
        // WayPoint 5 - At the back of Bridge
        if (Army3 -> IsAtWP () && Army3 -> WayPoint.x == WP5X 
        && Army3 -> WayPoint.y == WP5Y) { 
            Army3 -> WayPoint.x = B1X; Army3 -> WayPoint.y = B1Y;
            Army3 -> SetAIConst (2); // AGRESIVE ARMY
            Army3 -> Insert (Base1);
        }               
    }
    
    
    // ARMY 4 - SPECIAL ACTIVATION  
    if (ActualTurn >= ActivationTurn4 && Army4 -> Status == asSleeping) {
        Army4 -> Place ();      
        Army4 -> Status = asActive;
        Army4 -> WayPoint.x = WP1X; Army4 -> WayPoint.y = WP1Y;
        Army4 -> SetAIConst (3); // Moving Army     
    }
    if (Army4 -> Status == asActive) {
        // WayPoint 1 - in front of Left Outpost
        if (Army4 -> IsAtWP () && Army4 -> WayPoint.x == WP1X 
        && Army4 -> WayPoint.y == WP1Y) {
            if (Warehouse1 >= BADLIFE) { // Left OutPost lost   
                Army4 -> WayPoint.x = WP5X; Army4 -> WayPoint.y = WP5Y;
                Army4 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army4 -> WayPoint.x = W1X; Army4 -> WayPoint.y = W1Y;
                Army4 -> SetAIConst (2); // AGRESIVE ARMY               
                UnloadUran4 = ActualTurn;
            }
        }
        // At Warehouse 1 
        if (Army4 -> IsAtWP () && Army4 -> WayPoint.x == W1X 
        && Army4 -> WayPoint.y == W1Y && Warehouse1 >= BADLIFE) {
            Army4 -> WayPoint.x = WP5X; Army4 -> WayPoint.y = WP5Y;
            Army4 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
        }       
        // WayPoint 5 - At the back of Bridge
        if (Army4 -> IsAtWP () && Army4 -> WayPoint.x == WP5X 
        && Army4 -> WayPoint.y == WP5Y) { 
            Army4 -> WayPoint.x = B1X; Army4 -> WayPoint.y = B1Y;
            Army4 -> SetAIConst (2); // AGRESIVE ARMY
            Army4 -> Insert (Base1);
            UnloadUran4 = ActualTurn;
        }               
    }
    
    
    // ARMY 5 - SPECIAL ACTIVATION  
    if (ActualTurn >= ActivationTurn5 && Army5 -> Status == asSleeping) {
        Army5 -> Place ();      
        Army5 -> Status = asActive;
        Army5 -> WayPoint.x = WP1X; Army5 -> WayPoint.y = WP1Y;
        Army5 -> SetAIConst (3); // Moving Army     
    }
    if (Army5 -> Status == asActive) {
        // WayPoint 1 - in front of Left Outpost
        if (Army5 -> IsAtWP () && Army5 -> WayPoint.x == WP1X 
        && Army5 -> WayPoint.y == WP1Y) {
            if (Warehouse1 >= BADLIFE) { // Left OutPost lost   
                Army5 -> WayPoint.x = WP5X; Army5 -> WayPoint.y = WP5Y;
                Army5 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army5 -> WayPoint.x = W1X; Army5 -> WayPoint.y = W1Y;
                Army5 -> SetAIConst (2); // AGRESIVE ARMY               
                UnloadUran5 = ActualTurn;
            }
        }
        // At Warehouse 1 
        if (Army5 -> IsAtWP () && Army5 -> WayPoint.x == W1X 
        && Army5 -> WayPoint.y == W1Y && Warehouse1 >= BADLIFE) {
            Army5 -> WayPoint.x = WP5X; Army5 -> WayPoint.y = WP5Y;
            Army5 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
        }       
        // WayPoint 5 - At the back of Bridge
        if (Army5 -> IsAtWP () && Army5 -> WayPoint.x == WP5X 
        && Army5 -> WayPoint.y == WP5Y) { 
            Army5 -> WayPoint.x = B1X; Army5 -> WayPoint.y = B1Y;
            Army5 -> SetAIConst (2); // AGRESIVE ARMY
            Army5 -> Insert (Base1);
            UnloadUran5 = ActualTurn;
        }               
    }
    
    // ARMY 6 - SPECIAL ACTIVATION  
    if (ActualTurn >= ActivationTurn6 && Army6 -> Status == asSleeping) {
        Army6 -> Place ();      
        Army6 -> Status = asActive;
        Army6 -> WayPoint.x = WP1X; Army6 -> WayPoint.y = WP1Y;
        Army6 -> SetAIConst (3); // Moving Army     
    }
    if (Army6 -> Status == asActive) {
        // WayPoint 1 - in front of Left Outpost
        if (Army6 -> IsAtWP () && Army6 -> WayPoint.x == WP1X 
        && Army6 -> WayPoint.y == WP1Y) {
            if (Warehouse1 >= BADLIFE) { // Left OutPost lost   
                Army6 -> WayPoint.x = WP5X; Army6 -> WayPoint.y = WP5Y;
                Army6 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army6 -> WayPoint.x = W1X; Army6 -> WayPoint.y = W1Y;
                Army6 -> SetAIConst (2); // AGRESIVE ARMY               
                UnloadUran6 = ActualTurn;
            }
        }
        // At Warehouse 1 
        if (Army6 -> IsAtWP () && Army6 -> WayPoint.x == W1X 
        && Army6 -> WayPoint.y == W1Y && Warehouse1 >= BADLIFE) {
            Army6 -> WayPoint.x = WP5X; Army6 -> WayPoint.y = WP5Y;
            Army6 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
        }       
        // WayPoint 5 - At the back of Bridge
        if (Army6 -> IsAtWP () && Army6 -> WayPoint.x == WP5X 
        && Army6 -> WayPoint.y == WP5Y) { 
            Army6 -> WayPoint.x = B1X; Army6 -> WayPoint.y = B1Y;
            Army6 -> SetAIConst (2); // AGRESIVE ARMY
            Army6 -> Insert (Base1);
            UnloadUran6 = ActualTurn;
        }               
    }

    // ARMY 7 - SPECIAL ACTIVATION  
    if (ActualTurn >= ActivationTurn7 && Army7 -> Status == asSleeping) {
        Army7 -> Place ();      
        Army7 -> Status = asActive;
        Army7 -> WayPoint.x = WP1X; Army7 -> WayPoint.y = WP1Y;
        Army7 -> SetAIConst (3); // Moving Army     
    }
    if (Army7 -> Status == asActive) {
        // WayPoint 1 - in front of Left Outpost
        if (Army7 -> IsAtWP () && Army7 -> WayPoint.x == WP1X 
        && Army7 -> WayPoint.y == WP1Y) {
            if (Warehouse1 >= BADLIFE) { // Left OutPost lost   
                Army7 -> WayPoint.x = WP5X; Army7 -> WayPoint.y = WP5Y;
                Army7 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army7 -> WayPoint.x = W1X; Army7 -> WayPoint.y = W1Y;
                Army7 -> SetAIConst (2); // AGRESIVE ARMY               
            }
        }
        // At Warehouse 1 
        if (Army7 -> IsAtWP () && Army7 -> WayPoint.x == W1X 
        && Army7 -> WayPoint.y == W1Y && Warehouse1 >= BADLIFE) {
            Army7 -> WayPoint.x = WP5X; Army7 -> WayPoint.y = WP5Y;
            Army7 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
        }       
        // WayPoint 5 - At the back of Bridge
        if (Army7 -> IsAtWP () && Army7 -> WayPoint.x == WP5X 
        && Army7 -> WayPoint.y == WP5Y) { 
            Army7 -> WayPoint.x = B1X; Army7 -> WayPoint.y = B1Y;
            Army7 -> SetAIConst (2); // AGRESIVE ARMY
            Army7 -> Insert (Base1);
        }               
    }

    // ARMY 8 - SPECIAL ACTIVATION  
    if (ActualTurn >= ActivationTurn8 && Army8 -> Status == asSleeping) {
        Army8 -> Place ();      
        Army8 -> Status = asActive;
        Army8 -> WayPoint.x = WP1X; Army8 -> WayPoint.y = WP1Y;
        Army8 -> SetAIConst (3); // Moving Army     
    }
    if (Army8 -> Status == asActive) {
        // WayPoint 1 - in front of Left Outpost
        if (Army8 -> IsAtWP () && Army8 -> WayPoint.x == WP1X 
        && Army8 -> WayPoint.y == WP1Y) {
            if (Warehouse1 >= BADLIFE) { // Left OutPost lost   
                Army8 -> WayPoint.x = WP5X; Army8 -> WayPoint.y = WP5Y;
                Army8 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army8 -> WayPoint.x = W1X; Army8 -> WayPoint.y = W1Y;
                Army8 -> SetAIConst (2); // AGRESIVE ARMY               
                UnloadUran8 = ActualTurn;
            }
        }
        // At Warehouse 1 
        if (Army8 -> IsAtWP () && Army8 -> WayPoint.x == W1X 
        && Army8 -> WayPoint.y == W1Y && Warehouse1 >= BADLIFE) {
            Army8 -> WayPoint.x = WP5X; Army8 -> WayPoint.y = WP5Y;
            Army8 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
        }       
        // WayPoint 5 - At the back of Bridge
        if (Army8 -> IsAtWP () && Army8 -> WayPoint.x == WP5X 
        && Army8 -> WayPoint.y == WP5Y) { 
            Army8 -> WayPoint.x = B1X; Army8 -> WayPoint.y = B1Y;
            Army8 -> SetAIConst (2); // AGRESIVE ARMY
            Army8 -> Insert (Base1);
            UnloadUran8 = ActualTurn;
        }               
    }

    // ARMY 9 - SPECIAL ACTIVATION
    if (ActualTurn >= ActivationTurn9 && Army9 -> Status == asSleeping) {
        Army9 -> Status = asActive;
        Army9 -> SetAIConst (2); // AGRESIVE ARMY
        UnlockDraw ();
        if (Units [Mohykan1] != NULL 
        && Units [Mohykan1] -> Type - BADLIFE == unMohykan) {
            Units [Mohykan1] -> Select ();
            Units [Mohykan1] -> MoveFar (M1DX, M1DY);
            Army9 -> Insert (Mohykan1);
        }
        if (Units [Mohykan2] != NULL
        && Units [Mohykan2] -> Type - BADLIFE == unMohykan) {
            Units [Mohykan2] -> Select ();
            Units [Mohykan2] -> MoveFar (M2DX, M2DY);
            Army9 -> Insert (Mohykan2);
        }
        if (Units [Olymp9] != NULL
        && Units [Olymp9] -> Type - BADLIFE == unOlymp) {
            Units [Olymp9] -> Select ();
            Units [Olymp9] -> MoveFar (O9DX, O9DY);
            for (i = 0; i < ((TOlymp *)Units [Olymp9]) -> LoadedUnits; i++) {
                id = ((TOlymp *) Units [Olymp9]) -> Inventory [i];
                st = ((TOlymp *) Units [Olymp9]) -> UnloadUnit ((TUnit *)Units [id]);
                if (st > 0) {
                    Army9 -> Insert (id);
                    i--;
                }
            }
            RedrawMap ();
        }
        LockDraw ();
    }   
    if (ActualTurn >= ActivationTurn9 && Units [Olymp9] != NULL) {
        UnlockDraw ();
        for (i = 0; i < ((TOlymp *)Units [Olymp9]) -> LoadedUnits; i++) {
            id = ((TOlymp *) Units [Olymp9]) -> Inventory [i];
            st = ((TOlymp *) Units [Olymp9]) -> UnloadUnit ((TUnit *)Units [id]);
            if (st > 0) {
                Army9 -> Insert (id);
                i--;
            }
            RedrawMap ();   
        }
        LockDraw ();
        
    }

    // ARMY 10 - SPECIAL ACTIVATION 
    if (ActualTurn >= ActivationTurn10 && Army10 -> Status == asSleeping) {
        Army10 -> Place ();     
        Army10 -> Status = asActive;
        Army10 -> WayPoint.x = WP2X; Army10 -> WayPoint.y = WP2Y;
        Army10 -> SetAIConst (3); // Moving Army        
    }
    if (Army10 -> Status == asActive) {
        // WayPoint 2 - in front of Right Outpost
        if (Army10 -> IsAtWP () && Army10 -> WayPoint.x == WP2X 
        && Army10 -> WayPoint.y == WP2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army10 -> WayPoint.x = WP3X; Army10 -> WayPoint.y = WP3Y;
                Army10 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army10 -> WayPoint.x = W2X; Army10 -> WayPoint.y = W2Y;
                Army10 -> SetAIConst (2); // AGRESIVE ARMY                  
            }
        }
        // At the Right Outpost 1
        if (Army10 -> IsAtWP () && Army10 -> WayPoint.x == W2X 
        && Army10 -> WayPoint.y == W2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army10 -> WayPoint.x = WP3X; Army10 -> WayPoint.y = WP3Y;
                Army10 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
        }
        // WayPoint 3 - At the back of Bridge 1
        if (Army10 -> IsAtWP () && Army10 -> WayPoint.x == WP3X 
        && Army10 -> WayPoint.y == WP3Y) { 
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost
                Army10 -> WayPoint.x = WP4X; Army10 -> WayPoint.y = WP4Y;
                Army10 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
            else {
                Army10 -> WayPoint.x = W4X; Army10 -> WayPoint.y = W4Y;
                Army10 -> SetAIConst (2); // AGRESIVE ARMY                  
            }
        }
        // At the Right Outpost 2
        if (Army10 -> IsAtWP () && Army10 -> WayPoint.x == W4X 
        && Army10 -> WayPoint.y == W4Y) {
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost           
                Army10 -> WayPoint.x = WP4X; Army10 -> WayPoint.y = WP4Y;
                Army10 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY
            }
        }
        // WayPoint 4 - At the back of Bridge 2
        if (Army10 -> IsAtWP () && Army10 -> WayPoint.x == WP4X 
        && Army10 -> WayPoint.y == WP4Y) { //WP4
            Army10 -> WayPoint.x = B2X; Army10 -> WayPoint.y = B2Y;
            Army10 -> SetAIConst (2); // AGRESIVE ARMY                  
        }                       
    }
        
    // ARMY 11 - SPECIAL ACTIVATION 
    if (ActualTurn >= ActivationTurn11 && Army11 -> Status == asSleeping) {
        Army11 -> Place ();     
        Army11 -> Status = asActive;
        Army11 -> WayPoint.x = WP2X; Army11 -> WayPoint.y = WP2Y;
        Army11 -> SetAIConst (3); // Moving Army        
    }
    if (Army11 -> Status == asActive) {
        // WayPoint 2 - in front of Right Outpost
        if (Army11 -> IsAtWP () && Army11 -> WayPoint.x == WP2X 
        && Army11 -> WayPoint.y == WP2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army11 -> WayPoint.x = WP3X; Army11 -> WayPoint.y = WP3Y;
                Army11 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
            else {
                Army11 -> WayPoint.x = W2X; Army11 -> WayPoint.y = W2Y;
                Army11 -> SetAIConst (2); // AGRESIVE ARMY                  
                UnloadUran11 = ActualTurn;
            }
        }
        // At the Right Outpost 1
        if (Army11 -> IsAtWP () && Army11 -> WayPoint.x == W2X 
        && Army11 -> WayPoint.y == W2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army11 -> WayPoint.x = WP3X; Army11 -> WayPoint.y = WP3Y;
                Army11 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
        }
        // WayPoint 3 - At the back of Bridge 1
        if (Army11 -> IsAtWP () && Army11 -> WayPoint.x == WP3X 
        && Army11 -> WayPoint.y == WP3Y) { 
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost
                Army11 -> WayPoint.x = WP4X; Army11 -> WayPoint.y = WP4Y;
                Army11 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
            else {
                Army11 -> WayPoint.x = W4X; Army11 -> WayPoint.y = W4Y;
                Army11 -> SetAIConst (2); // AGRESIVE ARMY                  
                UnloadUran11 = ActualTurn;
            }
        }
        // At the Right Outpost 2
        if (Army11 -> IsAtWP () && Army11 -> WayPoint.x == W4X 
        && Army11 -> WayPoint.y == W4Y) {
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost           
                Army11 -> WayPoint.x = WP4X; Army11 -> WayPoint.y = WP4Y;
                Army11 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
        }
        // WayPoint 4 - At the back of Bridge 2
        if (Army11 -> IsAtWP () && Army11 -> WayPoint.x == WP4X 
        && Army11 -> WayPoint.y == WP4Y) { //WP4
            Army11 -> WayPoint.x = B2X; Army11 -> WayPoint.y = B2Y;
            Army11 -> SetAIConst (2); // AGRESIVE ARMY                  
            UnloadUran11 = ActualTurn;
        }                       
    }

    // ARMY 12 - SPECIAL ACTIVATION 
    if (ActualTurn >= ActivationTurn12 && Army12 -> Status == asSleeping) {
        Army12 -> Place ();     
        Army12 -> Status = asActive;
        Army12 -> WayPoint.x = WP2X; Army12 -> WayPoint.y = WP2Y;
        Army12 -> SetAIConst (3); // Moving Army        
    }
    if (Army12 -> Status == asActive) {
        // WayPoint 2 - in front of Right Outpost
        if (Army12 -> IsAtWP () && Army12 -> WayPoint.x == WP2X 
        && Army12 -> WayPoint.y == WP2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army12 -> WayPoint.x = WP3X; Army12 -> WayPoint.y = WP3Y;
                Army12 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
            else {
                Army12 -> WayPoint.x = W2X; Army12 -> WayPoint.y = W2Y;
                Army12 -> SetAIConst (2); // AGRESIVE ARMY                  
                UnloadUran12 = ActualTurn;
            }
        }
        // At the Right Outpost 1
        if (Army12 -> IsAtWP () && Army12 -> WayPoint.x == W2X 
        && Army12 -> WayPoint.y == W2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army12 -> WayPoint.x = WP3X; Army12 -> WayPoint.y = WP3Y;
                Army12 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
        }
        // WayPoint 3 - At the back of Bridge 1
        if (Army12 -> IsAtWP () && Army12 -> WayPoint.x == WP3X 
        && Army12 -> WayPoint.y == WP3Y) { 
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost
                Army12 -> WayPoint.x = WP4X; Army12 -> WayPoint.y = WP4Y;
                Army12 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
            else {
                Army12 -> WayPoint.x = W4X; Army12 -> WayPoint.y = W4Y;
                Army12 -> SetAIConst (2); // AGRESIVE ARMY                  
                UnloadUran12 = ActualTurn;
            }
        }
        // At the Right Outpost 2
        if (Army12 -> IsAtWP () && Army12 -> WayPoint.x == W4X 
        && Army12 -> WayPoint.y == W4Y) {
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost           
                Army12 -> WayPoint.x = WP4X; Army12 -> WayPoint.y = WP4Y;
                Army12 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
        }
        // WayPoint 4 - At the back of Bridge 2
        if (Army12 -> IsAtWP () && Army12 -> WayPoint.x == WP4X 
        && Army12 -> WayPoint.y == WP4Y) { //WP4
            Army12 -> WayPoint.x = B2X; Army12 -> WayPoint.y = B2Y;
            Army12 -> SetAIConst (2); // AGRESIVE ARMY                  
            UnloadUran12 = ActualTurn;
        }                       
    }

    // ARMY 13 - SPECIAL ACTIVATION 
    if (ActualTurn >= ActivationTurn13 && Army13 -> Status == asSleeping) {
        Army13 -> Place ();     
        Army13 -> Status = asActive;
        Army13 -> WayPoint.x = WP2X; Army13 -> WayPoint.y = WP2Y;
        Army13 -> SetAIConst (3); // Moving Army        
    }
    if (Army13 -> Status == asActive) {
        // WayPoint 2 - in front of Right Outpost
        if (Army13 -> IsAtWP () && Army13 -> WayPoint.x == WP2X 
        && Army13 -> WayPoint.y == WP2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army13 -> WayPoint.x = WP3X; Army13 -> WayPoint.y = WP3Y;
                Army13 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
            else {
                Army13 -> WayPoint.x = W2X; Army13 -> WayPoint.y = W2Y;
                Army13 -> SetAIConst (2); // AGRESIVE ARMY                  
                UnloadUran13 = ActualTurn;
            }
        }
        // At the Right Outpost 1
        if (Army13 -> IsAtWP () && Army13 -> WayPoint.x == W2X 
        && Army13 -> WayPoint.y == W2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army13 -> WayPoint.x = WP3X; Army13 -> WayPoint.y = WP3Y;
                Army13 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
        }
        // WayPoint 3 - At the back of Bridge 1
        if (Army13 -> IsAtWP () && Army13 -> WayPoint.x == WP3X 
        && Army13 -> WayPoint.y == WP3Y) { 
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost
                Army13 -> WayPoint.x = WP4X; Army13 -> WayPoint.y = WP4Y;
                Army13 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
            else {
                Army13 -> WayPoint.x = W4X; Army13 -> WayPoint.y = W4Y;
                Army13 -> SetAIConst (2); // AGRESIVE ARMY                  
                UnloadUran13 = ActualTurn;
            }
        }
        // At the Right Outpost 2
        if (Army13 -> IsAtWP () && Army13 -> WayPoint.x == W4X 
        && Army13 -> WayPoint.y == W4Y) {
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost           
                Army13 -> WayPoint.x = WP4X; Army13 -> WayPoint.y = WP4Y;
                Army13 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
        }
        // WayPoint 4 - At the back of Bridge 2
        if (Army13 -> IsAtWP () && Army13 -> WayPoint.x == WP4X 
        && Army13 -> WayPoint.y == WP4Y) { //WP4
            Army13 -> WayPoint.x = B2X; Army13 -> WayPoint.y = B2Y;
            Army13 -> SetAIConst (2); // AGRESIVE ARMY                  
            UnloadUran13 = ActualTurn;
        }                       
    }

    // ARMY 14 - SPECIAL ACTIVATION 
    if (ActualTurn >= ActivationTurn14 && Army14 -> Status == asSleeping) {
        Army14 -> Place ();     
        Army14 -> Status = asActive;
        Army14 -> WayPoint.x = WP2X; Army14 -> WayPoint.y = WP2Y;
        Army14 -> SetAIConst (3); // Moving Army        
    }
    if (Army14 -> Status == asActive) {
        // WayPoint 2 - in front of Right Outpost
        if (Army14 -> IsAtWP () && Army14 -> WayPoint.x == WP2X 
        && Army14 -> WayPoint.y == WP2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army14 -> WayPoint.x = WP3X; Army14 -> WayPoint.y = WP3Y;
                Army14 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
            else {
                Army14 -> WayPoint.x = W2X; Army14 -> WayPoint.y = W2Y;
                Army14 -> SetAIConst (2); // AGRESIVE ARMY                  
                UnloadUran14 = ActualTurn;
            }
        }
        // At the Right Outpost 1
        if (Army14 -> IsAtWP () && Army14 -> WayPoint.x == W2X 
        && Army14 -> WayPoint.y == W2Y) {
            if (Warehouse2 >= BADLIFE || Warehouse3 >= BADLIFE) {
                Army14 -> WayPoint.x = WP3X; Army14 -> WayPoint.y = WP3Y;
                Army14 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
        }
        // WayPoint 3 - At the back of Bridge 1
        if (Army14 -> IsAtWP () && Army14 -> WayPoint.x == WP3X 
        && Army14 -> WayPoint.y == WP3Y) { 
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost
                Army14 -> WayPoint.x = WP4X; Army14 -> WayPoint.y = WP4Y;
                Army14 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
            else {
                Army14 -> WayPoint.x = W4X; Army14 -> WayPoint.y = W4Y;
                Army14 -> SetAIConst (2); // AGRESIVE ARMY                  
                UnloadUran14 = ActualTurn;
            }
        }
        // At the Right Outpost 2
        if (Army14 -> IsAtWP () && Army14 -> WayPoint.x == W4X 
        && Army14 -> WayPoint.y == W4Y) {
            if (Warehouse4 >= BADLIFE && Tower4 >= BADLIFE) { // Right OutPost 2 lost           
                Army14 -> WayPoint.x = WP4X; Army14 -> WayPoint.y = WP4Y;
                Army14 -> SetAIConst (5); // ULTRA HYPER AGRESIVE ARMY                  
            }
        }
        // WayPoint 4 - At the back of Bridge 2
        if (Army14 -> IsAtWP () && Army14 -> WayPoint.x == WP4X 
        && Army14 -> WayPoint.y == WP4Y) { //WP4
            Army14 -> WayPoint.x = B2X; Army14 -> WayPoint.y = B2Y;
            Army14 -> SetAIConst (2); // AGRESIVE ARMY                  
            UnloadUran14 = ActualTurn;
        }                       
    }

// UNLOADOVANI TRANSPORTERU

    UnlockDraw ();

    if (Uran4 != NO_UNIT && Units [Uran4] != NULL 
    && Units [Uran4] -> Type - BADLIFE == unUran && UnloadUran4 <= ActualTurn) {                
        do {
            if (((TUran *) Units [Uran4]) -> LoadedUnits > 0) {
                id = ((TUran *) Units [Uran4]) -> Inventory [0];
                st = ((TUran *) Units [Uran4]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army4 -> Insert (id);
        }
        while (st == TRUE); 
    }
    
    if (Uran5 != NO_UNIT && Units [Uran5] != NULL 
    && Units [Uran5] -> Type - BADLIFE == unUran && UnloadUran5 <= ActualTurn) {                
        do {
            if (((TUran *) Units [Uran5]) -> LoadedUnits > 0) {
                id = ((TUran *) Units [Uran5]) -> Inventory [0];
                st = ((TUran *) Units [Uran5]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army5 -> Insert (id);
        }
        while (st == TRUE); 
    }
    
    if (Uran6 != NO_UNIT && Units [Uran6] != NULL 
    && Units [Uran6] -> Type - BADLIFE == unUran && UnloadUran6 <= ActualTurn) {                
        do {
            if (((TUran *) Units [Uran6]) -> LoadedUnits > 0) {
                id = ((TUran *) Units [Uran6]) -> Inventory [0];
                st = ((TUran *) Units [Uran6]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army6 -> Insert (id);
        }
        while (st == TRUE); 
    }
    
    if (Uran8 != NO_UNIT && Units [Uran8] != NULL 
    && Units [Uran8] -> Type - BADLIFE == unUran && UnloadUran8 <= ActualTurn) {                
        do {
            if (((TUran *) Units [Uran8]) -> LoadedUnits > 0) {
                id = ((TUran *) Units [Uran8]) -> Inventory [0];
                st = ((TUran *) Units [Uran8]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army8 -> Insert (id);
        }
        while (st == TRUE); 
    }

    if (Uran11 != NO_UNIT && Units [Uran11] != NULL 
    && Units [Uran11] -> Type - BADLIFE == unUran && UnloadUran11 <= ActualTurn) {              
        do {
            if (((TUran *) Units [Uran11]) -> LoadedUnits > 0) {
                id = ((TUran *) Units [Uran11]) -> Inventory [0];
                st = ((TUran *) Units [Uran11]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army11 -> Insert (id);
        }
        while (st == TRUE); 
    }

    if (Uran12 != NO_UNIT && Units [Uran12] != NULL 
    && Units [Uran12] -> Type - BADLIFE == unUran && UnloadUran12 <= ActualTurn) {              
        do {
            if (((TUran *) Units [Uran12]) -> LoadedUnits > 0) {
                id = ((TUran *) Units [Uran12]) -> Inventory [0];
                st = ((TUran *) Units [Uran12]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army12 -> Insert (id);
        }
        while (st == TRUE); 
    }

    if (Uran13 != NO_UNIT && Units [Uran13] != NULL 
    && Units [Uran13] -> Type - BADLIFE == unUran && UnloadUran13 <= ActualTurn) {              
        do {
            if (((TUran *) Units [Uran13]) -> LoadedUnits > 0) {
                id = ((TUran *) Units [Uran13]) -> Inventory [0];
                st = ((TUran *) Units [Uran13]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army13 -> Insert (id);
        }
        while (st == TRUE); 
    }

    if (Uran14 != NO_UNIT && Units [Uran14] != NULL 
    && Units [Uran14] -> Type - BADLIFE == unUran && UnloadUran14 <= ActualTurn) {              
        do {
            if (((TUran *) Units [Uran14]) -> LoadedUnits > 0) {
                id = ((TUran *) Units [Uran14]) -> Inventory [0];
                st = ((TUran *) Units [Uran14]) -> UnloadUnit ((TUnit *)Units [id]);
            }
            else st = FALSE;
            if (st > 0) Army14 -> Insert (id);
        }
        while (st == TRUE); 
    }
    
    
    LockDraw ();


// AKCE ARMAD

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
        
        if (ActivationTurn2 == MaxInt) ActivationTurn2 = ActualTurn + MaxDist;
        
        // Pokyn k sebevrazde       
        Army1 -> CountDangerous ();
        if (Army1 -> Dangerous <= CSuicide1) {
            Army1 -> Status = asSuicide;
            if (ActivationTurn2 > ActualTurn) ActivationTurn2 = ActualTurn;
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

        if (ActivationTurn3 == MaxInt) ActivationTurn3 = ActualTurn + MaxDist;

        // Pokyn k sebevrazde       
        Army2 -> CountDangerous ();
        if (Army2 -> Dangerous <= CSuicide2) {
            Army2 -> Status = asSuicide;
            if (ActivationTurn3 > ActualTurn) ActivationTurn3 = ActualTurn;
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

        if (ActivationTurn4 == MaxInt) ActivationTurn4 = ActualTurn + MaxDist;

        // Pokyn k sebevrazde       
        Army3 -> CountDangerous ();
        if (Army3 -> Dangerous <= CSuicide3) {
            Army3 -> Status = asSuicide;
            if (ActivationTurn4 > ActualTurn) ActivationTurn4 = ActualTurn;
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

        if (ActivationTurn5 == MaxInt) ActivationTurn5 = ActualTurn + MaxDist;
        
        // Pokyn k sebevrazde       
        Army4 -> CountDangerous ();
        if (Army4 -> Dangerous <= CSuicide4) {
            Army4 -> Status = asSuicide;
            if (ActivationTurn5 > ActualTurn) ActivationTurn5 = ActualTurn;
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

        if (ActivationTurn6 == MaxInt) ActivationTurn6 = ActualTurn + MaxDist;
        
        // Pokyn k sebevrazde       
        Army5 -> CountDangerous ();
        if (Army5 -> Dangerous <= CSuicide5) {
            Army5 -> Status = asSuicide;
            if (ActivationTurn6 > ActualTurn) ActivationTurn6 = ActualTurn;
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

        if (ActivationTurn7 == MaxInt) ActivationTurn7 = ActualTurn + MaxDist;
        
        // Pokyn k sebevrazde       
        Army6 -> CountDangerous ();
        if (Army6 -> Dangerous <= CSuicide6) {
            Army6 -> Status = asSuicide;
            if (ActivationTurn7 > ActualTurn)  ActivationTurn7 = ActualTurn;
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

        if (ActivationTurn8 == MaxInt) ActivationTurn8 = ActualTurn + MaxDist;
        
        // Pokyn k sebevrazde       
        Army7 -> CountDangerous ();
        if (Army7 -> Dangerous <= CSuicide7) {
            Army7 -> Status = asSuicide;
            if (ActivationTurn8 > ActualTurn) ActivationTurn8 = ActualTurn;
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
        
        if (ActivationTurn10 == MaxInt) ActivationTurn10 = ActualTurn + MaxDist;        
        
        // Pokyn k sebevrazde       
        Army9 -> CountDangerous ();
        if (Army9 -> Dangerous <= CSuicide9) {
            Army9 -> Status = asSuicide;
            if (ActivationTurn10 > ActualTurn) ActivationTurn10 = ActualTurn;
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

        if (ActivationTurn11 == MaxInt) ActivationTurn11 = ActualTurn + MaxDist;        

        // Pokyn k sebevrazde       
        Army10 -> CountDangerous ();
        if (Army10 -> Dangerous <= CSuicide10) {
            Army10 -> Status = asSuicide;
            if (ActivationTurn11 > ActualTurn) ActivationTurn11 = ActualTurn;
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

        if (ActivationTurn12 == MaxInt) ActivationTurn12 = ActualTurn + MaxDist;        
        
        // Pokyn k sebevrazde       
        Army11 -> CountDangerous ();
        if (Army11 -> Dangerous <= CSuicide11) {
            Army11 -> Status = asSuicide;
            if (ActivationTurn12 > ActualTurn) ActivationTurn12 = ActualTurn;
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

        if (ActivationTurn13 == MaxInt) ActivationTurn13 = ActualTurn + MaxDist;        
        
        // Pokyn k sebevrazde       
        Army12 -> CountDangerous ();
        if (Army12 -> Dangerous <= CSuicide12) {
            Army12 -> Status = asSuicide;
            if (ActivationTurn13 > ActualTurn) ActivationTurn13 = ActualTurn;
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

        if (ActivationTurn14 == MaxInt) ActivationTurn14 = ActualTurn + MaxDist;        
        
        // Pokyn k sebevrazde       
        Army13 -> CountDangerous ();
        if (Army13 -> Dangerous <= CSuicide13) {
            Army13 -> Status = asSuicide;
            if (ActivationTurn14 > ActualTurn) ActivationTurn14 = ActualTurn;
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
    
    AttackFieldPos = 0;
    
    UnlockDraw ();  
    AllowBadlife (FALSE);
    return AssignResult ();

    
}




int AssignResult4 ()
{
    int i, t = 0, j = 0;
    double d = 0;
    
    for (i = 0; i < BADLIFE; i++)
        if (Units [i] != NULL && Units [i] -> Type < unRadar) t++;
    if (t == 0) return -1; // GoodLife prohral  
    
    t = 0;
    for (i = BADLIFE; i < UNITS_TOP; i++) {
        if (Units [i] != NULL && Units [i] -> Type - BADLIFE == unMedea) t++;
    }
    if (t == 0) { // Dosli Medey
        for (i = BADLIFE; i < UNITS_TOP; i++) {
            if (Units [i] != NULL) d += TabDanger [Units [i] -> Type % BADLIFE];
        }
        if (d < 12) return 1; // Badlife prohral
    }

    d = 0;
    for (i = BADLIFE; i < UNITS_TOP; i++) {
        if (Units [i] == NULL) continue;
        t = FALSE;
        for (j = 0; j < Army9 -> nofBadLife; j++) {
            if (Units [i] -> ID == Army9 -> BL [j]) {
                t = TRUE;
                break;
            }
        }
        if (!t && Units [i] != NULL) d += TabDanger [Units [i] -> Type % BADLIFE];
    }
    if (d < 10) return 1; // Badlife prohral
    
        
    if (Army1 -> Status == asDestroyed && Army2 -> Status == asDestroyed
    && Army3 -> Status == asDestroyed && Army4 -> Status == asDestroyed
    && Army5 -> Status == asDestroyed && Army6 -> Status == asDestroyed
    && Army7 -> Status == asDestroyed && Army8 -> Status == asDestroyed
    && Army9 -> Status == asDestroyed && Army10 -> Status == asDestroyed
    && Army11 -> Status == asDestroyed && Army12 -> Status == asDestroyed 
    && Army13 -> Status == asDestroyed && Army14 -> Status == asDestroyed)
        return 1; // BadLife prohral    
    
    
    return 0; // Zatim se jeste hraje
}


int *GetManufacturableUnits4 (int FactoryID)
{
    return GFactoryList;
}


void DoneArtificialIntelligence4 ()
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
    if (Towers != NULL) {delete Towers; Towers = NULL; }
}

// OK
