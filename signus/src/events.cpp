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
// Modul pro klavesnici a mys
//


#include "headers.h"

#include <SDL.h>
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
    char *home = getenv("HOME");

    for (i = 0; i < 10000; i++) {
        if (home)
            sprintf(jm, "%s/signus-%04i.bmp", home, i);
        else
            sprintf(jm, "./signus-%04i.bmp", i);
        f = fopen(jm, "rb");
        if (f == NULL) break;
        fclose(f);
    }

    SDL_SaveBMP(SDL_GetVideoSurface(), jm);
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

void GetEvent(TEvent *e)
{
    if (e && IsPuttedEvent) {
        *e = PuttedEvent;
        IsPuttedEvent = FALSE;
    }
    else
    {
        if (!e) e = &null_event;

        if (!IgnoreEvent)  // FIXME - what the hell?!
        {
            SDL_Event event;
            if (!SDL_PollEvent(&event))
            {
                e->What = evNothing;
                if (needsMouseRedraw)
                {
                if (!Mouse.SuperLocks) MousePaint();
                needsMouseRedraw = false;
                }

                // Let the kernel switch to other process, so that we don't consume 
                // nearly 100% of CPU power (this is ideal place for it, since GetEvent
                // is called often and we do it only if there weren't any new
                // events in the queue):
                SDL_Delay(1);
            }
            else
            {
                switch (event.type)
                {
#if 0
                    // case SDL_ACTIVEEVENT:       FIXME
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

                    // case SDL_VIDEOEXPOSE:       FIXME
                    // case SDL_QUIT:              FIXME

                    case SDL_KEYDOWN:
                            e->What = evKeyDown;
                            e->Key.KeyCode = event.key.keysym.sym;
                            e->Key.CharCode = event.key.keysym.sym;
                                        // FIXME -- use Unicode for CharCode !!!

                            // Special handling:
                            if (e->Key.KeyCode == kbF8 && getenv("HELLMASTER") != NULL)
                                SaveScreenshot();
                            else if (e->Key.KeyCode == SDLK_f && 
                                    (event.key.keysym.mod & KMOD_CTRL))
                                ToggleFullscreen();
                            break;

                    //case SDL_KEYUP:
                    //        break;

                    case SDL_MOUSEMOTION:
                            e->What = evMouseMove;
                            e->Mouse.Where.x = event.motion.x;
                            e->Mouse.Where.y =  event.motion.y;
                            e->Mouse.Buttons = 0;

                            // update global mouse information and repaint it:
                            Mouse.buttons = event.motion.state;
                            Mouse.x = event.motion.x;
                            Mouse.y = event.motion.y;
                            needsMouseRedraw = true;
                            break;

                    case SDL_MOUSEBUTTONDOWN:
                            e->What = evMouseDown;
                            e->Mouse.Where.x = event.motion.x;
                            e->Mouse.Where.y =  event.motion.y;
                            e->Mouse.Buttons = event.button.button;
                            break;

                    case SDL_MOUSEBUTTONUP:
                            e->What = evMouseUp;
                            e->Mouse.Where.x = event.motion.x;
                            e->Mouse.Where.y =  event.motion.y;
                            e->Mouse.Buttons = event.button.button;
                            break;

                    default:
                            e->What = evNothing;
                }
            }
        }
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


