/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002
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

Specialni osetreni nejruznejsich operaci nad policky - utoky na barely,
animace ohnu, davani novych terenu, ...

*/

#include "headers.h"
#include "fields.h"
#include "explode.h"
#include "weapons.h"
#include "untables.h"
#include "units.h"



int MegaDestruction = FALSE;



void MakeCrater(int x, int y, int crater_type)
{
    TField *f = GetField(x, y);
    
    if (MegaDestruction) {
        switch (f->Terrain2) {
            // hradby:
            case 317 : case 318 : case 319 : case 320 : case 321 : case 322 : 
            case 323 : case 324 : case 325 : case 326 : case 327 : 
            // mosty : 
            case 163 : case 164 : case 165 : case 166 : case 167 : case 168 : 
            case 157 : case 158 : case 159 : case 160 : case 161 : case 162 : 
            case 429 : case 430 : case 431 : case 432 : case 433 : case 434 :
            case 435 : case 436 : case 437 : case 438 : case 439 : case 440 :
            case 447 : case 448 : case 449 : case 450 : case 451 : case 452 :
            case 453 : case 454 : case 455 : case 456 : case 457 : case 458 :
            // zeleznice :
            case 72 : case 73 : case 74 : case 75 : case 76 : case 77 : 
            case 78 : case 79 : case 80 : case 81 : case 82 : case 83 : 
            case 84 : case 85 : case 86 : case 87 : case 88 : case 89 : 
            case 90 : case 91 : case 92 : case 193 : case 201 : case 225 :
            case 241 : 
            // ponton :
            case 361 : 
                GetField(x, y)->Terrain2 = 0;
                break;
            default : break;
        }
    }

    switch (f->Terrain2) {
        case terBarrel : case terBarrels : 
        case 316 : case 421 : //bedny
        case 294 : case 295 : case 296 : //zatarasy
        case 425 : case 426 : case 427 : case 428 : // zavory
        case 297 : case 298 : case 299 : case 300 : case 301 : 
        case 302 : case 303 : case 304 : case 305 : case 306 : case 307 : //ploty
            f->Terrain2 = 0;
            crater_type = cratCrater;
            break;
        // stromy, kroviska, atd:
        case 287 : case 288 : case 289 : case 290 : case 256 : case 257 : case 258 : case 259 :
        case 264 : case 265 : case 266 : case 267 : case 308 : case 309 : case 310 : case 311 :
            f->Terrain2 = 0;
            crater_type = cratCrater;
            RedrawMap();
            break;
    }
    if (crater_type == cratNothing) return; 
    if ((!MegaDestruction) && (f->Terrain2 != 0) /*&& (f->Terrain2 < 256)*/) return;
    if ((f->Terrain == 6) || (f->Terrain == 18)) return;
    if (((f->Terrain2 >= 51) && (f->Terrain2 <= 71)) ||
        (f->Terrain2 == 112) || (f->Terrain2 == 208) || (f->Terrain2 == 224) || (f->Terrain2 == 240)) return;
    if (L1TerrainType[f->Terrain] != tofsL1A) return;
    if (f->Unit != NO_UNIT) return;

label_1:
    if (crater_type == cratCrater) {
        switch (f->Terrain) {
            // trava:
            case 3 : case 12 : case 13 : case 316 : case 317 : case 318 : case 319 :
                    PlaceL2(x, y, 169 + 4 * rand() / RAND_MAX);
                    break;
            // pisek:
            case 1 : case 8 : case 9 : case 320 : case 321 : case 322 : case 323 :
                    PlaceL2(x, y, 181 + 4 * rand() / RAND_MAX);
                    break;
            // hlina:
            case 4 : case 14 : case 15 : case 330 : case 331 : case 332 : case 333 :
                    PlaceL2(x, y, 173 + 4 * rand() / RAND_MAX);
                    break;
            // mars:
            case 2 : case 10 : case 11 : case 326 : case 327 : case 328 : case 329 :
                    PlaceL2(x, y, 185 + 4 * rand() / RAND_MAX);
                    break;
            // snih:
            case 5 : case 16 : case 17 : case 334 : case 335 : case 336 : case 337 :
                    PlaceL2(x, y, 177 + 4 * rand() / RAND_MAX);
                    break;
            default : 
                    if (rand() % 2) PlaceL2(x, y, 393);
                    else PlaceL2(x, y, 401);
                    break;
        }
    }
    else if (crater_type == cratCrash) {
        switch (12 * rand() / RAND_MAX) {
            case 0 : PlaceL2(x, y, 393); break;
            case 1 : PlaceL2(x, y, 391, 1); break;
            case 2 : PlaceL2(x, y, 389, 1); break;
            case 3 : PlaceL2(x, y, 401); break;
            case 4 : PlaceL2(x, y, 398, 1); break;
            case 5 : PlaceL2(x, y, 395, 1); break;
            case 6 : case 7 : case 8 : case 9 : 
            case 10 : case 11 : 
                    crater_type = cratCrater;
                    goto label_1;
                    break;
            default : f->Terrain2 = 0; break;
        }
    }
}









static int CycleF(int i,
                  int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
    if (i == i8) return i1;
    if (i == i7) return i8;
    if (i == i6) return i7;
    if (i == i5) return i6;
    if (i == i4) return i5;
    if (i == i3) return i4;
    if (i == i2) return i3;
    if (i == i1) return i2;
    return i;
}


void ProcessFieldAnim(int x, int y)
{
    int oldt, t;
    TSprite *s;
    int rx, ry, drawx, drawy;
    
    t = oldt = GetField(x, y)->Terrain2;
    t = CycleF(t, 389, 390, 900, 901, 902, 903, 904, 905);
    t = CycleF(t, 391, 392, 906, 907, 908, 909, 910, 911);
    t = CycleF(t, 395, 396, 397, 912, 913, 914, 915, 916);
    t = CycleF(t, 398, 399, 400, 918, 919, 920, 921, 922);
    if (t != oldt) {
        PlaceL2(x, y, t, 1);
        s = (TSprite*)(BmpTerr2[GetField(x, y)->Terrain2]);
        if (s) {
            rx = GetRelX(x), ry = GetRelY(y);
            drawx = 28 * (rx - ry) + 28;
            drawy = 14 * (rx + ry - (GetField(x, y)->Height)) + 14;
            StartDraw(drawx - s->dx, drawy - s->dy, s->w, s->h);
            PaintUnits();
            EndDraw();
        }
    }
}







static int DegradeF(int f, int ft, 
                    int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
    if ((f == i1) || (f == i2) || (f == i3) || (f == i4) || 
        (f == i5) || (f == i6) || (f == i7) || (f == i8))
      return ft;
    else return f;
}

void FieldTurnEnd(int x, int y)
{
    int oldt, t;
    int anim = 1;
    
    t = oldt = GetField(x, y)->Terrain2;
    t = DegradeF(t, 401, 398, 399, 400, 918, 919, 920, 921, 922);
    t = DegradeF(t, 398, 395, 396, 397, 912, 913, 914, 915, 916);
    t = DegradeF(t, 393, 391, 392, 906, 907, 908, 909, 910, 911);
    t = DegradeF(t, 391, 389, 390, 900, 901, 902, 903, 904, 905);
    if (t != oldt) {
        if ((t == 401) || (t == 393)) anim = 0;
        PlaceL2(x, y, t, anim);
    }
}









////////////////////////////////////




#define MAX_BARREL_SEQ     64
static TPoint Barrels[MAX_BARREL_SEQ] = {{-1,-1}};
static int BarrelsPos = 0;


static int ExplodeBarrel(int X, int Y)
{
    static int called = 0;
    int dmg = 0;

    {
        if ((Barrels[0].x == -1) || 
            ((GetField(Barrels[0].x, Barrels[0].y)->Terrain2 != terBarrel) &&
             (GetField(Barrels[0].x, Barrels[0].y)->Terrain2 != terBarrels)))
            BarrelsPos = 0;
        for (int i = 0; i < BarrelsPos; i++)
            if ((Barrels[i].x == X) && (Barrels[i].y == Y)) return 0;
        Barrels[BarrelsPos].x = X, Barrels[BarrelsPos].y = Y;
        ++BarrelsPos;
    }
    called++;

    AddExplode3x3(X, Y, 0, 
            10 - 20 * rand() / RAND_MAX, 10 - 20 * rand() / RAND_MAX);
    IncExplodeTime(+1);

    dmg += WeaponAttack(X - 1, Y - 1, wpnExplos, utBARREL_DESTROY_AN, utBARREL_DESTROY_BN);
    dmg += WeaponAttack(X + 1, Y - 1, wpnExplos, utBARREL_DESTROY_AN, utBARREL_DESTROY_BN);
    dmg += WeaponAttack(X    , Y - 1, wpnExplos, utBARREL_DESTROY_AN, utBARREL_DESTROY_BN);
    dmg += WeaponAttack(X - 1, Y + 1, wpnExplos, utBARREL_DESTROY_AN, utBARREL_DESTROY_BN);
    dmg += WeaponAttack(X + 1, Y + 1, wpnExplos, utBARREL_DESTROY_AN, utBARREL_DESTROY_BN);
    dmg += WeaponAttack(X    , Y + 1, wpnExplos, utBARREL_DESTROY_AN, utBARREL_DESTROY_BN);
    dmg += WeaponAttack(X - 1, Y    , wpnExplos, utBARREL_DESTROY_AN, utBARREL_DESTROY_BN);
    dmg += WeaponAttack(X + 1, Y    , wpnExplos, utBARREL_DESTROY_AN, utBARREL_DESTROY_BN);

    if (--called == 0) Barrels[0].x = -1, BarrelsPos = 0;
    return dmg;
}





static void DestroyBridge(int x, int y)
{
    int i, j;

    AddExplode3x3(x, y, 0, 
            10 - 20 * rand() / RAND_MAX, 10 - 20 * rand() / RAND_MAX);
    GetField(x, y)->Terrain2 = 0;
    WeaponAttack(x, y, wpnExplos, 9999, 9999);
    
    for (i = x-1; i <= x+1; i++)
        for (j = y-1; j <= y+1; j++) {
            switch (GetField(i, j)->Terrain2) {
                // mosty : 
                case 163 : case 164 : case 165 : case 166 : case 167 : case 168 : 
                case 157 : case 158 : case 159 : case 160 : case 161 : case 162 : 
                case 429 : case 430 : case 431 : case 432 : case 433 : case 434 :
                case 435 : case 436 : case 437 : case 438 : case 439 : case 440 :
                case 447 : case 448 : case 449 : case 450 : case 451 : case 452 :
                case 453 : case 454 : case 455 : case 456 : case 457 : case 458 :
                    if (GetMineAt(i, j) != -1) MineRemove(i, j);
                    DestroyBridge(i, j);
                    break;
            }
        }           
}




extern TPoint AttackPoint;

int AttackField(int x, int y, int WpnType, int AtN, int BoN)
{
    TField *f = GetField(x, y);

    switch (f->Terrain2) {
        case terBarrel : case terBarrels :
            return ExplodeBarrel(x, y);
            break;
        // mosty : 
        case 163 : case 164 : case 165 : case 166 : case 167 : case 168 : 
        case 157 : case 158 : case 159 : case 160 : case 161 : case 162 : 
        case 429 : case 430 : case 431 : case 432 : case 433 : case 434 :
        case 435 : case 436 : case 437 : case 438 : case 439 : case 440 :
        case 447 : case 448 : case 449 : case 450 : case 451 : case 452 :
        case 453 : case 454 : case 455 : case 456 : case 457 : case 458 :
            if (MegaDestruction) {
                if (GetMineAt(x, y) != -1) MineRemove(x, y);
                DestroyBridge(x, y);
            }
            break;
            
        default : 
            break;
    }
    
    if ((WpnType == wpnExplos) &&
        ((AttackPoint.x == x) && (AttackPoint.y == y)) &&
        (GetMineAt(x, y) != -1)) MineExplode(x, y);
    
    return 0;
}










#ifdef DEBUG
extern int MisLoads[1024];
#endif

void PlaceL2(int x, int y, int terrain2, int anim)
{
    char b[20];
    
    GetField(x, y)->Terrain2 = terrain2;
    GetField(x, y)->IsAnim = anim;
#ifdef DEBUG
    if (MisLoads[terrain2]) {
        if (BmpTerr2[terrain2]) memfree(BmpTerr2[terrain2]); 
        if (BmpTerr2D[terrain2]) memfree(BmpTerr2D[terrain2]);
        MisLoads[terrain2] = FALSE;
    }
#endif

    if (BmpTerr2[terrain2] == NULL) {
        sprintf(b, "tr2n%i", terrain2);
        BmpTerr2[terrain2] = GraphicsDF->get(b);
        sprintf(b, "tr2d%i", terrain2);
        BmpTerr2D[terrain2] = GraphicsDF->get(b);
    }
#ifdef DEBUG
    {
            int i = terrain2;
            TSprite *s;
            char ds[20];
            
            if (BmpTerr2[i] == NULL) {
                MisLoads[i] = FALSE;
                BmpTerr2[i] = GraphicsDF->get("sprite");
                BmpTerr2D[i] = GraphicsDF->get("sprite");
                s = (TSprite*)BmpTerr2[i];
                sprintf(ds,"%i", i);
                PutStr(s->data, s->w, 0, 0, ds, NormalFont, clrWhite, clrBlack);
                s = (TSprite*)BmpTerr2D[i];
                if (s) {
                    sprintf(ds,"%i", i);
                    PutStr(s->data, s->w, 0, 0, ds, NormalFont, clrWhite, clrBlack);
                }
            }
    }
#endif
}









////////////////////////////// minova pole /////////////////////////

TMineRecord MinesList[MAX_MINES_CNT];
int MinesCnt = 0;

int flagUnitOnMine = FALSE;

void MinePlace(int x, int y, int party)
{
    if (GetMineAt(x, y) != -1) return;
    if (MinesCnt == MAX_MINES_CNT) return;
    MinesList[MinesCnt].x = x,
    MinesList[MinesCnt].y = y,
    MinesList[MinesCnt].party = party;
    MinesCnt++;
}



void MineRemove(int x, int y)
{
    if (GetMineAt(x, y) == -1) return;

    int i, j;
    
    for (i = 0; i < MinesCnt; i++)
        if ((MinesList[i].x == x) && (MinesList[i].y == y)) {
            for (j = i+1; j < MinesCnt; j++) MinesList[j-1] = MinesList[j];
            break;
        }
    MinesCnt--;
}



void MineExplode(int x, int y)
{
    if (GetMineAt(x, y) == -1) return;
    MineRemove(x, y);
    AddExplode3x3(x, y, 0, 0, 0, TRUE);

    AttackPoint.x = x, AttackPoint.y = y;
    MegaDestruction = TRUE;
    WeaponAttack(x, y, wpnExplos, utMINE_AN, utMINE_BN);
    MegaDestruction = FALSE;
    WeaponAttack(x-1, y-1, wpnExplos, utMINE_AN2, utMINE_BN2);
    WeaponAttack(x  , y-1, wpnExplos, utMINE_AN2, utMINE_BN2);
    WeaponAttack(x+1, y-1, wpnExplos, utMINE_AN2, utMINE_BN2);
    WeaponAttack(x-1, y+1, wpnExplos, utMINE_AN2, utMINE_BN2);
    WeaponAttack(x  , y+1, wpnExplos, utMINE_AN2, utMINE_BN2);
    WeaponAttack(x+1, y+1, wpnExplos, utMINE_AN2, utMINE_BN2);
    WeaponAttack(x-1, y  , wpnExplos, utMINE_AN2, utMINE_BN2);
    WeaponAttack(x+1, y  , wpnExplos, utMINE_AN2, utMINE_BN2);
}



int GetMineAt(int x, int y)
{
    for (int i = 0; i < MinesCnt; i++)
        if ((MinesList[i].x == x) && (MinesList[i].y == y)) 
            return MinesList[i].party;
    return -1;
}



int MineIsSeen(int x, int y, int by_which_side)
{   
    for (int i = by_which_side; i < by_which_side + GOODLIFE_TOP; i++)
        if ((Units[i] != NULL) &&
          ((Units[i]->Type % GOODLIFE_TOP) == unXenon) &&
        ((x - Units[i]->X) * (x - Units[i]->X) + 
             (y - Units[i]->Y) * (y - Units[i]->Y) < (utXE_VIS+1) * (utXE_VIS+1)))
            return TRUE;
    return FALSE;
}
