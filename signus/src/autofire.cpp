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


#include <cassert>

#include "autofire.h"
#include "units.h"
#include "visiblty.h"
#include "sound.h"
#include "mouse.h"


static word *AF_Map = NULL;
static int AF_Units[UNITS_TOP] = {0};
static int AF_from, AF_to;
static int AF_Reseted = FALSE;



#ifdef DEBUG
    
void ShowAutofireDebug(int x, int y, int drawx, int drawy)
{
    char b[20];
    if (!AF_Reseted) return;
    if (!AF_Map[x + MapSizeX * y]) return;
    sprintf(b, "%i", AF_Map[x + MapSizeX * y]);
    PutStr(MapBuf, VIEW_PIXSZ_X, drawx+20, drawy+5, b, NormalFont, 2, 1);
}
    
#endif




void SaveAutofire(FILE *f)
{
    fwrite(&AF_Reseted, 4, 1, f);
    fwrite(&AF_from, 4, 1, f);
    fwrite(&AF_to, 4, 1, f);
    fwrite(AF_Map, MapSizeX * MapSizeY * sizeof(word), 1, f);
    fwrite(AF_Units, UNITS_TOP * 4, 1, f);
}

void LoadAutofire(FILE *f)
{
    fread(&AF_Reseted, 4, 1, f);
    fread(&AF_from, 4, 1, f);
    fread(&AF_to, 4, 1, f);
    fread(AF_Map, MapSizeX * MapSizeY * sizeof(word), 1, f);
    fread(AF_Units, UNITS_TOP * 4, 1, f);
}




/////////////

void InitAutofire()
{
    if (!AF_Map) AF_Map = (word *) memalloc(MapSizeX * MapSizeY * sizeof(word));
    AF_Reseted = FALSE;
}



void DoneAutofire()
{
    AF_Reseted = FALSE;
    if (AF_Map) {
        memfree(AF_Map);
        AF_Map = NULL;
    }
}




///////////////////////////////////////////////////////////////////////////



int ChooseWeapon(int attackerID, int defenderID)
{
    TUnit *attacker;
    TObject *defender;
    
    attacker = (TUnit *)Units [attackerID];
    
    defender = Units [defenderID];
    
    switch (attacker->Type % BADLIFE) {
        case 1 : { //PerseusAlfa  OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // jine jednotky
            return -1;          
            break;
        }
        case 2 : { //PerseusBeta OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // jine jednotky
            return -1;          
            break;
        }
        case 3 : { // PerseusGama OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Letadla
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [1]->Ammo > 0
            && defender->Alt == 8) return 1;
            // jine jednotky
            return -1;          
            break;
        }
        case 4 : { //Dirrac OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // jine jednotky
            return -1;          
            break;
        }
        case 5 : { //Agni OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // jine jednotky
            return -1;          
            break;
        }
        case 6 : { //Skorpion OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // jine jednotky
            return -1;          
            break;
        }
        case 7 : { //Herkules OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // jine jednotky
            return -1;          
            break;
        }
        case 8 : { //Falanga OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [1]->Ammo > 0) return 1;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [1]->Ammo > 0) return 1;
            // letadla
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [0]->Ammo > 0
            && defender->Alt == 8) return 0;
            return -1;
            break;
        }
        case 9 : { //Golias OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 10 : { //Patriot OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 11 : { //Bumerang OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 12 : { //Skiatron
            return -1;
            break;
        }
        case 13 : { //Xenon
            return -1;
            break;
        }
        case 14 : { //Uran
            return -1;          
            break;
        }
        case 15 : { //Gragantua
            return -1;
            break;
        }
        case 16 : { //Medea
            return -1;
            break;
        }
        case 17 : { //Ceres
            return -1;
            break;
        }
        case 18 : { //Gnom
            return -1;
            break;
        }
        case 19 : { //Iris
            return -1;
            break;
        }
        case 20 : { //Horizont OK
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [0]->Ammo > 0
            && defender->Alt == 8) return 0;          
            return -1;
            break;
        }
        case 21 : { //Pagoda OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 22 : { //Minotaurus OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 23 : { //Thor OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna) 
                return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse) 
            return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 24 : { //Spektrum OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 25 : { //Neptun
            return -1;
            break;
        }
        case 26 : { //Helios
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Letadla
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [1]->Ammo > 0
            && defender->Alt == 8) return 1;
            return -1;
            break;
        }
        case 27 : { //Olymp
            return -1;
            break;
        }
        case 28 : { //Mohykan
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 29 : { //Mesias
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 30 : { //Ganymedes
            return -1;
            break;
        }
        case 31 : { //Flint
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;          
            break;
        }
        case 32 : { //Viking
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Barracuda - hlubinna mina
            if (defender->Type % BADLIFE == unBarracuda
            && attacker->Weapons [1]->Ammo > 0) return 1;
            // Letadla
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [2]->Ammo > 0
            && defender->Alt == 8) return 2;
            // jine jednotky
            return -1;
            break;
        }
        case 33 : { //Poseidon
            // Torpeda na lode krome Flinta
            if (defender->Type % BADLIFE >= unViking 
            && defender->Type % BADLIFE <= unBarracuda
            && attacker->Weapons [1]->Ammo > 0) return 1;
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse) return 0; 
            // Letadla
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [2]->Ammo > 0
            && defender->Alt == 8) return 2;
            // jine jednotky
            return -1;
            break;
        }
        case 34 : { //Kraken
            // Torpeda na lode krome Flinta
            if (defender->Type % BADLIFE >= unViking 
            && defender->Type % BADLIFE <= unBarracuda
            && attacker->Weapons [1]->Ammo > 0) return 1;
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [0]->Ammo > 0) return 0; 
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse) return 0; 
            // Letadla
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [2]->Ammo > 0
            && defender->Alt == 8) return 2;
            // jine jednotky
            return -1;
            break;
        }
        case 35 : { //Oasa
            return -1;
            break;
        }
        case 36 : { //Laguna
            // Letadla
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [0]->Ammo > 0
            && defender->Alt == 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 37 : { //Barakuda
            // Torpeda na lode krome Flinta
            if (defender->Type % BADLIFE >= unViking 
            && defender->Type % BADLIFE <= unBarracuda
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 38 : { //Ikaros
            // rakety
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [1]->Ammo > 0
            && defender->Alt == 8) return 1;
            // kulomet
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [0]->Ammo > 0
            && defender->Alt == 8) return 0;          
            // jine jednotky
            return -1;
            break;
        }
        case 39 : { //Kobra
            // kulomet AA
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [0]->Ammo > 0
            && defender->Alt == 8) return 0;
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku - bomby
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [2]->Ammo > 0) return 2; 
            // Satan a baraky - bomby
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [2]->Ammo > 0) return 2; 
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku - AGcan
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [1]->Ammo > 0) return 1; 
            // Satan a baraky - bomby
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [1]->Ammo > 0) return 1;
            // jine jednotky
            return -1;          
            break;
        }
        case 40 : { //Saturn
            return -1;
            break;
        }
        case 41 : { //Mystik
            // rakety AA
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [1]->Ammo > 0
            && defender->Alt == 8) return 1;
            // kulomet AA
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [0]->Ammo > 0
            && defender->Alt == 8) return 0;
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku - rakety AG
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna
            && attacker->Weapons [2]->Ammo > 0) return 2; 
            // Satan a baraky - bomby
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [2]->Ammo > 0) return 2; 
            // jine jednotky
            return -1;          

            break;
        }
        case 42 : { //Cesar
            return -1;
            break;
        }
        case 43 : { //Proton
            return -1;
            break;
        }
        case 44 : { //Rex
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // letadla
            if (defender->Type % BADLIFE >= unIkaros 
            && defender->Type % BADLIFE <= unRex
            && attacker->Weapons [1]->Ammo > 0
            && defender->Alt == 8) return 1;
            // jine jednotky
            return -1;
            break;
        }
        case 45 : { //Satan
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender->Type % BADLIFE >= unPerseusAlfa 
            && defender->Type % BADLIFE <= unLaguna 
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // Satan a baraky
            if (defender->Type % BADLIFE >= unSatan 
            && defender->Type % BADLIFE <= unWarehouse
            && attacker->Weapons [0]->Ammo > 0) return 0;
            // jine jednotky
            return -1;
            break;
        }
        default : {
            return -1;
        }
        
        
    }
}


























///////////////////////////////////////////////////////////////////////////


static int UnitCanShoot(TUnit *u)
{
    int i;

    if (u->Type % BADLIFE == unKobra) return FALSE;   
    if (u->Type % BADLIFE == unBarracuda) return FALSE;   
    if ((u->Type >= unIkaros) && (u->Type <= unRex)) {
        if (((TAircraft*)u)->FlyLevel == 0) return FALSE;
    }
    
    int cnt = u->WeaponsCnt;
    assert(cnt <= 4);
    
    for (i = 0; i < cnt; i++)
        if ((u->Weapons[i]->Ammo > 0) &&
            (u->Weapons[i]->TimeLost <= u->TimeUnits)) return TRUE;
    
    return FALSE;
}




static int PossibleAttack(TUnit *u, int x, int y)
{
    if ((AF_from == GOODLIFE) && (GetField(x, y)->Visib != 2)) return FALSE;
    if ((AF_from == BADLIFE) && (GetBadlifeVisib(x, y) != 2)) return FALSE;
    
    int cnt = u->WeaponsCnt;
    assert(cnt <= 4);
    
    for (int i = 0; i < cnt; i++)
        if ((u->Weapons[i]->Ammo > 0) &&
            (u->Weapons[i]->TimeLost <= u->TimeUnits) &&
            (u->Weapons[i]->IsInRange(u, u->X, u->Y, x, y))) return TRUE;
    return FALSE;   
}


static int GetUnitMaxRange(TUnit *u)
{
    int mr = 0;
    
    int cnt = u->WeaponsCnt;
    assert(cnt <= 4);
    
    for (int i = 0; i < cnt; i++) {
        if (u->Weapons[i]->MaxRange > mr)
            mr = u->Weapons[i]->MaxRange;
    }
    return mr;
}





static void AF_AddUnit(TUnit *u) 
{
    register int i, j;
    int m = GetUnitMaxRange(u);
    int x1 = u->X - m, x2 = u->X + m, 
        y1 = u->Y - m, y2 = u->Y + m;
        
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= MapSizeX) x2 = MapSizeX-1;
    if (y2 >= MapSizeY) y2 = MapSizeY-1;
    
    for (i = x1; i <= x2; i++)
        for (j = y1; j <= y2; j++) 
            if (PossibleAttack(u, i, j)) AF_Map[i + j * MapSizeX]++;
}


static void AF_RemoveUnit(TUnit *u) 
{
    register int i, j;
    int m = GetUnitMaxRange(u);
    int x1 = u->X - m, x2 = u->X + m, 
        y1 = u->Y - m, y2 = u->Y + m;
        
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= MapSizeX) x2 = MapSizeX-1;
    if (y2 >= MapSizeY) y2 = MapSizeY-1;
    
    for (i = x1; i <= x2; i++)
        for (j = y1; j <= y2; j++) 
            if (PossibleAttack(u, i, j)) AF_Map[i + j * MapSizeX]--;
}




void RemoveFromAutofire(int ID)
{
    if (!AF_Units[ID]) return;
    AF_RemoveUnit((TUnit*)Units[ID]);
}




void ResetAutofire(int party)
{
    int i, k, m;
    TUnit *u;
    
    AF_Reseted = TRUE;
    if (party == GOODLIFE) AF_from = BADLIFE, AF_to = UNITS_TOP;
    else AF_from = GOODLIFE, AF_to = GOODLIFE_TOP;
    for (i = 0; i < UNITS_TOP; i++) AF_Units[i] = FALSE;
    for (i = 0; i < MapSizeX * MapSizeY; i++)   AF_Map[i] = 0;
    
    for (i = AF_from; i < AF_to; i++)   {
        if ((Units[i] != NULL) && (Units[i]->Type % BADLIFE <= unSatan))
            AF_Units[i] = (Units[i]->X != -1) && UnitCanShoot((TUnit*)Units[i]);
    }   

    // vytvoreni tabulky strelby:
    for (m = 0, k = AF_from; k < AF_to; k++) if (AF_Units[k]) m++;
    if (m) ProgressLimit(m); else ProgressLimit(1);
    ProgressNull();
    for (k = AF_from; k < AF_to; k++) {
        if (!AF_Units[k]) continue;
        u = (TUnit*)Units[k];
        AF_AddUnit(u);
        ProgressInc();
        ProcessMapAnim();
    }
    ProgressNull();
}








int ChooseAttack(TUnit *u, TUnit *unit)
{
    int w;
    
    if (unit == NULL) return -1;
    
    w = ChooseWeapon(u->ID, unit->ID);
    if (w == -1) return w;
    if (u->Weapons[w]->Ammo <= 0) return -1;
    if (u->Weapons[w]->TimeLost > u->TimeUnits) return -1;
    if (!u->Weapons[w]->IsInRange(u, u->X, u->Y, unit->X, unit->Y)) return -1;
    return w;
}




int CanAutofireOn(TUnit *unit)
{
    int i, ca;
    TUnit *u;
    
    if (!AF_Reseted) return FALSE;
    if (!AF_Map[unit->X + MapSizeX * unit->Y]) return FALSE;
    if (unit->Type % BADLIFE == unSatan) return FALSE;

    if ((AF_from == GOODLIFE) && (GetField(unit->X, unit->Y)->Visib != 2)) return FALSE;
    if ((AF_from == BADLIFE) && (GetBadlifeVisib(unit->X, unit->Y) != 2)) return FALSE;

    for (i = 0; i < UNITS_TOP; i++) 
        if (AF_Units[i]) AF_Units[i] = (Units[i] != NULL) && (Units[i]->X != -1);
    for (i = AF_from; i < AF_to; i++)
        if (AF_Units[i]) {
            u = (TUnit*)Units[i];
            ca = ChooseAttack(u, unit);
            if (ca != -1) return TRUE;
        }
    return FALSE;
}




// pravdepodobnost dobreho zasahu podle urovne jednotky
static int RozptylTable[10] = {30, 40, 50, 60, 70, 75, 80, 85, 90, 95};



static TPoint GetRozptyl(int x, int y, TUnit *u)
{
    TPoint p;
    int i, cs;
    
    if ((100 * frand()) > RozptylTable[u->Level]) {
        cs = 1 + rand() % 8;
        p = CartezianSnail(cs);
        if (!u->Weapons[u->CurWpn]->IsInRange(u, u->X, u->Y, x+p.x, y+p.y)) {
            p.x = p.y = 0;
            for (i = 1; i < 9; i++) {
                if (i == cs) continue;
                p = CartezianSnail(i);
                if (u->Weapons[u->CurWpn]->IsInRange(u, u->X, u->Y, x+p.x, y+p.y)) 
                    break;
                else
                    p.x = p.y = 0;
            }
        }
    }
    else 
        p.x = p.y = 0;
    return p;
}





// potrvdi u hrace, ze chce utocit.
int ConAF_always = FALSE;
TUnit *ConAF_unit = NULL;
static int ConAF_used = FALSE;
static int ConAF_table[UNITS_TOP];

int ConfirmAF(TUnit *u, TUnit *t)
{
    char b[200];
    int cm;

    if (ConAF_always) return TRUE;
    if (ConAF_table[u->ID]) return FALSE;
    switch (u->GetType()) {
        case unBumerang : case unPerseusAlfa : case unPerseusBeta :
        case unFalanga : case unPagoda : case unFlint :
        case unHorizont :
            return TRUE; break;
    }

    sprintf(b, SigText[TXT_READYSHOOT], u->GetName(), t->GetName());
    MouseShow();
    ShowHelpers();
    ShowHelpers();
    cm = PromtBox(b, cmYes | cmNo | cmAlways);
    HideHelpers();
    HideHelpers();
    MouseHide();
    if (cm == cmAlways) ConAF_always = TRUE;
    if (cm == cmNo) ConAF_table[u->ID] = TRUE;
    ConAF_unit = t;
    ConAF_used = TRUE;
    return (cm != cmNo);
}




extern MIDASsamplePlayHandle     SampleMoving;

static int DAF_founds[GOODLIFE_TOP];
static int DAF_found_units = 0;

void DoAutofire(TUnit *unit)
{
    int cyc, i, ca, oldcw, oldtu, oldtu2;
    TPoint poz;
    TUnit *u;
    int su, uu;
       
    ConAF_used = FALSE;
    if (ConAF_unit != unit) memset(ConAF_table, 0, sizeof(ConAF_table));
    if (!AF_Reseted) return;
    for (i = 0; i < UNITS_TOP; i++) 
        if (AF_Units[i]) AF_Units[i] = (Units[i] != NULL) && (Units[i]->X != -1);

    DAF_found_units = 0;
    for (cyc = 0; cyc < 1680 /*snek pro r=20*/; cyc++) {
        poz = CartezianSnail(cyc);
        poz.x += unit->X, poz.y += unit->Y;
        if ((poz.x < 0) || (poz.y < 0) || (poz.x >= MapSizeX) || (poz.y >= MapSizeY)) continue;
        uu = GetField(poz.x, poz.y)->Unit;
        if ((uu != NO_UNIT) && (AF_Units[uu]))
            DAF_founds[DAF_found_units++] = uu;
        if (GetAircraftAt(poz.x, poz.y) != NULL) 
            DAF_founds[DAF_found_units++] = GetAircraftAt(poz.x, poz.y)->ID;
            
    }

    StopSample(SampleMoving);   SampleMoving = INVALID_SAMPLE_HANDLE;
    for (cyc = 0; cyc < DAF_found_units; cyc++) {
        i = DAF_founds[cyc];

        if (AF_Units[i]) {
            u = (TUnit*)Units[i];
            uu = unit->ID;
            ca = ChooseAttack(u, unit);
            if (ca != -1) {
                oldcw = u->CurWpn;
                oldtu = u->TimeUnits;
                u->CurWpn = ca;
                su = SelectedUnit->ID;
                u->Select();
                poz = GetRozptyl(unit->X, unit->Y, u);

                if ((u->ID >= BADLIFE) || (ConfirmAF(u, unit)))
                    u->Attack(unit->X + poz.x, unit->Y + poz.y);
                u->CurWpn = oldcw;

                if (Units[su]) Units[su]->Select();
                else {
                    int aID = su, oldID = su;

                    if (su < BADLIFE) {
                        while (++aID != oldID) {
                            if (aID >= BADLIFE) aID = 0;
                            if ((Units[aID] != NULL) && (Units[aID]->HitPoints > 0) &&
                                (Units[aID]->X != -1)) break;
                        }
                    }
                    else {
                        while (++aID != oldID) {
                            if (aID >= UNITS_TOP) aID = BADLIFE;
                            if ((Units[aID] != NULL)  && (Units[aID]->HitPoints > 0) 
                                && (Units[aID]->X != -1)) break;
                        }
                    }
                    SelectedUnit = NULL;
                    if (oldID != aID) Units[aID]->Select();
                }

                if (!Units[i]) AF_Units[i] = FALSE;
                else {
                    AF_Units[i] = UnitCanShoot(u);
                    if (!AF_Units[i]) {
                        oldtu2 = u->TimeUnits;
                        u->TimeUnits = oldtu;
                        AF_RemoveUnit(u);
                        u->TimeUnits = oldtu2;
                    }
                }
                if (!Units[uu]) break;
            }
        }
    }
}

