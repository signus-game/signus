/*
 *  This file is part of Signus: The Artefact Wars
 *  https://github.com/signus-game
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
// Zakladni objekt
//


#include <SDL_timer.h>

#include <cstring>
#include <cmath>
#include "units.h"
#include "events.h"
#include "graphio.h"
#include "sound.h"
#include "explode.h"
#include "showrngs.h"
#include "moveunit.h"
#include "untables.h"
#include "shortcut.h"
#include "fields.h"
#include "autofire.h"
#include "mouse.h"


/////////////////////////////////// COMMON FUNCTIONS AND VARS /////




TObject *SelectedUnit = NULL;
TObject *Units[UNITS_TOP] = {NULL};
TObject **Goodlife = Units + GOODLIFE;
TObject **Badlife = Units + BADLIFE;

int SelectionHistory[GOODLIFE_TOP];

int UnitActionType = uatMove;
int SelectCrossLock = 0;
int StatusbarLock = 0;
int UnitInfoLock = 0;

void *BmpSelected[13];
void *UInfoBuf, *UInfoBkgr;
void *WpnInfoBkg[2];
void *LevelBmps[10];
void *BmpRepair[2];
void *BmpBombing[2];
#ifdef DEBUG
TSprite *SpriteUniversal, *SpriteUniversalBad;
#endif
void *BmpAmmoIcons[4];
void *BmpBigInventory = NULL,
     *BmpSmallInventory = NULL,
     *BmpMediumInventory = NULL;

TIcon *InfoIcon;
TIcon *IconThorOut = NULL, *IconThorIn = NULL;
TIcon *IconSatan = NULL;
TIcon *IconXenon = NULL, *IconXenon2 = NULL;
TIcon *IconTransport = NULL;    
TIcon *IconLand, *IconTakeoff;

char *WpnNames[WPNNM_COUNT];

MIDASsample SelectSnd, WpnSelectSnd, ZavoraSnd;

int *SpriteLocators[UNITS_TOP];

int GoodlifeDeads, BadlifeDeads;

int TimeReserve = 0; // priznak vyhrazeni casy na vystrely: 0,1,2,3



// seznam zvuku:
MIDASsample UnitsSoundSamples[UNIT_TYPES_COUNT][16];
const char *UnitsSoundIndex[UNIT_TYPES_COUNT][16] = {
	{NULL},
	{"0", "tank_l", "lehkana", NULL, NULL, NULL, "zavora", "0", "0"},
	{"0", "tank_s", "tezkana"},
	{"0", "tank", "double", "tezkula", "0", "0"},
	{"0", "tank_ss", "sonic"},
	{"0", "tank_sx", "laedel"},
	{"0", "kolovy1", "raketa_l"},
	{"0", "tank", "raketa"},
	{"0", "tank_l", "pletkan", "tezkul"},
	{"0", "tank", "dalekdel", NULL, NULL, NULL, "0"},
	{"0", "tank_l", "dalekdel"},
	{NULL, "jeeeeep", "kulome2", "0"},
	{NULL, "jeeeeep"},
	{NULL, "kolovy2"},
	{NULL, "kolovy3", "lehkan"},
	{NULL, "kolovy3", NULL, NULL, NULL, NULL, "gargan"},
	{"0", "kolovy3"},
	{"0", "kolovy1"},
	{"0", "kolovy1"},
	{"0", "kolovy2"},
	{"0", NULL, "prletkan"},
	{NULL, NULL, "tezkan"},
	{"0", NULL, "dalekkan"},
	{NULL, NULL, "raketa", NULL, NULL, NULL, "vysu", "zasu"},
	{NULL, NULL, "laedel"},
	{"0", "vlak1"},
	{"0", "vlak", "tezkan", "prletkul"},
	{"0", "vlak3"},
	{"0", "vznas", "tezkan", "prletkul"},
	{"0", "vlak2", "dalekdel"},
	{"0", "vznas2"},
	{"0", "lod1", "lehkan"},
	{"0", "lod1", "tezkan", "hlmina", "prletdel"},
	{"0", "lod7", "tezkan", "torpedo2", "prletdel", "prletdel"},
	{"0", "lod", "dalekkan", "torpedo2", "prletdel"},
	{"0", "lod2"},
	{"0", "lod", "prletkul"},
	{"0", "ponorka", "torpedo", "0"},
	{"0", "stiha", "kulome2", "raketa", NULL, NULL, "stihas", "stihap"},
	{"0", "tebom", "kulome2", "tezkul", NULL, NULL, "teboms", "tebomp"},
	{"0", "lebom", NULL, NULL, NULL, NULL, "leboms", "lebomp"},
	{"0", "letadlo", "kulome", "raketa_r", "raketa_r", NULL, "letadlos", "letadlop"},
	{"0", "letadlo", NULL, NULL, NULL, NULL, "letadlos", "letadlop"},
	{"0", "lebom", NULL, NULL, NULL, NULL, "leboms", "lebomp"},
	{"0", "vrtulnik", "raketa_r", "kulome2", NULL, NULL, "vrtulnis", "vrtulnip", "0"},
	{NULL, "satan", "lehkan", NULL, NULL, NULL, "satan_vy"},
};



// koure u poskozenych jednotek:
TSprite *Smokes[32];


/////////// zaznamy poslednich oblelniku:

int LD_used[UNITS_TOP] = {0};
TRect LD_rect[UNITS_TOP];

void ClearLastDraws()
{
    for (int i = 0; i < UNITS_TOP; i++) LD_used[i] = FALSE;
}







////////////////////////////////////////////////////////////////////////////


////////////////////// TObject 

void TObject::Init(int x, int y, int party, ReadStream *stream) {
    X = x; Y = y; 
    LittleX = 0, LittleY = 0;
    ActualSprite = 0;
    CanChoose = TRUE;
    
    if (party == GOODLIFE)
        for (ID = 0; (ID < BADLIFE) && (Units[ID] != NULL); ID++) {}
    else 
        for (ID = BADLIFE; (ID < UNITS_TOP) && (Units[ID] != NULL); ID++) {}
        
    Units[ID] = this;
    
    Type = GetType();
    if ((ID >= BADLIFE) && (Type < BADLIFE)) Type += BADLIFE;
    Setup();
    AfterSetup();
    PlaceGround(TRUE);
    ChangeInstCnt(+1);
    AI_Info.State = 1; // aistSleeping
    AI_Info.Hps = 0;
    AI_Info.point.x = 0; AI_Info.point.y = 0;
    SmokeOn = FALSE;
}




void TObject::ChangeInstCnt(int delta)
{
    UnitsCounts[Type] += delta;
    if (((delta < 0) && (UnitsCounts[Type] == 0)) || 
        ((delta > 0) && (UnitsCounts[Type] == 1)))
        UpdateUnitsMem(Type);
}



void TObject::PaintUnit(int DoCheck)
{
    TRect r;
    
    if (GetField(X, Y)->Visib == 2) {
        if (DoCheck && (!IsOnScreen())) Center();
        else {
            GetDrawRect(&r);
            if (LD_used[ID]) Union(&r, &(LD_rect[ID]));
            StartDraw(r);
            PaintUnits();
            EndDraw();
        }
    }
    else if (LD_used[ID]) {
        StartDraw(LD_rect[ID]);
        PaintUnits();
        EndDraw();
        LD_used[ID] = FALSE;
    }
}



int TObject::PaintUnitInMove(int bx, int by)
{
    if (GetField(X, Y)->Visib == 2) {
        PaintUnit(TRUE);
        ProcessMapAnim();
	UpdateScreen();
        return TRUE;
    }
    else return FALSE;
}



void TObject::Setup()
{
    MaxHitPoints = 1;
    MaxTimeUnits = 1;
    Defense[0] = Defense[1] = Defense[2] = 1;
}

void TObject::AfterSetup()
{
    HitPoints = MaxHitPoints;
    TimeUnits = MaxTimeUnits;
}



void TObject::Read(ReadStream &stream) {
        int tmp;

	MaxHitPoints = stream.readSint32LE();
	MaxTimeUnits = stream.readSint32LE();
	Defense[0] = stream.readSint32LE();
	Defense[1] = stream.readSint32LE();
	Defense[2] = stream.readSint32LE();

	HitPoints = stream.readSint32LE();
	TimeUnits = stream.readSint32LE();

	AI_Info.State = stream.readSint32LE();
	AI_Info.point.x = stream.readSint32LE();
	AI_Info.point.y = stream.readSint32LE();
	AI_Info.Hps = stream.readSint32LE();

	Visib = stream.readSint32LE();
	ActualSprite = stream.readSint32LE();

	// tohle je cheat - omylem to tu zustalo, tak jsme byli lini to vyndat
	tmp = stream.readSint32LE();

        if (tmp != ID) {
#ifdef DEBUG
		PromtBox("unit-ID misread!", cmOk);
#endif
	}

	SmokeOn = stream.readSint32LE();
}

void TObject::Write(WriteStream &stream) {
	stream.writeSint32LE(MaxHitPoints);
	stream.writeSint32LE(MaxTimeUnits);
	stream.writeSint32LE(Defense[0]);
	stream.writeSint32LE(Defense[1]);
	stream.writeSint32LE(Defense[2]);

	stream.writeSint32LE(HitPoints);
	stream.writeSint32LE(TimeUnits);

	stream.writeSint32LE(AI_Info.State);
	stream.writeSint32LE(AI_Info.point.x);
	stream.writeSint32LE(AI_Info.point.y);
	stream.writeSint32LE(AI_Info.Hps);

	stream.writeSint32LE(Visib);
	stream.writeSint32LE(ActualSprite);
	stream.writeSint32LE(ID); // cheat na shit...

	stream.writeSint32LE(SmokeOn);
}

void TObject::PlaceGround(int place)
{
    if ((X == -1) && (Y == -1)) return; // v baraku nebo transporteru
    
    TField *f = GetField(X, Y);
    if (place) {
        f->Unit = ID;
        Alt = f->Height;
        if (IsAnimated()) f->IsAnim = 1; else f->IsAnim = 0;
    }
    else {
        f->Unit = NO_UNIT;
        f->IsAnim = 0;
    }
    GenerateUnitsList();
}




static int TR_Modify(TUnit *u)
{
    int tr = TimeReserve;
    int tu = u->TimeUnits;
    int tl;
    
    if (tr == 0) return tu;
    if (u->CurWpn == -1) return tu;
    tl = u->Weapons[u->CurWpn]->TimeLost;
    if (tr == 3) {if (tu - tl >= 0) tu -= tl; tr--;}
    if (tr == 2) {if (tu - tl >= 0) tu -= tl; tr--;}
    if (tr == 1) {if (tu - tl >= 0) tu -= tl;}
    return tu;
}



void TObject::Select()
{
    TObject *OSU = SelectedUnit;
    SelectedUnit = this;
    if (OSU != NULL) OSU->Deselect();
    RedrawMapLocks++;

    if ((Type <= unSatan ) && TimeReserve) {
        int oldtu = TimeUnits;
        TimeUnits = TR_Modify((TUnit*)this);
        PrepareMoveTable();
        TimeUnits = oldtu;
        CreateRangeHelpers();
    }
    else {
        PrepareMoveTable();
        CreateRangeHelpers();
    }
    
    if (ID < BADLIFE) {
        SelectField(0, 0);
        PaintUnit();
        SelectField(Mouse.x, Mouse.y);
        ShowUnitInfo();
        
        if (!UnitInfoLock) {
            // pozn: kdyz je UIL>1, nekresli se unitinfo vpravo dole. To nastava
            //   jen kdyz je na tahu pocitac (zabran. to blikani); ve stejnem
            //   okamziku neni vhodne pamatovat si poradi zamerovani jednotek...
            int i;
            for (i = 0; SelectionHistory[i] != ID; i++) {};
            for (; i < GOODLIFE_TOP-1; i++) SelectionHistory[i] = SelectionHistory[i+1];
            SelectionHistory[GOODLIFE_TOP-1] = ID;
        }
    }
    RedrawMapLocks--;
    if (ID < BADLIFE) RedrawMap();
}



void TObject::Deselect()
{
    if (IsPartOnScreen()) PaintUnit(FALSE);
}



void TObject::TurnReset()
{
    TimeUnits = MaxTimeUnits;
    CanChoose = TRUE; 
}



int TObject::MoveRel(int rx, int ry) {return 0;}
int TObject::MoveSUB(int x, int y) {return TRUE;}
int TObject::MoveFarSUB(int x, int y) {return FALSE;}




TSprite *TObject::GetSprite()
{
    return UnitsSprites[Type][ActualSprite];
}



int TObject::IsOnScreen()
{
    TRect r;
    
    GetSpriteRect(&r, GetSprite(), X, Y, LittleX, LittleY);

    return (r.x1 > 0) && (r.y1 > 0) && (r.x2 < VIEW_SX) && (r.y2 < VIEW_SY);
}



int TObject::IsPartOnScreen()
{
    TRect r;
    
    GetSpriteRect(&r, GetSprite(), X, Y, LittleX, LittleY);

    return Intersect(&r, &ScreenArea);
}



/////////

unsigned TObject::GetFieldActions(int x, int y)
{
    unsigned aa = 0;
    TField *f = GetField(x, y);
    TAircraft *a = GetAircraftAt(x, y);
    
    if ((x == X) && (y == Y)) {
        if (a == NULL)
            return GetSupportedActions() & (uatSelect | uatSelectAir);
        else
            return GetSupportedActions() & (uatSelect | a->GetAvailableActions());
    }

    aa = uatMove | uatMoveAir | uatAttackField; 
    if (f->Visib != 2) aa = uatMove | uatMoveAir;
    else if (a != NULL) aa = uatMove | uatAttackField | a->GetAvailableActions();
    if ((f->Unit != NO_UNIT) && (f->Unit != this->ID) && (f->Visib == 2)) {
        aa |= Units[f->Unit]->GetAvailableActions();
        aa ^= uatMove | uatAttackField; // ten zobak znamena "xor" ;-)
    }
        
    return aa & GetSupportedActions();
}



void TObject::SwitchActionType()
{
    unsigned fa = GetFieldActions(SelPos.x, SelPos.y);

    if (fa == 0) {UnitActionType = uatNothing; return;}

    do {
        UnitActionType <<= 1;
        if (UnitActionType == 0) UnitActionType = 1;
    } while ((UnitActionType & fa) == 0);
}



unsigned TObject::GetSupportedActions()
{
    return uatSelect | uatSelectAir;
}



unsigned TObject::GetAvailableActions()
{
    if (ID >= BADLIFE) {
        if (GetField(X, Y)->Visib == 2) return uatAttack;
        else return uatNothing;
    }
    else return uatSelect | uatAttackGood;
}



void TObject::FieldSelected(int x, int y)
{
    if ((UnitActionType & GetFieldActions(x, y)) == 0) {
        UnitActionType = uatNothing;
        SwitchActionType();
    }
}



void TObject::Action(int x, int y)
{
    TField *f = GetField(x, y);
    
    switch (UnitActionType) {
        case uatSelect :             // zamereni jine jednotky
                if (f->Unit == ID) break;
                if (f->Unit == NO_UNIT) break;
                if (Units[f->Unit] != SelectedUnit) {
                    PlaySample(SelectSnd, 8, EffectsVolume, 128);
                    Units[f->Unit]->Select();                   
                }
                UnitActionType = uatSelect;
                break;

        case uatSelectAir :             // zamereni letadla
                {
                TAircraft *a = GetAircraftAt(x, y);
                
                if (a == NULL) break;
                if ((a != NULL) && (a != SelectedUnit)) {
                    PlaySample(SelectSnd, 8, EffectsVolume, 128);
                    a->Select();                  
                }
                break;
                }
    }
}



void TObject::GetCursor(int x, int y, int *cursor, int *selbold)
{
    TField *f = GetField(x, y);

    *cursor = mcurArrow, *selbold = TRUE;
    AirCursorOn = 0;

    switch (UnitActionType) {
        case uatNothing : 
                *cursor = mcurArrow, *selbold = TRUE;
                break;
        case uatSelect : 
                if (f->Unit != ID) *cursor = mcurSelect;
                else *cursor = mcurArrow;
                *selbold = TRUE;
                break;
        case uatSelectAir : 
                {
                TAircraft *a = GetAircraftAt(x, y);
                if (a != NULL) {
                    if (a->ID != ID) *cursor = mcurSelect;
                    else *cursor = mcurArrow;
                    *selbold = TRUE;
                    AirCursorOn = a->FlyLevel;
                }
                else {
                    *selbold = FALSE, *cursor = mcurArrow, AirCursorOn = 2;
                }
                break;
                }
    }
}
//////////////



void TObject::GetFieldInfo(int x, int y, char *buf)
{
    TField *f = GetField(x, y);
    // FIXME: sprintf()
    char b[100];

    if ((f->Unit != NO_UNIT) && (f->Visib == 2)) {
        if (f->Unit >= BADLIFE) {
            sprintf(b, SigText[TXT_TELL_STATE],
                100 * Units[f->Unit]->HitPoints / Units[f->Unit]->MaxHitPoints);
            sprintf(buf, "%s - %s", Units[f->Unit]->GetName(), b);
        }
    else sprintf(buf, "%s", Units[f->Unit]->GetName());
                
    }
    else strcpy(buf, "");
}



void TObject::ShowUnitInfo()
{
    if (UnitInfoLock) return;
    memcpy(UInfoBuf, UInfoBkgr, UINFO_SX * UINFO_SY);
    GetUnitInfo();
    PutBitmap32(UINFO_X, UINFO_Y, UInfoBuf, UINFO_SX, UINFO_SY);
    
    if (iniResolution != SVGA_640x480)
        PutBitmap32(RES_X-118, RES_Y-337, UnitsPictures[Type], 108, 100);
}



void TObject::Center()
{
    switch (iniResolution) {
        case SVGA_640x480  : ScrollTo(X - 11, Y - 3); break;
        case SVGA_800x600  : ScrollTo(X - 15, Y - 5); break;
        case SVGA_1024x768 : ScrollTo(X - 19, Y - 4); break;
    }       
}



void TObject::GetUnitInfo(bool alt_wpinfo)
{
    CopyBmp(UInfoBuf, UINFO_SX, 62, 186, InfoIcon->IconPic[0], 44, 20);
}




int TObject::InfoEvent(TEvent *e)
{
    if (InfoIcon->Handle(e)) {
        TDialog *dlg;
        TEvent e;
        
        SelectField(0, 0);
        do {GetEvent(&e);} while (e.What != evNothing);
        dlg = DetailedInfo();
        dlg->Exec();      
        SelectField(Mouse.x, Mouse.y);
        delete dlg;
        return TRUE;
    }
    else return FALSE;
}



int TObject::Damage(int WpnType, int AttackN, int BonusN)
{
    if (HitPoints <= 0) return 0;

    int UC = AttackN + RollDice();
    int OC = Defense[WpnType] + RollDice();

#ifdef DEBUG
    if (MIDAS_disabled) {
        char buff[100];     
        sprintf(buff, "ATTACK by type %i, UC=%i(%i)/+%i, OC=%i(%i), HPs=%i",
                WpnType, UC, AttackN, BonusN, OC, Defense[WpnType], HitPoints);
        Message(buff);
    }
#endif
    
    if ((UC > OC) || (WpnType == wpnExplos)) { // zasah
        SmokeOn = TRUE;
        if (UC <= OC) {
            HitPoints -= 2; // default zasah raketou nebo granatem
            UC = OC + 2;
        }
        else {
            UC += BonusN;
            HitPoints -= (UC - OC);
        }
        PlaceGround(FALSE); PlaceGround(TRUE); //aby se nastavilo isanim podle dymu
        PaintUnit();
	UpdateScreen();
        if (HitPoints <= 0) {HitPoints = 0; Destroy();}
        return UC - OC;
    }
    else return 0;
}



static int TheShootingOneIsDead = FALSE, TheShootingOne = NO_UNIT;

extern void AI_DeleteUnit(int ID);

void RemoveUnit(int ID)
{
    RemoveFromAutofire(ID);
    AI_DeleteUnit(ID);
    delete Units[ID];   Units[ID] = NULL;
    LD_used[ID] = FALSE;
    if (ID < BADLIFE) GoodlifeDeads++;
    else BadlifeDeads++;
    if (TheShootingOne == ID) TheShootingOneIsDead = TRUE;
}




int TObject::DoVisib()
{
    return ProcessEyeVisib(X, Y, Visib, ID);
}



void TObject::GetDrawRect(TRect *r)
{
    TField *f = GetField(X, Y);
    TSprite *s = GetSprite();
    int rrx = GetRelX(X), rry = GetRelY(Y);
    int drawx = 28 * (rrx - rry) + LittleX + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
    TRect r1;
    
    r->x1 = drawx - s->dx, r->y1 = drawy - s->dy;
    r->x2 = r->x1 + s->w, r->y2 = r->y1 + s->h;

    s = GetSmoke();
    if (s) {
        r1.x1 = drawx - s->dx, r1.y1 = drawy - s->dy;
        r1.x2 = r1.x1 + s->w, r1.y2 = r1.y1 + s->h;
        Union(r, &r1);
    }
}



int TObject::IsAnimated()
{
    return (HitPoints < 2 * MaxHitPoints / 3) &&
           (SmokeOn);
}



void TObject::DoAnim()
{
    PaintUnit(FALSE);   
}



TSprite *TObject::GetSmoke()
{
    if (!SmokeOn) return NULL;
    if ((HitPoints >= 2 * MaxHitPoints / 3) || (HitPoints <= 0)) return NULL;
    if (HitPoints < MaxHitPoints / 3) 
        return Smokes[16 + ((ID % 2) * 8) + ((TimerValue +ID) % 8)];
    else
        return Smokes[00 + ((ID % 2) * 8) + ((TimerValue +ID) % 8)];
}

void TObject::Draw()
{
    TField *f = GetField(X, Y);
    TSprite *s, *sst;
    int rrx = GetRelX(X), rry = GetRelY(Y);
    int drawx = 28 * (rrx - rry) + LittleX + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
    
    if ((!SelectCrossLock) && (SelectedUnit == this) && (f->OnScreen)) {
        void *oldb = MapBuf;
        MapBuf = FullBuf;
        DrawL2Selector(drawx-LittleX, drawy+28-LittleY, f->Terrain, BmpSelected);
        MapBuf = oldb;
  }

    s = GetSprite();
    DrawSprite(drawx, drawy, s);
    
    s = GetSmoke();
    if (s) DrawSprite(drawx, drawy, s);

    if (iniShowStatusbar && (!StatusbarLock)) {
        if ((sst = GetStatusBar()) != NULL) {
            DrawSprite(drawx, drawy, sst);
            memfree(sst);
        }
    }
    GetDrawRect(&(LD_rect[ID])); LD_used[ID] = TRUE;
}





void TObject::Explode()
{
    int drw = Units[ID]->IsPartOnScreen() && (GetField(X, Y)->Visib == 2);
    
    if (drw) {
        AddExplode1x1(X, Y);
        if (rand() % 20 == 0) { // bonus - vetsi boom
            for (int i = 0; i < 6; i++)
                AddExplode1x1(X, Y, 0, 20 - 40 * frand(), 10 - 20 * frand());
        }
    }
    else this->RemoveFromWorld();
}



void TObject::Destroy()
{
    int aID = ID,oldID = ID;

    Explode();
    
    if (this == SelectedUnit) { // bezpecne zamer jinou jednotku...
        if (oldID < BADLIFE) {
            while (++aID != oldID) {
                if (aID >= BADLIFE) aID = 0;
                if ((Units[aID] != NULL) && (Units[aID]->HitPoints > 0) &&
                    (Units[aID]->X != -1)) break;
            }
        }
        else {
            while (++aID != oldID) {
                if (aID >= UNITS_TOP) aID = BADLIFE;
                if ((Units[aID] != NULL)  && (Units[aID]->HitPoints > 0) 
                    && (Units[aID]->X != -1)) break;
            }
        }
        SelectedUnit = NULL;
        if (oldID != aID) Units[aID]->Select();       
    }
}



void TObject::RemoveFromWorld()
{
    PlaceGround(FALSE);
    RemoveUnit(ID);

    UpdateLitMap(TRUE);
    MouseFreeze(LITMAP_X, LITMAP_Y, LITMAP_SIZE, LITMAP_SIZE);
    DrawLitMap();
    MouseUnfreeze();
    UpdateScreen();
}



void TObject::ChangeParty()
{
    PlaceGround(FALSE);
    ChangeInstCnt(-1);
    Units[ID] = NULL;
    if (ID < BADLIFE) {
        Type += BADLIFE;
        for (ID = BADLIFE; (ID < UNITS_TOP) && (Units[ID] != NULL); ID++) {}
    }
    else {
        Type -= BADLIFE;
        for (ID = 0; (ID < BADLIFE) && (Units[ID] != NULL); ID++) {}
    }
    Units[ID] = this;
    ChangeInstCnt(+1);
    PlaceGround(TRUE);  
}



MIDASsample TObject::GetUnitSound(int sound_id)
{
    return UnitsSoundSamples[Type % BADLIFE][sound_id];
}



TObject::~TObject()
{
    ChangeInstCnt(-1);
}










/////////////////// TUnit 

void TUnit::Init(int x, int y, int party, ReadStream *stream) {
	Orient = 0;
	ActualSprite = 0;
	Level = 0;
	Experience = 0;

	TObject::Init(x, y, party, stream);

	if (stream) {
		int moreinfo;

		PlaceGround(FALSE);
		Orient = stream->readSint32LE();
		PlaceGround(TRUE);
		ActualSprite = Orient;
		Level = stream->readSint32LE();
		Experience = TabUnitsExperience[Type % BADLIFE][Level];
		IncLevel(Level);
		// done in AfterSetup(), but IncLevel might have changed HP
		HitPoints = MaxHitPoints;
		moreinfo = stream->readSint32LE();

		if (moreinfo)   {
			PlaceGround(FALSE);
			Read(*stream);
			PlaceGround(TRUE);
		}
	}
}



void TUnit::Setup()
{
    TObject::Setup();
    for (int i = 0; i < 4; i++) Weapons[i] = NULL;
    MaxFuel = 200;
    WeaponsCnt = 0, CurWpn = -1;
    Velocity = 128/*str. hodn.*/;
}

void TUnit::AfterSetup()
{
    TObject::AfterSetup();
    Fuel = MaxFuel;
}



void TUnit::Read(ReadStream &stream) {
	TObject::Read(stream);

	MaxFuel = stream.readSint32LE();

	Orient = stream.readSint32LE();
	Fuel = stream.readSint32LE();
	Experience = stream.readSint32LE();
	Level = stream.readSint32LE();

	WeaponsCnt = stream.readSint32LE();
	CurWpn = stream.readSint32LE();

	for (int i = 0; i < WeaponsCnt; i++) {
		Weapons[i]->Read(stream);
	}
}

void TUnit::Write(WriteStream &stream) {
	TObject::Write(stream);

	stream.writeSint32LE(MaxFuel);

	stream.writeSint32LE(Orient);
	stream.writeSint32LE(Fuel);
	stream.writeSint32LE(Experience);
	stream.writeSint32LE(Level);

	stream.writeSint32LE(WeaponsCnt);
	stream.writeSint32LE(CurWpn);

	for (int i = 0; i < WeaponsCnt; i++) {
		Weapons[i]->Write(stream);
	}
}

void TUnit::WriteInitReq(WriteStream &stream) {
	stream.writeSint32LE(Orient);
	stream.writeSint32LE(Level);
	stream.writeSint32LE(TRUE);
}

void TUnit::draw_special_button(const void *bitmap, unsigned width,
	unsigned height, int apcost) {
	char buf[16];

	sprintf(buf, "%u", apcost);
	CopyBmp(UInfoBuf, UINFO_SX, 2, UINFO_SY - height - 2, bitmap, width,
		height);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 6, UINFO_SY - height, buf,
		NormalFont, clrWhite, clrBlack);
}

int TUnit::CanGoOnField(int x, int y) {
	TField *f = GetField(x, y);
	int *mt1, *mt2;

	if ((x < 0) || (y < 0) || (x >= MapSizeX) || (y >= MapSizeY)) {
		return FALSE;
	}

	if (L1TerrainType[f->Terrain] != tofsL1A) {
		return FALSE;
	}

	if (f->Unit != NO_UNIT) {
		return FALSE;
	}

	GetTerrMove(&mt1, &mt2);

	if (mt2[f->Terrain2] == 0xFF) {
		return FALSE;
	}

	if ((mt1[f->Terrain] == 0xFF) && (mt2[f->Terrain2] == 0)) {
		return FALSE;
	}

	if (field_has_visible_mine(x, y, ID & BADLIFE)) {
		return FALSE;
	}

	return TRUE;
}



unsigned TUnit::GetSupportedActions()
{
    unsigned sa = uatMove | uatSelect | uatSelectAir | uatLoadIn;
    if (CurWpn != -1) sa |= uatAttack | uatAttackGood | uatAttackField;
    return sa;
}



unsigned TUnit::GetAvailableActions()
{
    if (ID >= BADLIFE) {
        if (GetField(X, Y)->Visib == 2) return uatAttack;
        else return uatNothing;
    }
    else return uatSelect | uatAttackGood | uatSupport;
}



void TUnit::GetCursor(int x, int y, int *cursor, int *selbold)
{
    TObject::GetCursor(x, y, cursor, selbold);

    switch (UnitActionType) {
        case uatMove : 
                *cursor = mcurArrow, *selbold = TimeToMove(x, y) >= 0;
                break;
        case uatAttack : case uatAttackField : case uatAttackGood : case uatAttackSub : 
                *cursor = mcurTarget, *selbold = Weapons[CurWpn]->IsInRange(this, X, Y, x, y);
                break;
        case uatAttackAir : case uatAttackAirG :
                *cursor = mcurTarget, *selbold = Weapons[CurWpn]->IsInRange(this, X, Y, x, y);
                AirCursorOn = 2;
                break;
        case uatLoadIn : 
                *cursor = mcurLoadIn, *selbold = TRUE;
                break;
    }
}



void TUnit::Action(int x, int y)
{
    TObject::Action(x, y);

    switch (UnitActionType) {
        case uatAttack : case uatAttackField : case uatAttackGood :
        case uatAttackAir : case uatAttackAirG : case uatAttackSub : 
                Attack(x, y);
                break;
        case uatMove : 
                int ttm;
                if ((ttm = TimeToMove(x, y)) >= 0) Move(x, y);
                else {
                    if (ID < BADLIFE) {
                        if (ttm == -1) Message(MSG_CANNOT_GO);
                        else if (ttm == -2) Message(MSG_NO_ACCESS);
                    }
                }
                break;
        case uatLoadIn : 
                TObject *u = Units[GetField(x, y)->Unit];
                int rt = TRUE;
                
                if (u == this) break;
                if (u->Type == unWarehouse) {
                    ((TWarehouse*)u)->RefullUnit(this);
                    this->Select();
                }
                else if (u->Type == unBase) rt = ((TBase*)u)->LoadUnit(this);
                else if (u->Type == unDocks) rt = ((TDocks*)u)->LoadUnit(this);
                else if (u->Type == unRepairBay) rt = ((TRepairBay*)u)->LoadUnit(this);
                else if (u->Type == unCaesar) rt = ((TCaesar*)u)->LoadUnit(this);
                else rt = ((TTransporter*)u)->LoadUnit(this);

                switch (rt) {
                    case -1 : Message(MSG_OBJECT_IN_WAY); break;
                    case -2 : Message(MSG_NO_FREE_ROOM); break;
                    default : break;
                }
                break;
    }
}



void TUnit::FieldSelected(int x, int y)
{
    TObject::FieldSelected(x, y);
    
    if (UnitActionType & (uatAttack | uatAttackField | uatAttackGood)) {
        TField *f = GetField(X, Y);
        if ((L1TerrainType[f->Terrain] != tofsL1A) || (!f->OnScreen)) return;
    
      WpnRotate(x, y);
  }
}




TSprite *TUnit::GetStatusBar()
{
    TSprite *s = (TSprite*) memalloc(4 * sizeof(int) + 22 * 6);
    int i, sz, clr;

    memset(s, 1, 4 * sizeof(int) + 22 * 6);
    s->w = 22, s->h = 6;
    s->dx = 11, s->dy = 15;
    
    for (i = 0; i < 20; i++)
        s->data[s->w * 3 + i + 1] = s->data[s->w * 4 + i + 1] = 72; // dark green
    sz = 20 * HitPoints / MaxHitPoints;
    if (iniAltEnemyStatusBarColors && ID >= BADLIFE) {
        if (sz < 6) {
            clr = 117; /*purple*/
        } else {
            clr = 227; /*biege*/
        }
    } else {
        if (sz < 6) {
            clr = 10; /*red*/
        } else {
            clr = 59; /*light green*/
        }
    }

    for (i = 0; i < sz; i++) 
        s->data[s->w * 3 + i + 1] = s->data[s->w * 4 + i + 1] = clr;
    sz = 20 * TimeUnits / MaxTimeUnits;
    for (i = 0; i < 20; i++) s->data[s->w * 1 + i + 1] =  clrSeaBlue;
    for (i = 0; i < sz; i++) s->data[s->w * 1 + i + 1] =  242;
    if (ID >= BADLIFE) memset(s->data, 0, 22*2);
    return s;
}



void TUnit::GetUnitInfo(bool alt_wpinfo) {
	char cbuf[30];
	int i, clr;

	TObject::GetUnitInfo(alt_wpinfo);
	CopyBmpNZ(UInfoBuf, UINFO_SX, 77, 1, LevelBmps[Level], 29, 16);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 2, GetName(), NormalFont,
		clrLightBlue, clrBlack);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 26, SigText[TXT_STATE],
		NormalFont, clrWhite, clrBlack);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 42, SigText[TXT_TIME],
		NormalFont, clrWhite, clrBlack);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 58, SigText[TXT_FUEL],
		NormalFont, clrWhite, clrBlack);

	sprintf(cbuf, "%i %%", 100 * HitPoints / MaxHitPoints);
	clr = (100 * HitPoints < 20 * MaxHitPoints) ? clrRed : clrLightBlue2;
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 54, 28, 52, 13, clr,
		clrSeaBlue, (double)HitPoints / MaxHitPoints, cbuf);
	sprintf(cbuf, "%i/%i", TimeUnits, MaxTimeUnits);
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 54, 44, 52, 13, clrLightBlue2,
		clrSeaBlue, (double)TimeUnits / MaxTimeUnits, cbuf);
	sprintf(cbuf, "%i %%", 100 * Fuel / MaxFuel);
	clr = (100 * Fuel < 20 * MaxFuel) ? clrRed : 157;
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 54, 60, 52, 13, clr, 164,
		(double)Fuel / MaxFuel, cbuf);

	if (clr == clrRed) {
		PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 58, SigText[TXT_FUEL],
			NormalFont, clrRed, clrBlack);
	}

	for (i = 0; i < WeaponsCnt; i++) {
		CopyBmp(UInfoBuf, UINFO_SX, 3, 83 + i * 26,
			((i == CurWpn) && (WeaponsCnt != 1)) ?
			WpnInfoBkg[1] : WpnInfoBkg[0], 102, 23);
		PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 5, 86 + i * 26,
			Weapons[i]->Name, NormalFont, clrWhite, clrBlack);
		sprintf(cbuf, "%i/%i", Weapons[i]->Ammo, Weapons[i]->MaxAmmo);
		PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 60, 88 + i * 26, 41,
			13, clrLightBlue2, clrSeaBlue,
			(double)Weapons[i]->Ammo / Weapons[i]->MaxAmmo, cbuf);
	}

	if (CurWpn != -1) {
		CopyBmpNZ(UInfoBuf, UINFO_SX, alt_wpinfo ? 68 : 2,
			alt_wpinfo ? 168 : 188,
			BmpAmmoIcons[Weapons[CurWpn]->GetType()], 30, 13);
		sprintf(cbuf, "%i", Weapons[CurWpn]->TimeLost);
		PutStr(UInfoBuf, UINFO_SX, UINFO_SY, alt_wpinfo ? 77 : 35,
			alt_wpinfo ? 148 : 188, cbuf, NormalFont, clrWhite,
			clrBlack);
	}
}



void TUnit::GetFieldInfo(int x, int y, char *buf)
{
    TField *f = GetField(x, y);
    int ttm;
    char b[100];

    TObject::GetFieldInfo(x, y, buf);
    
    switch (UnitActionType) {
        case uatAttack : case uatAttackField : case uatAttackGood : 
                if (!Weapons[CurWpn]->IsInRange(this, X, Y, x, y)) {
                    if (f->Unit != NO_UNIT)
                        sprintf(buf, "%s - %s", Units[f->Unit]->GetName(), SigText[TXT_OUT_OF_WPNRNG]);
                    else
                        strcpy(buf, SigText[TXT_OUT_OF_WPNRNG]);
                }
                else if (f->Unit != NO_UNIT) {
                    sprintf(b, SigText[TXT_TELL_STATE],
                            100 * Units[f->Unit]->HitPoints / Units[f->Unit]->MaxHitPoints);
                    sprintf(buf, "%s - %s", Units[f->Unit]->GetName(), b);
                }
                else strcpy(buf, "");
                break;
        case uatAttackAir : case uatAttackAirG : 
                if (!Weapons[CurWpn]->IsInRange(this, X, Y, x, y)) {
                    if (GetAircraftAt(x, y))
                        sprintf(buf, "%s - %s", GetAircraftAt(x, y)->GetName(), SigText[TXT_OUT_OF_WPNRNG]);
                    else
                        strcpy(buf, SigText[TXT_OUT_OF_WPNRNG]);
                }
                else if (GetAircraftAt(x, y) != NULL) {
                    sprintf(b, SigText[TXT_TELL_STATE],
                            100 * GetAircraftAt(x, y)->HitPoints / GetAircraftAt(x, y)->MaxHitPoints);
                    sprintf(buf, "%s - %s", GetAircraftAt(x, y)->GetName(), b);
                }
                else strcpy(buf, "");
                break;
        case uatMove : case uatMoveAir : 
                if ((ttm = TimeToMove(x, y)) >= 0)
                    sprintf(buf, SigText[TXT_TELL_TIME], TimeUnits-ttm);
                else {
                    if (ttm == -1) strcpy(buf, SigText[TXT_OUT_OF_RANGE]);
                    else if (ttm == -2) strcpy(buf, SigText[TXT_NO_ACCESS]);
                }
                break;
    }
}



int TUnit::InfoEvent(TEvent *e)
{
    if (TObject::InfoEvent(e)) return TRUE;
    
    // zbrane:
    if ((e->What == evMouseDown) && (e->Mouse.Buttons == mbLeftButton)) {
        for (int i = 0; i < WeaponsCnt; i++) {
            if (IsInRect(e->Mouse.Where.x - UINFO_X, e->Mouse.Where.y - UINFO_Y,
                         3, 83 + i * 26, 104, 105 + i * 26)) {
              if (i !=  CurWpn) {
                    PlaySample(WpnSelectSnd, 8, EffectsVolume, 128);
                  CurWpn = i;
                  if (TimeReserve) Select();
                  else {
                      CreateRangeHelpers();
                        ShowUnitInfo();
                    }
                }
                    return TRUE;
            }
        }   
    }
    else if ((e->What == evKeyDown) && (e->Key.KeyCode == SHORTCUT_SWITCHWPN) &&
             (WeaponsCnt > 1)) {
        if (++CurWpn == WeaponsCnt) CurWpn = 0;
        PlaySample(WpnSelectSnd, 8, EffectsVolume, 128);
      if (TimeReserve) Select();
      else {
          CreateRangeHelpers();
            ShowUnitInfo();
        }
        return TRUE;
    }
    return FALSE;     
}




MIDASsamplePlayHandle       ShootSound;


extern void AI_Attack(TUnit *Unit, int x, int y);
extern int IsEnemyTurn;

int TUnit::Attack(int x, int y)
{
    int shot, lastlives = TRUE;

    if ((X == x) && (Y == y) && (Type % BADLIFE != unKobra)) return FALSE;
    if (CurWpn == -1) return FALSE;
    if (
        (Alt == GetField(X, Y)->Height)/*tj test na letadla*/ &&
        ((GetField(X, Y)->Terrain % 256 >= tofsL1B) &&
        //test na ty, co muzou strilet na svahu:
        ((GetType() != unPerseusAlfa) && (GetType() != unPerseusBeta) && (GetType() != unPerseusGama) &&
         (GetType() != unBumerang) && (GetType() != unFalanga) && (GetType() != unDirrac) &&
         (GetType() != unAgni)))) {
        if (ID < BADLIFE) Message(MSG_CANNOT_SHOOT);
        return -4;
    }
    if (!Weapons[CurWpn]->IsInRange(this, X, Y, x, y)) {
        if (ID < BADLIFE) Message(MSG_OUT_OF_WPNRNG);
        return -3;
    }
    if (TimeUnits < Weapons[CurWpn]->TimeLost) {
        if (ID < BADLIFE) Message(MSG_OUT_OF_TIME);
        return -2;
    }
    HideHelpers();
    if (GetField(x, y)->Unit != NO_UNIT) 
        lastlives = Units[GetField(x, y)->Unit]->HitPoints;
    
    ShootSound = PlaySample(GetUnitSound(2 + CurWpn), 6, EffectsVolume, GetFieldPanning(X, Y));
    shot = Weapons[CurWpn]->Shoot(this, X, Y, x, y);
    if (shot == -1) {
        StopSample(ShootSound);
        if ((ID < BADLIFE) && (!IsEnemyTurn)) Message(MSG_OUT_OF_AMMO);
        ShowHelpers();
        return -1;
    }
    TheShootingOne = this->ID, TheShootingOneIsDead = FALSE;
    DoExplosion(); // anim
    //StopSample(ShootSound);
    if (!TheShootingOneIsDead) {
      AI_Attack(this, x, y);
        IncExper(shot); 
        TimeUnits -= Weapons[CurWpn]->TimeLost;
        if (this == SelectedUnit) Select();
        if (GetField(x, y)->Unit != NO_UNIT)
            lastlives -= Units[GetField(x, y)->Unit]->HitPoints;
    }
    ShowHelpers();
    return lastlives;
}



void TUnit::IncExper(int value)
{
    int newlev, i;
    Experience += value;
    if (Level == 9) return;
    for (newlev = i = 0; i < 10; i++)
        if (Experience >= TabUnitsExperience[Type % BADLIFE][i]) newlev = i;
    if (newlev != Level) {
        int HPd = MaxHitPoints - HitPoints;
        if (ID < BADLIFE) SaySpeech("increas", 100);
        Level = newlev; IncLevel(Level);
        HitPoints = MaxHitPoints - HPd;   // aby jednotku neochudili
        if (ID < BADLIFE) ShowUnitInfo();
    }
}






static int attack_done;

void TUnit::GoOnField()
{
    TField *f = GetField(X, Y);
    
    // zavory:
    if (f->Terrain2 == 260) {
        PlaceL2(X, Y, 261);
        PlaySample(ZavoraSnd, 7, EffectsVolume, GetFieldPanning(X, Y));
    }
    if (f->Terrain2 == 275) {
        PlaceL2(X, Y, 276);
        PlaySample(ZavoraSnd, 7, EffectsVolume, GetFieldPanning(X, Y));
    }
    
    // autofire:
    if (CanAutofireOn(this)) {
        DoAutofire(this);
        attack_done = TRUE;
    }
}



void TUnit::GoOnMine()
{
    MineExplode(X, Y);
    attack_done = TRUE;
}




////// otaceni:


int RottTbl[8][8] = 
{{ 0,+1,+1,+1,+1,-1,-1,-1},
 {-1, 0,+1,+1,+1,+1,-1,-1},
 {-1,-1, 0,+1,+1,+1,+1,-1},
 {-1,-1,-1, 0,+1,+1,+1,+1},
 {+1,-1,-1,-1, 0,+1,+1,+1},
 {+1,+1,-1,-1,-1, 0,+1,+1},
 {+1,+1,+1,-1,-1,-1, 0,+1},
 {+1,+1,+1,+1,-1,-1,-1, 0}};

void TUnit::Rotate(int angle)
{
    if (angle == Orient) return;
    if ((GetField(X, Y)->Terrain < tofsL1B) || (GetField(X, Y)->Height != Alt)) {
        while (angle != Orient)
            RotateRel(RottTbl[Orient][angle]);
    }
    else {
        LittleX = 0, LittleY = 0;
        Orient = angle;
        ActualSprite = Orient;
        if (GetField(X, Y)->Visib == 2) {
            PaintUnit();
	    UpdateScreen();
            SDL_Delay(iniAnimDelay2);
        }
    }
}



#define PI 3.14

void TUnit::Rotate(int x, int y)
{
    double a;
    int angle;
    
    if ((x == X) && (y == Y)) return;
    if (x > X) {
        if (y > Y) a = PI * 3/2 - atan((double)(100*y - 100*Y) / (double)(100*x - 100*X));
        else a = PI * 3/2 + atan((double)(100*Y - 100*y) / (double)(100*x - 100*X));
    }
    else {
        if (y > Y) a = PI * 1/2 + atan((double)(100*y - 100*Y) / (double)(100*X - 100*x));
        else a = PI * 1/2 - atan((double)(100*Y - 100*y) / (double)(100*X - 100*x));
    }
    a -= PI/4;
    angle = roundnum(8 * a / (2*PI));
    if (angle < 0) angle = 8 + angle;
    if (angle >= 8) angle -= 8;

    Rotate(angle);
}



void TUnit::RotateRel(int ra)
{
    Orient += ra;
    LittleX = 0, LittleY = 0;
    if (Orient < 0) Orient = 7;
    else if (Orient > 7) Orient = 0;
    ActualSprite = Orient;
    if (GetField(X, Y)->Visib == 2) {
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);
    }
}



void TUnit::WpnRotate(int x, int y)
{
    double a;
    int angle;
    
    if ((x == X) && (y == Y)) return;
    if (x > X) {
        if (y > Y) a = PI * 3/2 - atan((double)(100*y - 100*Y) / (double)(100*x - 100*X));
        else a = PI * 3/2 + atan((double)(100*Y - 100*y) / (double)(100*x - 100*X));
    }
    else {
        if (y > Y) a = PI * 1/2 + atan((double)(100*y - 100*Y) / (double)(100*X - 100*x));
        else a = PI * 1/2 - atan((double)(100*Y - 100*y) / (double)(100*X - 100*x));
    }
    a -= PI/4;
    angle = roundnum(8 * a / (2*PI));
    if (angle < 0) angle = 8 + angle;
    if (angle >= 8) angle -= 8;

    if ((abs(angle - Orient) == 1) || (abs(angle - Orient) == 7)) {
        Orient = ActualSprite = angle;
        PaintUnit();
	UpdateScreen();
    }
    else
        Rotate(angle);
}



void TUnit::ShowShootAt(int x, int y, int phase)
{
    if (GetField(X, Y)->Terrain >= tofsL1B) return;
    if (phase == 0) {
        HideHelpers();
        WpnRotate(x, y);
        ActualSprite = 24 + Orient;
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);   
        ActualSprite = 32 + Orient;
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);   
        ActualSprite = 24 + Orient;
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);   
        ActualSprite =  0 + Orient;
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);   
        ShowHelpers();
    }
}



#define REPAIR_INDEX    1.0

int TUnit::GetRepairCost()
{
    int max;
    
    if (HitPoints < MaxHitPoints/2) max = MaxHitPoints/2;
    else max = MaxHitPoints;
    return (((int)(REPAIR_INDEX * TabUnitsCost[Type % BADLIFE]))
            * (max - HitPoints) / MaxHitPoints);
}



void TUnit::Repair()
{
    if (HitPoints == MaxHitPoints) return;
    if (GetMoney(ID) < GetRepairCost()) return;
    SetMoney(ID, GetMoney(ID) - GetRepairCost());
    if (HitPoints < MaxHitPoints/2) HitPoints = MaxHitPoints/2;
    else HitPoints = MaxHitPoints;
}



TUnit::~TUnit()
{
    for (int i = 0; i < WeaponsCnt; i++) delete Weapons[i];
}









///// odvozene jendotku:

//////////////// TToweredUnit:

void TToweredUnit::Init(int x, int y, int party, ReadStream *stream) {
	WpnOrient = -1;
	TUnit::Init(x, y, party, stream);

	if (WpnOrient == -1) {
		WpnOrient = Orient;
	}
}



void TToweredUnit::Rotate(int angle)
{
    if ((angle == Orient) && (angle == WpnOrient)) return;
    if (GetField(X, Y)->Terrain < tofsL1B) {
        while ((angle != Orient) || (angle != WpnOrient)) {
            if (angle != WpnOrient) WpnRotRel(RottTbl[WpnOrient][angle]);
            RotateRel(RottTbl[Orient][angle]);
        }
    }
    else {
        LittleX = 0, LittleY = 0;
        Orient = WpnOrient = angle;
        ActualSprite = Orient;
        if (GetField(X, Y)->Visib == 2) {
            PaintUnit();
	    UpdateScreen();
            SDL_Delay(iniAnimDelay2);
        }
    }
}



void TToweredUnit::WpnRot(int ang)
{
    if (ang == WpnOrient) return;
    if (GetField(X, Y)->Terrain < tofsL1B) {
        while (ang != WpnOrient) {
            WpnRotRel(RottTbl[WpnOrient][ang]);
            if (GetField(X, Y)->Visib == 2) {
                PaintUnit();
		UpdateScreen();
                SDL_Delay(iniAnimDelay2);
            }
        }
    }
    else {
        WpnOrient = ang;
        if (GetField(X, Y)->Visib == 2) {
            PaintUnit();
	    UpdateScreen();
            SDL_Delay(iniAnimDelay2);
        }
    }
}



void TToweredUnit::WpnRotRel(int r)
{
    WpnOrient += r;
    if (WpnOrient < 0) WpnOrient = 7;
    else if (WpnOrient > 7) WpnOrient = 0;
}



void TToweredUnit::WpnRotate(int x, int y)
{
    double a;
    int angle;
    
    if ((x == X) && (y == Y)) return;
    if (x > X) {
        if (y > Y) a = PI * 3/2 - atan((double)(100*y - 100*Y) / (double)(100*x - 100*X));
        else a = PI * 3/2 + atan((double)(100*Y - 100*y) / (double)(100*x - 100*X));
    }
    else {
        if (y > Y) a = PI * 1/2 + atan((double)(100*y - 100*Y) / (double)(100*X - 100*x));
        else a = PI * 1/2 - atan((double)(100*Y - 100*y) / (double)(100*X - 100*x));
    }
    a -= PI/4;
    angle = roundnum(8 * a / (2*PI));
    if (angle < 0) angle = 8 + angle;
    if (angle >= 8) angle -= 8;

    if ((abs(angle - WpnOrient) == 1) || (abs(angle - WpnOrient) == 7)) {
        WpnOrient = angle;
        PaintUnit();
	UpdateScreen();
    }
    else
        WpnRot(angle);
}



void TToweredUnit::Draw()
{
    TField *f = GetField(X, Y);
    TSprite *s, *sst;
    int rrx = GetRelX(X), rry = GetRelY(Y);
    int drawx = 28 * (rrx - rry) + LittleX + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
    
    if ((!SelectCrossLock) && (SelectedUnit == this)) {
        void *oldb = MapBuf;
        MapBuf = FullBuf;
        DrawL2Selector(drawx-LittleX, drawy+28-LittleY, f->Terrain, BmpSelected);
        MapBuf = oldb;
  }

    s = GetSprite();
    DrawSprite(drawx, drawy, s);
    if (ActualSprite < 8/*tj.jsme v rovine*/) {
        s = UnitsSprites[Type][40 + WpnOrient];
        DrawSprite(drawx + SpriteLocators[Type][Orient*2], 
                   drawy + SpriteLocators[Type][Orient*2+1], s);
    }

    s = GetSmoke();
    if (s) DrawSprite(drawx, drawy, s);

    if (iniShowStatusbar && (!StatusbarLock)) {
        if ((sst = GetStatusBar()) != NULL) {
            DrawSprite(drawx, drawy, sst);
            memfree(sst);
        }
    }
    GetDrawRect(&(LD_rect[ID])); LD_used[ID] = TRUE;
}



void TToweredUnit::GetDrawRect(TRect *r)
{
    TRect r1;
    TField *f = GetField(X, Y);
    TSprite *s = GetSprite();
    int rrx = GetRelX(X), rry = GetRelY(Y);
    int drawx = 28 * (rrx - rry) + LittleX + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
    
    r->x1 = drawx - s->dx, r->y1 = drawy - s->dy;
    r->x2 = r->x1 + s->w, r->y2 = r->y1 + s->h;

    s = UnitsSprites[Type][40 + WpnOrient]; 
    r1.x1 = drawx + SpriteLocators[Type][Orient*2] - s->dx, 
    r1.y1 = drawy + SpriteLocators[Type][Orient*2+1] - s->dy;
    r1.x2 = r1.x1 + s->w, r1.y2 = r1.y1 + s->h;
    Union(r, &r1);
    
    s = GetSmoke();
    if (s) {
        r1.x1 = drawx - s->dx, r1.y1 = drawy - s->dy;
        r1.x2 = r1.x1 + s->w, r1.y2 = r1.y1 + s->h;
        Union(r, &r1);
    }
}



void TToweredUnit::Read(ReadStream &stream) {
	TUnit::Read(stream);
	WpnOrient = stream.readSint32LE();
}

void TToweredUnit::Write(WriteStream &stream) {
	TUnit::Write(stream);
	stream.writeSint32LE(WpnOrient);
}


void TToweredUnit::ShowShootAt(int x, int y, int phase)
{
    int oldwo;

    if (GetField(X, Y)->Terrain >= tofsL1B) return;

    if (phase == 0) {
        HideHelpers();
        WpnRotate(x, y);
        oldwo = WpnOrient;
        WpnOrient = oldwo - 16;    /*odkaz na jiny sprite- hlaven v zakluzu*/
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);   
        WpnOrient = oldwo - 8;
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);   
        WpnOrient = oldwo - 16;
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);   
        WpnOrient = oldwo;
        PaintUnit();
	UpdateScreen();
        SDL_Delay(iniAnimDelay2);   
        ShowHelpers();
    }
}






///// tsupportunit:

void TSupportUnit::Explode() {
	int i;
	int drw = Units[ID]->IsPartOnScreen() && (GetField(X, Y)->Visib == 2);

	if (drw) {
		for (i = 0; i < 6; i++) {
			AddExplode1x1(X, Y, 0, 20 - 40 * frand(),
				10 - 20 * frand());
		}

		IncExplodeTime(+2);
		AddExplode1x1(X, Y);
	}

	WeaponAttack(X - 1, Y - 1, wpnExplos, utSUPPORT_DESTROY_AN, utSUPPORT_DESTROY_BN);
	WeaponAttack(X + 1, Y - 1, wpnExplos, utSUPPORT_DESTROY_AN, utSUPPORT_DESTROY_BN);
	WeaponAttack(X    , Y - 1, wpnExplos, utSUPPORT_DESTROY_AN, utSUPPORT_DESTROY_BN);
	WeaponAttack(X - 1, Y + 1, wpnExplos, utSUPPORT_DESTROY_AN, utSUPPORT_DESTROY_BN);
	WeaponAttack(X + 1, Y + 1, wpnExplos, utSUPPORT_DESTROY_AN, utSUPPORT_DESTROY_BN);
	WeaponAttack(X    , Y + 1, wpnExplos, utSUPPORT_DESTROY_AN, utSUPPORT_DESTROY_BN);
	WeaponAttack(X - 1, Y    , wpnExplos, utSUPPORT_DESTROY_AN, utSUPPORT_DESTROY_BN);
	WeaponAttack(X + 1, Y    , wpnExplos, utSUPPORT_DESTROY_AN, utSUPPORT_DESTROY_BN);

	if (!drw) {
		this->RemoveFromWorld();
	}
}



unsigned TSupportUnit::GetSupportedActions()
{
    return uatMove | uatSelect | uatSelectAir | uatLoadIn | uatSupport;
}



void TSupportUnit::GetCursor(int x, int y, int *cursor, int *selbold)
{
    TField *f = GetField(x, y);
    
    TUnit::GetCursor(x, y, cursor, selbold);
    
    switch (UnitActionType) {
        case uatSupport:
                if (f->Unit != ID)    *cursor = mcurSupport;
                else    *cursor = mcurArrow;
                *selbold = TRUE;
                break;
    }
}



void TSupportUnit::Action(int x, int y)
{
    TUnit::Action(x, y);
    
    switch (UnitActionType) {
        case uatSupport : 
                DoSupport(x, y);
                break;
    }
}



void TSupportUnit::DoSupport(int x, int y)
{
    TField *f = GetField(x, y);
    int t, i, j, dx = 0, dy = 0, best = 0xFFFF;
    
    if (TimeUnits < utSUPP_SUPPORT) {
        if (ID < BADLIFE) Message(MsgText[MSG_OUT_OF_TIME]);
        return;
    }
    if (f->Unit == NO_UNIT) return;
    if ((f->Unit < BADLIFE) && (this->ID >= BADLIFE)) return;
    if ((f->Unit >= BADLIFE) && (this->ID < BADLIFE)) return;
    if (!WillSupport((TUnit*)Units[f->Unit])) return;
    
    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++) {
            if ((X == x + i) && (Y == y + j)) {
                dx = i, dy = j;
                best = 0;
            }
            t = TimeToMove(x + i, y + j);
            if ((t > 0) && (t < best)) {
                best = t;
                dx = i, dy = j;
            }
        }
    
    if ((dx == 0) && (dy == 0)) return;
    if (!Move(x + dx, y + dy)) return;
    if (TimeUnits < utSUPP_SUPPORT) {
        if (ID < BADLIFE) Message(MsgText[MSG_OUT_OF_TIME]);
        return;
    }
    if ((x + dx == X) && (y + dy == Y)) {
        SupportUnit((TUnit*)Units[f->Unit]);
        TimeUnits -= utSUPP_SUPPORT;
        if (this == SelectedUnit) Select();
    }
}

















////////////////////// ALGORITMY POHYBU JEDNOTEK //////////////////////


// prace s tabulkami:

#define SQRT2   1.4142

word *TimeTable = NULL;
byte *DirTable = NULL;
int MTsize, MTofs, MTofsX, MTofsY, MTsizeX, MTsizeY;
TRect MTrect;



inline word TTget(int x, int y)
{
    return TimeTable[(x - MTofsX) + (y - MTofsY) * MTsizeX];
}

inline byte DTget(int x, int y)
{
    return DirTable[(x - MTofsX) + (y - MTofsY) * MTsizeX];
}

inline void DTset(int x, int y, byte dir)
{
    DirTable[(x - MTofsX) + (y - MTofsY) * MTsizeX] = dir;
}

inline void TTset(int x, int y, word tim)
{
    TimeTable[(x - MTofsX) + (y - MTofsY) * MTsizeX] = tim;
}


inline void TDTset(int x, int y, word time, byte dir)
{
    TimeTable[(x - MTofsX) + (y - MTofsY) * MTsizeX] = time;
    DirTable[(x - MTofsX) + (y - MTofsY) * MTsizeX] = dir;
}




int fwDirX[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int fwDirY[8] = {1, 1, 0, -1, -1, -1, 0, 1};

void FindWay(int x, int y, byte setthis)
{
    byte d = DTget(x, y);
    
    DTset(x, y, setthis);
    if (d == 0xFF) return;
    FindWay(x + fwDirX[d], y + fwDirY[d], 100 + d);
}



extern MIDASsamplePlayHandle     SampleMoving;


int TObject::Move(int x, int y)
{
    int oldID = ID;
    int rt;
    
    do {
        attack_done = FALSE;
        rt = MoveSUB(x, y);
        if (!Units[oldID]) break;
        if (attack_done) this->Select();
    } while (attack_done);
    return rt;
}


int TObject::MoveFar(int x, int y)
{
    int oldID = ID;
    int rt;
    
    do {
        attack_done = FALSE;
        rt = MoveFarSUB(x, y);
        if (!Units[oldID]) break;
        if (attack_done) this->Select();
    } while (attack_done);
    if (Units[oldID]) this->Select();
    return rt;
}








int TUnit::MoveSUB(int x, int y)
{
    byte dir;
    int oldtime = TimeUnits;
    int bre;
    int canin = (ID >= BADLIFE) || (iniStopOnNewEnemy);
    
    if ((x == X) && (y == Y)) return TRUE;
    MoveRel_Counter = 0;
    if (TimeToMove(x, y) <= 0) return TRUE;
    RemoveSubmarines();
    FindWay(x, y, 128/*magicnumber*/);
    SmokeOn = FALSE;
    HideHelpers();
    SampleMoving = INVALID_SAMPLE_HANDLE;
    attack_done = FALSE;
    while ((X != x) || (Y != y)) {
        if ((--Fuel) <= 0) {
            Fuel = 0;
            if (ID < BADLIFE) Message(MSG_OUT_OF_FUEL);
            break;
        };
        dir = DTget(X, Y);
        bre=0;
        if      (dir == 100) {Rotate(7); if ((MoveRel( 0, -1) > 0) && canin) bre=1;}
        else if (dir == 101) {Rotate(0); if ((MoveRel(-1, -1) > 0) && canin) bre=1;}
        else if (dir == 102) {Rotate(1); if ((MoveRel(-1,  0) > 0) && canin) bre=1;}
        else if (dir == 103) {Rotate(2); if ((MoveRel(-1, +1) > 0) && canin) bre=1;}
        else if (dir == 104) {Rotate(3); if ((MoveRel( 0, +1) > 0) && canin) bre=1;}
        else if (dir == 105) {Rotate(4); if ((MoveRel(+1, +1) > 0) && canin) bre=1;}
        else if (dir == 106) {Rotate(5); if ((MoveRel(+1,  0) > 0) && canin) bre=1;}
        else if (dir == 107) {Rotate(6); if ((MoveRel(+1, -1) > 0) && canin) bre=1;}
    
        TimeUnits = oldtime - TimeToMove(X, Y);

        if (flagUnitOnMine) {
            GoOnMine();
            flagUnitOnMine = FALSE;
            DoExplosion();
            break;
        }

        GoOnField();
        if (attack_done) {
            DoExplosion();
            break;
        }
        if (bre==1) break;
    }
    PlaceSubmarines();
    FadeOutSample(SampleMoving, EffectsVolume);
    StopSample(SampleMoving);   SampleMoving = INVALID_SAMPLE_HANDLE;
    if (!attack_done) {
        if (canin && (ID < BADLIFE) && (VisibEnemies > 0)) Message(MSG_NEW_ENEMY);
        DoVisib();
        if (this == SelectedUnit) Select();
    }
    ShowHelpers();
    UpdateLitMap(TRUE);
    MouseFreeze(LITMAP_X, LITMAP_Y, LITMAP_SIZE, LITMAP_SIZE);
    DrawLitMap();
    MouseUnfreeze();
    UpdateScreen();
    return (!attack_done);
}



int TObject::TimeToMove(int x, int y)
{
    if (TimeTable == NULL) return -1;
    if ((x < MTrect.x1) || (y < MTrect.y1) || 
        (x > MTrect.x2) || (y > MTrect.y2)) return -1;
    if (TTget(x, y) == 0xFFFF) return -2;
    else if (TTget(x, y) == 0xFFFE) return -1;
    else return TTget(x, y);
}




int *TerrTbl, *Terr2Tbl;
byte *FieldTimeTbl[9];

const double DirecModif[8] = {1, SQRT2, 1, SQRT2, 1, SQRT2, 1, SQRT2};

word raw_field_time(const TField *f) {
	if (Terr2Tbl[f->Terrain2]) {
		return Terr2Tbl[f->Terrain2];
	}

	return TerrTbl[f->Terrain];
}

void TObject::PrepareFieldTime(int x, int y)
{
    word ft, ter;
    TField *f = GetField(x, y);
    int i;
    int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

    ter = f->Terrain;
    FieldTimeTbl[8][tbofs] = ft = raw_field_time(f);
    if (((f->Unit != NO_UNIT) && (f->Unit != ID)) ||
        (field_has_visible_mine(x, y, ID & BADLIFE) && (f->Unit != ID))) {
        for (i = 0; i < 9; i++)
            FieldTimeTbl[i][tbofs] = 0xFF;
    }
    else if ((L1TerrainType[ter] == tofsL1B) ||
             (L1TerrainType[ter] == tofsL1D)) {
        FieldTimeTbl[0][tbofs] = ft;
        FieldTimeTbl[4][tbofs] = ft;
        for (i = 1; i < 4; i++)
            FieldTimeTbl[i][tbofs] = 0xFF;
        for (i = 5; i < 8; i++)
            FieldTimeTbl[i][tbofs] = 0xFF;      
    }
    else if ((L1TerrainType[ter] == tofsL1C) ||
             (L1TerrainType[ter] == tofsL1E)) {
        FieldTimeTbl[2][tbofs] = ft;
        FieldTimeTbl[6][tbofs] = ft;
        for (i = 3; i < 6; i++)
            FieldTimeTbl[i][tbofs] = 0xFF;
        FieldTimeTbl[0][tbofs] = 0xFF;
        FieldTimeTbl[1][tbofs] = 0xFF;
        FieldTimeTbl[7][tbofs] = 0xFF;
    }
    else if ((L1TerrainType[ter] == tofsL1H) ||
             (L1TerrainType[ter] == tofsL1F) ||
             (L1TerrainType[ter] == tofsL1L) ||
             (L1TerrainType[ter] == tofsL1J)) {
        FieldTimeTbl[1][tbofs] = ft;
        FieldTimeTbl[5][tbofs] = ft;
        for (i = 2; i < 5; i++)
            FieldTimeTbl[i][tbofs] = 0xFF;
        FieldTimeTbl[0][tbofs] = 0xFF;
        FieldTimeTbl[6][tbofs] = 0xFF;
        FieldTimeTbl[7][tbofs] = 0xFF;
    }
    else if ((L1TerrainType[ter] == tofsL1I) ||
             (L1TerrainType[ter] == tofsL1G) ||
             (L1TerrainType[ter] == tofsL1K) ||
             (L1TerrainType[ter] == tofsL1M)) {
        FieldTimeTbl[3][tbofs] = ft;
        FieldTimeTbl[7][tbofs] = ft;
        for (i = 4; i < 7; i++)
            FieldTimeTbl[i][tbofs] = 0xFF;
        FieldTimeTbl[0][tbofs] = 0xFF;
        FieldTimeTbl[1][tbofs] = 0xFF;
        FieldTimeTbl[2][tbofs] = 0xFF;
    }
    else {
        for (i = 0; i < 8; i++)
            FieldTimeTbl[i][tbofs] = ft;
    }
}

void TToweredTrainUnit::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	FieldTimeTbl[8][tbofs] = ft = raw_field_time(f);
	if (((f->Unit != NO_UNIT) && (f->Unit != ID)) ||
	    (field_has_visible_mine(x, y, ID & BADLIFE) && (f->Unit != ID))) {
		for (i = 0; i < 9; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1B) ||
	         (L1TerrainType[ter] == tofsL1D)) {
		FieldTimeTbl[0][tbofs] = ft;
		FieldTimeTbl[4][tbofs] = ft;
		for (i = 1; i < 4; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		for (i = 5; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1C) ||
	         (L1TerrainType[ter] == tofsL1E)) {
		FieldTimeTbl[2][tbofs] = ft;
		FieldTimeTbl[6][tbofs] = ft;
		for (i = 3; i < 6; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else { //terA
		for (i = 0; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		switch (f->Terrain2) {
		case 71+1 :
			FieldTimeTbl[0][tbofs] = FieldTimeTbl[4][tbofs] = ft;
			break;
		case 71+2 :
			FieldTimeTbl[2][tbofs] = FieldTimeTbl[6][tbofs] = ft;
			break;
		case 71+16 :
			FieldTimeTbl[3][tbofs] = FieldTimeTbl[7][tbofs] = ft;
			break;
		case 71+19 :
			FieldTimeTbl[1][tbofs] = FieldTimeTbl[5][tbofs] = ft;
			break;
		default:
			for (i = 0; i < 8; i++)
				FieldTimeTbl[i][tbofs] = ft;
			break;
		}
	}
}


void TOlymp::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	FieldTimeTbl[8][tbofs] = ft = raw_field_time(f);
	if (((f->Unit != NO_UNIT) && (f->Unit != ID)) ||
	    (field_has_visible_mine(x, y, ID & BADLIFE) && (f->Unit != ID))) {
		for (i = 0; i < 9; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1B) ||
	         (L1TerrainType[ter] == tofsL1D)) {
		FieldTimeTbl[0][tbofs] = ft;
		FieldTimeTbl[4][tbofs] = ft;
		for (i = 1; i < 4; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		for (i = 5; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1C) ||
	         (L1TerrainType[ter] == tofsL1E)) {
		FieldTimeTbl[2][tbofs] = ft;
		FieldTimeTbl[6][tbofs] = ft;
		for (i = 3; i < 6; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else { //terA
		for (i = 0; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		switch (f->Terrain2) {
		case 71+1 :
			FieldTimeTbl[0][tbofs] = FieldTimeTbl[4][tbofs] = ft;
			break;
		case 71+2 :
			FieldTimeTbl[2][tbofs] = FieldTimeTbl[6][tbofs] = ft;
			break;
		case 71+16 :
			FieldTimeTbl[3][tbofs] = FieldTimeTbl[7][tbofs] = ft;
			break;
		case 71+19 :
			FieldTimeTbl[1][tbofs] = FieldTimeTbl[5][tbofs] = ft;
			break;
		default:
			for (i = 0; i < 8; i++)
				FieldTimeTbl[i][tbofs] = ft;
			break;
		}
	}
}


void TTrainSupportUnit::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	FieldTimeTbl[8][tbofs] = ft = raw_field_time(f);
	if (((f->Unit != NO_UNIT) && (f->Unit != ID)) ||
	    (field_has_visible_mine(x, y, ID & BADLIFE) && (f->Unit != ID))) {
		for (i = 0; i < 9; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1B) ||
	         (L1TerrainType[ter] == tofsL1D)) {
		FieldTimeTbl[0][tbofs] = ft;
		FieldTimeTbl[4][tbofs] = ft;
		for (i = 1; i < 4; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		for (i = 5; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1C) ||
	         (L1TerrainType[ter] == tofsL1E)) {
		FieldTimeTbl[2][tbofs] = ft;
		FieldTimeTbl[6][tbofs] = ft;
		for (i = 3; i < 6; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else { //terA
		for (i = 0; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		switch (f->Terrain2) {
		case 71+1 :
			FieldTimeTbl[0][tbofs] = FieldTimeTbl[4][tbofs] = ft;
			break;
		case 71+2 :
			FieldTimeTbl[2][tbofs] = FieldTimeTbl[6][tbofs] = ft;
			break;
		case 71+16 :
			FieldTimeTbl[3][tbofs] = FieldTimeTbl[7][tbofs] = ft;
			break;
		case 71+19 :
			FieldTimeTbl[1][tbofs] = FieldTimeTbl[5][tbofs] = ft;
			break;
		default:
			for (i = 0; i < 8; i++)
				FieldTimeTbl[i][tbofs] = ft;
			break;
		}
	}
}





void TXenon::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	FieldTimeTbl[8][tbofs] = ft = raw_field_time(f);
	if (((f->Unit != NO_UNIT) && (f->Unit != ID))) {
		for (i = 0; i < 9; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1B) ||
	         (L1TerrainType[ter] == tofsL1D)) {
		FieldTimeTbl[0][tbofs] = ft;
		FieldTimeTbl[4][tbofs] = ft;
		for (i = 1; i < 4; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		for (i = 5; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1C) ||
	         (L1TerrainType[ter] == tofsL1E)) {
		FieldTimeTbl[2][tbofs] = ft;
		FieldTimeTbl[6][tbofs] = ft;
		for (i = 3; i < 6; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1H) ||
	         (L1TerrainType[ter] == tofsL1F) ||
	         (L1TerrainType[ter] == tofsL1L) ||
	         (L1TerrainType[ter] == tofsL1J)) {
		FieldTimeTbl[1][tbofs] = ft;
		FieldTimeTbl[5][tbofs] = ft;
		for (i = 2; i < 5; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[6][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1I) ||
	         (L1TerrainType[ter] == tofsL1G) ||
	         (L1TerrainType[ter] == tofsL1K) ||
	         (L1TerrainType[ter] == tofsL1M)) {
		FieldTimeTbl[3][tbofs] = ft;
		FieldTimeTbl[7][tbofs] = ft;
		for (i = 4; i < 7; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[2][tbofs] = 0xFF;
	}
	else {
		for (i = 0; i < 8; i++)
			FieldTimeTbl[i][tbofs] = ft;
	}
}



void TAircraft::PrepareFieldTime(int x, int y)
{
	word ft;
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ft = raw_field_time(GetField(x, y));
	if (((x != X) || (y != Y)) && (GetAircraftAt(x, y) != NULL)) ft = 0xFF;
	for (i = 0; i < 9; i++)	FieldTimeTbl[i][tbofs] = ft;
}










//// lode:

static word ShipFieldTime(int x, int y, int ID, int Ctr = FALSE)
{
	word ft, ter;
	TField *f = GetField(x, y);
//	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	if ((x < 0) || (y < 0) || (x >= MapSizeX) || (y >= MapSizeY)) return 0xFF;
	ter = f->Terrain;
	ft = raw_field_time(f);
	if (((f->Unit != NO_UNIT) && (f->Unit != ID))) return 0xFF;
	if ((Ctr) && (ter != 18 /*hluboka voda*/)) return 0xFF;
	else return ft;
}


static int ship_Xdir[8] = {  0, -1, -1, -1,  0,  1,  1,  1};
static int ship_Ydir[8] = { -1, -1,  0,  1,  1,  1,  0, -1};

static word Ship3_FT(int x, int y, int ID, int Orient)
{
	int ft1;

	ft1 = ShipFieldTime(x, y, ID, TRUE);
	if (ft1 == 0xFF) return 0xFF;
	if (ShipFieldTime(x + ship_Xdir[Orient], y + ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (ShipFieldTime(x - ship_Xdir[Orient], y - ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (Orient % 2 == 1) {
		if (ShipFieldTime(x-1, y, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x+1, y, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x, y-1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x, y+1, ID) == 0xFF) return 0xFF;
	}
	return ft1;
}



void TPoseidon::PrepareFieldTime(int x, int y) {
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	FieldTimeTbl[8][tbofs] = raw_field_time(GetField(x, y));

	for (i = 0; i < 8; i++)	{
		FieldTimeTbl[i][tbofs] = Ship3_FT(x, y, this->ID, i);
//	 	if (FieldTimeTbl[i][tbofs] != 0xFF)
//			FieldTimeTbl[i][tbofs] = Ship3_FT(x - ship_Xdir[i], y - ship_Ydir[i], ID, i);
	}
}




static word Ship5_FT(int x, int y, int ID, int Orient)
{
	int ft1, xX, yY;

	ft1 = ShipFieldTime(x, y, ID, TRUE);
	if (ft1 == 0xFF) return 0xFF;
	if (ShipFieldTime(x + ship_Xdir[Orient], y + ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (ShipFieldTime(x - ship_Xdir[Orient], y - ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (ShipFieldTime(x + 2*ship_Xdir[Orient], y + 2*ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (ShipFieldTime(x - 2*ship_Xdir[Orient], y - 2*ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (Orient % 2 == 1) {
		if (ShipFieldTime(x-1, y, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x+1, y, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x, y-1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x, y+1, ID) == 0xFF) return 0xFF;

		xX = x + ship_Xdir[Orient],	yY = y + ship_Ydir[Orient];
		if (ShipFieldTime(xX-2, yY-1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX+2, yY+1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX-1, yY-2, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX+1, yY+2, ID) == 0xFF) return 0xFF;
		xX = x - ship_Xdir[Orient],	yY = y - ship_Ydir[Orient];
		if (ShipFieldTime(xX-2, yY-1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX+2, yY+1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX-1, yY-2, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX+1, yY+2, ID) == 0xFF) return 0xFF;
	}
	return ft1;
}




void TKraken::PrepareFieldTime(int x, int y) {
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	FieldTimeTbl[8][tbofs] = raw_field_time(GetField(x, y));

	for (i = 0; i < 8; i++)	{
		FieldTimeTbl[i][tbofs] = Ship5_FT(x, y, this->ID, i);
//	 	if (FieldTimeTbl[i][tbofs] != 0xFF)
//			FieldTimeTbl[i][tbofs] = Ship5_FT(x - ship_Xdir[i], y - ship_Ydir[i], ID, i);
	}
}




void TLaguna::PrepareFieldTime(int x, int y) {
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	FieldTimeTbl[8][tbofs] = raw_field_time(GetField(x, y));

	for (i = 0; i < 8; i++)	{
		FieldTimeTbl[i][tbofs] = Ship5_FT(x, y, this->ID, i);
//	 	if (FieldTimeTbl[i][tbofs] != 0xFF)
//			FieldTimeTbl[i][tbofs] = Ship5_FT(x - ship_Xdir[i], y - ship_Ydir[i], ID, i);
	}
}


void ApplyNoAccess()
{
    byte ft;
    TField *f;
    int x, y;

    for (x = MTrect.x1; x <= MTrect.x2; x++)
        for (y = MTrect.y1; y <= MTrect.y2; y++) {
            f = GetField(x, y);
            if ((ft = Terr2Tbl[f->Terrain2]) == 0) ft = TerrTbl[f->Terrain];
            if ((TTget(x, y) == 0xFFFF) && (ft != 0xFF)) 
                TTset(x, y, 0xFFFE);
        }
}




// rekurze vyplneni policka:

#define PROPAG_DELTA  200

typedef struct {
            int x, y;
            double time;
            int direc;          
    } TPropagItem;


TPropagItem *PropagList;
int PropagSize, PropagLast;
int PropagMaxTime;

inline void AddPropag(int x, int y, double time, int direc)
{
    TPropagItem *p;

    if ((x < MTrect.x1) || (y < MTrect.y1) || 
        (x > MTrect.x2) || (y > MTrect.y2)) return;
    byte ft = FieldTimeTbl[direc][(x - MTofsX) + (y - MTofsY) * MTsizeX];
    if (ft == 0xFF) return;
    double t = time + DirecModif[direc] * ft;
    word tim = t; /* zaokrouhleno smerem dolu */
    if (tim > PropagMaxTime) return;    
    if (tim > TTget(x, y)) return;
    TDTset(x, y, tim, direc); 

    if ((++PropagLast) == PropagSize) {
        PropagSize += PROPAG_DELTA;
        PropagList = (TPropagItem *) memrealloc(PropagList, PropagSize * sizeof(TPropagItem));
    }
    p = &(PropagList[PropagLast]);
    p->x = x;
    p->y = y;
    p->time = t;
    p->direc = direc;
}

inline void DelPropag(int order)
{
    if (order < PropagLast)
        memmove(&PropagList[order], &PropagList[order+1], 
                sizeof(TPropagItem) * (PropagLast - order));
    PropagLast--;
}



void Propagation(int x, int y, double time, byte direc)
{
    int tblofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

    if (FieldTimeTbl[0][tblofs] != 0xFF) AddPropag(x  , y-1, time, 0);
    if (FieldTimeTbl[1][tblofs] != 0xFF) AddPropag(x-1, y-1, time, 1);
    if (FieldTimeTbl[2][tblofs] != 0xFF) AddPropag(x-1, y  , time, 2);
    if (FieldTimeTbl[3][tblofs] != 0xFF) AddPropag(x-1, y+1, time, 3);
    if (FieldTimeTbl[4][tblofs] != 0xFF) AddPropag(x  , y+1, time, 4);
    if (FieldTimeTbl[5][tblofs] != 0xFF) AddPropag(x+1, y+1, time, 5);
    if (FieldTimeTbl[6][tblofs] != 0xFF) AddPropag(x+1, y  , time, 6);
    if (FieldTimeTbl[7][tblofs] != 0xFF) AddPropag(x+1, y-1, time, 7);
}



void TObject::PrepareMoveTable()
{
    int i;
    TRect mapr = {0, 0, MapSizeX-1, MapSizeY-1};

    if (TimeTable != NULL) { memfree(TimeTable); memfree(DirTable);}
    TimeTable = NULL; DirTable = NULL;
    if (TimeUnits / 5 == 0) {return;}
    RemoveSubmarines(); 
    MTsize = (TimeUnits / 5) * 2 + 1;
    MTofs = TimeUnits / 5;
    MTrect.x1 = X - MTofs;
    MTrect.y1 = Y - MTofs;
    MTrect.x2 = MTrect.x1 + MTsize - 1;
    MTrect.y2 = MTrect.y1 + MTsize - 1;
    Intersect(&MTrect, &mapr);
    MTofsX = MTrect.x1;
    MTofsY = MTrect.y1; 
    MTsizeX = MTrect.x2 - MTofsX + 1;
    MTsizeY = MTrect.y2 - MTofsY + 1;

    TimeTable = (word *) memalloc(MTsizeX * MTsizeY * sizeof(word));
    DirTable = (byte *) memalloc(MTsizeX * MTsizeY);    
    memset(TimeTable, 0xFF, MTsizeX * MTsizeY * sizeof(word));
    memset(DirTable, 0, MTsizeX * MTsizeY);
    GetTerrMove(&TerrTbl, &Terr2Tbl);
    for (i = 0; i < 9; i++) FieldTimeTbl[i] = (byte *) memalloc(MTsizeX * MTsizeY);
    {
        int x, y;
        for (x = MTrect.x1; x <= MTrect.x2; x++)
            for (y = MTrect.y1; y <= MTrect.y2; y++)
                PrepareFieldTime(x, y);
    }

    PropagSize = PROPAG_DELTA;
    PropagLast = -1;
    PropagList = (TPropagItem *) memalloc(PropagSize * sizeof(TPropagItem));
    PropagMaxTime = TimeUnits;
    Propagation(X, Y, 0, 0);

    { // vlastni propagace:
      int i, nearest;
      double neartim;
      TPropagItem p;
      
      while (PropagLast > -1) {
          nearest = 0, neartim = PropagList[0].time;
          for (i = 1; i <= PropagLast; i++) {
            if (PropagList[i].time < neartim) {
                nearest = i;
                neartim = PropagList[i].time;
            }
        }
          p = PropagList[nearest];
          {
              DelPropag(nearest);
              for (i = 0; i <= PropagLast; i++) {
                if ((p.x == PropagList[i].x) && (p.y == PropagList[i].y))
                    DelPropag(i);
              }
            }
          Propagation(p.x, p.y, p.time, p.direc);
      }
    }
    TDTset(X, Y, 0, 0xFF);
    ApplyNoAccess();

    memfree(PropagList);
    for (i = 0; i < 9; i++) memfree(FieldTimeTbl[i]);

    PlaceSubmarines();  
}





///////////////////// fce na vypocet pohybu na velkou vzdalenost (pro AI)

#define MOVEFAR_DELTA  5

int FarTgtX, FarTgtY, FarFound = FALSE;

inline void AddFarPropag(int x, int y, double time, int direc)
{
    TPropagItem *p;

    if ((x < MTrect.x1) || (y < MTrect.y1) || 
        (x > MTrect.x2) || (y > MTrect.y2)) return;
    byte ft = FieldTimeTbl[direc][(x - MTofsX) + (y - MTofsY) * MTsizeX];
    if (ft == 0xFF) return;
    double t = time + DirecModif[direc] * ft;
    word tim = t; /* zaokrouhleno smerem dolu */
    if (tim > TTget(x, y)) return;
    TDTset(x, y, tim, direc); 
    if ((x == FarTgtX) && (y == FarTgtY)) FarFound = TRUE;

    if ((++PropagLast) == PropagSize) {
        PropagSize += PROPAG_DELTA;
        PropagList = (TPropagItem *) memrealloc(PropagList, PropagSize * sizeof(TPropagItem));
    }
    p = &(PropagList[PropagLast]);
    p->x = x;
    p->y = y;
    p->time = t;
    p->direc = direc;
}

void FarPropagation(int x, int y, double time, byte direc)
{
    int tblofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

    if (FieldTimeTbl[0][tblofs] != 0xFF) AddFarPropag(x  , y-1, time, 0);
    if (FieldTimeTbl[1][tblofs] != 0xFF) AddFarPropag(x-1, y-1, time, 1);
    if (FieldTimeTbl[2][tblofs] != 0xFF) AddFarPropag(x-1, y  , time, 2);
    if (FieldTimeTbl[3][tblofs] != 0xFF) AddFarPropag(x-1, y+1, time, 3);
    if (FieldTimeTbl[4][tblofs] != 0xFF) AddFarPropag(x  , y+1, time, 4);
    if (FieldTimeTbl[5][tblofs] != 0xFF) AddFarPropag(x+1, y+1, time, 5);
    if (FieldTimeTbl[6][tblofs] != 0xFF) AddFarPropag(x+1, y  , time, 6);
    if (FieldTimeTbl[7][tblofs] != 0xFF) AddFarPropag(x+1, y-1, time, 7);
}


int TUnit::MoveFarSUB(int x, int y)
{
    int i;
    TRect mapr = {0, 0, MapSizeX-1, MapSizeY-1};
    byte dir;
    int oldtime;
    int ReturnValue = TRUE;
    int tries = 0;

    FarTgtX = x, FarTgtY = y;

    if ((x == X) && (y == Y)) return TRUE;

    RemoveSubmarines();
    // cast opisujici PrepareMoveTable()

    if (x < X) MTrect.x1 = x, MTrect.x2 = X;
    else MTrect.x1 = X, MTrect.x2 = x;
    if (y < Y) MTrect.y1 = y, MTrect.y2 = Y;
    else MTrect.y1 = Y, MTrect.y2 = y;

    FarFound = FALSE;
    for (i = 0; i < 9; i++) FieldTimeTbl[i] = NULL;
    do {
        if (TimeTable != NULL) { memfree(TimeTable); memfree(DirTable);}
        for (i = 0; i < 9; i++) if (FieldTimeTbl[i]) memfree(FieldTimeTbl[i]);
        TimeTable = NULL; DirTable = NULL;
        MTrect.x1 -= MOVEFAR_DELTA, MTrect.y1 -= MOVEFAR_DELTA;
        MTrect.x2 += MOVEFAR_DELTA, MTrect.y2 += MOVEFAR_DELTA;
        if (tries) MTrect = mapr; // jdeme na sneka

        Intersect(&MTrect, &mapr);
        MTofsX = MTrect.x1;
        MTofsY = MTrect.y1; 
        MTsizeX = MTrect.x2 - MTofsX + 1;
        MTsizeY = MTrect.y2 - MTofsY + 1;

        TimeTable = (word *) memalloc(MTsizeX * MTsizeY * sizeof(word));
        DirTable = (byte *) memalloc(MTsizeX * MTsizeY);    
        memset(TimeTable, 0xFF, MTsizeX * MTsizeY * sizeof(word));
        memset(DirTable, 0, MTsizeX * MTsizeY);
        GetTerrMove(&TerrTbl, &Terr2Tbl);
        for (i = 0; i < 9; i++) FieldTimeTbl[i] = (byte *) memalloc(MTsizeX * MTsizeY);
        {
            int x, y;
            for (x = MTrect.x1; x <= MTrect.x2; x++)
                for (y = MTrect.y1; y <= MTrect.y2; y++)
                    PrepareFieldTime(x, y);
        }
    
        PropagSize = PROPAG_DELTA;
        PropagLast = -1;
        PropagList = (TPropagItem *) memalloc(PropagSize * sizeof(TPropagItem));
        FarPropagation(X, Y, 0, 0);
    
        { // vlastni propagace:
          int i, nearest;
          double neartim;
          TPropagItem p;
          
          while (PropagLast > -1) {
              nearest = 0, neartim = PropagList[0].time;
              for (i = 1; i <= PropagLast; i++) {
                if (PropagList[i].time < neartim) {
                    nearest = i;
                    neartim = PropagList[i].time;
                }
            }
              p = PropagList[nearest];
              {
                  DelPropag(nearest);
                  for (i = 0; i <= PropagLast; i++) {
                    if ((p.x == PropagList[i].x) && (p.y == PropagList[i].y))
                        DelPropag(i);
                  }
                }
              FarPropagation(p.x, p.y, p.time, p.direc);
          }
        }

        if (tries && (!FarFound)) {  // neni jina moznost nez jet na nejake
                                     // policko v blizkem okoli
            TPoint al;
            int i;
            
            for (i = 0; ; i++) {
                al = CartezianSnail(i); al.x += x, al.y += y;
                if ((al.x < 0) || (al.y < 0) || (al.x >= MapSizeX) || (al.y >= MapSizeY)) continue;
                if ((al.x == X) && (al.y == Y)) {PlaceSubmarines(); return -2;}
                if (TTget(al.x, al.y) < 0xFFF0) {
                    FarFound = TRUE;
                    x = al.x, y = al.y; // prepis cile
                    break;
                }
            }
        }
        
        memfree(PropagList);
        tries++;
    } while (!(
        FarFound /*nalezena cesta*/ ||
      ((MTsizeX == MapSizeX) && (MTsizeY == MapSizeY)) /*rozsir pruzkum*/));
    TDTset(X, Y, 0, 0xFF);
    ApplyNoAccess();

    for (i = 0; i < 9; i++) memfree(FieldTimeTbl[i]);
    if (TimeToMove(x, y) <= 0) {PlaceSubmarines(); return -2;}


    // sekce vlastniho pohybu:

    MoveRel_Counter = 0;
    oldtime = TimeUnits;
    FindWay(x, y, 128/*magicnumber*/);
    SmokeOn = FALSE;
    HideHelpers();
    SampleMoving = INVALID_SAMPLE_HANDLE;
    attack_done = FALSE;
    while ((X != x) || (Y != y)) {
        if ((--Fuel) <= 0) {
            Fuel = 0;
            ReturnValue = -3;
            goto ending_label;
        };
        if ((oldtime - TimeToMove(X, Y)) <= 0) {ReturnValue = -1; goto ending_label;}
        TimeUnits = oldtime - TimeToMove(X, Y);
        dir = DTget(X, Y);
        if      (dir == 100) {Rotate(7); MoveRel( 0, -1);}
        else if (dir == 101) {Rotate(0); MoveRel(-1, -1);}
        else if (dir == 102) {Rotate(1); MoveRel(-1,  0);}
        else if (dir == 103) {Rotate(2); MoveRel(-1, +1);}
        else if (dir == 104) {Rotate(3); MoveRel( 0, +1);}
        else if (dir == 105) {Rotate(4); MoveRel(+1, +1);}
        else if (dir == 106) {Rotate(5); MoveRel(+1,  0);}
        else if (dir == 107) {Rotate(6); MoveRel(+1, -1);}
        GoOnField();
        if (attack_done) {
            DoExplosion();
            break;
        }
        if (flagUnitOnMine) {
            GoOnMine();
            flagUnitOnMine = FALSE;
            DoExplosion();
            break;
        }
    }
ending_label: // ja vim, prasarna
    PlaceSubmarines();
    FadeOutSample(SampleMoving, EffectsVolume);
    StopSample(SampleMoving);   SampleMoving = INVALID_SAMPLE_HANDLE;
    ShowHelpers();
    if (!attack_done) {
        DoVisib();
//  RedrawMap();
        if (this == SelectedUnit) Select();
    }
    UpdateLitMap(TRUE);
    MouseFreeze(LITMAP_X, LITMAP_Y, LITMAP_SIZE, LITMAP_SIZE);
    DrawLitMap();
    MouseUnfreeze();
    UpdateScreen();
    return ReturnValue;
}











///////////////////////// INIT & DONE ROUTINES //////////////////////////////

int *load_sprite_locator(TDataFile *df, const char *name) {
	void *data;
	int i, *ret = NULL;

	data = df->get(name);

	if (data) {
		MemoryReadStream stream(data, 64);

		ret = (int*)memalloc(16 * sizeof(int));

		for (i = 0; i < 16; i++) {
			ret[i] = stream.readSint32LE();
		}

		memfree(data);
	}

	return ret;
}

int InitUnits() {
	char name[32];

	LoadArray(BmpSelected, 13, GraphicsDF, "terpul%i");
#ifdef DEBUG
	SpriteUniversal = load_sprite(GraphicsDF, "sprite");
	SpriteUniversalBad = load_sprite(GraphicsDF, "sprite2");
#endif
	GoodlifeDeads = BadlifeDeads = 0;
	UInfoBuf = memalloc(UINFO_SX * UINFO_SY);
	UInfoBkgr = GraphicsDF->get("infobk");
	LoadArray(LevelBmps, 10, GraphicsDF, "level%i");
	LoadArray((void**)WpnNames, WPNNM_COUNT, TextsDF, "wpnnm%i");
	LoadArray((void**)WpnInfoBkg, 2, GraphicsDF, "wpninfo%i");
	LoadArray((void**)BmpAmmoIcons, 4, GraphicsDF, "bmpammo%i");
	InfoIcon = new TIcon(RES_X-56, UINFO_Y+186, 44, 20, "icon10_%i", SHORTCUT_INFO);
	SelectSnd = LoadSample("select", FALSE);
	WpnSelectSnd = LoadSample("wpnclick", FALSE);
	ZavoraSnd = LoadSample("zavora", FALSE);
	IconTransport = new TIcon(RES_X-115, UINFO_Y+110, 102, 23, "tranbut%i", 13);
	LoadArray((void**)UnitsNames, GOODLIFE_TOP, TextsDF, "unam%i");
	LoadArray((void**)UnitsTransIcons, GOODLIFE_TOP, GraphicsDF, "un%iicn");
	LoadArray((void**)BmpRepair, 2, GraphicsI18nDF, "repair%i");
	LoadArray((void**)BmpBombing, 2, GraphicsDF, "icbomb%i");
	LoadSpriteArray(Smokes, 32, GraphicsDF, "dym0_%i");

	// NB: "sprlc%i.mem" files from signus-data were created in such way that
	//     locators for enemy units has numbers starting from 128. This dates
	//     back to when BADLIFE was 128. Since BADLIFE is now 1024, we have to
	//     move the data to higher offset in the array.
	// (FIXME?)
	memset(SpriteLocators, 0, sizeof(SpriteLocators));

	for (int i = 0; i < 128; i++) {
		sprintf(name, "sprlc%i", i);
		SpriteLocators[i] = load_sprite_locator(GraphicsDF, name);
		sprintf(name, "sprlc%i", i + 128);
		SpriteLocators[i + BADLIFE] = load_sprite_locator(GraphicsDF,
			name);
	}

	if (!InitWeapons()) {
		return FALSE;
	}

	for (int i = 0; i < GOODLIFE_TOP; i++) {
		SelectionHistory[i] = i;
	}

	return InitExplode();
}



int DoneUnits()
{
    DoneExplode();
    DoneWeapons();
    if (BadlifeVisib) {memfree(BadlifeVisib); BadlifeVisib = NULL;}
    DisposeArray(BmpSelected, 13);
#ifdef DEBUG
    memfree(SpriteUniversal);
    memfree(SpriteUniversalBad);
#endif
    memfree(UInfoBuf); memfree(UInfoBkgr);
    if (BmpBigInventory) {memfree(BmpBigInventory); BmpBigInventory = NULL;}
    if (BmpSmallInventory) {memfree(BmpSmallInventory); BmpSmallInventory = NULL;}
    if (BmpMediumInventory) {memfree(BmpMediumInventory); BmpMediumInventory = NULL;}
    DisposeArray((void**)Smokes, 32);
    DisposeArray(LevelBmps, 10);
    DisposeArray((void**)WpnNames, WPNNM_COUNT);
    DisposeArray((void**)WpnInfoBkg, 2);
    DisposeArray((void**)BmpAmmoIcons, 4);  
    DisposeArray((void**)BmpRepair, 2);
    DisposeArray((void**)BmpBombing, 2);
    delete InfoIcon;
    if (IconThorOut != NULL) {
        delete IconThorOut;
        delete IconThorIn;
        IconThorIn = IconThorOut = NULL;
    }
    if (IconLand != NULL) {
        delete IconLand;
        delete IconTakeoff;
        IconLand = IconTakeoff = NULL;
    }
    if (IconSatan != NULL) {delete IconSatan; IconSatan = NULL;}
    if (IconXenon != NULL) {delete IconXenon; IconXenon = NULL;}
    if (IconXenon2 != NULL) {delete IconXenon2; IconXenon2 = NULL;}
    if (IconTransport != NULL) {delete IconTransport; IconTransport = NULL;}
    FreeSample(SelectSnd);
    FreeSample(WpnSelectSnd);
    FreeSample(ZavoraSnd);
    DisposeArray((void**)SpriteLocators, UNITS_TOP);
    DisposeArray((void**)UnitsNames, GOODLIFE_TOP);
    DisposeArray((void**)UnitsTransIcons, GOODLIFE_TOP);
    
    for (int i = 0; i < UNITS_TOP; i++)
        if (Units[i] != NULL) {
            delete Units[i];
            Units[i] = NULL;
        }

    if (TimeTable != NULL) memfree(TimeTable);
    if (DirTable != NULL) memfree(DirTable);
    
    // obnoveni default hodnot promnenych:
    SelectedUnit = NULL;
    memset(Units, 0, sizeof(Units[0]) * UNITS_TOP);
    TimeTable = NULL; DirTable = NULL;
    DoneAutofire();

    return TRUE;
}

////////////////////////// READ & WRITE FUNCS ////////////////////

TObject *createUnit(int type) {
	TObject *obj = NULL;

	switch (type) {
// FIXME: copy-paste iounreg.h contents here and delete the file
#include "iounreg.h"
#ifdef DEBUG
	default:
		PromtBox("ERROR READING UNITS", cmOk);
		break;
#endif
	}

	return obj;
}

void ReadUnits(ReadStream &stream) {
	int dummy;
	int typ, x, y, party = 0, i;
	TObject *obj;
	// Create just the memory space, real TObject instance would call
	// destructor at the end of this function and crash the game
	char fakeunit[sizeof(TObject)] = {0};

	while ((typ = stream.readSint32LE())) {
		if (typ == -1) {
			// Dead unit. Just fill the first empty slot with
			// a fake unit to preserve IDs and move on.
			// Note that this will break if Units[BADLIFE] gets
			// destroyed.
			for (i = party; i < UNITS_TOP && Units[i]; i++);

			if (i < UNITS_TOP) {
				Units[i] = (TObject*)fakeunit;
			}

			continue;
		}

		UpdateLoading();
		party = stream.readSint32LE();
		x = stream.readSint32LE();
		y = stream.readSint32LE();
		obj = createUnit(typ);

		if (obj) {
			obj->Init(x, y, party, &stream);
		}
	}

	// Remove fake units from unit list
	for (i = 0; i < UNITS_TOP; i++) {
	        if (Units[i] == (TObject*)fakeunit) {
			Units[i] = NULL;
		}
	}

	dummy = stream.readSint32LE();
	SelectedUnit = Units[dummy];

	ComputeVisib();
	InitAutofire();
}

void WriteUnits(WriteStream &stream) {
	int i;
	TObject *o;

	// save units to file
	for (i = 0; i < UNITS_TOP; i++) {
		if (!Units[i]) {
			stream.writeSint32LE(-1);
			continue;
		}

		o = Units[i];

		if (o->ID < BADLIFE) {
			stream.writeSint32LE(o->Type);
			stream.writeSint32LE(GOODLIFE);
		} else {
			stream.writeSint32LE(o->Type - BADLIFE);
			stream.writeSint32LE(BADLIFE);
		}

		stream.writeSint32LE(o->X);
		stream.writeSint32LE(o->Y);
		o->WriteInitReq(stream);
		o->Write(stream);
	}

	stream.writeSint32LE(0);
	stream.writeSint32LE(SelectedUnit->ID);
}



///////////////////////////// MEMORY MANAGING ////////////////////////

int UnitsCounts[UNITS_TOP] = {0};
TSprite **UnitsSprites[UNITS_TOP] = {NULL};
void *UnitsTransIcons[GOODLIFE_TOP] = {NULL};
void *UnitsPictures[GOODLIFE_TOP] = {NULL};
char *UnitsNames[UNITS_TOP] = {NULL};
char *UnitsDescripts[GOODLIFE_TOP] = {NULL};




// provede zkraceni tabulek se sprity - setri pamet:
void OptiSpriteTbls(int un)
{
    int max;
    
    for (max = 0; (max < UNITS_TOP) && UnitsSprites[un][max]; max++) {}
    max++;
    UnitsSprites[un] = (TSprite**) memrealloc((void*)UnitsSprites[un], max * sizeof(TSprite*));
    UnitsSprites[un][max-1] = NULL;
}

int GetSpriteTblSize(int un)
{
    int i;
    for (i = 0; UnitsSprites[un][i]; i++) {}
    return i;
}

void UpdateUnitsMem(int UnType) {
	int i = UnType, j;
	char ds[29];

	// Uvolneni spritu z pameti pri zmizeni posledni jednotky typu:
	if ((UnitsCounts[i] == 0) && (UnitsSprites[i] != NULL)) {
		DisposeArray((void **)UnitsSprites[i], GetSpriteTblSize(i));
		memfree(UnitsSprites[i]);
		UnitsSprites[i] = NULL;

		if (i < BADLIFE) {
			for (j = 0; j < 16; j++) {
				FreeSample(UnitsSoundSamples[i][j]);
			}
		} else {
			for (j = 0; j < 16; j++) {
				FreeSample(UnitsSoundSamples[i-BADLIFE][j]);
			}
		}

		if (i < BADLIFE) {
			memfree(UnitsDescripts[i]);
			UnitsDescripts[i] = NULL;

			if (UnitsPictures[i] != NULL) {
				memfree(UnitsPictures[i]);
				UnitsPictures[i] = NULL;
			}
		}
	// Nacteni nove jednotky - nacte jmena, popisy, alokuje potrebne tabulky
	} else if ((UnitsCounts[i] != 0) && (UnitsSprites[i] == NULL)) {
		UnitsSprites[i] = (TSprite**)memalloc(256 * sizeof(TSprite*));

		if (i < BADLIFE) {
			sprintf(ds, "un%i_%%i", i);
		} else {
			sprintf(ds, "un%i_%%i", i - BADLIFE + 128);
		}

		LoadSpriteArray(UnitsSprites[i], 256, GraphicsDF, ds);

#ifdef DEBUG
		if (UnitsSprites[i][0] == NULL) {
			UnitsSprites[i][0] = (TSprite*)memalloc(SpriteUniversal->w * SpriteUniversal->h + 16);

			if (i < BADLIFE) {
				memcpy(UnitsSprites[i][0], SpriteUniversal, SpriteUniversal->w * SpriteUniversal->h + 16);
			} else {
				memcpy(UnitsSprites[i][0], SpriteUniversalBad, SpriteUniversalBad->w * SpriteUniversalBad->h + 16);
			}

			TSprite *s = UnitsSprites[i][0];
			sprintf(ds,"%i", i);
			PutStr(s->data, s->w, s->h, 0, 0, ds, NormalFont,
				clrWhite, clrBlack);

			for (int j = 1; j < 48; j++) {
				UnitsSprites[i][j] = (TSprite*)memalloc(s->w * s->h + 16);
				memcpy(UnitsSprites[i][j], s, s->w * s->h + 16);
				sprintf(ds,"%i", j);
				PutStr(UnitsSprites[i][j]->data,
					UnitsSprites[i][j]->w,
					UnitsSprites[i][j]->h, 10, 10, ds,
					NormalFont, clrWhite, clrBlack);
			}

			PutStr(s->data, s->w, s->h, 10, 10, "0", NormalFont,
				clrWhite, clrBlack);
		}
#endif

		OptiSpriteTbls(i);

		if (i < BADLIFE) {
			for (j = 0; j < 2; j++) {
				UnitsSoundSamples[i][j] = LoadSample(UnitsSoundIndex[i][j], TRUE);
			}

			for (j = 2; j < 16; j++) {
				UnitsSoundSamples[i][j] = LoadSample(UnitsSoundIndex[i][j], FALSE);
			}

			sprintf(ds, "udes%i", i);
			UnitsDescripts[i] = (char *) TextsDF->get(ds);

			if (iniResolution != SVGA_640x480) {
				sprintf(ds, "un%ilit", i);
				UnitsPictures[i] = (char*)GraphicsDF->get(ds);
			}
		} else {
			for (j = 0; j < 2; j++) {
				UnitsSoundSamples[i-BADLIFE][j] = LoadSample(UnitsSoundIndex[i-BADLIFE][j], TRUE);
			}

			for (j = 2; j < 16; j++) {
				UnitsSoundSamples[i-BADLIFE][j] = LoadSample(UnitsSoundIndex[i-BADLIFE][j], FALSE);
			}
		}
	}
}





////////////////////////////// helpers:


void TObject::CreateRangeHelpers()
{
    if (ID < BADLIFE) {
        HideHelpers();
        ClearRanges();
        GenerateVisibRng(X, Y, Visib);
        ShowHelpers();
    }
    else if (ClearRanges()) RedrawMap();
}



void TUnit::CreateRangeHelpers()
{
    if (ID < BADLIFE) {
        HideHelpers();
        ClearRanges();
        if ((CurWpn != -1) && (Weapons[CurWpn]->TimeLost <= TimeUnits)) 
          GenerateShootRng(X, Y, Weapons[CurWpn]->MinRange, Weapons[CurWpn]->MaxRange);
        GenerateMoveRng(MTrect.x1, MTrect.y1, MTrect.x2, MTrect.y2, this);
        GenerateVisibRng(X, Y, Visib);
        ShowHelpers();
    }
    else if (ClearRanges()) RedrawMap();
}




static int HelpersLocks = 0;

void ShowHelpers()
{
    HelpersLocks--;
    if (HelpersLocks == 0) {
        UnlockSelectCross();
        StatusbarLock--;
        ShowRanges();
        SelFldLocks--;
        SelectField(Mouse.x, Mouse.y);
    }
}



void HideHelpers()
{
    if (HelpersLocks == 0) {
        LockSelectCross();
        StatusbarLock++;
        HideRanges();
        SelectField(0, 0);
        SelFldLocks++;
    }
    HelpersLocks++;
}

