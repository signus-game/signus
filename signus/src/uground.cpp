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



// Pozemni jednotky:



#include "headers.h"
#include <SDL_timer.h>

#include "uground.h"
#include "explode.h"
#include "untables.h"
#include "fields.h"
#include "mouse.h"


void MakeGroundTrack(int X, int Y, int Orient)
{
}


/////////////// TGroundUnit - obecny predek


void TGroundUnit::MakeTrack() {MakeGroundTrack(X, Y, Orient);}

void TGroundToweredUnit::MakeTrack() {MakeGroundTrack(X, Y, Orient);}

void TGroundSupportUnit::MakeTrack() {MakeGroundTrack(X, Y, Orient);}










/////////////////////////////////////////////////////////////////////////
// pozemni jednotky (normalni)


// Perseus A:

void TPerseusAlfa::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utPA_FUEL;
    MaxHitPoints = utPA_HP;
    MaxTimeUnits = utPA_TU;
    Defense[0] = utPA_DN0;
    Defense[1] = utPA_DN1;
    Defense[2] = utPA_DN2;
    Visib = utPA_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TCannon(WpnNames[WPNNM_CANNON], 
                     utPA_AN, utPA_ANB, utPA_AT, utPA_MAXR, utPA_AMMO);
    Velocity = utPA_VEL;
}

void TPerseusAlfa::IncLevel(int alevel)
{
    MaxHitPoints = utPA_HP + alevel * utPA_INC_HP;
    Defense[0] = utPA_DN0 + alevel * utPA_INC_DN;
    Defense[1] = utPA_DN1 + alevel * utPA_INC_DN;
    Defense[2] = utPA_DN2 + alevel * utPA_INC_DN;
    Weapons[0]->IncAN(utPA_AN + alevel * utPA_INC_AN - Weapons[0]->AttackNum);
}



// Perseus B:

void TPerseusBeta::Setup()
{
    TGroundToweredUnit::Setup();
    MaxFuel = utPB_FUEL;
    MaxHitPoints = utPB_HP;
    MaxTimeUnits = utPB_TU;
    Defense[0] = utPB_DN0;
    Defense[1] = utPB_DN1;
    Defense[2] = utPB_DN2;
    Visib = utPB_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TCannon(WpnNames[WPNNM_CANNON], 
                     utPB_AN, utPB_ANB, utPB_AT, utPB_MAXR, utPB_AMMO);
    Velocity = utPB_VEL;
}

void TPerseusBeta::IncLevel(int alevel)
{
    MaxHitPoints = utPB_HP + alevel * utPB_INC_HP;
    Defense[0] = utPB_DN0 + alevel * utPB_INC_DN;
    Defense[1] = utPB_DN1 + alevel * utPB_INC_DN;
    Defense[2] = utPB_DN2 + alevel * utPB_INC_DN;
    Weapons[0]->IncAN(utPB_AN + alevel * utPB_INC_AN - Weapons[0]->AttackNum);
}



// Perseus ?:

void TPerseusGama::Setup()
{
    TGroundToweredUnit::Setup();
    MaxFuel = utPG_FUEL;
    MaxHitPoints = utPG_HP;
    MaxTimeUnits = utPG_TU;
    Defense[0] = utPG_DN0;
    Defense[1] = utPG_DN1;
    Defense[2] = utPG_DN2;
    Visib = utPG_VIS;
    CurWpn = 0;
    WeaponsCnt = 2;
    Weapons[0] = new TCannon(WpnNames[WPNNM_CANNON], 
                     utPG_AN, utPG_ANB, utPG_AT, utPG_MAXR, utPG_AMMO);
    Weapons[1] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
                     utPG_2AN, utPG_2ANB, utPG_2AT, utPG_2MAXR, utPG_2AMMO);
    Velocity = utPG_VEL;
}

void TPerseusGama::IncLevel(int alevel)
{
    MaxHitPoints = utPG_HP + alevel * utPG_INC_HP;
    Defense[0] = utPG_DN0 + alevel * utPG_INC_DN;
    Defense[1] = utPG_DN1 + alevel * utPG_INC_DN;
    Defense[2] = utPG_DN2 + alevel * utPG_INC_DN;
    Weapons[0]->IncAN(utPG_AN + alevel * utPG_INC_AN - Weapons[0]->AttackNum);
    Weapons[1]->IncAN(utPG_2AN + alevel * utPG_INC_2AN - Weapons[1]->AttackNum);
}



unsigned TPerseusGama::GetSupportedActions()
{
    unsigned sa = uatMove | uatSelect | uatSelectAir | uatLoadIn;
    if (CurWpn == 0) sa |= uatAttack | uatAttackGood | uatAttackField;
    else if (CurWpn == 1) sa |= uatAttackAir | uatAttackAirG;
    return sa;
}



void TPerseusGama::ShowShootAt(int x, int y, int phase)
{
    if (GetField(X, Y)->Terrain >= tofsL1B) return;

    if ((phase == 0) && (CurWpn == 1)) {}
    else 
        TGroundToweredUnit::ShowShootAt(x, y, phase);
}

void TPerseusGama::WpnRotate(int x, int y)
{
    if (CurWpn == 0) TGroundToweredUnit::WpnRotate(x, y);
}





// Golias:

void TGolias::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utGO_FUEL;
    MaxHitPoints = utGO_HP;
    MaxTimeUnits = utGO_TU;
    Defense[0] = utGO_DN0;
    Defense[1] = utGO_DN1;
    Defense[2] = utGO_DN2;
    Visib = utGO_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TBalisticArtefactWeapon(WpnNames[WPNNM_CANNON], 
                     utGO_AN, utGO_ANB, utGO_AN2, utGO_ANB2, utGO_AT, 
                     utGO_MINR, utGO_MAXR, utGO_AMMO);
    Velocity = utGO_VEL;
}

void TGolias::IncLevel(int alevel)
{
    MaxHitPoints = utGO_HP + alevel * utGO_INC_HP;
    Defense[0] = utGO_DN0 + alevel * utGO_INC_DN;
    Defense[1] = utGO_DN1 + alevel * utGO_INC_DN;
    Defense[2] = utGO_DN2 + alevel * utGO_INC_DN;
    Weapons[0]->IncAN(utGO_AN + alevel * utGO_INC_AN - Weapons[0]->AttackNum);
}





// Patriot:

void TPatriot::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utPT_FUEL;
    MaxHitPoints = utPT_HP;
    MaxTimeUnits = utPT_TU;
    Defense[0] = utPT_DN0;
    Defense[1] = utPT_DN1;
    Defense[2] = utPT_DN2;
    Visib = utPT_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TBalisticWeapon(WpnNames[WPNNM_CANNON], 
                     utPT_AN, utPT_ANB, utPT_AN2, utPT_ANB2, utPT_AT,
                     utPT_MINR, utPT_MAXR, utPT_AMMO);
    Velocity = utPT_VEL;
}

void TPatriot::IncLevel(int alevel)
{
    MaxHitPoints = utPT_HP + alevel * utPT_INC_HP;
    Defense[0] = utPT_DN0 + alevel * utPT_INC_DN;
    Defense[1] = utPT_DN1 + alevel * utPT_INC_DN;
    Defense[2] = utPT_DN2 + alevel * utPT_INC_DN;
    Weapons[0]->IncAN(utPT_AN + alevel * utPT_INC_AN - Weapons[0]->AttackNum);
}





// Herkules:

void THerkules::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utHE_FUEL;
    MaxHitPoints = utHE_HP;
    MaxTimeUnits = utHE_TU;
    Defense[0] = utHE_DN0;
    Defense[1] = utHE_DN1;
    Defense[2] = utHE_DN2;
    Visib = utHE_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TRocket(WpnNames[WPNNM_ROCKETS],
                     utHE_AN, utHE_ANB, utHE_AN2, utHE_ANB2, utHE_AT, 
                     utHE_MINR, utHE_MAXR, utHE_AMMO);
    Velocity = utHE_VEL;
}

void THerkules::IncLevel(int alevel)
{
    MaxHitPoints = utHE_HP + alevel * utHE_INC_HP;
    Defense[0] = utHE_DN0 + alevel * utHE_INC_DN;
    Defense[1] = utHE_DN1 + alevel * utHE_INC_DN;
    Defense[2] = utHE_DN2 + alevel * utHE_INC_DN;
    Weapons[0]->IncAN(utHE_AN + alevel * utHE_INC_AN - Weapons[0]->AttackNum);
}





// Skorpion:

void TSkorpion::Setup()
{
    TGroundToweredUnit::Setup();
    MaxFuel = utSK_FUEL;
    MaxHitPoints = utSK_HP;
    MaxTimeUnits = utSK_TU;
    Defense[0] = utSK_DN0;
    Defense[1] = utSK_DN1;
    Defense[2] = utSK_DN2;
    Visib = utSK_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TRocket(WpnNames[WPNNM_ROCKETS],
                     utSK_AN, utSK_ANB, utSK_AN2, utSK_ANB2, utSK_AT, 
                     utSK_MINR, utSK_MAXR, utSK_AMMO);
    Velocity = utSK_VEL;
}

void TSkorpion::IncLevel(int alevel)
{
    MaxHitPoints = utSK_HP + alevel * utSK_INC_HP;
    Defense[0] = utSK_DN0 + alevel * utSK_INC_DN;
    Defense[1] = utSK_DN1 + alevel * utSK_INC_DN;
    Defense[2] = utSK_DN2 + alevel * utSK_INC_DN;
    Weapons[0]->IncAN(utSK_AN + alevel * utSK_INC_AN - Weapons[0]->AttackNum);
}



void TSkorpion::ShowShootAt(int x, int y, int phase)
{
    int oldwo;

    if (phase == 0) {
        HideHelpers();
        WpnRotate(x, y);
        oldwo = WpnOrient;
        WpnOrient = oldwo - 16;    /*odkaz na jiny sprite- hlaven v zakluzu*/
        PaintUnit();
        SDL_Delay(iniAnimDelay2);   
        WpnOrient = oldwo - 8;
        PaintUnit();
        SDL_Delay(iniAnimDelay2);   
        WpnOrient = oldwo - 16;
        PaintUnit();
        SDL_Delay(iniAnimDelay2);   
        WpnOrient = oldwo;
        ShowHelpers();
    }
    else {
        HideHelpers();
        PaintUnit();
        SDL_Delay(iniAnimDelay2);   
        ShowHelpers();
    }
}





// Bumerang:

void TBumerang::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utBM_FUEL;
    MaxHitPoints = utBM_HP;
    MaxTimeUnits = utBM_TU;
    Defense[0] = utBM_DN0;
    Defense[1] = utBM_DN1;
    Defense[2] = utBM_DN2;
    Visib = utBM_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TGun(WpnNames[WPNNM_MACHGUN], 
                     utBM_AN, utBM_ANB, utBM_AT, utBM_MAXR, utBM_AMMO);
    Velocity = utBM_VEL;
}

void TBumerang::IncLevel(int alevel)
{
    MaxHitPoints = utBM_HP + alevel * utBM_INC_HP;
    Defense[0] = utBM_DN0 + alevel * utBM_INC_DN;
    Defense[1] = utBM_DN1 + alevel * utBM_INC_DN;
    Defense[2] = utBM_DN2 + alevel * utBM_INC_DN;
    Weapons[0]->IncAN(utBM_AN + alevel * utBM_INC_AN - Weapons[0]->AttackNum);
}



void TBumerang::ShowShootAt(int x, int y, int phase)
{
}



unsigned TBumerang::GetSupportedActions()
{
    return TGroundUnit::GetSupportedActions() | uatOccupy;  
}



void TBumerang::Action(int x, int y)
{
    TGroundUnit::Action(x, y);

    switch (UnitActionType) {
        case uatOccupy :
                switch (OccupyBuilding((TBuilding*)Units[GetField(x, y)->Unit])) {
                    case -1 : Message(MSG_OUT_OF_TIME); break;
                    case -2 : Message(MSG_OBJECT_IN_WAY); break;
                }
                break;
    }
}



void TBumerang::GetCursor(int x, int y, int *cursor, int *selbold)
{
    TGroundUnit::GetCursor(x, y, cursor, selbold);

    switch (UnitActionType) {
        case uatOccupy : 
                *cursor = mcurLoadIn, *selbold = TimeUnits == MaxTimeUnits;
                break;
    }   
}





int GetMoney100percent(int party)  // zjisti pomer financi
{
    int i, total = 0;
    for (i = party; i < party + GOODLIFE_TOP; i++)
        if ((Units[i] != NULL) && (Units[i]->Type % BADLIFE >= unRadar))
            total += ((TBuilding*)Units[i])->MoneyIndex;
    return total;
}

int TBumerang::OccupyBuilding(TObject *b)
{
    int i, j, isth = FALSE;
    int delta;

    // prijeti: 
    if (TimeUnits != MaxTimeUnits) return -1;
    if (((ID < BADLIFE) && (b->ID < BADLIFE)) ||
        ((ID >= BADLIFE) && (b->ID >= BADLIFE))) return -3;
    for (i = X - 1; i <= X + 1; i++)
        for (j = Y - 1; j <= Y + 1; j++)
            if (GetField(i, j)->Unit == b->ID) {isth = TRUE; break;}
    if (!isth) return -2;
    TimeUnits = 0;
    
    // ziskani kreditu:
    if (((TBuilding*)b)->MoneyIndex != 0) {
        if (ID < BADLIFE) {
            delta = MoneyBadlife * ((TBuilding*)b)->MoneyIndex / GetMoney100percent(BADLIFE);
            MoneyGoodlife += delta;
            MoneyBadlife -= delta;
        }
        else {
            delta = MoneyGoodlife * ((TBuilding*)b)->MoneyIndex / GetMoney100percent(GOODLIFE);
            MoneyGoodlife -= delta;
            MoneyBadlife += delta;
        }
    }
    
    // vlastni zmena
    b->ChangeParty();
    b->DoVisib();
    RedrawMap();
    Select();   
    return TRUE;
}







/////////////////////////////////////////////////////////////////////////
// podpurne jednotky



// Ceres:

void TCeres::Setup()
{
    TGroundSupportUnit::Setup();
    MaxFuel = utCR_FUEL;
    MaxHitPoints = utCR_HP;
    MaxTimeUnits = utCR_TU;
    Defense[0] = utCR_DN0;
    Defense[1] = utCR_DN1;
    Defense[2] = utCR_DN2;
    Visib = utCR_VIS;
    Velocity = utCR_VEL;
    CurWpn = -1;
    WeaponsCnt = 0;
}



void TCeres::SupportUnit(TUnit *Unit)
{
    int delta = Unit->MaxFuel - Unit->Fuel;
    if (Unit->Type == unCeres) delta = (Unit->MaxFuel - Unit->Fuel) / 10;
    if (delta > this->Fuel) delta =  this->Fuel;
    Fuel -= delta;
    Unit->Fuel += delta;
    if (ID < BADLIFE) SaySpeech("refuel", 90);
}



int TCeres::WillSupport(TUnit *Unit)
{
    return Unit->Fuel < Unit->MaxFuel;
}





// Gnom:

void TGnom::Setup()
{
    TGroundSupportUnit::Setup();
    MaxFuel = utGN_FUEL;
    MaxHitPoints = utGN_HP;
    MaxTimeUnits = utGN_TU;
    Defense[0] = utGN_DN0;
    Defense[1] = utGN_DN1;
    Defense[2] = utGN_DN2;
    Visib = utGN_VIS;
    Velocity = utGN_VEL;
    CurWpn = -1;
    WeaponsCnt = 0;
    MaxAmmo[1] = utGN_AMGN, MaxAmmo[2] = utGN_AMEX,
    MaxAmmo[0] = utGN_AMEN, MaxAmmo[3] = utGN_AMMI;
}

void TGnom::AfterSetup()
{
    TGroundSupportUnit::AfterSetup();
    for (int i = 0; i < 4; i++) Ammo[i] = MaxAmmo[i];
}



void TGnom::Read(FILE *f)
{
    TGroundSupportUnit::Read(f);
    for (int i = 0; i < 4; i++) {
        fread(&Ammo[i], 4, 1, f);
        fread(&MaxAmmo[i], 4, 1, f);
    }
}

void TGnom::Write(FILE *f)
{
    TGroundSupportUnit::Write(f);
    for (int i = 0; i < 4; i++) {
        fwrite(&Ammo[i], 4, 1, f);
        fwrite(&MaxAmmo[i], 4, 1, f);
    }
}



void TGnom::GetUnitInfo()
{
    char cbuf[30];
    int i, clr;

    TGroundSupportUnit::GetUnitInfo();  

    for (i = 0; i <4; i++) {
        sprintf(cbuf, "%i/%i", Ammo[i], MaxAmmo[i]);
        PercentBar(UInfoBuf, UINFO_SX, 34, 80+i*20, 72, 13, clrLightBlue2, clrSeaBlue, (double)Ammo[i] / MaxAmmo[i], cbuf);
        CopyBmpNZ(UInfoBuf, UINFO_SX, 2, 80+i*20, BmpAmmoIcons[i], 30, 13);
    }
}



void TGnom::SupportUnit(TUnit *Unit)
{
    int i, delta, typ;
    
    if (Unit->Type == unXenon) {
        TXenon *xu = (TXenon*)Unit;
        delta = utXE_MINES - xu->Mines;
        if (delta > Ammo[wpnMine]) delta = Ammo[wpnMine];
        if (!delta) return;
        Ammo[wpnMine] -= delta;
        xu->Mines += delta;
        if (ID < BADLIFE) SaySpeech("reammo", 90);
    }
    else {
        if (ID < BADLIFE) SaySpeech("reammo", 90);
        for (i = 0; i < Unit->WeaponsCnt; i++) {
            typ = Unit->Weapons[i]->GetType();
            delta = Unit->Weapons[i]->MaxAmmo - Unit->Weapons[i]->Ammo;
            if (delta > Ammo[typ]) delta = Ammo[typ];
            if (!delta) return;
            Ammo[typ] -= delta;
            Unit->Weapons[i]->Ammo += delta;
        }
    }
}



int TGnom::WillSupport(TUnit *Unit)
{
    if ((Unit->Type == unXenon) && (((TXenon*)Unit)->Mines < utXE_MINES)) return TRUE;
    if (Unit->WeaponsCnt == 0) return FALSE;
    for (int i = 0; i < Unit->WeaponsCnt; i++)
        if (Unit->Weapons[i]->Ammo < Unit->Weapons[i]->MaxAmmo) return TRUE;
    return FALSE;
}








// Iris:

void TIris::Setup()
{
    TGroundSupportUnit::Setup();
    MaxFuel = utIR_FUEL;
    MaxHitPoints = utIR_HP;
    MaxTimeUnits = utIR_TU;
    Defense[0] = utIR_DN0;
    Defense[1] = utIR_DN1;
    Defense[2] = utIR_DN2;
    Visib = utIR_VIS;
    Velocity = utIR_VEL;
    CurWpn = -1;
    WeaponsCnt = 0;
}



void TIris::SupportUnit(TUnit *Unit)
{
    if (Unit->HitPoints <= Unit->MaxHitPoints / 2)
        Unit->HitPoints = Unit->MaxHitPoints / 2;
    else 
        Unit->HitPoints = Unit->MaxHitPoints;
    Unit->PaintUnit(FALSE);
    if (ID < BADLIFE) SaySpeech("repair", 90);
}



int TIris::WillSupport(TUnit *Unit)
{
    return (Unit->HitPoints != Unit->MaxHitPoints / 2) &&
           (Unit->HitPoints != Unit->MaxHitPoints);
}











/////////////////////////////////////////////////////////////////////////
// specialni jednotky


// Satan:

void TSatan::Init(int x, int y, int party, FILE *f)
{
    TGroundUnit::Init(x, y, party, f);
    if (IconSatan == NULL) 
        IconSatan = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "icsatan%i", 13);
}



void TSatan::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utST_FUEL;
    MaxHitPoints = utST_HP;
    MaxTimeUnits = utST_TU;
    Defense[0] = utST_DN0;
    Defense[1] = utST_DN1;
    Defense[2] = utST_DN2;
    Visib = utST_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TMine(WpnNames[WPNNM_MINES], 
                     utST_AN, utST_ANB, utST_AN2, utST_ANB2, utST_AT,
                     utST_MINR, utST_MAXR, utST_AMMO);
    DestruAtN1 = utST_DAN1, DestruBoN1 = utST_DANB1;
    DestruAtN2 = utST_DAN2, DestruBoN2 = utST_DANB2;
    Velocity = utST_VEL;
}

void TSatan::IncLevel(int alevel)
{
    Defense[0] = utST_DN0 + alevel * utST_INC_DN;
    Defense[1] = utST_DN1 + alevel * utST_INC_DN;
    Defense[2] = utST_DN2 + alevel * utST_INC_DN;
    Weapons[0]->IncAN(utST_AN + alevel * utST_INC_AN - Weapons[0]->AttackNum);
    DestruAtN1 = utST_DAN1 + alevel * utST_INC_DAN;
    DestruAtN2 = utST_DAN2 + alevel * utST_INC_DAN;
}



TSprite *TSatan::GetSprite()
{
    if (ActualSprite < 8) return UnitsSprites[Type][0];
    if (ActualSprite < 16) return UnitsSprites[Type][ActualSprite-7];
    if (ActualSprite < 24) return UnitsSprites[Type][ActualSprite-7-8];
    return UnitsSprites[Type][0];
}



void TSatan::GetUnitInfo()
{
    char cbuf[30];
    
    TGroundUnit::GetUnitInfo();
    CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconSatan->IconPic[0], 59, 59);

    CopyBmpNZ(UInfoBuf, UINFO_SX, 2, 129, BmpAmmoIcons[Weapons[CurWpn]->GetType()], 30, 13);
    sprintf(cbuf, "%i", Weapons[CurWpn]->TimeLost);
    PutStr(UInfoBuf, UINFO_SX, 35, 129, cbuf, NormalFont, clrWhite, clrBlack);     
}



void TSatan::GlobalDestruction()
{
    HideHelpers();
    HitPoints = 0;
    Destroy();
    DoExplosion();
    ShowHelpers();
}



int TSatan::InfoEvent(TEvent *e)
{
    int rt = TGroundUnit::InfoEvent(e);
    
    if (!rt & IconSatan->Handle(e)) {
        GlobalDestruction();
        return TRUE;
    }
    return rt;
}



void TSatan::Explode()
{
    int i, j, k;
    
    Center();
    DrawFieldSafe2(X, Y);

    AddExplode1x1(X, Y);
    {
        for (k = 0; k < 1; k++) {
            for (i = X - 3; i <= X + 3; i++) 
                for (j = Y - 3; j <= Y + 3; j++) 
                    AddExplode3x3(i, j, 0, 
                        20 - 40 * rand() / RAND_MAX, 20 - 40 * rand() / RAND_MAX, TRUE);
            IncExplodeTime(+2);
        }
    }

    MegaDestruction = TRUE;
    for (i = X - 2; i <= X + 2; i++)
        for (j = Y - 2; j <= Y + 2; j++) 
            if ((i != X) || (j != Y)) 
                WeaponAttack(i, j, wpnExplos, DestruAtN1, DestruBoN1);
    for (i = X - 3; i <= X + 3; i++) {
        WeaponAttack(i, Y - 3, wpnExplos, DestruAtN2, DestruBoN2);
        WeaponAttack(i, Y + 3, wpnExplos, DestruAtN2, DestruBoN2);
    }
    for (j = Y - 2; j <= Y + 2; j++) {
        WeaponAttack(X - 3, j, wpnExplos, DestruAtN2, DestruBoN2);
        WeaponAttack(X + 3, j, wpnExplos, DestruAtN2, DestruBoN2);
    }
    MegaDestruction = FALSE;
    PlaySample(GetUnitSound(6), 16, EffectsVolume, GetFieldPanning(X, Y));
}



void TSatan::Read(FILE *f)
{
    TGroundUnit::Read(f);

    fread(&DestruAtN1, 4, 1, f);
    fread(&DestruBoN1, 4, 1, f);
    fread(&DestruAtN2, 4, 1, f);
    fread(&DestruBoN2, 4, 1, f);
}

void TSatan::Write(FILE *f)
{
    TGroundUnit::Write(f);

    fwrite(&DestruAtN1, 4, 1, f);
    fwrite(&DestruBoN1, 4, 1, f);
    fwrite(&DestruAtN2, 4, 1, f);
    fwrite(&DestruBoN2, 4, 1, f);
}











// Artefakt. transporter Gargantua:

void TGargantua::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utGR_FUEL;
    MaxHitPoints = utGR_HP;
    MaxTimeUnits = utGR_TU;
    Defense[0] = utGR_DN0;
    Defense[1] = utGR_DN1;
    Defense[2] = utGR_DN2;
    Visib = utGR_VIS;
    CurWpn = -1;
    WeaponsCnt = 0;
    Velocity = utGR_VEL;
}



void TGargantua::IncLevel(int alevel)
{
}



void TGargantua::GetUnitInfo()
{
    TGroundUnit::GetUnitInfo();
    PutStr(UInfoBuf, UINFO_SX, 5, 100, SigText[TXT_ARTIFACT_TRANS], NormalFont, clrWhite, clrBlack);
}



void TGargantua::Explode()
{
    int i, j, k;
    
    Center();
    DrawFieldSafe2(X, Y);

    AddExplode1x1(X, Y);
    {
        for (i = X - 5; i <= X + 5; i++) 
            for (j = Y - 5; j <= Y + 5; j++) 
                    AddExplode3x3(i, j, 0, 
                        20 - 40 * rand() / RAND_MAX, 20 - 40 * rand() / RAND_MAX, TRUE);
    }
    IncExplodeTime(+3);
    MegaDestruction = TRUE;
    for (i = X - 5; i <= X + 5; i++)
        for (j = Y - 5; j <= Y + 5; j++) 
            if ((i != X) || (j != Y)) 
                WeaponAttack(i, j, wpnExplos, 666, +666);
    MegaDestruction = FALSE;
    PlaySample(GetUnitSound(6), 16, EffectsVolume, GetFieldPanning(X, Y));
}










// Xenon - miner

void TXenon::Init(int x, int y, int party, FILE *f)
{
    TGroundUnit::Init(x, y, party, f);
    if (IconXenon == NULL) {
        IconXenon = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "icxenon%i", 13);
        IconXenon2 = new TIcon(RES_X-116, UINFO_Y+147, 59, 59, "icxenob%i", 13);
    }
}



void TXenon::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utXE_FUEL;
    MaxHitPoints = utXE_HP;
    MaxTimeUnits = utXE_TU;
    Defense[0] = utXE_DN0;
    Defense[1] = utXE_DN1;
    Defense[2] = utXE_DN2;
    Mines = utXE_MINES;
    Visib = utXE_VIS;
    CurWpn = -1;
    WeaponsCnt = 0;
    Velocity = utXE_VEL;
}

void TXenon::IncLevel(int alevel)
{
}



void TXenon::GetUnitInfo()
{
    char cbuf[80];

    TGroundUnit::GetUnitInfo();
    PutStr(UInfoBuf, UINFO_SX, 2, 78, SigText[TXT_MINES_LEFT], NormalFont, clrWhite, clrBlack);

    sprintf(cbuf, "%i / %i", Mines, utXE_MINES);
    PercentBar(UInfoBuf, UINFO_SX, 54, 80, 52, 13, clrLightBlue2, clrSeaBlue, (double)Mines / utXE_MINES, cbuf);
    if (GetMineAt(X, Y) == -1)
        CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconXenon->IconPic[0], 59, 59);
    else
        CopyBmp(UInfoBuf, UINFO_SX, 2, 147, IconXenon2->IconPic[0], 59, 59);
}



int TXenon::PlaceMine()
{
    if (L1TerrainType[GetField(X, Y)->Terrain] != tofsL1A) return FALSE;
    if (Mines == 0) {
        if (ID < BADLIFE) Message(MSG_OUT_OF_AMMO);
        return FALSE;
    }
    if (TimeUnits < utXE_TL) {
        if (ID < BADLIFE) Message(MSG_OUT_OF_TIME);
        return FALSE;
    }
    if (GetMineAt(X, Y) != -1) return TRUE;
    if (ID < BADLIFE)   MinePlace(X, Y, GOODLIFE);
    else MinePlace(X, Y, BADLIFE);
    Mines--;
    TimeUnits -= utXE_TL;
    if (this == SelectedUnit) Select();
    return TRUE;
}



int TXenon::InfoEvent(TEvent *e)
{
    int rt = TGroundUnit::InfoEvent(e);
    
    if (GetMineAt(X, Y) == -1) {
        if (!rt & IconXenon->Handle(e)) {
            PlaceMine();
            return TRUE;
        }
    }
    else {
        if (!rt & IconXenon2->Handle(e)) {
            UnplaceMine();
            ShowUnitInfo();
            return TRUE;
        }
    }
    return rt;
}



void TXenon::UnplaceMine()
{
    MineRemove(X, Y);
    if (Mines < utXE_MINES) Mines++;
}



void TXenon::GoOnMine()
{
}



void TXenon::Read(FILE *f)
{
    TGroundUnit::Read(f);
    fread(&Mines, 4, 1, f);
}

void TXenon::Write(FILE *f)
{
    TGroundUnit::Write(f);
    fwrite(&Mines, 4, 1, f);
}








// Falanga:

void TFalanga::Setup()
{
    TGroundToweredUnit::Setup();
    MaxFuel = utFL_FUEL;
    MaxHitPoints = utFL_HP;
    MaxTimeUnits = utFL_TU;
    Defense[0] = utFL_DN0;
    Defense[1] = utFL_DN1;
    Defense[2] = utFL_DN2;
    Visib = utFL_VIS;
    CurWpn = 0;
    WeaponsCnt = 2;
    Weapons[0] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
                     utFL_AN, utFL_ANB, utFL_AT, utFL_MAXR, utFL_AMMO);
    Weapons[1] = new TGun(WpnNames[WPNNM_MACHGUN], 
                     utFL_2AN, utFL_2ANB, utFL_2AT, utFL_2MAXR, utFL_2AMMO);
    Velocity = utFL_VEL;
}



void TFalanga::IncLevel(int alevel)
{
    MaxHitPoints = utFL_HP + alevel * utFL_INC_HP;
    Defense[0] = utFL_DN0 + alevel * utFL_INC_DN;
    Defense[1] = utFL_DN1 + alevel * utFL_INC_DN;
    Defense[2] = utFL_DN2 + alevel * utFL_INC_DN;
    Weapons[0]->IncAN(utFL_AN + alevel * utFL_INC_AN - Weapons[0]->AttackNum);
    Weapons[1]->IncAN(utFL_2AN + alevel * utFL_INC_2AN - Weapons[1]->AttackNum);
}



unsigned TFalanga::GetSupportedActions()
{
    unsigned sa = uatMove | uatSelect | uatSelectAir | uatLoadIn;
    if (CurWpn == 1) sa |= uatAttack | uatAttackGood | uatAttackField;
    else if (CurWpn == 0) sa |= uatAttackAir | uatAttackAirG;
    return sa;
}










/////////////////////////////////
// Medea:

void TMedea::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utMD_FUEL;
    MaxHitPoints = utMD_HP;
    MaxTimeUnits = utMD_TU;
    Defense[0] = utMD_DN0;
    Defense[1] = utMD_DN1;
    Defense[2] = utMD_DN2;
    Visib = utMD_VIS;
    CurWpn = -1;
    WeaponsCnt = 0;
    Velocity = utMD_VEL;
}

void TMedea::IncLevel(int alevel) {}



unsigned TMedea::GetFieldActions(int x, int y)
{
    unsigned o = TGroundUnit::GetFieldActions(x, y);
    TField *f = GetField(x, y);
    
    if (((x - X) / 2 == 0) && ((y - Y) / 2 == 0)
       && (TimeUnits >= utMD_TIME) &&
       (!((f->Terrain2 != 0) && ((f->Terrain2 < 169)
        || ((f->Terrain2 > 188) && (f->Terrain2 < 389)) || 
        ((f->Terrain2 > 404))))) &&
       ((X == x) || (Y == y)/*pravouhlost*/)) o |= uatBuild;
    return o;
}



void TMedea::GetCursor(int x, int y, int *cursor, int *selbold)
{
    TGroundUnit::GetCursor(x, y, cursor, selbold);

    switch (UnitActionType) {
        case uatBuild : 
                *cursor = mcurBuild, *selbold = TRUE;
                break;
    }
}



void TMedea::Action(int x, int y)
{
    TGroundUnit::Action(x, y);

    switch (UnitActionType) {
        case uatBuild : 
                if (!BuildField(x, y)) Message(MSG_OUT_OF_TIME);
                break;
    }
}



static int WaterInNeighbour(int x, int y)
{
    int i, j;
    word t;
    
    for (i = x-1; i <= x+1; i++)
        for (j = y-1; j <= y+1; j++) {
            t = GetField(i, j)->Terrain;
            if ((t == 6) || (t == 18)) return TRUE;
        }
    return FALSE;
}

int TMedea::BuildField(int x, int y)
{
    TField *f = GetField(x, y);
    if (TimeUnits < utMD_TIME) return FALSE;
    if ((f->Terrain2 != 0) && ((f->Terrain2 < 169)
        || ((f->Terrain2 > 188) && (f->Terrain2 < 389)) || 
        ((f->Terrain2 > 404)))) return FALSE;
    
    // pontonove mosty:
    if (WaterInNeighbour(x, y)) {
        PlaceL2(x, y, 361);
    }
    // zeleznice:
    else {
        switch (L1TerrainType[f->Terrain]) {
            case tofsL1B : PlaceL2(x, y, 193); break;
            case tofsL1C : PlaceL2(x, y, 201); break;
            case tofsL1D : PlaceL2(x, y, 225); break;
            case tofsL1E : PlaceL2(x, y, 241); break;
            default:       {
                                         if (GetField(x, y-1)->Terrain2 == 72) PlaceL2(x, y, 72);
                                         if (GetField(x, y+1)->Terrain2 == 72) PlaceL2(x, y, 72);
                                         if (GetField(x-1, y)->Terrain2 == 73) PlaceL2(x, y, 73);
                                         if (GetField(x+1, y)->Terrain2 == 73) PlaceL2(x, y, 73);
                                         break;
                                         }
        }
    }
    
    TimeUnits -= utMD_TIME;
    RedrawMapLocks++;
    this->Select();
    RedrawMapLocks--;
    RedrawMap();
    return TRUE;
}










// Skiatron:

void TSkiatron::Setup()
{
    TGroundUnit::Setup();
    MaxFuel = utSI_FUEL;
    MaxHitPoints = utSI_HP;
    MaxTimeUnits = utSI_TU;
    Defense[0] = utSI_DN0;
    Defense[1] = utSI_DN1;
    Defense[2] = utSI_DN2;
    Visib = utSI_VIS;
    CurWpn = -1;
    WeaponsCnt = 0;
    Velocity = utSI_VEL;
}

void TSkiatron::IncLevel(int alevel)
{
}


int TSkiatron::DoVisib()
{
    return ProcessRadarVisib(X, Y, Visib, ID);
}






// Dirrac:

void TDirrac::Setup()
{
    TGroundToweredUnit::Setup();
    MaxFuel = utDI_FUEL;
    MaxHitPoints = utDI_HP;
    MaxTimeUnits = utDI_TU;
    Defense[0] = utDI_DN0;
    Defense[1] = utDI_DN1;
    Defense[2] = utDI_DN2;
    Visib = utDI_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TSonic(WpnNames[WPNNM_SONIC],
                     utDI_AN, utDI_ANB, utDI_AT, utDI_MAXR, utDI_AMMO);
    Velocity = utDI_VEL;
}

void TDirrac::IncLevel(int alevel)
{
    MaxHitPoints = utDI_HP + alevel * utDI_INC_HP;
    Defense[0] = utDI_DN0 + alevel * utDI_INC_DN;
    Defense[1] = utDI_DN1 + alevel * utDI_INC_DN;
    Defense[2] = utDI_DN2 + alevel * utDI_INC_DN;
    Weapons[0]->IncAN(utDI_AN + alevel * utDI_INC_AN - Weapons[0]->AttackNum);
}









// Agni:

void TAgni::Setup()
{
    TGroundToweredUnit::Setup();
    MaxFuel = utAG_FUEL;
    MaxHitPoints = utAG_HP;
    MaxTimeUnits = utAG_TU;
    Defense[0] = utAG_DN0;
    Defense[1] = utAG_DN1;
    Defense[2] = utAG_DN2;
    Visib = utAG_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TLaser(WpnNames[WPNNM_LASER], 
                     utAG_AN, utAG_ANB, utAG_AT, utAG_MAXR, utAG_AMMO);
    Velocity = utAG_VEL;
}

void TAgni::IncLevel(int alevel)
{
    MaxHitPoints = utAG_HP + alevel * utAG_INC_HP;
    Defense[0] = utAG_DN0 + alevel * utAG_INC_DN;
    Defense[1] = utAG_DN1 + alevel * utAG_INC_DN;
    Defense[2] = utAG_DN2 + alevel * utAG_INC_DN;
    Weapons[0]->IncAN(utAG_AN + alevel * utAG_INC_AN - Weapons[0]->AttackNum);
}

