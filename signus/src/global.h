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
// Hlavickovy soubor pro GLOBAL.CPP
//
// Obsah: Globalni herni definice, jako predek objektu apod.
//


#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "config.h"

#include "system.h"
#include "fonts.h"
#include "datafile.h"

#include "config.h"


//////////////////////////////////////////
// Nejruznejsi konstanty:
//

#include "consts.h"




//////////////////////////////////////////
// Datove typy:
//

// TPoint a sprazene fce:
typedef struct {int x, y;} TPoint;
typedef struct {int x1, y1, x2, y2;} TRect;
extern int Intersect(TRect *r1, TRect *r2);
extern void Union(TRect *r1, TRect *r2);
extern void Union2(int *x1, int *y1, int *w1, int *h1, int x2, int y2, int w2, int h2);
extern int IsInRect(int x, int y, int x1, int y1, int x2, int y2);
inline int IsInRect(int x, int y, TRect r) {return IsInRect(x, y, r.x1, r.y1, r.x2, r.y2);}


//////////////////////////////////////////
// Vseobecne funkce:
//

#define max(a,b)  ((a) > (b) ? (a) : (b))
#define min(a,b)  ((a) < (b) ? (a) : (b))


// zaokrouhli cislo:
int roundnum(double d);

extern FILE *fopensafe(const char *name, const char *mode);
  // Otevre soubor, umisteny bud na disku nebo na CD

extern bool fileExists(const char *name);

// Returns directory with Signus data:
extern const char *getSignusDataDir();

// Returns directory with Signus configuration
extern const char *getSignusConfigDir();

// hazeni kostkou:
extern int RollDice();


// zapnuti a vypnuti ukazatele zanebrazdnenosti (hodiny):
extern void WaitCursor(int turn_on);


////////////////////////////////////////////
// Inicializacni hodnoty a sprazene fce:
//

extern int iniVMM, VMMPhyMem;

extern int iniMouseRatioX, iniMouseRatioY;

extern char iniLanguage[2];
extern char iniLocale[200];

extern int iniIdleDelay;
extern int iniScrollDelay, iniAnimDelay, iniAnimDelay2;

extern int iniResolution;
#define SVGA_640x480   0x101
#define SVGA_800x600   0x103
#define SVGA_1024x768  0x105

extern int iniBrightCorr;

extern int iniMusicVol, iniSoundVol, iniSpeechVol;

extern int iniEnhancedGuiOn, iniShowStatusbar, iniShowMoveRange, iniShowShootRange,
                 iniShowVisibRange, iniStopOnNewEnemy;

extern int iniJukeboxRepeat, iniJukeboxRandom, iniJukeboxListSize, iniJukeboxSave;

extern int iniTitledAnims, iniInterpolateAnims;

extern bool LoadINI();
extern void SaveINI();
extern void ApplyINI();




/////////////////////////////////////////////////
// Vseobecne pouzivane datove soubory a datova pole
// POZOR! Ne vsechny promnene jsou inicializovany automaticky!
//        Napr. fontove tabulky nacte az volani metody InitGlobal()
//

#ifdef DEBUG
// Soubor pro ukladani Debug informaci 
extern FILE *dbgOutput;
#endif

// Datovy soubor s grafikou:
extern TDataFile *GraphicsDF, *GraphicsI18nDF;

// Datovy soubor s texty:
extern TDataFile *TextsDF;

// Oba textove fonty:
extern TFont *NormalFont;
extern TFont *HugeFont;
extern TFont *TinyFont;

// Datove pole zprav:
extern char *SigText[TXT_COUNT];
extern char *MsgText[MSG_COUNT];

// Citac tiku (55/s):
#define TICKS_PER_SEC  1000 / 55
extern volatile int TimerValue;

extern char *Palette;     // aktualni barevna paleta


/////////////////////////////////////////
// Globalni funkce
//

extern int InitGlobal();
extern int DoneGlobal();

#ifdef __win32__
    extern void EnterTCS();
    // = Enter Time Critical Section - za??tek bloku programu, kter2 mus? b?Ret rychle
    extern void LeaveTCS();
    // konec bloku
#else
    #define EnterTCS()
    #define LeaveTCS()
#endif


/////////////////////////////////////////
// Tabulky pro prevod barev v palete:
//

extern "C" byte DarkingTable[256], GrayingTable[256];
extern "C" void DoDarking(void *buf, int size);
extern "C" void DoGraying(void *buf, int size);




/////////////////////////////////////////
// Funkce & data pro komunikaci s uzivatelem
//

// Uzky prouzek nahore, kde se zobrazuje zprava:
#define MSGBUF_SX     (RES_X-136)
#define MSGBUF_SY      18
#define MSGBUF_X_POS    9
#define MSGBUF_Y_POS    9
extern void *MessageBuf;
// zobrazi nahore zpravu s danym kodem a prehraje prslusny text
extern void Message(int msg);
// zobrazi pouze text, bez zvuku:
extern void Message(char *msg);


// ukazatel prubehu nejake operace:
#define PROGRESS_SX      (RES_X-290)
#define PROGRESS_SY      6
#define PROGRESS_X_POS   9
#define PROGRESS_Y_POS   ((iniResolution == SVGA_640x480) ? 465 : (RES_Y - 19))
extern byte *ProgressBuf;
extern void ProgressNull();
extern void ProgressLimit(int limit);
extern void ProgressInc();
extern void ProgressSet(int value);


// Fce pro velkou hlasku na obrazovce:
// pozn: kdyz str = NULL, vymaze se.
#define MSGBOX_X_POS    80
#define MSGBOX_Y_POS   160
#define MSGBOX_SX      480
#define MSGBOX_SY      160
extern void MsgBox(char *str);




// Trojice fci pro nahravani mise: objevi se obrazek a na nem bezi
// progres bar...
extern void StartLoading(char *picname);
extern void UpdateLoading();
extern void DoneLoading();



// Ukazatele casu vpravo dole:
extern void ShowTime(int left, int right);

// Carteziansky snek
extern TPoint CartezianSnail (int i);



#ifdef DEBUG
extern void DEBUG_POINT(int param = 0);
        // universal. debugger - pro hledani fucking schovanych chyb
#endif


#endif
