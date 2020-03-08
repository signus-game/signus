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
//       Local Intelligence
//
//        Global Functions
//
// Programmed by Richard Wunsch
//

#include <cassert>

#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "consts.h"
#include "engine.h"
#include "mouse.h"


TUnAnalysePart *UnAnalyse = NULL;


int AttackFieldPos;
TAttackedField AttackedField [1024];

int DeletedUnits [1024];
int DUPos;


void AI_Attack (TUnit *Unit, int x, int y)
{
	if (AttackFieldPos < 1023) {
		AttackedField [AttackFieldPos].x = x;
		AttackedField [AttackFieldPos].y = y;
		AttackedField [AttackFieldPos].Unit = Unit -> ID;
		AttackFieldPos++;
	}
}



void AI_DeleteUnit (int ID)
{
	if (DUPos < 1023) {
		DeletedUnits [DUPos] = ID;
		DUPos++;
	}
	
}

void IntegrityTest () {
	int i, j;	

	for (i = 0; i < BADLIFE; i++) {
		if (Units [i] == NULL) continue;
		
		if (Units [i] -> Type < unRadar) {
			int wcnt = ((TUnit *)Units [i]) -> WeaponsCnt;
			assert(wcnt <= 4);
			
			for (j = wcnt; j < 4; j++) {
				if (((TUnit *)Units [i]) -> Weapons [j] != NULL) {
					AIError ();
				}
			}
			
			for (j = 0; j < wcnt; j++) {
				if (((TUnit *)Units [i]) -> Weapons [j] -> TimeLost < 0) {
					AIError ();
				}
			}
			
			
		}
	}

	
}


void InitArtificialIntelligence (int mission)
{
  
  switch (mission) {
  	case 1 :
  		ArtificialIntelligence = ArtificialIntelligence1;
  		SaveArtificialIntelligence = SaveArtificialIntelligence1;
  		InitAI = InitAI1;
  		DoneArtificialIntelligence = DoneArtificialIntelligence1;
  		AssignResult = AssignResult1;
  		GetManufacturableUnits = GetManufacturableUnits1;
  		break;
  	case 2 :
  		ArtificialIntelligence = ArtificialIntelligence2;
  		SaveArtificialIntelligence = SaveArtificialIntelligence2;
  		InitAI = InitAI2;
  		DoneArtificialIntelligence = DoneArtificialIntelligence2;
  		AssignResult = AssignResult2;
  		GetManufacturableUnits = GetManufacturableUnits2;
  		break;
  	case 3 :
  		ArtificialIntelligence = ArtificialIntelligence3;
  		SaveArtificialIntelligence = SaveArtificialIntelligence3;
  		InitAI = InitAI3;
  		DoneArtificialIntelligence = DoneArtificialIntelligence3;
  		AssignResult = AssignResult3;
  		GetManufacturableUnits = GetManufacturableUnits3;
  		break;
  	case 4 :
  		ArtificialIntelligence = ArtificialIntelligence4;
  		SaveArtificialIntelligence = SaveArtificialIntelligence4;
  		InitAI = InitAI4;
  		DoneArtificialIntelligence = DoneArtificialIntelligence4;
  		AssignResult = AssignResult4;
  		GetManufacturableUnits = GetManufacturableUnits4;
  		break;
  	case 5 :
  		ArtificialIntelligence = ArtificialIntelligence5;
  		SaveArtificialIntelligence = SaveArtificialIntelligence5;
  		InitAI = InitAI5;
  		DoneArtificialIntelligence = DoneArtificialIntelligence5;
  		AssignResult = AssignResult5;
  		GetManufacturableUnits = GetManufacturableUnits5;
  		break;
  	case 6 :
  		ArtificialIntelligence = ArtificialIntelligence6;
  		SaveArtificialIntelligence = SaveArtificialIntelligence6;
  		InitAI = InitAI6;
  		DoneArtificialIntelligence = DoneArtificialIntelligence6;
  		AssignResult = AssignResult6;
  		GetManufacturableUnits = GetManufacturableUnits6;
  		break; 
  	case 7 :
  		ArtificialIntelligence = ArtificialIntelligence7;
  		SaveArtificialIntelligence = SaveArtificialIntelligence7;
  		InitAI = InitAI7;
  		DoneArtificialIntelligence = DoneArtificialIntelligence7;
  		AssignResult = AssignResult7;
  		GetManufacturableUnits = GetManufacturableUnits7;
  		break;
  	case 8 :
  		ArtificialIntelligence = ArtificialIntelligence8;
  		SaveArtificialIntelligence = SaveArtificialIntelligence8;
  		InitAI = InitAI8;
  		DoneArtificialIntelligence = DoneArtificialIntelligence8;
  		AssignResult = AssignResult8;
  		GetManufacturableUnits = GetManufacturableUnits8;
  		break;
  	case 9 :
  		ArtificialIntelligence = ArtificialIntelligence9;
  		SaveArtificialIntelligence = SaveArtificialIntelligence9;
  		InitAI = InitAI9;
  		DoneArtificialIntelligence = DoneArtificialIntelligence9;
  		AssignResult = AssignResult9;
  		GetManufacturableUnits = GetManufacturableUnits9;
  		break;
  	case 10 :
  		ArtificialIntelligence = ArtificialIntelligence10;
  		SaveArtificialIntelligence = SaveArtificialIntelligence10;
  		InitAI = InitAI10;
  		DoneArtificialIntelligence = DoneArtificialIntelligence10;
  		AssignResult = AssignResult10;
  		GetManufacturableUnits = GetManufacturableUnits10;
  		break; 
  	case 11 :
  		ArtificialIntelligence = ArtificialIntelligence11;
  		SaveArtificialIntelligence = SaveArtificialIntelligence11;
  		InitAI = InitAI11;
  		DoneArtificialIntelligence = DoneArtificialIntelligence11;
  		AssignResult = AssignResult11;
  		GetManufacturableUnits = GetManufacturableUnits11;
  		break;
  	case 12 :
  		ArtificialIntelligence = ArtificialIntelligence12;
  		SaveArtificialIntelligence = SaveArtificialIntelligence12;
  		InitAI = InitAI12;
  		DoneArtificialIntelligence = DoneArtificialIntelligence12;
  		AssignResult = AssignResult12;
  		GetManufacturableUnits = GetManufacturableUnits12;
  		break;
  	case 13 :
  		ArtificialIntelligence = ArtificialIntelligence13;
  		SaveArtificialIntelligence = SaveArtificialIntelligence13;
  		InitAI = InitAI13;
  		DoneArtificialIntelligence = DoneArtificialIntelligence13;
  		AssignResult = AssignResult13;
  		GetManufacturableUnits = GetManufacturableUnits13;
  		break;
  	case 14 :
  		ArtificialIntelligence = ArtificialIntelligence14;
  		SaveArtificialIntelligence = SaveArtificialIntelligence14;
  		InitAI = InitAI14;
  		DoneArtificialIntelligence = DoneArtificialIntelligence14;
  		AssignResult = AssignResult14;
  		GetManufacturableUnits = GetManufacturableUnits14;
  		break;
  	case 15 :
  		ArtificialIntelligence = ArtificialIntelligence15;
  		SaveArtificialIntelligence = SaveArtificialIntelligence15;
  		InitAI = InitAI15;
  		DoneArtificialIntelligence = DoneArtificialIntelligence15;
  		AssignResult = AssignResult15;
  		GetManufacturableUnits = GetManufacturableUnits15;
  		break;
  	case 16 :
  		ArtificialIntelligence = ArtificialIntelligence16;
  		SaveArtificialIntelligence = SaveArtificialIntelligence16;
  		InitAI = InitAI16;
  		DoneArtificialIntelligence = DoneArtificialIntelligence16;
  		AssignResult = AssignResult16;
  		GetManufacturableUnits = GetManufacturableUnits16;
  		break;
  	case 17 :
  		ArtificialIntelligence = ArtificialIntelligence17;
  		SaveArtificialIntelligence = SaveArtificialIntelligence17;
  		InitAI = InitAI17;
  		DoneArtificialIntelligence = DoneArtificialIntelligence17;
  		AssignResult = AssignResult17;
  		GetManufacturableUnits = GetManufacturableUnits17;
  		break;
  	case 18 :
  		ArtificialIntelligence = ArtificialIntelligence18;
  		SaveArtificialIntelligence = SaveArtificialIntelligence18;
  		InitAI = InitAI18;
  		DoneArtificialIntelligence = DoneArtificialIntelligence18;
  		AssignResult = AssignResult18;
  		GetManufacturableUnits = GetManufacturableUnits18;
  		break;
  	case 19 :
  		ArtificialIntelligence = ArtificialIntelligence19;
  		SaveArtificialIntelligence = SaveArtificialIntelligence19;
  		InitAI = InitAI19;
  		DoneArtificialIntelligence = DoneArtificialIntelligence19;
  		AssignResult = AssignResult19;
  		GetManufacturableUnits = GetManufacturableUnits19;
  		break;
  		
  }
  
	InitAI ();
  ScrollTo((MapSizeX - VIEW_SIZE_X) / 2, (MapSizeY - VIEW_SIZE_Y) / 2);
  SelectedUnit -> Center ();

	UpdateLitMap(TRUE);
	MouseFreeze(LITMAP_X, LITMAP_Y, LITMAP_SIZE, LITMAP_SIZE);
	DrawLitMap();
	MouseUnfreeze();
}



void LoadArtificialIntelligence(ReadStream &stream, int format)
{
	switch (ActualMission) {
  	case 1 :
  		ArtificialIntelligence = ArtificialIntelligence1;
  		SaveArtificialIntelligence = SaveArtificialIntelligence1;
  		InitAI = InitAI1;
  		DoneArtificialIntelligence = DoneArtificialIntelligence1;
  		AssignResult = AssignResult1;
  		GetManufacturableUnits = NULL;
		LoadArtificialIntelligence1(stream, format);
  		break;
  	case 2 :
  		ArtificialIntelligence = ArtificialIntelligence2;
  		SaveArtificialIntelligence = SaveArtificialIntelligence2;
  		InitAI = InitAI2;
  		DoneArtificialIntelligence = DoneArtificialIntelligence2;
  		AssignResult = AssignResult2;
  		GetManufacturableUnits = NULL;
		LoadArtificialIntelligence2(stream, format);
  		break;
  	case 3 :
  		ArtificialIntelligence = ArtificialIntelligence3;
  		SaveArtificialIntelligence = SaveArtificialIntelligence3;
  		InitAI = InitAI3;
  		DoneArtificialIntelligence = DoneArtificialIntelligence3;
  		AssignResult = AssignResult3;
  		GetManufacturableUnits = GetManufacturableUnits3;
		LoadArtificialIntelligence3(stream, format);
  		break;
   	case 4 :
  		ArtificialIntelligence = ArtificialIntelligence4;
  		SaveArtificialIntelligence = SaveArtificialIntelligence4;
  		InitAI = InitAI4;
  		DoneArtificialIntelligence = DoneArtificialIntelligence4;
  		AssignResult = AssignResult4;
  		GetManufacturableUnits = GetManufacturableUnits4;
		LoadArtificialIntelligence4(stream, format);
  		break;
   	case 5 :
  		ArtificialIntelligence = ArtificialIntelligence5;
  		SaveArtificialIntelligence = SaveArtificialIntelligence5;
  		InitAI = InitAI5;
  		DoneArtificialIntelligence = DoneArtificialIntelligence5;
  		AssignResult = AssignResult5;
  		GetManufacturableUnits = GetManufacturableUnits5;
		LoadArtificialIntelligence5(stream, format);
  		break;
   	case 6 :
  		ArtificialIntelligence = ArtificialIntelligence6;
  		SaveArtificialIntelligence = SaveArtificialIntelligence6;
  		InitAI = InitAI6;
  		DoneArtificialIntelligence = DoneArtificialIntelligence6;
  		AssignResult = AssignResult6;
  		GetManufacturableUnits = GetManufacturableUnits6;
		LoadArtificialIntelligence6(stream, format);
  		break; 
   	case 7 :
  		ArtificialIntelligence = ArtificialIntelligence7;
  		SaveArtificialIntelligence = SaveArtificialIntelligence7;
  		InitAI = InitAI7;
  		DoneArtificialIntelligence = DoneArtificialIntelligence7;
  		AssignResult = AssignResult7;
  		GetManufacturableUnits = GetManufacturableUnits7;
		LoadArtificialIntelligence7(stream, format);
  		break;
   	case 8 :
  		ArtificialIntelligence = ArtificialIntelligence8;
  		SaveArtificialIntelligence = SaveArtificialIntelligence8;
  		InitAI = InitAI8;
  		DoneArtificialIntelligence = DoneArtificialIntelligence8;
  		AssignResult = AssignResult8;
  		GetManufacturableUnits = GetManufacturableUnits8;
		LoadArtificialIntelligence8(stream, format);
  		break;
   	case 9 :
  		ArtificialIntelligence = ArtificialIntelligence9;
  		SaveArtificialIntelligence = SaveArtificialIntelligence9;
  		InitAI = InitAI9;
  		DoneArtificialIntelligence = DoneArtificialIntelligence9;
  		AssignResult = AssignResult9;
  		GetManufacturableUnits = GetManufacturableUnits9;
		LoadArtificialIntelligence9(stream, format);
  		break;
   	case 10 :
  		ArtificialIntelligence = ArtificialIntelligence10;
  		SaveArtificialIntelligence = SaveArtificialIntelligence10;
  		InitAI = InitAI10;
  		DoneArtificialIntelligence = DoneArtificialIntelligence10;
  		AssignResult = AssignResult10;
  		GetManufacturableUnits = GetManufacturableUnits10;
		LoadArtificialIntelligence10(stream, format);
  		break; 
   	case 11 :
  		ArtificialIntelligence = ArtificialIntelligence11;
  		SaveArtificialIntelligence = SaveArtificialIntelligence11;
  		InitAI = InitAI11;
  		DoneArtificialIntelligence = DoneArtificialIntelligence11;
  		AssignResult = AssignResult11;
  		GetManufacturableUnits = GetManufacturableUnits11;
		LoadArtificialIntelligence11(stream, format);
  		break;
   	case 12 :
  		ArtificialIntelligence = ArtificialIntelligence12;
  		SaveArtificialIntelligence = SaveArtificialIntelligence12;
  		InitAI = InitAI12;
  		DoneArtificialIntelligence = DoneArtificialIntelligence12;
  		AssignResult = AssignResult12;
  		GetManufacturableUnits = GetManufacturableUnits12;
		LoadArtificialIntelligence12(stream, format);
  		break;
   	case 13 :
  		ArtificialIntelligence = ArtificialIntelligence13;
  		SaveArtificialIntelligence = SaveArtificialIntelligence13;
  		InitAI = InitAI13;
  		DoneArtificialIntelligence = DoneArtificialIntelligence13;
  		AssignResult = AssignResult13;
  		GetManufacturableUnits = GetManufacturableUnits13;
		LoadArtificialIntelligence13(stream, format);
  		break;
   	case 14 :
  		ArtificialIntelligence = ArtificialIntelligence14;
  		SaveArtificialIntelligence = SaveArtificialIntelligence14;
  		InitAI = InitAI14;
  		DoneArtificialIntelligence = DoneArtificialIntelligence14;
  		AssignResult = AssignResult14;
  		GetManufacturableUnits = GetManufacturableUnits14;
		LoadArtificialIntelligence14(stream, format);
  		break;
   	case 15 :
  		ArtificialIntelligence = ArtificialIntelligence15;
  		SaveArtificialIntelligence = SaveArtificialIntelligence15;
  		InitAI = InitAI15;
  		DoneArtificialIntelligence = DoneArtificialIntelligence15;
  		AssignResult = AssignResult15;
  		GetManufacturableUnits = GetManufacturableUnits15;
		LoadArtificialIntelligence15(stream, format);
  		break;
   	case 16 :
  		ArtificialIntelligence = ArtificialIntelligence16;
  		SaveArtificialIntelligence = SaveArtificialIntelligence16;
  		InitAI = InitAI16;
  		DoneArtificialIntelligence = DoneArtificialIntelligence16;
  		AssignResult = AssignResult16;
  		GetManufacturableUnits = GetManufacturableUnits16;
		LoadArtificialIntelligence16(stream, format);
  		break;
   	case 17 :
  		ArtificialIntelligence = ArtificialIntelligence17;
  		SaveArtificialIntelligence = SaveArtificialIntelligence17;
  		InitAI = InitAI17;
  		DoneArtificialIntelligence = DoneArtificialIntelligence17;
  		AssignResult = AssignResult17;
  		GetManufacturableUnits = GetManufacturableUnits17;
		LoadArtificialIntelligence17(stream, format);
  		break;
   	case 18 :
  		ArtificialIntelligence = ArtificialIntelligence18;
  		SaveArtificialIntelligence = SaveArtificialIntelligence18;
  		InitAI = InitAI18;
  		DoneArtificialIntelligence = DoneArtificialIntelligence18;
  		AssignResult = AssignResult18;
  		GetManufacturableUnits = GetManufacturableUnits18;
		LoadArtificialIntelligence18(stream, format);
  		break;
   	case 19 :
  		ArtificialIntelligence = ArtificialIntelligence19;
  		SaveArtificialIntelligence = SaveArtificialIntelligence19;
  		InitAI = InitAI19;
  		DoneArtificialIntelligence = DoneArtificialIntelligence19;
  		AssignResult = AssignResult19;
  		GetManufacturableUnits = GetManufacturableUnits19;
		LoadArtificialIntelligence19(stream, format);
  		break;
		
	}
	
}


int  (*ArtificialIntelligence) () = NULL;
int  (*AssignResult) () = NULL;
int *(*GetManufacturableUnits) (int FactoryID) = NULL;
void (*InitAI) () = NULL;
void (*SaveArtificialIntelligence) (WriteStream &stream) = NULL;
void (*DoneArtificialIntelligence) () = NULL; 




void AnalyseLandscape ()
{
	int i, r, mr, ar, x, y, xx, yy;
	int mtu, ttm, tta;
	TUnit *Unit;
	int VisFactor;
	
	// Init ProgressBar
	r = 0;
	for (i = 0; i < BADLIFE; i++) if (Units [i] != NULL) r++;	
	ProgressLimit (r);
	ProgressNull ();
	

/*	
	// Vyrazeni jednotek z mapy
	dummap = (word *)memalloc (MapSizeX * MapSizeY * sizeof (word));
	for (x = 0; x < MapSizeX; x++) {
		for (y = 0; y < MapSizeY; y++) {
			dummap [y * MapSizeX + x] = Map [y * MapSizeX + x].Unit;
			Map [y * MapSizeX + x].Unit = NO_UNIT;
		}
	}
	
*/
	
	
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
	}
	
	// Alokace pole tabulek
	UnAnalyse = (TUnAnalysePart *)memalloc (BADLIFE * sizeof (TUnAnalysePart));
	
	for (i = 0; i < BADLIFE; i++) { // pro vsechny Units		
		
		// neexistuje-li jednotka, snuluju pole
		if (Units [i] == NULL) {
			UnAnalyse [i].w0 = NULL;
			UnAnalyse [i].w1 = NULL;
			UnAnalyse [i].w2 = NULL;
			UnAnalyse [i].w3 = NULL;
			continue;
		}
		// neni-li to TUnit, snuluju pole
		if (Units [i] -> Type >= unRadar) {
			UnAnalyse [i].w0 = NULL;
			UnAnalyse [i].w1 = NULL;
			UnAnalyse [i].w2 = NULL;
			UnAnalyse [i].w3 = NULL;
			continue;
		}
		
		ProgressInc ();	
		
		Unit = (TUnit *)Units [i];
		// Je-li mimo mapu
		if (Unit -> X < 0 || Unit -> X >= MapSizeX 
		|| Unit -> Y < 0 || Unit -> Y >= MapSizeY) {
			UnAnalyse [i].w0 = NULL;
			UnAnalyse [i].w1 = NULL;
			UnAnalyse [i].w2 = NULL;
			UnAnalyse [i].w3 = NULL;
			continue;
		}
		
		
		Unit -> Select ();
		if (GetBadlifeVisib (Unit -> X, Unit -> Y) == 2)
			VisFactor = 3;
		else
			VisFactor = 1;
		mtu = Unit -> MaxTimeUnits;
		
		
		// zbran 0
		if (Unit -> Weapons [0] != NULL) { 
		
			tta = Unit -> Weapons [0] -> TimeLost;
			r = (mtu - tta) / 5 + Unit -> Weapons [0] -> MaxRange;			
			// specialitka Thora
			if (Unit -> Type == unThor)
				if (!((TThor *)Unit) -> IsOverground) mtu -= 30; 
			                                            // magic number vysunuti
						
			// alokace & vynulovani
			UnAnalyse [i].w0 = (byte *)memalloc ((2*r + 1) * (2*r + 1));			
			for (x = 0; x < (2*r + 1) * (2*r + 1); x++) UnAnalyse [i].w0 [x] = 0;
			
			// 3 attacky
			if (mtu - 3 * tta >= 0) { 
			
				// move range			
				mr = (mtu - 3 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [0] -> MaxRange;	
				// projdeme cely move range
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++) { 
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;
						// Time To Move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 
						
						
						if (ttm >= 0 && ttm <= mtu - 3 * tta) { // je-li dost casu
							// projdeme cely attack range
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w0 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [0] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w0 [(yy + r) * (2*r + 1) + (xx + r)] = 3 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
										}
									}
								}
							}
						}
					}
				}
			}
			
			// 2 attacky
			if (mtu - 2 * tta >= 0) { 
			
			  // move range
				mr = (mtu - 2 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [0] -> MaxRange;	// attack range
				// projdeme cely move range
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++){
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;						
						// Time to move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 

						if (ttm >= 0 && ttm <= mtu - 2 * tta) { // je-li dost casu
							// projdeme cely attack range
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w0 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [0] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w0 [(yy + r) * (2*r + 1) + (xx + r)] = 2 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}
			
			// 1 attack
			if (mtu - 1 * tta >= 0) { 
			
				// move range
				mr = (mtu - 1 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [0] -> MaxRange;	
				// projdeme cely move range				
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++){
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;						
						// Time To Move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 

						if (ttm >= 0 && ttm <= mtu - 1 * tta) { // je-li dost casu
							// projdeme cely attack range				
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w0 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [0] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w0 [(yy + r) * (2*r + 1) + (xx + r)] = 1 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}			


/*
			for (x = -r; x <= r; x++) {
				for (y = -r; y <= r; y++) {
					xx = x + Unit -> X;
					yy = y + Unit -> Y;
					if (xx < 0 || yy < 0) continue;
					f =  GetField (xx, yy);
					rx = GetRelX(xx), ry = GetRelY(yy);
					drawx = VIEW_OFS_X + 28 * (rx - ry);
					drawy = VIEW_OFS_Y + 14 * (rx + ry - (f -> Height));
					if (drawx < 0 || drawy < 0 || drawx > 560 || drawy > 450) continue;
					a = UnAnalyse [i].w0 [(y + r) * (2*r + 1) + (x + r)];
					sprintf (str, "%i", a);
					if (x == 0 && y == 0) str[0] = 'X';
					PutStr (FullBuf, 560, drawx + 15, drawy, str, NormalFont
					, clrWhite, clrBlack);
					
				}
			}


			EndDraw ();
			getchar();
*/
		} else UnAnalyse [i].w0 = NULL;
		
		// zbran 1
		if (((TUnit *)Units [i]) -> Weapons [1] != NULL) {
		
			tta = Unit -> Weapons [1] -> TimeLost;
			r = (mtu - tta) / 5 + Unit -> Weapons [1] -> MaxRange;			
			// specialitka Thora
			if (Unit -> Type == unThor)
				if (!((TThor *)Unit) -> IsOverground) mtu -= 30; 
			                                            // magic number vysunuti			
			// alokace & vynulovani
			UnAnalyse [i].w1 = (byte *)memalloc ((2*r + 1) * (2*r + 1));			
			for (x = 0; x < (2*r + 1) * (2*r + 1); x++) UnAnalyse [i].w1 [x] = 0;
			
			// 3 attacky
			if (mtu - 3 * tta >= 0) { 
			
				// move range			
				mr = (mtu - 3 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [1] -> MaxRange;	
				// projdeme cely move range
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++) { 
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;					
						// Time To Move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 
						
						
						if (ttm >= 0 && ttm <= mtu - 3 * tta) { // je-li dost casu
							// projdeme cely attack range
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w1 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [1] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w1 [(yy + r) * (2*r + 1) + (xx + r)] = 3 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}
			
			// 2 attacky
			if (mtu - 2 * tta >= 0) { 
			
			  // move range
				mr = (mtu - 2 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [1] -> MaxRange;	// attack range
				// projdeme cely move range
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++){
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;						
						// Time to move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 

						if (ttm >= 0 && ttm <= mtu - 2 * tta) { // je-li dost casu
							// projdeme cely attack range
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w1 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [1] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w1 [(yy + r) * (2*r + 1) + (xx + r)] = 2 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput,"Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}
			
			// 1 attack
			if (mtu - 1 * tta >= 0) { 
			
				// move range
				mr = (mtu - 1 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [1] -> MaxRange;	
				// projdeme cely move range				
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++){
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;						
						// Time To Move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 

						if (ttm >= 0 && ttm <= mtu - 1 * tta) { // je-li dost casu
							// projdeme cely attack range				
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w1 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [1] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w1 [(yy + r) * (2*r + 1) + (xx + r)] = 1 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}			


			
		} else UnAnalyse [i].w1 = NULL;
		
		// zbran 2
		if (((TUnit *)Units [i]) -> Weapons [2] != NULL) {
		
			tta = Unit -> Weapons [2] -> TimeLost;
			r = (mtu - tta) / 5 + Unit -> Weapons [2] -> MaxRange;			
			// specialitka Thora
			if (Unit -> Type == unThor)
				if (!((TThor *)Unit) -> IsOverground) mtu -= 30; 
			                                            // magic number vysunuti
			// alokace & vynulovani
			UnAnalyse [i].w2 = (byte *)memalloc ((2*r + 1) * (2*r + 1));			
			for (x = 0; x < (2*r + 1) * (2*r + 1); x++) UnAnalyse [i].w2 [x] = 0;
			
			// 3 attacky
			if (mtu - 3 * tta >= 0) { 
			
				// move range			
				mr = (mtu - 3 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [2] -> MaxRange;	
				// projdeme cely move range
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++) { 
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;					
						// Time To Move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 
						
						
						if (ttm >= 0 && ttm <= mtu - 3 * tta) { // je-li dost casu
							// projdeme cely attack range
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w2 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [2] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w2 [(yy + r) * (2*r + 1) + (xx + r)] = 3 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}
			
			// 2 attacky
			if (mtu - 2 * tta >= 0) { 
			
			  // move range
				mr = (mtu - 2 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [2] -> MaxRange;	// attack range
				// projdeme cely move range
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++){
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;						
						// Time to move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 

						if (ttm >= 0 && ttm <= mtu - 2 * tta) { // je-li dost casu
							// projdeme cely attack range
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w2 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [2] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w2 [(yy + r) * (2*r + 1) + (xx + r)] = 2 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}
			
			// 1 attack
			if (mtu - 1 * tta >= 0) { 
			
				// move range
				mr = (mtu - 1 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [2] -> MaxRange;	
				// projdeme cely move range				
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++){
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;						
						// Time To Move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 

						if (ttm >= 0 && ttm <= mtu - 1 * tta) { // je-li dost casu
							// projdeme cely attack range				
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w2 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [2] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w2 [(yy + r) * (2*r + 1) + (xx + r)] = 1 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}			

			
		
			
		} else UnAnalyse [i].w2 = NULL;
		
		// zbran 3
		if (((TUnit *)Units [i]) -> Weapons [3] != NULL) {
		
			tta = Unit -> Weapons [3] -> TimeLost;
			r = (mtu - tta) / 5 + Unit -> Weapons [3] -> MaxRange;			
			// specialitka Thora
			if (Unit -> Type == unThor)
				if (!((TThor *)Unit) -> IsOverground) mtu -= 30; 
			                                            // magic number vysunuti
			// alokace & vynulovani
			UnAnalyse [i].w3 = (byte *)memalloc ((2*r + 1) * (2*r + 1));			
			for (x = 0; x < (2*r + 1) * (2*r + 1); x++) UnAnalyse [i].w3 [x] = 0;
			
			// 3 attacky
			if (mtu - 3 * tta >= 0) { 
			
				// move range			
				mr = (mtu - 3 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [3] -> MaxRange;	
				// projdeme cely move range
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++) { 
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;					
						// Time To Move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 
						
						
						if (ttm >= 0 && ttm <= mtu - 3 * tta) { // je-li dost casu
							// projdeme cely attack range
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w3 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [3] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w3 [(yy + r) * (2*r + 1) + (xx + r)] = 3 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}
			
			// 2 attacky
			if (mtu - 2 * tta >= 0) { 
			
			  // move range
				mr = (mtu - 2 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [3] -> MaxRange;	// attack range
				// projdeme cely move range
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++){
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;						
						// Time to move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 

						if (ttm >= 0 && ttm <= mtu - 2 * tta) { // je-li dost casu
							// projdeme cely attack range
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w3 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [3] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w3 [(yy + r) * (2*r + 1) + (xx + r)] = 2 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}
			
			// 1 attack
			if (mtu - 1 * tta >= 0) { 
			
				// move range
				mr = (mtu - 1 * tta) / 5; 
				// attack range
				ar = Unit -> Weapons [3] -> MaxRange;	
				// projdeme cely move range				
				for (x = -mr; x <= mr; x++) {
					if (Unit -> X + x < 0 || Unit -> X + x >= MapSizeX) continue;					
					UnlockDraw (FALSE);
					ProcessMapAnim ();
					LockDraw ();
					
					for (y = -mr; y <= mr; y++){
						if (Unit -> Y + y < 0 || Unit -> Y + y >= MapSizeY) continue;						
						// Time To Move
						ttm = Unit -> TimeToMove (Unit -> X + x, Unit -> Y + y); 

						if (ttm >= 0 && ttm <= mtu - 1 * tta) { // je-li dost casu
							// projdeme cely attack range				
							for (xx = x - ar; xx <= x + ar; xx++) {
								for (yy = y - ar; yy <= y + ar; yy++) {
									
									if (UnAnalyse [i].w3 [(yy + r) * (2*r + 1) + (xx + r)] 
									== 0) { // je-li polozka zatim prazdna
										
										if (Unit -> Weapons [3] -> IsInRangeAI (Unit, x + Unit -> X
										, y + Unit -> Y, xx + Unit -> X, yy + Unit -> Y, TRUE)) {
											
											UnAnalyse [i].w3 [(yy + r) * (2*r + 1) + (xx + r)] = 1 * VisFactor;
											if ((yy + r) * (2*r + 1) + (xx + r) < 0 || (yy + r) * (2*r + 1) + (xx + r) >= (2*r+1)*(2*r+1)) {
#ifdef DEBUG												
												fprintf (dbgOutput, "Error: Analyse: Unit: %i, xx: %i, yy: %i, r: %i \n",Unit->ID,xx,yy,r);
#endif												
												AIError ();
											}
											
										}
									}
								}
							}
						}
					}
				}
			}			


			
		} else UnAnalyse [i].w3 = NULL;
		
	
	}


/*	
	// Vraceni jednotek do mapy
	for (x = 0; x < MapSizeX; x++) {
		for (y = 0; y < MapSizeY; y++) {
			if (dummap [y * MapSizeX + x] != NO_UNIT) {
				Map [y * MapSizeX + x].Unit = dummap [y * MapSizeX + x];
			}
		}
	}
	
	memfree (dummap);
	
	StartDraw (0,0,VIEW_SX, VIEW_SY);
	PaintUnits ();
	

	EndDraw ();
*/

	ProgressNull ();	
	Message ("");
}

void loadAttackedFields(ReadStream &stream, int format) {
	int i;

	AttackFieldPos = stream.readSint32LE();

	for (i = 0; i < AttackFieldPos + 1; i++) {
		AttackedField[i].x = stream.readUint8();
		AttackedField[i].y = stream.readUint8();

		if (format < 1) {
			stream.readUint16LE();	// struct padding
		}

		AttackedField[i].Unit = stream.readSint32LE();
	}
}

void saveAttackedFields(WriteStream &stream) {
	int i;

	stream.writeSint32LE(AttackFieldPos);

	for (i = 0; i < AttackFieldPos + 1; i++) {
		stream.writeUint8(AttackedField[i].x);
		stream.writeUint8(AttackedField[i].y);
		stream.writeSint32LE(AttackedField[i].Unit);
	}
}
