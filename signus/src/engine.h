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
// Hlavickovy soubor pro ENGINE.CPP a ENGDRAW.ASM
//
// Obsah: graficky IZO-3D engine, vykreslovani mapy apod.
//



#ifndef _ENGINE_H
#define _ENGINE_H


#include "global.h"
#include "system.h"
#include "graphio.h"
#include "events.h"
#include "stream.h"




/////////////////////////////////////////
// Podpurne objekty a typy pro ENGINE
//


// Policko mapy:
#pragma pack(1)
typedef struct {
			word Terrain;        // prvni level
			word Terrain2;       // druha uroven terenu
			word Unit;           // where 0xFFFF = none
			byte Height    : 3;  // vyska terenu (0..7)
			byte IsAnim    : 1;  // je to animovane?
			byte Visib     : 2;  // viditelnost 00-nic, 01-sede, 11-plne
			byte OnScreen  : 1;  // je prave ted videt na obrazovce?
			byte HasHelper : 1;  // je na tomto policku videt oznac.
			                     // dostrelu, dojezdu?
} TField;
#pragma pack()

#define NO_UNIT     0xFFFF


// Sprite:
#pragma pack(1)
typedef struct {
			int32_t dx, dy, w, h;
			byte data[65536];
} TSprite;
#pragma pack()

extern void GetSpriteRect(TRect *r, TSprite *s, int x, int y, int lx = 0, int ly = 0);
	// zjisti, jakou oblast by zabiral sprite pri kresleni na policku x, y
	// s relativnim posunem lx, ly




/////////////////////////////////////////
// Konstanty s globalni platnosti
//


// Ofsety bitmap:
#define tofsL1A      0
#define tofsL1B    104
#define tofsL1C    124
#define tofsL1D    144
#define tofsL1E    164
#define tofsL1F    184
#define tofsL1G    192
#define tofsL1H    200
#define tofsL1I    208
#define tofsL1J    216
#define tofsL1K    224
#define tofsL1L    240
#define tofsL1M    248

#define tofsL2A      0
#define tofsL2B    192
#define tofsL2C    208
#define tofsL2D    224
#define tofsL2E    240
#define tofsL2Spec 256



// Rozmery policka:
#define FIELD_X       56
#define FIELD_Y       28

// Rozmery okna s mapou v polickach:
#define    VIEW_SIZE_X  12
#define    VIEW_SIZE_Y  19

// Rozmery kresliciho buf:
#define VIEW_PIXSZ_X 728  /* = ((VIEW_SIZE_X + 1) * FIELD_X) */
#define VIEW_PIXSZ_Y 616  /* = ((VIEW_SIZE_Y + 3) * FIELD_Y) */

// Pozice kresliciho buf. na obrazovce a skutecne rozmery:
#define VIEW_X_POS     9
#define VIEW_Y_POS    36

#define VIEW_SX     664   /* = VIEW_SIZE_X * FIELD_X */
#define VIEW_SY     532   /* = VIEW_SIZE_Y * FIELD_Y */


// Offset prvniho viditelneho pixelu v MapBuf:
#define VIEW_OFS_X    28
#define VIEW_OFS_Y    42







/////////////////////////////////////////
// Vykonna cast enginu
//


// Ofsety kresleni jednotlivych terenu vuci pocatku typu A:
extern int TerrOfssBeg[1024], TerrOfssEnd[1024];


// Aktualni barevna paleta - blik8ni:
extern int PulsarValue, PulsarDirection;

// Inicializace enginu a jeho casti:
extern int InitEngine(int mission, ReadStream *stream = NULL);
   // ma varianty: bud z DF podle misname nebo ze souboru
extern int DoneEngine();
extern void InitBitmaps(int MaskL1[], int MaskL2[]);
extern void DoneBitmaps();



// Buffer pro vykreslovani:
extern void *MapBuf;
extern void *FullBuf;


// Mapa a pomocne promnene:
extern int MapSizeX, MapSizeY;
extern TField *Map;

// Pozice v mape:
extern TPoint MapPos;

// Nynejsi tah:
extern int ActualTurn;
// Nynejsi mise:
extern int ActualMission;
// Nastavena obtiznost ('e','n','h'):
extern char ActualDifficulty;

// Casove udaje potrebne k zaznamu:
extern int TurnTime, MissionTime;

// Horni a dolni hranice viditelne oblasti:
extern TPoint RangeUpIn[20], RangeUpOut[20],
              RangeDownIn[20], RangeDownOut[20];
              // ta 20 je trochu vic, kvuli ruznych rozlisenim

// tyto konst se pouzivaji pri praci s ranges. Zavisi na rozliseni:
#define RNG_WD1      12
#define RNG_WD2      13
#define RNG_HG       18



////////////////// Lokalizace policka:

// Buffer pro lokalizaci:
extern void *LocalBufX, *LocalBufY;

// Pozice zamereneho (cervene) policka:
extern TPoint SelPos;

// Zjisti zamerene policko a barevne ho oznaci:
// Parametry jsou FYZICKE souradnice kurzoru mysi
extern void SelectField(int x, int y);
extern int SelFldLocks; // zamek na zamerovac

extern int AirCursorOn;	
		// urcuje, jestli je sloupovy kurzor - pro letadla





////////////////// Manipulace s mapou:

// Nacte/zapise mapu mise:
void InitMap(char *misname, ReadStream *stream);
void StoreMap(WriteStream &stream);
// Fce pro cteni a zapis misi do dataku:
unsigned MissionWrite(WriteStream &stream, void *ptr, size_t size);
void *MissionRead(ReadStream &stream);



// Skrolovani mapy:
// pozn.: souradnice jsou ve fyzickych (soust. sour. dle obrazovky)
extern void ScrollTo(int x, int y);
extern void ScrollRel(int rx, int ry);

extern int RedrawMapLocks;
inline void RedrawMap() {if (!RedrawMapLocks) ScrollTo(MapPos.x, MapPos.y);}


// Funkce sprazene s malou mapickou vpravo nahore:
#define LITMAP_SIZE   108
#define LITMAP_X      682
#define LITMAP_Y        9

extern void *LitMapBuf, *LitMapTrans;
extern int LMapSizeX, LMapSizeY;
// Ofset aktualne zobrazene casti mapicky:
extern TPoint LitMapOfs;

// UpdateListMap() vykresli do bufferu mapicku
extern void UpdateLitMap(int DrawAll = FALSE, int DoBigUnits = TRUE);

/////


// Pole pro okraje mapy:
extern TField *ZeroUp, *ZeroDown, *ZeroLeft, *ZeroRight;
extern TField ZeroUL, ZeroUR, ZeroDL, ZeroDR;
// Vygeneruje potrebna pole v zavislosti na mape:
extern void SetupMap();


// Kresli policko:
extern void DrawField(int x, int y);

// Kresli policko, zabezpeci prekryvani:
extern void DrawFieldSafe(int x, int y);
extern void DrawFieldSafe2(int x, int y);

// Kresli na policku selektor (SelectField, SelectedUnit):
extern void DrawL2Selector(int drawx, int drawy, word Ter1, void *BmpSl[]);


// Vykresli na obrazovku oblast policka:
extern void DrawFieldS(int x, int y);

// Vrati ukazatel na dane policko:
extern TField *GetField(int x, int y);

// Makra pro zjisteni ofsetu policka na obrazovce:
#define GetRelX(aX) ((aX) - MapPos.x)
#define GetRelY(aY) ((aY) - MapPos.y)


extern int GetFieldPanning(int x, int y);
	// vraci zvukovou odchylku pro tohle policko




////////////////// Datove promnene:

// Teren , bitmapy:
extern void *BmpTerr1[1024];
extern void *BmpTerr2[1024];
extern void *BmpTerr1D[1024];
extern void *BmpTerr2D[1024];

// Bitmapy cerveneho ramovani vybraneho terenu:
extern void *BmpSel[13], *BmpSelBold[13];




//////////////////////// Kresleni spritu:

// zamek na kresleni - pokud non0, tak EndDraw()
// nema zadny efektextern void LockDraw():
extern void LockDraw();
extern void UnlockDraw(int redraw = FALSE);

extern TRect ScreenArea;
extern TRect DrawingArea;
		// oblast, v niz se maji kreslit sprity
extern unsigned int DrawingID;
    // ID cislo kresleni. Pri kazdem kresleni se zvysuje o 1

extern "C" void PutSpritePart(void *screen, int sizes, void *data, int adding);
    // rutina pro interni pouziti, norm. se vola jen DrawSprite()

extern void DrawSprite(int x, int y, TSprite *s);
		// Vykresli sprite. X,Y jsou stred policka


extern void StartDraw(int x, int y, int w, int h);
		// Pripravi se na kresleni spritu, provede DrwID++
		// fce bez parametru obsahuje celou obrazovku:
inline void StartDraw();
inline void StartDraw(TRect r) {StartDraw(r.x1, r.y1, r.x2 - r.x1 + 1, r.y2 - r.y1 + 1);}

extern void EndDraw();
    // ukonci kresleni, provede vystup na obrazovku

// Fce pro praci se seznamem jednotek na obrazovce:
extern void PaintUnits();
extern void PaintUnitsAbove(int x, int y);
extern void PaintUnitsBelow(int x, int y);
extern void PaintUnits();
extern void GenerateUnitsList();

extern TPoint NowPaintedUnitCoord;
	// souradnice aktualne kresleneho policka, kdyz se kresli jednotka
	// (potrebuji to baraky a lode, aby se neshitilo kresleni

extern int ProcessMapAnim();
		// zabezpecuje animovani mapy


// Tabulky:

// Barvicky pro mapicku:
extern byte TabTerrIcons[1024], TabTerr2Icons[1024],
            TabTerrIconsD[1024], TabTerr2IconsD[1024];

// Pohybova narocnost terenu:
extern byte TabTerrMove[1024], TabTerr2Move[1024];


#endif
