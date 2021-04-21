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
// Modul pro klavesnici a mys
//


#include <SDL.h>
#include <cctype>
#include "events.h"
#include "graphio.h"
#include "system.h"
#include "global.h"
#include "mouse.h"
#include <time.h>

//////////////////////////// GLOBALNI FCE ////////////////////////////////


/////////////// Screenshots....

void SaveScreenshot()
{
    FILE *f;
    char jm[400];
    int i;
    const char *home = getenv("HOME");

    for (i = 0; i < 10000; i++) {
        if (home)
            sprintf(jm, "%s/signus-%04i.bmp", home, i);
        else
            sprintf(jm, "./signus-%04i.bmp", i);
        f = fopen(jm, "rb");
        if (f == NULL) break;
        fclose(f);
    }

    SDL_SaveBMP(GetScreenSurface(), jm);
}




////////////////////////////// UDALOSTI //////////////////////////////////

int IsPuttedEvent = FALSE;
TEvent PuttedEvent;

void PutEvent(TEvent *e)
{
    IsPuttedEvent = TRUE;
    PuttedEvent = *e;
}



static TEvent null_event;
int IgnoreEvent = FALSE;

static bool needsMouseRedraw = false;

char key2ascii(const SDL_Keysym &key) {
	int caps = 0, ascii = key.sym;

	if (key.mod & (KMOD_CTRL | KMOD_ALT | KMOD_MODE)) {
		return 0;
	}

	if (key.mod & KMOD_CAPS) {
		caps = 1;
	}

	if (key.mod & KMOD_SHIFT) {
		caps = !caps;
	}

	if (key.mod & KMOD_NUM) {
		switch (ascii) {
		case SDLK_KP_0:
			ascii = '0';
			break;
		case SDLK_KP_1:
			ascii = '1';
			break;
		case SDLK_KP_2:
			ascii = '2';
			break;
		case SDLK_KP_3:
			ascii = '3';
			break;
		case SDLK_KP_4:
			ascii = '4';
			break;
		case SDLK_KP_5:
			ascii = '5';
			break;
		case SDLK_KP_6:
			ascii = '6';
			break;
		case SDLK_KP_7:
			ascii = '7';
			break;
		case SDLK_KP_8:
			ascii = '8';
			break;
		case SDLK_KP_9:
			ascii = '9';
			break;
		}
	}

	switch (ascii) {
	case SDLK_KP_DIVIDE:
		ascii = '/';
		break;
	case SDLK_KP_MULTIPLY:
		ascii = '*';
		break;
	case SDLK_KP_MINUS:
		ascii = '-';
		break;
	case SDLK_KP_PLUS:
		ascii = '+';
		break;
	}

	if (ascii >= ' ' && ascii <= 127) {
		if (caps && islower(ascii)) {
			ascii = toupper(ascii);
		}

		return ascii;
	}

	return 0;
}

int clamp(int value, int min, int max) {
	value = value > min ? value : min;
	return value < max ? value : max;
}

void GetEvent(TEvent *e) {
	SDL_Event event;

	if (e && IsPuttedEvent) {
		*e = PuttedEvent;
		IsPuttedEvent = FALSE;
		return;
	}

	if (!e) {
		e = &null_event;
	}

	if (!SDL_PollEvent(&event)) {
		e->What = evNothing;

		if (needsMouseRedraw) {
			if (!Mouse.SuperLocks) {
				MousePaint();
			}

			needsMouseRedraw = false;
		}

		// Let the kernel switch to other process, so that we don't consume 
		// nearly 100% of CPU power (this is ideal place for it, since GetEvent
		// is called often and we do it only if there weren't any new
		// events in the queue):
		SDL_Delay(1);
		return;
	}

	switch (event.type) {
#if 0
	//case SDL_ACTIVEEVENT:       FIXME
	    if (FullBuf != NULL) { // draw eng.scr.
	        char buf[20];
	        void *ptr;

	        sprintf(buf, "%imainscr", iniResolution - 0x100);
	        ptr = GraphicsDF->get(buf);       
	        MouseHide();
	        DrawPicture(ptr); 
	        MouseShow();
	        memfree(ptr);
	        UpdateLitMap(TRUE);
	        RedrawMap();
	        if (SelectedUnit) SelectedUnit->Select();
	    }
	    signus_suspended = FALSE;
	}
	else {
	    signus_suspended = TRUE;
	}
#endif

	//case SDL_QUIT:              FIXME

	case SDL_KEYDOWN:
		e->What = evKeyDown;
		e->Key.KeyCode = event.key.keysym.sym;
		e->Key.CharCode = key2ascii(event.key.keysym);
		            // FIXME -- use Unicode for CharCode !!!

		// Special handling:
		if (e->Key.KeyCode == kbF8 && getenv("HELLMASTER") != NULL) {
			SaveScreenshot();
		} else if (e->Key.KeyCode == SDLK_f &&
			(event.key.keysym.mod & KMOD_CTRL)) {
			ToggleFullscreen();
		}

		break;

	//case SDL_KEYUP:
	//        break;

	case SDL_MOUSEMOTION:
		e->What = evMouseMove;
		e->Mouse.Where.x = clamp(event.motion.x, 0, RES_X);
		e->Mouse.Where.y = clamp(event.motion.y, 0, RES_Y);
		e->Mouse.Buttons = 0;
		e->Mouse.Scroll = 0;

		// update global mouse information and repaint it:
		Mouse.buttons = event.motion.state;
		Mouse.x = e->Mouse.Where.x;
		Mouse.y = e->Mouse.Where.y;
		needsMouseRedraw = true;
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (!IsInRect(event.button.x, event.button.y, 0, 0, RES_X - 1,
			RES_Y - 1)) {
			e->What = evNothing;
			break;
		}

		e->What = evMouseDown;
		e->Mouse.Where.x = event.button.x;
		e->Mouse.Where.y = event.button.y;
		e->Mouse.Buttons = event.button.button;
		e->Mouse.Scroll = 0;
		break;

	case SDL_MOUSEBUTTONUP:
		if (!IsInRect(event.button.x, event.button.y, 0, 0, RES_X - 1,
			RES_Y - 1)) {
			e->What = evNothing;
			break;
		}

		e->What = evMouseUp;
		e->Mouse.Where.x = event.button.x;
		e->Mouse.Where.y = event.button.y;
		e->Mouse.Buttons = event.button.button;
		e->Mouse.Scroll = 0;
		break;

	case SDL_MOUSEWHEEL:
		e->What = evMouseScroll;
		e->Mouse.Where.x = Mouse.x;
		e->Mouse.Where.y = Mouse.y;
		e->Mouse.Buttons = 0;
		e->Mouse.Scroll = event.wheel.y;

		if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
			e->Mouse.Scroll *= -1;
		}

		break;

	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
			redraw_screen();
		}

		e->What = evNothing;
		break;

	default:
		e->What = evNothing;
		break;
	}
}



void ClearEvent()
{
    TEvent e;
    
    while (TRUE) {
        GetEvent(&e);
        if (e.What == evNothing) break;
    }
}





volatile byte ShiftState = FALSE;


