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
// Tento modul vytvari dialogy pro load/save a provadi i vlastni akci
//

#include "headers.h"
#include <time.h>
#include <dirent.h>

#include "system.h"
#include "global.h"
#include "ui_toolkit.h"
#include "engine.h"
#include "engtimer.h"
#include "units.h"
#include "ai.h"
#include "fields.h"
#include "autofire.h"

#include "loadsave.h"


///// Struktury a promnene:

#define STAMP_SZ  ((504 / 2) * (420 / 2))

typedef struct {
            char     Magic[12];           // == "SIGNUS SAVE"0
            char     Name[64];
            time_t   Time;
    }   TSavegameHdr;




////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


int SaveGameState(FILE *f)
{
    int dummy;
    
    WaitCursor(TRUE);
    LockDraw(); LockDraw(); LockDraw(); LockDraw();
    StoreMap(f);
    
    // engine.h:
    fwrite(&MapPos, sizeof(MapPos), 1, f);
    fwrite(&TurnTime, sizeof(TurnTime), 1, f);
    fwrite(&MissionTime, sizeof(MissionTime), 1, f);
    fwrite(&ActualMission, sizeof(ActualMission), 1, f);
    fwrite(&ActualDifficulty, sizeof(ActualDifficulty), 1, f);
    // visiblty.h:
    fwrite(&AllowedBadlife, sizeof(AllowedBadlife), 1, f);
    if (BadlifeVisib != NULL) {
        dummy = TRUE; fwrite(&dummy, 4, 1, f);
        fwrite(BadlifeVisib, MapSizeX * MapSizeY, 1, f);
    }
    else {dummy = FALSE; fwrite(&dummy, 4, 1, f);}
    // units.h:
    fwrite(SelectionHistory, sizeof(SelectionHistory), 1, f);
    fwrite(&GoodlifeDeads, sizeof(GoodlifeDeads), 1, f);
    fwrite(&BadlifeDeads, sizeof(BadlifeDeads), 1, f);
    fwrite(&TimeReserve, 4, 1, f);
    // fields.h:
    fwrite(&MinesCnt, sizeof(MinesCnt), 1, f);
    fwrite(MinesList, sizeof(TMineRecord) * MinesCnt, 1, f);
    // building.h:
    fwrite(&MoneyGoodlife, 4, 1, f);
    fwrite(&MoneyBadlife, 4, 1, f);
    // autofire.h:
    SaveAutofire(f);

    // loadsave.h:

    // NB: this used to save SavesRemaining value, but we no longer have
    //     such stupid misfeature in the code. We still write out 4bytes
    //     of worthless data, though, to keep compatibility with original
    //     Signus saved game states:
    int SavesRemainingFakeValue = 100;
    fwrite(&SavesRemainingFakeValue, 4, 1, f);
       
    SaveArtificialIntelligence(f);

    UnlockDraw(); UnlockDraw(); UnlockDraw(); UnlockDraw(); 
    WaitCursor(FALSE);
    return TRUE;
}





int LoadGameState(FILE *f)
{
    int dummy;
    TPoint pz;

    MinesCnt = 0;
    WaitCursor(TRUE);
    InitEngine(0, f);

    // engine.h:
    fread(&pz, sizeof(pz), 1, f);
    fread(&TurnTime, sizeof(TurnTime), 1, f);
    fread(&MissionTime, sizeof(MissionTime), 1, f);
    fread(&ActualMission, sizeof(ActualMission), 1, f);
    fread(&ActualDifficulty, sizeof(ActualDifficulty), 1, f);
    ScrollTo(pz.x, pz.y);
    HideHelpers();
    // visiblty.h:
    fread(&AllowedBadlife, sizeof(AllowedBadlife), 1, f);
    fread(&dummy, 4, 1, f);
    if (dummy) {
        if (BadlifeVisib != NULL) memfree(BadlifeVisib);
        BadlifeVisib = (byte*) memalloc(MapSizeX * MapSizeY);
        fread(BadlifeVisib, MapSizeX * MapSizeY, 1, f);
    }
    // units.h:
    fread(SelectionHistory, sizeof(SelectionHistory), 1, f);
    fread(&GoodlifeDeads, sizeof(GoodlifeDeads), 1, f);
    fread(&BadlifeDeads, sizeof(BadlifeDeads), 1, f);
    fread(&TimeReserve, 4, 1, f);
    // fields.h:
    fread(&MinesCnt, sizeof(MinesCnt), 1, f);
    fread(MinesList, sizeof(TMineRecord) * MinesCnt, 1, f);
    // building.h:
    fread(&MoneyGoodlife, 4, 1, f);
    fread(&MoneyBadlife, 4, 1, f);
    // autofire.h:
    LoadAutofire(f);
    
    // loadsave.h:
    // NB: this used to save SavesRemaining value, but we no longer have
    //     such stupid misfeature in the code. We still write out 4bytes
    //     of worthless data, though, to keep compatibility with original
    //     Signus saved game states:
    int SavesRemainingFakeValue;
    fread(&SavesRemainingFakeValue, 4, 1, f);

    LoadArtificialIntelligence(f);
    
    WaitCursor(FALSE);
    RedrawMap();
    ShowHelpers();
    PulsarProcess = TRUE;
    SetPalette(Palette);
    return TRUE;
}









////////////////////////////////////////////////////////////////////////////


//////////////////////// LOADSAVEDIALOG ///////////////////////////////////

// Dialog osetrujici load/save fce:

#define cmDelete  1234


class TLoadSaveDialog : public TDialog {
        public:
            TBitmap *m_Stamp;
            TListBox *m_List;
        
            void *Stamp;
            int IsSave; // pri save je 0. polozka new...
            char *SNames[1000], *SFiles[1000];
            time_t STimes[1000];
            int SCount;                        // Sxxxx jsou savegame.000 atd.

            TLoadSaveDialog(int aSv);
            void SearchFiles();
            void SortFiles(int l, int r);
            char *GetSelectedFile() {return SFiles[m_List->Current];};
                    // vraci jmeno vybraneho souboru (u new ho vytvori)
            char *GetSelectedName() {return SNames[m_List->Current];};
            void GetStamp(int num);
                    // nacte obrazek k danemu sejvu
            int SpecialHandle(TEvent *e, int Cmd);
            ~TLoadSaveDialog();
    };


void LSD_Focus(TListBox *l)
{
    TLoadSaveDialog *lsd = (TLoadSaveDialog*) l->Owner;

    lsd->GetStamp(l->Current);  
    lsd->m_Stamp->Draw();
    lsd->m_Stamp->Paint();
    {
        TEvent e;
        do {GetEvent(&e);} while (e.What != evNothing);
    }
}



TLoadSaveDialog::TLoadSaveDialog(int aSv) : TDialog(VIEW_X_POS + (VIEW_SX-500)/2, VIEW_Y_POS + (VIEW_SY-416)/2, 500, 416, "dlgloads")
{    
    Stamp = memalloc(STAMP_SZ);
    IsSave = aSv ? 1 : 0;
    SearchFiles();
    if (SCount > 0) {
        GetStamp(0);
        m_List = new TListBox(8, 8, 483, 10, SNames, SCount, cmOk, LSD_Focus);
        Insert(m_List);
        m_Stamp = new TBitmap(235, 192, TRUE, Stamp, 504 / 2, 420 / 2, FALSE);
        Insert(m_Stamp);
        Insert(new TButton(55, 240, IsSave ? SigText[TXT_SAVE_GAME] : SigText[TXT_LOAD_GAME], cmOk, TRUE));
        if (IsSave) {
            Insert(new TButton(55, 280, SigText[TXT_DELETE], cmDelete));
            Insert(new TButton(55, 320, SigText[TXT_CANCEL], cmCancel));
        }
        else {
            Insert(new TButton(55, 280, SigText[TXT_CANCEL], cmCancel));
        }
    }

    else {
        Insert(new TStaticText(10, 10, 200, 40, SigText[TXT_NO_SAVES], TRUE));
        Insert(new TButton(55, 320, SigText[TXT_CANCEL], cmCancel));        
    }
}



void TLoadSaveDialog::SearchFiles()
{
    DIR *dir;
    struct dirent *fi;
    int i, j;
    TSavegameHdr hdr;
    FILE *f;
    char buf[20];
    
    SCount = IsSave; /*0,1*/
    for (i = 0; i < 1000; i++) SNames[i] = SFiles[i] = NULL;

    // searching for savefiles...:
    dir = opendir("."); // FIXME -- save them in ~/.signus/saved_games
    fi = readdir(dir);
    while (fi) {
        if (strncmp(fi->d_name, "savegame", 8) != 0) continue;
        if ((f = fopen(fi->d_name, "rb")) != NULL) {
            fread(&hdr, sizeof(hdr), 1, f);
            if (strcmp(hdr.Magic, "SIGNUS SAVE") == 0) {
                SNames[SCount] = (char*) memalloc(strlen(hdr.Name) + 1);
                strcpy(SNames[SCount], hdr.Name);
                SFiles[SCount] = (char*) memalloc(13);
                strcpy(SFiles[SCount], fi->d_name);
                STimes[SCount] = 0xFFFFFFFF - hdr.Time;
                SCount++;
            }
            fclose(f);
        }
        fi = readdir(dir);
    }
    closedir(dir);

    if (IsSave) { // udela jmeno pro new save:
        int rc;    
        for (i = 0; i < 1000; i++) {
            sprintf(buf, "savegame.%03i", i);
            rc = TRUE;
            for (j = 1; j < SCount; j++)
                if (strcasecmp(buf, SFiles[j]) == 0) {rc = FALSE; break;}
            if (rc) {
                SFiles[0] = (char*) memalloc(13);
                strcpy(SFiles[0], buf);
                SNames[0] = (char*) memalloc(strlen(SigText[TXT_NEW]) + 1);
                strcpy(SNames[0], SigText[TXT_NEW]);
                STimes[0] = 0;
                break;
            }
        }
    }
    
    SortFiles(0, SCount-1);
}



void TLoadSaveDialog::SortFiles(int l, int r)
{
    int i = l, j = r;
    time_t x = STimes[(l + r) / 2], y;
    char *y2, *y3;
    
    if (!(r > l)) return;
    
    do {
        while (STimes[i] < x) i++;
        while (x < STimes[j]) j--;
        if (i <= j) {
            y = STimes[i];
            y2 = SNames[i];
            y3 = SFiles[i];
            STimes[i] = STimes[j];
            SFiles[i] = SFiles[j];
            SNames[i] = SNames[j];
            STimes[j] = y;
            SNames[j] = y2;
            SFiles[j] = y3;
            i++, j--;
        }
    } while (i <= j);
    if (l < j) SortFiles(l, j);
    if (i < r) SortFiles(i, r);
}



void TLoadSaveDialog::GetStamp(int num)
{
    FILE *f;
    
    if (IsSave && (num == 0)) {
        void *buf = GraphicsDF->get("newsave");
        memcpy(Stamp, buf, STAMP_SZ);
        memfree(buf);
    }
    else {
        f = fopen(SFiles[num], "rb");
        fseek(f, sizeof(TSavegameHdr), SEEK_SET);
        fread(Stamp, STAMP_SZ, 1, f);
        fclose(f);
    }
}



int TLoadSaveDialog::SpecialHandle(TEvent *e, int Cmd)
{
    int Cur;
    char buf[64];
    
    if (Cmd == cmDelete) {
        Cur = m_List->Current;
        if (Cur > 0) {
            remove(SFiles[Cur]);
            memfree(SFiles[Cur]);
            memfree(SNames[Cur]);
            SCount--;
            if (Cur < SCount) {
                memmove(SFiles + Cur, SFiles + (Cur+1), sizeof(char*) * (SCount - Cur));
                memmove(SNames + Cur, SNames + (Cur+1), sizeof(char*) * (SCount - Cur));
                memmove(STimes + Cur, STimes + (Cur+1), sizeof(time_t) * (SCount - Cur));
            }
            m_List->Cnt--;
            if (m_List->Current >= m_List->Cnt) m_List->Current = m_List->Cnt - 1;
            if ((m_List->ScrLn < m_List->Cnt) && (m_List->Delta + m_List->ScrLn > m_List->Cnt)) m_List->Delta--;
            m_List->Draw(); m_List->Paint();
        }
        GetStamp(m_List->Current);    
        m_Stamp->Draw();
        m_Stamp->Paint();
        return -1;
    }
    else if ((Cmd == cmOk) && IsSave) {
        Cur = m_List->Current;
        if (Cur == 0) strcpy(buf, "");
        else strcpy(buf, SNames[Cur]);
        if (InputBox("", buf, 63) == cmOk) {
            memfree(SNames[Cur]);
            SNames[Cur] = (char*) memalloc(strlen(buf) + 1);
            strcpy(SNames[Cur], buf);           
            return cmOk;
        }
        else return -1;
    }
    else return TDialog::SpecialHandle(e, Cmd);
}



TLoadSaveDialog::~TLoadSaveDialog()
{
    memfree(Stamp);
    for (int i = 0; i < SCount; i++) {
        memfree(SNames[i]);
        memfree(SFiles[i]);
    }
}









//////////////////////////   LOADING & SAVING ///////////////////////////////

int LoadGame()
{
    TLoadSaveDialog *dlg;
    FILE *f;
    int rtn = FALSE;    

    dlg = new TLoadSaveDialog(FALSE/*is NOT save*/);

    if (dlg->Exec() == cmOk) {
        if ((f = fopen(dlg->GetSelectedFile(), "rb")) != NULL) {
            if (ActualMission != -1) DoneEngine();
            fseek(f, sizeof(TSavegameHdr) + STAMP_SZ, SEEK_SET);
            LoadGameState(f);
            fclose(f);
            rtn = TRUE;
        }
    }

    delete dlg;
    {
        TEvent e;
        do {GetEvent(&e);} while (e.What != evNothing);
    }
    return rtn;
}





void SG_PutStamp(FILE *f)
{
    byte *buf = (byte*) memalloc(STAMP_SZ);
    byte *b = buf;
    int i, j;
    
    for (j = 0; j < 420; j += 2)
        for (i = 0; i < 504; i += 2) 
            *(b++) = ((byte*)FullBuf)[(VIEW_SX * i / 504) + VIEW_OFS_X + ((VIEW_SY * j / 420) + VIEW_OFS_Y) * VIEW_PIXSZ_X];
    fwrite(buf, STAMP_SZ, 1, f);
    memfree(buf);
}



int SaveGame()
{
    TLoadSaveDialog *dlg;
    TSavegameHdr hdr;
    FILE *f;
    int rtn = FALSE;    

    dlg = new TLoadSaveDialog(TRUE/*is save*/);

    if (dlg->Exec() == cmOk) {
        strcpy(hdr.Magic, "SIGNUS SAVE");
        strcpy(hdr.Name, dlg->GetSelectedName());
        hdr.Time = time(NULL);
        if ((f = fopen(dlg->GetSelectedFile(), "wb")) != NULL) {
            fwrite(&hdr, sizeof(hdr), 1, f);
            MsgBox(SigText[TXT_SAVING]);
            SG_PutStamp(f);
            SaveGameState(f);
            MsgBox(NULL);
            fclose(f);
            rtn = TRUE;
        }
    }

    delete dlg;
    {
        TEvent e;
        do {GetEvent(&e);} while (e.What != evNothing);
    }
    return rtn;
}



#ifdef DEBUG

int SaveGameDebug() // na auto-ukladani po kazdem kole
{
    TLoadSaveDialog *dlg;
    TSavegameHdr hdr;
    FILE *f;
    int rtn = FALSE;    
    char fil[13];

    dlg = new TLoadSaveDialog(TRUE/*is save*/);
    strcpy(hdr.Magic, "SIGNUS SAVE");
    sprintf(hdr.Name, "Betatest save: turn %i", ActualTurn);
    hdr.Time = time(NULL);
    if ((f = fopen(dlg->SFiles[0], "wb")) != NULL) {
        fwrite(&hdr, sizeof(hdr), 1, f);
        SG_PutStamp(f);
        SaveGameState(f);
        fclose(f);
        rtn = TRUE;
    }

    delete dlg;
    return rtn;
}

#endif
