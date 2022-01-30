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


/*

Implementace BUDOV

*/

#include <SDL_timer.h>

#include "building.h"
#include "untables.h"
#include "transbox.h"
#include "explode.h"
#include "ai.h"
#include "fields.h"
#include "mouse.h"



int MoneyGoodlife = 0, MoneyBadlife = 0;


//////////////////////////// TBuilding /////////////////////////////////////


void TBuilding::Init(int x, int y, int party, ReadStream *stream) {
	TObject::Init(x, y, party, stream);
	RepairingNow = FALSE;

	if (stream) {
		int moreinfo = stream->readSint32LE();

		if (moreinfo) {
			Read(*stream);
		}
	}
}

void TBuilding::WriteInitReq(WriteStream &stream) {
	stream.writeSint32LE(TRUE);
}

void TBuilding::Read(ReadStream &stream) {
	TObject::Read(stream);
	RepairingNow = stream.readSint32LE();
	MoneyIndex = stream.readSint32LE();
}

void TBuilding::Write(WriteStream &stream) {
	TObject::Write(stream);
	stream.writeSint32LE(RepairingNow);
	stream.writeSint32LE(MoneyIndex);
}

void TBuilding::PlaceGround(int place)
{
    int ax, ay;
    TField *f;
    
    for (ax = 0; ax < SizeX; ax++)
        for (ay = 0; ay < SizeY; ay++) {
            f = GetField(X+ax, Y+ay);
            if (place) {
                f->Unit = ID;
                Alt = f->Height;
                if (IsAnimated()) f->IsAnim = 1;
            }
            else {
                f->Unit = NO_UNIT;
                f->IsAnim = 0;
            }
        }
    GenerateUnitsList();
}



TSprite *TBuilding::GetSprite()
{
    int v = GetAllVisib();
    if (v == 2) return UnitsSprites[Type][0];
    else return UnitsSprites[Type][1];
}



extern int ProcessingShoot;

void TBuilding::Draw()
{
    static unsigned lastdrawid[UNITS_TOP] = {0};

    if ((!ProcessingShoot) && (!((NowPaintedUnitCoord.x - X) == SizeX-1 - (NowPaintedUnitCoord.y - Y)))) return;
        // zabezp. ze se to cele kresli jenom jednou na jeden pruchod paintUnits
    if (lastdrawid[ID] == DrawingID) return;
    lastdrawid[ID] = DrawingID;
    
    TField *f = GetField(X, Y);
    TSprite *s, *sst;
    int rrx = GetRelX(X), rry = GetRelY(Y);
    int drawx = 28 * (rrx - rry) + LittleX + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
    
    if ((!SelectCrossLock) && (SelectedUnit == this)) {
        void *oldb = MapBuf;
        int i, j;
        
        MapBuf = FullBuf;
        for (i = 0; i < SizeX; i++)
            for (j = 0; j < SizeY; j++) 
                if (GetField(X+i, Y+j)->OnScreen)
                    DrawL2Selector(drawx + 28*(i-j), 
                                   drawy + 14*(i+j) + 28, f->Terrain, BmpSelected);
        MapBuf = oldb;
  }

    s = GetSprite();
    DrawSprite(drawx, drawy, s);

    if (iniShowStatusbar && (!StatusbarLock)) {
        if ((sst = GetStatusBar()) != NULL) {
            DrawSprite(drawx, drawy, sst);
            memfree(sst);
        }
    }
    GetDrawRect(&(LD_rect[ID])); LD_used[ID] = TRUE;    
}



TSprite *TBuilding::GetStatusBar()
{
    TSprite *s = (TSprite*) memalloc(4 * sizeof(int) + 44 * 6);
    int i, sz, clr;

    memset(s, 1, 4 * sizeof(int) + 44 * 6);
    s->w = 44, s->h = 6;
    s->dx = 22, s->dy = 15;
    
    for (i = 0; i < 42; i++) 
        s->data[s->w * 1 + i + 1] = s->data[s->w * 2 + i + 1] = 
        s->data[s->w * 3 + i + 1] = s->data[s->w * 4 + i + 1] = 72; // dark green
    sz = 42 * HitPoints / MaxHitPoints;

    if (iniAltEnemyStatusBarColors && ID >= BADLIFE) {
        if (sz < 12) {
            clr = 117; /*purple*/
        } else {
            clr = 227; /*biege*/
        }
    } else {
        if (sz < 12) {
            clr = 10; /*red*/
        } else {
            clr = 59; /*light green*/
        }
    }

    for (i = 0; i < sz; i++) 
        s->data[s->w * 1 + i + 1] = s->data[s->w * 2 + i + 1] = 
        s->data[s->w * 3 + i + 1] = s->data[s->w * 4 + i + 1] = clr;
    return s;
}



void TBuilding::GetUnitInfo(bool alt_wpinfo) {
	char cbuf[30];
	int clr;

	TObject::GetUnitInfo(alt_wpinfo);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 2, GetName(), NormalFont,
		clrLightBlue, clrBlack);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 26, SigText[TXT_STATE],
		NormalFont, clrWhite, clrBlack);

	sprintf(cbuf, "%i %%", 100 * HitPoints / MaxHitPoints);
	clr = (100 * HitPoints < 20 * MaxHitPoints) ? clrRed : clrLightBlue2;
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 54, 28, 52, 13, clr,
		clrSeaBlue, (double)HitPoints / MaxHitPoints, cbuf);

	sprintf(cbuf, SigText[TXT_FINANCE], MoneyGoodlife);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 60, cbuf, NormalFont, clrWhite,
		clrBlack);

	if (HitPoints < MaxHitPoints) {
		if (RepairingNow) {
			CopyBmp(UInfoBuf, UINFO_SX, 90, 46, BmpRepair[1], 16,
				52);
		} else {
			CopyBmp(UInfoBuf, UINFO_SX, 90, 46, BmpRepair[0], 16,
				52);
		}
	}
}



int TBuilding::InfoEvent(TEvent *e)
{
    int rt = TObject::InfoEvent(e);
    
    if (!rt && (e->What == evMouseDown) && 
       (IsInRect(e->Mouse.Where.x - UINFO_X, e->Mouse.Where.y - UINFO_Y,
                 90, 46, 90+16, 46+52))) {
        if (HitPoints < MaxHitPoints) RepairBuilding(!RepairingNow);
        ShowUnitInfo();
        return TRUE;
    }
    return rt;  
}



void TBuilding::TurnReset()
{
    int money, i;
    
    TObject::TurnReset();
    if (RepairingNow && (HitPoints < MaxHitPoints)) { // oprava budovy
        money = GetMoney(ID);
        for (i = 0; i < 50; i++) { // oprav max 50 hp, cena 2cr/hp
            if (money <= 0) break;
            money -= 2;
            if (++HitPoints == MaxHitPoints) {
                RepairingNow = FALSE;
                break;
            };
        }
        SetMoney(ID, money);
    }
}



int TBuilding::GetAllVisib()
{
    int ax, ay;
    int vis, hvis = 0;

    for (ax = 0; ax < SizeX; ax++)
        for (ay = 0; ay < SizeY; ay++) {
            vis =   GetField(X+ax, Y+ay)->Visib;
            if (vis > hvis) hvis = vis;
        }
    return hvis;
}



unsigned TBuilding::GetAvailableActions()
{
    if (ID >= BADLIFE) {
        if (GetAllVisib() == 2) return uatAttack | uatOccupy;
        else return uatNothing;
    }
    else return uatSelect | uatAttackGood;
}



void TBuilding::Explode()
{
    int i, j, k;
    int drw = Units[ID]->IsPartOnScreen() && (GetAllVisib() == 2);
    
    if (drw) {
        for (i = 0; i < SizeX; i++)
            for (j = 0; j < SizeY; j++) {
                GetField(X+i, Y+j)->Unit = NO_UNIT;
                MakeCrater(X+i, Y+j, cratCrater);
                GetField(X+i, Y+j)->Unit = ID;
                for (k = 0; k < 2; k++)
                    AddExplode1x1(X+i, Y+j, 0, 
                            10 - 20 * frand(), 10 - 20 * frand());
            }
    }

    for (i = X-1; i < X + SizeX+1; i++)
        for (j = Y-1; j < Y + SizeY+1; j++) 
            if (GetField(i, j)->Unit != this->ID)
                WeaponAttack(i, j, wpnExplos, utBUILDING_BOOM_AN, utBUILDING_BOOM_BN);

    if (!drw)
        this->RemoveFromWorld();
}






//////// TMutableBuilding:

void TMutableBuilding::Init(int x, int y, int party, ReadStream *stream) {
	Mutation = 0;
	TObject::Init(x, y, party, stream); // Intentional!!!
	RepairingNow = FALSE;

	if (stream) {
		int moreinfo;

		Mutation = stream->readSint32LE();
		PlaceGround(FALSE);
		Setup(); // once more - update!!!
		AfterSetup();
		PlaceGround(TRUE);
		moreinfo = stream->readSint32LE();

		if (moreinfo) {
			Read(*stream);
		}
	}
}

void TMutableBuilding::WriteInitReq(WriteStream &stream) {
	stream.writeSint32LE(Mutation);
	stream.writeSint32LE(TRUE);
}

TSprite *TMutableBuilding::GetSprite()
{
    int v = GetAllVisib();
    if (v == 2) return UnitsSprites[Type][Mutation];
    else return UnitsSprites[Type][8+Mutation];
}















//////////////////////////// TRadar

void TRadar::Setup()
{
    TBuilding::Setup();
    MaxHitPoints = utRD_HP;
    Visib = utRD_VIS;
    Defense[0] = utRD_DN0, Defense[1] = utRD_DN1, Defense[2] = utRD_DN2;
    SizeX = 1, SizeY = 1;
    AnimPhase = 0;
    MoneyIndex = utRD_MONEY;
}



int TRadar::DoVisib()
{
    return ProcessRadarVisib(X, Y, Visib, ID);
}



void TRadar::DoAnim()
{
    if (++AnimPhase == 16) AnimPhase = 0;
    PaintUnit(FALSE);
}



TSprite *TRadar::GetSprite()
{
    int v = GetAllVisib();
    if (v == 2) return UnitsSprites[Type][(AnimPhase == 0) ? 0 : (AnimPhase+1)];
    else return UnitsSprites[Type][1];
}











//////////////////////////// TPowerplant

void TPowerplant::Setup()
{
    TMutableBuilding::Setup();
    MaxHitPoints = utPP_HP;
    Visib = utPP_VIS;
    Defense[0] = utPP_DN0, Defense[1] = utPP_DN1, Defense[2] = utPP_DN2;
    SizeX = 1, SizeY = 1;
    MoneyIndex = utPP_MONEY;
}



void TPowerplant::DoAnim()
{
    if (GetAllVisib() != 2) return;
    if (++Mutation == 7) Mutation = 0;
    PaintUnit(FALSE);
}



unsigned TPowerplant::GetAvailableActions()
{
    if (ID >= BADLIFE) {
        if (GetAllVisib() == 2) return uatAttack;
        else return uatNothing;
    }
    else return uatSelect | uatAttackGood;
}





//////////// cecenci:

void THouse::Setup()
{
    TMutableBuilding::Setup();
    MaxHitPoints = utH_HP;
    Visib = utH_VIS;
    Defense[0] = utH_DN0, Defense[1] = utH_DN1, Defense[2] = utH_DN2;
    switch (Mutation) {
        case 0 :
            SizeX = 2, SizeY = 3;
            break;
        case 1 :
            SizeX = 3, SizeY = 2;
            break;
        case 2 : case 3 : case 5 : case 6 :
            SizeX = 2, SizeY = 2;
            break;
        case 4 :
            SizeX = 3, SizeY = 3;
            break;
    }
    MoneyIndex = 0;
}



unsigned THouse::GetAvailableActions()
{
    if (ID >= BADLIFE) {
        if (GetAllVisib() == 2) return uatAttack;
        else return uatNothing;
    }
    else return uatSelect | uatAttackGood;
}




//////////// artefakti:

void TArtefactHouse::Setup()
{
    TMutableBuilding::Setup();
    MaxHitPoints = utAH_HP;
    Visib = utAH_VIS;
    Defense[0] = utAH_DN0, Defense[1] = utAH_DN1, Defense[2] = utAH_DN2;
    
    switch (Mutation) {
        case 0 : case 1 : 
            SizeX = 1, SizeY = 1;
            break;
        case 2 : case 3 : case 4 : case 5 :
            SizeX = SizeY = 3;
            break;
        case 6 : 
            SizeX = SizeY = 7;
            break;
        case 7 :
            SizeX = 8; SizeY = 12;
            break;
    }
    MoneyIndex = 0;
}



unsigned TArtefactHouse::GetAvailableActions()
{
    if (ID >= BADLIFE) {
        if (GetAllVisib() == 2) return uatAttack;
        else return uatNothing;
    }
    else return uatSelect | uatAttackGood;
}



void TArtefactHouse::Draw()
{
    static unsigned lastdrawid[UNITS_TOP] = {0};

    if ((!ProcessingShoot) && (!((NowPaintedUnitCoord.x - X) == SizeX-1 - (NowPaintedUnitCoord.y - Y)))) return;
        // zabezp. ze se to cele kresli jenom jednou na jeden pruchod paintUnits
    if (lastdrawid[ID] == DrawingID) return;
    lastdrawid[ID] = DrawingID;
    
    TField *f = GetField(X, Y);
    TSprite *s;
    int rrx = GetRelX(X), rry = GetRelY(Y);
    int drawx = 28 * (rrx - rry) + LittleX + 28;
    int drawy = 14 * (rrx + rry - (f->Height)) + LittleY + 14;
    
    if ((!SelectCrossLock) && (SelectedUnit == this)) {
        void *oldb = MapBuf;
        int i, j;
        
        MapBuf = FullBuf;
        for (i = 0; i < SizeX; i++)
            for (j = 0; j < SizeY; j++) 
                if (GetField(X+i, Y+j)->OnScreen)
                    DrawL2Selector(drawx + 28*(i-j), 
                                   drawy + 14*(i+j) + 28, f->Terrain, BmpSelected);
        MapBuf = oldb;
  }

    s = GetSprite();    
    if (Mutation == 7) {
        DrawSprite(drawx, drawy, UnitsSprites[Type][Mutation]);
        DrawSprite(drawx, drawy, UnitsSprites[Type][Mutation+8]);
    }
    else
        DrawSprite(drawx, drawy, s);

    GetDrawRect(&(LD_rect[ID])); LD_used[ID] = TRUE;    
}








/////////////////////////////// TBase /////////////////////////////////////

void TBase::Setup()
{
    TBuilding::Setup();
    MaxHitPoints = utBS_HP;
    Visib = utBS_VIS;
    Defense[0] = utBS_DN0, Defense[1] = utBS_DN1, Defense[2] = utBS_DN2;
    SizeX = 6, SizeY = 6;
    LoadedUnits = 0;
    MoneyIndex = utBS_MONEY;
    Capacity = BASE_CAPACITY;
}



void TBase::Init(int x, int y, int party, ReadStream *stream) {
	TBuilding::Init(x, y, party, stream);

	if (IconTransport == NULL) {
		IconTransport = new TIcon(RES_X-115, UINFO_Y+110, 102, 23, "tranbut%i", 13);
	}

	if ((Capacity == TCAPACITY_BIG) && !BmpBigInventory) {
		BmpBigInventory = GraphicsDF->get("tranbox0");
	} else if ((Capacity == TCAPACITY_SMALL) && !BmpSmallInventory) {
		BmpSmallInventory = GraphicsDF->get("tranbox1");
	} else if ((Capacity == TCAPACITY_MEDIUM) && !BmpMediumInventory) {
		BmpMediumInventory = GraphicsDF->get("tranbox2");
	}
}



unsigned TBase::GetAvailableActions()
{
    unsigned x = TBuilding::GetAvailableActions();
    if (ID < BADLIFE) x |= uatLoadIn;
    return x;
}



int TBase::WantToLoad(TUnit *u)
{
    return !((u->Type >= unFlint) && (u->Type <= unBarracuda));
}



int TBase::LoadUnit(TUnit *u)
{
    int ttm;
    
    if (LoadedUnits == Capacity) return -2;
    if (!WantToLoad(u)) return -1;
    ttm = u->TimeToMove(X + 2, Y + 6);
    if (ttm < 0) return -1;
    HideHelpers();
    if (!u->Move(X + 2, Y + 6)) return -1;
    if ((u->X != X + 2) || (u->Y != Y + 6)) {ShowHelpers(); return -1;}

    u->PlaceGround(FALSE);
    u->X = u->Y = -1;
    u->TimeUnits = 0;
    if ((u->Type % BADLIFE) != unCeres) u->Fuel = u->MaxFuel;
    for (int iw = 0; iw < u->WeaponsCnt; iw++)
        u->Weapons[iw]->Ammo = u->Weapons[iw]->MaxAmmo;
    if (u->HitPoints != u->MaxHitPoints/2) u->Repair();
    Inventory[LoadedUnits++] = u->ID;
    this->Select();
    ShowHelpers();
    return TRUE;
}



int TBase::UnloadUnit(TUnit *u)
{
    TField *f = GetField(X + 2, Y + 6);
    
    if (u->TimeUnits == 0) return -2;
    if (f->Unit != NO_UNIT) {
        if (((f->Unit < BADLIFE) && (ID >= BADLIFE)) ||
            ((f->Unit >= BADLIFE) && (ID < BADLIFE))) return -1;
        HideHelpers();
        Units[f->Unit]->Select();
                             Units[f->Unit]->Move(X + 1, Y + 6);
        if (f->Unit != NO_UNIT) Units[f->Unit]->Move(X + 3, Y + 6);
        if (f->Unit != NO_UNIT) Units[f->Unit]->Move(X + 3, Y + 7);
        if (f->Unit != NO_UNIT) Units[f->Unit]->Move(X + 1, Y + 7);
        if (f->Unit != NO_UNIT) Units[f->Unit]->Move(X + 2, Y + 7);
        this->Select();
        ShowHelpers();
        if (f->Unit != NO_UNIT) return -1;
        if (!(u->CanGoOnField(X + 2, Y + 6))) return -1;
    }

    u->X = X + 2, u->Y = Y + 6;
    u->PlaceGround(TRUE);
    int i;
    for (i = 0; Inventory[i] != u->ID; i++) {};
    for (++i; i < LoadedUnits; i++) Inventory[i-1] = Inventory[i];
    LoadedUnits--;
    this->Select();
    return TRUE;
}



void TBase::DoInventory()
{
    int rt, uurt;
    TTransBox *tb;
    
    SelectField(0, 0);
    
	switch (Capacity) {
	case TCAPACITY_BIG:
		tb = new TTransBox(472, 202, Inventory, LoadedUnits, (byte*)BmpBigInventory);
		break;
	case TCAPACITY_SMALL:
		tb = new TTransBox(128, 202, Inventory, LoadedUnits, (byte*)BmpSmallInventory);
		break;
	case TCAPACITY_MEDIUM:
		tb = new TTransBox(248, 202, Inventory, LoadedUnits, (byte*)BmpMediumInventory);
		break;
	default:
		abort();
	}

    tb->Show();
    while (TRUE) {      
        rt = tb->Handle();
        if (rt == -1) {tb->Hide(); break;}
        RedrawMapLocks++, SelFldLocks++;
        tb->Hide(TRUE);
        uurt = UnloadUnit((TUnit*)Units[Inventory[rt]]);
        RedrawMapLocks--, SelFldLocks--;
        if (uurt == -1) {
            tb->Show(TRUE);
            Message(MSG_OBJECT_IN_WAY2);
            continue;
        }
        else if (uurt == -2) {
            tb->Show(TRUE);
            Message(MSG_OUT_OF_TIME);
            continue;
        }
        tb->Cnt--;
        RedrawMap();
        tb->Show(TRUE);
        if (LoadedUnits == 0) {tb->Hide(); break;}
    }
    delete tb;
    SelectField(Mouse.x, Mouse.y);
}



int TBase::InfoEvent(TEvent *e)
{
    int rt = TBuilding::InfoEvent(e);
    
    if (!rt && IconTransport->Handle(e)) {
        DoInventory();
        return TRUE;
    }
    return rt;
}



void TBase::Action(int x, int y)
{
    {
        int i, j;
    
        for (i = 0; i < SizeX; i++)
            for (j = 0; j < SizeY; j++)
                if ((x == X+i) && (y == Y+j) && (UnitActionType != uatSelectAir)) {
                    DoInventory();
                    return;
                }
    }
    
    TBuilding::Action(x, y);
}



void TBase::GetUnitInfo(bool alt_wpinfo) {
	TBuilding::GetUnitInfo(alt_wpinfo);
	CopyBmp(UInfoBuf, UINFO_SX, 3, 110, IconTransport->IconPic[0], 102, 23);
	PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 3, 135, 102, 8, clrLightBlue2,
		clrSeaBlue, ((double)LoadedUnits / Capacity), "");
}



void TBase::Read(ReadStream &stream) {
	TBuilding::Read(stream);
	LoadedUnits = stream.readSint32LE();

	for (int i = 0; i < LoadedUnits; i++) {
		Inventory[i] = stream.readSint32LE();
	}
}

void TBase::Write(WriteStream &stream) {
	TBuilding::Write(stream);
	stream.writeSint32LE(LoadedUnits);

	for (int i = 0; i < LoadedUnits; i++) {
		stream.writeSint32LE(Inventory[i]);
	}
}

void TBase::ChangeParty()
{
    TObject *dum;
    
    TBuilding::ChangeParty();
    for (int i = 0; i < LoadedUnits; i++) {
        dum = Units[Inventory[i]];
        dum->ChangeParty();
        Inventory[i] = dum->ID;
    }
}



void TBase::RemoveFromWorld()
{
    for (int i = 0; i < LoadedUnits; i++) 
        Units[Inventory[i]]->RemoveFromWorld();
    TBuilding::RemoveFromWorld();
}







//////////////////// TWarehouse 

void TWarehouse::Setup()
{
    TMutableBuilding::Setup();
    MaxHitPoints = utWR_HP;
    Visib = utWR_VIS;
    Defense[0] = utWR_DN0, Defense[1] = utWR_DN1, Defense[2] = utWR_DN2;
    SizeX = 2, SizeY = 2;
    MoneyIndex = utWR_MONEY;
}



unsigned TWarehouse::GetAvailableActions()
{
    if (ID >= BADLIFE) return TMutableBuilding::GetAvailableActions();
    else return TMutableBuilding::GetAvailableActions() | uatLoadIn;
}



int TWarehouse::RefullUnit(TUnit *u)
{
    int i, j, ttm, bttm = 9999, bx, by;
    TGnom *ugn;
    TGanymedes *uga;
    
    for (i = -1; i < SizeX+1; i++)
        for (j = -1; j < SizeY+1; j++) {
            ttm = u->TimeToMove(X + i, Y + j);
            if ((ttm >= 0) && (ttm < bttm)) bttm = ttm, bx = X + i, by = Y + j;
        }
    if (bttm == 9999) return FALSE;
    HideHelpers();
    if (!u->Move(bx, by)) {ShowHelpers(); return FALSE;}
    if ((u->X != bx) || (u->Y != by)) {ShowHelpers(); return FALSE;}

    u->TimeUnits = 0;
    
    switch (u->Type) {
        case unCeres : 
                u->Fuel = u->MaxFuel;
                break;
        case unGnom : 
                ugn = (TGnom*) u;
                u->Fuel = u->MaxFuel;
                for (i = 0; i < 4; i++) ugn->Ammo[i] = ugn->MaxAmmo[i];
                break;
        case unGanymedes :
                uga = (TGanymedes*) u;
                for (i = 0; i < 4; i++) uga->Ammo[i] = uga->MaxAmmo[i];
                break;
        case unXenon : 
                ((TXenon*)u)->Mines = utXE_MINES;
                u->Fuel = u->MaxFuel;
                break;
        default : 
                u->Fuel = u->MaxFuel;
                for (i = 0; i < u->WeaponsCnt; i++)
                    u->Weapons[i]->Ammo = u->Weapons[i]->MaxAmmo;
                break;
    }   

    ShowHelpers();
    return TRUE;
}






///////////////////////// tovarna na jednotky ///////////////////////////

void TFactory::Setup()
{
    TBuilding::Setup();
    MaxHitPoints = utFA_HP;
    Visib = utFA_VIS;
    Defense[0] = utFA_DN0, Defense[1] = utFA_DN1, Defense[2] = utFA_DN2;
    SizeX = 4, SizeY = 4;
    MoneyIndex = utFA_MONEY;
    CurrentJob = 0;
}



int TFactory::InfoEvent(TEvent *e)
{
    int rt = TBuilding::InfoEvent(e);
    
    if (!rt && IconTransport->Handle(e)) {
        DoProducing();
        return TRUE;
    }
    return rt;
}



void TFactory::Action(int x, int y)
{
    {
        int i, j;
    
        for (i = 0; i < SizeX; i++)
            for (j = 0; j < SizeY; j++)
                if ((x == X+i) && (y == Y+j) && (UnitActionType != uatSelectAir)) {
                    DoProducing();
                    return;
                }
    }
    
    TBuilding::Action(x, y);
}



extern void *ListBoxSprite[6]; // z InterActu :-) ...

void TFactory::DoProducing() {
	char b[40];
	void *bmp = NULL;
	void *bmp2 = memalloc(296 * 431);
	TEvent e;
	int i, top, cnt, scrsz;
	int px, py;
	int *list = GetManufacturableUnits(ID);

	for (cnt = 0; list[cnt]; cnt++);

	top = 0;
	scrsz = (cnt < 8) ? cnt : 7;

	px = RES_X-421;
	py = RES_Y-453-8;

	if (iniResolution == SVGA_640x480) {
		py += 8;
	}

	GetBitmap32(px, py, bmp2, 296, 431);

draw_it_now:
	if (bmp) {
		memfree(bmp);
	}

	bmp = GraphicsDF->get("factory");
	PutStr(bmp, 296, 431, 74, 8, SigText[TXT_FACTORY_TYPE], NormalFont,
		clrWhite, clrBlack);
	PutStr(bmp, 296, 431, 180, 8, SigText[TXT_FACTORY_COST], NormalFont,
		clrWhite, clrBlack);
	PutStr(bmp, 296, 431, 240, 8, SigText[TXT_FACTORY_TURNS], NormalFont,
		clrWhite, clrBlack);

	for (i = 0; i < scrsz; i++) {
		CopyBmp(bmp, 296, 12, 20 + i * 58, UnitsTransIcons[list[top+i]],
			56, 56);
		PutStr(bmp, 296, 431, 74, 40 + i * 58, UnitsNames[list[top+i]],
			NormalFont, clrWhite, clrBlack);
		sprintf(b, SigText[TXT_CREDITS], TabUnitsCost[list[top+i]]);

		if (MoneyGoodlife < TabUnitsCost[list[top+i]]) {
			PutStr(bmp, 296, 431, 180, 40 + i * 58, b, NormalFont,
				clrRed, clrBlack);
		} else {
			PutStr(bmp, 296, 431, 180, 40 + i * 58, b, NormalFont,
				clrWhite, clrBlack);
		}

		sprintf(b, "%i", TabUnitsProducTime[list[top+i]]);
		PutStr(bmp, 296, 431, 260, 40 + i * 58, b, NormalFont,
			clrWhite, clrBlack);
	}

	if (top != 0) {
		CopyBmp(bmp, 296, 140, 10, ListBoxSprite[0], 18, 18);
	}

	if (top < cnt-7) {
		CopyBmp(bmp, 296, 140, 405, ListBoxSprite[2], 18, 18);
	}

	PutBitmap32(px, py, bmp, 296, 431);
	UpdateScreen();

	for (;;) {
		GetEvent(&e);

		if ((e.What == evMouseDown) && (e.Mouse.Buttons == mbRightButton)) {
			break;
		}

		if ((e.What == evMouseDown) && (e.Mouse.Buttons == mbLeftButton)) {
			e.Mouse.Where.x -= px;
			e.Mouse.Where.y -= py;

			if (!IsInRect(e.Mouse.Where.x, e.Mouse.Where.y, 0, 0, 296, 431)) {
				break;
			}

			if (IsInRect(e.Mouse.Where.x, e.Mouse.Where.y, 140, 10, 140+18, 10+18)) {
				if (top > 0) {
					top--;
					goto draw_it_now;
				}
			}

			if (IsInRect(e.Mouse.Where.x, e.Mouse.Where.y, 140, 405, 140+18, 405+18)) {
				if (top < cnt-7) {
					top++;
					goto draw_it_now;
				}
			}

			i = top + (e.Mouse.Where.y - 20) / 58;
			ProduceUnit(list[i]);
			break;
		}
	}

	PutBitmap32(px, py, bmp2, 296, 431);
	memfree(bmp);
	memfree(bmp2);
	this->Select();
}



void TFactory::GetUnitInfo(bool alt_wpinfo) {
	char cbuf[80];

	TBuilding::GetUnitInfo(alt_wpinfo);
	CopyBmp(UInfoBuf, UINFO_SX, 3, 110, IconTransport->IconPic[0], 102, 23);
	sprintf(cbuf, SigText[TXT_FINANCE], MoneyGoodlife);
	PutStr(UInfoBuf, UINFO_SX, UINFO_SY, 2, 60, cbuf, NormalFont, clrWhite,
		clrBlack);

	if (CurrentJob != 0) {
		CopyBmp(UInfoBuf, UINFO_SX, 3, 140, UnitsTransIcons[CurrentJob],
			56, 56);
		sprintf(cbuf, "%i %%", 100 * CurrentPhase / CurrentNeed);
		PercentBar(UInfoBuf, UINFO_SX, UINFO_SY, 62, 140, 45, 12,
			clrLightBlue2, clrSeaBlue,
			((double)CurrentPhase) / CurrentNeed, cbuf);
	}
}



void TFactory::Select()
{
    TBuilding::Select();
    if ((CurrentJob != 0) && (CurrentPhase == CurrentNeed)) { // hotovo
        TObject *obj;
        if (GetField(X + 4, Y + 3)->Unit != NO_UNIT) return;
        switch (CurrentJob) {
#include "iounreg.h"
        }
        obj->Init(X + 4, Y + 3, ID & BADLIFE, NULL);
        CurrentJob = 0;
        RedrawMap();
    }
}



void TFactory::TurnReset()
{
    TObject *obj = NULL;
    
    TBuilding::TurnReset();
    if (CurrentJob != 0) {
        if (CurrentPhase >= CurrentNeed) CurrentPhase = CurrentNeed-1;
        if (++CurrentPhase == CurrentNeed) { // hotovo
            if (GetField(X + 4, Y + 3)->Unit != NO_UNIT) return;
            switch (CurrentJob) {
#include "iounreg.h"
            }
            obj->Init(X + 4, Y + 3, ID & BADLIFE, NULL);
            obj->Rotate(3);
            CurrentJob = 0;         
        }
    }
}


        
int TFactory::ProduceUnit(int untype)
{
    if (CurrentJob != 0) { // vrati money
        SetMoney(ID, GetMoney(ID) + TabUnitsCost[CurrentJob % BADLIFE]);
        CurrentJob = 0;
    }

    if (GetMoney(ID) < TabUnitsCost[untype % BADLIFE]) return FALSE;
    SetMoney(ID, GetMoney(ID) - TabUnitsCost[untype % BADLIFE]);
    
    CurrentJob = untype;
    CurrentPhase = 0;
    CurrentNeed = TabUnitsProducTime[CurrentJob % BADLIFE];
    return TRUE;
}



void TFactory::Read(ReadStream &stream) {
	TBuilding::Read(stream);
	CurrentJob = stream.readSint32LE();
	CurrentPhase = stream.readSint32LE();
	CurrentNeed = stream.readSint32LE();
}

void TFactory::Write(WriteStream &stream) {
	TBuilding::Write(stream);
	stream.writeSint32LE(CurrentJob);
	stream.writeSint32LE(CurrentPhase);
	stream.writeSint32LE(CurrentNeed);
}













/////////////////// TAirport

void TAirport::Setup()
{
    TBase::Setup();
    MaxHitPoints = utAI_HP;
    Visib = utAI_VIS;
    Defense[0] = utAI_DN0, Defense[1] = utAI_DN1, Defense[2] = utAI_DN2;
    SizeX = 5, SizeY = 3;
    LoadedUnits = 0;
    MoneyIndex = utAI_MONEY;
    Capacity = TCAPACITY_MEDIUM;
}



unsigned TAirport::GetAvailableActions()
{
    unsigned x = TBuilding::GetAvailableActions();
    if (ID < BADLIFE) x |= uatLandAircr;
    return x;
}



int TAirport::LoadUnit(TUnit *u)
{
    int ttm;
    
    if (LoadedUnits == Capacity) return -2;
    ttm = u->TimeToMove(X-1, Y+2);
    if (ttm < 0) return -1;
    HideHelpers();
    if (!u->Move(X-1, Y+2)) return -1;
    if ((u->X != X-1) || (u->Y != Y+2)) {ShowHelpers(); return -1;}

    if (GetAllVisib() == 2) {
        int i, altd;
        
        PlaySample(u->GetUnitSound(7), 6, EffectsVolume, GetFieldPanning(u->X, u->Y));
        altd = (u->Alt - Alt) * 14;
        u->Rotate(5);
        for (i = 0; i < 4*28/2; i += 4) {
            u->LittleX = i * 2, u->LittleY = i;
            LD_rect[u->ID].x1 -= 40, LD_rect[u->ID].y1 -= 40;
            LD_rect[u->ID].x2 += 40, LD_rect[u->ID].y2 += 40;
            ((TAircraft*)u)->LittleAlt = -i * altd / 56;
            u->PaintUnit(TRUE);
            ProcessMapAnim();
	    UpdateScreen();
            SDL_Delay(iniAnimDelay);
        }
    }

    u->PlaceGround(FALSE);
    u->X = u->Y = -1;
    u->TimeUnits = 0;
    u->Fuel = u->MaxFuel;
    for (int iw = 0; iw < u->WeaponsCnt; iw++)
        u->Weapons[iw]->Ammo = u->Weapons[iw]->MaxAmmo;
    if (u->Type % BADLIFE == unSaturn) ((TSaturn*)u)->Bombs = utSA_BAMMO;

    Inventory[LoadedUnits++] = u->ID;
    this->Select();
    ShowHelpers();
    UpdateScreen();
    return TRUE;
}



int TAirport::UnloadUnit(TUnit *u)
{
    TAircraft *a = GetAircraftAt(X-1, Y+2);
    
    if (u->TimeUnits == 0) return -2;
    if (a != NULL) {
        if (((a->ID < BADLIFE) && (ID >= BADLIFE)) ||
            ((a->ID >= BADLIFE) && (ID < BADLIFE))) return -1;
        HideHelpers();
        a->Select();
        a->Move(X-2, Y+2  );
	a = GetAircraftAt(X-1, Y+2);
        if (a != NULL)
		a->Move(X-3, Y+2);
	a = GetAircraftAt(X-1, Y+2);
        if (a != NULL)
		a->Move(X-4, Y+2);
	a = GetAircraftAt(X-1, Y+2);
        this->Select();
        ShowHelpers();
        if (a != NULL)
		return -1;
    }

    u->X = X-1, u->Y = Y+2;
    LockDraw();
    u->PlaceGround(TRUE);
    u->Rotate(1);
    UnlockDraw(FALSE);
    int i;
    for (i = 0; Inventory[i] != u->ID; i++) {};
    for (++i; i < LoadedUnits; i++) Inventory[i-1] = Inventory[i];
    LoadedUnits--;

    if (GetAllVisib() == 2) {
        int i, altd;
        
        PlaySample(u->GetUnitSound(6), 6, EffectsVolume, GetFieldPanning(u->X, u->Y));
        HideHelpers();
        u->Select();
        altd = (u->Alt - Alt) * 14;
        LockDraw();
        u->Rotate(1);
        UnlockDraw();
        for (i = 4*28/2; i >= 0; i -= 4) {
            u->LittleX = i * 2, u->LittleY = i;
            LD_rect[u->ID].x1 -= 40, LD_rect[u->ID].y1 -= 40;
            LD_rect[u->ID].x2 += 40, LD_rect[u->ID].y2 += 40;
            ((TAircraft*)u)->LittleAlt = -i * altd / 56;
            u->PaintUnit(TRUE);
            ProcessMapAnim();
	    UpdateScreen();
            SDL_Delay(iniAnimDelay);
        }
        ShowHelpers();
    }

    this->Select();
    UpdateScreen();
    return TRUE;
}



void TAirport::TurnReset()
{
    int i;
    TUnit *u;
    
    TBase::TurnReset();
    for (i = 0; i < LoadedUnits; i++) {
        u = (TUnit*) Units[Inventory[i]];
        u->Repair();
    }
}








/////////////////// THeliport

void THeliport::Setup()
{
    TBase::Setup();
    MaxHitPoints = utHP_HP;
    Visib = utHP_VIS;
    Defense[0] = utHP_DN0, Defense[1] = utHP_DN1, Defense[2] = utHP_DN2;
    SizeX = 4, SizeY = 2;
    LoadedUnits = 0;
    MoneyIndex = utHP_MONEY;
    Capacity = TCAPACITY_SMALL;
}



unsigned THeliport::GetAvailableActions()
{
    unsigned x = TBuilding::GetAvailableActions();
    if (ID < BADLIFE) x |= uatLandHelip;
    return x;
}



int THeliport::LoadUnit(TUnit *u)
{
    int ttm;
    
    if (LoadedUnits == Capacity) return -2;
    ttm = u->TimeToMove(X+3, Y+1);
    if (ttm < 0) return -1;
    HideHelpers();
    if (!u->Move(X+3, Y+1)) return -1;
    if ((u->X != X+3) || (u->Y != Y+1)) {ShowHelpers(); return -1;}

    if (GetAllVisib() == 2) {
        int i, altd;
        
        PlaySample(u->GetUnitSound(7), 6, EffectsVolume, GetFieldPanning(u->X, u->Y));
        altd = (u->Alt - Alt) * 14;
        u->LittleY = -7;
        for (i = 7; i < altd; i += 4) {
            ((TAircraft*)u)->LittleAlt = -i;
            u->PaintUnit(TRUE);
            ProcessMapAnim();
	    UpdateScreen();
            SDL_Delay(iniAnimDelay);
        }
    }

    u->PlaceGround(FALSE);
    u->LittleY = ((TAircraft*)u)->LittleAlt = 0;
    u->X = u->Y = -1;
    u->TimeUnits = 0;
    u->Fuel = u->MaxFuel;
    for (int iw = 0; iw < u->WeaponsCnt; iw++)
        u->Weapons[iw]->Ammo = u->Weapons[iw]->MaxAmmo;
    if (u->Type == unSaturn) ((TSaturn*)u)->Bombs = utSA_BAMMO;

    Inventory[LoadedUnits++] = u->ID;
    this->Select();
    ShowHelpers();
    UpdateScreen();
    return TRUE;
}



int THeliport::UnloadUnit(TUnit *u)
{
    TAircraft *a = GetAircraftAt(X+3, Y+1);
    
    if (u->TimeUnits == 0) return -2;
    if (a != NULL) {
        if (((a->ID < BADLIFE) && (ID >= BADLIFE)) ||
            ((a->ID >= BADLIFE) && (ID < BADLIFE))) return -1;
        HideHelpers();
        a->Select();
        a->Move(X+3, Y+0);
	a = GetAircraftAt(X+3, Y+1);
        if (a != NULL)
		a->Move(X+3, Y-1);
	a = GetAircraftAt(X+3, Y+1);
        if (a != NULL)
		a->Move(X+3, Y+2);
	a = GetAircraftAt(X+3, Y+1);
        this->Select();
        ShowHelpers();
        if (a != NULL)
		return -1;
    }

    u->X = X+3, u->Y = Y+1;
    u->PlaceGround(TRUE);
    int i;
    for (i = 0; Inventory[i] != u->ID; i++) {};
    for (++i; i < LoadedUnits; i++) Inventory[i-1] = Inventory[i];
    LoadedUnits--;

    if (GetAllVisib() == 2) {
        int i, altd;
        
        PlaySample(u->GetUnitSound(6), 6, EffectsVolume, GetFieldPanning(u->X, u->Y));
        HideHelpers();
        u->Select();
        altd = (u->Alt - Alt) * 14;
        u->LittleY = -7;
        for (i = altd; i >= 7; i -= 4) {
            ((TAircraft*)u)->LittleAlt = -i;
            u->PaintUnit(TRUE);
            ProcessMapAnim();
	    UpdateScreen();
            SDL_Delay(iniAnimDelay);
        }
        u->LittleY = ((TAircraft*)u)->LittleAlt = 0;
        ShowHelpers();
    }

    this->Select();
    UpdateScreen();
    return TRUE;
}








///////////////////// TRepairBay:


void TRepairBay::Setup()
{
    TBase::Setup();
    MaxHitPoints = utRB_HP;
    Visib = utRB_VIS;
    Defense[0] = utRB_DN0, Defense[1] = utRB_DN1, Defense[2] = utRB_DN2;
    SizeX = 3, SizeY = 3;
    LoadedUnits = 0;
    MoneyIndex = utRB_MONEY;
    Capacity = TCAPACITY_MEDIUM;
}



void TRepairBay::TurnReset()
{
    int i;
    TUnit *u;
    
    TBase::TurnReset();
    for (i = 0; i < LoadedUnits; i++) {
        u = (TUnit*) Units[Inventory[i]];
        u->Repair();
    }
}



int TRepairBay::GetLoadingPoint(int *x, int *y, TUnit *u)
{
    int t, i, j, best = 0xFFFF;
    
    if (u->X != -1) {
        if ((u->ID < BADLIFE) && (this->ID >= BADLIFE)) return FALSE;
        if ((u->ID >= BADLIFE) && (this->ID < BADLIFE)) return FALSE;
    
        for (i = -1; i <= 3; i++)
            for (j = -1; j <= 3; j++) {
                t = u->TimeToMove(X + i, Y + j);
                if ((t >= 0) && (t < best) && (u->CanGoOnField(X+i, Y+j))) {
                    best = t;
                    *x = X + i, *y = Y + j;                 
                }
            }
    }
    else {
        for (i = -1; i <= 3; i++)
            for (j = -1; j <= 3; j++) {
                if (u->CanGoOnField(X + i, Y + j)) {
                    *x = X+ i, *y = Y + j;
                    best = 0;
                    break;
                }
            }
    }
    
    return (best != 0xFFFF);
}



int TRepairBay::LoadUnit(TUnit *u)
{
    int ttm, xx, yy;
    
    if (LoadedUnits == Capacity) return -2;
    if (!GetLoadingPoint(&xx, &yy, u)) return -1;
    ttm = u->TimeToMove(xx, yy);
    if (ttm < 0) return -1;
    HideHelpers();
    if (!u->Move(xx, yy)) return -1;
    if ((u->X != xx) || (u->Y != yy)) {ShowHelpers(); return -1;}

    u->PlaceGround(FALSE);
    u->X = u->Y = -1;
    u->TimeUnits = 0;

    Inventory[LoadedUnits++] = u->ID;
    this->Select();
    ShowHelpers();
    return TRUE;
}



int TRepairBay::UnloadUnit(TUnit *u)
{
    int xx, yy;
    
    if (!GetLoadingPoint(&xx, &yy, u)) return -1;
    if (u->TimeUnits == 0) return -2;

    u->X = xx, u->Y = yy;
    u->PlaceGround(TRUE);
    int i;
    for (i = 0; Inventory[i] != u->ID; i++) {};
    for (++i; i < LoadedUnits; i++) Inventory[i-1] = Inventory[i];
    LoadedUnits--;
    this->Select();
    return TRUE;
}












/////////////////// TDocks

void TDocks::Setup()
{
    TBase::Setup();
    MaxHitPoints = utDC_HP;
    Visib = utDC_VIS;
    Defense[0] = utDC_DN0, Defense[1] = utDC_DN1, Defense[2] = utDC_DN2;
    SizeX = 5, SizeY = 5;
    LoadedUnits = 0;
    MoneyIndex = utDC_MONEY;
    Capacity = TCAPACITY_BIG;
}



void TDocks::TurnReset()
{
    int i;
    TUnit *u;
    
    TBase::TurnReset();
    for (i = 0; i < LoadedUnits; i++) {
        u = (TUnit*) Units[Inventory[i]];
        u->Repair();
    }
}




int TDocks::WantToLoad(TUnit *u)
{
    return (u->Type >= unFlint) && (u->Type <= unBarracuda);
}



inline int freefx(int id, int x, int y, int delka)
{
    if (GetField(x, y)->Unit != NO_UNIT) return FALSE;
    if (delka >= 1) {
        if (GetField(x-1, y)->Unit != NO_UNIT) 
            if (GetField(x-1, y)->Unit != id) return FALSE;
        if (GetField(x+1, y)->Unit != NO_UNIT)
            if (GetField(x+1, y)->Unit != id) return FALSE;
    }
    if (delka == 2) {
        if (GetField(x-2, y)->Unit != NO_UNIT)
            if (GetField(x-2, y)->Unit != id) return FALSE;
        if (GetField(x+2, y)->Unit != NO_UNIT)
            if (GetField(x+2, y)->Unit != id) return FALSE;
    }
    return TRUE;
}

inline int freefy(int id, int x, int y, int delka)
{
    if (GetField(x, y)->Unit != NO_UNIT) return FALSE;
    if (delka >= 1) {
        if (GetField(x, y-1)->Unit != NO_UNIT) 
            if (GetField(x, y-1)->Unit != id) return FALSE;
        if (GetField(x, y+1)->Unit != NO_UNIT) 
            if (GetField(x, y+1)->Unit != id) return FALSE;
    }
    if (delka == 2) {
        if (GetField(x, y+2)->Unit != NO_UNIT) 
            if (GetField(x, y+2)->Unit != id) return FALSE;
        if (GetField(x, y-2)->Unit != NO_UNIT) 
            if (GetField(x, y-2)->Unit != id) return FALSE;
    }
    return TRUE;
}

TPoint TDocks::GetLoadingPoint(TUnit *u)
{
    int delka;
    TPoint p;
    
    switch (u->Type) {
        case unLaguna : 
        case unKraken :   delka = 2; break;
        case unPoseidon : delka = 1; break;
        default :         delka = 0; break;
    }
    switch (Orient) {
        case 1 : 
                p.y = Y + 2, p.x = X - 1 - delka; 
                break;
        case 3 : 
                p.y = Y + 5 + delka, p.x = X + 2; 
                break;
        case 5 : 
                p.y = Y + 2, p.x = X + 5 + delka; 
                break;
        case 7 : 
                p.y = Y - 1 - delka, p.x = X + 2; 
                break;
    }
    PlaceGround(FALSE);
    if ((Orient == 1) || (Orient == 5)) {
        if (!freefx(u->ID, p.x, p.y, delka)) p.x = -1;
    }
    else {
        if (!freefy(u->ID, p.x, p.y, delka)) p.x = -1;
    }
    PlaceGround(TRUE);
    return p;
}



int TDocks::LoadUnit(TUnit *u)
{
    int ttm;
    TPoint p;
    
    if (LoadedUnits == Capacity) return -2;
    if (!WantToLoad(u)) return -1;
    
    p = GetLoadingPoint(u);
    if (p.x == -1) return -1;
    ttm = u->TimeToMove(p.x, p.y);
    if (ttm < 0) return -1;
    HideHelpers();
    if (!u->Move(p.x, p.y)) return -1;
    if ((u->X != p.x) || (u->Y != p.y)) {ShowHelpers(); return -1;}

    u->PlaceGround(FALSE);
    u->X = u->Y = -1;
    u->TimeUnits = 0;
    if ((u->Type % BADLIFE) != unCeres) u->Fuel = u->MaxFuel;
    for (int iw = 0; iw < u->WeaponsCnt; iw++)
        u->Weapons[iw]->Ammo = u->Weapons[iw]->MaxAmmo;
    Inventory[LoadedUnits++] = u->ID;
    this->Select();
    ShowHelpers();
    return TRUE;
}



int TDocks::UnloadUnit(TUnit *u)
{
    TPoint p;
    
    if (u->TimeUnits == 0) return -2;
    p = GetLoadingPoint(u);
    if (p.x == -1) return -1;

    u->X = p.x, u->Y = p.y; u->Orient = Orient;
    u->PlaceGround(TRUE);
    int i;
    for (i = 0; Inventory[i] != u->ID; i++) {};
    for (++i; i < LoadedUnits; i++) Inventory[i-1] = Inventory[i];
    LoadedUnits--;
    this->Select();
    return TRUE;
}



void TDocks::Init(int x, int y, int party, ReadStream *stream) {
	TObject::Init(x, y, party, stream); // Intentional!!!
	RepairingNow = FALSE;
	Orient = 1;

	if (stream) {
		int moreinfo;

		Orient = stream->readSint32LE();
		moreinfo = stream->readSint32LE();

		if (moreinfo) {
			Read(*stream);
		}
	}

	if (IconTransport == NULL) {
		IconTransport = new TIcon(RES_X-115, UINFO_Y+110, 102, 23, "tranbut%i", 13);
	}

	if ((Capacity == TCAPACITY_BIG) && !BmpBigInventory) {
		BmpBigInventory = GraphicsDF->get("tranbox0");
	} else if ((Capacity == TCAPACITY_SMALL) && !BmpSmallInventory) {
		BmpSmallInventory = GraphicsDF->get("tranbox1");
	} else if ((Capacity == TCAPACITY_MEDIUM) && !BmpMediumInventory) {
		BmpMediumInventory = GraphicsDF->get("tranbox2");
	}
}

void TDocks::WriteInitReq(WriteStream &stream) {
	stream.writeSint32LE(Orient);
	stream.writeSint32LE(TRUE);
}

TSprite *TDocks::GetSprite()
{
    int v = GetAllVisib();
    if (v == 2) return UnitsSprites[Type][Orient];
    else return UnitsSprites[Type][8 + Orient];
}
 

 
