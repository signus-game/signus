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


/*


UAIRCRAF : osetreni letadel (aircrafts...)


*/


#include "headers.h"
#include <SDL_timer.h>


#include "uaircraf.h"
#include "explode.h"
#include "untables.h"
#include "transbox.h"
#include "fields.h"
#include "mouse.h"



///////////////////// obhospodareni basic mechanismu //////////////////////


TAircraft *Aircrafts[256] = {NULL};
int AircraftsCnt = 0;



TAircraft *GetAircraftAt(int x, int y)
{
	for (int i = 0; i < AircraftsCnt; i++)	
		if ((Aircrafts[i]->X == x) && (Aircrafts[i]->Y == y))
			return Aircrafts[i];
	return NULL;
}










//////////////// P5edek - TAircraft //////////////////////////////////


void TAircraft::Init(int x, int y, int party, FILE *f)
{
	TUnit::Init(x, y, party, f);
	LittleAlt = 0;
}



void TAircraft::GetTerrMove(int **terr, int **terr2)
{
	if (FlyLevel == 1) {*terr = TabTerrMoveAir; *terr2 = TabTerr2MoveAmphi;}
	else {*terr = TabTerrMoveAir; *terr2 = TabTerr2MoveAir;}
}



void TAircraft::TurnReset()
{
	if ((X != -1) && (FlyLevel != 0)) {
		Fuel -= TimeUnits / 5; // letadlu dochazi palivo i kdyz se nehybe
		if (Fuel < 0) Fuel = HitPoints = 0;
	}
	TUnit::TurnReset();
}



int TAircraft::DoVisib()
{
	if (FlyLevel == 2) return ProcessRadarVisib(X, Y, Visib, ID);
	else return ProcessEyeVisib(X, Y, Visib, ID);
}




static int qs_air(const void *a1, const void *a2)
{
	return ((*(TAircraft**)a1)->X + (*(TAircraft**)a1)->Y) - 
	       ((*(TAircraft**)a2)->X + (*(TAircraft**)a2)->Y);
}

void TAircraft::PlaceGround(int place)
{
	if ((X == -1) && (Y == -1)) return; // v baraku nebo transporteru

	if (FlyLevel == 0 /* on land */) {
		TField *f = GetField(X, Y);
		if (place) {
			f->Unit = ID;
			Alt = f->Height;
			if (IsAnimated()) f->IsAnim = 1; else f->IsAnim = 0;
		}
		else {
			f->Unit = NO_UNIT;
			f->IsAnim = 0;
		}
		GenerateUnitsList();
	}

	else { 
		if (place) {
			
			{ // debug - odstraneni duplicit (stejne by byt nemely, ale jsou):
			for (int i = 0; i < AircraftsCnt; i++)
				if (Aircrafts[i] == this) {
#ifdef DEBUG
					PromtBox("AIRCRAFT DUPLICITY: removed...", cmOk);
#endif
					for (int j = i; j < AircraftsCnt-1; j++) Aircrafts[j] = Aircrafts[j+1];
					AircraftsCnt--;
				}
			}
			
			Aircrafts[AircraftsCnt++] = this;
			if (FlyLevel == 1) Alt = GetField(X, Y)->Height + 1;
			else Alt = 8;
			qsort(Aircrafts, AircraftsCnt, sizeof(TAircraft*), qs_air);
		}
		else {
			for (int i = 0; i < AircraftsCnt; i++)
				if (Aircrafts[i] == this) {
					for (int j = i; j < AircraftsCnt-1; j++) Aircrafts[j] = Aircrafts[j+1];
					AircraftsCnt--;
					break;
				}
		}
	}		
}



TSprite *TAircraft::GetShadowSprite()
{
	return UnitsSprites[Type][8 + Orient];
}



void TAircraft::GetDrawRect(TRect *r)
{
	TRect r1;
	TField *f = GetField(X, Y);
	TSprite *s = GetSprite();
	int rrx = GetRelX(X), rry = GetRelY(Y);
	int drawx = 28 * (rrx - rry) + LittleX + 28;
	int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
	if (FlyLevel == 1) drawy -= 14;
	else if (FlyLevel == 2) drawy = 14 * (rrx + rry - 8) + LittleY + 14;
	
	r->x1 = drawx - s->dx, r->y1 = drawy - s->dy;
	r->x2 = r->x1 + s->w, r->y2 = r->y1 + s->h;

	drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
	s = GetShadowSprite();
	r1.x1 = drawx - s->dx,	r1.y1 = drawy - s->dy;
	r1.x2 = r1.x1 + s->w, r1.y2 = r1.y1 + s->h;
	Union(r, &r1);
}



void TAircraft::Draw()
{
	TField *f = GetField(X, Y);
	if (f->Visib != 2) return;
	
	TSprite *s, *sst;
	int rrx = GetRelX(X), rry = GetRelY(Y);
	int drawx = 28 * (rrx - rry) + LittleX + 28;
	int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
	
	if ((f->OnScreen) && (!SelectCrossLock) && (SelectedUnit == this)) {
		void *oldb = MapBuf;
		MapBuf = FullBuf;
		DrawL2Selector(drawx-LittleX, drawy+28-LittleY, f->Terrain, BmpSelected);
		MapBuf = oldb;
  }

	s = GetShadowSprite();
	DrawSprite(drawx, drawy, s);

	if (FlyLevel == 1) drawy -= 14;
	else if (FlyLevel == 2) drawy = 14 * (rrx + rry - 8) + LittleY + 14;
	drawy -= LittleAlt;
	s = GetSprite();
	DrawSprite(drawx, drawy, s);

	if (iniShowStatusbar && (!StatusbarLock)) {
		if ((sst = GetStatusBar()) != NULL) {
			DrawSprite(drawx, drawy, sst);
			memfree(sst);
		}
	}
	GetDrawRect(&(LD_rect[ID])); LD_used[ID] = TRUE;
}



int TAircraft::PaintUnitInMove(int bx, int by)
{
	ActualSprite = Orient;
	return TUnit::PaintUnitInMove(bx, by);
}



void TAircraft::ShowShootAt(int x, int y, int phase)
{
	if (phase == 0) WpnRotate(x, y);
}



void TAircraft::Read(FILE *f)
{
	TUnit::Read(f);
	fread(&FlyLevel, 4, 1, f);
}

void TAircraft::Write(FILE *f)
{
	TUnit::Write(f);
	fwrite(&FlyLevel, 4, 1, f);
}



unsigned TAircraft::GetSupportedActions()
{
	unsigned sa = uatMoveAir | uatSelect | uatSelectAir;
	if (CurWpn != -1) sa |= uatAttack | uatAttackGood | uatAttackField |
	                        uatAttackAir | uatAttackAirG;
	return sa;
}



unsigned TAircraft::GetAvailableActions()
{
	if (ID >= BADLIFE) {
		if (GetField(X, Y)->Visib == 2) return uatAttackAir;
		else return uatNothing;
	}
	else return uatSelectAir | uatAttackAirG | uatSupportAir;
}



unsigned TAircraft::GetFieldActions(int x, int y)
{
	unsigned aa = 0;
	TField *f = GetField(x, y);
	TAircraft *a = GetAircraftAt(x, y);
	
	if ((x == X) && (y == Y)) {
		if ((f->Unit >= BADLIFE /*vc. NO_UNIT*/)) return uatNothing;
		else return uatSelect;
	}
	
	aa = uatMoveAir | uatAttackField;
	if (f->Unit != NO_UNIT) {
		aa = Units[f->Unit]->GetAvailableActions();
		if (GetSupportedActions() & uatAttack) aa |= uatAttack;
	}
	if (f->Visib != 2) aa = uatMoveAir;
	else if (a != NULL) aa = a->GetAvailableActions();
	return aa & GetSupportedActions();
}




void TAircraft::GetCursor(int x, int y, int *cursor, int *selbold)
{
	TUnit::GetCursor(x, y, cursor, selbold);

	switch (UnitActionType) {
		case uatMoveAir : 
				*cursor = mcurArrow, *selbold = TimeToMove(x, y) >= 0;
				AirCursorOn = FlyLevel;
				break;
		case uatAttackAir :
				*cursor = mcurTarget, *selbold = Weapons[CurWpn]->IsInRange(this, X, Y, x, y);
				AirCursorOn = GetAircraftAt(x, y)->FlyLevel;
				break;
		case uatLandHelip : 
		case uatLandAircr : 
				*cursor = mcurLoadIn, *selbold = TRUE;
				AirCursorOn = 0;
				break;
	}
}



void TAircraft::Action(int x, int y)
{
	int rt;
	
	switch (UnitActionType) {
		case uatMoveAir : 
				int ttm;
				if ((ttm = TimeToMove(x, y)) >= 0) Move(x, y);
				else {
					if (ID < BADLIFE) {
						if (ttm == -1) Message(MSG_CANNOT_GO);
						else if (ttm == -2) Message(MSG_NO_ACCESS);
					}
				}
				break;
		case uatLandHelip : 
		case uatLandAircr : 
				{
				TObject *u = Units[GetField(x, y)->Unit];
				if (u->Type == unLaguna)
					rt = ((TLaguna*)u)->LoadUnit(this);
				else 
					rt = ((TAirport*)u)->LoadUnit(this);
				switch (rt) {
					case -1 : Message(MSG_OBJECT_IN_WAY); break;
					case -2 : Message(MSG_NO_FREE_ROOM); break;
					default : break;
				}
				break;
				}
		default :
				TUnit::Action(x, y);
				break;			
	}
}



void TAircraft::Explode()
{
	int drw = Units[ID]->IsPartOnScreen() && (GetField(X, Y)->Visib == 2);
	
	if (drw) {
		AddExplode1x1(X, Y, 0, 0, -14 * (Alt-1));
		{
			for (int i = 0; i < 6; i++) {
				AddExplode1x1(X, Y, 0, 
						40 - 80 * rand() / RAND_MAX, 20 - 40 * rand() / RAND_MAX -14 * (Alt-1));
				AddExplode3x3(X, Y, 0, 
						60 - 120 * rand() / RAND_MAX, 30 - 60 * rand() / RAND_MAX -14 * (Alt-1));
			}
		}
	}
	else this->RemoveFromWorld();
}



int TAircraft::CanGoOnField(int x, int y)
{
	return (GetAircraftAt(x, y) == NULL);
}
















////////////////////////////// LETOUNY ////////////////////////////////////


// Mystik:

void TMystik::Setup()
{
	TAircraft::Setup();
	FlyLevel = 2;
	MaxFuel = utMY_FUEL;
	MaxHitPoints = utMY_HP;
	MaxTimeUnits = utMY_TU;
	Defense[0] = utMY_DN0;
	Defense[1] = utMY_DN1;
	Defense[2] = utMY_DN2;
	Visib = utMY_VIS;
	CurWpn = 0;
	WeaponsCnt = 3;
	Weapons[0] = new TCannonAntiAir(WpnNames[WPNNM_CANNON], 
	                 utMY_AN, utMY_ANB, utMY_AT, utMY_MAXR, utMY_AMMO);
	Weapons[1] = new TRocketAntiAir(WpnNames[WPNNM_AAROCKETS], 
	                 utMY_2AN, utMY_2ANB, utMY_2AT, 
	                 utMY_2MINR, utMY_2MAXR, utMY_2AMMO);
	Weapons[2] = new TRocketDir(WpnNames[WPNNM_AGROCKETS],
	                 utMY_3AN, utMY_3ANB, utMY_3AN2, utMY_3ANB2, utMY_3AT, 
	                 utMY_3MINR, utMY_3MAXR, utMY_3AMMO);
	Velocity = utMY_VEL;
}



void TMystik::IncLevel(int alevel)
{
	MaxHitPoints = utMY_HP + alevel * utMY_INC_HP;
	Defense[0] = utMY_DN0 + alevel * utMY_INC_DN;
	Defense[1] = utMY_DN1 + alevel * utMY_INC_DN;
	Defense[2] = utMY_DN2 + alevel * utMY_INC_DN;
	Weapons[0]->IncAN(utMY_AN + alevel * utMY_INC_AN - Weapons[0]->AttackNum);
	Weapons[1]->IncAN(utMY_2AN + alevel * utMY_INC_2AN - Weapons[0]->AttackNum);
	Weapons[2]->IncAN(utMY_3AN + alevel * utMY_INC_3AN - Weapons[0]->AttackNum);
}



unsigned TMystik::GetSupportedActions()
{
	unsigned sa = uatMoveAir | uatSelect | uatSelectAir | uatLandAircr | uatLandHelip;
	switch (CurWpn) {
		case 0 : case 1 : sa |= uatAttackAir | uatAttackAirG; break;
		case 2 : sa |= uatAttack | uatAttackGood | uatAttackField; break;
		
	}
	return sa;
}









// Rex:

void TRex::Init(int x, int y, int party, FILE *f)
{
	TAircraft::Init(x, y, party, f);
	if (IconLand == NULL) {
		IconLand = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "icland%i", 13);
		IconTakeoff = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "ictakof%i", 13);
	}
}



void TRex::Setup()
{
	TAircraft::Setup();
	FlyLevel = 2;
	MaxFuel = utRX_FUEL;
	MaxHitPoints = utRX_HP;
	MaxTimeUnits = utRX_TU;
	Defense[0] = utRX_DN0;
	Defense[1] = utRX_DN1;
	Defense[2] = utRX_DN2;
	Visib = utRX_VIS;
	CurWpn = 0;
	WeaponsCnt = 2;
	Weapons[0] = new TRocketDir(WpnNames[WPNNM_AGROCKETS],
	                 utRX_2AN, utRX_2ANB, utRX_2AN2, utRX_2ANB2, utRX_2AT,
	                 utRX_2MINR, utRX_2MAXR, utRX_2AMMO);
	Weapons[1] = new TCannonAntiAir(WpnNames[WPNNM_CANNON], 
	                 utRX_AN, utRX_ANB, utRX_AT, utRX_MAXR, utRX_AMMO);
	Velocity = utRX_VEL;
	APhase = 0;
}

void TRex::IncLevel(int alevel)
{
	MaxHitPoints = utRX_HP + alevel * utRX_INC_HP;
	Defense[0] = utRX_DN0 + alevel * utRX_INC_DN;
	Defense[1] = utRX_DN1 + alevel * utRX_INC_DN;
	Defense[2] = utRX_DN2 + alevel * utRX_INC_DN;
	Weapons[1]->IncAN(utRX_AN + alevel * utRX_INC_AN - Weapons[1]->AttackNum);
	Weapons[0]->IncAN(utRX_2AN + alevel * utRX_INC_2AN - Weapons[0]->AttackNum);
}



TSprite *TRex::GetSprite()
{
	if (APhase)	return UnitsSprites[Type][Orient];
	else return UnitsSprites[Type][8 + Orient];
}

TSprite *TRex::GetShadowSprite()
{
	if (APhase)	return UnitsSprites[Type][16 + Orient];
	else return UnitsSprites[Type][16 + 8 + Orient];
}



void TRex::DoAnim()
{
	if (X == -1) return;
	APhase = !APhase;
	ActualSprite = Orient + APhase * 8;
	PaintUnit(FALSE);
}



unsigned TRex::GetSupportedActions()
{
	unsigned sa;
	
	if (FlyLevel == 0/*on land*/) {
		sa = uatSelect | uatSelectAir;
	}
	else {
		sa = uatMoveAir | uatSelect | uatSelectAir | uatLandHelip;
		switch (CurWpn) {
			case 1 : sa |= uatAttackAir | uatAttackAirG; break;
			case 0 : sa |= uatAttack | uatAttackGood | uatAttackField; break;	
		}
	}
	return sa;
}



unsigned TRex::GetAvailableActions()
{
	if (FlyLevel == 0 /*on land*/) {
		if (ID >= BADLIFE) {
			if (GetField(X, Y)->Visib == 2) return uatAttack;
			else return uatNothing;
		}
		else return uatSelect | uatAttackGood | uatSupport;
	}
	else {
		if (ID >= BADLIFE) {
			if (GetField(X, Y)->Visib == 2) return uatAttackAir;
			else return uatNothing;
		}
		else return uatSelectAir | uatAttackAirG;
	}
}



int TRex::CanLand(int x, int y)
{
	if ((L1TerrainType[GetField(x, y)->Terrain] != tofsL1A) ||
	    (TabHghtModif[GetField(x, y)->Terrain2] != 0) ||
	    (TabTerrMoveGround[GetField(x, y)->Terrain] == 0xFF) ||
	    (GetField(x, y)->Unit != NO_UNIT)) return FALSE;
	else return TRUE;
}



int TRex::Land()
{
	if (!CanLand(X, Y)) return FALSE;
	if (TimeUnits < utRX_LAND) return FALSE;
	HideHelpers();
	if (GetField(X, Y)->Visib == 2) {
		int dif = Alt - GetField(X, Y)->Height-1;
		PlaySample(GetUnitSound(7), 6, EffectsVolume, GetFieldPanning(X, Y));
		for (int i = 0; i < dif * 4; i++) {
			LittleAlt = -i * 4;
			ProcessMapAnim();
			PaintUnit(FALSE);
			SDL_Delay(iniAnimDelay);
		}
	}		
	
	PlaceGround(FALSE);
	FlyLevel = 0;
	LittleAlt = 0;
	PlaceGround(TRUE);
	TimeUnits = 0;
	if (this == SelectedUnit) Select();
	ShowHelpers();
	if (GetMineAt(X, Y) != -1) {MineExplode(X, Y); DoExplosion();}
	return TRUE;
}



int TRex::TakeOff()
{
	if (TimeUnits < utRX_LAND) return FALSE;
	if (GetAircraftAt(X, Y) != NULL) return FALSE;

	HideHelpers();
	PlaceGround(FALSE);
	FlyLevel = 2;
	TimeUnits -= utRX_LAND;
	PlaceGround(TRUE);
	if (GetField(X, Y)->Visib == 2) {
		int dif = Alt - GetField(X, Y)->Height-1;
		PlaySample(GetUnitSound(6), 6, EffectsVolume, GetFieldPanning(X, Y));
		for (int i = dif*4-1; i >= 0; i--) {
			LittleAlt = -i * 4;
			ProcessMapAnim();
			PaintUnit(FALSE);
			SDL_Delay(iniAnimDelay);
		}
		LittleAlt = 0;
	}

	if (this == SelectedUnit) Select();
	ShowHelpers();
	return TRUE;
}



void TRex::GetUnitInfo()
{
	TAircraft::GetUnitInfo();
	if (FlyLevel == 0)
		CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconTakeoff->IconPic[0], 59, 59);
	else
		CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconLand->IconPic[0], 59, 59);
}



int TRex::InfoEvent(TEvent *e)
{
	int rt = TAircraft::InfoEvent(e);
	
	if (FlyLevel == 0) {
		if (!rt & IconTakeoff->Handle(e)) {
			if (!TakeOff()) Message(MSG_OUT_OF_TIME);
			return TRUE;
		}
	}
	else {
		if (!rt & IconLand->Handle(e)) {
			if (!Land()) {
				if (TimeUnits < utRX_LAND) Message(MSG_OUT_OF_TIME);
				else Message(MSG_CANNOT_LAND);
			}				
			return TRUE;
		}
	}
	return rt;
}









// Caesar:

void TCaesar::Init(int x, int y, int party, FILE *f)
{
	TAircraft::Init(x, y, party, f);
	if (IconLand == NULL) {
		IconLand = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "icland%i", 13);
		IconTakeoff = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "ictakof%i", 13);
	}
	if (IconTransport == NULL) 
		IconTransport = new TIcon(RES_X-115, UINFO_Y+110, 102, 23, "tranbut%i", 13);
	if (BmpSmallInventory == NULL)
		BmpSmallInventory = GraphicsDF->get("tranbox1");
}



void TCaesar::Setup()
{
	TAircraft::Setup();
	FlyLevel = 2;
	MaxFuel = utCA_FUEL;
	MaxHitPoints = utCA_HP;
	MaxTimeUnits = utCA_TU;
	Defense[0] = utCA_DN0;
	Defense[1] = utCA_DN1;
	Defense[2] = utCA_DN2;
	Visib = utCA_VIS;
	CurWpn = -1;
	WeaponsCnt = 0;
	Velocity = utCA_VEL;
	LoadedUnits = 0;
	for (int i = 0; i < 6; i++) Inventory[i] = NULL;
}

void TCaesar::IncLevel(int alevel)
{
}



unsigned TCaesar::GetSupportedActions()
{
	if (FlyLevel == 0/*on land*/) return uatSelect | uatSelectAir;
	else return uatMoveAir | uatSelect | uatSelectAir | uatLandHelip;
}



unsigned TCaesar::GetAvailableActions()
{
	if (FlyLevel == 0 /*on land*/) {
		if (ID >= BADLIFE) {
			if (GetField(X, Y)->Visib == 2) return uatAttack;
			else return uatNothing;
		}
		else return uatSelect | uatAttackGood | uatSupport | uatLoadIn;
	}
	else {
		if (ID >= BADLIFE) {
			if (GetField(X, Y)->Visib == 2) return uatAttackAir;
			else return uatNothing;
		}
		else return uatSelectAir | uatAttackAirG | uatSupportAir;
	}
}



int TCaesar::CanLand(int x, int y)
{
	if ((L1TerrainType[GetField(x, y)->Terrain] != tofsL1A) ||
	    (TabHghtModif[GetField(x, y)->Terrain2] != 0) ||
	    (TabTerrMoveGround[GetField(x, y)->Terrain] == 0xFF) ||
	    (GetField(x, y)->Unit != NO_UNIT)) return FALSE;
	else return TRUE;
}



int TCaesar::Land()
{
	if (!CanLand(X, Y)) return FALSE;
	if (TimeUnits < utRX_LAND) return FALSE;
	HideHelpers();
	if (GetField(X, Y)->Visib == 2) {
		int dif = Alt - GetField(X, Y)->Height-1;
		PlaySample(GetUnitSound(7), 6, EffectsVolume, GetFieldPanning(X, Y));
		for (int i = 0; i < dif * 4; i++) {
			LittleAlt = -i * 4;
			ProcessMapAnim();
			PaintUnit(FALSE);
			SDL_Delay(iniAnimDelay);
		}
	}		
	
	PlaceGround(FALSE);
	FlyLevel = 0;
	LittleAlt = 0;
	PlaceGround(TRUE);
	TimeUnits = 0;
	if (this == SelectedUnit) Select();
	ShowHelpers();
	if (GetMineAt(X, Y) != -1) {MineExplode(X, Y); DoExplosion();}
	return TRUE;
}



int TCaesar::TakeOff()
{
	if (TimeUnits < utRX_LAND) return FALSE;
	if (GetAircraftAt(X, Y) != NULL) return FALSE;

	HideHelpers();
	PlaceGround(FALSE);
	FlyLevel = 2;
	TimeUnits -= utRX_LAND;
	PlaceGround(TRUE);
	if (GetField(X, Y)->Visib == 2) {
		int dif = Alt - GetField(X, Y)->Height-1;
		PlaySample(GetUnitSound(6), 6, EffectsVolume, GetFieldPanning(X, Y));
		for (int i = dif*4-1; i >= 0; i--) {
			LittleAlt = -i * 4;
			ProcessMapAnim();
			PaintUnit(FALSE);
			SDL_Delay(iniAnimDelay);
		}
		LittleAlt = 0;
	}

	if (this == SelectedUnit) Select();
	ShowHelpers();
	return TRUE;
}



void TCaesar::GetUnitInfo()
{
	TAircraft::GetUnitInfo();
	if (FlyLevel == 0) {
		CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconTakeoff->IconPic[0], 59, 59);
		CopyBmp(UInfoBuf, UINFO_SX, 3, 110, IconTransport->IconPic[0], 102, 23);
	}
	else
		CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconLand->IconPic[0], 59, 59);
	PercentBar(UInfoBuf, UINFO_SX, 3, 135, 102, 8, clrLightBlue2, clrSeaBlue,
	           ((double)GetTotalWeight() / Capacity), "");
}



int TCaesar::InfoEvent(TEvent *e)
{
	int rt = TAircraft::InfoEvent(e);
	
	if (FlyLevel == 0) {
		if (!rt & IconTakeoff->Handle(e)) {
			if (!TakeOff()) Message(MSG_OUT_OF_TIME);
			return TRUE;
		}
		else if (!rt & IconTransport->Handle(e)) {
			DoInventory();
			return TRUE;
		}
	}
	else {
		if (!rt & IconLand->Handle(e)) {
			if (!Land()) Message(MSG_CANNOT_LAND);
			return TRUE;
		}
	}
	return rt;
}



void TCaesar::Action(int x, int y)
{
	if ((x == X) && (y == Y)) {
		DoInventory();
		return;
	}
	
	TAircraft::Action(x, y);
}



void TCaesar::Read(FILE *f)
{
	int id;
	
	TAircraft::Read(f);
	fread(&LoadedUnits, 4, 1, f);
	for (int i = 0; i < LoadedUnits; i++) {
		id = 0; fread(&id, 4, 1, f);
		Inventory[i] = id;
	}
}

void TCaesar::Write(FILE *f)
{
	int id;
	
	TAircraft::Write(f);
	fwrite(&LoadedUnits, 4, 1, f);
	for (int i = 0; i < LoadedUnits; i++)
		fwrite(&(Inventory[i]), 4, 1, f);
}



void TCaesar::ChangeParty()
{
	TObject *dum;

	TAircraft::ChangeParty();
	for (int i = 0; i < LoadedUnits; i++) {
		dum = Units[Inventory[i]];
		dum->ChangeParty();
		Inventory[i] = dum->ID;
	}
}



void TCaesar::RemoveFromWorld()
{
	for (int i = 0; i < LoadedUnits; i++) 
		Units[Inventory[i]]->RemoveFromWorld();
	TAircraft::RemoveFromWorld();
}



int TCaesar::GetTotalWeight()
{
	int i, total = 0;
	
	for (i = 0; i < LoadedUnits; i++) total += Units[Inventory[i]]->GetWeight();
	return total;
}



int TCaesar::GetLoadingPoint(int *x, int *y, TUnit *u)
{
	int t, i, j, best = 0xFFFF;
	TField *f;
	
	if (u->X != -1) {
		if ((u->ID < BADLIFE) && (this->ID >= BADLIFE)) return FALSE;
		if ((u->ID >= BADLIFE) && (this->ID < BADLIFE)) return FALSE;
	
		for (i = -1; i <= 1; i++)
			for (j = -1; j <= 1; j++) {
				t = u->TimeToMove(X + i, Y + j);
				if ((t >= 0) && (t < best) && (u->CanGoOnField(X+i, Y+j))) {
					best = t;
					*x = X + i, *y = Y + j;					
				}
			}
	}
	else {
		for (i = -1; i <= 1; i++)
			for (j = -1; j <= 1; j++) {
				f = GetField(X + i, Y + j);
				if (u->CanGoOnField(X + i, Y + j)) {
					*x = X+ i, *y = Y + j;
					best = 0;
					break;
				}
			}
	}
	
	return (best != 0xFFFF);
}




int TCaesar::LoadUnit(TUnit *u)
{
	int lpx, lpy;

	if (FlyLevel == 2) return -1;	
	if (GetTotalWeight() + u->GetWeight() > Capacity) return -2;
	if (!GetLoadingPoint(&lpx, &lpy, u)) return -1;
	HideHelpers();
	if (!u->Move(lpx, lpy)) return -1;
	if ((u->X != lpx) || (u->Y != lpy)) {ShowHelpers(); return -1;}
	u->PlaceGround(FALSE);
	u->X = u->Y = -1;
	Inventory[LoadedUnits++] = u->ID;
	u->TimeUnits = 0;
	this->Select();
	ShowHelpers();
	return TRUE;
}



int TCaesar::UnloadUnit(TUnit *u)
{
    int i;
	int lpx, lpy;
	
	if (FlyLevel == 2) return -1;	
	if (u->TimeUnits == 0) return -2;
	if (!GetLoadingPoint(&lpx, &lpy, u)) return -1;
	
	u->X = lpx, u->Y = lpy;
	u->PlaceGround(TRUE);
	for (i = 0; Inventory[i] != u->ID; i++) {};
	for (++i; i < LoadedUnits; i++) Inventory[i-1] = Inventory[i];
	LoadedUnits--;
	TimeUnits = 0;
	this->Select();
	return TRUE;
}



void TCaesar::DoInventory()
{
	int rt, uurt;
	TUnit *u;
	TTransBox *tb;
	
	SelectField(0, 0);
	tb = new TTransBox(128, 202, Inventory, LoadedUnits, (byte*)BmpSmallInventory);
	
	tb->Show();
	while (TRUE) {		
		rt = tb->Handle();
		if (rt == -1) {tb->Hide(); break;}
		RedrawMapLocks++, SelFldLocks++;
		tb->Hide(TRUE);
		uurt = UnloadUnit((TUnit*)Units[Inventory[rt]]);
		RedrawMapLocks--, SelFldLocks--;
		if (uurt == -1) {
			tb->Show(TRUE);
			Message(MSG_OBJECT_IN_WAY2);
			continue;
		}
		else if (uurt == -2) {
			tb->Show(TRUE);
			Message(MSG_OUT_OF_TIME);
			continue;
		}
		tb->Cnt--;
		RedrawMap();
		tb->Show(TRUE);
		if (LoadedUnits == 0) {tb->Hide(); break;}
	}
	delete tb;
	SelectField(Mouse.x, Mouse.y);
}







// Ikaros:

void TIkaros::Setup()
{
	TAircraft::Setup();
	FlyLevel = 2;
	MaxFuel = utIK_FUEL;
	MaxHitPoints = utIK_HP;
	MaxTimeUnits = utIK_TU;
	Defense[0] = utIK_DN0;
	Defense[1] = utIK_DN1;
	Defense[2] = utIK_DN2;
	Visib = utIK_VIS;
	CurWpn = 0;
	WeaponsCnt = 2;
	Weapons[0] = new TCannonAntiAir(WpnNames[WPNNM_CANNON],
	                 utIK_AN, utIK_ANB, utIK_AT, utIK_MAXR, utIK_AMMO);
	Weapons[1] = new TRocketAntiAir(WpnNames[WPNNM_AAROCKETS], 
	                 utIK_2AN, utIK_2ANB, utIK_2AT, 
	                 utIK_2MINR, utIK_2MAXR, utIK_2AMMO);
	Velocity = utIK_VEL;
}



void TIkaros::IncLevel(int alevel)
{
	MaxHitPoints = utIK_HP + alevel * utIK_INC_HP;
	Defense[0] = utIK_DN0 + alevel * utIK_INC_DN;
	Defense[1] = utIK_DN1 + alevel * utIK_INC_DN;
	Defense[2] = utIK_DN2 + alevel * utIK_INC_DN;
	Weapons[0]->IncAN(utIK_AN + alevel * utIK_INC_AN - Weapons[0]->AttackNum);
	Weapons[1]->IncAN(utIK_2AN + alevel * utIK_INC_2AN - Weapons[0]->AttackNum);
}



unsigned TIkaros::GetSupportedActions()
{
	return uatMoveAir | uatSelect | uatSelectAir | 
         uatLandAircr | uatAttackAir | uatAttackAirG;
}







// Kobra:

void TKobra::Setup()
{
	TAircraft::Setup();
	FlyLevel = 2;
	MaxFuel = utKO_FUEL;
	MaxHitPoints = utKO_HP;
	MaxTimeUnits = utKO_TU;
	Defense[0] = utKO_DN0;
	Defense[1] = utKO_DN1;
	Defense[2] = utKO_DN2;
	Visib = utKO_VIS;
	CurWpn = 0;
	WeaponsCnt = 3;
	Weapons[0] = new TCannonAntiAir(WpnNames[WPNNM_CANNON],
	                 utKO_AN, utKO_ANB, utKO_AT, utKO_MAXR, utKO_AMMO);
	Weapons[1] = new TCannon(WpnNames[WPNNM_AGCANNON], 
	                 utKO_2AN, utKO_2ANB, utKO_2AT, utKO_2MAXR, utKO_2AMMO);
	Weapons[2] = new TBomb(WpnNames[WPNNM_BOMB],
	                 utKO_3AN, utKO_3AN2, utKO_3ANB, utKO_3ANB2, utKO_3AT, utKO_3AMMO);
	Velocity = utKO_VEL;
}



void TKobra::IncLevel(int alevel)
{
	MaxHitPoints = utKO_HP + alevel * utKO_INC_HP;
	Defense[0] = utKO_DN0 + alevel * utKO_INC_DN;
	Defense[1] = utKO_DN1 + alevel * utKO_INC_DN;
	Defense[2] = utKO_DN2 + alevel * utKO_INC_DN;
	Weapons[0]->IncAN(utKO_AN + alevel * utKO_INC_AN - Weapons[0]->AttackNum);
	Weapons[1]->IncAN(utKO_2AN + alevel * utKO_INC_2AN - Weapons[0]->AttackNum);
	Weapons[2]->IncAN(utKO_3AN + alevel * utKO_INC_3AN - Weapons[0]->AttackNum);
}



unsigned TKobra::GetSupportedActions()
{
	unsigned s = uatMoveAir | uatSelect | uatSelectAir | 
         uatLandAircr;
  if (CurWpn == 0) s |= uatAttackAir;
  else s |= uatAttack | uatAttackGood | uatAttackField;
  return s;
}



void TKobra::Select()
{
	Weapons[2]->MaxRange = (TimeUnits - utKO_3AT) / 5;
	TAircraft::Select();
}



int TKobra::Attack(int x, int y)
{
	//if ((X == x) && (Y == y)) return FALSE;
	if (CurWpn == 2/*bomba*/) {
		if (!Move(x, y)) return FALSE;
	}
	return TAircraft::Attack(x, y);
}







// Saturn:

void TSaturn::Setup()
{
	TAircraft::Setup();
	FlyLevel = 2;
	MaxFuel = utSA_FUEL;
	MaxHitPoints = utSA_HP;
	MaxTimeUnits = utSA_TU;
	Defense[0] = utSA_DN0;
	Defense[1] = utSA_DN1;
	Defense[2] = utSA_DN2;
	Visib = utSA_VIS;
	CurWpn = -1;
	WeaponsCnt = 0;
	Velocity = utSA_VEL;
	IsBombing = FALSE;
	Bombs= utSA_BAMMO;
	BombAN = utSA_BAN;
}



void TSaturn::IncLevel(int alevel)
{
	MaxHitPoints = utSA_HP + alevel * utSA_INC_HP;
	Defense[0] = utSA_DN0 + alevel * utSA_INC_DN;
	Defense[1] = utSA_DN1 + alevel * utSA_INC_DN;
	Defense[2] = utSA_DN2 + alevel * utSA_INC_DN;
	BombAN = utSA_BAN + alevel * utSA_INC_BAN;
}



unsigned TSaturn::GetSupportedActions()
{
	return uatMoveAir | uatSelect | uatSelectAir | uatLandAircr;
}



void TSaturn::Read(FILE *f)
{
	TAircraft::Read(f);
	fread(&IsBombing, 4, 1, f);
	fread(&Bombs, 4, 1, f);
	fread(&BombAN, 4, 1, f);
}

void TSaturn::Write(FILE *f)
{
	TAircraft::Write(f);
	fwrite(&IsBombing, 4, 1, f);
	fwrite(&Bombs, 4, 1, f);
	fwrite(&BombAN, 4, 1, f);
}



static int saturn_counter = 0;

void TSaturn::GoOnField()
{
	int i, j;
	
	if (IsBombing) {
		if (saturn_counter == 0) { // do bombing
			if (Bombs == 0) return;
			Bombs--;
			for (i = X-2; i <= X+2; i++)
				for (j = Y-2; j <= Y+2; j++) {
					WeaponAttack(i, j, wpnExplos, BombAN, utSA_BANB);
					AddExplode3x3(i, j, 0, 10 - 20 * rand() / RAND_MAX, 10 - 20 * rand() / RAND_MAX);
				}
			IncExplodeTime(+3);
		}
		if (++saturn_counter == 3) saturn_counter = 0;
	}
}



int TSaturn::MoveSUB(int x, int y)
{
	int rt = TAircraft::MoveSUB(x, y);
	if (rt && IsBombing) DoExplosion();
	return rt;
}

int TSaturn::MoveFarSUB(int x, int y)
{
	int rt = TAircraft::MoveFarSUB(x, y);
	if (IsBombing) DoExplosion();
	return rt;
}



int TSaturn::StartBombing()
{
	if (TimeUnits < utSA_BAT) {
		if (ID < BADLIFE) Message(MSG_OUT_OF_TIME);
		return FALSE;
	}
	if (Bombs == 0) {
		if (ID < BADLIFE) Message(MSG_OUT_OF_AMMO);
		return FALSE;
	}
	
	IsBombing = TRUE;
	TimeUnits -= utSA_BAT;
	saturn_counter = 0;
	return TRUE;
}

int TSaturn::EndBombing()
{
	if (TimeUnits < utSA_BAT) {
		if (ID < BADLIFE) Message(MSG_OUT_OF_TIME);
		return FALSE;
	}
	IsBombing = FALSE;
	TimeUnits -= utSA_BAT;
	return TRUE;
}



void TSaturn::GetUnitInfo()
{
	char cbuf[80];

	TAircraft::GetUnitInfo();
	PutStr(UInfoBuf, UINFO_SX, 2, 78, SigText[TXT_BOMBS_LEFT], NormalFont, clrWhite, clrBlack);

	sprintf(cbuf, "%i / %i", Bombs, utSA_BAMMO);
	PercentBar(UInfoBuf, UINFO_SX, 54, 80, 52, 13, clrLightBlue2, clrSeaBlue, (double)Bombs / utSA_BAMMO, cbuf);
	if (IsBombing) CopyBmp(UInfoBuf, UINFO_SX, 2, 147, BmpBombing[1], 59, 59);	
	else CopyBmp(UInfoBuf, UINFO_SX, 2, 147, BmpBombing[0], 59, 59);	
}



int TSaturn::InfoEvent(TEvent *e)
{
	int rt = TAircraft::InfoEvent(e);
	
	if (!rt && ((e->What == evMouseDown) && 
	   (IsInRect(e->Mouse.Where.x - UINFO_X, e->Mouse.Where.y - UINFO_Y,
	             2, 147, 2+59, 147+59))) ||
	   ((e->What == evKeyDown) && (e->Key.KeyCode == kbEnter))) {
		if (IsBombing) EndBombing();
		else StartBombing();
		Select();
		return TRUE;
	}
	return rt;
}












/////////////// Protom:

void TProton::Setup()
{
	TAircraft::Setup();
	FlyLevel = 2;
	MaxFuel = utPR_FUEL;
	MaxHitPoints = utPR_HP;
	MaxTimeUnits = utPR_TU;
	Defense[0] = utPR_DN0;
	Defense[1] = utPR_DN1;
	Defense[2] = utPR_DN2;
	Visib = utPR_VIS;
	CurWpn = -1;
	WeaponsCnt = 0;
	Velocity = utPR_VEL;
}



void TProton::IncLevel(int alevel)
{
}



unsigned TProton::GetSupportedActions()
{
	return uatMoveAir | uatSelect | uatSelectAir | 
         uatLandAircr | uatSupportAir;
}



void TProton::GetCursor(int x, int y, int *cursor, int *selbold)
{
	TField *f = GetField(x, y);
	
	TAircraft::GetCursor(x, y, cursor, selbold);
	
	switch (UnitActionType) {
		case uatSupportAir:
				if (f->Unit != ID) 	*cursor = mcurSupport;
				else 	*cursor = mcurArrow;
				*selbold = TRUE;
				AirCursorOn = GetAircraftAt(x, y)->FlyLevel;
				break;
	}
}



void TProton::Action(int x, int y)
{
	TField *f = GetField(x, y);

	TAircraft::Action(x, y);
	
	switch (UnitActionType) {
		case uatSupportAir : 
				DoSupport(x, y);
				break;
	}
}



void TProton::DoSupport(int x, int y)
{
	TField *f = GetField(x, y);
	int t, i, j, dx = 0, dy = 0, best = 0xFFFF;
	TUnit *u = GetAircraftAt(x, y);
	
	if (TimeUnits < utSUPP_SUPPORT) {
		if (ID < BADLIFE) Message(MsgText[MSG_OUT_OF_TIME]);
		return;
	}
	if (u->ID == NO_UNIT) return;
	if ((u->ID < BADLIFE) && (this->ID >= BADLIFE)) return;
	if ((u->ID >= BADLIFE) && (this->ID < BADLIFE)) return;
	if (!WillSupport((TUnit*)u)) return;
	
	for (i = -1; i <= 1; i++)
		for (j = -1; j <= 1; j++) {
			if ((X == x + i) && (Y == y + j)) {
				dx = i, dy = j;
				best = 0;
			}
			t = TimeToMove(x + i, y + j);
			if ((t > 0) && (t < best)) {
				best = t;
				dx = i, dy = j;
			}
		}
	
	if ((dx == 0) && (dy == 0)) return;
	Move(x + dx, y + dy);
	if (TimeUnits < utSUPP_SUPPORT) {
		if (ID < BADLIFE) Message(MsgText[MSG_OUT_OF_TIME]);
		return;
	}
	if ((x + dx == X) && (y + dy == Y)) {
		SupportUnit((TUnit*)u);
		TimeUnits -= utSUPP_SUPPORT;
		if (this == SelectedUnit) Select();
	}
}



void TProton::SupportUnit(TUnit *Unit)
{
	int delta = Unit->MaxFuel - Unit->Fuel;
	if (Unit->Type == unCeres) delta = Unit->MaxFuel / 10 - Unit->Fuel;
	if (delta > this->Fuel) delta =  this->Fuel;
	Fuel -= delta;
	Unit->Fuel += delta;
}



int TProton::WillSupport(TUnit *Unit)
{
	return Unit->Fuel < Unit->MaxFuel;
}
