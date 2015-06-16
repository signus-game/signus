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
// Definice vsech zakladnich typu zbrani
//


#include <SDL_timer.h>

#include <math.h>
#include "global.h"
#include "weapons.h"

#include "units.h"
#include "explode.h"
#include "sound.h"
#include "fields.h"





TSprite *SpriteProjectil, *SpriteGrenade, *SpriteTorpedo, *SpriteLaser;
TSprite *SpriteRocket[11];


static int DrwBufX, DrwBufY, DrwBufW, DrwBufH, DrwBufOn = FALSE;
        // tyto promnene slouzi behem vykreslovani projektilu
static int DrawAlways = FALSE;

int ProcessingShoot = FALSE;
        // priznak pro kreslici rutiny, aby poznaly ze leti naboj


int InitWeapons()
{
    SpriteProjectil = (TSprite *) GraphicsDF->get("prjectil");
    SpriteGrenade = (TSprite *) GraphicsDF->get("grenade");
    SpriteLaser = (TSprite *) GraphicsDF->get("laser");
    SpriteTorpedo = (TSprite *) GraphicsDF->get("torpedo");
    LoadArray((void**)SpriteRocket, 11, GraphicsDF, "rocket%i");
    return TRUE;
}


int DoneWeapons()
{
    memfree(SpriteProjectil); memfree(SpriteGrenade);
    memfree(SpriteTorpedo); memfree(SpriteLaser); 
    DisposeArray((void**)SpriteRocket, 11);
    return TRUE;
}












int WeaponAttack(int x, int y, int WpnType, int AtN, int BoN)
{
    if ((x < 0) || (y < 0) || (x >= MapSizeX) || (y >= MapSizeY)) return 0;
    
    TField *f = GetField(x, y);
    int dmg = 0;
    
    if (f->Unit != NO_UNIT) 
        dmg = Units[f->Unit]->Damage(WpnType, AtN, BoN);
    dmg += AttackField(x, y, WpnType, AtN, BoN);

    return dmg;
}






////////////////////////// Obecny predek: TWEAPON //////////////////////////


static TUnit *TheShooter = NULL;
        // pomocna prom. - ukazuje na toho, kdo v danem okamziku prave strili
        // (ja vim, moc objektovosti to nepobralo...)
static int IgnoreUnits = FALSE;
        // AI pry potrebuje nekdy neuvazovat jedntoky
TPoint AttackPoint;
        // misto, kam se strili (tj. ne vedlejsi efekty.)
extern MIDASsamplePlayHandle       ShootSound;
        // zvuk vystrelu




TWeapon::TWeapon(char *aname,
                 int an, int bn, int tl, 
                 int minr, int maxr, 
                 int maxam)
{
    Name = aname;
    AttackNum = an; BonusNum = bn;
    TimeLost = tl;
    MinRange = minr; MaxRange = maxr;
    MaxAmmo = maxam;
    Ammo = MaxAmmo;
}



static inline double Pythagoras(double x, double y)
{
    return sqrt(x * x + y * y);
}


int TWeapon::IsInRange(void *shooter, int fromx, int fromy, int x, int y)
{
    TheShooter = (TUnit*)shooter;

    int vzdalenost = Pythagoras(abs(fromx - x), abs(fromy - y));
    int ax, ay;
    
    if ((GetField(fromx, fromy)->Height == TheShooter->Alt) &&
        (L1TerrainType[GetField(fromx, fromy)->Terrain] != tofsL1A) &&
        //test na ty, co muzou strilet na svahu:
        ((GetType() != unPerseusAlfa) && (GetType() != unPerseusBeta) && (GetType() != unPerseusGama) &&
         (GetType() != unBumerang) && (GetType() != unFalanga) && (GetType() != unDirrac) &&
         (GetType() != unAgni))
        ) return FALSE;
    if (!((vzdalenost <= MaxRange) && (vzdalenost >= MinRange))) return FALSE;
    else {
        AnimProcess(fromx, fromy, x, y, &ax, &ay, FALSE);
        return (ax == x) && (ay == y);
    }
}



int TWeapon::IsInRangeAI(void *shooter, int fromx, int fromy, int x, int y, int ignore_units)
{
    int rt;
    TUnit *shoter = (TUnit*)shooter;
    int sho_alt = shoter->Alt;
    
    if (shoter->Alt == GetField(shoter->X, shoter->Y)->Height)
        shoter->Alt = GetField(fromx, fromy)->Height;
    IgnoreUnits = ignore_units;
    rt = IsInRange(shooter, fromx, fromy, x, y);
    IgnoreUnits = FALSE;
    shoter->Alt = sho_alt;
    return rt;
}



int TWeapon::ShootResult(int x, int y)
{
    return WeaponAttack(x, y, GetType(), AttackNum, BonusNum);
}



int TWeapon::Shoot(void *shooter, int fromx, int fromy, int x, int y)
{
    int tarx, tary;
    
    TheShooter = (TUnit*)shooter;
    AttackPoint.x = x, AttackPoint.y = y;
    if (!IsInRange(shooter, fromx, fromy, x, y)) return -1; 
    if (Ammo == 0) return -1;   
    Ammo--;

    AnimStart(fromx, fromy, x, y);
    AnimProcess(fromx, fromy, x, y, &tarx, &tary);
    AnimStop(fromx, fromy, tarx, tary);
    return ShootResult(tarx, tary);
}






#define WANI_STEP    4
                       
void TWeapon::AnimProcess(int fromx, int fromy, int tox, int toy,
                                int *resx, int *resy, int draw)
{
    int x1 = fromx * 28 + 14, y1 = fromy * 28 + 14, 
        x2 = tox * 28 + 14, y2 = toy * 28 + 14;
    int x = x1, y = y1, h, fih;
    int length = Pythagoras(abs(x2 - x1), abs(y2 - y1));
    int steps = length / WANI_STEP;
    int actpos = 0, actstep = 0;
    int anistar = AnimFlyFrom(length) / WANI_STEP;
    TField *f;
    
    DrawAlways = GetField(fromx, fromy)->Visib == 2;
    ProcessingShoot = TRUE;
    for (actstep = 1; actstep < steps; actstep++) {
        actpos = length * actstep / steps;
        x = x1 + (x2 - x1) * actstep / steps;
        y = y1 + (y2 - y1) * actstep / steps;
        if ((x / 28 == fromx) && (y / 28 == fromy)) continue;
        
        h = AnimGetHeight(fromx, fromy, tox, toy, length, actpos);
        f = GetField(x / 28, y / 28);
        if (!CanShootThrough(x / 28, y / 28)) break;
        if (f->Visib == 2) DrawAlways = TRUE;
        fih = f->Height + TabHghtModif[f->Terrain2];
        if (L1TerrainType[f->Terrain] >= tofsL1B) fih--;
        if ((f->Unit != NO_UNIT) && (f->Unit != TheShooter->ID) && (!IgnoreUnits)) fih++;
        if ((h / 14) < fih) break;
        if ((x / 28 == tox) && (y / 28 == toy)) break;
        
        if (draw && (actstep >= anistar)) {
            PanSample(ShootSound, GetFieldPanning(x / 28, y / 28));
            ProcessMapAnim();
            AnimDraw(x, y, h);
            GetEvent(NULL);
            SDL_Delay(AnimGetDelay());
        }
    }
    ProcessingShoot = FALSE;
    *resx = x / 28, *resy = y / 28;
}


int TWeapon::CanShootThrough(int x, int y)
{
    return TRUE;
}






void TWeapon::Read(FILE *f)
{
    fread(&AttackNum, 4, 1, f);
    fread(&BonusNum, 4, 1, f);
    fread(&Ammo, 4, 1, f);
}



void TWeapon::Write(FILE *f)
{
    fwrite(&AttackNum, 4, 1, f);
    fwrite(&BonusNum, 4, 1, f);
    fwrite(&Ammo, 4, 1, f);
}



void TWeapon::IncAN(int delta)
{
    AttackNum += delta;
}














////////////////////////////////////////////////////////////////////////////

/////////////////// TGun ---------------------


TGun::TGun(char *aname, int an, int bn, int tl, int maxr, int maxam) :
            TWeapon(aname, an, bn, tl, 1, maxr, maxam) {}



void TGun::AnimStart(int fromx, int fromy, int tox, int toy)
{
    TheShooter->ShowShootAt(tox, toy, 0);
}

void TGun::AnimStop(int fromx, int fromy, int tox, int toy)
{
    TheShooter->ShowShootAt(tox, toy, 1);
}







/////////////////// TBomb ---------------------


TBomb::TBomb(char *aname, int an, int bn, int an2, int bn2, int tl, int maxam) :
            TWeapon(aname, an, bn, tl, 0, 20, maxam)
{
    AttackNum2 = an2;
    BonusNum2 = bn2;
}


void TBomb::AnimStart(int fromx, int fromy, int tox, int toy)
{
    TheShooter->ShowShootAt(tox, toy, 0);
}

void TBomb::AnimStop(int fromx, int fromy, int tox, int toy)
{
    for (int i = 0; i < 6; i++)
        AddExplode1x1(tox, toy, 0, 20 - 40 * rand()/RAND_MAX, 20 - 40 * rand()/RAND_MAX);
    IncExplodeTime(+3);
    TheShooter->ShowShootAt(tox, toy, 1);
}


void TBomb::Read(FILE *f)
{
    TWeapon::Read(f);
    fread(&AttackNum2, 4, 1, f);
    fread(&BonusNum2, 4, 1, f);
}


void TBomb::Write(FILE *f)
{
    TWeapon::Write(f);
    fwrite(&AttackNum2, 4, 1, f);
    fwrite(&BonusNum2, 4, 1, f);
}


int TBomb::ShootResult(int x, int y)
{
    int dmg = 0;
    
    dmg += WeaponAttack(x  , y  , GetType(), AttackNum, BonusNum);
    dmg += WeaponAttack(x  , y-1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x  , y+1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x+1, y-1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x+1, y  , GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x+1, y+1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x-1, y-1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x-1, y  , GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x-1, y+1, GetType(), AttackNum2, BonusNum2);
    return dmg;
}



void TBomb::IncAN(int delta)
{
    AttackNum += delta;
    AttackNum2 += delta;
}



void TBomb::AnimProcess(int fromx, int fromy, int tox, int toy,
                                int *resx, int *resy, int draw)
{
    *resx = tox, *resy = toy;
}





/////////////////////// TProjectilWeapon /////////////////////////////////

TProjectilWeapon::TProjectilWeapon(char *aname, int an, int bn, int tl, int minr, int maxr, int maxam) :
            TWeapon(aname, an, bn, tl, minr, maxr, maxam) {}

int TProjectilWeapon::AnimGetHeight(int fromx, int fromy, int tox, int toy, 
                                    int path_length, int act_pos)
{
    int h1 = 14 * TheShooter->Alt + 7;
    int h2 = 14 * GetField(tox, toy)->Height + 7;

    return h1 + (h2 - h1) * act_pos / path_length;
}




void TProjectilWeapon::AnimStart(int fromx, int fromy, int tox, int toy)
{
    TheShooter->ShowShootAt(tox, toy, 0);
}




void TProjectilWeapon::AnimDraw(int x, int y, int h)
{
    int bx = x / 28, by = y / 28;
    int lx = x % 28, ly = y % 28;
    TField *f = GetField(bx, by);
    int rrx = GetRelX(bx), rry = GetRelY(by);
    int drawx = 28 * (rrx - rry) + 28 + (lx - ly);
    int drawy = 14 * (rrx + rry) + 14 + ((lx + ly) / 2 - 14) - h;
    TSprite *s = AnimGetSprite();
    void *buf;
    int ddx, ddy, ddw, ddh;
    
    ddx = drawx - s->dx, ddy = drawy - s->dy;
    ddw = s->w, ddh = s->h;

    // test, jestli je sprite mimo screen:
    if ((ddx < 0) || (ddy < 0) || (ddx + ddw >= VIEW_SX) || (ddy + ddh >= VIEW_SY)) { 
        DrwBufOn = FALSE;
        if ((f->Visib == 2) || DrawAlways) {
            switch (iniResolution) {
                case SVGA_640x480  : ScrollTo(bx - 11, by - 3); break;
                case SVGA_800x600  : ScrollTo(bx - 15, by - 5); break;
                case SVGA_1024x768 : ScrollTo(bx - 19, by - 4); break;
            }       
            AnimDraw(x, y, h);
        }
        return;
    }

    if (DrwBufOn) Union2(&ddx, &ddy, &ddw, &ddh, DrwBufX, DrwBufY, DrwBufW, DrwBufH);
    DrwBufOn = FALSE;

    if (DrawAlways || (f->Visib == 2)) { // projektil je videt jen nad "sviticim" fieldem
        DrwBufX = drawx - s->dx, DrwBufY = drawy - s->dy;
        DrwBufW = s->w, DrwBufH = s->h;
        StartDraw(ddx, ddy, ddw, ddh);

        buf = memalloc(s->w * s->h);
        CopyFromBmp(MapBuf, VIEW_PIXSZ_X, VIEW_OFS_X + DrwBufX, VIEW_OFS_Y + DrwBufY, buf, DrwBufW, DrwBufH);

        PaintUnitsAbove(bx, by);
        DrawSprite(drawx, drawy, s);    
        PaintUnitsBelow(bx, by);
    
        EndDraw();
        CopyBmp(MapBuf, VIEW_PIXSZ_X, VIEW_OFS_X + DrwBufX, VIEW_OFS_Y + DrwBufY, buf, DrwBufW, DrwBufH);
        memfree(buf);
        DrwBufOn = TRUE;
    }
}



void TProjectilWeapon::AnimStop(int fromx, int fromy, int tox, int toy)
{
    StartDraw(DrwBufX, DrwBufY, DrwBufW, DrwBufH);
    PaintUnits();
    EndDraw();
    DrwBufOn = FALSE;
    TheShooter->ShowShootAt(tox, toy, 1);
}


TSprite *TProjectilWeapon::AnimGetSprite() {return SpriteProjectil;}





void TCannon::AnimStart(int fromx, int fromy, int tox, int toy)
{
    TProjectilWeapon::AnimStart(fromx, fromy, tox, toy);
}
        


TSprite *TTorpedo::AnimGetSprite() {return SpriteTorpedo;}

int TTorpedo::CanShootThrough(int x, int y)
{
    TField *f = GetField(x, y);
    if (f->Terrain == 6) return TRUE;
    else if (f->Terrain == 18) return TRUE;
    else return FALSE;
}





int TCannonAntiAir::AnimGetHeight(int fromx, int fromy, int tox, int toy, 
                                  int path_length, int act_pos)
{
    int h1 = 14 * TheShooter->Alt + 7;
    TAircraft *a = GetAircraftAt(tox, toy);
    int h2 = (a) ? (14 * a->Alt + 7) : (14 * 7 + 7);

    return h1 + (h2 - h1) * act_pos / path_length;
}



int TCannonAntiAir::ShootResult(int x, int y)
{
    int dmg = 0;
    TAircraft *a = GetAircraftAt(x, y);
    
    if ((x < 0) || (y < 0) || (x >= MapSizeX) || (y >= MapSizeY)) return 0;
    if (a != NULL)
        dmg += a->Damage(GetType(), AttackNum, BonusNum);
    return dmg;
}





// sonic:

void TSonic::AnimProcess(int fromx, int fromy, int tox, int toy,
                                int *resx, int *resy, int draw)
{
    if (!draw) TCannon::AnimProcess(fromx, fromy, tox, toy, resx, resy, draw);
    else *resx = tox, *resy = toy;
}






// laser:

void TLaser::AnimDraw(int x, int y, int h)
{
    int bx = x / 28, by = y / 28;
    int lx = x % 28, ly = y % 28;
    int rrx = GetRelX(bx), rry = GetRelY(by);
    int drawx = 28 * (rrx - rry) + 28 + (lx - ly);
    int drawy = 14 * (rrx + rry) + 14 + ((lx + ly) / 2 - 14) - h;
    TSprite *s = AnimGetSprite();
    void *buf;
    int ddx, ddy, ddw, ddh;
    
    ddx = drawx - s->dx, ddy = drawy - s->dy;
    ddw = s->w, ddh = s->h;

    DrwBufOn = FALSE;

/*  if (DrawAlways || (f->Visib == 2))*/ { // projektil je videt jen nad "sviticim" fieldem
        DrwBufX = drawx - s->dx, DrwBufY = drawy - s->dy;
        DrwBufW = s->w, DrwBufH = s->h;
        StartDraw(ddx, ddy, ddw, ddh);

        buf = memalloc(s->w * s->h);
        CopyFromBmp(MapBuf, VIEW_PIXSZ_X, VIEW_OFS_X + DrwBufX, VIEW_OFS_Y + DrwBufY, buf, DrwBufW, DrwBufH);

        PaintUnitsAbove(bx, by);
        DrawSprite(drawx, drawy, s);    
        PaintUnitsBelow(bx, by);
    
        EndDraw();
        CopyBmp(MapBuf, VIEW_PIXSZ_X, VIEW_OFS_X + DrwBufX, VIEW_OFS_Y + DrwBufY, buf, DrwBufW, DrwBufH);
        memfree(buf);
        DrwBufOn = TRUE;
    }
}



#define WANI_STEP2    2

void TLaser::AnimProcess(int fromx, int fromy, int tox, int toy,
                                int *resx, int *resy, int draw)
{
    int x1 = fromx * 28 + 14, y1 = fromy * 28 + 14, 
        x2 = tox * 28 + 14, y2 = toy * 28 + 14;
    int x = x1, y = y1, h, fih;
    int length = Pythagoras(abs(x2 - x1), abs(y2 - y1));
    int steps = length / WANI_STEP2;
    int actpos = 0, actstep = 0;
    int anistar = AnimFlyFrom(length) / WANI_STEP2;
    TField *f;
    
    DrawAlways = GetField(fromx, fromy)->Visib == 2;
    ProcessingShoot = TRUE;
    for (actstep = 1; actstep < steps; actstep++) {
        actpos = length * actstep / steps;
        x = x1 + (x2 - x1) * actstep / steps;
        y = y1 + (y2 - y1) * actstep / steps;
        if ((x / 28 == fromx) && (y / 28 == fromy)) continue;
        
        h = AnimGetHeight(fromx, fromy, tox, toy, length, actpos);
        f = GetField(x / 28, y / 28);
        if (!CanShootThrough(x / 28, y / 28)) break;
        if (f->Visib == 2) DrawAlways = TRUE;
        fih = f->Height + TabHghtModif[f->Terrain2];
        if (f->Terrain >= tofsL1B) fih--;
        if ((f->Unit != NO_UNIT) && (f->Unit != TheShooter->ID) && (!IgnoreUnits)) fih++;
        if ((h / 14) < fih) break;
        if ((x / 28 == tox) && (y / 28 == toy)) break;
        
        if (draw && (actstep >= anistar)) {
            AnimDraw(x, y, h);
        }
    }
    ProcessingShoot = FALSE;
    *resx = x / 28, *resy = y / 28;
}



void TLaser::AnimStop(int fromx, int fromy, int tox, int toy)
{
    RedrawMap();
    TCannon::AnimStop(fromx, fromy, tox, toy);
}



TSprite *TLaser::AnimGetSprite() {return SpriteLaser;}



int TLaser::AnimFlyFrom(int length)
{
    return 0;
}








/////////////////////// TBalisticWeapon /////////////////////////////////


TBalisticWeapon::TBalisticWeapon(char *aname, int an, int bn, int an2, int bn2, int tl, int minr, int maxr, int maxam) : 
                             TProjectilWeapon(aname, an, bn, tl, minr, maxr, maxam)
{
    AttackNum2 = an2;
    BonusNum2 = bn2;
}



TSprite *TBalisticWeapon::AnimGetSprite() {return SpriteGrenade;}

int TBalisticWeapon::AnimGetHeight(int fromx, int fromy, int tox, int toy, int path_length, int act_pos)
{
    int oldH = TProjectilWeapon::AnimGetHeight(fromx, fromy, tox, toy, path_length, act_pos);
    double parabola_pos = (2 * (double)act_pos / (double)path_length) - 1;  // <-1;1>
    int newH = GetAmplitude(path_length) * (1 - (parabola_pos * parabola_pos));
    return (oldH + newH);
}



void TBalisticWeapon::AnimStop(int fromx, int fromy, int tox, int toy)
{
    StartDraw(DrwBufX, DrwBufY, DrwBufW, DrwBufH);
    PaintUnits();
    EndDraw();
    DrwBufOn = FALSE;
    AddExplode3x3(tox, toy);
    IncExplodeTime(+3);
    TheShooter->ShowShootAt(tox, toy, 1);
}



int TBalisticWeapon::GetAmplitude(int path_length)
{
    return path_length / 6;
}


void TBalisticWeapon::Read(FILE *f)
{
    TWeapon::Read(f);
    fread(&AttackNum2, 4, 1, f);
    fread(&BonusNum2, 4, 1, f);
}



void TBalisticWeapon::Write(FILE *f)
{
    TWeapon::Write(f);
    fwrite(&AttackNum2, 4, 1, f);
    fwrite(&BonusNum2, 4, 1, f);
}



int TBalisticWeapon::ShootResult(int x, int y)
{
    int dmg = 0;
    
    dmg += WeaponAttack(x  , y  , GetType(), AttackNum, BonusNum);
    dmg += WeaponAttack(x  , y-1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x  , y+1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x+1, y-1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x+1, y  , GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x+1, y+1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x-1, y-1, GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x-1, y  , GetType(), AttackNum2, BonusNum2);
    dmg += WeaponAttack(x-1, y+1, GetType(), AttackNum2, BonusNum2);
    return dmg;
}



void TBalisticWeapon::IncAN(int delta)
{
    AttackNum += delta;
    AttackNum2 += delta;
}





void TBalisticArtefactWeapon::AnimStop(int fromx, int fromy, int tox, int toy)
{
    StartDraw(DrwBufX, DrwBufY, DrwBufW, DrwBufH);
    PaintUnits();
    EndDraw();
    DrwBufOn = FALSE;
    AddExplode3x3(tox, toy, 7/*special explosion*/);
    IncExplodeTime(+3);
    TheShooter->ShowShootAt(tox, toy, 1);
}








/////////////////// TRocket ---------------------


#define PI        3.14
#define RCH_LNG   3

static int RChvost[RCH_LNG][2];     // obrazove souradnice oblacku chvostu
static TSprite *RChvostSp[RCH_LNG]; // sprity jednotlivych oblacku
static int RChvostSz;               // pocet oblacku - max RCH_LNG
static int RChvostPh;               // Phaze chvostu - pouz. k dirkovani mezi obl.

void TRocket::AnimDraw(int x, int y, int h)
{
    static int olddrawx, olddrawy;

    int bx = x / 28, by = y / 28;
    int lx = x % 28, ly = y % 28;
    TField *f = GetField(bx, by);
    int rrx = GetRelX(bx), rry = GetRelY(by);
    int drawx = 28 * (rrx - rry) + 28 + (lx - ly);
    int drawy = 14 * (rrx + rry) + 14 + ((lx + ly) / 2 - 14) - h;
    TSprite *s = AnimGetSprite();
    void *buf;
    int ddx, ddy, ddw, ddh, oddx, oddy, oddw, oddh;
    int i;
    
    ddx = drawx - s->dx, ddy = drawy - s->dy;
    ddw = s->w, ddh = s->h;

    // veci kolem chvostu - udelani oblasti, ktera je kreslena:
    for (i = 0; i < RChvostSz; i++)
        Union2(&ddx, &ddy, &ddw, &ddh, 
               RChvost[i][0] - RChvostSp[i]->dx,
               RChvost[i][1] - RChvostSp[i]->dy,
               RChvostSp[i]->w, RChvostSp[i]->h);


    // test, jestli je sprite mimo screen:
    if ((ddx < 0) || (ddy < 0) || (ddx + ddw >= VIEW_SX) || (ddy + ddh >= VIEW_SY)) { 
        DrwBufOn = FALSE;
        RChvostSz = 0;
        if (DrawAlways || (f->Visib == 2)) {
            switch (iniResolution) {
                case SVGA_640x480  : ScrollTo(bx - 11, by - 3); break;
                case SVGA_800x600  : ScrollTo(bx - 15, by - 5); break;
                case SVGA_1024x768 : ScrollTo(bx - 19, by - 4); break;
            }       
            AnimDraw(x, y, h);
        }
        return;
    }

    oddx = ddx, oddy = ddy, oddw = ddw, oddh = ddh;
    if (DrwBufOn) Union2(&ddx, &ddy, &ddw, &ddh, DrwBufX, DrwBufY, DrwBufW, DrwBufH);
    DrwBufOn = FALSE;

    if (DrawAlways || (GetField(bx, by)->Visib == 2)) {
        DrwBufX = oddx, DrwBufY = oddy, DrwBufW = oddw, DrwBufH = oddh;
        StartDraw(ddx, ddy, ddw, ddh);
        buf = memalloc(oddw * oddh);
        CopyFromBmp(MapBuf, VIEW_PIXSZ_X, VIEW_OFS_X + DrwBufX, VIEW_OFS_Y + DrwBufY, buf, DrwBufW, DrwBufH);

        PaintUnitsAbove(bx, by);
        for (i = 0; i < RChvostSz; i++) 
            DrawSprite(RChvost[i][0], RChvost[i][1], RChvostSp[i]); 
        DrawSprite(drawx, drawy, s);        
        PaintUnitsBelow(bx, by);
            
        // pridani chvostovych oblacku:
        if (RChvostPh) {
            if (RChvostSz < RCH_LNG) RChvostSz++;
            else {
                for (i = 0; i < RChvostSz - 1; i++) {
                    RChvost[i][0] = RChvost[i+1][0], RChvost[i][1] = RChvost[i+1][1];
                    RChvostSp[i] = RChvostSp[i+1];
                }
            }
            i = RChvostSz - 1;
            RChvost[i][0] = olddrawx, RChvost[i][1] = olddrawy;
            RChvostSp[i] = SpriteRocket[8 + 3 * rand() / RAND_MAX];
        }
        RChvostPh = !RChvostPh;

        EndDraw();
        CopyBmp(MapBuf, VIEW_PIXSZ_X, VIEW_OFS_X + DrwBufX, VIEW_OFS_Y + DrwBufY, buf, DrwBufW, DrwBufH);
        memfree(buf);
        DrwBufOn = TRUE;
    }
    else {
        DrwBufOn = FALSE;
        RChvostSz = 0;
    }

    olddrawx = drawx, olddrawy = drawy;
}





void TRocket::AnimStart(int fromx, int fromy, int tox, int toy)
{
    double a;
    
    TBalisticWeapon::AnimStart(fromx, fromy, tox, toy);
    
    if ((tox == fromx) && (toy == fromy)) return;
    if (tox > fromx) {
        if (toy > fromy) a = PI * 3/2 - atan((double)(100*toy - 100*fromy) / (double)(100*tox - 100*fromx));
        else a = PI * 3/2 + atan((double)(100*fromy - 100*toy) / (double)(100*tox - 100*fromx));
    }
    else {
        if (toy > fromy) a = PI * 1/2 + atan((double)(100*toy - 100*fromy) / (double)(100*fromx - 100*tox));
        else a = PI * 1/2 - atan((double)(100*fromy - 100*toy) / (double)(100*fromx - 100*tox));
    }
    a -= PI/4;
    FlyDirection = roundnum(8 * a / (2*PI));
    if (FlyDirection < 0) FlyDirection = 8 + FlyDirection;
    if (FlyDirection >= 8) FlyDirection -= 8;
    RChvostSz = 0;
    RChvostPh = 0;
}


        
void TRocket::AnimStop(int fromx, int fromy, int tox, int toy)
{
    StartDraw(DrwBufX, DrwBufY, DrwBufW, DrwBufH);
    PaintUnits();
    EndDraw();
    DrwBufOn = FALSE;
    AddExplode3x3(tox, toy);
    IncExplodeTime(+3);
    TheShooter->ShowShootAt(tox, toy, 1);
}



TSprite *TRocket::AnimGetSprite()
{
    return SpriteRocket[FlyDirection];
}










int TRocketAntiAir::ShootResult(int x, int y)
{
    int dmg = 0;
    TAircraft *a = GetAircraftAt(x, y);
    
    if ((x < 0) || (y < 0) || (x >= MapSizeX) || (y >= MapSizeY)) return 0;
    if (a != NULL)
        dmg += a->Damage(GetType(), AttackNum, BonusNum);
    return dmg;
}



int TRocketAntiAir::AnimGetHeight(int fromx, int fromy, int tox, int toy, 
                                  int path_length, int act_pos)
{
    int h1 = 14 * TheShooter->Alt + 7;
    TAircraft *a = GetAircraftAt(tox, toy);
    int h2 = (a) ? (14 * a->Alt + 7) : (14 * 7 + 7);

    return h1 + (h2 - h1) * act_pos / path_length;
}


        
void TRocketAntiAir::AnimStop(int fromx, int fromy, int tox, int toy)
{
    TAircraft *a;
    
    StartDraw(DrwBufX, DrwBufY, DrwBufW, DrwBufH);
    PaintUnits();
    EndDraw();
    DrwBufOn = FALSE;
    a = GetAircraftAt(tox, toy);
    if (a) AddExplode3x3(tox, toy, 0, 0, -14 * (a->Alt - 1));
    else AddExplode3x3(tox, toy, 0, 0, -14 * 6);
    IncExplodeTime(+3);
    TheShooter->ShowShootAt(tox, toy, 1);
}
