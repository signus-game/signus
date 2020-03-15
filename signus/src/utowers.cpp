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



// Obranne veze:


#include <SDL_timer.h>

#include "utowers.h"
#include "sound.h"
#include "showrngs.h"
#include "untables.h"


/////////////// TTower - obecny predek


void TTower::GetUnitInfo() {
	char cbuf[30];
	int clr;

	TObject::GetUnitInfo();
	CopyBmpNZ(UInfoBuf, UINFO_SX, 77, 1, LevelBmps[Level], 29, 16);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 2, GetName(), NormalFont,
		clrLightBlue, clrBlack);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 26, SigText[TXT_STATE],
		NormalFont, clrWhite, clrBlack);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 42, SigText[TXT_TIME],
		NormalFont, clrWhite, clrBlack);

	sprintf(cbuf, "%i %%", 100 * HitPoints / MaxHitPoints);
	clr = (100 * HitPoints < 20 * MaxHitPoints) ? clrRed : clrLightBlue2;
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 54, 28, 52, 13, clr,
		clrSeaBlue, (double)HitPoints / MaxHitPoints, cbuf);
	sprintf(cbuf, "%i/%i", TimeUnits, MaxTimeUnits);
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 54, 44, 52, 13, clrLightBlue2,
		clrSeaBlue, (double)TimeUnits / MaxTimeUnits, cbuf);

	// zbrane:
	for (int i = 0; i < WeaponsCnt; i++) {
		CopyBmp(UInfoBuf, UINFO_SX, 3, 83 + i * 26,
			((i == CurWpn) && (WeaponsCnt != 1)) ?
			WpnInfoBkg[1] : WpnInfoBkg[0], 102, 23);
		PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 5, 86 + i * 26,
			Weapons[i]->Name, NormalFont, clrWhite, clrBlack);
		sprintf(cbuf, "%i/%i", Weapons[i]->Ammo, Weapons[i]->MaxAmmo);
		PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 60, 88 + i * 26, 41,
			13, clrLightBlue2, clrSeaBlue,
			(double)Weapons[i]->Ammo / Weapons[i]->MaxAmmo, cbuf);
	}

	if (CurWpn != -1) {
		CopyBmpNZ(UInfoBuf, UINFO_SX, 2, 188,
			BmpAmmoIcons[Weapons[CurWpn]->GetType()], 30, 13);
		sprintf(cbuf, "%i", Weapons[CurWpn]->TimeLost);
		PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 35, 188, cbuf, NormalFont,
			clrWhite, clrBlack);
	}
}



unsigned TTower::GetSupportedActions()
{
    return uatSelect | uatSelectAir | uatAttack | uatAttackGood | uatAttackField;
}



void TTower::CreateRangeHelpers()
{
    if (ID < BADLIFE) {
        HideHelpers();
        ClearRanges();
        if ((CurWpn != -1) && (Weapons[CurWpn]->TimeLost <= TimeUnits)) 
          GenerateShootRng(X, Y, Weapons[CurWpn]->MinRange, Weapons[CurWpn]->MaxRange);
        GenerateVisibRng(X, Y, Visib);
        ShowHelpers();
    }
    else if (ClearRanges()) RedrawMap();
}



void TTower::ShowShootAt(int x, int y, int phase)
{
    if (phase == 0) {
        HideHelpers();
        WpnRotate(x, y);
        ActualSprite = 1*8 + Orient;
        PaintUnit();
        SDL_Delay(iniAnimDelay2);   
        ActualSprite = 2*8 + Orient;
        PaintUnit();
        SDL_Delay(iniAnimDelay2);   
        ActualSprite = 1*8 + Orient;
        PaintUnit();
        SDL_Delay(iniAnimDelay2);   
        ActualSprite = 0*8 + Orient;
        PaintUnit();
        SDL_Delay(iniAnimDelay2);   
        ShowHelpers();
    }
}



void TTower::FieldSelected(int x, int y)
{
    TObject::FieldSelected(x, y);
    
    if (UnitActionType & uatAttack) {
        TField *f = GetField(X, Y);
        if ((L1TerrainType[f->Terrain] != tofsL1A) || (!f->OnScreen)) return;
    
      WpnRotate(x, y);
  }
}

















////////////// TThor

void TThor::Setup()
{
    TTower::Setup();
    MaxFuel = 0;
    MaxHitPoints = utTH_HP;
    MaxTimeUnits = utTH_TU;
    Defense[0] = utTH_DN0;
    Defense[1] = utTH_DN1;
    Defense[2] = utTH_DN2;
    Visib = utTH_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TRocket(WpnNames[WPNNM_ROCKETS], 
                     utTH_AN, utTH_ANB, utTH_AN2, utTH_ANB2, utTH_AT,
                     utTH_MINR,utTH_MAXR, 8);
    Weapons[0]->Ammo = 0;
    TotalRockets = utTH_AMMO;
    ActualSprite = 8;
    IsOverground = FALSE;
}

void TThor::IncLevel(int alevel)
{
    MaxHitPoints = utTH_HP + alevel * utTH_INC_HP;
    // kdyz se zlepsuje level, Thor musi byt venku (zrovna vystrelil)
    // proto muzem pouzit to nizsi obranny cislo pro vypocet
    Defense[0] = (utTH_DN0 - utTH_DND0) + alevel * utTH_INC_DN;
    Defense[1] = (utTH_DN1 - utTH_DND1) + alevel * utTH_INC_DN;
    Defense[2] = (utTH_DN2 - utTH_DND2) + alevel * utTH_INC_DN;
    Weapons[0]->IncAN(utTH_AN + alevel * utTH_INC_AN - Weapons[0]->AttackNum);
}



void TThor::Init(int x, int y, int party, ReadStream *stream) {
	TTower::Init(x, y, party, stream);

	if (!IsOverground) {
		ActualSprite = 8;
	}

	if (IconThorOut == NULL) {
		IconThorOut = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "icthora%i", 13);
		IconThorIn = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "icthorb%i", 13);
	}
}



void TThor::Read(ReadStream &stream) {
	TTower::Read(stream);
	IsOverground = stream.readSint32LE();
	TotalRockets = stream.readSint32LE();
}

void TThor::Write(WriteStream &stream) {
	TTower::Write(stream);
	stream.writeSint32LE(IsOverground);
	stream.writeSint32LE(TotalRockets);
}



void TThor::GetUnitInfo() {
	char cbuf[40];

	TTower::GetUnitInfo();
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 58, SigText[TXT_ROCKETS_LEFT],
		NormalFont, clrWhite, clrBlack);
	sprintf(cbuf, "%i / 100", TotalRockets);
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 54, 60, 52, 13, clrLightBlue2,
		clrSeaBlue, (double)TotalRockets / 100, cbuf);

	if (IsOverground) {
		CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconThorIn->IconPic[0], 59,
			59);
	} else {
		CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconThorOut->IconPic[0],
			59, 59);
	}

	CopyBmpNZ(UInfoBuf, UINFO_SX, 2, 129,
		BmpAmmoIcons[Weapons[CurWpn]->GetType()], 30, 13);
	sprintf(cbuf, "%i", Weapons[CurWpn]->TimeLost);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 35, 129, cbuf, NormalFont,
		clrWhite, clrBlack);
}



unsigned TThor::GetSupportedActions()
{
    if (IsOverground) return TTower::GetSupportedActions();
    else return uatSelect | uatSelectAir;
}



void TThor::GoOverground(int over)
{
    int i;

    if (over == IsOverground) return;

    if (TimeUnits < 30) {
        if (ID < BADLIFE) Message(MSG_OUT_OF_TIME);
        return;
    }
    TimeUnits -= 30;
    IsOverground = over;
    HideHelpers();
    if (GetField(X, Y)->Visib == 2) {
        if (over) PlaySample(GetUnitSound(6), 6, EffectsVolume, GetFieldPanning(X, Y));
        else PlaySample(GetUnitSound(7), 6, EffectsVolume, GetFieldPanning(X, Y));
    }
    if (over) {
        for (i = 9; i < 16; i++) {
            ActualSprite = i;
            PaintUnit();
            SDL_Delay(iniAnimDelay2);
        }
        Orient = 3; ActualSprite = 3;
        Weapons[0]->Ammo = 8;
        TotalRockets -= 8;
        if (TotalRockets < 0) {
            Weapons[0]->Ammo += TotalRockets;
            TotalRockets = 0;
        }
        Defense[0] = -utTH_DND0,    Defense[1] -= utTH_DND1,    Defense[2] -= utTH_DND2;
    }
    else {
        Rotate(3);
        for (i = 15; i > 8; i--) {
            ActualSprite = i;
            PaintUnit();
            SDL_Delay(iniAnimDelay2);
        }
        ActualSprite = 8;
        TotalRockets += Weapons[0]->Ammo;
        Weapons[0]->Ammo = 0;
        Defense[0] += utTH_DND0,    Defense[1] += utTH_DND1,    Defense[2] += utTH_DND2;
    }
    ShowHelpers();
    PaintUnit();
}



int TThor::InfoEvent(TEvent *e)
{
    int rt = TTower::InfoEvent(e);
    TIcon *i;
    if (IsOverground) i = IconThorIn; else i = IconThorOut;
    
    if (!rt && i->Handle(e)) {     
        GoOverground(!IsOverground);
        ShowUnitInfo();
        return TRUE;
    }
    return rt;
}



void TThor::ShowShootAt(int x, int y, int phase)
{
    if (phase == 0) WpnRotate(x, y);
}









////////////// TPagoda

void TPagoda::Setup()
{
    TTower::Setup();
    MaxFuel = 0;
    MaxHitPoints = utPD_HP;
    MaxTimeUnits = utPD_TU;
    Defense[0] = utPD_DN0;
    Defense[1] = utPD_DN1;
    Defense[2] = utPD_DN2;
    Visib = utPD_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TCannon(WpnNames[WPNNM_CANNON], 
                 utPD_AN, utPD_ANB, utPD_AT, utPD_MAXR, utPD_AMMO);
}

void TPagoda::IncLevel(int alevel)
{
    MaxHitPoints = utPD_HP + alevel * utPD_INC_HP;
    Defense[0] = utPD_DN0 + alevel * utPD_INC_DN;
    Defense[1] = utPD_DN1 + alevel * utPD_INC_DN;
    Defense[2] = utPD_DN2 + alevel * utPD_INC_DN;
    Weapons[0]->IncAN(utPD_AN + alevel * utPD_INC_AN - Weapons[0]->AttackNum);
}









////////////// TMinotaurus:

void TMinotaurus::Setup()
{
    TTower::Setup();
    MaxFuel = 0;
    MaxHitPoints = utMI_HP;
    MaxTimeUnits = utMI_TU;
    Defense[0] = utMI_DN0;
    Defense[1] = utMI_DN1;
    Defense[2] = utMI_DN2;
    Visib = utMI_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TBalisticWeapon(WpnNames[WPNNM_CANNON], 
                     utMI_AN, utMI_ANB, utMI_AN2, utMI_ANB2, utMI_AT,
                     utMI_MINR, utMI_MAXR, utMI_AMMO);
}

void TMinotaurus::IncLevel(int alevel)
{
    MaxHitPoints = utMI_HP + alevel * utMI_INC_HP;
    Defense[0] = utMI_DN0 + alevel * utMI_INC_DN;
    Defense[1] = utMI_DN1 + alevel * utMI_INC_DN;
    Defense[2] = utMI_DN2 + alevel * utMI_INC_DN;
    Weapons[0]->IncAN(utMI_AN + alevel * utMI_INC_AN - Weapons[0]->AttackNum);
}










////////////// THorizont

void THorizont::Setup()
{
    TTower::Setup();
    MaxFuel = 0;
    MaxHitPoints = utHR_HP;
    MaxTimeUnits = utHR_TU;
    Defense[0] = utHR_DN0;
    Defense[1] = utHR_DN1;
    Defense[2] = utHR_DN2;
    Visib = utHR_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
                     utHR_AN, utHR_ANB, utHR_AT, utHR_MAXR, utHR_AMMO);
}

void THorizont::IncLevel(int alevel)
{
    MaxHitPoints = utHR_HP + alevel * utHR_INC_HP;
    Defense[0] = utHR_DN0 + alevel * utHR_INC_DN;
    Defense[1] = utHR_DN1 + alevel * utHR_INC_DN;
    Defense[2] = utHR_DN2 + alevel * utHR_INC_DN;
    Weapons[0]->IncAN(utHR_AN + alevel * utHR_INC_AN - Weapons[0]->AttackNum);
}


unsigned THorizont::GetSupportedActions()
{
    unsigned sa = uatSelect | uatSelectAir | uatAttackAir | uatAttackAirG;
    return sa;
}









////////////// TSpektrum

void TSpektrum::Setup()
{
    TTower::Setup();
    MaxFuel = 0;
    MaxHitPoints = utSP_HP;
    MaxTimeUnits = utSP_TU;
    Defense[0] = utSP_DN0;
    Defense[1] = utSP_DN1;
    Defense[2] = utSP_DN2;
    Visib = utSP_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TLaser(WpnNames[WPNNM_LASER], 
                 utSP_AN, utSP_ANB, utSP_AT, utSP_MAXR, utSP_AMMO);
}

void TSpektrum::IncLevel(int alevel)
{
    MaxHitPoints = utSP_HP + alevel * utSP_INC_HP;
    Defense[0] = utSP_DN0 + alevel * utSP_INC_DN;
    Defense[1] = utSP_DN1 + alevel * utSP_INC_DN;
    Defense[2] = utSP_DN2 + alevel * utSP_INC_DN;
    Weapons[0]->IncAN(utSP_AN + alevel * utSP_INC_AN - Weapons[0]->AttackNum);
}

