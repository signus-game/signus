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



#include "headers.h"
#include "events.h"
#include "graphio.h"
#include "engtimer.h"
#include "engine.h"
#include "units.h"
#include <stdarg.h>

#include <unistd.h>


#define CAPTION "Signus: The Artefact Wars"



static int SDL_inited          = FALSE;
static int signus_inited       = FALSE;
volatile int signus_suspended  = FALSE;
volatile int allow_mouse       = FALSE;
unsigned signus_thread_id      =     0;
volatile int signus_running    = FALSE;

void *Signus_BackupScr = NULL;


// Destrukce wokenich objektu:

extern void DoneSignus();



static void finiObjects()
{
    char b[100];

    allow_mouse = FALSE;
    if (signus_inited) DoneSignus();
    signus_inited = FALSE;
    sprintf(b, "%s/crashguard_saved_state", getSignusConfigDir());
    remove(b);
    
    if (SDL_inited)
    {
        SDL_Quit();
    }
}

// Initializer:
extern int InitSignus();

static bool doInit()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) == -1)
        goto badtimes_label;
    SDL_inited = TRUE;
    
    SDL_WM_SetCaption(CAPTION, NULL/*FIXME*/);
    SDL_ShowCursor(0);

    if (InitSignus()) {
        signus_inited = TRUE;
        allow_mouse = TRUE;
        return TRUE;
    }

badtimes_label:
    finiObjects();
    return FALSE;
}




/////////////////////////////////////////////

/////// thread-management:

extern void signus_main();

#define STACK_SIZE                8192
int signus_thread_is_running   = FALSE;

static void signus_thread(void *)
{
    signus_thread_is_running = FALSE;
    signus_running = TRUE;
    signus_main();
    signus_running = FALSE;
    signus_thread_is_running = FALSE;
    finiObjects();
}



//////////////////////////////////////////////////////////////////////////////////////////

// MAIN THREAD-EXECUTOR:


extern int InitGlobal();

int main(int argc, char *argv[])
{
    bool flagDebug = argc > 1 && strcmp(argv[1], "--debug") == 0;
    bool flagServer = argc > 1 && strcmp(argv[1], "--from-crashguard") == 0;

#if 0 // FIXME
    if (!flagDebug && !flagServer)
    {
        // execute server
        execlp("signus-crashguard", "signus-crashguard", argv[0], NULL);
        // ...if execution of the wrapper fails, continue as if --from-crashguard
        // was given...
    }
#endif

    if (!InitGlobal()) return FALSE;
    
    if (!doInit()) return FALSE;

    signus_thread_is_running = TRUE;
    signus_thread(NULL);
    
    return 0;
}



