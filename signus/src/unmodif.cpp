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
// Modifikace jednotek, kter? v dan? misi pojedou do akce...
//

#include "global.h"
#include "ui_toolkit.h"
#include "units.h"
#include "etables.h"
#include "mouse.h"

#include "unmodif.h"



/////////////////////////// seynam:

typedef struct {
        int type, level, experience;
    } unit_muinfo;

static unit_muinfo *units_info = NULL;
static int units_cnt = 0;

void MU_SaveList(WriteStream &stream) {
	int i;

	stream.writeSint32LE(units_cnt);

	for (i = 0; i < units_cnt; i++) {
		stream.writeSint32LE(units_info[i].type);
		stream.writeSint32LE(units_info[i].level);
		stream.writeSint32LE(units_info[i].experience);
	}
}

void MU_LoadList(ReadStream &stream) {
	int i;

	MU_ClearList();
	units_cnt = stream.readSint32LE();

	if (units_cnt > 0) {
		units_info = (unit_muinfo*)memalloc(sizeof(unit_muinfo) * units_cnt);
	}

	for (i = 0; i < units_cnt; i++) {
		units_info[i].type = stream.readSint32LE();
		units_info[i].level = stream.readSint32LE();
		units_info[i].experience = stream.readSint32LE();
	}
}



void MU_UpdateList()
{
    int i;
    unit_muinfo *inf;
    
    for (i = 0; i < GOODLIFE_TOP; i++)
    {
        if ((Units[i]) && 
            ((Units[i]->Type < unHorizont) || 
                  ((Units[i]->Type >= unNeptun) && (Units[i]->Type <= unSatan)))) 
        {
            TUnit *u = static_cast<TUnit*>(Units[i]);
            units_cnt++;
            units_info = (unit_muinfo*) memrealloc(units_info, sizeof(unit_muinfo) * units_cnt);
            inf = units_info + units_cnt-1;
            inf->type = u->Type, inf->level = u->Level, inf->experience = u->Experience;
        }
    }
}



void MU_ClearList()
{
    if (units_info) memfree(units_info);
    units_info = NULL;
    units_cnt = 0;
}





/////////////////////////// Tabulky pen?z k dobru:

static int MMoney_e[20] = {0,  200, 1000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
                              5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000};
static int MMoney_n[20] = {0,  200, 1000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
                              5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000};
static int MMoney_h[20] = {0,  200, 1000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
                              5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000};




/////////////////////////// Podpùrn? funkce a prom.:

static int MU_Use = FALSE;
static int UnitList[GOODLIFE_TOP];
static int UL_x[GOODLIFE_TOP], UL_y[GOODLIFE_TOP], UL_orient[GOODLIFE_TOP];
    // odkazy na jednotky, ktere se budou modifikovat


void MU_Reset()
{
    MU_Use = TRUE;
    for (int i = 0; i < BADLIFE; i++) UnitList[i] = FALSE;
}



void MU_AddUnit(int ID)
{
    TUnit *u = (TUnit*) Units[ID];
    if (!MU_Use) return;
    UnitList[ID] = TRUE;
    UL_x[ID] = u->X;
    UL_y[ID] = u->Y;
    UL_orient[ID] = u->Orient;
}




//////////////////////// DIALOG:

#define cmUpgrade 1234
#define cmSell    1235
#define cmBuy     1236
#define cmReset   1237


class TModifyDialog : public TDialog {
        public:
            TListBox *m_ListUnit, *m_ListType;
            TStaticText *m_Info;
            char *LU_Data[GOODLIFE_TOP], *LT_Data[GOODLIFE_TOP];
            int LU_index[GOODLIFE_TOP], LT_type[GOODLIFE_TOP];
            int LU_cnt, LT_cnt;
            char info[100];
            int left_units, left_credits;

            TModifyDialog();
            int SpecialHandle(TEvent *e, int Cmd);
            void GenerateLists(int first);
            ~TModifyDialog();
    };





TModifyDialog::TModifyDialog() : TDialog(RES_X/2 - 320, RES_Y/2 - 240, 640, 480, "dlgunits")
{
    for (int i = 0; i < GOODLIFE_TOP; i++) LU_Data[i] = LT_Data[i] = NULL;
    LU_cnt = LT_cnt = 0;
    left_units = 0;
    switch (ActualDifficulty) {
        case 'e' : left_credits = MMoney_e[ActualMission]; break;
        case 'n' : left_credits = MMoney_n[ActualMission]; break;
        case 'h' : left_credits = MMoney_h[ActualMission]; break;
    }

    GenerateLists(TRUE);
    Insert(new TStaticText(15, 15, 100, 16, SigText[TXT_UNITSTOMIS]));
    Insert(new TStaticText(325, 15, 100, 16, SigText[TXT_AVAILABLEUNITS]));
    m_ListUnit = new TListBox(15, 40, 300, 20, LU_Data, LU_cnt, cmSell);
    Insert(m_ListUnit);
    m_ListType = new TListBox(325, 40, 300, 20, LT_Data, LT_cnt, cmBuy);
    Insert(m_ListType);
    m_Info = new TStaticText(30, 410, 200, 16, info);
    Insert(m_Info);
    Insert(new TButton(30, 430, SigText[TXT_SELL], cmSell));
    Insert(new TButton(160, 430, SigText[TXT_UPGRADE], cmUpgrade));
    Insert(new TButton(325, 430, SigText[TXT_BUY], cmBuy));
    Insert(new TButton(455, 430, SigText[TXT_OK], cmOk));
}



void TModifyDialog::GenerateLists(int first)
{
    int i, j;
    char b[50];
    TUnit *u;

    for (i = 0; i < LU_cnt; i++) memfree(LU_Data[i]);
    LU_cnt = 0;

    for (i = 0; i < GOODLIFE_TOP; i++) {
        if ((!UnitList[i]) || (Units[i] == NULL)) continue;
        u = (TUnit*)Units[i];
        sprintf(b, "%s    (%i)", u->GetName(), u->Level + 1);
        LU_Data[LU_cnt] = (char*) memalloc(strlen(b) + 1);
        strcpy(LU_Data[LU_cnt], b);
        LU_index[LU_cnt] = i;
        LU_cnt++;
    }

    if (first) {
    for (i = 0; i <= unSatan; i++) {
        for (j = 0; j < GOODLIFE_TOP; j++) {
            if ((!UnitList[j]) || (Units[j] == NULL)) continue;
            if (Units[j]->Type == i) {
                sprintf(b, "%s    (%i cr)", Units[j]->GetName(), TabUnitsCost[i]);
                LT_Data[LT_cnt] = (char*) memalloc(strlen(b) + 1);
                strcpy(LT_Data[LT_cnt], b);
                LT_type[LT_cnt] = i;
                LT_cnt++;
                break;
            }
        }
    }
    }
    sprintf(info, SigText[TXT_UNITSCOUNT], left_units, left_credits);
}



int TModifyDialog::SpecialHandle(TEvent *e, int Cmd)
{
    int i, Cur, cost, selID;
    TUnit *u;
    TObject *obj;
    
    if (Cmd == cmUpgrade) {
        Cur = LU_index[m_ListUnit->Current];
        u = (TUnit*)Units[Cur];
        cost = TabUnitsCost[u->Type] / 10;
        if ((u->Level < 9) && (left_credits >= cost)) {
            u->Level++;
            u->Experience = TabUnitsExperience[u->Type % BADLIFE][u->Level];
            u->IncLevel(u->Level);
            u->HitPoints = u->MaxHitPoints;
            left_credits -= cost;
            GenerateLists(FALSE);
            Draw(); PaintRect(0, h);
        }
        return -1;
    }

    else if (Cmd == cmSell) {
        selID = SelectedUnit->ID;
        Cur = LU_index[m_ListUnit->Current];
        u = (TUnit*)Units[Cur];
        cost = TabUnitsCost[u->Type] + TabUnitsCost[u->Type] * u->Level / 10;
        left_credits += cost;
        left_units++;
        delete Units[Cur];
        Units[Cur] = NULL;
        GenerateLists(FALSE);
        m_ListType->Cnt = LT_cnt;
        if (m_ListType->Current >= m_ListType->Cnt) m_ListType->Current = m_ListType->Cnt - 1;
        if ((m_ListType->ScrLn < m_ListType->Cnt) && (m_ListType->Delta + m_ListType->ScrLn > m_ListType->Cnt)) m_ListType->Delta--;
        m_ListUnit->Cnt = LU_cnt;
        if (m_ListUnit->Current >= m_ListUnit->Cnt) m_ListUnit->Current = m_ListUnit->Cnt - 1;
        if ((m_ListUnit->ScrLn < m_ListUnit->Cnt) && (m_ListUnit->Delta + m_ListUnit->ScrLn > m_ListUnit->Cnt)) m_ListUnit->Delta--;
        Draw(); PaintRect(0, h);
        for (i = 0; i < MapSizeX * MapSizeY; i++)
            if (Map[i].Unit == Cur) Map[i].Unit = NO_UNIT;
        if (Cur == selID)
            for (i = 0; i < GOODLIFE_TOP; i++) if (Units[i]) SelectedUnit = Units[i];
        return -1;
    }

    else if (Cmd == cmBuy) {
        Cur = LT_type[m_ListType->Current];
        cost = TabUnitsCost[Cur];
        if ((left_units > 0) && (left_credits >= cost)) {
            left_units--;
            left_credits -= cost;
            for (i = 0; i < BADLIFE; i++)
                if (UnitList[i] && (Units[i] == NULL)) break;
            obj = NULL;
            switch (Cur) {
#include "iounreg.h"
            }
            if (obj) {
                obj->Init(UL_x[i], UL_y[i], GOODLIFE, NULL);
                u = (TUnit*) obj;
                u->Orient = u->ActualSprite = UL_orient[i];
            }
            GenerateLists(FALSE);
            m_ListType->Cnt = LT_cnt;
            if (m_ListType->Current >= m_ListType->Cnt) m_ListType->Current = m_ListType->Cnt - 1;
            if ((m_ListType->ScrLn < m_ListType->Cnt) && (m_ListType->Delta + m_ListType->ScrLn > m_ListType->Cnt)) m_ListType->Delta--;
            m_ListUnit->Cnt = LU_cnt;
            if (m_ListUnit->Current >= m_ListUnit->Cnt) m_ListUnit->Current = m_ListUnit->Cnt - 1;
            if ((m_ListUnit->ScrLn < m_ListUnit->Cnt) && (m_ListUnit->Delta + m_ListUnit->ScrLn > m_ListUnit->Cnt)) m_ListUnit->Delta--;
            Draw(); PaintRect(0, h);
        }
        return -1;
    }
    
    else return TDialog::SpecialHandle(e, Cmd);
}



TModifyDialog::~TModifyDialog()
{
    int i;
    
    for (i = 0; i < LU_cnt; i++) memfree(LU_Data[i]);
    for (i = 0; i < LT_cnt; i++) memfree(LT_Data[i]);
    LU_cnt = LT_cnt = 0;
}





void ModifyUnits()
{
    MouseHide();
    ClearScr();
    MouseShow();

    TModifyDialog *dlg = new TModifyDialog();

    if (!units_info) return; // no more commandos available...
    dlg->Exec();
    delete dlg;
    MU_Use = FALSE;
    for (int i = 0; i < GOODLIFE_TOP; i++) if (Units[i]) SelectedUnit = Units[i];
}
