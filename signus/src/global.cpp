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



// Globalni definice, deklarace a funkce pro Signus


#include "headers.h"

#include "global.h"
#include "events.h"
#include "graphio.h"
#include "sound.h"
#include "mouse.h"

#include <SDL_timer.h>
#include <math.h>
#include <sys/stat.h>

extern "C" {
#include "iniparser.h"
}


#define MIN_MEM_NEEDED     (1024 * 1024 * 12)
        // minim. pamet potrebna pro signus



#ifdef DEBUG
/////////////////////////// DEBUG SECTION ///////////////////////////

#include <time.h>

FILE *dbgOutput;


void dbgReport()
{
    fprintf(dbgOutput, 
        "\n ---=== SIGNUS DEBUGGING REPORT FILE ===--- \n\n" 
        "Memory needed: %i bytes (%ikB)\n"
        "  [ physical memory needed - locking: %i bytes (%iKB) ]\n"
        "Actually allocated memory (should be 0): %i bytes\n"
        "Running time: %i ticks (%i seconds)\n",
        dbgMaxMemAlloc, dbgMaxMemAlloc / 1024,      
        dbgMaxMemLocked, dbgMaxMemLocked / 1024,
        dbgMemAlloc,
        clock(), clock() / CLOCKS_PER_SEC);
}

#endif





/////////////////////// INI

int iniVMM, iniVMMPhyMem;

int iniMouseRatioX, iniMouseRatioY;

char iniLanguage[2];
char iniLocale[200];

int iniIdleDelay;
int iniScrollDelay, iniAnimDelay, iniAnimDelay2;

int iniMusicVol, iniSoundVol, iniSpeechVol;
        
int iniEnhancedGuiOn, iniShowStatusbar, iniShowMoveRange, iniShowShootRange,
    iniShowVisibRange, iniStopOnNewEnemy;

int iniJukeboxRandom, iniJukeboxRepeat, iniJukeboxListSize, iniJukeboxSave;

int iniResolution;

int iniBrightCorr;

int iniTitledAnims, iniInterpolateAnims;




bool dirExists(const char *filename)
{
    struct stat st;
    return stat(filename, &st) == 0 && S_ISDIR(st.st_mode);
}

static const char *GetConfigFileName()
{
    char *home = getenv("HOME");
    if (!home) home = ".";

    static char inifile[1024] = "";
    
    if (*inifile == 0)
    {
        strncpy(inifile, getSignusConfigDir(), 1024);
        strncat(inifile, "/signus.ini", 1024);
    }
    
    return inifile;
}

bool LoadINI()
{
    dictionary *dict = NULL;
    
    const char *configname = GetConfigFileName();
    if (fileExists(configname))
        dict = iniparser_load((char*)configname);
    if (dict == NULL)
        dict = iniparser_load(SIGNUS_DATA_DIR "/default_signus.ini");
    if (dict == NULL)
    {
        fprintf(stderr, "Fatal error: cannot read configuration file.\n"
                        "Please reinstall Signus.\n");
        return FALSE;
    }
    
    // FIXME -- use locales!!!
    strcpy(iniLanguage, "e");
    
    //iniResolution = iniparser_getint(dict, "video:resolution", -1);
    iniResolution = SVGA_800x600; // FIXME -- get rid of iniResolution !!
    
    iniBrightCorr = iniparser_getint(dict, "video:brightness", -1);
    iniTitledAnims = iniparser_getint(dict, "video:anims_titled", -1);
    iniInterpolateAnims = iniparser_getint(dict, "video:anims_interpolated", -1);

    iniMusicVol = iniparser_getint(dict, "audio:music_volume", -1);
    iniSoundVol = iniparser_getint(dict, "audio:sound_volume", -1);
    iniSpeechVol = iniparser_getint(dict, "audio:speech_volume", -1);
    iniJukeboxRepeat = iniparser_getint(dict, "audio:jukebox_repeat", -1);
    iniJukeboxRandom = iniparser_getint(dict, "audio:jukebox_random_order", -1);
    iniJukeboxListSize = iniparser_getint(dict, "audio:jukebox_play_list_size", -1);
    iniJukeboxSave = iniparser_getint(dict, "audio:jukebox_save_changes", -1);

    iniAnimDelay = iniparser_getint(dict, "interface:anim_delay", -1);
    iniAnimDelay2 = iniparser_getint(dict, "interface:anim_delay2", -1);
    iniIdleDelay = iniparser_getint(dict, "interface:idle_delay", -1);
    iniScrollDelay = iniparser_getint(dict, "interface:scroll_delay", -1);
    iniEnhancedGuiOn = iniparser_getint(dict, "interface:enable_anim_gui", -1);
    iniShowStatusbar = iniparser_getint(dict, "interface:unit_status_bar", -1);
    iniShowMoveRange = iniparser_getint(dict, "interface:unit_move_rng", -1);
    iniShowShootRange = iniparser_getint(dict, "interface:unit_shoot_rng", -1);
    iniShowVisibRange = iniparser_getint(dict, "interface:unit_visib_rng", -1);
    iniStopOnNewEnemy = iniparser_getint(dict, "interface:stop_on_new_enemy", -1);

    iniparser_freedict(dict);
    
    strcpy(iniLocale, "en"); // FIXME FIXME FIXME !
    
    return true;
}


void SaveINI()
{
    FILE *f = fopen(GetConfigFileName(), "wt");

    fprintf(f, "\n[video]\n"
               "brightness             = %i ;\n"
               "anims_titled           = %i ;\n"
               "anims_interpolated     = %i ;\n",
               iniBrightCorr, iniTitledAnims, iniInterpolateAnims);

    fprintf(f, "\n[audio]\n"
               "music_volume           = %i ;\n"
               "sound_volume           = %i ;\n"
               "speech_volume          = %i ;\n"
               "jukebox_repeat         = %i ;\n"
               "jukebox_random_order   = %i ;\n"
               "jukebox_play_list_size = %i ;\n"
               "jukebox_save_changes   = %i ;\n",
               iniMusicVol, iniSoundVol, iniSpeechVol,
               iniJukeboxRepeat, iniJukeboxRandom, iniJukeboxListSize,
               iniJukeboxSave);

    fprintf(f, "\n[interface]\n"
               "anim_delay             = %i ;\n"
               "anim_delay2            = %i ;\n"
               "idle_delay             = %i ;\n"
               "scroll_delay           = %i ;\n"
               "enable_anim_gui        = %i ;\n"
               "unit_status_bar        = %i ;\n"
               "unit_move_rng          = %i ;\n"
               "unit_shoot_rng         = %i ;\n"
               "unit_visib_rng         = %i ;\n"
               "stop_on_new_enemy      = %i ;\n",
               iniAnimDelay, iniAnimDelay2, iniIdleDelay, iniScrollDelay,
               iniEnhancedGuiOn, iniShowStatusbar, iniShowMoveRange, iniShowShootRange,
               iniShowVisibRange, iniStopOnNewEnemy);

    fclose(f);
}


void ApplyINI()
{
    MouseSetRatio(iniMouseRatioX, iniMouseRatioY);
}





///////////////////////



TDataFile *GraphicsDF, *GraphicsI18nDF, *TextsDF;

TFont *NormalFont;
TFont *HugeFont;
TFont *TinyFont;

char *SigText[TXT_COUNT];
char *MsgText[MSG_COUNT];

static void *MessageFrames[3];
static void *TimerWatchBkg, *TimerWatchBuf;

byte DarkingTable[256], GrayingTable[256];

volatile int TimerValue = 0;

char *Palette;                            // aktualni barevna paleta




// Exponencieni kostka (hazeni):
#define CRollDice 2.0

int RollDice()
{
    double r;
    
    r = double ((rand () + 1)) / (RAND_MAX + 1);
    r = log (r) / log (CRollDice);
    return int (-r);

}



// Zkontroluje pritomnost souboru:

int CheckFile(char *name)
{
    FILE *f = fopensafe(name, "rb");
    
    if (f == NULL) {
        fprintf(stderr, "Cannot find data file '%s'!\n", name);
        return FALSE;
    }
    fclose(f);
    return TRUE;
}

int CheckFiles()
{   
    return (CheckFile("fonts.dat") && 
            CheckFile("graphics.dat") && CheckFile("graphics-common.dat") &&
            CheckFile("missions.dat") && CheckFile("texts.dat") &&
            CheckFile("unitsnd.idx"));
}




int DoMemoryCheck()
{   // assume always Ok : let try swaping!
    return TRUE;
}




int InitGlobal()
{
#ifdef DEBUG
    dbgOutput = fopen("debug.nfo", "wt");
    {   
        TDPMIInfo i;
        
        dpmiinfo(&i);
        fprintf(dbgOutput, "DPMI %i.%i\nVirtual memory: %i\n",
                i.VersionMajor, i.VersionMinor, i.Flags & VMM_Present);
    }
    fprintf(dbgOutput, 
            "Free memory (on start): %ikB\n",   GetFreeMem() / 1024);
#endif
   
    if (!LoadINI()) return FALSE;

    if (!CheckFiles()) return FALSE;
    if (!DoMemoryCheck()) return FALSE;

    TextsDF = new TDataFile("texts.dat", dfOpenRead);
   
    int rt = 1;
    TDataFile FontsDF("fonts.dat", dfOpenRead, NULL, '?', FontDataWrite, FontDataRead);

    GraphicsDF = new TDataFile("graphics-common.dat", dfOpenRead, NULL);
    GraphicsI18nDF = new TDataFile("graphics.dat", dfOpenRead, NULL);

    NormalFont = (TFont *) FontsDF.get("normal");
    HugeFont = (TFont *) FontsDF.get("huge");
    TinyFont = (TFont *) FontsDF.get("tiny");
    
    rt = (int)NormalFont & (int)NormalFont & (int)TinyFont;
    if (rt == 0) return 0;
    else {
        lockfont(NormalFont);
        lockfont(HugeFont);
        lockfont(TinyFont);
    }
    
    MessageBuf = memalloc(MSGBUF_SX * MSGBUF_SY);
    lockmem(MessageBuf, MSGBUF_SX * MSGBUF_SY);
    {
        char bu[20];
        sprintf(bu, "%imseg%%i", iniResolution - 0x100);
        LoadArray(MessageFrames, 3, GraphicsDF, bu);
    }
    TimerWatchBkg = GraphicsDF->get("timerbkg");
    TimerWatchBuf = memalloc(50*14);
    ProgressBuf = (byte*) memalloc(PROGRESS_SX * PROGRESS_SY);
    
    {
        void *p;
        p = (byte*)GraphicsDF->get("paletted");
        memcpy(DarkingTable, p, 256);
        memfree(p);
        p = (byte*)GraphicsDF->get("paletteg");
        memcpy(GrayingTable, p, 256);
        memfree(p);
    }

    LoadArray((void**)SigText, TXT_COUNT, TextsDF, "txt%i");    
    LoadArray((void**)MsgText, MSG_COUNT, TextsDF, "msg%i");    
    
#ifdef DEBUG
    //memset(MessageFrames[2], 0, MSGBUF_SX * MSGBUF_SY);
#endif
    return 1;
}



int DoneGlobal()
{
    DisposeArray((void**)SigText, TXT_COUNT);
    DisposeArray((void**)MsgText, MSG_COUNT);
    unlockmem(MessageBuf, MSGBUF_SX * MSGBUF_SY);
    memfree(MessageBuf);
    memfree(TimerWatchBkg); memfree(TimerWatchBuf);
    DisposeArray(MessageFrames, 3);
    memfree(ProgressBuf);
    unlockfont(HugeFont);
    unlockfont(NormalFont);
    unlockfont(TinyFont);
    freefont(NormalFont);
    freefont(HugeFont);
    freefont(TinyFont);
    delete GraphicsDF;
    delete GraphicsI18nDF;
    delete TextsDF;
#ifdef DEBUG
    dbgReport(); // hlaseni o behu programu
    fclose(dbgOutput);
#endif
    return 1;
}










int IsInRect(int x, int y, int x1, int y1, int x2, int y2)
{
    return ((x >= x1) && (x <= x2) && (y >= y1) && (y <= y2));
}



int Intersect(TRect *r1, TRect *r2)
{
    r1->x1 = max(r1->x1, r2->x1);
    r1->y1 = max(r1->y1, r2->y1);
    r1->x2 = min(r1->x2, r2->x2);
    r1->y2 = min(r1->y2, r2->y2);
    return (r1->x1 <= r1->x2) && (r1->y1 <= r1->y2);
}



void Union(TRect *r1, TRect *r2)
{
    r1->x1 = min(r1->x1, r2->x1);
    r1->y1 = min(r1->y1, r2->y1);
    r1->x2 = max(r1->x2, r2->x2);
    r1->y2 = max(r1->y2, r2->y2);
}



void Union2(int *x1, int *y1, int *w1, int *h1, int x2, int y2, int w2, int h2)
{
    TRect r1, r2;
    
    r1.x1 = *x1, r1.x2 = *x1 + *w1 - 1;
    r1.y1 = *y1, r1.y2 = *y1 + *h1 - 1;
    r2.x1 = x2, r2.x2 = x2 + w2 - 1;
    r2.y1 = y2, r2.y2 = y2 + h2 - 1;
    Union(&r1, &r2);
    *x1 = r1.x1, *w1 = r1.x2 - r1.x1 + 1;
    *y1 = r1.y1, *h1 = r1.y2 - r1.y1 + 1;
}



const char *getSignusDataDir()
{
    const char *datdir = getenv("SIGNUS_DATA_DIR");
    if (datdir && strlen(datdir) > 0)
        return datdir;
    else
        return SIGNUS_DATA_DIR;
}

const char *getSignusConfigDir()
{
    char *home = getenv("HOME");
    if (!home) home = ".";

    static char inidir[1024] = "";
    
    if (*inidir == 0)
    {
        char *home = getenv("HOME");
        if (!home) home = ".";
        strncpy(inidir, home, 1024);
        strncat(inidir, "/.signus", 1024);
        
        if (!dirExists(inidir))
        {
            mkdir(inidir, 0700);
        }
    }

    return inidir;
}


FILE *fopensafe(const char *name, const char *mode)
{
    FILE *f = NULL;
    char nm[1024];

    if (getenv("HOME"))
    {
        snprintf(nm, 1024, "%s/.signus/%s", getenv("HOME"), name);
        f = fopen(nm, mode);
    }

    if (!f)
    {
        snprintf(nm, 1024, "%s/nolang/%s", getSignusDataDir(), name);
        f = fopen(nm, mode);
    }
    
    if (!f)
    {
        snprintf(nm, 1024, "%s/%s/%s", getSignusDataDir(), iniLocale, name);
        f = fopen(nm, mode);
    }
    
    if (!f)
    {
        snprintf(nm, 1024, "%s/default/%s", getSignusDataDir(), name);
        f = fopen(nm, mode);
    }

    return f;
}


bool fileExists(const char *name)
{
   FILE *f;
   f = fopen(name, "rb");
   if (!f) return false;
   fclose(f);
   return true;
}




int roundnum(double d)
{
    int i = floor(d);
    
    if (d - (double)i < 0.5) return i;
    else return i+1;
}








void WaitCursor(int turn_on)
{
    static int locks = 0;
    static int lastcur = 0;
    
    
    if (turn_on) {
        if (!locks) {
            lastcur = Mouse.ActCur;
            MouseSetCursor(mcurWait);
        }
        locks++;
    }   
    else {
        locks--;
        if (!locks)
            MouseSetCursor(lastcur);
    }   
}







//////////////////////////// PROGRESS /////////////////////////////////////


byte *ProgressBuf;
int ProgLimit = 100, ProgValue = 0;

void ProgressUpdate()
{
    int pix = (PROGRESS_SX * ProgValue) / ProgLimit;
    int i, j;
    static byte clrs[PROGRESS_SY] = {36, 36, 4, 7, 36, 36};
    
    if (pix < 3)
        memset(ProgressBuf, clrBlue, PROGRESS_SX * PROGRESS_SY);
    else {  
        for (j = 0; j < PROGRESS_SY; j++) {
            for (i = 0; i < pix; i++) ProgressBuf[j * PROGRESS_SX + i] = clrs[j];
            for (; i < PROGRESS_SX; i++) ProgressBuf[j * PROGRESS_SX + i] = clrBlue;
        }
    }
    
    MouseFreeze(PROGRESS_X_POS, PROGRESS_Y_POS, PROGRESS_SX, PROGRESS_SY);
    PutBitmap(PROGRESS_X_POS, PROGRESS_Y_POS, ProgressBuf, PROGRESS_SX, PROGRESS_SY);
    MouseUnfreeze();
}



void ProgressNull()
{
    ProgValue = 0;
    ProgressUpdate();
}

void ProgressLimit(int limit)
{
    ProgLimit = limit;
    ProgressNull();
}

void ProgressInc()
{
    ProgValue++;
    if (ProgValue > ProgLimit) ProgValue = ProgLimit;
    ProgressUpdate();
}

void ProgressSet(int value)
{
    ProgValue = (value > ProgLimit) ? ProgLimit : value;
    ProgressUpdate();
}











///////////////////////////// MESSAGES ///////////////////////////////////

void *MessageBuf;


void Message(char *msg)
{
    if (*msg == 0) {
        memcpy(MessageBuf, MessageFrames[2], MSGBUF_SX * MSGBUF_SY);
        PutBitmap32(MSGBUF_X_POS, MSGBUF_Y_POS, MessageBuf, MSGBUF_SX, MSGBUF_SY);
    }
    else {
        memset(MessageBuf, clrWhite, MSGBUF_SX * MSGBUF_SY);
        PutBitmap32(MSGBUF_X_POS, MSGBUF_Y_POS, MessageBuf, MSGBUF_SX, MSGBUF_SY);
        SDL_Delay(50);

        for (int i = 0; i < 3; i++) {
            memcpy(MessageBuf, MessageFrames[i], MSGBUF_SX * MSGBUF_SY);
            PutStr(MessageBuf, MSGBUF_SX, 2, 1, msg, NormalFont, clrWhite, clrBlack);
            PutBitmap32(MSGBUF_X_POS, MSGBUF_Y_POS, MessageBuf, MSGBUF_SX, MSGBUF_SY);
            SDL_Delay(50);
        }
    }
}



void Message(int msg)
{
    char b[30];
    
    sprintf(b, "msg%i", msg);
    SaySpeech(b, 1000);
    Message(MsgText[msg]);
}











////////////////////////////// MSGBOX /////////////////////////////////////

void *MsgBoxBuf = NULL;

void MsgBox(char *str)
{
    if (str == NULL) {
        if (MsgBoxBuf == NULL) return;
        PutBitmap32(MSGBOX_X_POS, MSGBOX_Y_POS, MsgBoxBuf, MSGBOX_SX, MSGBOX_SY);
        memfree(MsgBoxBuf);
        MsgBoxBuf = NULL;
    }
    else {
        void *dummy;
    
        MsgBoxBuf = memalloc(MSGBOX_SX * MSGBOX_SY);
        GetBitmap32(MSGBOX_X_POS, MSGBOX_Y_POS, MsgBoxBuf, MSGBOX_SX, MSGBOX_SY);
        
        dummy = GraphicsDF->get("msgbox");
        PutStr(dummy, MSGBOX_SX,
               (MSGBOX_SX - GetStrWidth(str, HugeFont)) / 2, (MSGBOX_SY - GetStrHeight(str, HugeFont)) / 2,
               str, HugeFont, clrWhite, clrBlack);
        PutBitmap32(MSGBOX_X_POS, MSGBOX_Y_POS, dummy, MSGBOX_SX, MSGBOX_SY);   
        memfree(dummy);
    }
}











////////////////////////// LOADING //////////////////////////////////////


static int LoadPhase = 0;
static void *LoadBuf[16];

void StartLoading(char *picname)
{
    void *ptr;
    char buf[20];
//  sprintf(buf, "%i%s", iniResolution - 0x100, picname);
    sprintf(buf, "%imis1", iniResolution - 0x100);
    LoadArray(LoadBuf, 16, GraphicsDF, "load%i");

    SDL_Delay(1000);
    MouseHide();
    ptr = GraphicsDF->get(buf);
    if (ptr) {
        DrawPicture(ptr); 
        memfree(ptr);
    }

    ptr = GraphicsI18nDF->get("load");
    PutBitmap32(RES_X/2-240, RES_Y-80, ptr, 400, 50);
    LoadPhase = 0;
    UpdateLoading();
}



void UpdateLoading()
{
    char b[9];
    void *buf;
    
    sprintf(b, "load%i", LoadPhase++);
    if (LoadPhase == 16) LoadPhase = 0;
    buf = LoadBuf[LoadPhase];
    PutBitmap(RES_X/2+180, RES_Y-80, buf, 60, 50);
}



void DoneLoading()
{
    DisposeArray(LoadBuf, 16);
    MouseShow();
    SDL_Delay(1000);
}









///////////////////////////// ukazatel casu //////////////////////////////

static void DisplayWatch(int x, int y, int value)
{
    int h, m, s;
    char b[9];
    
    s = value % 60; value /= 60;
    m = value % 60; value /= 60;
    h = value % 24;
    sprintf(b, "%02i:%02i:%02i", h, m, s);
    
    memcpy(TimerWatchBuf, TimerWatchBkg, 50*14);
    PutStr(TimerWatchBuf, 50, 2, 3, b, TinyFont, clrWhite, clrBlack);
    PutBitmap(x, y, TimerWatchBuf, 50, 14);
}


#ifdef DEBUG

extern TPoint SelPos;

static void DisplayWatchField(int x, int y)
{
    char b[9];
    
    sprintf(b, "%03i:%03i", SelPos.x, SelPos.y);
    
    memcpy(TimerWatchBuf, TimerWatchBkg, 50*14);
    PutStr(TimerWatchBuf, 50, 2, 3, b, TinyFont, clrWhite, clrBlack);
    PutBitmap(x, y, TimerWatchBuf, 50, 14);
}

#endif



void ShowTime(int left, int right)
{
#ifdef DEBUG
    DisplayWatchField(RES_X-257, RES_Y-((iniResolution == SVGA_640x480) ? 18 : 22));
#else
    DisplayWatch(RES_X-257, RES_Y-((iniResolution == SVGA_640x480) ? 18 : 22), left);
#endif
    DisplayWatch(RES_X-186, RES_Y-((iniResolution == SVGA_640x480) ? 18 : 22), right);
}





TPoint CartezianSnail (int i)
{
    TPoint pos = {0,0};
    if (i <= 0) return pos; // stred
    
    int r = floor (0.5*sqrt(double(i)+1) - 0.5); // polomer nejvetsi uplne kruznice
    int n = i - 4*(r*r + r); // cislo pozice na nejvetsi kruznici
    
    if (n == 0) { // nejvetsi kruznice je uplna
        pos.x = r;
        pos.y = -1;
        return pos;
    }
    
    if (n < r + 2) { // Re x = r + 1, Im x >= 0
        pos.x = r + 1;
        pos.y = n - 1;
        return pos;
    }
    
    if (n < 3*r + 4) { // Im x = r + 1
        pos.x = 2*r + 3 - n;
        pos.y = r + 1;
        return pos;
    }
    
    if (n < 5*r + 6) { // Re x = -(r + 1)
        pos.x = -(r + 1);
        pos.y = 4*r + 5 - n;
        return pos;
    }
    
    if (n < 7*r + 8) {
        pos.x = n - (6*r + 7);
        pos.y = -(r + 1);
        return pos;
    }
    
    if (n < 8*r + 8) {
        pos.x = r + 1;
        pos.y = n - (8*r + 9);
        return pos;
    }
    
    return pos;
}





#ifdef DEBUG
/////////////////////////// UNIVERSAL DEBUGGER //////////////////////////////

#include "ui_toolkit.h"

static int FIRST_DBG_PT = 0;

void DEBUG_POINT(int param)
{
}





#endif





// ...
