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
// Hlavickovy soubor pro SOUND.CPP
//
// Obsah: inicializace zvuk.karty a prehravani zvuku + hudby
//



#ifndef _SOUND_H
#define _SOUND_H

#include "system.h"

#include <SDL_mixer.h>

// nejake veci navic: (prechod midas->wss)
#define MIDASsamplePlayHandle       int /*channel number*/
#define MIDASsample                 unsigned
#define INVALID_SAMPLE              0 /* WAS "NULL" */
#define INVALID_SAMPLE_HANDLE       (-1)


// kanaly:
#define MUSIC_CHANNELS       1
#define EFFECT_CHANNELS      4
#define SPEECH_CHANNELS      1
#define TOTAL_CHANNELS       (MUSIC_CHANNELS + EFFECT_CHANNELS + SPEECH_CHANNELS)

#define MUSIC_CHANNEL        4
#define SPEECH_CHANNEL       5


#define PLAY_LOOPING         1

// Inicializacni rutiny:

extern int InitSound();
extern int DoneSound();



// promnnee: ----------------------------------------------------------

extern int MIDAS_disabled;
        // vypnuti/zapnuti midase (flag):

extern int MusicVolume, SpeechVolume, EffectsVolume;
        // hlasitosti jednotlivych prvku (0..64)
extern int MusicOn;
        // is music played><






// fce: ----------------------------------------------------------------

extern void OpenChannels(int music, int sfx);
        // vytvori dany pocet audiokanalu, z toho automatic nastavi
        // jako auto-efektove 
extern void CloseChannels();
        // zavre otevrene kanaly

extern void SetVolume(int effects, int speech, int music);
        // nastavi hlasitosti





// hudba: -------

extern int PlayMusic(char *name);
        // prehraje hudbu daneho jmena. Nejprve zkousi v music.dat, potom
        // to jmeno jakozto soubor...
extern void StopMusic();
        // vypne hudbu
extern int IsMusicPlaying();
        // vraci true, pokud hraje hudba




// samply: ------


extern MIDASsample LoadSample(char *name, int loop);
        // nacte sample (pokud uz je v pameti, jenom zvysi citac
        // pouzivanosti

extern void FreeSample(MIDASsample sample);
        // uvolni z pameti sample (nebo snizi citac)
        
extern MIDASsamplePlayHandle PlaySample(MIDASsample sample,
          int priority, unsigned volume, int panning);
    // zacte prehravat sample
    
extern void StopSample(MIDASsamplePlayHandle sample);
        // zastavi prehravani zvuku
        
extern void PanSample(MIDASsamplePlayHandle sample, int panning);
        // meni smer panningu por zvuk

extern void FadeOutSample(MIDASsamplePlayHandle sample, unsigned volume);
extern void FadeInSample(MIDASsamplePlayHandle sample, unsigned volume);
        // fadein / fadeout sample


// speech: --------

extern void SaySpeech(char *name, int priority);
        // rekne to ten urcity sample 

        




// jukebox: -----

#include "jukebox.h"


#endif
