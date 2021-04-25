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
// Exploze a vybuchy
//



#include <SDL_timer.h>

#include "units.h"
#include "explode.h"
#include "fields.h"
#include "events.h"


//////////////////////////////// PROMNENE //////////////////////////////////

// pocet fazi animace exploze:
#define EXPL_PHASES       8


// male exploze - sprity:
#define EXPL_1x1_CNT      8
#define EXPL_1x1_RND      (EXPL_1x1_CNT-0)
TSprite *Explosions1x1[EXPL_1x1_CNT][EXPL_PHASES];

// velke exploze - sprity:
#define EXPL_3x3_CNT      8
#define EXPL_3x3_RND      (EXPL_3x3_CNT-1)
TSprite *Explosions3x3[EXPL_3x3_CNT][EXPL_PHASES];


MIDASsample ExplosionA_Sound[EXPL_1x1_CNT];
MIDASsample ExplosionB_Sound[EXPL_3x3_CNT];

int ProcessingExplosion = FALSE;


/////////////////////////////////// INI-FCE ////////////////////////////////

int InitExplode()
{
    int i;
    char s[20];
    
    for (i = 0; i < EXPL_1x1_CNT; i++) {
        sprintf(s, "expa%i_%%i", i);
        LoadArray((void**)(Explosions1x1[i]), EXPL_PHASES, GraphicsDF, s);
        sprintf(s, "expl_a%i", i);
        ExplosionA_Sound[i] = LoadSample(s, FALSE);
    }
    for (i = 0; i < EXPL_3x3_CNT; i++) {
        sprintf(s, "expb%i_%%i", i);
        LoadArray((void**)(Explosions3x3[i]), EXPL_PHASES, GraphicsDF, s);
        sprintf(s, "expl_b%i", i);
        ExplosionB_Sound[i] = LoadSample(s, FALSE);
    }

    return TRUE;
}



int DoneExplode()
{
    int i;
    
    for (i = 0; i < EXPL_1x1_CNT; i++) {
        DisposeArray((void**)(Explosions1x1[i]), EXPL_PHASES);
        FreeSample(ExplosionA_Sound[i]);
    }
    for (i = 0; i < EXPL_3x3_CNT; i++) {
        DisposeArray((void**)(Explosions3x3[i]), EXPL_PHASES);
        FreeSample(ExplosionB_Sound[i]);
    }
    return TRUE;
}







////////////////////////////////////////////////////////////////////////////




///////////////////////////// bouchaci buffer:

#define MAX_BOOMS    8192

typedef struct {
        word time;
        MIDASsample sound;
        int x, y, litx, lity;
        byte fx, fy;
        TSprite *sprite;
        byte mega, start;
    } TBoomItem;

static int BoomBufSize = 0;
static int BoomBufPos = 0;
static int BoomBufTime = 0;
static int BoomMaxTime = 8;
static TBoomItem BoomBuf[MAX_BOOMS];




void AddExplode1x1(int x, int y, int typ, int xofs, int yofs)
{
    if (BoomBufSize + 8 >= MAX_BOOMS)   return;
    
    int i;
    int expnum;
    if (typ) expnum = typ;
    else expnum = rand() % EXPL_1x1_RND;

    BoomBufSize += EXPL_PHASES;
    BoomBuf[BoomBufPos].sound = ExplosionA_Sound[expnum];
    BoomBuf[BoomBufPos].start = 1;
    for (i = 0; i < 8; i++, BoomBufPos++) {
        BoomBuf[BoomBufPos].time = BoomBufTime + i;
        BoomBuf[BoomBufPos].x = x;
        BoomBuf[BoomBufPos].y = y;
        BoomBuf[BoomBufPos].fx = x;
        BoomBuf[BoomBufPos].fy = y;
        BoomBuf[BoomBufPos].mega = FALSE;
        BoomBuf[BoomBufPos].litx = xofs;
        BoomBuf[BoomBufPos].lity = yofs;
        BoomBuf[BoomBufPos].sprite = Explosions1x1[expnum][i];
        if (i != 0) {
            BoomBuf[BoomBufPos].sound = 0;
            BoomBuf[BoomBufPos].start = 0;  
        }
    }
}



void AddExplode3x3(int x, int y, int typ, int xofs, int yofs, int mega)
{
    if (BoomBufSize + 8 >= MAX_BOOMS) return;
    
    int i;
    int expnum;
    if (typ) expnum = typ;
    else expnum = EXPL_3x3_RND * frand();
    
    BoomBufSize += EXPL_PHASES;
    BoomBuf[BoomBufPos].sound = ExplosionB_Sound[expnum];
    BoomBuf[BoomBufPos].start = 1;
    for (i = 0; i < 8; i++, BoomBufPos++) {
        BoomBuf[BoomBufPos].time = BoomBufTime + i;
        BoomBuf[BoomBufPos].x = x;
        BoomBuf[BoomBufPos].y = y;
        BoomBuf[BoomBufPos].fx = x;
        BoomBuf[BoomBufPos].fy = y;
        BoomBuf[BoomBufPos].mega = mega;
        BoomBuf[BoomBufPos].litx = xofs;
        BoomBuf[BoomBufPos].lity = yofs;
        BoomBuf[BoomBufPos].sprite = Explosions3x3[expnum][i];
        if (i != 0) {
            BoomBuf[BoomBufPos].sound = 0;
            BoomBuf[BoomBufPos].start = 0;  
        }
    }
}



void IncExplodeTime(int delta)
{
    BoomBufTime += delta;
    if (BoomBufTime + 8 > BoomMaxTime) BoomMaxTime = BoomBufTime + 8;
}















static int ExTime;

void DoExplosion()
{
    int i, time;
    TBoomItem *b;
    int drx, dry, drw, drh;
    int dr_used;
    int troska;

    TField *f;
    int tr, rrx, rry;
    
    if (BoomBufSize == 0) return;

    if ((!GetField(BoomBuf[0].fx, BoomBuf[0].fy)->OnScreen)) {
        switch (iniResolution) {
            case SVGA_640x480  : ScrollTo(BoomBuf[0].fx - 11, BoomBuf[0].fy - 3); break;
            case SVGA_800x600  : ScrollTo(BoomBuf[0].fx - 15, BoomBuf[0].fy - 5); break;
            case SVGA_1024x768 : ScrollTo(BoomBuf[0].fx - 19, BoomBuf[0].fy - 4); break;
        }
    }
    
    // zapsani fyzickych souradnic vybuchu misto polickovych:
    
    for (i = 0; i < BoomBufSize; i++) {
        b = BoomBuf + i;
        f = GetField(b->x, b->y);
        tr = f->Terrain;
        rrx = GetRelX(b->x), rry = GetRelY(b->y);
        b->x = 28 * (rrx - rry) + 28 + b->litx;
        b->y = 14 * (rrx + rry - (f->Height)) + (TerrOfssEnd[tr] + TerrOfssBeg[tr]) / 2 + b->lity;
    }
    
    // vlastni animace exploze:
    ProcessingExplosion = TRUE;
    dr_used = FALSE;
    for (time = 0; time < BoomMaxTime; time++) {
        // zjisti oblast:
        for (i = 0; i < BoomBufSize; i++) {
            b = BoomBuf + i;
            if (!dr_used) {
                drx = b->x - b->sprite->dx, dry = b->y - b->sprite->dy,
                drw = b->sprite->w, drh = b->sprite->h;
                dr_used = TRUE;
            }
            else 
                Union2(&drx, &dry, &drw, &drh,
                    b->x - b->sprite->dx, b->y - b->sprite->dy,
                    b->sprite->w, b->sprite->h);

            f = GetField(b->fx, b->fy);
            if ((f->Unit != NO_UNIT) && (Units[f->Unit]->HitPoints <= 0)) {
                Union2(&drx, &dry, &drw, &drh,
                    b->x - 56, b->y - 56, 2*56, 2*56);
            }
        }

        // proved kresleni:
        ExTime = time;
        ProcessMapAnim();
        StartDraw(drx, dry, drw, drh);
        PaintUnits();
        for (i = 0; i < BoomBufSize; i++) {         
            b = BoomBuf + i;            
            if (b->time == time) {
                DrawSprite(b->x, b->y, b->sprite);
                if (b->sound != 0) PlaySample(b->sound, 100+time, EffectsVolume, GetFieldPanning(b->fx, b->fy));

                // kratery + odstraneni jednotek znicenych:
                {
                    if (b->start) troska = cratCrater; else troska = cratNothing;
                    f = GetField(b->fx, b->fy);
                    if ((f->Unit != NO_UNIT) && (Units[f->Unit]->HitPoints <= 0)) {
                        Units[f->Unit]->RemoveFromWorld();
                        troska = cratCrash;
                    }
                    TAircraft *a = GetAircraftAt(b->fx, b->fy);
                    if ((a != NULL) && (a->HitPoints <= 0)) {
                         a->RemoveFromWorld();
                         troska = cratNothing;
                    }
                    if (b->mega && (troska = cratCrater)) MegaDestruction = TRUE;
                    MakeCrater(b->fx, b->fy, troska);
                    MegaDestruction = FALSE;
                    DrawField(b->fx, b->fy);
                }
            }
        }
        EndDraw();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);
        GetEvent(NULL);
    }
    ProcessingExplosion = FALSE;
    StartDraw(DrawingArea.x1, DrawingArea.y1, 
              DrawingArea.x2-DrawingArea.x1+1, DrawingArea.y2-DrawingArea.y1+1);
    PaintUnits();
    EndDraw();
    UpdateScreen();

    BoomBufPos = 0;
    BoomBufSize = 0;
    BoomBufTime = 0;
    BoomMaxTime = 8;
}





void UpdateExplodes()
{
    int i;
    TBoomItem *b;
        
    for (i = 0; i < BoomBufSize; i++) {         
        b = BoomBuf + i;            
        if (b->time == ExTime) DrawSprite(b->x, b->y, b->sprite);
    }
}
