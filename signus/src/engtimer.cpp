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

// wce_cmd: zu option


/*


    ENGINE  TIMER


*/

#include "headers.h"
#include "sound.h"
#include "global.h"
#include "graphio.h"
#include "engtimer.h"
#include "events.h"

#include <SDL_timer.h>


///////////////////////// Cachry s paletou a timerem:


int PulsarValue = 0, PulsarDirection = 1; // stav barvy clrPulsar
int PulsarTick;
int PulsarProcess = TRUE;

#define PULSAR_INC 10
#define PULSAR_SPD  2



static int Last_UP_Tick = 0;

void UpdatePalette()
{
    if (TimerValue == Last_UP_Tick) return;
    Last_UP_Tick = TimerValue;
    
    //pulzujici zelena:
    if (++PulsarTick % PULSAR_SPD == 0) {
        PulsarValue += (PulsarDirection == 1) ? PULSAR_INC : -PULSAR_INC;
        if (PulsarValue <= 20) {PulsarValue = 20, PulsarDirection = 1;}
        else if (PulsarValue >= 63) {PulsarValue = 63, PulsarDirection = 0;}
        Palette[3 * clrPulsar + 1] = PulsarValue;
#if 0 // FIXME -- to slow in SDL when not using 8bpp display...
        if (PulsarProcess) SetPalettePart(&(Palette[3 * clrPulsar]), clrPulsar, 3);
#endif
    }
}






#define TIMER_ID           1
#define TIMER_TIMEOUT     50

static SDL_TimerID   uTimerID = 0;
extern volatile int signus_suspended;
extern int MusicPlaying;
extern int IgnoreEvent;

volatile int palette_timer_on = FALSE;

Uint32 TimerFunction(Uint32 interval, void*)
{
    if (signus_suspended)
        return interval;

//    if (MusicOn) MD_Update();       FIXME --- not needed anymore ... (?)
    if (palette_timer_on)
    {
        TimerValue++;           // FIXME -- too fast?
        UpdatePalette();
    }
#if 0
    IgnoreEvent = TRUE;         // FIXME
    GetEvent(NULL);             // FIXME -- what the heck was it doing here????
    IgnoreEvent = FALSE;        // FIXME
#endif
    
    return interval;
}



void InitTimer()
{
    uTimerID = SDL_AddTimer(TIMER_RESOLUTION, TimerFunction, NULL);
}


void DoneTimer()
{
    SDL_RemoveTimer(uTimerID);
    uTimerID = 0;
}



void InitPalTimer()
{
    PulsarValue = 0;
    PulsarDirection = 1;
    PulsarTick = 0;
    palette_timer_on = TRUE;
}



void DonePalTimer()
{
    palette_timer_on = FALSE;
}
