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
// Hlavni modul programu SIGNUS
//
//
//
//

#include <time.h>
#include <climits>
#include <SDL_timer.h>

#include "mission_screen.h"
#include "events.h"
#include "fonts.h"
#include "global.h"
#include "system.h"
#include "engine.h"
#include "sound.h"
#include "ui_toolkit.h"
#include "ai.h"
#include "units.h"
#include "loadsave.h"
#include "anims.h"
#include "mouse.h"
#include "explode.h"
#include "shortcut.h"
#include "mainmenu.h"
#include "briefing.h"
#include "fields.h"
#include "engtimer.h"
#include "autofire.h"



/////////////////////////////// PROMNENE ////////////////////////////////


// Vlajka, podle niz se pozna ukonceni hry (v akci na mape):
int SignusTerminated = FALSE;
static int TerminationStatus = 0;


// Ukonceni jednoho kola:
int TurnEnded = FALSE;

// Cas posledni non-null udalosti v ticich:
int LastEvent;


// Panel s ikonkami:
TIconPanel *MainIcons;
// Hlavni menu:
TMenu *MainMenu;




////////////////////////////////////////////////////////////////////////////
// Vykonne jadro (inicializace a nacitani levelu je odbyto nize):





////////////////////////////////////////////////////////////////////////
// IDLE:

/////////////// hodinky na monitoru:

// Cas posledni kontroly casu:
int LastWatchTimer;
#define getsec(x) (x * TIMER_RESOLUTION / 1000)        /* FIXME */

void UpdateWatch()
{
    if (getsec(TimerValue) > getsec(LastWatchTimer)) {
        int delta = TimerValue - LastWatchTimer;
        TurnTime += delta, MissionTime += delta;
        LastWatchTimer = TimerValue;
        ShowTime(getsec(TurnTime), getsec(MissionTime));
    }
}





    

// animovane kurzory mysi:

static int AnimCurOldTimer = 0;
#define ANIM_CUR_SPD  10

void AnimateCursor()
{
    static int basecur[mcurCnt] = {0, 1, 2, 3, 2, 2, 3, 3};
    static int selmorph[4] = {4, 2, 5, 2};
    static int tarmorph[4] = {6, 3, 7, 3};
    static int phase = 0;
    static int cur;
    
    cur = basecur[Mouse.ActCur];
    if (cur == mcurSelect) {
        Mouse.SuperLocks++;
	MouseSetCursor(selmorph[phase++]);
        if (phase == 4) phase = 0;
        MousePaint();
        Mouse.SuperLocks--;
    }
    else if (cur == mcurTarget) {
        Mouse.SuperLocks++;
	MouseSetCursor(tarmorph[phase++]);
        if (phase == 4) phase = 0;
        MousePaint();
        Mouse.SuperLocks--;
    }
    else phase = 0;
    AnimCurOldTimer = TimerValue;
}



// Nic se nedeje => skrolovani citlive na pozici kurzoru:
void IdleEvent() {
	int ScX = 0, ScY = 0;
	static int Scrolled;

	ProcessMapAnim();

	// Obhospodareni skrolovani:
	if (Mouse.x < 8) {
		ScX = -1;
	} else if (Mouse.x > RES_X-8) {
		ScX = 1;
	}

	if (Mouse.y < 8) {
		ScY = -1;
	} else if (Mouse.y > RES_Y-8) {
		ScY = 1;
	}

	if ((ScX != 0) || (ScY != 0)) {
		if (GetShiftState()) {
			ScX *= VIEW_SIZE_X/2, ScY *= VIEW_SIZE_Y/2;
			ScrollTo(MapPos.x + ScX + ScY, MapPos.y - ScX + ScY);
			SDL_Delay(iniScrollDelay);
			Scrolled = FALSE;
		} else {
			ScrollRel(ScX, ScY);
			SDL_Delay(iniScrollDelay);
			Scrolled = TRUE;
		}
	} else if (Scrolled) {
		Scrolled = FALSE;
		UpdateLitMap();
		MouseFreeze(LITMAP_X, LITMAP_Y, LITMAP_SIZE, LITMAP_SIZE);
		DrawLitMap();
		MouseUnfreeze();
	}

	// Jukebox:
	if (TimerValue % (20 * 5) == 0) {
		if (MusicOn && (!IsMusicPlaying())) {
			JukeboxNext();
		}
	}

	if ((TimerValue % ANIM_CUR_SPD == 0) && (TimerValue > AnimCurOldTimer)) {
		AnimateCursor();
		MainIcons->Update();
	}

	// Idle texty u kurzoru mysi:
	if ((SelPos.x != 0xFF) && (TimerValue - LastEvent >= iniIdleDelay)) {
		TEvent e;
		void *buf, *buf2;
		int bufszx, bufszy;
		int xp, yp;
		int OMX, OMY, oldtv = TimerValue - 1;
		char itext[100];

		SelectedUnit->GetFieldInfo(SelPos.x, SelPos.y, itext);

		if (*itext != 0) {
			bufszx = GetStrWidth(itext, NormalFont) + 4;
			bufszy = GetStrHeight(itext, NormalFont);
			buf = memalloc(bufszx * bufszy);
			buf2 = memalloc(bufszx * bufszy);
			xp = 18; yp = Mouse.y - bufszy / 2;

			if (xp + Mouse.x + bufszx < VIEW_X_POS + VIEW_SX) {
				xp += Mouse.x;
			} else {
				xp = Mouse.x - bufszx - xp;
			}

			if (Mouse.ActCur == mcurArrow) {
				yp += 10;

				if (xp < Mouse.x) {
					xp += 18;
				}
			}

			GetBitmap(xp, yp, buf, bufszx, bufszy);
			memcpy(buf2, buf, bufszx * bufszy);
			DoDarking(buf, bufszx * bufszy);
			PutStr(buf, bufszx, bufszy, 2, 0, itext, NormalFont,
				clrWhite, clrBlack);
			PutBitmap(xp, yp, buf, bufszx, bufszy);
			OMX = Mouse.x;
			OMY = Mouse.y;

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

				UpdateWatch();

				if ((TimerValue % ANIM_CUR_SPD == 0) && (TimerValue > AnimCurOldTimer)) {
					AnimateCursor();
				}

				if (TimerValue != oldtv) {
					UpdateScreen();
					oldtv = TimerValue;
				}
			} while ((e.What == evNothing) ||
				((e.What == evMouseMove) &&
				IsInRect(e.Mouse.Where.x, e.Mouse.Where.y, OMX - 10, OMY - 10, OMX + 10, OMY + 10)));

			PutEvent(&e);
			LastEvent = TimerValue;
			PutBitmap(xp, yp, buf2, bufszx, bufszy);
			memfree(buf);
			memfree(buf2);
		}
	}

	// idle helpy u ikonek:
	if ((TimerValue - LastEvent >= 2*iniIdleDelay) && (MainIcons->Help())) {
		LastEvent = TimerValue;
	}

	UpdateWatch();
	UpdateScreen();
}











////////////////////////////////////////////////////////////////////////

// Zameri dalsi dobrou jednotku v seznamu:

int SelectNextUnit(int mark = FALSE)
{   
    int i, ID, oldID; 

    ID = SelectedUnit->ID;
    oldID = ID;

    for (i = 0; i < BADLIFE; i++)
        if ((Units[SelectionHistory[i]] != NULL) && 
            (Units[SelectionHistory[i]]->X != -1) &&
            ((!mark) || (Units[SelectionHistory[i]]->CanChoose))) break;
    ID = SelectionHistory[i];

    if (oldID == ID) return FALSE;
    if (mark) Units[oldID]->CanChoose = FALSE;
    Units[ID]->Select();
    return TRUE;
}


int SelectFirstGoodUnit()
{
    int i, j = 0;
    for (i = GOODLIFE_TOP-1; i >= 0; i--)
        if (Units[SelectionHistory[i]] != NULL) {
            j = SelectionHistory[i];
            if (Units[j]->X != -1) {
                Units[j]->Select();
                break;
            }
        }
    return j < BADLIFE;
}






////////////////////////////////////////////////////////////////////////
///////////////////////////// Menu:

// Nastavi hlasitost zvuku (DLG):

int ssvMusic, ssvSound, ssvSpeech;
MIDASsample ssvSnd;

void ssvUpdateGauge(int *Value, int Release)
{
    if (Value == &ssvMusic)
        SetVolume(ssvSound, ssvSpeech, ssvMusic);
    else if (Release) {
        if (Value == &ssvSound) {
            SetVolume(ssvSound, ssvSpeech, ssvMusic);
            PlaySample(ssvSnd, 10, EffectsVolume, 128);
        }
        else if (Value == &ssvSpeech) {
            SetVolume(ssvSound, ssvSpeech, ssvMusic);
            PlaySample(ssvSnd, 10, SpeechVolume, 128);
        }
    }
}


void SetSoundVolume()
{
    TDialog *dlg;
    
    ssvSnd = LoadSample("sndtest", FALSE);
    ssvMusic = iniMusicVol, ssvSound = iniSoundVol, ssvSpeech = iniSpeechVol;
    dlg = new TDialog(9+(VIEW_SX-420)/2, 36+(VIEW_SY-300)/2, 420, 160, "dlgsndvo");
    dlg->Insert(new TBarGauge(10, 10, 230, 20, &ssvMusic, 64, ssvUpdateGauge));
    dlg->Insert(new TStaticText(250, 12, 130, 20, SigText[TXT_SNDVOL_MUSIC]));
    dlg->Insert(new TBarGauge(10, 40, 230, 20, &ssvSound, 64, ssvUpdateGauge));
    dlg->Insert(new TStaticText(250, 42, 130, 20, SigText[TXT_SNDVOL_SOUND]));
    dlg->Insert(new TBarGauge(10, 70, 230, 20, &ssvSpeech, 64, ssvUpdateGauge));
    dlg->Insert(new TStaticText(250, 72, 130, 20, SigText[TXT_SNDVOL_SPEECH]));
    dlg->Insert(new TButton(10, 114, SigText[TXT_OK], cmOk, TRUE));
    dlg->Insert(new TButton(145, 114, SigText[TXT_CANCEL], cmCancel));

    if (dlg->Exec() == cmOk) {
        iniMusicVol = ssvMusic;
        iniSoundVol = ssvSound;
        iniSpeechVol = ssvSpeech;
        SetVolume(iniSoundVol, iniSpeechVol, iniMusicVol);
        SaveINI();
    }
    else {
        SetVolume(iniSoundVol, iniSpeechVol, iniMusicVol);
    }
    
    delete dlg;
    FreeSample(ssvSnd);
}




#if 0
static void soUpdateGaugePal(int *Value, int Release)
{
    int oldi = iniBrightCorr;
    iniBrightCorr = *Value;
    SetPalette(Palette);
    iniBrightCorr = oldi;   
}
#endif


void SetOptions()
{
    TDialog *dlg;
    int _EnhancedGuiOn = iniEnhancedGuiOn,
        _SOE = iniStopOnNewEnemy,
        _IdleD = 60 - iniIdleDelay,
        _ScrollD = 200 - iniScrollDelay,
        _AnimD = 200 - iniAnimDelay,
        _AnimD2 = 200 - iniAnimDelay2,
        _Brigh = iniBrightCorr;

    dlg = new TDialog(9+(VIEW_SX-490)/2, 36+(VIEW_SY-300)/2, 490, 300, "dlgopti");
//    dlg->Insert(new TStaticText(10, 36, 150, 16, SigText[TXT_BRIGHTNESS]));
//    dlg->Insert(new TBarGauge(10, 52, 310, 20, &_Brigh, 128, soUpdateGaugePal));
    dlg->Insert(new TStaticText(10, 80, 150, 16, SigText[TXT_SCROLL_DELAY]));
    dlg->Insert(new TBarGauge(10, 96, 310, 20, &_ScrollD, 200));
    dlg->Insert(new TStaticText(10, 120, 150, 16, SigText[TXT_IDLE_DELAY]));
    dlg->Insert(new TBarGauge(10, 136, 310, 20, &_IdleD, 60));
    dlg->Insert(new TStaticText(10, 160, 150, 16, SigText[TXT_MOVE_DELAY]));
    dlg->Insert(new TBarGauge(10, 176, 310, 20, &_AnimD, 200));
    dlg->Insert(new TStaticText(10, 200, 150, 16, SigText[TXT_ANIM_DELAY]));
    dlg->Insert(new TBarGauge(10, 216, 310, 20, &_AnimD2, 200));
    dlg->Insert(new TCheckBox(10, 245, 250, SigText[TXT_ENABLE_GUI], &_EnhancedGuiOn));
    dlg->Insert(new TCheckBox(10, 265, 250, SigText[TXT_STOP_ON_ENEMY], &_SOE));
    dlg->Insert(new TButton(340, 20, SigText[TXT_OK], cmOk, TRUE));
    dlg->Insert(new TButton(340, 60, SigText[TXT_CANCEL], cmCancel));
        
    if (dlg->Exec() == cmOk) {
        iniEnhancedGuiOn = _EnhancedGuiOn;
        iniStopOnNewEnemy = _SOE;
        iniIdleDelay = 60 - _IdleD;
        iniScrollDelay = 200 - _ScrollD;
        iniAnimDelay = 200 - _AnimD;
        iniAnimDelay2 = 200 - _AnimD2;
        iniBrightCorr = _Brigh;
      ApplyINI();
      SaveINI();
    }
    else {SetPalette(Palette); ApplyINI();}
    
    delete dlg;
}



// Provede menu a osetri vysledek:

#define mnCancel   0
#define mnSave     1
#define mnLoad     2
#define mnVolume   3
#define mnOptions  4
#define mnQuit     5
#define mnAbort    6

int quit_mission(void) {
	if (PromtBox(SigText[TXT_CONFIRM_QUIT], cmYes | cmNo) != cmYes)  {
		return 0;
	}

	SignusTerminated = TRUE;
	TerminationStatus = 666;

	if (PromtBox(SigText[TXT_SAVEMISSION], cmYes | cmNo) == cmYes) {
		SaveGame();
	}

	return 1;
}

void DoMenu()
{   
    int rtn = 0;
    
        MainMenu->Show();
        rtn = MainMenu->Exec();
        MainMenu->Hide();
        switch (rtn) {
            case mnQuit : 
                quit_mission();
                break;
            case mnVolume :
                SetSoundVolume();
                break;
            case mnOptions:
                SetOptions();
                break;
            case mnSave:
                SaveGame();
                break;
            case mnLoad:
                LoadGame();
                MainIcons->Draw();
                break;
        };
}







// zobrazi hlavni cile mise plus da moznost jit do briefinu

#define cmDetails     666

void DoBriefingPlus()
{
    char buf[300], buf2[20], *b2;
    
    TDialog *dlg = new TDialog(RES_X - 560, 78, 420, 282, "dlgbrief");
    dlg->Insert(new TButton(270, 230, SigText[TXT_DETAILS], cmDetails));
    sprintf(buf, SigText[TXT_BRIEFING], ActualMission, ActualTurn, MoneyGoodlife, GoodlifeDeads, BadlifeDeads);
    dlg->Insert(new TStaticText(10, 20, 100, 100, buf));
    sprintf(buf2, "objec%i", ActualMission);
    b2 = (char*)TextsDF->get(buf2);
    dlg->Insert(new TStaticText(20, 150, 100, 100, b2));
    
    if (dlg->Exec() == cmDetails) {
        char name[20];
        FadeOut(Palette, 0);
        sprintf(name, "brief%i", ActualMission);        
        BriefGo(name);        
        draw_mission_bezel(0);
        RedrawMap();
        SelectedUnit->Select();
        FadeIn(Palette, 0);
    }
    delete dlg;
    memfree(b2);
}










////////////////////////////////////////////
// setup toho, co se zobrazuje na mape:

#define VIS_SET_ITEMS      4
#define VIS_SET_X        (RES_X-64)

void DoVisSetup() {
	static int *ItemRels[VIS_SET_ITEMS] = {&iniShowStatusbar,
		&iniShowShootRange, &iniShowMoveRange, &iniShowVisibRange};

	void *Back;
	byte *Drw, *DOn, *DOff, *dum;
	int h = (3 + VIS_SET_ITEMS * 15);
	int i, y;
	TEvent e;
    
	Back = memalloc(54 * h);
	GetBitmap(VIS_SET_X, 252, Back, 54, h);
	Drw = (byte*) GraphicsI18nDF->get("vsettr0");
	PutBitmap(VIS_SET_X, 252, Drw, 54, h);  
	DOn = (byte *) GraphicsI18nDF->get("vsettr2");
	DOff = (byte*) GraphicsI18nDF->get("vsettr1");
    
	for (i = 0; i < VIS_SET_ITEMS; i++) {
		dum = *(ItemRels[i]) ? DOn : DOff;

		for (y = 0; y < 15; y++) {
			memcpy(Drw + 54 * (i * 15 + 1 + y),
				dum + 54 * (i * 15 + 1 + y), 54);
		}

		PutBitmap(VIS_SET_X, 252, Drw, 54, h);  
		UpdateScreen();
		SDL_Delay(100);
	}   

	while (TRUE) {
		GetEvent(&e);

		if (e.What == evMouseUp) {
			LastEvent = TimerValue;

			if (IsInRect(e.Mouse.Where.x, e.Mouse.Where.y,
				VIS_SET_X, 252, VIS_SET_X+54, 252+h)) {

				i = (e.Mouse.Where.y - 253) / 15;
				*(ItemRels[i]) = !(*(ItemRels[i]));
				dum = *(ItemRels[i]) ? DOn : DOff;

				for (y = 0; y < 15; y++) {
					memcpy(Drw + 54 * (i * 15 + 1 + y),
						dum + 54 * (i * 15 + 1 + y),
						54);
				}

				PutBitmap(VIS_SET_X, 252, Drw, 54, h);
				RedrawMap();
				UpdateScreen();
			} else {
				break;
			}
		} else if (e.What == evQuit) {
			PutEvent(&e);
			break;
		}
	}

	PutBitmap(VIS_SET_X, 252, Back, 54, h); 
	UpdateScreen();
	memfree(Back);
	memfree(Drw);
	memfree(DOn);
	memfree(DOff);
}






////////////////////////////////////////////////////////////////////////////
////////////////////////// Zpracovani udalosti: ////////////////////////////


// Seznam ikon:

void HandleIcons(TEvent *e)
{
    int rt = MainIcons->Handle(e);
    if (rt) {
        MainIcons->LightsOff();
        switch (rt) {
            case 1 : SelectNextUnit(FALSE);
                     break;
            case 3 : TurnEnded = TRUE;
                             break;
            case 4 : SelectedUnit->Center();
                             break;
            case 5 : DoBriefingPlus();
                             break;
            case 7 : DoMenu();
                             break;
            case 8 : JukeboxSetup();
                             break;
            case 9 : DoVisSetup();
                             break;
            case 6 : if (TimeReserve == 1) TimeReserve = 0; else TimeReserve = 1;
                             MainIcons->Draw();
                             SelectedUnit->Select();
                             break;
            case 10 : if (TimeReserve == 2) TimeReserve = 0; else TimeReserve = 2;
                             MainIcons->Draw();
                             SelectedUnit->Select();
                             break;
            case 2 : if (TimeReserve == 3) TimeReserve = 0; else TimeReserve = 3;
                             MainIcons->Draw();
                             SelectedUnit->Select();
                             break;
        }
    }
}






int SetUnitCursor(unsigned cur)
{
    if (SelPos.x == -1) return TRUE;
    unsigned x = SelectedUnit->GetFieldActions(SelPos.x, SelPos.y);
    if ((x & cur) != 0) {
        UnitActionType = cur;
        SelectField(0, 0);
        SelectField(Mouse.x, Mouse.y);
        return TRUE;
    }
    return FALSE;
}





/////////// handle event:


void HandleEvent(TEvent *e)
{   
    if (e->What == evMouseMove) { 
        SelectField(e->Mouse.Where.x, e->Mouse.Where.y);
    }

    else if (e->What == evMouseDown) {
        if (IsInRect(e->Mouse.Where.x, e->Mouse.Where.y,
                          LITMAP_X, LITMAP_Y, 
                              LITMAP_X + LITMAP_SIZE - 1, LITMAP_Y + LITMAP_SIZE - 1)) {
            // Rolovani s malou mapou pomoci mysi:
            if (e->Mouse.Buttons == mbRightButton) {
            }
            
            // Zamereni dane oblasti kliknutim:
            else if (e->Mouse.Buttons == mbLeftButton) {
                TEvent e2 = *e;
                do {
                    e2.Mouse.Where.x = Mouse.x; e2.Mouse.Where.y = Mouse.y; // zrhcleni
                    e2.Mouse.Where.x -= LITMAP_X, e2.Mouse.Where.y -= LITMAP_Y;
                    e2.Mouse.Where.x += LitMapOfs.x, e2.Mouse.Where.y += LitMapOfs.y;
                    e2.Mouse.Where.x -= (VIEW_SIZE_X+VIEW_SIZE_Y)/2;
                    e2.Mouse.Where.y -= (VIEW_SIZE_X+VIEW_SIZE_Y)/2-VIEW_SIZE_X;
                    ScrollTo(e2.Mouse.Where.x-1, e2.Mouse.Where.y-1);
                    do { 
                      GetEvent(&e2);
                    } while ((e2.What != evNothing) && (e2.What != evMouseUp));
                } while (!(e2.What == evMouseUp));
            }
        }
        

        // Udalosti kliknuti v okne mapy:
        else if (IsInRect(e->Mouse.Where.x, e->Mouse.Where.y,
                          VIEW_X_POS, VIEW_Y_POS, 
                              VIEW_X_POS + VIEW_SX - 1, VIEW_Y_POS + VIEW_SX - 1)) {
            // Prepinani na AlwaysShoot:
            if (e->Mouse.Buttons == mbRightButton) {
                SelectedUnit->SwitchActionType();
                SelectField(0, 0);
                SelectField(e->Mouse.Where.x, e->Mouse.Where.y);
            }

            // Akce nad jednotkami:
            else if (e->Mouse.Buttons == mbLeftButton) {
                if ((SelPos.x != 0xFF) && (SelPos.y != 0xFF) && 
                    (GetField(SelPos.x, SelPos.y)->Visib != 0)) {
                    TEvent e;
                    SelectedUnit->Action(SelPos.x, SelPos.y);
                    do {GetEvent(&e);} while (e.What != evNothing);
                }
            }
        }


        // Udalosti kliknuti na infobox pro jednotky:
        else if (IsInRect(e->Mouse.Where.x, e->Mouse.Where.y,
                          UINFO_X, UINFO_Y, 
                              UINFO_X + UINFO_SX - 1, UINFO_Y + UINFO_SY - 1)) {
            SelectedUnit->InfoEvent(e);
        }

        // Udalosti kliknuti na ikony:
        else if (IsInRect(e->Mouse.Where.x, e->Mouse.Where.y,
                 RES_X-118, 126, RES_X, 251)) 
            HandleIcons(e);
    }

    

    // Udalosti z klavesnice:   
    else if (e->What == evKeyDown) {
        HandleIcons(e);
        SelectedUnit->InfoEvent(e);
        
        switch (e->Key.KeyCode) {
            case kbF11 : 
                if (getenv("HELLMASTER")) {
                    SignusTerminated = TRUE; TerminationStatus = 1; 
                }
                break;

            case kbF9 : if (getenv("HELLMASTER")) {
                int xxx, yyy;
                for (xxx = 0; xxx < MapSizeX; xxx++)
                    for (yyy = 0; yyy < MapSizeY; yyy++)
                        GetField(xxx, yyy)->Visib = 2;
                RedrawMap();
                }
                break;

            case kbUp : if (GetShiftState())
                ScrollTo(MapPos.x-VIEW_SIZE_Y/2, MapPos.y-VIEW_SIZE_Y/2);
                else ScrollTo(MapPos.x-2, MapPos.y-2); 
                ClearEvent();
                break;
            case kbDown : if (GetShiftState())
                ScrollTo(MapPos.x+VIEW_SIZE_Y/2, MapPos.y+VIEW_SIZE_Y/2);
                else ScrollTo(MapPos.x+2, MapPos.y+2); 
                ClearEvent();
                break;
            case kbLeft : if (GetShiftState())
                ScrollTo(MapPos.x-VIEW_SIZE_X/2, MapPos.y+VIEW_SIZE_X/2);
                else ScrollTo(MapPos.x-2, MapPos.y+2); 
                ClearEvent();
                break;
            case kbRight : if (GetShiftState())
                ScrollTo(MapPos.x+VIEW_SIZE_X/2, MapPos.y-VIEW_SIZE_X/2);
                else ScrollTo(MapPos.x+2, MapPos.y-2); 
                ClearEvent();
                break;      

            case SHORTCUT_SAVE : 
                SelectField(0, 0);
                SaveGame(); 
                SelectField(Mouse.x, Mouse.y);
                do {GetEvent(e);} while (e->What != evNothing);
                break;
            case SHORTCUT_LOAD : 
                SelectField(0, 0);
                LoadGame(); 
                MainIcons->Draw();
                SelectField(Mouse.x, Mouse.y);
                do {GetEvent(e);} while (e->What != evNothing);
                break;
            case SHORTCUT_QUIT : 
                quit_mission();
                break;
        
            default :
                switch (e->Key.CharCode) {
			// FIXME
                    case SHORTCUT_STATUSBAR :/* case SHORTCUT_STATUSBAR+32 :*/
                        iniShowStatusbar = !iniShowStatusbar;
                        RedrawMap();
                        break;
                    case SHORTCUT_SELCURSOR :
                        if (!SetUnitCursor(uatSelect)) SetUnitCursor(uatSelectAir);
                        break;
                    case SHORTCUT_MOVECURSOR :
                        if (!SetUnitCursor(uatMove)) SetUnitCursor(uatMoveAir);
                        break;
                    case SHORTCUT_ATTACKCURSOR :
                        if (!SetUnitCursor(uatAttack)) if (!SetUnitCursor(uatAttackGood))
                        if (!SetUnitCursor(uatAttackAir)) if (!SetUnitCursor(uatAttackAirG))
                        SetUnitCursor(uatAttackField);
                        break;
                    case SHORTCUT_SPECIALCURSOR :
                        if (!SetUnitCursor(uatOccupy)) if (!SetUnitCursor(uatLoadIn)) 
                        SetUnitCursor(uatSupport);
                        break;
			// FIXME
                    case SHORTCUT_NEXTUNIT2 : /*case SHORTCUT_NEXTUNIT2+32 :*/ 
                        SelectNextUnit(TRUE);
                        break;
                }
        }       
    } else if (e->What == evQuit) {
	if (quit_mission()) {
		PutEvent(e);
	}
    }

    UpdateScreen();
}













/////////////////////////////////////////////////////////////////////////////

// Inicializuje hru a zkontroluje, zda jsou vsechny potrebne
// veci pritomny:

void InitRndGenerator()
{
    srand(time(NULL));
}



int InitSignus()
{
    InitRndGenerator();
/*    if (!InitGlobal()) {
        MessageBox(NULL, "Initialization failed.", "CRITICAL ERROR", MB_OK | MB_ICONERROR);
        return FALSE;
    }*/       // --- this is done in WINMAIN()
    {
        if (!InitVideo()) {
            return FALSE;
        }
        Palette = (uint8_t*) GraphicsDF->get("palette");
        memset(PaletteSDL, 0, sizeof(PaletteSDL));
        for (unsigned i = 0; i < 256; i++) {
            PaletteSDL[i].r = Palette[i*3+0] << 2;
            PaletteSDL[i].g = Palette[i*3+1] << 2;
            PaletteSDL[i].b = Palette[i*3+2] << 2;
        }
        lockmem(Palette, 768);
        SetPalette(Palette);
    }

    set_window_icon();
    MouseInit();
    if (!InitSound()) {
        DoneVideo();
        fprintf(stderr, "Error: Sound initialization failed.\n");
        return FALSE;
    }
    InitInteract();
    InitTimer();
    return TRUE;
}






/////////////////////////////////////////////////////////////////////////////

// Zajisti beh programu


#ifdef DEBUG
//extern int SaveGameDebug();
#endif

extern int WC_On;
extern void *WorkingControl[3];


void CrashSave() {
	File stream;
	char *path;

	path = signus_config_path("crashguard_saved_state");

	if (path) {
		stream.open(path, File::WRITE | File::TRUNCATE);
		memfree(path);
	}

	if (!stream.isOpen()) {
		fprintf(stderr, "Warning: cannot create crash save %s\n", path);
		return;
	}

	SaveGameState(stream);
}

int CrashLoad() {
	File stream;
	char *path;

	path = signus_config_path("crashguard_saved_state");

	if (path) {
		stream.open(path, File::READ);
		memfree(path);
	}

	if (!stream.isOpen()) {
		return FALSE;
	}

	LoadGameState(stream);
	return TRUE;
}


int IsEnemyTurn = FALSE;

extern int ConAF_always;
extern TUnit *ConAF_unit;

void TurnEnd()
{
    int i, j, oldTRe;

#ifdef DEBUG
    //SaveGameDebug();
#endif
    if (MinesCnt > MAX_MINES_CNT) MinesCnt = 0;
    CrashSave();
    MouseHide();
    HideHelpers(); 
    UpdateWatch();

    // redraw. mainscrean to ensure there is NO fucking cmaranice na screenu
    draw_mission_bezel(1);
    UpdateLitMap(TRUE);
    RedrawMap();
    if (SelectedUnit) SelectedUnit->Select();
    MainIcons->Draw();
    UpdateScreen();

    UnitInfoLock++;
    for (i = 0; i < MapSizeX; i++)
        for (j = 0; j < MapSizeY; j++)
            FieldTurnEnd(i, j);
    for (i = BADLIFE; i < UNITS_TOP; i++)
        if (Units[i] != NULL) Units[i]->TurnReset();
    WC_On = TRUE;   
    PlaceSubmarines(TRUE);
    oldTRe = TimeReserve;   TimeReserve = 0;
    Message(SigText[60]);
    ResetAutofire(BADLIFE);
    IsEnemyTurn = TRUE;
    ConAF_always = FALSE;
    ConAF_unit = NULL;
    TerminationStatus = ArtificialIntelligence();   DrawLocks = 0;
    IsEnemyTurn = FALSE;
    if (MinesCnt > MAX_MINES_CNT) MinesCnt = 0;
    TimeReserve = oldTRe;
    RemoveSubmarines();
    SignusTerminated = TerminationStatus != 0;

    for (i = GOODLIFE; i < GOODLIFE_TOP; i++)
        if (Units[i] != NULL) Units[i]->TurnReset();
    { //zniceni letadel, kterym doslo palivo
        for (i = 0; i < AircraftsCnt; i++)
            if (Aircrafts[i]->Fuel == 0) {
                Aircrafts[i]->Destroy();
                DoExplosion();
            }
    }
    Message(SigText[56]);
    ResetAutofire(GOODLIFE);
    Message("");
    UnitInfoLock--;
    WC_On = FALSE; PutBitmap(RES_X-268, RES_Y+2-((iniResolution == SVGA_640x480) ? 18 : 22), WorkingControl[0], 8, 8);

    ActualTurn++;

    ComputeVisib();
    RedrawMap();
    if (!SelectFirstGoodUnit()) SignusTerminated = TRUE;
    UpdateLitMap(TRUE);
    MouseFreeze(LITMAP_X, LITMAP_Y, LITMAP_SIZE, LITMAP_SIZE);
    DrawLitMap();
    MouseUnfreeze();
    ShowHelpers();
    UpdateWatch();
    TurnTime = 0;
    UpdateWatch();
    MouseShow();
    UpdateScreen();
    {
        TEvent e;
        do {GetEvent(&e);} while (e.What != evNothing);
    }
}




extern void Clear_PSmp();

int RunSignus(int from_save)
                             // param from_save znamena, ze se nemusi init engine, ai
                             // a nedela se briefing a animacky
{
    TEvent Event;

    MainIcons = new TIconPanel();
    MainMenu = new TMenu();

    if (!from_save) {
        char name[20];
        
        ShowMissionIntroScreen(ActualMission);
        sprintf(name, "brief%i", ActualMission);
        BriefGo(name);
        sprintf(name, "mis%iin", ActualMission);
        PlayAnimation(name);
        SetPalette(Palette);
        if (!InitEngine(ActualMission)) return FALSE;
        Clear_PSmp();
        HideHelpers();
        InitArtificialIntelligence(ActualMission);
        ShowHelpers();
        SaySpeech(name, 2000);
    }
    MainIcons->Draw();
    
    if (MusicOn && (!IsMusicPlaying())) JukeboxNext();

    LastEvent = LastWatchTimer = TimerValue;
    UnitActionType = uatNothing;
    SignusTerminated = FALSE, TerminationStatus = 0;

    PulsarProcess = TRUE;
    SetPalette(Palette);
    // redraw. mainscrean to ensure there is NO fucking cmaranice na screenu
    draw_mission_bezel(1);
    UpdateLitMap(TRUE);
    RedrawMap();
    if (SelectedUnit) SelectedUnit->Select();
    MainIcons->Draw();

   // Hlavni smycka:
   for (SignusTerminated = FALSE; SignusTerminated == FALSE;) {
    for (TurnEnded = FALSE;
        (SignusTerminated == FALSE) && (TurnEnded == FALSE);) {
      if (SelectedUnit == NULL) {
        SignusTerminated = TRUE;
        break;
      }
        GetEvent(&Event);
        if (Event.What == evNothing) 
            IdleEvent();
        else {
            LastEvent = TimerValue;
            HandleEvent(&Event);
        }
    }
    if (SignusTerminated) break;
    TurnEnd();
  }
    DoneArtificialIntelligence(); 
    DoneEngine();

    delete MainIcons;
    delete MainMenu;

  // vyhodnoceni ukonceni:
  if (TerminationStatus == -1) { // neuspech
    SaySpeech("failed", 9000);      
    if (PromtBox(SigText[TXT_UNSUCCESSFULL], cmYes | cmNo) == cmYes) 
        return TRUE;
    else 
        return FALSE;
  } 
  else if (TerminationStatus == 1) { // uspech
        char name[20];

        SaySpeech("success", 9000);
        sprintf(name, "debrf%i", ActualMission);
        if (ActualMission != 19) {
            MouseSetCursor(mcurArrow);
            FadeOut(Palette, 0);
            BriefGo(name);
	    ClearScr();
	    UpdateScreen();
        }
        
        if (ActualMission == 6) {   // zacina valka
            PlayAnimation("war_on");
        }
        
        if (ActualMission == 19) {  // konec hry
            PlayAnimation("outro");
            return FALSE;
        }
        
        ActualMission++;
        return TRUE;
  }

    return FALSE;
}






/////////////////////////////////////////////////////////////////////////////

// Vypne Signus

void DoneSignus()
{
    SaveINI();
    DoneTimer();
    DoneInteract();
    DoneSound();
    MouseDone();
    {
        unlockmem(Palette, 768);
        free(Palette);
        DoneVideo();
    }
    DoneGlobal();
}







/////////////////////////////////////////////////////////////////////////////

// Hlavni procedura:


void signus_main() {
	int result = 0;
	int crash = CrashLoad();
	int fs = TRUE;
	TEvent e;

	if (!crash) {
		PlayAnimation("present2");
		PlayAnimation("present1");
		PlayAnimation("present3");
		PlayAnimation("intro");
	}

	while (result != 3) {
		if (crash) {
			result = 666;
		} else {
			result = DoMainMenu();
		}

		switch (result) {
		case 0:
			PlayAnimation("intro2");
			ActualMission = 1;
			if (getenv("HELLMASTER")) {
				sscanf(getenv("HELLMASTER"), "%i", &ActualMission);
			}

			while (RunSignus(FALSE));
			break;

		case 1: ActualMission = -1;
			if (!LoadGame()) {
				break;
			}

			fs = TRUE;

			while (RunSignus(fs)) {
				fs = FALSE;
			}

			break;

		case 666:     // signus nacten v CrashLoad() --> server chce rovnou spustit AI...
			fs = TRUE;
			crash = FALSE;
			e.What = evKeyDown;
			e.Key.CharCode = SHORTCUT_ENDTURN; //->causes AI running
			PutEvent(&e);

			while (RunSignus(fs)) {
			    fs = FALSE;
			}

			break;

		case 2:
			ShowCredits();
			break;

		case 3:
			break;
		}
	}
}
