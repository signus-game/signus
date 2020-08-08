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
// Tento modul vytvari dialogy pro load/save a provadi i vlastni akci
//

#include <limits.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

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
            char     Magic[12];           // == "SIGNUS GPL0"0
            char     Name[64];
            time_t   Time;
	    int      format;
    }   TSavegameHdr;

static const char *magic_list[] = {
	"SIGNUS SAVE",
	"SIGNUS GPL0",
	NULL
};


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


int SaveGameState(WriteStream &stream) {
	int i;
    
	WaitCursor(TRUE);

	for (i = 0; i < 4; i++) {
		LockDraw();
	}

	StoreMap(stream);
    
	// engine.h:
	stream.writeSint32LE(MapPos.x);
	stream.writeSint32LE(MapPos.y);
	stream.writeSint32LE(TurnTime);
	stream.writeSint32LE(MissionTime);
	stream.writeSint32LE(ActualMission);
	stream.writeSint8(ActualDifficulty);

	// visiblty.h:
	stream.writeSint32LE(AllowedBadlife);

	if (BadlifeVisib) {
		stream.writeSint32LE(1);
		stream.write(BadlifeVisib, MapSizeX * MapSizeY);
	} else {
		stream.writeSint32LE(0);
	}

	// units.h:
	for (i = 0; i < GOODLIFE_TOP; i++) {
		stream.writeSint32LE(SelectionHistory[i]);
	}

	stream.writeSint32LE(GoodlifeDeads);
	stream.writeSint32LE(BadlifeDeads);
	stream.writeSint32LE(TimeReserve);

	// fields.h:
	stream.writeSint32LE(MinesCnt);

	for (i = 0; i < MinesCnt; i++) {
		stream.writeSint32LE(MinesList[i].x);
		stream.writeSint32LE(MinesList[i].y);
		stream.writeSint32LE(MinesList[i].party);
	}

	// building.h:
	stream.writeSint32LE(MoneyGoodlife);
	stream.writeSint32LE(MoneyBadlife);

	// autofire.h:
	SaveAutofire(stream);

	// Intentional compatibility break. The first commercial release had
	// a limited number of saves per game. The number of remaining saves
	// was stored here.
	//stream.writeSint32LE(1U << 31);

	SaveArtificialIntelligence(stream);

	for (i = 0; i < 4; i++) {
		UnlockDraw();
	}

	WaitCursor(FALSE);
	return TRUE;
}

int LoadGameState(ReadStream &stream, int format) {
	int i, dummy;
	TPoint pz;

	WaitCursor(TRUE);
	InitEngine(0, &stream);

	// engine.h:
	pz.x = stream.readSint32LE();
	pz.y = stream.readSint32LE();
	TurnTime = stream.readSint32LE();
	MissionTime = stream.readSint32LE();
	ActualMission = stream.readSint32LE();
	ActualDifficulty = stream.readSint8();
	ScrollTo(pz.x, pz.y);
	HideHelpers();

	// visiblty.h:
	AllowedBadlife = stream.readSint32LE();
	dummy = stream.readSint32LE();

	if (BadlifeVisib) {
		memfree(BadlifeVisib);
	}

	if (dummy) {
		BadlifeVisib = (byte*)memalloc(MapSizeX * MapSizeY);
		stream.read(BadlifeVisib, MapSizeX * MapSizeY);
	}

	// units.h:
	for (i = 0; i < GOODLIFE_TOP; i++) {
		SelectionHistory[i] = stream.readSint32LE();
	}

	GoodlifeDeads = stream.readSint32LE();
	BadlifeDeads = stream.readSint32LE();
	TimeReserve = stream.readSint32LE();

	// fields.h:
	MinesCnt = stream.readSint32LE();

	for (i = 0; i < MinesCnt; i++) {
		MinesList[i].x = stream.readSint32LE();
		MinesList[i].y = stream.readSint32LE();
		MinesList[i].party = stream.readSint32LE();
	}

	// building.h:
	MoneyGoodlife = stream.readSint32LE();
	MoneyBadlife = stream.readSint32LE();

	// autofire.h:
	LoadAutofire(stream);

	// loadsave.h:
	// The first commercial release had a limited number of saves per game.
	// The number of remaining saves was stored here.
	if (format < 1) {
		stream.readSint32LE();
	}

	LoadArtificialIntelligence(stream, format);

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


extern bool dirExists(const char *filename);
static const char *getSavesDir()
{
    static char savesDir[PATH_MAX - 256] = "";
    strncpy(savesDir, getSignusConfigDir(), sizeof(savesDir));
    savesDir[sizeof(savesDir) - 1] = '\0';
    strncat(savesDir, "/saved_games", sizeof(savesDir) - strlen(savesDir) - 1);
    savesDir[sizeof(savesDir) - 1] = '\0';
    if (!dirExists(savesDir))
        mkdir(savesDir, 0700);
    return savesDir;
}

int detectFormat(const char *magic) {
	for (int i = 0; magic_list[i]; i++) {
		if (!strcmp(magic, magic_list[i])) {
			return i;
		}
	}

	return -1;
}

void readSavegameHeader(ReadStream &stream, TSavegameHdr &header) {
	stream.read(header.Magic, 12);
	header.format = detectFormat(header.Magic);

	if (header.format < 0) {
		return;
	}

	stream.read(header.Name, 64);

	// Original save format had 32-bit timestamp.
	if (header.format < 1) {
		header.Time = stream.readSint32LE();
	} else {
		header.Time = stream.readSint64LE();
	}
}

void writeSavegameHeader(WriteStream &stream, const char *name) {
	TSavegameHdr header;

	strcpy(header.Magic, magic_list[DEFAULT_SAVEGAME_FORMAT]);
	strncpy(header.Name, name, 63);
	header.Name[63] = '\0';
	header.Time = time(NULL);
	stream.write(header.Magic, 12);
	stream.write(header.Name, 64);
	// Intentional compatibility break. Original save format would contain
	// 32-bit timestamp.
	//stream.writeSint32LE(header.Time);
	stream.writeSint64LE(header.Time);
}

void TLoadSaveDialog::SearchFiles() {
	DIR *dir;
	struct dirent *fi;
	int i, j;
	TSavegameHdr hdr;
	char buf[PATH_MAX];
	File f;

	SCount = IsSave ? 1 : 0;
	for (i = 0; i < 1000; i++) SNames[i] = SFiles[i] = NULL;

	const char *dirname = getSavesDir();
	// searching for savefiles...:
	dir = opendir(dirname);

	for (fi = readdir(dir); fi; fi = readdir(dir)) {
		if (strncmp(fi->d_name, "savegame", 8)) {
			continue;
		}

		snprintf(buf, PATH_MAX, "%s/%s", dirname, fi->d_name);
		f.open(buf);

		if (f.isOpen()) {
			readSavegameHeader(f, hdr);

			if (hdr.format < 0) {
				f.close();
				continue;
			}

			SNames[SCount] = strdup(hdr.Name);
			SFiles[SCount] = strdup(buf);
			STimes[SCount] = -hdr.Time;
			SCount++;
			f.close();
		}
	}

	closedir(dir);

	if (IsSave) { // udela jmeno pro new save:
		int rc;

		for (i = 0; i < 1000; i++) {
			snprintf(buf, PATH_MAX, "%s/savegame.%03i", dirname, i);
			rc = TRUE;

			for (j = 1; j < SCount; j++) {
				if (!strcmp(buf, SFiles[j])) {
					rc = FALSE;
					break;
				}
			}

			if (rc) {
				SFiles[0] = (char*)strdup(buf);
				SNames[0] = (char*)memalloc(strlen(SigText[TXT_NEW]) + 1);
				strcpy(SNames[0], SigText[TXT_NEW]);
				STimes[0] = 0;
				break;
			}
		}
	}

	SortFiles(IsSave ? 1 : 0, SCount-1);
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



void TLoadSaveDialog::GetStamp(int num) {
	TSavegameHdr tmp;

	if (!IsSave || (num != 0)) {
		File f(SFiles[num]);

		if (f.isOpen()) {
			readSavegameHeader(f, tmp);

			if (tmp.format >= 0) {
				f.read(Stamp, STAMP_SZ);
				return;
			}
		}
	}

	void *buf = GraphicsDF->get("newsave");
	memcpy(Stamp, buf, STAMP_SZ);
	memfree(buf);
}



int TLoadSaveDialog::SpecialHandle(TEvent *e, int Cmd) {
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
				memmove(SFiles + Cur, SFiles + (Cur+1),
					sizeof(char*) * (SCount - Cur));
				memmove(SNames + Cur, SNames + (Cur+1),
					sizeof(char*) * (SCount - Cur));
				memmove(STimes + Cur, STimes + (Cur+1),
					sizeof(time_t) * (SCount - Cur));
			}

			m_List->Cnt--;

			if (m_List->Current >= m_List->Cnt) {
				m_List->Current = m_List->Cnt - 1;
			}

			if ((m_List->ScrLn < m_List->Cnt) &&
				(m_List->Delta + m_List->ScrLn > m_List->Cnt)) {
				m_List->Delta--;
			}

			m_List->Draw();
			m_List->Paint();
		}

		GetStamp(m_List->Current);    
		m_Stamp->Draw();
		m_Stamp->Paint();
		return -1;
	} else if ((Cmd == cmOk) && IsSave) {
		Cur = m_List->Current;

		if (Cur == 0) {
			strcpy(buf, "");
		} else {
			strcpy(buf, SNames[Cur]);
		}

		if (InputBox("", buf, 63) == cmOk) {
		    memfree(SNames[Cur]);
		    SNames[Cur] = (char*) memalloc(strlen(buf) + 1);
		    strcpy(SNames[Cur], buf);           
		    return cmOk;
		}

		Draw();
		PaintRect(0, h);
		return -1;
	} else {
		return TDialog::SpecialHandle(e, Cmd);
	}
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

void validate_load_status(SeekableReadStream &stream, const char *filename) {
	if (stream.eos() || stream.pos() != stream.size()) {
		fprintf(stderr, "WARNING: Savegame size mismatch. Some game"
			" state data may be invalid.\n");

		if (filename) {
			fprintf(stderr, "Please send the savegame file %s to"
				" game developers\n", filename);
		}
	}
}

int LoadGame() {
	TLoadSaveDialog *dlg;
	int rtn = FALSE;

	dlg = new TLoadSaveDialog(FALSE/*is NOT save*/);

	if (dlg->Exec() == cmOk) {
		File f(dlg->GetSelectedFile());

		if (f.isOpen()) {
			TSavegameHdr tmp;
			readSavegameHeader(f, tmp);

			if (tmp.format >= 0) {
				if (ActualMission != -1) {
					DoneEngine();
				}

				f.seek(STAMP_SZ, SEEK_CUR);
				LoadGameState(f, tmp.format);
				validate_load_status(f, dlg->GetSelectedFile());
				rtn = TRUE;
			}
		}
	}

	delete dlg;
	{
		TEvent e;
		do {GetEvent(&e);} while (e.What != evNothing);
	}
	return rtn;
}

void SG_PutStamp(WriteStream &stream) {
	byte *buf = (byte*)memalloc(STAMP_SZ);
	byte *b = buf;
	int i, j;

	for (j = 0; j < 420; j += 2) {
		for (i = 0; i < 504; i += 2) {
			*(b++) = ((byte*)FullBuf)[(VIEW_SX * i / 504) + VIEW_OFS_X + ((VIEW_SY * j / 420) + VIEW_OFS_Y) * VIEW_PIXSZ_X];
		}
	}

	stream.write(buf, STAMP_SZ);
	memfree(buf);
}

int SaveGame() {
	TLoadSaveDialog *dlg;
	int rtn = FALSE;

	dlg = new TLoadSaveDialog(TRUE/*is save*/);

	if (dlg->Exec() == cmOk) {
		File f(dlg->GetSelectedFile(), File::WRITE | File::TRUNCATE);

		if (f.isOpen()) {
			writeSavegameHeader(f, dlg->GetSelectedName());
			MsgBox(SigText[TXT_SAVING]);
			SG_PutStamp(f);
			SaveGameState(f);
			MsgBox(NULL);
			rtn = TRUE;
		}
	}

	delete dlg;
	{
		TEvent e;
		do {
			GetEvent(&e);
		} while (e.What != evNothing);
	}
	return rtn;
}

#ifdef DEBUG

// autosave after each turn
int SaveGameDebug() {
	TLoadSaveDialog *dlg;
	int rtn = FALSE;
	char buf[64];

	dlg = new TLoadSaveDialog(TRUE/*is save*/);
	WriteFile f(dlg->SFiles[0]);
	sprintf(buf, "Betatest save: turn %i", ActualTurn);

	if (f.isOpen()) {
		writeSavegameHeader(f, buf);
		SG_PutStamp(stream);
		SaveGameState(stream);
		rtn = TRUE;
	}

	delete dlg;
	return rtn;
}

#endif
