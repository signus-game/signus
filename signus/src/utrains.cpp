/*
 *  This file is part of Signus: The Artefact Wars
 *  https://github.com/signus-game
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


Vlakove jednotky...


*/


#include "units.h"
#include "utrains.h"
#include "untables.h"






//////////////////////////// obecne:

void TToweredTrainUnit::Setup()
{
    TToweredUnit::Setup();
    SpriteOrient = -1;
}



void TToweredTrainUnit::Write(WriteStream &stream) {
	TToweredUnit::Write(stream);
	stream.writeSint32LE(SpriteOrient);
}

void TToweredTrainUnit::Read(ReadStream &stream) {
	TToweredUnit::Read(stream);
	SpriteOrient = stream.readSint32LE();
}



void TToweredTrainUnit::Rotate(int angle)
{
    if (SpriteOrient == -1) SpriteOrient = Orient;
    int chng = Orient != SpriteOrient;
    if (abs(Orient - angle) != 4) {
        SpriteOrient = angle;
        if (chng) /*jeden pospatku*/ SpriteOrient = (angle + 4) % 8;
    }
    TToweredUnit::Rotate(angle);
}



TSprite *TToweredTrainUnit::GetSprite()
{
    if (SpriteOrient == -1) SpriteOrient = Orient;
    if (ActualSprite < 8) 
        return UnitsSprites[Type][SpriteOrient];
    else
        return UnitsSprites[Type][ActualSprite];
}




void TToweredTrainUnit::Draw()
{
    TField *f = GetField(X, Y);
    TSprite *s, *sst;
    int rrx = GetRelX(X), rry = GetRelY(Y);
    int drawx = 28 * (rrx - rry) + LittleX + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
    
    if ((!SelectCrossLock) && (SelectedUnit == this)) {
        void *oldb = MapBuf;
        MapBuf = FullBuf;
        DrawL2Selector(drawx-LittleX, drawy+28-LittleY, f->Terrain, BmpSelected);
        MapBuf = oldb;
  }

    s = GetSprite();
    DrawSprite(drawx, drawy, s);
    if (ActualSprite < 8/*tj.jsme v rovine*/) {
        s = UnitsSprites[Type][40 + WpnOrient];
        DrawSprite(drawx + SpriteLocators[Type][SpriteOrient*2], 
                   drawy + SpriteLocators[Type][SpriteOrient*2+1], s);
    }

    s = GetSmoke();
    if (s) DrawSprite(drawx, drawy, s);

    if (iniShowStatusbar && (!StatusbarLock)) {
        if ((sst = GetStatusBar()) != NULL) {
            DrawSprite(drawx, drawy, sst);
            memfree(sst);
        }
    }
    GetDrawRect(&(LD_rect[ID])); LD_used[ID] = TRUE;
}



void TToweredTrainUnit::GetDrawRect(TRect *r)
{
    TRect r1;
    TField *f = GetField(X, Y);
    TSprite *s = GetSprite();
    int rrx = GetRelX(X), rry = GetRelY(Y);
    int drawx = 28 * (rrx - rry) + LittleX + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
    
    r->x1 = drawx - s->dx, r->y1 = drawy - s->dy;
    r->x2 = r->x1 + s->w, r->y2 = r->y1 + s->h;

    s = UnitsSprites[Type][40 + WpnOrient];
    r1.x1 = drawx + SpriteLocators[Type][SpriteOrient*2] - s->dx, 
    r1.y1 = drawy + SpriteLocators[Type][SpriteOrient*2+1] - s->dy;
    r1.x2 = r1.x1 + s->w, r1.y2 = r1.y1 + s->h;
    Union(r, &r1);

    s = GetSmoke();
    if (s) {
        r1.x1 = drawx - s->dx, r1.y1 = drawy - s->dy;
        r1.x2 = r1.x1 + s->w, r1.y2 = r1.y1 + s->h;
        Union(r, &r1);
    }
}





void TTrainSupportUnit::Setup()
{
    TSupportUnit::Setup();
    SpriteOrient = -1;
}



void TTrainSupportUnit::Write(WriteStream &stream) {
	TSupportUnit::Write(stream);
	stream.writeSint32LE(SpriteOrient);
}

void TTrainSupportUnit::Read(ReadStream &stream) {
	TSupportUnit::Read(stream);
	SpriteOrient = stream.readSint32LE();
}



void TTrainSupportUnit::Rotate(int angle)
{
    if (SpriteOrient == -1) SpriteOrient = Orient;
    int chng = Orient != SpriteOrient;
    if (abs(Orient - angle) != 4) {
        SpriteOrient = angle;
        if (chng) /*jeden pospatku*/ SpriteOrient = (angle + 4) % 8;
    }
    TSupportUnit::Rotate(angle);
}



TSprite *TTrainSupportUnit::GetSprite()
{
    if (SpriteOrient == -1) SpriteOrient = Orient;
    if (ActualSprite < 8) 
        return UnitsSprites[Type][SpriteOrient];
    else
        return UnitsSprites[Type][ActualSprite];
}







//////////////////////////////////


// Mohakan:

void TMohykan::Setup()
{
    TToweredTrainUnit::Setup();
    MaxFuel = utMH_FUEL;
    MaxHitPoints = utMH_HP;
    MaxTimeUnits = utMH_TU;
    Defense[0] = utMH_DN0;
    Defense[1] = utMH_DN1;
    Defense[2] = utMH_DN2;
    Visib = utMH_VIS;
    CurWpn = 0;
    WeaponsCnt = 2;
    Weapons[0] = new TCannon(WpnNames[WPNNM_CANNON], 
                     utMH_AN, utMH_ANB, utMH_AT, utMH_MAXR, utMH_AMMO);
    Weapons[1] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
                     utMH_2AN, utMH_2ANB, utMH_2AT, utMH_2MAXR, utMH_2AMMO);
    Velocity = utMH_VEL;
}

void TMohykan::IncLevel(int alevel)
{
    MaxHitPoints = utMH_HP + alevel * utMH_INC_HP;
    Defense[0] = utMH_DN0 + alevel * utMH_INC_DN;
    Defense[1] = utMH_DN1 + alevel * utMH_INC_DN;
    Defense[2] = utMH_DN2 + alevel * utMH_INC_DN;
    Weapons[0]->IncAN(utMH_AN + alevel * utMH_INC_AN - Weapons[0]->AttackNum);
    Weapons[1]->IncAN(utMH_2AN + alevel * utMH_INC_2AN - Weapons[1]->AttackNum);
}





// Mesias:

void TMesias::Setup()
{
    TToweredTrainUnit::Setup();
    MaxFuel = utME_FUEL;
    MaxHitPoints = utME_HP;
    MaxTimeUnits = utME_TU;
    Defense[0] = utME_DN0;
    Defense[1] = utME_DN1;
    Defense[2] = utME_DN2;
    Visib = utME_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TBalisticWeapon(WpnNames[WPNNM_CANNON], 
                     utME_AN, utME_ANB, utME_AN2, utME_ANB2, utME_AT, 
                     utME_MINR, utME_MAXR, utME_AMMO);
    Velocity = utME_VEL;
}

void TMesias::IncLevel(int alevel)
{
    MaxHitPoints = utME_HP + alevel * utME_INC_HP;
    Defense[0] = utME_DN0 + alevel * utME_INC_DN;
    Defense[1] = utME_DN1 + alevel * utME_INC_DN;
    Defense[2] = utME_DN2 + alevel * utME_INC_DN;
    Weapons[0]->IncAN(utME_AN + alevel * utME_INC_AN - Weapons[0]->AttackNum);
}





// Ganymedes

void TGanymedes::Setup()
{
    TTrainSupportUnit::Setup();
    MaxFuel = utGA_FUEL;
    MaxHitPoints = utGA_HP;
    MaxTimeUnits = utGA_TU;
    Defense[0] = utGA_DN0;
    Defense[1] = utGA_DN1;
    Defense[2] = utGA_DN2;
    Visib = utGA_VIS;
    Velocity = utGA_VEL;
    CurWpn = -1;
    WeaponsCnt = 0;
    MaxAmmo[1] = utGA_AMGN, MaxAmmo[2] = utGA_AMEX,
    MaxAmmo[0] = utGA_AMEN, MaxAmmo[3] = utGA_AMMI;
}

void TGanymedes::AfterSetup()
{
    TTrainSupportUnit::AfterSetup();
    for (int i = 0; i < 4; i++) Ammo[i] = MaxAmmo[i];
}



void TGanymedes::Read(ReadStream &stream) {
	TTrainSupportUnit::Read(stream);

	for (int i = 0; i < 4; i++) {
		Ammo[i] = stream.readSint32LE();
		MaxAmmo[i] = stream.readSint32LE();
	}
}

void TGanymedes::Write(WriteStream &stream) {
	TTrainSupportUnit::Write(stream);

	for (int i = 0; i < 4; i++) {
		stream.writeSint32LE(Ammo[i]);
		stream.writeSint32LE(MaxAmmo[i]);
	}
}



void TGanymedes::GetUnitInfo(bool alt_wpinfo) {
	char cbuf[30];
	int i;

	TTrainSupportUnit::GetUnitInfo(alt_wpinfo);

	for (i = 0; i <4; i++) {
		sprintf(cbuf, "%i/%i", Ammo[i], MaxAmmo[i]);
		PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 34, 80+i*20, 72, 13,
			clrLightBlue2, clrSeaBlue,
			(double)Ammo[i] / MaxAmmo[i], cbuf);
		CopyBmpNZ(UInfoBuf, UINFO_SX, 2, 80+i*20, BmpAmmoIcons[i], 30,
			13);
	}
}



void TGanymedes::SupportUnit(TUnit *Unit) {
	int i, delta, typ;

	if (Unit->Type == unXenon) {
		TXenon *xu = (TXenon*)Unit;

		delta = utXE_MINES - xu->Mines;

		if (delta > Ammo[wpnMine]) {
			delta = Ammo[wpnMine];
		}

		Ammo[wpnMine] -= delta;
		xu->Mines += delta;
	}

	for (i = 0; i < Unit->WeaponsCnt; i++) {
		typ = Unit->Weapons[i]->GetType();
		delta = Unit->Weapons[i]->MaxAmmo - Unit->Weapons[i]->Ammo;

		if (delta > Ammo[typ]) {
			delta = Ammo[typ];
		}

		Ammo[typ] -= delta;
		Unit->Weapons[i]->Ammo += delta;
	}

	delta = Unit->MaxFuel - Unit->Fuel;

	if (Unit->Type == unCeres) {
		delta = Unit->MaxFuel / 10 - Unit->Fuel;
	}

	if (delta > this->Fuel) {
		delta = this->Fuel;
	}

	Fuel -= delta;
	Unit->Fuel += delta;

	if (Unit->HitPoints < Unit->MaxHitPoints) {
		Unit->HitPoints = Unit->MaxHitPoints;
	}

	Unit->PaintUnit(FALSE);
	UpdateScreen();
}


int TGanymedes::WillSupport(TUnit *Unit)
{
    if (Unit->Fuel < Unit->MaxFuel) return TRUE;
    if ((Unit->HitPoints != Unit->MaxHitPoints / 2) &&
           (Unit->HitPoints != Unit->MaxHitPoints)) return TRUE;
    if ((Unit->Type == unXenon) && (((TXenon*)Unit)->Mines < utXE_MINES)) return TRUE;
    if (Unit->WeaponsCnt == 0) return FALSE;
    for (int i = 0; i < Unit->WeaponsCnt; i++)
        if (Unit->Weapons[i]->Ammo < Unit->Weapons[i]->MaxAmmo) return TRUE;
    return TRUE;
}
