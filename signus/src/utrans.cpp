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

Trasportni jednotky - Uran, Gargantua, Oasa, ...

*/


#define CROSS_REFERENCE_TO_SHIPS

#include "utrans.h"
#include "untables.h"
#include "transbox.h"
#include "mouse.h"





//////////////// obecne


void TTransporter::Init(int x, int y, int party, ReadStream *stream)
{
	TUnit::Init(x, y, party, stream);

	if (IconTransport == NULL) {
		IconTransport = new TIcon(RES_X-115, UINFO_Y+110, 102, 23, "tranbut%i", 13);
	}

	if ((Capacity == TCAPACITY_BIG) && !BmpBigInventory) {
		BmpBigInventory = GraphicsDF->get("tranbox0");
	} else if ((Capacity == TCAPACITY_SMALL) && !BmpSmallInventory) {
		BmpSmallInventory = GraphicsDF->get("tranbox1");
	} else if ((Capacity == TCAPACITY_MEDIUM) && !BmpMediumInventory) {
		BmpMediumInventory = GraphicsDF->get("tranbox2");
	}
}



void TTransporter::Setup()
{
	TUnit::Setup();
	LoadedUnits = 0;
	for (int i = 0; i < MAX_TCAPACITY; i++) Inventory[i] = 0;
}



unsigned TTransporter::GetAvailableActions()
{
	if (ID >= BADLIFE) {
		if (GetField(X, Y)->Visib == 2) return uatAttack;
		else return uatNothing;
	}
	else return uatSelect | uatAttackGood | uatLoadIn | uatSupport;
}



int TTransporter::GetTotalWeight()
{
	int i, total = 0;
	
	for (i = 0; i < LoadedUnits; i++) total += Units[Inventory[i]]->GetWeight();
	return total;
}



int TTransporter::GetLoadingPoint(int *x, int *y, TUnit *u)
{
	int t, i, j, best = 0xFFFF;
	
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
				if (u->CanGoOnField(X + i, Y + j)) {
					*x = X+ i, *y = Y + j;
					best = 0;
					break;
				}
			}
	}
	
	return (best != 0xFFFF);
}



int TTransporter::WantToLoad(TUnit *u)
{
	return !((u->Type >= unHorizont) && (u->Type <= unBarracuda));
}




int TTransporter::LoadUnit(TUnit *u)
{
	int lpx, lpy;
	
	if (!WantToLoad(u)) return -1;
	if (Type == u->Type) return -2;
	if ((Type != unOasa) && (
	    (u->Type == unOlymp) || (u->Type == unUran) ||
	    (u->Type == unNeptun))) return -2;
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



int TTransporter::UnloadUnit(TUnit *u)
{
    int i;
	int lpx, lpy;
	
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



void TTransporter::DoInventory()
{
	int rt, uurt;
	TTransBox *tb;
	
	SelectField(0, 0);
	if (Capacity == TCAPACITY_BIG)
		tb = new TTransBox(472, 202, Inventory, LoadedUnits, (byte*)BmpBigInventory);
	else if (Capacity == TCAPACITY_SMALL)
		tb = new TTransBox(128, 202, Inventory, LoadedUnits, (byte*)BmpSmallInventory);
	else if (Capacity == TCAPACITY_MEDIUM)
		tb = new TTransBox(248, 202, Inventory, LoadedUnits, (byte*)BmpMediumInventory);
	else
		abort();
	
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



int TTransporter::InfoEvent(TEvent *e)
{
	int rt = TUnit::InfoEvent(e);
	
	if (!rt && IconTransport->Handle(e)) {
		DoInventory();
		return TRUE;
	}
	return rt;
}



void TTransporter::Action(int x, int y)
{
	if ((x == X) && (y == Y) && (UnitActionType != uatSelectAir)) {
		DoInventory();
		return;
	}
	
	TUnit::Action(x, y);
}



void TTransporter::GetUnitInfo(bool alt_wpinfo) {
	TUnit::GetUnitInfo(alt_wpinfo);
	CopyBmp(UInfoBuf, UINFO_SX, 3, 110, IconTransport->IconPic[0], 102, 23);
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 3, 135, 102, 8, clrLightBlue2,
		clrSeaBlue, ((double)GetTotalWeight() / Capacity), "");
}



void TTransporter::Read(ReadStream &stream) {
	TUnit::Read(stream);
	LoadedUnits = stream.readSint32LE();

	for (int i = 0; i < LoadedUnits; i++) {
		Inventory[i] = stream.readSint32LE();
	}
}

void TTransporter::Write(WriteStream &stream) {
	TUnit::Write(stream);
	stream.writeSint32LE(LoadedUnits);

	for (int i = 0; i < LoadedUnits; i++) {
		stream.writeSint32LE(Inventory[i]);
	}
}



void TTransporter::ChangeParty()
{
	TObject *dum;

	TUnit::ChangeParty();
	for (int i = 0; i < LoadedUnits; i++) {
		dum = Units[Inventory[i]];
		dum->ChangeParty();
		Inventory[i] = dum->ID;
	}
}



void TTransporter::RemoveFromWorld()
{
	for (int i = 0; i < LoadedUnits; i++) 
		Units[Inventory[i]]->RemoveFromWorld();
	TUnit::RemoveFromWorld();
}














///////////////////////////// P O Z E M N I ///////////////////////////////

void TGroundTransporter::MakeTrack() {MakeGroundTrack(X, Y, Orient);}








// Uran:

void TUran::Setup()
{
	TGroundTransporter::Setup();
	MaxFuel = utUR_FUEL;
	MaxHitPoints = utUR_HP;
	MaxTimeUnits = utUR_TU;
	Defense[0] = utUR_DN0;
	Defense[1] = utUR_DN1;
	Defense[2] = utUR_DN2;
	Visib = utUR_VIS;
	CurWpn = -1;
	WeaponsCnt = 0;
	Velocity = utUR_VEL;
}

void TUran::IncLevel(int alevel)
{
}








// Neptun:

void TNeptun::Setup()
{
	TTransporter::Setup();
	MaxFuel = utNE_FUEL;
	MaxHitPoints = utNE_HP;
	MaxTimeUnits = utNE_TU;
	Defense[0] = utNE_DN0;
	Defense[1] = utNE_DN1;
	Defense[2] = utNE_DN2;
	Visib = utNE_VIS;
	CurWpn = -1;
	WeaponsCnt = 0;
	Velocity = utNE_VEL;
}

void TNeptun::IncLevel(int alevel)
{
}










//////////////////////////// VLAKOIDNI ///////////////////////////////////


// Olymp:

void TOlymp::Setup()
{
	TTransporter::Setup();
	MaxFuel = utOL_FUEL;
	MaxHitPoints = utOL_HP;
	MaxTimeUnits = utOL_TU;
	Defense[0] = utOL_DN0;
	Defense[1] = utOL_DN1;
	Defense[2] = utOL_DN2;
	Visib = utOL_VIS;
	CurWpn = -1;
	WeaponsCnt = 0;
	Velocity = utOL_VEL;
	SpriteOrient = -1;
}

void TOlymp::IncLevel(int alevel)
{
}



void TOlymp::Write(WriteStream &stream) {
	TTransporter::Write(stream);
	stream.writeSint32LE(SpriteOrient);
}

void TOlymp::Read(ReadStream &stream) {
	TTransporter::Read(stream);
	SpriteOrient = stream.readSint32LE();
}



void TOlymp::Rotate(int angle)
{
	if (SpriteOrient == -1) SpriteOrient = Orient;
	int chng = Orient != SpriteOrient;
	if (abs(Orient - angle) != 4) {
		SpriteOrient = angle;
		if (chng) /*jeden pospatku*/ SpriteOrient = (angle + 4) % 8;
	}
	TTransporter::Rotate(angle);
}



TSprite *TOlymp::GetSprite()
{
	if (SpriteOrient == -1) SpriteOrient = Orient;
	if (ActualSprite < 8) 
		return UnitsSprites[Type][SpriteOrient];
	else
		return UnitsSprites[Type][ActualSprite];
}




