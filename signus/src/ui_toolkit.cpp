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
// Modul pro interaktivni (Wokna) grafiku
//



#include <SDL_timer.h>

#include <time.h>
#include "ui_toolkit.h"
#include "global.h"
#include "sound.h"
#include "shortcut.h"
#include "mouse.h"


MIDASsample IconClickSnd, IconClickSnd2, 
        ButtonClickSnd, 
        TypeOnSnd, TypeOnSnd2;

void *ButtonSprite[2];
void *CheckBoxSprite[2];
void *RadioButtonSprite[2];
void *ListBoxSprite[6];


int InitInteract()
{
    IconClickSnd = LoadSample("click", FALSE);
    IconClickSnd2 = LoadSample("click2", FALSE);
    ButtonClickSnd = LoadSample("button", FALSE);
    TypeOnSnd = LoadSample("typeon", FALSE);
    TypeOnSnd2 = LoadSample("typeon2", FALSE);
    LoadArray(ButtonSprite, 2, GraphicsDF, "button%i");
    LoadArray(CheckBoxSprite, 2, GraphicsDF, "chckbox%i");
    LoadArray(RadioButtonSprite, 2, GraphicsDF, "radbut%i");
    LoadArray(ListBoxSprite, 6, GraphicsDF, "listbox%i");
    return TRUE;
}



int DoneInteract()
{
    FreeSample(IconClickSnd);
    FreeSample(IconClickSnd2);
    FreeSample(ButtonClickSnd);
    FreeSample(TypeOnSnd);
    FreeSample(TypeOnSnd2);
    DisposeArray(ButtonSprite, 2);
    DisposeArray(CheckBoxSprite, 2);
    DisposeArray(RadioButtonSprite, 2);
    DisposeArray(ListBoxSprite, 6);
    return TRUE;
}



///////////////////////////// IKONA //////////////////////////////////


TIcon::TIcon(int ax, int ay, int aw, int ah, const char *iconid, char sh)
{   
    x = ax, y = ay, w = aw, h = ah;
    Shortcut = sh;
    LoadArray(IconPic, 2, GraphicsDF, iconid);
}



void TIcon::Draw(int isdown)
{   
    PutBitmap(x, y, IconPic[isdown], w, h);
}



int TIcon::Handle(TEvent *e)
{
    if ((e->What == evMouseDown) &&
        (IsInRect(e->Mouse.Where.x, e->Mouse.Where.y, 
        x, y, x + w -1, y + h - 1))) {
        Draw(1);
        PlaySample(IconClickSnd, 8, EffectsVolume, 128);
        do {GetEvent(e);} while (e->What != evMouseUp);
        Draw(0);
        return IsInRect(e->Mouse.Where.x, e->Mouse.Where.y, 
                   x, y, x + w -1, y + h - 1);
    }
    else if (Shortcut && (e->What == evKeyDown) && (e->Key.CharCode == Shortcut)) {
        Draw(1);
        PlaySample(IconClickSnd, 8, EffectsVolume, 128);
        SDL_Delay(50);
        Draw(0);
        e->What = evNothing;
        e->Key.KeyCode = SDLK_UNKNOWN;
        return 1;
    }
    else 
        return 0;
}








////////////////////// ikonovy panel /////////////////////////////

#define ICPN_SX      108
#define ICPN_SY      126
#define ICPN_RX      (RES_X-118)
#define ICPN_RY      126
        
extern int TimeReserve;
        
TIconPanel::TIconPanel()
{
    int i;
    
    Buf = (byte*) memalloc(ICPN_SX * ICPN_SY);
    lockmem(Buf, ICPN_SX * ICPN_SY);
    LoadArray((void**)Bmps, 6, GraphicsDF, "icblock%i");
    for (i = 0; i < 6; i++) lockmem(Bmps[i], ICPN_SX * ICPN_SY);
    Mask = Bmps[0];
    for (i = 1; i < 11; i++) IconState[i] = 1;
    Selected = 0;
    LoadArray((void**)HelpStrs, 11, TextsDF, "help%i");
}



static int reserX[4] = {255, 6, 10, 2};

void TIconPanel::Draw()
{
    for (int i = 0; i < ICPN_SX * ICPN_SY; i++) {
        Buf[i] = Bmps[IconState[Mask[i]]][i];
        if (Mask[i] == reserX[TimeReserve]) {   
            if (Buf[i] == 203) Buf[i] = 59;
        }
    }
    PutBitmap32(ICPN_RX, ICPN_RY, Buf, ICPN_SX, ICPN_SY);
}



int TIconPanel::Handle(TEvent *e)
{
    if ((e->What == evMouseDown) &&
        (IsInRect(e->Mouse.Where.x, e->Mouse.Where.y, 
        ICPN_RX, ICPN_RY, ICPN_RX + ICPN_SX -1, ICPN_RY + ICPN_SY - 1))) {
        IconState[Selected] = 5;
        Draw();
        PlaySample(IconClickSnd2, 8, EffectsVolume, 128);
        do {GetEvent(e);} while (e->What != evMouseUp);
        IconState[Selected] = 4;
        Draw();
        if (IsInRect(e->Mouse.Where.x, e->Mouse.Where.y, 
                 ICPN_RX, ICPN_RY, ICPN_RX + ICPN_SX -1, ICPN_RY + ICPN_SY - 1))
            return Selected;
        else return 0;
    }
    
    if (e->What == evKeyDown) {
        if (e->Key.KeyCode == SHORTCUT_MENU) return 7;
        if (e->Key.KeyCode == SHORTCUT_RESERVE1) return 6;
        if (e->Key.KeyCode == SHORTCUT_RESERVE2) return 10;
        if (e->Key.KeyCode == SHORTCUT_RESERVE3) return 2;
        switch (e->Key.CharCode) {
		// FIXME
            case SHORTCUT_NEXTUNIT : /*case SHORTCUT_NEXTUNIT+32 :*/ return 1;
            case SHORTCUT_ENDTURN : /*case SHORTCUT_ENDTURN+32 :*/ return 3;
            case SHORTCUT_CENTER : /*case SHORTCUT_CENTER+32 :*/ return 4;
        }
    }
    return 0;
}


extern int ProcessMapAnim();
extern void UpdatePalette();

void TIconPanel::Update()
{
    int chng = FALSE;
    int i;
    int x = Mouse.x - ICPN_RX, y = Mouse.y - ICPN_RY;   
    
    if (IsInRect(x, y, 0, 0, ICPN_SX-1, ICPN_SY-1)) 
        Selected = Mask[x + y * ICPN_SX];
    else Selected = 0;
    
    for (i = 1; i < 11; i++) {
        if (i == Selected) {if (IconState[i] < 4) {IconState[i]++; chng = TRUE;}}
        else if (IconState[i] > 1) {IconState[i]--; chng = TRUE;}
    }
    if (chng) Draw();
}



void TIconPanel::LightsOff()
{
    int i, st;

    do {
        st = FALSE;
        for (i = 1; i < 11; i++) if (IconState[i] > 1) {
            st = TRUE;
            IconState[i]--;
        }       
        if (st) {Draw(); SDL_Delay(10);};
    } while (st);
}



int TIconPanel::Help() {
	TEvent e;
	void *buf, *buf2;
	int bufszx, bufszy;
	int xp, yp;
	int OMX, OMY;
	char *itext;

	if (!IsInRect(Mouse.x - ICPN_RX, Mouse.y - ICPN_RY,
		0, 0, ICPN_SX-1, ICPN_SY-1)) {
		return FALSE;
	}

	itext = HelpStrs[Mask[(Mouse.x - ICPN_RX) + ICPN_SX * (Mouse.y - ICPN_RY)]];
	bufszx = GetStrWidth(itext, NormalFont) + 4;
	bufszy = GetStrHeight(itext, NormalFont);
	buf = memalloc(bufszx * bufszy);
	buf2 = memalloc(bufszx * bufszy);
	yp = Mouse.y - bufszy / 2 + 10;
	xp = Mouse.x - bufszx;

	GetBitmap(xp, yp, buf, bufszx, bufszy);
	memcpy(buf2, buf, bufszx * bufszy);
	DoDarking(buf, bufszx * bufszy);
	PutStr(buf, bufszx, bufszy, 2, 0, itext, NormalFont, clrWhite,
		clrBlack);
	PutBitmap(xp, yp, buf, bufszx, bufszy);
	OMX = Mouse.x, OMY = Mouse.y;

	do {
		GetEvent(&e);

		if (TimerValue % (20 * 5) == 0) { // jukebox
			if (MusicOn && (!IsMusicPlaying())) {
				JukeboxNext();
			}
		}

		if (ProcessMapAnim()) {
			PutBitmap(xp, yp, buf, bufszx, bufszy);
		}
	} while ((e.What == evNothing) || ((e.What == evMouseMove) &&
		IsInRect(e.Mouse.Where.x, e.Mouse.Where.y, OMX - 10, OMY - 10, OMX + 10, OMY + 10)));

	PutEvent(&e);
	PutBitmap(xp, yp, buf2, bufszx, bufszy);
	memfree(buf);
	memfree(buf2);
	return TRUE;
}



TIconPanel::~TIconPanel()
{
    DisposeArray((void**)HelpStrs, 11);
    unlockmem(Buf, ICPN_SX * ICPN_SY);
    memfree(Buf);
    for (int i = 0; i < 6; i++) unlockmem(Bmps[i], ICPN_SX * ICPN_SY);
    DisposeArray((void**)Bmps, 6);
}









///////////////////////////// HERNI MENU /////////////////////////////


#define MENU_X   (RES_X-418)
#define MENU_Y   126
#define MENU_SX  300
#define MENU_SY  126
#define MENU_STEP 20


            
TMenu::TMenu()
{
    LoadArray((void**)Bmps, 3, GraphicsI18nDF, "menu%i");
    Selected = 0;
}



void TMenu::Show()
{
    MouseSetCursor(mcurArrow);
    Buf = (byte*) memalloc(MENU_SX * MENU_SY);
    Bkg = (byte*) memalloc(MENU_SX * MENU_SY);
    lockmem(Buf, MENU_SX * MENU_SY);
    lockmem(Bkg, MENU_SX * MENU_SY);
    GetBitmap32(MENU_X, MENU_Y, Bkg, MENU_SX, MENU_SY);

    MouseHide();
    if (iniEnhancedGuiOn)
        for (int i = 300; i > 0; i -= MENU_STEP) {Draw(i); SDL_Delay(15);}
    else Draw(0);
    MouseShow();
}



void TMenu::Draw(int phase)
{
    if (phase != 0) {
        for (int i = 0; i < MENU_SY; i++) {
            memcpy(Buf + i * MENU_SX, Bkg + i * MENU_SX, phase);
            memcpy(Buf + i * MENU_SX + phase, Bmps[1] + i * MENU_SX, MENU_SX - phase);
        }
    }
    PutBitmap32(MENU_X, MENU_Y, Buf, MENU_SX, MENU_SY);
}



void TMenu::Hide()
{
    MouseHide();
    if (iniEnhancedGuiOn)
        for (int i = 0; i < 300; i += MENU_STEP) {Draw(i); SDL_Delay(15);}
    PutBitmap32(MENU_X, MENU_Y, Bkg, MENU_SX, MENU_SY);
    MouseShow();
    unlockmem(Buf, MENU_SX * MENU_SY);
    unlockmem(Bkg, MENU_SX * MENU_SY);
    memfree(Bkg);
    memfree(Buf);
}



int TMenu::Exec()
{
    TEvent e;
    
    Selected = -1; Select(0, 0);
  while (TRUE) {
        UpdatePalette();
    GetEvent(&e);
    e.Mouse.Where.x -= MENU_X; e.Mouse.Where.y -= MENU_Y; 
    if (e.What == evNothing) {
            if (TimerValue % (20 * 5) == 0) { // jukebox
                if (MusicOn && (!IsMusicPlaying())) JukeboxNext();
            }
    }
    else if (e.What == evMouseDown) {
        if (e.Mouse.Buttons == mbRightButton) return 0;
        else if (e.Mouse.Buttons == mbLeftButton) {
        if (Selected != 0) PlaySample(IconClickSnd2, 8, EffectsVolume, 128);
            return Selected;
        }
    }
    else if (e.What == evMouseMove) {
        Select(e.Mouse.Where.x, e.Mouse.Where.y);
        }
        else if (e.What == evKeyDown) return 0;
  }
}



void TMenu::Select(int x, int y)
{
    int olds = Selected;
    
    if (!IsInRect(x, y, 0, 0, MENU_SX-1, MENU_SY-1)) Selected = 0;
    else Selected = Bmps[0][x + y * MENU_SX];
    
    if (olds == Selected) return;
    
    if (Selected == 0) 
        memcpy(Buf, Bmps[1], MENU_SX * MENU_SY);
    else 
        for (int i = 0; i < MENU_SX * MENU_SY; i++)
            Buf[i] = (Bmps[0][i] == Selected) ? Bmps[2][i] : Bmps[1][i];
    Draw(0);
}



TMenu::~TMenu()
{
    DisposeArray((void**)Bmps, 3);
}










///////////////////////////// DIALOG /////////////////////////////////


void *DrwViewBf;
int DrwViewBfSzX, DrwViewBfSzY;
    
TDialog::TDialog(int ax, int ay, int aw, int ah, const char *backimg)
{
    x = ax, y = ay;
    w = (aw / 4) * 4, h = (ah / 4) * 4;
    strcpy(bgimage, backimg);
    SubviewsCnt = 0;
    BkgBuf = memalloc(w * h);
    if (backimg) DrwBuf = GraphicsDF->get(backimg);
    else {DrwBuf = memalloc(w * h); memset(DrwBuf, 0, w * h);}
    GetBitmap32(x, y, BkgBuf, w, h);
    lockmem(DrwBuf, w * h);
}



void TDialog::PaintRect(int ay, int ah)
{
    PutBitmap32(x, y + ay, ((byte*)DrwBuf) + ay * w, w, ah);
}



void TDialog::Draw() {
	if (*bgimage) {
		if (DrwBuf) {
			memfree(DrwBuf);
		}

		DrwBuf = GraphicsDF->get(bgimage);
	}

	DrwViewBf = DrwBuf;
	DrwViewBfSzX = w;
	DrwViewBfSzY = h;

	for (int i = 0; i < SubviewsCnt; i++) {
		Subviews[i]->Draw();
	}
}



void TDialog::Insert(TView *v)
{
    Subviews[SubviewsCnt++] = v;
    v->Owner = (void*)this;
}

int TDialog::SpecialHandle(TEvent *e, int Cmd)
{
    if (Cmd > 0) return FALSE;
    else { 
        if (e->What == evKeyDown) {
            if (e->Key.KeyCode == kbEsc) return cmCancel;     
        }
        else if (e->What == evMouseDown) {
            if (e->Mouse.Buttons == mbRightButton) return cmCancel;
        }
        return FALSE;
    }
}

int TDialog::HandleEvent(TEvent *e) {
	int i, tmp, ret = -1;
	TView *v;

	if (e->What == evNothing) {
		return -1;
	}

	DrwViewBf = DrwBuf;
	DrwViewBfSzX = w;
	e->Mouse.Where.x -= x;
	e->Mouse.Where.y -= y;

	for (i = 0; i < SubviewsCnt; i++) {
		v = Subviews[i];
		tmp = -1;

		if (((e->What & evMouse) == 0) || IsInRect(e->Mouse.Where.x,
			e->Mouse.Where.y, v->x, v->y, v->x + v->w - 1,
			v->y + v->h - 1)) {

			e->Mouse.Where.x -= v->x;
			e->Mouse.Where.y -= v->y;
			tmp = v->HandleEvent(e);
			e->Mouse.Where.x += v->x;
			e->Mouse.Where.y += v->y;

			if (tmp) {
				ret = tmp;
				break;
			}
		}
	}

	tmp = SpecialHandle(e, ret);

	if (tmp) {
		ret = tmp;
	}

	return ret;
}



int TDialog::Exec() {
	int rtn = -1;
	int OldTimer = -1;
	TEvent e;

	MouseSetCursor(mcurArrow);
	Draw();
	FadeDlg(TRUE);
	MouseSetRect(x, y, x+w-1, y+h-1);

	while (rtn == -1) {
		GetEvent(&e);

		if (e.What == evNothing) {
			if (OldTimer != TimerValue) {
				e.What = evTimer;
				OldTimer = TimerValue;
			}

			if (TimerValue % (20 * 5) == 0) { // jukebox
				if (MusicOn && (!IsMusicPlaying())) {
					JukeboxNext();
				}
			}
		}

		rtn = HandleEvent(&e);
	}

	MouseSetRect(0, 0, RES_X-1, RES_Y-1);
	FadeDlg(FALSE);
	return rtn;
}




void TDialog::FadeDlg(int In)
{
    int i, j, y1, y2, x1, x2;

    EnterTCS();
    MouseFreeze(x, y, w, h);
    if (In) {
        lockmem(DrwBuf, w * h);

        if (iniEnhancedGuiOn) {
            x1 = (w-1) / 2 - 8, x2 = (w-1) / 2;
            y1 = (h-1) / 2 - 8, y2 = (h-1) / 2;

            while ((x1 > 0) && (x2 < w - 8)) {
                for (i = 0; i < 8; i++) {
                    PutBitmap(x + x1, y + y1+i, ((byte*)DrwBuf) + w * (0+i) + x1, x2-x1+1, 1);
                    PutBitmap(x + x1, y + y2+i, ((byte*)DrwBuf) + w * (h-8+i) + x1, x2-x1+1, 1);
                }
                x1 -= 8, x2 += 8;
                if (x1 < 0) x1 = 0;
		if (x2 > w - 8) x2 = w - 8;
                GetEvent(NULL);
                SDL_Delay(4);
            }
            y1 -= 8, y2 += 8;
            while ((y1 > 0) && (y2 < h - 8)) {
                PutBitmap32(x, y + y1, ((byte*)DrwBuf) + w * 0, w, 8);
                PutBitmap32(x, y + y2, ((byte*)DrwBuf) + w * (h-8), w, 8);
                PutBitmap32(x, y + y1+8, ((byte*)DrwBuf) + w * (y1+8), w, 8);
                PutBitmap32(x, y + y2-8, ((byte*)DrwBuf) + w * (y2-8), w, 8);
                y1 -= 8, y2 += 8;
                GetEvent(NULL);
                SDL_Delay(10);                
            }
        }
        PaintRect(0, h);
        unlockmem(DrwBuf, w * h);
    }
    else {
        lockmem(BkgBuf, w * h);
        lockmem(DrwBuf, w * h);
        if (iniEnhancedGuiOn) {
            for (i = 0; i < w; i += 4)
                for (j = 0;j < h; j += 4)
                    ((byte*)DrwBuf)[j * w + i] = ((byte*)BkgBuf)[j * w + i];
            PutBitmap32(x, y, DrwBuf, w, h);
            SDL_Delay(40);
            for (i = 0; i < w; i += 2)
                for (j = 0;j < h; j += 2)
                    ((byte*)DrwBuf)[j * w + i] = ((byte*)BkgBuf)[j * w + i];
            PutBitmap32(x, y, DrwBuf, w, h);
            SDL_Delay(40);
        }

        PutBitmap32(x, y, BkgBuf, w, h);
        unlockmem(DrwBuf, w * h);
        unlockmem(BkgBuf, w * h);
    }
    MouseUnfreeze();
    LeaveTCS();
}



TDialog::~TDialog()
{
    unlockmem(DrwBuf, w * h);
    memfree(DrwBuf);
    memfree(BkgBuf);
    for (int i = 0; i < SubviewsCnt; i++) delete Subviews[i];
}





//////////////////////////// VIEWS //////////////////////////////////////


void TView::Paint(int ay, int ah)
{
    if (ah == -1) {
        if (Owner) ((TDialog*)Owner)->PaintRect(y, h);
    }
    else {
        if (Owner) ((TDialog*)Owner)->PaintRect(y + ay, ah);
    }
}




////////////////////// TButton:

TButton::TButton(int ax, int ay, char *txt, int aCmd, int aIsDef, char *aBmp) 
        : TView(ax, ay, 128, 32)
{
    Text = txt;
    Cmd = aCmd;
    IsDefault = aIsDef;
    Position = 0;
    Bmp = NULL;
    if (aBmp != NULL) Bmp = GraphicsDF->get(aBmp);
}



void TButton::Draw() {
	/*on-off*/
	int ax = x + (128 - GetStrWidth(Text, NormalFont)) / 2 + Position;
	int ay = y + (32 - GetStrHeight(Text, NormalFont)) / 2 + Position;

	CopyBmpNZ(DrwViewBf, DrwViewBfSzX, x, y, ButtonSprite[Position], 128, 32);

	if (Bmp) {
		CopyBmpNZ(DrwViewBf, DrwViewBfSzX, x, y, Bmp, 128, 32);
	}

	PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, ax, ay, Text, NormalFont, clrWhite, clrBlack);
}



int TButton::HandleEvent(TEvent *e)
{
    if ((IsDefault) && (e->What == evKeyDown) && (e->Key.KeyCode == kbEnter)) {
        Position = 1; Draw(); Paint();
        PlaySample(ButtonClickSnd, 8, EffectsVolume, 128);
        SDL_Delay(50);
        Position = 0; Draw(); Paint();
        return Cmd;
    }
    if ((e->What == evMouseDown) && (e->Mouse.Buttons == mbLeftButton)) {
        Position = 1; Draw(); Paint();
        PlaySample(ButtonClickSnd, 8, EffectsVolume, 128);
        do {GetEvent(e);} while (e->What != evMouseUp);
        Position = 0; Draw(); Paint();
        if (IsInRect(
               e->Mouse.Where.x - ((TDialog*)Owner)->x - x,
               e->Mouse.Where.y - ((TDialog*)Owner)->y - y,
                   0, 0, w -1, h - 1))
            return Cmd;
    }
    return FALSE;
}





////////////////////// TStaticText:

void TStaticText::Draw() {
	PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, x, y, Text,
		BigFnt ? HugeFont : NormalFont, clrWhite, clrBlack);
}







////////////////////// TStaticText2:


TStaticText2::TStaticText2(int ax, int ay, int aw, int ah, const char *aTxt)
             : TStaticText(ax, ay, aw, ah, aTxt) 
{
    Text = aTxt; BigFnt = FALSE; 
    IPosX = ax; IPosY = ay; ISizeX = aw; ISizeY = ah;
    
    
    char *Character;
    char *Word;
    const char *Brief;
    
    Brief = Text; // !!!!!!!!!!!!!!!

    int Exit = 0;
    int CharCnt = 0;
    int WordCharCnt = 0;
    int i;
    int Type = 0;
    
    WordCnt = 0;
    
    Word = (char*)memalloc(32);
    Character = (char*)memalloc(1);
    
    for (i = 0; i < MaxNumOfLines; i++) LineSpace[i] = 0;

    
    // vytvori seznam slov (priradi zacatky odstavcu a odkazy)
    do {
    *Character = Brief[CharCnt]; CharCnt++;

        // eliminuje ENTER
        if (*Character == 10) {         
      *Character = Brief[CharCnt]; CharCnt++;
      if (*Character == 13) {
        *Character = Brief[CharCnt]; CharCnt++;
      }
        }
        
        // zjisti specialni znak
        if (*Character == '#') {            
      *Character = Brief[CharCnt]; CharCnt++;
      
          if (*Character == '>') {  // new article
        *Character = Brief[CharCnt]; CharCnt++;
          Type = 1;
          } else
          
          
          if (*Character == 'e') {  // end of text
        *Character = Brief[CharCnt]; CharCnt++;
            Exit = 1;
          }
        }
        
        // dalsi slovo
        if ((*Character == ' ') || (Exit != 0)) {
          // ulozeni jednoho slova
          Word[WordCharCnt] = ' ';
          Word[WordCharCnt+1] = '\0';
          Words[WordCnt] = (char*)memalloc(strlen(Word)+1);
          strcpy(Words[WordCnt], Word);
          WordCharCnt = 0;
          WordsTypes[WordCnt+1] = Type; Type = 0;
          
            WordCnt++;
      *Character = Brief[CharCnt]; CharCnt++;
        }
        
        // nacpe dalsi znak do aktualniho slova
        Word[WordCharCnt] = *Character;
        WordCharCnt++;
        
  } while (Exit == 0);
    
    
  int LineLength = 0;
  int LineWordCnt = 0;
  int LastWordLength;
  
  NumOfArticles = 0;
  NumOfLines = -1;
  
  for (i=0; i<WordCnt; i++) {
    
    // Dalsi odstavec
    if (WordsTypes[i] == 1) {
        NumOfLines++;
        LineLength = GetStrWidth("  ", NormalFont);
        LineWordCnt = 0;
        NumOfArticles++;
    }
    
    LastWordLength = GetStrWidth(Words[i], NormalFont);
    LineLength += LastWordLength;
    LineWordCnt++;
    
    // Preteceni pres velikost jedne radky
    if (LineLength >= ISizeX) {
        LineWordCnt--;
        i--;
        LineLength -= LastWordLength;
        
        WordsTypes[i] = 2;
        if (LineWordCnt > 1)
            LineSpace[NumOfLines] = (double)(ISizeX-LineLength)/(double)(LineWordCnt-1);
        
        LineLength = 0;
        LineWordCnt = 0;
        NumOfLines++;
    }
  }
  
  
  if (LineWordCnt > 0) {
    NumOfLines++;
  }
  WordsTypes[WordCnt-1] = 3;
  
  
  BigDrawBuffer = memalloc(ISizeX*(ISizeY+200));
  memset(BigDrawBuffer, 0, ISizeX*ISizeY);
    memfree(Word);
    memfree(Character);
    
}


void TStaticText2::Draw() {
	int i;
	int WordLineCnt;
	int LocalWordCnt = 0;
	int PixColCnt = 0;
	int ArtCnt = -1; // Citac odstavcu
	double PixLineCnt;

	for (i = 0; i <= NumOfLines; i++) {
		WordLineCnt = -1;
		PixLineCnt = 0;

		do {
			WordLineCnt++;
			LocalWordCnt++;

			if (WordsTypes[LocalWordCnt] == 1) {
				if (LocalWordCnt != 1) {
					i++;
				}

				ArtCnt++;
				PixLineCnt = GetStrWidth("  ", NormalFont);

				if (LocalWordCnt != 1) {
					PixColCnt += ArticleSpace * 2;
				}

				WordLineCnt = 0;
			}

			PutStr(BigDrawBuffer, ISizeX, ISizeY, PixLineCnt,
				/*10,*/ PixColCnt, Words[LocalWordCnt],
				NormalFont, clrWhite, clrBlack);
			PixLineCnt += (double)GetStrWidth(Words[LocalWordCnt],
				NormalFont);
			PixLineCnt += LineSpace[i];

			if (WordsTypes[LocalWordCnt] == 3) {
				goto LastWord;
			}
		} while (WordsTypes[LocalWordCnt] != 2);

		PixColCnt += 16;
	}

LastWord:
	CopyBmpNZ(DrwViewBf, DrwViewBfSzX, IPosX, IPosY, BigDrawBuffer, ISizeX, ISizeY);

	// Tohle bude v destruktoru
	memfree(BigDrawBuffer);

	for (i = 0; i < WordCnt; i++) {
		memfree(Words[i]);
	}
}




////////////////////// TAnimText:

TAnimText::TAnimText(int ax, int ay, int aw, int ah, char *aTxt, TView *aAfter)
        : TStaticText(ax, ay, aw, ah, aTxt, FALSE)
{
    Phase = 0;
    Line = 1; OldLine = Line;
    Ended = FALSE;
    After = aAfter;
    OldBmp = NULL;
}



void TAnimText::Draw() {
	uint8_t buf[4096];
	int ln;

	if (Ended) {
		if (OldBmp != NULL) {
			CopyBmp(DrwViewBf, DrwViewBfSzX, x, y, OldBmp, w, h);
		}

		TStaticText::Draw();
		return;
	}

	if (Phase == 0) {
		return;
	}

	if (OldBmp == NULL) {
		OldBmp = memalloc(w * h);
		CopyFromBmp(DrwViewBf, DrwViewBfSzX, x, y, OldBmp, w, h);
	} else {
		CopyBmp(DrwViewBf, DrwViewBfSzX, x, y, OldBmp, w, h);
	}

	memcpy(buf, Text, Phase);

	if (Line == OldLine) {
		// FIXME: magic constants
		buf[Phase] = 219;
		buf[Phase+1] = 0;
		PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, x, y,
			(const char*)buf, NormalFont, clrWhite, clrBlack);
	} else {
		buf[Phase] = 0;
		PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, x, y,
			(const char*)buf, NormalFont, clrWhite, clrBlack);

		for (ln = 0; ln < 12; ln++) {
			buf[ln] = 219;
		}

		buf[ln] = 0;
		PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, x,
			y + (Line-1)*16, (const char*)buf, NormalFont,
			clrWhite, clrWhite);
	}
}



int TAnimText::HandleEvent(TEvent *e)
{
    if ((e->What == evTimer) && (!Ended)) {
        if ((After != NULL) && (!(((TAnimText*)After)->Ended))) return FALSE;
        Phase++;
        if (Text[Phase] == 13) Line++;
        if (Text[Phase] == 0) Ended = TRUE;
        else {
            Phase++;
            if (Text[Phase] == 13) Line++;
            if (Text[Phase] == 0) Ended = TRUE;
        }
        if (OldLine == Line) PlaySample(TypeOnSnd, 8, EffectsVolume, 128);
        else PlaySample(TypeOnSnd2, 8, EffectsVolume, 128);
        Draw(); 
        if (OldLine == Line) Paint((Line-1)*16, 16);
        else Paint((Line-2)*16, 2*16);
        OldLine = Line;
        if (Ended && (OldBmp != NULL)) {memfree(OldBmp); OldBmp = NULL;}
    }
    return FALSE;
}







////////////////////// TBarGauge:

TBarGauge::TBarGauge(int ax, int ay, int aw, int ah, int *aValue, int aMax, TGaugeUpdateProc aUpdat)
        : TView(ax, ay, aw, ah)
{
    UpdatProc = aUpdat;
    Value = aValue;
    MaxVal = aMax;
    if (*Value < 0) *Value = 0;
    else if (*Value > MaxVal) *Value = MaxVal;
    Back = NULL;
}



void TBarGauge::Draw()
{
    int i, blocks;
    
    if (Back == NULL) {
        Back = memalloc(w * h);
        CopyFromBmp(DrwViewBf, DrwViewBfSzX, x, y, Back, w, h);
    }
    CopyBmp(DrwViewBf, DrwViewBfSzX, x, y, Back, w, h);
    RectBmp(DrwViewBf, DrwViewBfSzX, x, y, w, h, 54, 62);
    blocks = ((*Value) * (w-4)) / (MaxVal * 3);
    if (*Value == 0) return;
    for (i = 0; i <= blocks; i++)
        BarBmp(DrwViewBf, DrwViewBfSzX, x+2 + i * 3, y+2, 2, h-4, 58);
}



int TBarGauge::HandleEvent(TEvent *e)
{
    if (((e->What == evMouseDown) || (e->What == evMouseMove))
        && (e->Mouse.Buttons == mbLeftButton)) {
        *Value = MaxVal * (e->Mouse.Where.x-2) / (w-4);
        if (*Value < 0) *Value = 0;
        if (*Value > MaxVal) *Value = MaxVal;
        Draw(); Paint();
        if (UpdatProc != NULL) UpdatProc(Value, FALSE);
    }
    if (e->What == evMouseUp) {
        if (UpdatProc != NULL) UpdatProc(Value, TRUE);
    }
    return FALSE;
}



TBarGauge::~TBarGauge()
{
    if (Back != NULL) memfree(Back);
}









////////////////////// TCheckBox:

TCheckBox::TCheckBox(int ax, int ay, int aw, char *aText, int *aValue) :
           TView(ax, ay, aw, 16)
{
    Value = aValue;
    Text = aText;
}



void TCheckBox::Draw() {
	if (*Value) {
		CopyBmp(DrwViewBf, DrwViewBfSzX, x, y, CheckBoxSprite[1], 16,
			16);
	} else {
		CopyBmp(DrwViewBf, DrwViewBfSzX, x, y, CheckBoxSprite[0], 16,
			16);
	}

	PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, x + 20, y, Text,
		NormalFont, clrWhite, clrBlack);
}



int TCheckBox::HandleEvent(TEvent *e)
{
    if ((e->What == evMouseDown) && 
        (IsInRect(e->Mouse.Where.x, e->Mouse.Where.y, 0, 0, GetStrWidth(Text, NormalFont), 16))) {
        *Value = !(*Value);
        Draw(); Paint();
    }
    return FALSE;
}









////////////////////// TRadioButtons:

TRadioButtons::TRadioButtons(int ax, int ay, int aw, char **aitems, int acnt, int *asel) :
            TView(ax, ay, aw, acnt * 16)
{
    Items = aitems;
    Selected = asel;
    Cnt = acnt;
}



void TRadioButtons::Draw() {
	for (int i = 0; i < Cnt; i++) {
		if (*Selected == i) {
			CopyBmpNZ(DrwViewBf, DrwViewBfSzX, x, y + i*16,
				RadioButtonSprite[1], 16, 16);
		} else {
			CopyBmpNZ(DrwViewBf, DrwViewBfSzX, x, y + i*16,
				RadioButtonSprite[0], 16, 16);
		}

		PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, x + 20, y + i*16,
			Items[i], NormalFont, clrWhite, clrBlack);
	}
}



int TRadioButtons::HandleEvent(TEvent *e)
{
    int it;
    
    if ((e->What == evMouseDown)) {
        it = e->Mouse.Where.y / 16;
        *Selected = it;
        Draw(); Paint();
    }
    return FALSE;
}




//////////////////////// TMapViewer:

TBitmap::TBitmap(int ax, int ay, int aframe, void *aPic, int aSx, int aSy, int aDispose)
                : TView(ax, ay, aSx, aSy)
{
    frame = aframe;
    sx = aSx, sy = aSy;
    Pic = aPic;
    if (frame) w += 4, h += 4;
    DisposeIt = aDispose;
}



void TBitmap::Draw()
{
    if (frame) {
        RectBmp(DrwViewBf, DrwViewBfSzX, x, y, w, h, 54, 62);
        CopyBmp(DrwViewBf, DrwViewBfSzX, x+2, y+2, Pic, sx, sy);
    }
    else 
        CopyBmp(DrwViewBf, DrwViewBfSzX, x, y, Pic, sx, sy);
}










//////////////////////// TListBox:
    
TListBox::TListBox(int ax, int ay, int aw, int aLns, char *aData[], int aCnt, int aCmd,
                   void (*aFocusProc)(TListBox *l), void (*aSelectProc)(TListBox *l))
         : TView(ax, ay, aw, aLns * 18)
{
    Data = aData;
    Cnt = aCnt;
    Current = 0;
    Delta = 0;
    ScrLn = aLns;
    Cmd = aCmd;
    UpArrow = 0, DownArrow = 2, PosArrow = 4;
    SelectProc = aSelectProc, FocusProc = aFocusProc;
}



void TListBox::Draw() {
	int i, ofs, clr;
	char helperBuffer[1024];

	for (i = Delta; (i < Cnt) && (i < Delta + ScrLn); i++) {
		ofs = y + (i - Delta) * 18;
		clr = (i == Current) ? 85 : 80;
		BarBmp(DrwViewBf, DrwViewBfSzX, x + 18, ofs + 1, w - 18, 16,
			clr);
		RectBmp(DrwViewBf, DrwViewBfSzX, x + 18, ofs, w - 18, 18,
			clr-2, clr+2);

		char *text = Data[i];

		while (GetStrWidth(text, NormalFont) > w-20) {
			if (text == Data[i]) {
				strcpy(helperBuffer, Data[i]);
				text = helperBuffer;
			}

			text[strlen(text)-1] = 0;
		}

		PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, x + 20, ofs,
			text, NormalFont, clrWhite, clrBlack);
	}

	for (; i < Delta + ScrLn; i++) {
		ofs = y + (i - Delta) * 18;
		clr = 80;
		BarBmp(DrwViewBf, DrwViewBfSzX, x + 18, ofs + 1, w - 18, 16,
			clr);
		RectBmp(DrwViewBf, DrwViewBfSzX, x + 18, ofs, w - 18, 18,
			clr-2, clr+2);
	}

	RectBmp(DrwViewBf, DrwViewBfSzX, x, y, w, h, 54, 62);
	CopyBmp(DrwViewBf, DrwViewBfSzX, x, y, ListBoxSprite[UpArrow], 18, 18);
	CopyBmp(DrwViewBf, DrwViewBfSzX, x, y + h - 18,
		ListBoxSprite[DownArrow], 18, 18);
	RectBmp(DrwViewBf, DrwViewBfSzX, x, y + 18, 18, h - 2 * 18, 54, 62);
	BarBmp(DrwViewBf, DrwViewBfSzX, x + 1, y + 19, 16, h - 2 * 19, 20);

	if (ScrLn < Cnt) {
		CopyBmp(DrwViewBf, DrwViewBfSzX, x + 1,
			y + 19 + (((ScrLn - 3) * 18) * Delta / (Cnt-ScrLn)),
			ListBoxSprite[PosArrow], 16, 16);
	}
}



int TListBox::HandleEvent(TEvent *e)
{
    int newCur, oldCur;
    
    TView::HandleEvent(e);

    if ((e->What == evMouseDown) && (e->Mouse.Buttons == mbLeftButton) &&
          (e->Mouse.Where.x >= 18)) {
        newCur = Delta + e->Mouse.Where.y / 18;
        if (Current != newCur) {
            if (newCur < Cnt) {
                oldCur = Current, Current = newCur;
                Draw();
                Paint((newCur - Delta) * 18, 18); 
                if ((oldCur >= Delta) && (oldCur < Delta + ScrLn)) Paint((oldCur - Delta) * 18, 18);
                if (FocusProc != NULL) FocusProc(this);
            }
        }
        else {
            if (SelectProc == NULL) return Cmd; 
            else SelectProc(this);
        }
    }       
    
    if ((e->What == evMouseDown) && (e->Mouse.Buttons == mbLeftButton) &&
        (e->Mouse.Where.x < 18) && (ScrLn < Cnt)) {
        if (e->Mouse.Where.y < 18) {       // scroll up by 1
            if (Delta > 0) Delta--;
            UpArrow = 1;
            Draw(); Paint();
            SDL_Delay(50);
            UpArrow = 0;
            Draw(); Paint(0, 18);
        }   
        else if (e->Mouse.Where.y > h - 18) {   // scroll down by 1
            if (Delta + ScrLn < Cnt) Delta++;
            DownArrow = 3;
            Draw(); Paint();
            SDL_Delay(50);
            DownArrow = 2;
            Draw(); Paint(h - 18, 18);
        }
        else {                                    // draging 
            PosArrow = 5;
            do {
                Delta = (e->Mouse.Where.y - 19-8) * (Cnt - ScrLn) / ((ScrLn-2) * 18-2-16);
                if (Delta < 0) Delta = 0;
                if (Delta + ScrLn > Cnt) Delta = Cnt - ScrLn;
                Draw(); Paint();
                do {GetEvent(e);} while (e->What == evNothing);
                e->Mouse.Where.x -= ((TDialog*)Owner)->x + x;
                e->Mouse.Where.y -= ((TDialog*)Owner)->y + y;
            } while (e->What != evMouseUp);
            PosArrow = 4; Draw(); Paint();
        }
    }
    
    if (e->What == evKeyDown) {
        if ((e->Key.KeyCode == kbDown) && (Current != Cnt-1)) {
            Current++;
            if (Current >= Delta + ScrLn) Delta = Current - (ScrLn-1);
            Draw(); Paint();
            if (FocusProc != NULL) FocusProc(this);
        }
        else if ((e->Key.KeyCode == kbUp) && (Current != 0)) {
            Current--;
            if (Delta > Current) Delta = Current;
            Draw(); Paint();
            if (FocusProc != NULL) FocusProc(this);
        }
        if ((e->Key.KeyCode == kbDown) || (e->Key.KeyCode == kbUp)) e->What = evNothing;
    }

	if (e->What == evMouseScroll) {
		Delta -= 3 * e->Mouse.Scroll;

		if (Delta + ScrLn > Cnt) {
			Delta = Cnt - ScrLn;
		}

		if (Delta < 0) {
			Delta = 0;
		}

		Draw();
		Paint();
	}

    return FALSE;
}











//////////////////////// TSingleInput:
    
TSingleInput::TSingleInput(int ax, int ay, char *aBuf, int aMC, int aMP)
             : TView(ax, ay, aMP + 20, 26)
{
    Buf = aBuf;
    MaxChars = aMC;
    MaxPixels = aMP;
    Modified = FALSE;
}



void TSingleInput::Draw() {
	BarBmp(DrwViewBf, DrwViewBfSzX, x, y, w, h, 85);
	RectBmp(DrwViewBf, DrwViewBfSzX, x, y, w, h, 87, 83);
	PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY, x+5, y+5, Buf,
		NormalFont, clrWhite, clrBlack);
	PutStr(DrwViewBf, DrwViewBfSzX, DrwViewBfSzY,
		x+5+GetStrWidth(Buf, NormalFont), y+5, "-",
		NormalFont, clrLightBlue, clrWhite);
}



int TSingleInput::HandleEvent(TEvent *e) {
	int i;

	if (e->What == evKeyDown) {
		if (e->Key.KeyCode == kbBack) {
			i = strlen(Buf);
			// 0x80-0xbff are UTF-8 sequence continuation bytes.
			// Delete the whole character sequence.
			while (i > 0 && (Buf[--i] & 0xc0) == 0x80) {
				Buf[i] = 0;
			}

			Buf[i] = 0;
		} else if (e->Key.KeyCode == kbEnter) {
		} else if ((e->Key.CharCode >= 32) && ((int)strlen(Buf) < MaxChars)) {
			PlaySample(TypeOnSnd2, 8, EffectsVolume, 128);

			if (Modified) {
				i = strlen(Buf);
				Buf[i] = e->Key.CharCode;
				Buf[i+1] = 0;
			} else {
				Buf[0] = e->Key.CharCode;
				Buf[1] = 0;
			}
		}

		if (GetStrWidth(Buf, NormalFont) > MaxPixels) {
			Buf[strlen(Buf)-1] = 0;
		}

		Modified = TRUE;
		Draw();
		Paint();
		return FALSE;
	} else {
		return TView::HandleEvent(e);
	}
}










/////////////////////////////// GLOBAL FCES /////////////////////////

int PromtBox(const char *text, int buttons)
{
    TDialog *dlg;
    int rtn, ax, as;

    if (buttons & cmAlways)
        dlg = new TDialog((RES_X-380)-4, (RES_Y-160)-4, 380, 160, "dlgprbx");
    else
        dlg = new TDialog((RES_X-380)/2, (RES_Y-160)/2, 380, 160, "dlgprbx");
    as = GetStrWidth(text, NormalFont);
    ax = (380 - as) / 2;
    dlg->Insert(new TStaticText(ax, 30, as, 40, text));
    if (buttons == (cmOk | cmCancel)) {
        dlg->Insert(new TButton(50, 100, SigText[TXT_OK], cmOk, TRUE));
        dlg->Insert(new TButton(200, 100, SigText[TXT_CANCEL], cmCancel));
    }
    else if (buttons == (cmYes | cmNo)) {
        dlg->Insert(new TButton(50, 100, SigText[TXT_YES], cmYes, TRUE));
        dlg->Insert(new TButton(200, 100, SigText[TXT_NO], cmNo));
    }
    else if (buttons == (cmYes | cmNo | cmAlways)) {
        dlg->Insert(new TButton(50, 60, SigText[TXT_YES], cmYes, TRUE));
        dlg->Insert(new TButton(200, 60, SigText[TXT_NO], cmNo));
        dlg->Insert(new TButton(50, 100, SigText[TXT_ALWAYS], cmAlways, TRUE));
    }
    else if (buttons == cmOk) {
        dlg->Insert(new TButton(125, 100, SigText[TXT_OK], cmOk, TRUE));
    }
    rtn = dlg->Exec();
    if ((rtn == cmCancel) && (buttons & cmYes)) rtn = cmNo;
    delete dlg; 
    return rtn;
}






int InputBox(const char *promt, char *buf, int maxchars)
{
    TDialog *dlg;
    int rt;
    
    dlg = new TDialog((RES_X - 542) / 2, (RES_Y-160)/2, 542, 160, "dlginput");
    dlg->Insert(new TStaticText(30, 20, 465, 16, promt));
    dlg->Insert(new TSingleInput(30, 50, buf, maxchars, 465));
    dlg->Insert(new TButton(465 - 245+20, 100, SigText[TXT_OK], cmOk, TRUE));
    dlg->Insert(new TButton(465 - 100+20, 100, SigText[TXT_CANCEL], cmCancel, TRUE));
    
    rt = dlg->Exec();
    delete dlg;
    return rt;
}

