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


//
// Viditelnost
//


#include "headers.h"

#include <math.h>
#include "units.h"
#include "visiblty.h"



int VisibEnemies = 2;



/////////////////////// R A D A R :

int ProcessRadarVisib(int X, int Y, int Visib/*range*/, int ID)
{
    int i, ax, ay, dx, dy, pred;

    VisibEnemies = 0;
    if (ID < BADLIFE) {
        ax = 0, ay = Visib, pred = 1 - Visib, dx = 3, dy = 2 * Visib - 2;
        do {
            for (i = Y - ay; i <= Y + ay; i++) SetGoodlifeVisib(X + ax, i, 2);
            for (i = Y - ay; i <= Y + ay; i++) SetGoodlifeVisib(X - ax, i, 2);
            for (i = Y - ax; i <= Y + ax; i++) SetGoodlifeVisib(X + ay, i, 2);
            for (i = Y - ax; i <= Y + ax; i++) SetGoodlifeVisib(X - ay, i, 2);
        
            if (pred >= 0) pred -= dy, dy -= 2, ay--;
            pred += dx, dx += 2, ax++;
        } while (!(ax > ay));
    }
    else if (AllowedBadlife) {
        ax = 0, ay = Visib, pred = 1 - Visib, dx = 3, dy = 2 * Visib - 2;
        do {
            for (i = Y - ay; i <= Y + ay; i++) SetBadlifeVisib(X + ax, i, 2);
            for (i = Y - ay; i <= Y + ay; i++) SetBadlifeVisib(X - ax, i, 2);
            for (i = Y - ax; i <= Y + ax; i++) SetBadlifeVisib(X + ay, i, 2);
            for (i = Y - ax; i <= Y + ax; i++) SetBadlifeVisib(X - ay, i, 2);
        
            if (pred >= 0) pred -= dy, dy -= 2, ay--;
            pred += dx, dx += 2, ax++;
        } while (!(ax > ay));
    }
    return VisibEnemies;
}







/////////////////////// E Y E S :

int Visib_hght, Visib_basehght;

inline int Pythagoras(int x, int y)
{
    return roundnum(sqrt(x * x + y * y));
}

inline int GetHeight(int x, int y)
{
    TField *f = GetField(x, y);
    int h = f->Height + TabHghtModifVisib[f->Terrain2];
    if (f->Terrain % 256 >= tofsL1B) h--;
    return h;
}

inline void SetUniveSee(int x, int y, int ID)
{
    if (!IsInRect(x, y, 0, 0, MapSizeX-1, MapSizeY-1)) return;

    int h = GetHeight(x, y);
    if ((h >= Visib_hght) || ((Visib_hght <= Visib_basehght) && (h < Visib_basehght))){
        if (h > Visib_hght) Visib_hght = h;
        if (ID < BADLIFE)   SetGoodlifeVisib(x, y, 2);
        else if (AllowedBadlife) SetBadlifeVisib(x, y, 2);
    }   
}




void Bresen(int x1, int y1, int x2, int y2, int id, int visib)
{
    int dx, dy, k1, k2, x, y, xkon, pred;
    int dex = abs(x2 - x1), dey = abs(y2 - y1);

    Visib_hght = Visib_basehght = GetHeight(x1, y1);
    
    if (x2 > x1) {
        if (y2 > y1) {
            if (dex > dey) {             // I. octadrant
                x = x1, y = y1, xkon = x2;
                dx = xkon - x, dy = y2 - y1;
                k1 = 2 * dy, k2 = 2 * (dy - dx);
                pred = 2 * dy - dx;
                SetUniveSee(x, y, id);
                while (x < xkon) {
                    x++;
                    if (pred < 0) pred += k1;
                    else y++, pred += k2;
                    if (((int)Pythagoras(x - x1, y - y1)) > visib) break;
                    SetUniveSee(x, y, id);
                }
            }
            else {                       // II. octadrant
                x = y1, y = x1, xkon = y2;
                dx = xkon - x, dy = x2 - x1;
                k1 = 2 * dy, k2 = 2 * (dy - dx);
                pred = 2 * dy - dx;
                SetUniveSee(y, x, id);
                while (x < xkon) {
                    x++;
                    if (pred < 0) pred += k1;
                    else y++, pred += k2;
                    if (((int)Pythagoras(y - x1, x - y1)) > visib) break;
                    SetUniveSee(y, x, id);
                }
            }
        }
        else {
            if (dex > dey) {             // VIII. octadrant
                x = x1, y = y1, xkon = x2;
                dx = xkon - x, dy = y1 - y2;
                k1 = 2 * dy, k2 = 2 * (dy - dx);
                pred = 2 * dy - dx;
                SetUniveSee(x, y, id);
                while (x < xkon) {
                    x++;
                    if (pred < 0) pred += k1;
                    else y--, pred += k2;
                    if (((int)Pythagoras(x - x1, y - y1)) > visib) break;
                    SetUniveSee(x, y, id);
                }
            }
            else {                       // VII. octadrant
                x = y1, y = x1, xkon = y2;
                dx = x - xkon, dy = x2 - x1;
                k1 = 2 * dy, k2 = 2 * (dy - dx);
                pred = 2 * dy - dx;
                SetUniveSee(y, x, id);
                while (x > xkon) {
                    x--;
                    if (pred < 0) pred += k1;
                    else y++, pred += k2;
                    if (((int)Pythagoras(y - x1, x - y1)) > visib) break;
                    SetUniveSee(y, x, id);
                }
            }
        }
    }
    else {
        if (y2 > y1) {
            if (dex > dey) {             // IV. octadrant
                x = x1, y = y1, xkon = x2;
                dx = x - xkon, dy = y2 - y1;
                k1 = 2 * dy, k2 = 2 * (dy - dx);
                pred = 2 * dy - dx;
                SetUniveSee(x, y, id);
                while (x > xkon) {
                    x--;
                    if (pred < 0) pred += k1;
                    else y++, pred += k2;
                    if (((int)Pythagoras(x - x1, y - y1)) > visib) break;
                    SetUniveSee(x, y, id);
                }
            }
            else {                       // III. octadrant
                x = y1, y = x1, xkon = y2;
                dx = xkon - x, dy = x1 - x2;
                k1 = 2 * dy, k2 = 2 * (dy - dx);
                pred = 2 * dy - dx;
                SetUniveSee(y, x, id);
                while (x < xkon) {
                    x++;
                    if (pred < 0) pred += k1;
                    else y--, pred += k2;
                    if (((int)Pythagoras(y - x1, x - y1)) > visib) break;
                    SetUniveSee(y, x, id);
                }
            }
        }
        else {
            if (dex > dey) {             // V. octadrant
                x = x1, y = y1, xkon = x2;
                dx = x - xkon, dy = y1 - y2;
                k1 = 2 * dy, k2 = 2 * (dy - dx);
                pred = 2 * dy - dx;
                SetUniveSee(x, y, id);
                while (x > xkon) {
                    x--;
                    if (pred < 0) pred += k1;
                    else y--, pred += k2;
                    if (((int)Pythagoras(x - x1, y - y1)) > visib) break;
                    SetUniveSee(x, y, id);
                }
            }
            else {                       // VI. octadrant
                x = y1, y = x1, xkon = y2;
                dx = x - xkon, dy = x1 - x2;
                k1 = 2 * dy, k2 = 2 * (dy - dx);
                pred = 2 * dy - dx;
                SetUniveSee(y, x, id);
                while (x > xkon) {
                    x--;
                    if (pred < 0) pred += k1;
                    else y--, pred += k2;
                    if (((int)Pythagoras(y - x1, x - y1)) > visib) break;
                    SetUniveSee(y, x, id);
                }
            }
        }
    }
}




int ProcessEyeVisib(int X, int Y, int Visib/*range*/, int ID)
{
    int i;

    VisibEnemies = 0;

    for (i = X - Visib; i <= X + Visib; i++) {
        Bresen(X, Y, i, Y - Visib, ID, Visib);
        Bresen(X, Y, i, Y + Visib, ID, Visib);
    }
    for (i = Y - Visib; i <= Y + Visib; i++) {
        Bresen(X, Y, X - Visib, i, ID, Visib);
        Bresen(X, Y, X + Visib, i, ID, Visib);
    }

    return VisibEnemies;
}











///////////////////////// BADLIFE VISIB //////////////////////////////////

int AllowedBadlife = FALSE;
byte *BadlifeVisib = NULL;



void AllowBadlife(int enable)
{   
    if (!BadlifeVisib)
        BadlifeVisib = (byte*)memalloc(MapSizeX * MapSizeY);
    AllowedBadlife = enable;
}



byte GetBadlifeVisib(int x, int y)
{
    if (!BadlifeVisib) return 0;
    if (!IsInRect(x, y, 0, 0, MapSizeX-1, MapSizeY-1)) return 0;
    return BadlifeVisib[x + y * MapSizeX];
}

void SetBadlifeVisib(int x, int y, byte value)
{
    if (!BadlifeVisib) return;
    if (IsInRect(x, y, 0, 0, MapSizeX-1, MapSizeY-1))
        BadlifeVisib[x + y * MapSizeX] = value;
}



byte GetGoodlifeVisib(int x, int y)
{
    if (!IsInRect(x, y, 0, 0, MapSizeX-1, MapSizeY-1)) return 0;
    return GetField(x, y)->Visib;
}

void SetGoodlifeVisib(int x, int y, byte value)
{
    if (IsInRect(x, y, 0, 0, MapSizeX-1, MapSizeY-1)) {
        TField *f = GetField(x, y);
        if ((f->Visib != 2) && 
            (f->Unit != NO_UNIT) && (f->Unit >= BADLIFE) &&
            (Units[f->Unit]->Type % BADLIFE < unRadar)) VisibEnemies++;
        f->Visib = value;
    }   
}



void ComputeVisib()
{
    int x, y;
    TField *f;
    
    for (x = 0; x < MapSizeX; x++)
        for (y = 0; y < MapSizeY; y++) {
            f = GetField(x, y);
            if (f->Visib == 2) f->Visib = 1;
        }
    if (AllowedBadlife) 
        for (x = 0; x < MapSizeX; x++)
            for (y = 0; y < MapSizeY; y++) {
                if (GetBadlifeVisib(x, y) == 2) 
                    SetBadlifeVisib(x, y, 1);
            }
    
    for (int i = 0; i < BADLIFE; i++)
        if ((Units[i] != NULL) && (Units[i]->X != -1)) 
            Units[i]->DoVisib();

    if (AllowedBadlife)
        for (int i = BADLIFE; i < UNITS_TOP; i++)
            if ((Units[i] != NULL) && (Units[i]->X != -1))
                Units[i]->DoVisib();
}
