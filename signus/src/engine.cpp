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


// Graficky IZO-3D engine

#include "headers.h"

#include "engine.h"
#include "graphio.h"

#include "units.h"
#include "showrngs.h"
#include "fields.h"
#include "explode.h"
#include "engtimer.h"
#include "mouse.h"






// Buffer pro vykreslovani:
void *MapBuf, *FullBuf;

// Mapa a pomocne promnene:
int MapSizeX, MapSizeY;
TField *Map;

int ActualTurn;
int ActualMission;
char ActualDifficulty;
int TurnTime, MissionTime;

TPoint MapPos = {0};
TPoint RangeUpIn[20], RangeUpOut[20], RangeDownIn[20], RangeDownOut[20];


/////// Letecky kurzor (lokalizace + kresleni):
TSprite *AirCursors[2];
TPoint AirCursorPos;
int AirCursorId;
int AirCursorOn = 0;
// 0 - vypnut
// 1 - nizko
// 2 - vysoko


TSprite *MineSprite = NULL; //mina


// Data bitmap:
void *BmpTerr1[1024];
void *BmpTerr2[1024];
void *BmpTerr1D[1024];
void *BmpTerr2D[1024];
void *BmpSel[13], *BmpSelBold[13];



// HDD-like kontrolka ukazujici praci:
void *WorkingControl[3];
int WC_Phase = 1;
int WC_Period = 1;
int WC_On = FALSE;




int RedrawMapLocks = 0;

// zamykani kresleni+skrolovani:
int DrawLocks = 0;

void LockDraw() 
{
    DrawLocks++;
}


void UnlockDraw(int redraw)
{
    
    DrawLocks--;
    if ((!DrawLocks) && (redraw)) RedrawMap();

#ifdef DEBUG
    if (DrawLocks < 0) Message(" U n l o c k D r a w ( )   U N D E R F L O W ! ! !");
#endif
}






//////////////////// "soukroma" data: tabulky fci apod.: /////////////////

#include "etables.h"


int EngineInited = FALSE;

// Ofsety kresleni jednotlivych terenu vuci pocatku typu A:
int TerrOfssBeg[1024], TerrOfssEnd[1024];


// Nastavi tabulky na prislusne hodnoty:
void SetTables()
{
    int i;
    for (i = 0; i < tofsL1B; i++) {TerrOfssBeg[i] = 0; TerrOfssEnd[i] = 27;}
    for (; i < tofsL1C; i++) {TerrOfssBeg[i] = 0; TerrOfssEnd[i] = 40;}
    for (; i < tofsL1D; i++) {TerrOfssBeg[i] = 0; TerrOfssEnd[i] = 40;}
    for (; i < tofsL1E; i++) {TerrOfssBeg[i] = 13; TerrOfssEnd[i] = 26;}
    for (; i < tofsL1F; i++) {TerrOfssBeg[i] = 13; TerrOfssEnd[i] = 26;}
    for (; i < tofsL1G; i++) {TerrOfssBeg[i] = 0; TerrOfssEnd[i] = 40;}
    for (; i < tofsL1H; i++) {TerrOfssBeg[i] = 14; TerrOfssEnd[i] = 40;}
    for (; i < tofsL1I; i++) {TerrOfssBeg[i] = 14; TerrOfssEnd[i] = 27;}
    for (; i < tofsL1J; i++) {TerrOfssBeg[i] = 14; TerrOfssEnd[i] = 40;}
    for (; i < tofsL1K; i++) {TerrOfssBeg[i] = 0; TerrOfssEnd[i] = 39;}
    for (; i < tofsL1L; i++) {TerrOfssBeg[i] = 0; TerrOfssEnd[i] = 27;}
    for (; i < tofsL1M; i++) {TerrOfssBeg[i] = 14; TerrOfssEnd[i] = 26;}
    for (; i < 256; i++) {TerrOfssBeg[i] = 0; TerrOfssEnd[i] = 27;}
    for (i = 0; i < 256; i++) {
        TerrOfssBeg[i+256] = TerrOfssBeg[i+512] = TerrOfssBeg[i+768] = TerrOfssBeg[i];
        TerrOfssEnd[i+256] = TerrOfssEnd[i+512] = TerrOfssEnd[i+768] = TerrOfssEnd[i];
    }
}






///////////////////////////// fce pro TSprite:

void GetSpriteRect(TRect *r, TSprite *s, int x, int y, int lx, int ly)
{
    TField *f = GetField(x, y);
    int rrx = GetRelX(x), rry = GetRelY(y);
    int drawx = 28 * (rrx - rry) + lx + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + ly + 14;
    
    r->x1 = drawx - s->dx;
    r->y1 = drawy - s->dy;
    r->x2 = r->x1 + s->w - 1;
    r->y2 = r->y1 + s->h - 1;
}




////////////////////////////// INICIALIZACE ///////////////////////////////



int InitEngine(int mission, FILE *fptr)
{
    void *ptr;
    char s[20];
    
    EngineInited = FALSE;
    WaitCursor(TRUE);
    if (fptr == NULL) {
        sprintf(s, "mis%i%c", mission, ActualDifficulty);
        ActualMission = mission;
    }
    else {s[0] = 0; ActualMission = 0;}
    SelPos.x = SelPos.y = 0xFF;
    MapBuf = memalloc(VIEW_PIXSZ_X * VIEW_PIXSZ_Y);
    FullBuf = memalloc(VIEW_PIXSZ_X * VIEW_PIXSZ_Y);
    lockmem(MapBuf, VIEW_PIXSZ_X * VIEW_PIXSZ_Y);
    lockmem(FullBuf, VIEW_PIXSZ_X * VIEW_PIXSZ_Y);
    LocalBufX = memalloc(VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2);
  lockmem(LocalBufX, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2);
    LocalBufY = memalloc(VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2);
    lockmem(LocalBufY, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2);

    if (!InitUnits()) return 0;
    
    SetTables();
    InitMap(s, fptr);

    {
        char p[768]  = {0};
        char buf[20];
        sprintf(buf, "%imainscr", iniResolution - 0x100);
        ptr = GraphicsDF->get(buf);       
        PulsarProcess = FALSE;
        SetPalette(p);
        MouseHide();
        DrawPicture(ptr); 
        MouseShow();
        memfree(ptr);
    }
    
  UpdateLitMap(TRUE);
  ScrollTo((MapSizeX - VIEW_SIZE_X) / 2, (MapSizeY - VIEW_SIZE_Y) / 2);
  SelectedUnit->Select();
        
    InitPalTimer();
    WaitCursor(FALSE);
#ifdef DEBUG
    fprintf(dbgOutput, "InitEngine() done\n  locked memory: %i bytes", dbgMemLocked);
#endif
    EngineInited = TRUE;
    return ((MapBuf != NULL) &&
            ((LocalBufX != NULL) && (LocalBufY != NULL)));
}



int DoneEngine()
{
    DonePalTimer();
    DoneUnits();
  unlockmem(Map, MapSizeX * MapSizeY * sizeof(TField));
  memfree(Map);
  AircraftsCnt = 0;
  memfree(ZeroUp); memfree(ZeroDown); 
  memfree(ZeroLeft); memfree(ZeroRight);
  memfree(LitMapBuf);
  memfree(LitMapTrans);
  
    DoneBitmaps();
    unlockmem(MapBuf, VIEW_PIXSZ_X * VIEW_PIXSZ_Y);
    memfree(MapBuf);
    unlockmem(FullBuf, VIEW_PIXSZ_X * VIEW_PIXSZ_Y);
    memfree(FullBuf);
    unlockmem(LocalBufX, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2);
    memfree(LocalBufX);
    unlockmem(LocalBufY, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2);
    memfree(LocalBufY);
    return 1;
}



#ifdef DEBUG
int MisLoads[1024];
#endif

void InitBitmaps(byte MaskL1[], byte MaskL2[])
{
    LoadArray(BmpTerr1, 1024, GraphicsDF, "tr1n%i", MaskL1);
    LoadArray(BmpTerr2, 1024, GraphicsDF, "tr2n%i", MaskL2);
    LoadArray(BmpTerr1D, 1024, GraphicsDF, "tr1d%i", MaskL1);
    LoadArray(BmpTerr2D, 1024, GraphicsDF, "tr2d%i", MaskL2);
    LoadArray(BmpSel, 13, GraphicsDF, "tersel%i");
    LoadArray(BmpSelBold, 13, GraphicsDF, "terslb%i");
    LoadArray((void**) AirCursors, 2, GraphicsDF, "airsel%i");
    LoadArray(WorkingControl, 3, GraphicsDF, "working%i");
    MineSprite = (TSprite*) GraphicsDF->get("sprmine");
#ifdef DEBUG
  {
    char ds[50];
    TSprite *s;
    
    for (int i = 0; i < 1024; i++) MisLoads[i] = FALSE;
    for (i = 0; i < 1024; i++) {
            if (BmpTerr2[i] == NULL) {
                MisLoads[i] = TRUE;
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
  }
#endif
}



void DoneBitmaps()
{
    DisposeArray(BmpTerr1, 1024);
    DisposeArray(BmpTerr2, 1024);
    DisposeArray(BmpTerr1D, 1024);
    DisposeArray(BmpTerr2D, 1024);
    DisposeArray(BmpSel, 13);
    DisposeArray(BmpSelBold, 13);
    DisposeArray((void**) AirCursors, 2);
    DisposeArray(WorkingControl, 3);
    memfree(MineSprite);
}







///////////////////////////// MISSION LOADING ///////////////////////////



unsigned MissionWrite(FILE *fi, void *ptr, size_t size)
{
    int i, j, k;
    TField *f;
    byte TL1[1024], TL2[1024];

    fwrite(&MapSizeX, 4, 1, fi); 
    fwrite(&MapSizeY, 4, 1, fi); 
    fwrite(Map, MapSizeX * MapSizeY * sizeof(TField), 1, fi);
  {
    // Tabulky pouzitych terenu:
    memset(TL1, 0, sizeof(TL1));
    memset(TL2, 0, sizeof(TL1));
    TL1[0] = 1; TL1[104] = 1; TL1[124] = 1; TL1[144] = 1; TL1[164] = 1;
    TL1[184] = 1; TL1[192] = 1; TL1[200] = 1; TL1[208] = 1; TL1[216] = 1;
    TL1[224] = 1; TL1[240] = 1; TL1[248] = 1;
    for (i = 0; i < MapSizeX; i++)
        for (j = 0; j < MapSizeY; j++) {
          f = GetField(i, j);
          TL1[f->Terrain] = 1;
        TL2[f->Terrain2] = 1;
        }
    fwrite(&TL1, sizeof(TL1), 1, fi);
        fwrite(&TL2, sizeof(TL2), 1, fi);
  }

    WriteUnits(fi);
    
    fwrite(&ActualTurn, sizeof(ActualTurn), 1, fi);
  
  return 0;
}



// doplni do tabulky k nacitani vzdy pouz. tereny:
void AddAllTimeTerrains(byte *TL1, byte *TL2)
{
    TL1[0] = 1; TL1[104] = 1; TL1[124] = 1; TL1[144] = 1; TL1[164] = 1; 
    TL1[184] = 1; TL1[192] = 1; TL1[200] = 1; TL1[208] = 1; TL1[216] = 1;
    TL1[224] = 1; TL1[240] = 1; TL1[248] = 1;
    
    TL2[93] = 1; TL2[94] = 1; TL2[95] = 1; TL2[96] = 1;
}



static char MR_missionpicture[20];

void *MissionRead(FILE *fi)
{
    int i, j, k;
    TField *f;
    byte TableL1[1024], TableL2[1024];
    
    StartLoading(MR_missionpicture);
    
    fread(&MapSizeX, 4, 1, fi); 
    fread(&MapSizeY, 4, 1, fi); 

  Map = (TField *) memalloc(MapSizeX * MapSizeY * sizeof(TField));
  lockmem(Map, MapSizeX * MapSizeY * sizeof(TField));
    
    fread(Map, MapSizeX * MapSizeY * sizeof(TField), 1, fi);
    UpdateLoading();
    for (i = 0; i < MapSizeX; i++)
        for (j = 0; j < MapSizeY; j++) {
            k = MapSizeX * j + i;
            Map[k].OnScreen = Map[k].HasHelper = 0;
            if (Map[k].Terrain2 == 285 /*mine*/) {
                Map[k].Terrain2 = 0;
                MinePlace(i, j, BADLIFE);
            }
            if (Map[k].Terrain2 == 286 /*another mine*/) {
                Map[k].Terrain2 = 0;
                MinePlace(i, j, GOODLIFE);
            }
            Map[k].Unit = NO_UNIT;
        }
    UpdateLoading();
    fread(&TableL1, sizeof(TableL1), 1, fi);
    UpdateLoading();
    fread(&TableL2, sizeof(TableL2), 1, fi);
    UpdateLoading();
    AddAllTimeTerrains(TableL1, TableL2);
    InitBitmaps(TableL1, TableL2);
    UpdateLoading();
  SetupMap();

    ReadUnits(fi);
    
    fread(&ActualTurn, sizeof(ActualTurn), 1, fi);
  UpdateLoading();

  LMapSizeX = ((MapSizeX > LITMAP_SIZE) ? MapSizeX : LITMAP_SIZE/*-2*/) + 2;
  LMapSizeY = ((MapSizeY > LITMAP_SIZE) ? MapSizeY : LITMAP_SIZE/*-2*/) + 2;
  LitMapBuf = memalloc(LMapSizeX * LMapSizeY);
  LitMapTrans = memalloc(LMapSizeX * LMapSizeY);
  memset(LitMapBuf, 0, LMapSizeX * LMapSizeY);
  for (i = 0; i < MapSizeX+2; i++) {
        ((byte*)LitMapBuf)[i] = clrWhite;
        ((byte*)LitMapBuf)[i + (MapSizeY+1) * LMapSizeX] = clrWhite;
    }
  for (i = 0; i < MapSizeY+2; i++) {
        ((byte*)LitMapBuf)[i * LMapSizeX] = clrWhite;
        ((byte*)LitMapBuf)[MapSizeX+1 + i * LMapSizeX] = clrWhite;
    }
      
  DoneLoading();
  TurnTime = MissionTime = 0;
    return (void*)&Map;
}




void InitMap(char *misname, FILE *fptr)
{
    if (fptr == NULL) {
        TDataFile MissionDF("missions.dat", dfOpenRead, NULL, '?', MissionWrite, MissionRead);
        sprintf(MR_missionpicture, "mis%i", ActualMission);
        MissionDF.get(misname);
    }
    else {
        strcpy(MR_missionpicture, "mis1");
        MissionRead(fptr);      
    }
}

void StoreMap(FILE *fptr)
{
//  MsgBox(SigText[TXT_SAVING]);
    MissionWrite(fptr, NULL/*dummy*/, 0/*dummy*/);
//  MsgBox(NULL);
}












/////////////////////////////// SKROLOVANI //////////////////////////////



// Pomocny seznam jednotek a special L2-spritu k vykresleni:

struct drawlistdef {
    int x, y, dy, dx;
    TField *f;
} drawlist[256];                 // sourad. policka a jeho offset
int drawlist_c = 0;              // jejich pocet



void quicksortu(int l, int r)
{
    int i = l, j = r, x = drawlist[(l+r) / 2].dy;
    struct drawlistdef y;
    
    do {
        while (drawlist[i].dy < x) i++;
        while (x < drawlist[j].dy) j--;
        if (i <= j) {
            y = drawlist[i]; drawlist[i] = drawlist[j]; drawlist[j] = y;
            i++, j--;
        }
    } while (!(i > j));
    if (l < j) quicksortu(l, j);
    if (i < r) quicksortu(i, r);
}



inline void AddTodrawlist(int x, int y)
{
    TField *f = GetField(x, y);
    
    if ((x < 0) || (y < 0) || (x >= MapSizeX) || (y >= MapSizeY)) return;
    
    if ((f->Unit != NO_UNIT) || 
        ((f->Terrain2 != 0) && (f->Terrain2 >= tofsL2Spec))) {
        if (f->Visib == 0) return;
      if ((f->Visib < 2) && (f->Unit != NO_UNIT) && ((Units[f->Unit]->Type % BADLIFE) < unRadar)) return;
            
        int rx = GetRelX(x), ry = GetRelY(y);
        int drawx = 28 * (rx - ry);
        int drawy = 14 * (rx + ry - (f->Height));
        
        drawlist[drawlist_c].x = x;
        drawlist[drawlist_c].y = y;
        drawlist[drawlist_c].dy = drawy;
        drawlist[drawlist_c].dx = drawx;        
        drawlist[drawlist_c].f = f;     
        drawlist_c++;
    }
}




    
TPoint NowPaintedUnitCoord;

void PaintUnits()
{
    int i;
    struct drawlistdef d;

    {
        int rx, ry, drawx, drawy;
        TMineRecord *r;
        TField *f;
        
        for (i = 0, r = MinesList; i < MinesCnt; i++, r++) {
            f = GetField(r->x, r->y);
            if (!f->OnScreen) continue;
            if ((r->party == GOODLIFE) ||
                ((f->Visib == 2) && (MineIsSeen(r->x, r->y, GOODLIFE)))) {
                rx = GetRelX(r->x), ry = GetRelY(r->y);
                drawx = 28 * (rx - ry) + 28;
                drawy = 14 * (rx + ry - (GetField(r->x, r->y)->Height)) + 14;
                DrawSprite(drawx, drawy, MineSprite);   
            }               
        }
    }

    if (drawlist_c != 0) {
        for (i = 0; i < drawlist_c; i++) {
            d = drawlist[i];
            if ((d.f->Terrain2 != 0) && (d.f->Terrain2 >= tofsL2Spec)) {
                if (d.f->Visib == 2)
                    DrawSprite(d.dx+28, d.dy+14, (TSprite*)BmpTerr2[d.f->Terrain2]);
                else if ((d.f->Visib == 1) && (!DontDrawL2[d.f->Terrain2])) 
                    DrawSprite(d.dx+28, d.dy+14, (TSprite*)BmpTerr2D[d.f->Terrain2]);
            }
            if (d.f->Unit != NO_UNIT) {
                NowPaintedUnitCoord.x = d.x, NowPaintedUnitCoord.y = d.y;
                Units[d.f->Unit]->Draw();
            }
        }
    }
    if (AirCursorOn && (AirCursorId != -1)) DrawSprite(AirCursorPos.x, AirCursorPos.y, AirCursors[AirCursorId]);
    for (i = 0; i < AircraftsCnt; i++) Aircrafts[i]->Draw();
    if (ProcessingExplosion) UpdateExplodes();
}


    
void PaintUnitsAbove(int x, int y)
{
    int i;
    struct drawlistdef d;
    
    if (drawlist_c != 0) {
        for (i = 0; i < drawlist_c; i++) {
            d = drawlist[i];
            if (!((d.x <= x) && (d.y <= y))) continue;
            if (d.f->Unit != NO_UNIT) {
                Units[d.f->Unit]->Draw();
            }
            if ((d.f->Terrain2 != 0) && (d.f->Terrain2 >= tofsL2Spec)) {
                if (d.f->Visib == 2)
                    DrawSprite(d.dx+28, d.dy+14, (TSprite*)BmpTerr2[d.f->Terrain2]);
                else if ((d.f->Visib == 1) && (!DontDrawL2[d.f->Terrain2])) 
                    DrawSprite(d.dx+28, d.dy+14, (TSprite*)BmpTerr2D[d.f->Terrain2]);
            }
        }
    }
}


    
void PaintUnitsBelow(int x, int y)
{
    int i;
    struct drawlistdef d;
    
    if (drawlist_c != 0) {
        for (i = 0; i < drawlist_c; i++) {
            d = drawlist[i];
            if ((d.x <= x) && (d.y <= y)) continue;
            if (d.f->Unit != NO_UNIT) {
                Units[d.f->Unit]->Draw();
            }
            if ((d.f->Terrain2 != 0) && (d.f->Terrain2 >= tofsL2Spec)) {
                if (d.f->Visib == 2)
                    DrawSprite(d.dx+28, d.dy+14, (TSprite*)BmpTerr2[d.f->Terrain2]);
                else if ((d.f->Visib == 1) && (!DontDrawL2[d.f->Terrain2])) 
                    DrawSprite(d.dx+28, d.dy+14, (TSprite*)BmpTerr2D[d.f->Terrain2]);
            }
        }
    }
    if (AirCursorOn && (AirCursorId != -1)) DrawSprite(AirCursorPos.x, AirCursorPos.y, AirCursors[AirCursorId]);
    for (i = 0; i < AircraftsCnt; i++) Aircrafts[i]->Draw();
}




void GenerateUnitsList()
{
    TField *f;
    TPoint a, b;
    int i;

    // generuj seznam:
    drawlist_c = 0;
    for (i = 0; i < RNG_WD1; i++) {
        b = RangeDownIn[i]; b.x++, b.y++;
        for (a = RangeUpIn[i]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++)
            AddTodrawlist(a.x, a.y);
    }
    for (i = 0; i < RNG_WD2; i++) {
        b = RangeDownOut[i]; b.x++, b.y++;
        for (a = RangeUpOut[i]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++) 
            AddTodrawlist(a.x, a.y);
    }
    if (drawlist_c != 0) quicksortu(0, drawlist_c-1);
}








// Pomucka: nahlasi ofset prvniho fakt kresleneho pixelu policka:
int GetDrawUpOfs(int x, int y)
{
    TField *f = GetField(x, y);
    return (14 * (GetRelY(y) + GetRelX(x) - (f->Height)) +
            TerrOfssBeg[f->Terrain]);
}

int GetDrawDownOfs(int x, int y)
{
    TField *f = GetField(x, y); 
    return (14 * (GetRelY(y) + GetRelX(x) - (f->Height)) +
            TerrOfssEnd[f->Terrain]);
}







// Apdejtne hodnoty TField.OnScr:
void UpdateOnScrValue()
{
    int i;
    TPoint a, b;

    for (i = 0; i < MapSizeX * MapSizeY; i++) Map[i].OnScreen = 0;
    for (i = 0; i < RNG_WD1; i++) {
        b = RangeDownIn[i];
        for (a = RangeUpIn[i]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++)
            GetField(a.x, a.y)->OnScreen = 1;
    }
    for (i = 0; i < RNG_WD2; i++) {
        b = RangeDownOut[i];
        for (a = RangeUpOut[i]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++) 
            GetField(a.x, a.y)->OnScreen = 1;
    }
}
    




////////////////////////////////////////////////
/// skroling



// Naskroluje mapu NA danou pozici:

void ScrollTo(int x, int y)
{
    if (DrawLocks) return;
    
    int i;
    TPoint a, b;

    SelectField(0, 0); // vynul. SelF.
    
    MapPos.x = x; MapPos.y = y; 
    
    // zjistit horni hranici oblasti:
    for (i = 0, a.x = MapPos.x, a.y = MapPos.y; i < RNG_WD1; i++, a.x++, a.y--) {
        for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.y++, b.x++) {}  
        RangeUpIn[i] = b;
    }
    for (i = 0, a.x = MapPos.x-1, a.y = MapPos.y; i < RNG_WD2; i++, a.x++, a.y--) {
        for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.x++, b.y++) {}  
        RangeUpOut[i] = b;
    }
    
    // ...a analogicky dolni hranici:
    for (i = 0, a.y = MapPos.y+RNG_HG, a.x = MapPos.x+RNG_HG; i < RNG_WD1; i++, a.x++, a.y--) {
        for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
        RangeDownIn[i] = b;
    }
    for (i = 0, a.x = MapPos.x+RNG_HG, a.y = MapPos.y+RNG_HG+1; i < RNG_WD2; i++, a.x++, a.y--) {
        for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
        RangeDownOut[i] = b;
    }
    

    // Vykreslit prostor na obrazovce:
    {
        // a) "plne" radky:
        for (i = 0; i < RNG_WD1; i++) {
            b = RangeDownIn[i];
            for (a = RangeUpIn[i]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++)
                DrawField(a.x, a.y);
        }
        // b) ty precuhujici:
        for (i = 0; i < RNG_WD2; i++) {
            b = RangeDownOut[i];
            for (a = RangeUpOut[i]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++) 
                DrawField(a.x, a.y);
        }
    }
    
    UpdateOnScrValue();

    GenerateUnitsList();
    ClearLastDraws();
    StartDraw();//fullscreen
    PaintUnits();
    
    MouseFreeze(VIEW_X_POS, VIEW_Y_POS, VIEW_SX, VIEW_SY);
    DrawMapBuf();
    MouseUnfreeze();

    UpdateLitMap();
    MouseFreeze(LITMAP_X, LITMAP_Y, LITMAP_SIZE, LITMAP_SIZE);
    DrawLitMap();
    MouseUnfreeze();
    
    SelectField(Mouse.x, Mouse.y);
}







// Naskroluje mapu O dany OFSET:
// pozn: rx, ry jsou v souradne soustave OBRAZOVKY!!!

void ScrollRel(int rx, int ry)
{
    int i, j;
    TPoint a, b;

    MapPos.x += rx;
    MapPos.y -= rx;
    
    // ---- doleva:
    if (rx < 0) {
        // Odskrolovani:
        memmove((byte*)MapBuf + FIELD_X, MapBuf, VIEW_PIXSZ_X * VIEW_PIXSZ_Y - FIELD_X);
        memmove((byte*)LocalBufX + FIELD_X/2, LocalBufX, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2 - FIELD_X/2);
        memmove((byte*)LocalBufY + FIELD_X/2, LocalBufY, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2 - FIELD_X/2);

        // Zjisti okraje: (stejne jako ve ScrollTo):
        for (i = 0, a.x = MapPos.x, a.y = MapPos.y; i < RNG_WD1; i++, a.x++, a.y--) {
            for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.y++, b.x++) {}  
            RangeUpIn[i] = b;
        }
        for (i = 0, a.x = MapPos.x-1, a.y = MapPos.y; i < RNG_WD2; i++, a.x++, a.y--) {
            for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.x++, b.y++) {}  
            RangeUpOut[i] = b;
        }
        for (i = 0, a.y = MapPos.y+RNG_HG, a.x = MapPos.x+RNG_HG; i < RNG_WD1; i++, a.x++, a.y--) {
            for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
            RangeDownIn[i] = b;
        }
        for (i = 0, a.x = MapPos.x+RNG_HG, a.y = MapPos.y+RNG_HG+1; i < RNG_WD2; i++, a.x++, a.y--) {
            for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
            RangeDownOut[i] = b;
        }

        // Vykresleni:
        b = RangeDownIn[0]; 
        for (a = RangeUpIn[0]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++) 
            DrawField(a.x, a.y);
        b = RangeDownOut[0]; 
        for (a = RangeUpOut[0]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++) 
            DrawField(a.x, a.y);
    }


    // ---- doprava:
    else if (rx > 0) {
        // Odskrolovani:
        memmove(MapBuf, (byte*)MapBuf + FIELD_X, VIEW_PIXSZ_X * VIEW_PIXSZ_Y - FIELD_X);
        memmove(LocalBufX, (byte*)LocalBufX + FIELD_X/2, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2 - FIELD_X/2);
        memmove(LocalBufY, (byte*)LocalBufY + FIELD_X/2, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2 - FIELD_X/2);

        // Zjisti okraje: (stejne jako ve ScrollTo):
        for (i = 0, a.x = MapPos.x, a.y = MapPos.y; i < RNG_WD1; i++, a.x++, a.y--) {
            for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.y++, b.x++) {}  
            RangeUpIn[i] = b;
        }
        for (i = 0, a.x = MapPos.x-1, a.y = MapPos.y; i < RNG_WD2; i++, a.x++, a.y--) {
            for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.x++, b.y++) {}  
            RangeUpOut[i] = b;
        }
        for (i = 0, a.y = MapPos.y+RNG_HG, a.x = MapPos.x+RNG_HG; i < RNG_WD1; i++, a.x++, a.y--) {
            for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
            RangeDownIn[i] = b;
        }
        for (i = 0, a.x = MapPos.x+RNG_HG, a.y = MapPos.y+RNG_HG+1; i < RNG_WD2; i++, a.x++, a.y--) {
            for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
            RangeDownOut[i] = b;
        }

        // Vykresleni:
        b = RangeDownIn[RNG_WD1-1]; 
        for (a = RangeUpIn[RNG_WD1-1]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++) 
            DrawField(a.x, a.y);
        b = RangeDownOut[RNG_WD2-1];
        for (a = RangeUpOut[RNG_WD2-1]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++) 
            DrawField(a.x, a.y);
    }
    
    
    MapPos.x += ry;
    MapPos.y += ry;
    // ---- nahoru:
    if (ry < 0) {
        memmove((byte*)MapBuf + FIELD_Y * VIEW_PIXSZ_X, MapBuf, VIEW_PIXSZ_X * VIEW_PIXSZ_Y - FIELD_Y * VIEW_PIXSZ_X);
        memmove((byte*)LocalBufX + FIELD_Y/2 * VIEW_PIXSZ_X/2, LocalBufX, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2 - FIELD_Y/2 * VIEW_PIXSZ_X/2);
        memmove((byte*)LocalBufY + FIELD_Y/2 * VIEW_PIXSZ_X/2, LocalBufY, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2 - FIELD_Y/2 * VIEW_PIXSZ_X/2);

        // zjistit horni hranici oblasti:
        for (i = 0, a.x = MapPos.x, a.y = MapPos.y; i < RNG_WD1; i++, a.x++, a.y--) {
            for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.y++, b.x++) {}  
            RangeUpIn[i] = b;
        }
        for (i = 0, a.x = MapPos.x-1, a.y = MapPos.y; i < RNG_WD2; i++, a.x++, a.y--) {
            for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.x++, b.y++) {}  
            RangeUpOut[i] = b;
        }
        // ...a analogicky dolni hranici:
        for (i = 0, a.y = MapPos.y+RNG_HG, a.x = MapPos.x+RNG_HG; i < RNG_WD1; i++, a.x++, a.y--) {
            for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
            RangeDownIn[i].x = b.x;
            RangeDownIn[i].y = b.y;
        }
        for (i = 0, a.x = MapPos.x+RNG_HG, a.y = MapPos.y+RNG_HG+1; i < RNG_WD2; i++, a.x++, a.y--) {
            for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
            RangeDownOut[i].x = b.x;
            RangeDownOut[i].y = b.y;
        }

        // Vykresleni:
        for (i = 0; i < RNG_WD1; i++) {
            a = RangeUpIn[i];
            DrawField(a.x, a.y);
            DrawField(a.x+1, a.y+1);
        }
        for (i = 0; i < RNG_WD2; i++) {
            a = RangeUpOut[i];
            DrawField(a.x, a.y);
            DrawField(a.x+1, a.y+1);
        }
    }

    // ---- dolu:
    else if (ry > 0) {
        memmove(MapBuf, (byte*)MapBuf + FIELD_Y * VIEW_PIXSZ_X, VIEW_PIXSZ_X * VIEW_PIXSZ_Y - FIELD_Y * VIEW_PIXSZ_X);
        memmove(LocalBufX, (byte*)LocalBufX + FIELD_Y/2 * VIEW_PIXSZ_X/2, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2 - FIELD_Y/2 * VIEW_PIXSZ_X/2);
        memmove(LocalBufY, (byte*)LocalBufY + FIELD_Y/2 * VIEW_PIXSZ_X/2, VIEW_PIXSZ_X/2 * VIEW_PIXSZ_Y/2 - FIELD_Y/2 * VIEW_PIXSZ_X/2);

        // zjistit horni hranici oblasti:
        for (i = 0, a.x = MapPos.x, a.y = MapPos.y; i < RNG_WD1; i++, a.x++, a.y--) {
            for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.y++, b.x++) {}  
            RangeUpIn[i] = b;
        }
        for (i = 0, a.x = MapPos.x-1, a.y = MapPos.y; i < RNG_WD2; i++, a.x++, a.y--) {
            for (b = a; GetDrawDownOfs(b.x, b.y) < 0; b.x++, b.y++) {}  
            RangeUpOut[i] = b;
        }
        // ...a analogicky dolni hranici:
        for (i = 0, a.y = MapPos.y+RNG_HG, a.x = MapPos.x+RNG_HG; i < RNG_WD1; i++, a.x++, a.y--) {
            for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
            RangeDownIn[i].x = b.x;
            RangeDownIn[i].y = b.y;
        }
        for (i = 0, a.x = MapPos.x+RNG_HG, a.y = MapPos.y+RNG_HG+1; i < RNG_WD2; i++, a.x++, a.y--) {
            for (b = a; GetDrawUpOfs(b.x, b.y) < VIEW_SY-1; b.x++, b.y++) {}            
            RangeDownOut[i].x = b.x;
            RangeDownOut[i].y = b.y;
        }

        // Vykresleni:      
        for (i = 0; i < RNG_WD1; i++) {
            a = RangeDownIn[i]; a.x--, a.y--;
            DrawField(a.x, a.y);
            DrawField(a.x-1, a.y-1);
        }
        for (i = 0; i < RNG_WD2; i++) {
            a = RangeDownOut[i]; a.x--, a.y--;
            DrawField(a.x, a.y);
            DrawField(a.x-1, a.y-1);
        }
    }

    UpdateOnScrValue();

    GenerateUnitsList();
    ClearLastDraws();
    StartDraw();//fullscreen
    PaintUnits();

    // Finalni nakresleni:  
    MouseFreeze(VIEW_X_POS, VIEW_Y_POS, VIEW_SX, VIEW_SY);
    DrawMapBuf();
    MouseUnfreeze();
}











///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


#include "eng800.cpp"

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////









/////////////////////////////// SPRITY /////////////////////////////////


TRect ScreenArea = {0,0,VIEW_SX-1,VIEW_SY-1}; //x1,y1,x2,y2
TRect DrawingArea;
unsigned int DrawingID = 0;


static void CopyMapToFull()
{
    byte *s = (byte*)MapBuf + (VIEW_OFS_X + DrawingArea.x1) + (VIEW_OFS_Y + DrawingArea.y1) * VIEW_PIXSZ_X;
    byte *t = (byte*)FullBuf + (VIEW_OFS_X + DrawingArea.x1) + (VIEW_OFS_Y + DrawingArea.y1) * VIEW_PIXSZ_X;
    int sx = DrawingArea.x2 - DrawingArea.x1 + 1,
        sy = DrawingArea.y2 - DrawingArea.y1 + 1;
    int d = VIEW_PIXSZ_X - sx;
    register int x, y;
    
    for (y = sy; y != 0; y--, t += d, s += d)
        for (x = sx; x != 0; x--)   
            *(t++) = *(s++);
}


void StartDraw(int x, int y, int w, int h)
{
    ++DrawingID;
    DrawingArea.x1 = x;
    DrawingArea.y1 = y;
    DrawingArea.x2 = x + w-1;
    DrawingArea.y2 = y + h-1;
    if (!Intersect(&DrawingArea, &ScreenArea)) {
        DrawingArea.x1 = 0;
        DrawingArea.y1 = 0;
        DrawingArea.x2 = 8;
        DrawingArea.y2 = 8;
    };
    // vymazani spritu v oblasti:
    CopyMapToFull();
}



void StartDraw()
{
    if (++DrawingID == 0) DrawingID = 1;
    DrawingArea.x1 = 0;
    DrawingArea.y1 = 0;
    DrawingArea.x2 = VIEW_SX-1;
    DrawingArea.y2 = VIEW_SY-1;
    // vymazani spritu v oblasti:
    memcpy(FullBuf, MapBuf, VIEW_PIXSZ_X * VIEW_PIXSZ_Y);
}




void EndDraw()
{
    if (DrawLocks) return;
    int x = DrawingArea.x1, y = DrawingArea.y1;
    int w = DrawingArea.x2 - x + 1, h = DrawingArea.y2 - y + 1;
    MouseFreeze(VIEW_X_POS + x, VIEW_Y_POS + y, w, h);
    if (w % 4 == 0)
        DrawMapBufP32(VIEW_OFS_X + x, VIEW_OFS_Y + y, w, h);
    else
        DrawMapBufP(VIEW_OFS_X + x, VIEW_OFS_Y + y, w, h);
    MouseUnfreeze();
}








/////////////////////////////// POLICKA ////////////////////////////////

TField *GetField(int x, int y)
{
    if (x < 0) {
        if (y < 0)  
            return &ZeroUL;
        else if (y >= MapSizeY) 
            return &ZeroDL;
        else return ZeroLeft + y;
    }
    else if (x >= MapSizeX) {
        if (y < 0) 
            return &ZeroUR;
        else if (y >= MapSizeY) 
            return &ZeroDR;
        else return ZeroRight + y;
    }
    else if (y < 0)
        return ZeroUp + x;
    else if (y >= MapSizeY) 
        return ZeroDown + x;
    else
        return Map + (y * MapSizeX + x);
}








void DrawFieldOcup(int x, int y)
{
    TField *f = GetField(x, y);
    word Ter1 = f->Terrain;
    int rx = GetRelX(x), ry = GetRelY(y);
    int drawx = 28 * (rx - ry);
    int drawy = 14 * (rx + ry - (f->Height));
    int sx = FIELD_X, sy = TerrOfssEnd[Ter1];

    drawy += TerrOfssBeg[Ter1];

    if (drawx < 0) {sx += drawx; drawx = 0;}
    else if (drawx+sx >= VIEW_SX) sx = VIEW_SX-drawx;
    if (drawy < 0) {sy += drawy; drawy = 0;}
    else if (drawy+sy >= VIEW_SY) sy = VIEW_SY-drawy;

    StartDraw(drawx, drawy, sx, sy);
    PaintUnits();
}



void DrawFieldSafe(int x, int y)
{
    DrawField(x, y);
    DrawFieldOcup(x, y);
}



void DrawFieldSafe2(int x, int y)
{
    TField *f;
    TRect rf, r;
    
    rf.x1 = 28 * (GetRelX(x-1) - GetRelY(y));
    rf.x2 = rf.x1 + 2 * FIELD_X - 1;
    f = GetField(x-1, y-1);
    rf.y1 = 14 * (GetRelX(x-1) + GetRelY(y-1) - f->Height) +
            TerrOfssBeg[f->Terrain];
    f = GetField(x, y);
    rf.y2 = 14 * (GetRelX(x) + GetRelY(y) - f->Height) +
            TerrOfssBeg[f->Terrain] + TerrOfssEnd[f->Terrain] - 1;

    if (GetField(x  , y  )->OnScreen) DrawField(x  , y  );
    if (GetField(x-1, y-1)->OnScreen) DrawField(x-1, y-1);
    if (GetField(x-1, y  )->OnScreen) DrawField(x-1, y  );
    if (GetField(x  , y-1)->OnScreen) DrawField(x  , y-1);

//  f = GetField(x, y); <- done above
    if (f->Unit != NO_UNIT) {
        GetSpriteRect(&r, Units[f->Unit]->GetSprite(), x, y,
                    Units[f->Unit]->LittleX, Units[f->Unit]->LittleY);
        Union(&rf, &r);
    }
    if ((f->Terrain2 != 0) && (f->Terrain2 >= tofsL2Spec)) {
        GetSpriteRect(&r, (TSprite*)BmpTerr2[f->Terrain2], x, y, 0, 0);
        Union(&rf, &r);
    }

    f = GetField(x-1, y-1);
    if (f->Unit != NO_UNIT) {
        GetSpriteRect(&r, Units[f->Unit]->GetSprite(), x-1, y-1,
                    Units[f->Unit]->LittleX, Units[f->Unit]->LittleY);
        Union(&rf, &r);
    }
    if ((f->Terrain2 != 0) && (f->Terrain2 >= tofsL2Spec)) {
        GetSpriteRect(&r, (TSprite*)BmpTerr2[f->Terrain2], x, y, 0, 0);
        Union(&rf, &r);
    }

    f = GetField(x-1, y);
    if (f->Unit != NO_UNIT) {
        GetSpriteRect(&r, Units[f->Unit]->GetSprite(), x-1, y,
                    Units[f->Unit]->LittleX, Units[f->Unit]->LittleY);
        Union(&rf, &r);
    }
    if ((f->Terrain2 != 0) && (f->Terrain2 >= tofsL2Spec)) {
        GetSpriteRect(&r, (TSprite*)BmpTerr2[f->Terrain2], x, y, 0, 0);
        Union(&rf, &r);
    }

    f = GetField(x, y-1);
    if (f->Unit != NO_UNIT) {
        GetSpriteRect(&r, Units[f->Unit]->GetSprite(), x, y-1,
                    Units[f->Unit]->LittleX, Units[f->Unit]->LittleY);
        Union(&rf, &r);
    }
    if ((f->Terrain2 != 0) && (f->Terrain2 >= tofsL2Spec)) {
        GetSpriteRect(&r, (TSprite*)BmpTerr2[f->Terrain2], x, y, 0, 0);
        Union(&rf, &r);
    }
    
    StartDraw(rf);
    PaintUnits();
}



void DrawFieldS(int x, int y)
{
    TField *f = GetField(x, y);
    word Ter1 = f->Terrain;
    int rx = GetRelX(x), ry = GetRelY(y);
    int drawx = 28 * (rx - ry);
    int drawy = 14 * (rx + ry - (f->Height));
    int sx = FIELD_X, sy = TerrOfssEnd[Ter1];
    
    drawy += TerrOfssBeg[Ter1];
    if (f->Unit != NO_UNIT) { drawy -= 28; sy += 28;}
    if ((f->Terrain2 != 0) && (f->Terrain2 >= tofsL2Spec)) {drawy -= 28; sy += 28;}

    if (drawx < 0) {sx += drawx; drawx = 0;}
    else if (drawx+sx >= VIEW_SX) sx = VIEW_SX-drawx;
    if (drawy < 0) {sy += drawy; drawy = 0;}
    else if (drawy+sy >= VIEW_SY) sy = VIEW_SY-drawy;

    EndDraw();
}








///////////////////////////// OKRAJE - CERNO

TField *ZeroUp, *ZeroDown, *ZeroLeft, *ZeroRight;
TField ZeroUL, ZeroUR, ZeroDL, ZeroDR;

void SetupMap()
{
    int i;
    TField *f;
    
    ZeroUp = (TField*) memalloc(sizeof(TField) * MapSizeX);
    ZeroDown = (TField*) memalloc(sizeof(TField) * MapSizeX);
    ZeroLeft = (TField*) memalloc(sizeof(TField) * MapSizeY);
    ZeroRight = (TField*) memalloc(sizeof(TField) * MapSizeY);
    

    for (i = 0; i < MapSizeY; i++) {
        f = GetField(0, i);
        ZeroLeft[i].Height = f->Height;
        ZeroLeft[i].Terrain2 = 0;
        ZeroLeft[i].Unit = NO_UNIT;
        ZeroLeft[i].IsAnim = 0;
        ZeroLeft[i].Visib = 2;
        ZeroLeft[i].HasHelper = 0;
        if (L1TerrainType[f->Terrain] == tofsL1D) 
            ZeroLeft[i].Terrain = tofsL1D;
        else if (L1TerrainType[f->Terrain] == tofsL1B) 
            ZeroLeft[i].Terrain = tofsL1B;
        else
            ZeroLeft[i].Terrain = 0;
        if (((f->Terrain % 256) >= tofsL1F) && ((f->Terrain % 256) < tofsL1J))
            ZeroLeft[i].Height--;

        f = GetField(MapSizeX-1, i);
        ZeroRight[i].Height = f->Height;
        ZeroRight[i].Terrain2 = 0;
        ZeroRight[i].Unit = NO_UNIT;
        ZeroRight[i].IsAnim = 0;
        ZeroRight[i].Visib = 2;
        ZeroRight[i].HasHelper = 0;
        if (L1TerrainType[f->Terrain] == tofsL1D) 
            ZeroRight[i].Terrain = tofsL1D;
        else if (L1TerrainType[f->Terrain] == tofsL1B) 
            ZeroRight[i].Terrain = tofsL1B;
        else 
            ZeroRight[i].Terrain = 0;
        if (((f->Terrain % 256) >= tofsL1F) && ((f->Terrain % 256) < tofsL1J))
            ZeroRight[i].Height--;
    }

    for (i = 0; i < MapSizeX; i++) {
        f = GetField(i, 0);
        ZeroUp[i].Height = f->Height;
        ZeroUp[i].Terrain2 = 0;
        ZeroUp[i].Unit = NO_UNIT;
        ZeroUp[i].IsAnim = 0;
        ZeroUp[i].Visib = 2;
        ZeroUp[i].HasHelper = 0;
        if (((f->Terrain % 256) >= tofsL1E) && ((f->Terrain % 256) < tofsL1F))
            ZeroUp[i].Terrain = tofsL1E;
        else if (((f->Terrain % 256) >= tofsL1C) && ((f->Terrain % 256) < tofsL1D))
            ZeroUp[i].Terrain = tofsL1C;
        else
            ZeroUp[i].Terrain = 0;
        if (((f->Terrain % 256) >= tofsL1F) && ((f->Terrain % 256) < tofsL1J))
            ZeroUp[i].Height--;

        f = GetField(i, MapSizeY-1);
        ZeroDown[i].Height = f->Height;
        ZeroDown[i].Terrain2 = 0;
        ZeroDown[i].Unit = NO_UNIT;
        ZeroDown[i].IsAnim = 0;
        ZeroDown[i].Visib = 2;
        ZeroDown[i].HasHelper = 0;
        if (((f->Terrain % 256) >= tofsL1E) && ((f->Terrain % 256) < tofsL1F))
            ZeroDown[i].Terrain = tofsL1E;
        else if (((f->Terrain % 256) >= tofsL1C) && ((f->Terrain % 256) < tofsL1D))
            ZeroDown[i].Terrain = tofsL1C;
        else
            ZeroDown[i].Terrain = 0;
        if (((f->Terrain % 256) >= tofsL1F) && ((f->Terrain % 256) < tofsL1J))
            ZeroDown[i].Height--;
    }
    
    ZeroUL = ZeroUp[0]; ZeroUL.Terrain = 0;
    ZeroUR = ZeroUp[MapSizeX-1]; ZeroUR.Terrain = 0;
    ZeroDL = ZeroDown[0]; ZeroDL.Terrain = 0;
    ZeroDR = ZeroDown[MapSizeX-1]; ZeroDR.Terrain = 0;
}







/////////////////////////////// LOKALIZACE ///////////////////////////////

void *LocalBufX, *LocalBufY;
TPoint SelPos = {0xFF, 0xFF};



void DrawSelFrame(int x, int y, void *BmpSl[])
{
    TField *f = GetField(x, y);
    word Ter1 = f->Terrain;
    
    int rx = GetRelX(x), ry = GetRelY(y);
    int drawx = VIEW_OFS_X + 28 * (rx - ry);
    int drawy = VIEW_OFS_Y + 14 * (rx + ry - (f->Height));
    
    DrawField(x, y);
    DrawL2Selector(drawx, drawy, Ter1, BmpSl);
    DrawFieldOcup(x, y);
}



void DrawAirCursor(int isbold)
{
    if (!AirCursorOn) return;

    TField *f = GetField(SelPos.x, SelPos.y);
    int rx = GetRelX(SelPos.x), ry = GetRelY(SelPos.y);
    AirCursorPos.x = 28 * (rx - ry) + 28;
    AirCursorPos.y = 14 * (rx + ry) + 14;
    switch (AirCursorOn) {
        case 1 : AirCursorPos.y -= 14 * (f->Height + 1); break;
        case 2 : AirCursorPos.y -= 14 * 8; break;
    }

    AirCursorId = isbold ? 1 : 0;
    StartDraw(AirCursorPos.x - 28, AirCursorPos.y - 14, 56, 35);
    PaintUnits();
    EndDraw();
}


void UndrawAirCursor()
{
    if (!AirCursorOn) return;
    AirCursorId = -1;
    StartDraw(AirCursorPos.x - 28, AirCursorPos.y - 14, 56, 35);
    PaintUnits();
    EndDraw();
}





int SelFldLocks = 0;

void SelectField(int x, int y)
{
    TPoint NewSel;
    int ax, ay;
    int mcur, mselb;
    
    if (SelFldLocks) return;    
    if ((x >= VIEW_X_POS) && (y >= VIEW_Y_POS) && 
        (x < VIEW_X_POS + VIEW_SX) && (y < VIEW_Y_POS + VIEW_SY)) {   
        ax = VIEW_OFS_X/2 + (x-VIEW_X_POS)/2;
        ay = VIEW_OFS_Y/2 + (y-VIEW_Y_POS)/2;
        
        NewSel.x = *((byte*)LocalBufX+ax+ay*VIEW_PIXSZ_X/2);
        NewSel.y = *((byte*)LocalBufY+ax+ay*VIEW_PIXSZ_X/2);
        
        if ((NewSel.x == 0xFF) || (NewSel.y == 0xFF)) { // mimo mapu:
            if (SelPos.x != 0xFF) {
                UndrawAirCursor();
                DrawFieldSafe(SelPos.x, SelPos.y);      
                DrawFieldS(SelPos.x, SelPos.y);     
                SelPos.x = 0xFF;
                MouseSetCursor(mcurArrow);
            }           
        }
        else if ((NewSel.x != SelPos.x) || (NewSel.y != SelPos.y)) {
            if (SelPos.x != 0xFF) {
                UndrawAirCursor();
                DrawFieldSafe(SelPos.x, SelPos.y);
                DrawFieldS(SelPos.x, SelPos.y);
            }
            SelPos = NewSel;
            SelectedUnit->FieldSelected(SelPos.x, SelPos.y);
            SelectedUnit->GetCursor(SelPos.x, SelPos.y, &mcur, &mselb);
            DrawAirCursor(mselb);
            DrawSelFrame(SelPos.x, SelPos.y, mselb ? BmpSelBold : BmpSel);
            DrawFieldS(SelPos.x, SelPos.y);
            MouseSetCursor(mcur);
        }
    }
    else if (SelPos.x != 0xFF) {   // kurzor mimo oblast:
        MouseSetCursor(mcurArrow);
        UndrawAirCursor();
        DrawFieldSafe(SelPos.x, SelPos.y);
        DrawFieldS(SelPos.x, SelPos.y);
        SelPos.x = 0xFF;
    }
}






#define PAN_EXCENTRITY     100

int GetFieldPanning(int x, int y)
{
    int rrx = GetRelX(x), rry = GetRelY(y);
    int drawx = 28 * (rrx - rry) + 28;
    int pan = -PAN_EXCENTRITY + (2*PAN_EXCENTRITY * drawx / VIEW_SX);
    if (pan < -PAN_EXCENTRITY) pan = -PAN_EXCENTRITY;
    else if (pan > PAN_EXCENTRITY) pan = PAN_EXCENTRITY;
    return (128 + pan);
}






///////////////////////////////// LITTLEMAP //////////////////////////////


TPoint LitMapOfs = {0};
void *LitMapBuf = NULL, *LitMapTrans = NULL;
int LMapSizeX, LMapSizeY;


inline byte GetTerrIcon(TField *f)
{
    if (f->Visib == 2) 
        return (TabTerr2Icons[f->Terrain2] == 0) ? (TabTerrIcons[f->Terrain]) : TabTerr2Icons[f->Terrain2];   
    else if (f->Visib == 1)
        return (f->Terrain2 == 0) ? (TabTerrIconsD[f->Terrain]) : TabTerr2IconsD[f->Terrain2];
    else return 0;
}



inline void DrawLMPix(int x, int y, byte clr)
{
    if ((x < 0) || (y < 0) || 
        (x >= MapSizeX) || (y >= MapSizeY)) return;
    *((byte*)LitMapTrans + (x+1) + (y+1) * LMapSizeX) = clr;
}

inline void DrawLMPix2(int x, int y, byte clr)
{
    if ((x < 0) || (y < 0) || 
        (x >= MapSizeX) || (y >= MapSizeY)) return;
    *((byte*)LitMapBuf + (x+1) + (y+1) * LMapSizeX) = clr;
}



void UpdateLitMap(int DrawAll, int DoBigUnits)
{
    int i, ax, ay;
    TPoint a;
    TField *f;
    byte *ptr;
    
    if (DrawAll) {
        // Vlastni mapu:
        for (a.y = 0, f = GetField(0, 0); a.y < MapSizeY; a.y++) {
            ptr = (byte*)LitMapBuf + LMapSizeY * a.y;
            for (a.x = 0; a.x < MapSizeX; a.x++, ptr++, f++) 
                ((byte*)LitMapBuf)[(a.x+1) + (a.y+1) * LMapSizeX] = GetTerrIcon(f);
        }
        // Jednotky:
        for (i = GOODLIFE; i < BADLIFE; i++)
            if (Units[i] != NULL) {
                ax = Units[i]->X, ay = Units[i]->Y;
                if ((ax == -1) || (ay == -1)) continue;
                if (GetField(ax, ay)->Visib == 2) {
                    DrawLMPix2(ax,     ay    , clrWhite);
                    if (DoBigUnits) {
                        DrawLMPix2(ax + 1, ay    , clrWhite);
                        DrawLMPix2(ax    , ay + 1, clrWhite);
                        DrawLMPix2(ax + 1, ay + 1, clrWhite);
                    }
                }
            }
        for (i = BADLIFE; i < UNITS_TOP; i++)
            if (Units[i] != NULL) {
                if (Units[i]->Type % BADLIFE == unBarracuda) continue;
                ax = Units[i]->X, ay = Units[i]->Y;
                if ((ax == -1) || (ay == -1)) continue;
                if (GetField(ax, ay)->Visib == 2) {
                    DrawLMPix2(ax,     ay    , clrRed);
                    if (DoBigUnits) {
                        DrawLMPix2(ax + 1, ay    , clrRed);
                        DrawLMPix2(ax    , ay + 1, clrRed);
                        DrawLMPix2(ax + 1, ay + 1, clrRed);
                    }
                }
            }
    }
    memcpy(LitMapTrans, LitMapBuf, LMapSizeX * LMapSizeY);

    // Ramecek s aktualni polohou:
    for (a = MapPos, i = 0; i < VIEW_SIZE_X; i++, a.x++, a.y--) {
        DrawLMPix(a.x, a.y, clrRed);
        DrawLMPix(a.x + VIEW_SIZE_Y-1, a.y + VIEW_SIZE_Y-1, clrRed);
    }
    for (a = MapPos, i = 0; i < VIEW_SIZE_Y; i++, a.x++, a.y++) {
        DrawLMPix(a.x, a.y, clrRed);
        DrawLMPix(a.x + VIEW_SIZE_X-1, a.y - (VIEW_SIZE_X-1), clrRed);
    }

    while ((MapPos.x + (VIEW_SIZE_X+VIEW_SIZE_Y)/2+1) > LitMapOfs.x + 5 * LITMAP_SIZE / 6) LitMapOfs.x++;
    while ((MapPos.x + (VIEW_SIZE_X+VIEW_SIZE_Y)/2+1) < LitMapOfs.x + 1 * LITMAP_SIZE / 6) LitMapOfs.x--;
    while ((MapPos.y + (VIEW_SIZE_X+VIEW_SIZE_Y)/2-VIEW_SIZE_X+1) > LitMapOfs.y + 5 * LITMAP_SIZE / 6) LitMapOfs.y++;
    while ((MapPos.y + (VIEW_SIZE_X+VIEW_SIZE_Y)/2-VIEW_SIZE_X+1) < LitMapOfs.y + 1 * LITMAP_SIZE / 6) LitMapOfs.y--;

    if (LitMapOfs.x < 0) LitMapOfs.x = 0;
    else if (LitMapOfs.x >= LMapSizeX-LITMAP_SIZE+1) LitMapOfs.x = LMapSizeX - LITMAP_SIZE;
    if (LitMapOfs.y < 0) LitMapOfs.y = 0;
    else if (LitMapOfs.y >= LMapSizeY-LITMAP_SIZE+1) LitMapOfs.y = LMapSizeY - LITMAP_SIZE;
}






///////////////////// animace policek:

static int LastAnimTick = 0;
static int PMA_draws = 0;
#define ANIM_SPD   3


inline void AnimField(int x, int y)
{
    TField *f = GetField(x, y);
    
    if (f->IsAnim == 0) return;
    if ((f->Unit != NO_UNIT) && (Units[f->Unit]->IsAnimated()))
        Units[f->Unit]->DoAnim();
    if (f->Visib == 2) ProcessFieldAnim(x, y);
    PMA_draws++;
}



int ProcessMapAnim()
{
    UpdatePalette();
    if (TimerValue < LastAnimTick + ANIM_SPD) return FALSE;

    if (WC_On && (--WC_Period <= 0)) {
        WC_Period = 1 * rand() / RAND_MAX;
        if (WC_Phase == 1) WC_Phase = 2; else WC_Phase = 1;
        PutBitmap(RES_X-268, RES_Y+2-((iniResolution == SVGA_640x480) ? 18 : 22), WorkingControl[WC_Phase], 8, 8);
    }

    PMA_draws = 0;
    {
        TPoint a, b;
        int i;
        // a) "plne" radky:
        for (i = 0; i < RNG_WD1; i++) {
            b = RangeDownIn[i];
            for (a = RangeUpIn[i]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++)
                AnimField(a.x, a.y);
        }
        // b) ty precuhujici:
        for (i = 0; i < RNG_WD2; i++) {
            b = RangeDownOut[i];
            for (a = RangeUpOut[i]; (a.x != b.x) && (a.y != b.y); a.x++, a.y++) 
                AnimField(a.x, a.y);
        }
    }

    {
        for (int i = 0; i < AircraftsCnt; i++)
            if (Aircrafts[i]->IsAnimated()) {
                Aircrafts[i]->DoAnim();   
                PMA_draws++;
            }
    }

    LastAnimTick = TimerValue;
    return (PMA_draws > 0);
}
