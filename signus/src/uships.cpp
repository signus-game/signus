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

USHIPS.CPP - lode a lodicky


*/

#include <SDL_timer.h>

#include "uships.h"
#include "untables.h"
#include "fields.h"






void MakeWaterTrack(int X, int Y, int Orient)
{
}



void TShip::MakeTrack()
{
    MakeWaterTrack(X, Y, Orient);
}



void TShip::ShowShootAt(int x, int y, int phase)
{
}




void TShipTransporter::MakeTrack()
{
    MakeWaterTrack(X, Y, Orient);
}



void TShipTransporter::ShowShootAt(int x, int y, int phase)
{
}






///////////////////////// MALE LODICKY ////////////////////////////////////


// Flint:

void TFlint::Setup()
{
    TShip::Setup();
    MaxFuel = utFT_FUEL;
    MaxHitPoints = utFT_HP;
    MaxTimeUnits = utFT_TU;
    Defense[0] = utFT_DN0;
    Defense[1] = utFT_DN1;
    Defense[2] = utFT_DN2;
    Visib = utFT_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TCannon(WpnNames[WPNNM_CANNON], 
                     utFT_AN, utFT_ANB, utFT_AT, utFT_MAXR, utFT_AMMO);
    Velocity = utFT_VEL;
}

void TFlint::IncLevel(int alevel)
{
    MaxHitPoints = utFT_HP + alevel * utFT_INC_HP;
    Defense[0] = utFT_DN0 + alevel * utFT_INC_DN;
    Defense[1] = utFT_DN1 + alevel * utFT_INC_DN;
    Defense[2] = utFT_DN2 + alevel * utFT_INC_DN;
    Weapons[0]->IncAN(utFT_AN + alevel * utFT_INC_AN - Weapons[0]->AttackNum);
}









// Oasa:

void TOasa::Setup()
{
    TShipTransporter::Setup();
    MaxFuel = utOA_FUEL;
    MaxHitPoints = utOA_HP;
    MaxTimeUnits = utOA_TU;
    Defense[0] = utOA_DN0;
    Defense[1] = utOA_DN1;
    Defense[2] = utOA_DN2;
    Visib = utOA_VIS;
    CurWpn = -1;
    WeaponsCnt = 0;
    Velocity = utOA_VEL;
}

void TOasa::IncLevel(int alevel)
{
}



static int oasa_fnd;
inline int IsShore(int x, int y)
{
    if ((GetField(x, y)->Terrain != 6) && (GetField(x, y)->Terrain != 18) &&
        (GetField(x, y)->Terrain2 == 0)) {
      oasa_fnd = TRUE;
      return TRUE;
  }
  return FALSE;
}
inline void RemPonton(int x, int y)
{
    if (GetField(x, y)->Terrain2 == 361) 
        GetField(x, y)->Terrain2 = 0;
}


extern int EngineInited;

void TOasa::PlaceGround(int place)
{
    TShipTransporter::PlaceGround(place);
    if ((X < 1) || (Y < 1) || (X > MapSizeX-2) || (Y > MapSizeY-2)) return;
    oasa_fnd = FALSE;
    if (place) {
        if (IsShore(X-1, Y)) PlaceL2(X-1, Y, 361);
        if (IsShore(X+1, Y)) PlaceL2(X+1, Y, 361);
        if (IsShore(X, Y-1)) PlaceL2(X, Y-1, 361);
        if (IsShore(X, Y+1)) PlaceL2(X, Y+1, 361);
        if (IsShore(X-1, Y-1)) PlaceL2(X-1, Y-1, 361);
        if (IsShore(X+1, Y+1)) PlaceL2(X+1, Y+1, 361);
        if (IsShore(X-1, Y+1)) PlaceL2(X-1, Y+1, 361);
        if (IsShore(X+1, Y-1)) PlaceL2(X+1, Y-1, 361);
        if (oasa_fnd && EngineInited) RedrawMap();
    }
    else {
        RemPonton(X-1, Y);
        RemPonton(X+1, Y);
        RemPonton(X, Y-1);
        RemPonton(X, Y+1);
        RemPonton(X-1, Y-1);
        RemPonton(X+1, Y+1);
        RemPonton(X-1, Y+1);
        RemPonton(X+1, Y-1);
        if (oasa_fnd && EngineInited) RedrawMap();
    }
}







// Viking:

void TViking::Setup()
{
    TShip::Setup();
    MaxFuel = utVI_FUEL;
    MaxHitPoints = utVI_HP;
    MaxTimeUnits = utVI_TU;
    Defense[0] = utVI_DN0;
    Defense[1] = utVI_DN1;
    Defense[2] = utVI_DN2;
    Visib = utVI_VIS;
    CurWpn = 0;
    WeaponsCnt = 3;
    Weapons[0] = new TBalisticWeapon(WpnNames[WPNNM_CANNON], 
                     utVI_AN, utVI_ANB, utVI_AN2, utVI_ANB2, utVI_AT, 
                     utVI_MINR, utVI_MAXR, utVI_AMMO);
    Weapons[1] = new TGun(WpnNames[WPNNM_MINES],
                     utVI_2AN, utVI_2ANB, utVI_AT, utVI_2MAXR, utVI_2AMMO);
    Weapons[2] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
                     utVI_3AN, utVI_3ANB, utVI_3AT, utVI_3MAXR, utVI_3AMMO);
    Velocity = utVI_VEL;
}

void TViking::IncLevel(int alevel)
{
    MaxHitPoints = utVI_HP + alevel * utVI_INC_HP;
    Defense[0] = utVI_DN0 + alevel * utVI_INC_DN;
    Defense[1] = utVI_DN1 + alevel * utVI_INC_DN;
    Defense[2] = utVI_DN2 + alevel * utVI_INC_DN;
    Weapons[0]->IncAN(utVI_AN + alevel * utVI_INC_AN - Weapons[0]->AttackNum);
    Weapons[1]->IncAN(utVI_2AN + alevel * utVI_INC_2AN - Weapons[1]->AttackNum);
    Weapons[2]->IncAN(utVI_3AN + alevel * utVI_INC_3AN - Weapons[2]->AttackNum);
}



unsigned TViking::GetSupportedActions()
{
    unsigned sa = uatMove | uatSelect | uatSelectAir | uatLoadIn;
    if (CurWpn == 0) sa |= uatAttack | uatAttackGood | uatAttackField;
    if (CurWpn == 1) sa |= uatAttackSub;
    if (CurWpn == 2) sa |= uatAttackAir | uatAttackAirG;
    return sa;
}






// Barracuda:

void TBarracuda::Setup()
{
    TShip::Setup();
    MaxFuel = utBA_FUEL;
    MaxHitPoints = utBA_HP;
    MaxTimeUnits = utBA_TU;
    Defense[0] = utBA_DN0;
    Defense[1] = utBA_DN1;
    Defense[2] = utBA_DN2;
    Visib = utBA_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TTorpedo(WpnNames[WPNNM_TORPEDO], 
                     utBA_AN, utBA_ANB, utBA_AT, utBA_MAXR, utBA_AMMO);
    Velocity = utBA_VEL;
}

void TBarracuda::IncLevel(int alevel)
{
    MaxHitPoints = utBA_HP + alevel * utBA_INC_HP;
    Defense[0] = utBA_DN0 + alevel * utBA_INC_DN;
    Defense[1] = utBA_DN1 + alevel * utBA_INC_DN;
    Defense[2] = utBA_DN2 + alevel * utBA_INC_DN;
    Weapons[0]->IncAN(utBA_AN + alevel * utBA_INC_AN - Weapons[0]->AttackNum);
}



#include <math.h>
static inline double Pythagoras(double x, double y)
{
    return sqrt(x * x + y * y);
}

int TBarracuda::IsVisib()
{
    if (ID < BADLIFE) return TRUE;
    for (int i = GOODLIFE; i < GOODLIFE_TOP; i++)
        if ((Units[i]) && 
            ((Units[i]->Type == unViking) || (Units[i]->Type == unBarracuda)) &&
            (Pythagoras(Units[i]->X - X, Units[i]->Y - Y) <= Units[i]->Visib)) return TRUE;
    return FALSE;
}



void TBarracuda::Draw()
{
    if ((ID >= BADLIFE) && (!IsVisib())) return;
    else TShip::Draw();
}




unsigned TBarracuda::GetSupportedActions()
{
    unsigned sa = uatMove | uatSelect | uatSelectAir | uatLoadIn;
    if (CurWpn != -1) sa |= uatAttack | uatAttackSub | uatAttackGood | uatAttackField;
    return sa;
}



unsigned TBarracuda::GetAvailableActions()
{
    if (ID >= BADLIFE) {
        if ((GetField(X, Y)->Visib == 2) && IsVisib()) return uatAttackSub;
        else return uatNothing | uatMove;
    }
    else return uatSelect | uatAttackGood | uatSupport;
}



void RemoveSubmarines()
{
    for (int i = BADLIFE; i < UNITS_TOP; i++)
        if ((Units[i]) && (Units[i]->Type % BADLIFE == unBarracuda) &&
            (!((TBarracuda*)Units[i])->IsVisib())) {
          if (GetField(Units[i]->X, Units[i]->Y)->Unit == i)
                Units[i]->PlaceGround(FALSE);
        }
}



void PlaceSubmarines(int all)
{
    int ii = 0, oldx, oldy;
    TPoint p;
    
    for (int i = BADLIFE; i < UNITS_TOP; i++)
        if ((all && Units[i] && (Units[i]->Type % BADLIFE == unBarracuda)) || 
            ((Units[i]) && (Units[i]->Type % BADLIFE == unBarracuda) &&
            (((TBarracuda*)Units[i])->IsVisib()))) {
            oldx = Units[i]->X, oldy = Units[i]->Y;
            p.x=p.y=0;
            while ((GetField(oldx+p.x, oldy+p.y)->Unit != NO_UNIT) &&
                         (GetField(oldx+p.x, oldy+p.y)->Unit != i)) {
                p = CartezianSnail(ii++);
                Units[i]->X = oldx + p.x, Units[i]->Y = oldy + p.y;
            }
            Units[i]->PlaceGround(TRUE);
        }
}





////////////////////////////// VELKE LODE ////////////////////////////






// Poseidon:

void TPoseidon::Setup()
{
    TShip::Setup();
    MaxFuel = utPO_FUEL;
    MaxHitPoints = utPO_HP;
    MaxTimeUnits = utPO_TU;
    Defense[0] = utPO_DN0;
    Defense[1] = utPO_DN1;
    Defense[2] = utPO_DN2;
    Visib = utPO_VIS;
    CurWpn = 0;
    WeaponsCnt = 3;
    Weapons[0] = new TBalisticWeapon(WpnNames[WPNNM_CANNON], 
                     utPO_AN, utPO_ANB, utPO_AN2, utPO_ANB2, utPO_AT, 
                     utPO_MINR, utPO_MAXR, utPO_AMMO);
    Weapons[1] = new TTorpedo(WpnNames[WPNNM_TORPEDO], 
                     utPO_2AN, utPO_2ANB, utPO_2AT, utPO_2MAXR, utPO_2AMMO);
    Weapons[2] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
                     utPO_2AN, utPO_2ANB, utPO_2AT, utPO_2MAXR, utPO_2AMMO);
    Velocity = utPO_VEL;
}



unsigned TPoseidon::GetSupportedActions()
{
    unsigned sa = uatMove | uatSelect | uatSelectAir | uatLoadIn;
    if (CurWpn == 0) sa |= uatAttack | uatAttackGood | uatAttackField;
    if (CurWpn == 1) sa |= uatAttackSub | uatAttack | uatAttackGood | uatAttackField;
    if (CurWpn == 2) sa |= uatAttackAir;
    return sa;
}



void TPoseidon::IncLevel(int alevel)
{
    MaxHitPoints = utPO_HP + alevel * utPO_INC_HP;
    Defense[0] = utPO_DN0 + alevel * utPO_INC_DN;
    Defense[1] = utPO_DN1 + alevel * utPO_INC_DN;
    Defense[2] = utPO_DN2 + alevel * utPO_INC_DN;
    Weapons[0]->IncAN(utPO_AN + alevel * utPO_INC_AN - Weapons[0]->AttackNum);
    Weapons[1]->IncAN(utPO_2AN + alevel * utPO_INC_2AN - Weapons[1]->AttackNum);
    Weapons[2]->IncAN(utPO_3AN + alevel * utPO_INC_3AN - Weapons[2]->AttackNum);
}




static int ship_Xdir[8] = {-1, -1, -1,  0,  1,  1,  1,  0};
static int ship_Ydir[8] = {-1,  0,  1,  1,  1,  0,  -1, -1};

static int PlaceJustCenter = FALSE;

inline void shipPlace(TField *f, int ID)
{
    if (f->Unit == NO_UNIT) f->Unit = ID;
}
inline void shipUnplace(TField *f, int ID)
{
    if (f->Unit == ID) f->Unit = NO_UNIT;
}

void TPoseidon::PlaceGround(int place)
{
    if ((X == -1) && (Y == -1)) return; // v baraku nebo transporteru
    
    TField *f = GetField(X, Y);
    if (place) {
        f->Unit = ID;
        Alt = f->Height;
        if (IsAnimated()) f->IsAnim = 1; else f->IsAnim = 0;
        if (!PlaceJustCenter) {
            shipPlace(GetField(X + ship_Xdir[Orient], Y + ship_Ydir[Orient]), ID);
            shipPlace(GetField(X - ship_Xdir[Orient], Y - ship_Ydir[Orient]), ID);
            if (Orient % 2 == 0) {
                shipPlace(GetField(X-1, Y), ID); 
                shipPlace(GetField(X+1, Y), ID);
                shipPlace(GetField(X, Y-1), ID);
                shipPlace(GetField(X, Y+1), ID);
            }
        }
    }
    else {
        f->Unit = NO_UNIT;
        f->IsAnim = 0;
        if (!PlaceJustCenter) {
            shipUnplace(GetField(X + ship_Xdir[Orient], Y + ship_Ydir[Orient]), ID);
            shipUnplace(GetField(X - ship_Xdir[Orient], Y - ship_Ydir[Orient]), ID);
            if (Orient % 2 == 0) {
                shipUnplace(GetField(X-1, Y), ID); 
                shipUnplace(GetField(X+1, Y), ID);
                shipUnplace(GetField(X, Y-1), ID);
                shipUnplace(GetField(X, Y+1), ID);
            }
        }
    }
    GenerateUnitsList();    
}




extern int RottTbl[8][8];

void TPoseidon::RotateRel(int ra)
{
    PlaceGround(FALSE);
    Orient += ra;
    LittleX = 0, LittleY = 0;
    if (Orient < 0) Orient = 7;
    else if (Orient > 7) Orient = 0;
    ActualSprite = Orient;
    PlaceGround(TRUE);
    if (GetField(X, Y)->Visib == 2) {     
        PaintUnit();
        SDL_Delay(iniAnimDelay2);
    }
}


void TPoseidon::Rotate(int angle)
{
    if (angle == Orient) return;    
    PlaceGround(FALSE);
    PlaceJustCenter = TRUE;
    PlaceGround(TRUE);
    while (angle != Orient) 
        RotateRel(RottTbl[Orient][angle]);
    PlaceGround(FALSE);
    PlaceJustCenter = FALSE;
    PlaceGround(TRUE);
}









// Kraken:

void TKraken::Setup()
{
    TShip::Setup();
    MaxFuel = utKR_FUEL;
    MaxHitPoints = utKR_HP;
    MaxTimeUnits = utKR_TU;
    Defense[0] = utKR_DN0;
    Defense[1] = utKR_DN1;
    Defense[2] = utKR_DN2;
    Visib = utKR_VIS;
    CurWpn = 0;
    WeaponsCnt = 3;
    Weapons[0] = new TBalisticWeapon(WpnNames[WPNNM_CANNON], 
                     utKR_AN, utKR_ANB, utKR_AN2, utKR_ANB2, utKR_AT, 
                     utKR_MINR, utKR_MAXR, utKR_AMMO);
    Weapons[1] = new TTorpedo(WpnNames[WPNNM_TORPEDO], 
                     utKR_2AN, utKR_2ANB, utKR_2AT, utKR_2MAXR, utKR_2AMMO);
    Weapons[2] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
                     utKR_2AN, utKR_2ANB, utKR_2AT, utKR_2MAXR, utKR_2AMMO);
    Velocity = utKR_VEL;
}



unsigned TKraken::GetSupportedActions()
{
    unsigned sa = uatMove | uatSelect | uatSelectAir | uatLoadIn;
    if (CurWpn == 0) sa |= uatAttack | uatAttackGood | uatAttackField;
    if (CurWpn == 1) sa |= uatAttackSub | uatAttack | uatAttackGood | uatAttackField;
    if (CurWpn == 2) sa |= uatAttackAir | uatAttackAirG;
    return sa;
}



void TKraken::IncLevel(int alevel)
{
    MaxHitPoints = utKR_HP + alevel * utKR_INC_HP;
    Defense[0] = utKR_DN0 + alevel * utKR_INC_DN;
    Defense[1] = utKR_DN1 + alevel * utKR_INC_DN;
    Defense[2] = utKR_DN2 + alevel * utKR_INC_DN;
    Weapons[0]->IncAN(utKR_AN + alevel * utKR_INC_AN - Weapons[0]->AttackNum);
    Weapons[1]->IncAN(utKR_2AN + alevel * utKR_INC_2AN - Weapons[1]->AttackNum);
    Weapons[2]->IncAN(utKR_3AN + alevel * utKR_INC_3AN - Weapons[2]->AttackNum);
}



void TKraken::PlaceGround(int place)
{
    if ((X == -1) && (Y == -1)) return; // v baraku nebo transporteru
    
    TField *f = GetField(X, Y);
    if (place) {
        f->Unit = ID;
        Alt = f->Height;
        if (IsAnimated()) f->IsAnim = 1; else f->IsAnim = 0;
        if (!PlaceJustCenter) {
            shipPlace(GetField(X + ship_Xdir[Orient], Y + ship_Ydir[Orient]), ID);
            shipPlace(GetField(X - ship_Xdir[Orient], Y - ship_Ydir[Orient]), ID);
            shipPlace(GetField(X + 2*ship_Xdir[Orient], Y + 2*ship_Ydir[Orient]), ID);
            shipPlace(GetField(X - 2*ship_Xdir[Orient], Y - 2*ship_Ydir[Orient]), ID);
            if (Orient % 2 == 0) {
                int xX, yY;
                
                shipPlace(GetField(X-1, Y), ID); 
                shipPlace(GetField(X+1, Y), ID);
                shipPlace(GetField(X, Y-1), ID);
                shipPlace(GetField(X, Y+1), ID);
                xX = X + ship_Xdir[Orient], yY = Y + ship_Ydir[Orient];
                shipPlace(GetField(xX-1, yY), ID); 
                shipPlace(GetField(xX+1, yY), ID);
                shipPlace(GetField(xX, yY-1), ID);
                shipPlace(GetField(xX, yY+1), ID);
                xX = X - ship_Xdir[Orient], yY = Y - ship_Ydir[Orient];
                shipPlace(GetField(xX-1, yY), ID); 
                shipPlace(GetField(xX+1, yY), ID);
                shipPlace(GetField(xX, yY-1), ID);
                shipPlace(GetField(xX, yY+1), ID);
            }
        }
    }
    else {
        f->Unit = NO_UNIT;
        f->IsAnim = 0;
        if (!PlaceJustCenter) {
            shipUnplace(GetField(X + ship_Xdir[Orient], Y + ship_Ydir[Orient]), ID);
            shipUnplace(GetField(X - ship_Xdir[Orient], Y - ship_Ydir[Orient]), ID);
            shipUnplace(GetField(X + 2*ship_Xdir[Orient], Y + 2*ship_Ydir[Orient]), ID);
            shipUnplace(GetField(X - 2*ship_Xdir[Orient], Y - 2*ship_Ydir[Orient]), ID);
            if (Orient % 2 == 0) {
                int xX, yY;
                
                shipUnplace(GetField(X-1, Y), ID); 
                shipUnplace(GetField(X+1, Y), ID);
                shipUnplace(GetField(X, Y-1), ID);
                shipUnplace(GetField(X, Y+1), ID);
                xX = X + ship_Xdir[Orient], yY = Y + ship_Ydir[Orient];
                shipUnplace(GetField(xX-1, yY), ID); 
                shipUnplace(GetField(xX+1, yY), ID);
                shipUnplace(GetField(xX, yY-1), ID);
                shipUnplace(GetField(xX, yY+1), ID);
                xX = X - ship_Xdir[Orient], yY = Y - ship_Ydir[Orient];
                shipUnplace(GetField(xX-1, yY), ID); 
                shipUnplace(GetField(xX+1, yY), ID);
                shipUnplace(GetField(xX, yY-1), ID);
                shipUnplace(GetField(xX, yY+1), ID);
            }
        }
    }
    GenerateUnitsList();    
}




extern int RottTbl[8][8];

void TKraken::RotateRel(int ra)
{
    PlaceGround(FALSE);
    Orient += ra;
    LittleX = 0, LittleY = 0;
    if (Orient < 0) Orient = 7;
    else if (Orient > 7) Orient = 0;
    ActualSprite = Orient;
    PlaceGround(TRUE);
    if (GetField(X, Y)->Visib == 2) {     
        PaintUnit();
        SDL_Delay(iniAnimDelay2);
    }
}


void TKraken::Rotate(int angle)
{
    if (angle == Orient) return;    
    PlaceGround(FALSE);
    PlaceJustCenter = TRUE;
    PlaceGround(TRUE);
    while (angle != Orient) 
        RotateRel(RottTbl[Orient][angle]);
    PlaceGround(FALSE);
    PlaceJustCenter = FALSE;
    PlaceGround(TRUE);
}










// Laguna:

void TLaguna::Setup()
{
    TShipTransporter::Setup();
    MaxFuel = utLA_FUEL;
    MaxHitPoints = utLA_HP;
    MaxTimeUnits = utLA_TU;
    Defense[0] = utLA_DN0;
    Defense[1] = utLA_DN1;
    Defense[2] = utLA_DN2;
    Visib = utLA_VIS;
    CurWpn = 0;
    WeaponsCnt = 1;
    Weapons[0] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
                     utLA_AN, utLA_ANB, utLA_AT, utLA_MAXR, utLA_AMMO);
    Velocity = utLA_VEL;
}



void TLaguna::IncLevel(int alevel)
{
    MaxHitPoints = utLA_HP + alevel * utLA_INC_HP;
    Defense[0] = utLA_DN0 + alevel * utLA_INC_DN;
    Defense[1] = utLA_DN1 + alevel * utLA_INC_DN;
    Defense[2] = utLA_DN2 + alevel * utLA_INC_DN;
    Weapons[0]->IncAN(utLA_AN + alevel * utLA_INC_AN - Weapons[0]->AttackNum);
}



void TLaguna::PlaceGround(int place)
{
    if ((X == -1) && (Y == -1)) return; // v baraku nebo transporteru
    
    TField *f = GetField(X, Y);
    if (place) {
        f->Unit = ID;
        Alt = f->Height;
        if (IsAnimated()) f->IsAnim = 1; else f->IsAnim = 0;
        if (!PlaceJustCenter) {
            shipPlace(GetField(X + ship_Xdir[Orient], Y + ship_Ydir[Orient]), ID);
            shipPlace(GetField(X - ship_Xdir[Orient], Y - ship_Ydir[Orient]), ID);
            shipPlace(GetField(X + 2*ship_Xdir[Orient], Y + 2*ship_Ydir[Orient]), ID);
            shipPlace(GetField(X - 2*ship_Xdir[Orient], Y - 2*ship_Ydir[Orient]), ID);
            if (Orient % 2 == 0) {
                int xX, yY;
                
                shipPlace(GetField(X-1, Y), ID); 
                shipPlace(GetField(X+1, Y), ID);
                shipPlace(GetField(X, Y-1), ID);
                shipPlace(GetField(X, Y+1), ID);
                xX = X + ship_Xdir[Orient], yY = Y + ship_Ydir[Orient];
                shipPlace(GetField(xX-1, yY), ID); 
                shipPlace(GetField(xX+1, yY), ID);
                shipPlace(GetField(xX, yY-1), ID);
                shipPlace(GetField(xX, yY+1), ID);
                xX = X - ship_Xdir[Orient], yY = Y - ship_Ydir[Orient];
                shipPlace(GetField(xX-1, yY), ID); 
                shipPlace(GetField(xX+1, yY), ID);
                shipPlace(GetField(xX, yY-1), ID);
                shipPlace(GetField(xX, yY+1), ID);
            }
        }
    }
    else {
        f->Unit = NO_UNIT;
        f->IsAnim = 0;
        if (!PlaceJustCenter) {
            shipUnplace(GetField(X + ship_Xdir[Orient], Y + ship_Ydir[Orient]), ID);
            shipUnplace(GetField(X - ship_Xdir[Orient], Y - ship_Ydir[Orient]), ID);
            shipUnplace(GetField(X + 2*ship_Xdir[Orient], Y + 2*ship_Ydir[Orient]), ID);
            shipUnplace(GetField(X - 2*ship_Xdir[Orient], Y - 2*ship_Ydir[Orient]), ID);
            if (Orient % 2 == 0) {
                int xX, yY;
                
                shipUnplace(GetField(X-1, Y), ID); 
                shipUnplace(GetField(X+1, Y), ID);
                shipUnplace(GetField(X, Y-1), ID);
                shipUnplace(GetField(X, Y+1), ID);
                xX = X + ship_Xdir[Orient], yY = Y + ship_Ydir[Orient];
                shipUnplace(GetField(xX-1, yY), ID); 
                shipUnplace(GetField(xX+1, yY), ID);
                shipUnplace(GetField(xX, yY-1), ID);
                shipUnplace(GetField(xX, yY+1), ID);
                xX = X - ship_Xdir[Orient], yY = Y - ship_Ydir[Orient];
                shipUnplace(GetField(xX-1, yY), ID); 
                shipUnplace(GetField(xX+1, yY), ID);
                shipUnplace(GetField(xX, yY-1), ID);
                shipUnplace(GetField(xX, yY+1), ID);
            }
        }
    }
    GenerateUnitsList();    
}




extern int RottTbl[8][8];

void TLaguna::RotateRel(int ra)
{
    PlaceGround(FALSE);
    Orient += ra;
    LittleX = 0, LittleY = 0;
    if (Orient < 0) Orient = 7;
    else if (Orient > 7) Orient = 0;
    ActualSprite = Orient;
    PlaceGround(TRUE);
    if (GetField(X, Y)->Visib == 2) {     
        PaintUnit();
        SDL_Delay(iniAnimDelay2);
    }
}


void TLaguna::Rotate(int angle)
{
    if (angle == Orient) return;    
    PlaceGround(FALSE);
    PlaceJustCenter = TRUE;
    PlaceGround(TRUE);
    while (angle != Orient) 
        RotateRel(RottTbl[Orient][angle]);
    PlaceGround(FALSE);
    PlaceJustCenter = FALSE;
    PlaceGround(TRUE);
}




int TLaguna::LoadUnit(TUnit *u)
{
    int rt = TShipTransporter::LoadUnit(u);

    if (rt == TRUE) {
        u->Fuel = u->MaxFuel;
        for (int i = 0; i < u->WeaponsCnt; i++)
            u->Weapons[i]->Ammo = u->Weapons[i]->MaxAmmo;
        if (u->Type % BADLIFE == unSaturn) ((TSaturn*)u)->Bombs = utSA_BAMMO;
    }
    return rt;
}



void TLaguna::TurnReset()
{
    int i;
    TUnit *u;
    
    TShipTransporter::TurnReset();
    for (i = 0; i < LoadedUnits; i++) {
        u = (TUnit*) Units[Inventory[i]];
        if (u->HitPoints < u->MaxHitPoints/2) u->HitPoints = u->MaxHitPoints/2;
        else u->HitPoints = u->MaxHitPoints;
    }
}



unsigned TLaguna::GetAvailableActions()
{
    if (ID >= BADLIFE) {
        if (GetField(X, Y)->Visib == 2) return uatAttack | uatOccupy;
        else return uatNothing;
    }
    else return uatSelect | uatAttackGood | uatLandHelip | uatLandAircr;
}









