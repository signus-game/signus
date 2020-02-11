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


// Zpracovani JUKEBOXU


#include "jukebox.h"
#include <dirent.h>
#include "global.h"
#include "sound.h"
#include "engine.h"
#include "ui_toolkit.h"



// Seznam jmen skladeb, ktere se maji hrat:
char **PlayList;
int PlayCount;
int CurrentPlayed;
int JukeboxOn = FALSE, PlayingSuspended = FALSE;



static const char *getJukeboxInifile()
{
    static char name[1024] = "";
    if (name[0] == 0)
    {
        snprintf(name, 1024, "%s/jukebox.ini", getSignusConfigDir());
    }
    return name;
}


int str_compare(const void *op1, const void *op2)
{
  const char **p1 = (const char **) op1;
  const char **p2 = (const char **) op2;
  return(strcmp(*p1, *p2));
}

// vyhledavaci procedura (najde vsechny skladby v music.dat + adresari \music

void doGatherFiles(const char *dirname, char ***List, int *Count)
{
    DIR *dir;
	struct dirent *fi;
	char **l = *List;
	
	// Gather modules from directory signus/music:

    dir = opendir(dirname);
    if (dir == NULL) return;

    fi = readdir(dir);
	while (fi) {
        if (strcmp(fi->d_name, ".") != 0 && strcmp(fi->d_name, "..") != 0)
        {
    		(*Count)++;
    		l = (char**) memrealloc(l, sizeof(char*) * *Count);		
    		l[(*Count)-1] = (char*) memalloc(1 + strlen(fi->d_name));
    		strcpy(l[(*Count)-1], fi->d_name);
        }
        fi = readdir(dir);
	}
	closedir(dir);
	*List = l;
}

char **GatherFiles(int *Count)
{
	*Count = 0;
	char **l = NULL;

    doGatherFiles(SIGNUS_DATA_DIR "/nolang/music", &l, Count);
    char dirname[1024];
    snprintf(dirname, 1024, "%s/music", getSignusConfigDir());
    doGatherFiles(dirname, &l, Count);

    qsort(l, *Count, sizeof(char *), str_compare);
	return l;
}



////////////////////////////////////////////////////////
// nacitani/ukladani seznamu skladeb:

void LoadPlayList()
{	
	FILE *f;
	char buffer[1024];

	if (iniJukeboxListSize == 0)
    {
        PlayList = GatherFiles(&PlayCount);
    }
	else 
    {
		if ((f = fopen(getJukeboxInifile(), "rt")) == NULL) {
			PlayList = GatherFiles(&PlayCount);
			iniJukeboxListSize = 0;
			SaveINI();
		}
		PlayCount = iniJukeboxListSize;
		PlayList = (char**) memalloc(PlayCount * sizeof(char*));
		for (int i = 0; i < PlayCount; i++) {
            fgets(buffer, 1024, f);
            size_t len = strlen(buffer);
            if (buffer[len] == '\n') { buffer[len] = 0; len--; }
			PlayList[i] = (char*) memalloc(1 + len);
			strcpy(PlayList[i], buffer);
		}
		fclose(f);
	}
}

void SavePlayList()
{
	FILE *f;
	
	if ((f = fopen(getJukeboxInifile(), "wt")) == NULL) return;
	iniJukeboxListSize = PlayCount;
	SaveINI();
	for (int i = 0; i < PlayCount; i++) fprintf(f, "%s\n", PlayList[i]);
	fclose(f);
}


////////////////////////////////////////////////////////

// init/done:

int InitJukebox()
{
	PlayCount = 0;
	PlayList = NULL;
	CurrentPlayed = -1;	
	LoadPlayList();
	JukeboxOn = TRUE;
	return TRUE;
}

int DoneJukebox()
{
	if (!JukeboxOn) return FALSE;
	JukeboxOn = FALSE;
	if (PlayList != NULL) {
		for (int i = 0; i < PlayCount; i++)	memfree(PlayList[i]);
		memfree(PlayList); PlayList = NULL;
	}
	return TRUE;
}





void JukeboxNext()
{	
	if (MIDAS_disabled) return;
	
	if (PlayingSuspended || (!JukeboxOn) || (PlayCount == 0)) return;
	StopMusic();
	if (iniJukeboxRandom)
    { 
        CurrentPlayed = rand() % PlayCount;
    }
	else
    {
        CurrentPlayed++;
    }
	if (CurrentPlayed >= PlayCount) {
		if (iniJukeboxRepeat)	CurrentPlayed = 0;
		else {PlayingSuspended = TRUE; CurrentPlayed = 0; return;}
	}
	PlayMusic(PlayList[CurrentPlayed]);
}







/////////////////////// JukeboxSetup() <<--- dialog

#define cmPlayNext      1111
#define cmAddMod        1112
#define cmRemoveMod     1113
#define cmAddAll        1114
#define cmRemoveAll     1115

class TJukeboxDlg : public TDialog {
		public:
			char **AvailList;
			int AvailCnt;
			TListBox *LB_Avail, *LB_Play;
		
			TJukeboxDlg();
			int SpecialHandle(TEvent *e, int Cmd);
			~TJukeboxDlg();
	};



TJukeboxDlg::TJukeboxDlg() : TDialog(9+(VIEW_SX-500)/2, 36+(VIEW_SY-416)/2, 500, 416, "dlgjuke") 
{
	AvailCnt = 0;
	AvailList = GatherFiles(&AvailCnt);
	Insert(new TStaticText(10, 5, 480, 30, SigText[TXT_JUKEBOX], TRUE));
	Insert(new TStaticText(10, 35, 150, 16, SigText[TXT_JBOX_AVAILABLE]));
	Insert(LB_Avail = new TListBox(10, 55, 150, 19, AvailList, AvailCnt, cmAddMod));
	Insert(new TStaticText(335, 35, 150, 16, SigText[TXT_JBOX_TOPLAY]));
	Insert(LB_Play = new TListBox(335, 55, 150, 16, PlayList, PlayCount, cmRemoveMod));
	Insert(new TButton(185, 70, SigText[TXT_JBOX_ADD], cmAddMod));
	Insert(new TButton(185, 110, SigText[TXT_JBOX_ADDALL], cmAddAll));
	Insert(new TButton(185, 150, SigText[TXT_JBOX_REMOVE], cmRemoveMod));
	Insert(new TButton(185, 190, SigText[TXT_JBOX_REMOVEALL], cmRemoveAll));
	Insert(new TButton(185, 266, SigText[TXT_JBOX_NEXTMOD], cmPlayNext));
	Insert(new TButton(185, 306, SigText[TXT_OK], cmOk));
	Insert(new TCheckBox(185, 350, 480, SigText[TXT_JBOX_RANDOM], &iniJukeboxRandom));
	Insert(new TCheckBox(185, 370, 480, SigText[TXT_JBOX_REPEAT], &iniJukeboxRepeat));
	Insert(new TCheckBox(185, 390, 480, SigText[TXT_JBOX_SAVE], &iniJukeboxSave));
}



int TJukeboxDlg::SpecialHandle(TEvent *e, int Cmd)
{
	int i;
	char *s;
	
	switch (Cmd) {
		case cmPlayNext : 
			JukeboxNext();
			LB_Play->Current = CurrentPlayed;
			LB_Play->Draw(); LB_Play->Paint();
			return -1;
			break;

		case cmAddMod:
			s = LB_Avail->Data[LB_Avail->Current];
			PlayList = (char **) memrealloc(PlayList, ++PlayCount * sizeof(char*));
			PlayList[PlayCount-1] = (char*) memalloc(1 + strlen(s));
			strcpy(PlayList[PlayCount-1], s);
			LB_Play->Data = PlayList;
			LB_Play->Cnt = PlayCount;
			LB_Play->Draw(); LB_Play->Paint();
			return -1;
			break;
			
		case cmRemoveMod:
			if ((PlayCount <= 0) || (LB_Play->Current < 0)) return -1;
			memfree(PlayList[LB_Play->Current]);
			memmove(PlayList + LB_Play->Current, PlayList + (LB_Play->Current+1),
			        sizeof(char*) * (PlayCount - LB_Play->Current));			
			PlayList = (char**) memrealloc(PlayList, --PlayCount * sizeof(char*));
			LB_Play->Data = PlayList;
			LB_Play->Cnt = PlayCount;
			if (LB_Play->Current >= LB_Play->Cnt) LB_Play->Current = LB_Play->Cnt - 1;
			if ((LB_Play->ScrLn < LB_Play->Cnt) && (LB_Play->Delta + LB_Play->ScrLn > LB_Play->Cnt)) LB_Play->Delta--;
			LB_Play->Draw(); LB_Play->Paint();
			return -1;
			break;
			
		case cmAddAll:
			if (PlayCount > 0) {
				for (i = 0; i < PlayCount; i++)	memfree(PlayList[i]);
				memfree(PlayList); PlayList = NULL; PlayCount = 0;
			}
			PlayList = (char**) memalloc(sizeof(char*) * AvailCnt);
			PlayCount = AvailCnt;
			for (i = 0; i < AvailCnt; i++) {
				PlayList[i] = (char*) memalloc(1 + strlen(AvailList[i]));
				strcpy(PlayList[i], AvailList[i]);
			}
			LB_Play->Data = PlayList;
			LB_Play->Cnt = PlayCount;
			LB_Play->Current = LB_Play->Delta = 0;
			LB_Play->Draw(); LB_Play->Paint();
			return -1;
		
		case cmRemoveAll:
			if (PlayCount <= 0) return -1;
			for (i = 0; i < PlayCount; i++)	memfree(PlayList[i]);
			memfree(PlayList); PlayList = NULL; PlayCount = 0;
			LB_Play->Data = PlayList;
			LB_Play->Cnt = PlayCount;
			LB_Play->Current = LB_Play->Delta = 0;
			LB_Play->Draw(); LB_Play->Paint();
			return -1;
	}
	return TDialog::SpecialHandle(e, Cmd);
}



TJukeboxDlg::~TJukeboxDlg()
{
	for (int i = 0; i < AvailCnt; i++) memfree(AvailList[i]);
	memfree(AvailList);
}





void JukeboxSetup()
{
	TDialog *dlg = new TJukeboxDlg();

	dlg->Exec();
	if (iniJukeboxSave) SavePlayList();
	else remove(getJukeboxInifile());
	SaveINI();
	delete dlg;
	PlayingSuspended = FALSE;
}

