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
// Animace pohybu jednotky
//


#include "headers.h"

#include <SDL_timer.h>

#include "moveunit.h"
#include "graphio.h"
#include "fields.h"
#include "sound.h"





MIDASsamplePlayHandle     SampleMoving;
        // zvuk vyluzovany behem pohybu
        
static void UpdateMovingSample(MIDASsample sample, int visib, int x, int y)
{
    if (SampleMoving == INVALID_SAMPLE_HANDLE) {                 // new sound
        if (visib == 2)
            SampleMoving = PlaySample(sample, 6, EffectsVolume, GetFieldPanning(x, y));
            FadeInSample(SampleMoving, EffectsVolume);
    }
    else {                                      // update panning
        if (visib != 2) {
            StopSample(SampleMoving); SampleMoving = INVALID_SAMPLE_HANDLE;
        }
        else {
            PanSample(SampleMoving, GetFieldPanning(x, y));
        }
    }
}



// Tyto dve hodnoty urcuji chovani obrazovky pri pohybu pratelske jednotky.
// Kdyz se pohybuje, obrazovka se musi prekreslovat, aby se projevily zmeny
// kruhu viditelnosti. Aby se to hybalo rychle, deje se tak jen po urcitych
// intervalech. Konkretne MoveRel_Delte volani MoveRel()...
#define MoveRel_Delta 6
int MoveRel_Counter = 0;

#define VELOCITY_FINING   180
  // hodnota Velocity, za niz se snizuje velikost kroku animace

// pocet pixelu, o ktere se animuje jednotka:
int MOVE_DELTA = 7;

int TUnit::MoveRel(int rx, int ry)
{
    int wasFnd = DoVisib();
    int AnimDelay = iniAnimDelay * Velocity / 0xFF;
    if (Velocity > VELOCITY_FINING) AnimDelay = AnimDelay * 3 / 7, MOVE_DELTA = 3;
    else MOVE_DELTA = 7;

    if (wasFnd) {
        RedrawMap(); // plne vykresleni
        MoveRel_Counter = 0;
    }
        
    // prvni polovina pohybu:
    if (GetField(X, Y)->Terrain % 256 < tofsL1B) {
        switch (Orient) {
            case 0 : 
                for (; LittleY >= -14; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY += MOVE_DELTA;
                break;
            case 1 :
                for (; LittleX >= -14; LittleX -= MOVE_DELTA) {
                    LittleY = LittleX / 2;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = LittleX / 2;
                break;
            case 2 :
                for (; LittleX >= -28; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                break;
            case 3 :
                for (; LittleX >= -14; LittleX -= MOVE_DELTA) {
                    LittleY = -LittleX / 2;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = -LittleX / 2;
                break;
            case 4 :
                for (; LittleY <= 14; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY -= MOVE_DELTA;
                break;
            case 5 :
                for (; LittleX <= 14; LittleX += MOVE_DELTA) {
                    LittleY = LittleX / 2;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = LittleX / 2;
                break;
            case 6 :
                for (; LittleX <= 28; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                break;
            case 7 :
                for (; LittleX <= 14; LittleX += MOVE_DELTA) {
                    LittleY = -LittleX / 2;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = -LittleX / 2;
                break;
        }
    }
    
    // Sikme tereny:
    else {
        byte T = GetField(X, Y)->Terrain % 256;
        if ((T >= tofsL1E) && (T < tofsL1F)) {
            if (Orient == 5) {
                for (; LittleX <= 14; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
            }
            else {
                for (; LittleX >= -14; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
            }
        }
        else if ((T >= tofsL1D) && (T < tofsL1E)) {
            if (Orient == 3) {
                for (; LittleX >= -14; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
            }
            else {
                for (; LittleX <= 14; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
            }
        }
        else if ((T >= tofsL1B) && (T < tofsL1C)) {
            if (Orient == 7) {
                for (; LittleX <= 14; LittleX += MOVE_DELTA) {
                    LittleY = -LittleX;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = -LittleX;
            }
            else {
                for (; LittleX >= -14; LittleX -= MOVE_DELTA) {
                    LittleY = -LittleX;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = -LittleX;
            }
        }
        else if ((T >= tofsL1C) && (T < tofsL1D)) {
            if (Orient == 1) {
                for (; LittleX >= -14; LittleX -= MOVE_DELTA) {
                    LittleY = LittleX;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = LittleX;
            }
            else {
                for (; LittleX <= 14; LittleX += MOVE_DELTA) {
                    LittleY = LittleX;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = LittleX;
            }
        }
        else if (L1TerrainType[T] == tofsL1F) {
            if (Orient == 0) {
                for (; LittleY >= -20; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }               
                PaintUnitInMove(X - rx, Y - ry);
                LittleY = -FIELD_Y/2;
            }
            else {
                for (;LittleY <= 14; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                ActualSprite = Orient;
                for (; LittleY < FIELD_Y/2; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleY -= MOVE_DELTA;
            }
        }
        else if (L1TerrainType[T] == tofsL1J) {
            if (Orient == 0) {
                for (; LittleY >= -14; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY += MOVE_DELTA;
            }
            else {
                ActualSprite = 20;
                for (LittleY = 0; LittleY <= 20; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }               
                LittleY -= MOVE_DELTA;
            }
        }
        else if (L1TerrainType[T] == tofsL1H) {
            if (Orient == 0) {
                ActualSprite = Orient;
                LittleY = -14;
            }
            else {
                ActualSprite = 8;
                if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(2*AnimDelay);
                LittleY = 14;
            }
        }
        else if (L1TerrainType[T] == tofsL1L) {
            if (Orient == 0) {
                ActualSprite = 16;
                LittleY = -14;
                if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(2*AnimDelay);
                LittleY = -FIELD_Y;
            }
            else {
                ActualSprite = Orient;
                LittleY = 0;
            }
        }
        else if (L1TerrainType[T] == tofsL1I) {
            if (Orient == 6) {
                for (; LittleX <= 28; LittleX += MOVE_DELTA, LittleY -= MOVE_DELTA/2) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                ActualSprite = Orient;
                LittleY = 0, LittleX = 28;
            }
            else {
                ActualSprite = Orient;
                for (LittleY = 14; LittleX >= -28; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY = 0;
                LittleX += MOVE_DELTA;
            }
        }
        else if (L1TerrainType[T] == tofsL1K) {
            if (Orient == 6) {
                for (; LittleX <= 28; LittleX += MOVE_DELTA, LittleY += MOVE_DELTA/2) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                ActualSprite = Orient;
                LittleY = 0, LittleX = 28;
            }
            else {
                ActualSprite = Orient;
                for (LittleY = 0; LittleX >= -28; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY = 0;
                LittleX += MOVE_DELTA;
            }
        }
        else if (L1TerrainType[T] == tofsL1G) {
            if (Orient == 6) {
                ActualSprite = Orient;
                for (LittleY = 14; LittleX <= 28; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY = 0;
                LittleX -= MOVE_DELTA;
            }
            else {
                for (; LittleX >= -28; LittleX -= MOVE_DELTA, LittleY -= MOVE_DELTA/2) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                ActualSprite = Orient;
                LittleY = 0, LittleX = -28;
            }
        }
        else if (L1TerrainType[T] == tofsL1M) {
            if (Orient == 6) {
                ActualSprite = Orient;
                for (LittleY = 0; LittleX <= 28; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY = 0;
                LittleX -= MOVE_DELTA;
            }
            else {
                for (; LittleX >= -28; LittleX -= MOVE_DELTA, LittleY += MOVE_DELTA/2) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                ActualSprite = Orient;
                LittleY = 0, LittleX = -28;
            }
        }
    }
    
    
    // prechod na jine policko:
    MakeTrack(); DrawField(X, Y);
    PlaceGround(FALSE);
    X += rx, Y += ry;
    PlaceGround(TRUE);
    GenerateUnitsList();
    if ((GetMineAt(X, Y) != -1) &&
        (GetType() != unSatan) && (GetType() != unNeptun) && (GetType() != unHelios))
        flagUnitOnMine = TRUE;
    UpdateMovingSample(GetUnitSound(1), GetField(X, Y)->Visib, X, Y);
    
    
    // druha polovina animace:
    if (GetField(X, Y)->Terrain % 256 < tofsL1B) {
        ActualSprite = Orient;
        switch (Orient) {
            case 0 : 
                for (LittleY += FIELD_Y; LittleY >= 0; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleY += MOVE_DELTA;
                break;
            case 1 : 
                for (LittleX += FIELD_X/2, LittleY += FIELD_Y/2; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    LittleY = LittleX / 2;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = LittleX / 2;
                break;
            case 2 : 
                for (LittleX += FIELD_X; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                break;
            case 3 :
                for (LittleX += FIELD_X/2, LittleY += FIELD_Y/2; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    LittleY = -LittleX / 2;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = -LittleX / 2;
                break;
            case 4 : 
                for (LittleY -= FIELD_Y; LittleY <= 0; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleY -= MOVE_DELTA;
                break;
            case 5 :
                for (LittleX -= FIELD_X/2, LittleY -= FIELD_Y/2; LittleX <= 0; LittleX += MOVE_DELTA) {
                    LittleY = LittleX / 2;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = LittleX / 2;
                break;
            case 6 : 
                for (LittleX -= FIELD_X; LittleX <= 0; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                break;
            case 7 :
                for (LittleX -= FIELD_X/2, LittleY -= FIELD_Y/2; LittleX <= 0; LittleX += MOVE_DELTA) {
                    LittleY = -LittleX / 2;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = -LittleX / 2;
                break;
        }
    }

    // Sikme tereny:
    else {
        LittleX = 0, LittleY = 0;
        byte T = GetField(X, Y)->Terrain % 256;
        if ((T >= tofsL1E) && (T < tofsL1F)) {
            if (Orient == 5) {
                ActualSprite = 9;
                for (LittleX = -14; LittleX <= 0; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
            }
            else {
                ActualSprite = 17;
                for (LittleX = 14; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
            }
        }
        else if ((T >= tofsL1D) && (T < tofsL1E)) {
            if (Orient == 3) {
                ActualSprite = 15;
                for (LittleX = 14; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
            }
            else {
                ActualSprite = 23;
                for (LittleX = -14; LittleX <= 0; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
            }
        }
        else if ((T >= tofsL1B) && (T < tofsL1C)) {
            if (Orient == 7) {
                ActualSprite = 11;
                for (LittleX = -14; LittleX <= 0; LittleX += MOVE_DELTA) {
                    LittleY = -LittleX;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = -LittleX;
            }
            else {
                ActualSprite = 19;
                for (LittleX = 14; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    LittleY = -LittleX;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = -LittleX;
            }
        }
        else if ((T >= tofsL1C) && (T < tofsL1D)) {
            if (Orient == 1) {
                ActualSprite = 13;
                for (LittleX = 14; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    LittleY = LittleX;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = LittleX;
            }
            else {
                ActualSprite = 21;
                for (LittleX = -14; LittleX <= 0; LittleX += MOVE_DELTA) {
                    LittleY = LittleX;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = LittleX;
            }
        }
        else if (L1TerrainType[T] == tofsL1F) {
            if (Orient == 0) {
                for (LittleY += FIELD_Y; LittleY > 14; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                ActualSprite = 12;
                for (;LittleY >= 0; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleY += MOVE_DELTA;
            }
            else {
                ActualSprite = 20;
                for (LittleY = -20; LittleY <= 0; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }               
                PaintUnitInMove(X - rx, Y - ry);
            }
        }
        else if (L1TerrainType[T] == tofsL1J) {
            if (Orient == 0) {
                ActualSprite = 12;
                for (LittleY = 20; LittleY >= 0; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }               
                if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                ActualSprite = Orient;
                PaintUnitInMove(X - rx, Y - ry);
                LittleY += MOVE_DELTA;
            }
            else {
                ActualSprite = Orient;
                for (LittleY = -14; LittleY <= 0; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleY -= MOVE_DELTA;
            }
        }
        else if (L1TerrainType[T] == tofsL1H) {
            if (Orient == 0) {
                ActualSprite = 16;
                if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(2*AnimDelay);
            }
            else {
                ActualSprite = 8;
                LittleY = 0;
                PaintUnitInMove(X - rx, Y - ry);
            }
        }
        else if (L1TerrainType[T] == tofsL1L) {
            if (Orient == 0) {
                for (LittleY = 14; LittleY >= 7; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                ActualSprite = Orient;
                LittleY = -14;
                ActualSprite = 8;
                PaintUnitInMove(X - rx, Y - ry);
            }
            else {
                ActualSprite = 8;
                LittleY = -14;
                if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(2*AnimDelay);
            }
        }
        else if (L1TerrainType[T] == tofsL1I) {
            if (Orient == 6) {
                ActualSprite = Orient;
                for (LittleY = 14, LittleX = -28; LittleX <= 0; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                ActualSprite = 10;
                LittleX = LittleY = 0;
                PaintUnitInMove(X - rx, Y - ry);
            }
            else {
                ActualSprite = 18;
                for (LittleY = -11, LittleX = 20; LittleX >= 0; LittleX -= MOVE_DELTA, LittleY += MOVE_DELTA/2) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX = LittleY = 0;
            }
        }
        else if (L1TerrainType[T] == tofsL1K) {
            if (Orient == 6) {
                ActualSprite = Orient;
                for (LittleY = 0, LittleX = -28; LittleX <= 0; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                ActualSprite = 22;
                LittleX = 0; LittleY = -11;
                PaintUnitInMove(X - rx, Y - ry);
            }
            else {
                ActualSprite = 14;
                for (LittleY = 0, LittleX = 20; LittleX >= 0; LittleX -= MOVE_DELTA, LittleY -= MOVE_DELTA/2) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX = 0, LittleY = -11;
            }
        }
        else if (L1TerrainType[T] == tofsL1G) {
            if (Orient == 6) {
                ActualSprite = 22;
                for (LittleY = -11, LittleX = -20; LittleX <= 0; LittleX += MOVE_DELTA, LittleY += MOVE_DELTA/2) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX = LittleY = 0;
            }
            else {
                ActualSprite = Orient;
                for (LittleY = 14, LittleX = 28; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                ActualSprite = 14;
                LittleX = LittleY = 0;
                PaintUnitInMove(X - rx, Y - ry);
            }
        }
        else if (L1TerrainType[T] == tofsL1M) {
            if (Orient == 6) {
                ActualSprite = 10;
                for (LittleY = 0, LittleX = -20; LittleX <= 0; LittleX += MOVE_DELTA, LittleY -= MOVE_DELTA/2) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX = 0, LittleY = -11;
            }
            else {
                ActualSprite = Orient;
                for (LittleY = 0, LittleX = 28; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry))    SDL_Delay(AnimDelay);
                }
                ActualSprite = 18;
                LittleX = 0; LittleY = -11;
                PaintUnitInMove(X - rx, Y - ry);
            }
        }
    }
    



    if (++MoveRel_Counter == MoveRel_Delta) {
        MoveRel_Counter = 0;
        RedrawMap();
    }

    return wasFnd;
}























int TAircraft::MoveRel(int rx, int ry)
{
    int wasFnd = DoVisib();
    int AnimDelay = iniAnimDelay * Velocity / 0xFF;
    if (Velocity > VELOCITY_FINING) AnimDelay = AnimDelay * 3 / 7, MOVE_DELTA = 3;
    else MOVE_DELTA = 7;

    if (wasFnd) {
        RedrawMap(); // plne vykresleni
        MoveRel_Counter = 0;
    }
        
    // prvni polovina pohybu:
    /*if (GetField(X, Y)->Terrain % 256 < tofsL1B)*/ {
        switch (Orient) {
            case 0 : 
                for (; LittleY >= -14; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY += MOVE_DELTA;
                break;
            case 1 :
                for (; LittleX >= -14; LittleX -= MOVE_DELTA) {
                    LittleY = LittleX / 2;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = LittleX / 2;
                break;
            case 2 :
                for (; LittleX >= -28; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                break;
            case 3 :
                for (; LittleX >= -14; LittleX -= MOVE_DELTA) {
                    LittleY = -LittleX / 2;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = -LittleX / 2;
                break;
            case 4 :
                for (; LittleY <= 14; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleY -= MOVE_DELTA;
                break;
            case 5 :
                for (; LittleX <= 14; LittleX += MOVE_DELTA) {
                    LittleY = LittleX / 2;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = LittleX / 2;
                break;
            case 6 :
                for (; LittleX <= 28; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                break;
            case 7 :
                for (; LittleX <= 14; LittleX += MOVE_DELTA) {
                    LittleY = -LittleX / 2;
                    if (PaintUnitInMove(X + rx, Y + ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = -LittleX / 2;
                break;
        }
    }
    
    
    // prechod na jine policko:
    MakeTrack(); DrawField(X, Y);
    PlaceGround(FALSE);
    X += rx, Y += ry;
    PlaceGround(TRUE);
    GenerateUnitsList();
    UpdateMovingSample(GetUnitSound(1), GetField(X, Y)->Visib, X, Y);

    
    // druha polovina animace:
    /*if (GetField(X, Y)->Terrain % 256 < tofsL1B)*/ {
        ActualSprite = Orient;
        switch (Orient) {
            case 0 : 
                for (LittleY += FIELD_Y; LittleY >= 0; LittleY -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleY += MOVE_DELTA;
                break;
            case 1 : 
                for (LittleX += FIELD_X/2, LittleY += FIELD_Y/2; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    LittleY = LittleX / 2;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = LittleX / 2;
                break;
            case 2 : 
                for (LittleX += FIELD_X; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                break;
            case 3 :
                for (LittleX += FIELD_X/2, LittleY += FIELD_Y/2; LittleX >= 0; LittleX -= MOVE_DELTA) {
                    LittleY = -LittleX / 2;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX += MOVE_DELTA;
                LittleY = -LittleX / 2;
                break;
            case 4 : 
                for (LittleY -= FIELD_Y; LittleY <= 0; LittleY += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleY -= MOVE_DELTA;
                break;
            case 5 :
                for (LittleX -= FIELD_X/2, LittleY -= FIELD_Y/2; LittleX <= 0; LittleX += MOVE_DELTA) {
                    LittleY = LittleX / 2;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = LittleX / 2;
                break;
            case 6 : 
                for (LittleX -= FIELD_X; LittleX <= 0; LittleX += MOVE_DELTA) {
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                break;
            case 7 :
                for (LittleX -= FIELD_X/2, LittleY -= FIELD_Y/2; LittleX <= 0; LittleX += MOVE_DELTA) {
                    LittleY = -LittleX / 2;
                    if (PaintUnitInMove(X - rx, Y - ry)) SDL_Delay(AnimDelay);
                }
                LittleX -= MOVE_DELTA;
                LittleY = -LittleX / 2;
                break;
        }
    }



    if (++MoveRel_Counter == MoveRel_Delta) {
        MoveRel_Counter = 0;
        RedrawMap();
    }

    return wasFnd;
}
