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

// Rozhrani mezi Signusem a MikModem - zvuky a hudba

#include "headers.h"

#include <malloc.h>
#include <math.h>
#include "global.h"
#include "sound.h"



int MIDAS_disabled = FALSE; // FIXME

int EffectsVolume, SpeechVolume, MusicVolume;

int MusicOn;



static MIDASsamplePlayHandle speech_handle = 0;
static int speech_handle_used = 0;
static int speech_priority = 0;
static MIDASsample speech_sample = 0;



// sekce popisujici prehravany modul:

Mix_Music   *PlayedModule = NULL;   // modul
int          MusicPlaying = FALSE;   // prehrava se vubec?



///////////////////////////// INIT & DONE /////////////////////////////////


void InitSamplesLoader();


int InitSound()
{
    speech_handle_used = 0;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
        return FALSE;

    OpenChannels(MUSIC_CHANNELS, EFFECT_CHANNELS);
    InitSamplesLoader();

    InitJukebox();
    SetVolume(iniSoundVol, iniSpeechVol, iniMusicVol);
    return TRUE;
}



int DoneSound()
{
    if (MusicPlaying) StopMusic();
    CloseChannels();
    DoneJukebox();
    return TRUE;
}





static int snd_music_channels = 0;

void OpenChannels(int music, int sfx)
{
    if (MIDAS_disabled) return;
    Mix_AllocateChannels(sfx + 2);
        // FIXME ? -- does SDL_mixer use one channel for MikMod or not?
}



void CloseChannels()
{
    if (MIDAS_disabled) return;
    Mix_CloseAudio();
}








///////////////////////////// STD ROUTINES ////////////////////////////////



void SetVolume(int effects, int speech, int music)
{
    if (MIDAS_disabled) return;

    EffectsVolume = effects;
    SpeechVolume = speech;
    MusicVolume = music;
    if (MusicOn && (music == 0)) StopMusic();
    MusicOn = MusicVolume > 0;
     // FIXME __ update music channel volume here
     //          update ALL channels information1!!
}







/////////////////////////// MUSIC ROUTINES ////////////////////////////////


static int MusicJustStarted = FALSE;

int PlayMusic(char *name)
{
    char filnm[0124];
    FILE *f;
    int size, pos;
    void *ptr;

    if (MIDAS_disabled) return TRUE;

    if (!MusicOn) return TRUE;
    if (IsMusicPlaying()) StopMusic();
    WaitCursor(TRUE);
    
    snprintf(filnm, 1024, "%s/nolang/music/%s", getSignusDataDir(), name);
    if ((f = fopen(filnm, "rb")) != NULL) {
        fclose(f);
        PlayedModule = Mix_LoadMUS(filnm);
    }
    if (PlayedModule == NULL) return FALSE;
    MusicPlaying = TRUE;
    Mix_PlayMusic(PlayedModule, FALSE/*loop*/);
    MusicJustStarted = 0;
    WaitCursor(FALSE);

    return TRUE;
}



void StopMusic()
{
    if (MIDAS_disabled) return;

    if (!MusicPlaying) return;
    Mix_HaltMusic();
    Mix_FreeMusic(PlayedModule);
    PlayedModule = NULL;
    MusicPlaying = FALSE;
}




int IsMusicPlaying()
{
    if (MIDAS_disabled) return TRUE;
    if (!MusicOn) return TRUE;
    if (!MusicPlaying) return FALSE;
    return Mix_PlayingMusic();
}















/////////////////////////// SAMPLE ROUTINES ////////////////////////////////



typedef struct {
            byte       loaded;
            int        counter;
            char       name[9];
            Mix_Chunk *sample;
            byte       flags;
    } TSampleRecord;

#define MAX_SAMPLES  512

static TSampleRecord Samples[MAX_SAMPLES];


struct {
    int                   priority;
    int                   volume;
    MIDASsamplePlayHandle handle;
} PSmp[EFFECT_CHANNELS];


void Clear_PSmp()
{
    for (int i = 0; i < EFFECT_CHANNELS; i++)
    {
        PSmp[i].handle = INVALID_SAMPLE_HANDLE;
        PSmp[i].priority = 0;
    }
}

void InitSamplesLoader()
{
    for (int i = 0; i < MAX_SAMPLES; i++)
        Samples[i].loaded = Samples[i].counter = 0;
    for (int i = 0; i < EFFECT_CHANNELS; i++)
        PSmp[i].priority = 0;
}


inline int SmpIsPlaying(MIDASsamplePlayHandle x)
{
    return Mix_Playing(x);
}



MIDASsample LoadSample(char *name, int loop)
{
    int i, pos;

    if (MIDAS_disabled) return INVALID_SAMPLE;

    for (i = 0; i < MAX_SAMPLES; i++) {
        if (Samples[i].loaded && (strcmp(name, Samples[i].name) == 0)) {
            Samples[i].counter++;
            return i;
        }
    }

    for (pos = 1; pos < MAX_SAMPLES; pos++) {
        if (!Samples[pos].loaded) break;
    }
    if (pos == MAX_SAMPLES) return INVALID_SAMPLE;

    char filename[1024];
    snprintf(filename, 1024, "%s/nolang/sfx/%s.ogg", getSignusDataDir(), name);
    if (!fileExists(filename))
       snprintf(filename, 1024, "%s/%s/speech/%s.ogg", getSignusDataDir(), iniLocale, name);
        // FIXME -- 1) look into ~/.signus, too (not sure yet, maybe...)
    Samples[pos].sample = Mix_LoadWAV(filename); 
    if (Samples[pos].sample == NULL) return INVALID_SAMPLE;
    Samples[pos].loaded = TRUE;
    Samples[pos].counter = 1;
    strcpy(Samples[pos].name, name);
    Samples[pos].flags = (loop) ? PLAY_LOOPING : 0;
    return pos;
}



void FreeSample(MIDASsample sample)
{
    if (MIDAS_disabled) return;

    if (sample == INVALID_SAMPLE) return;
    if ((--Samples[sample].counter) == 0) {
        //StopSample(Samples[sample].sample);  FIXME - hopefully not needed for SDL_mixer
        Mix_FreeChunk(Samples[sample].sample);
        Samples[sample].sample = NULL;
        Samples[sample].loaded = FALSE;
        Samples[sample].counter = 0;
    }
}



MIDASsamplePlayHandle PlaySample(MIDASsample sample,
                                 int priority, unsigned volume, int panning)
{
    int v, i;

    if (MIDAS_disabled) return INVALID_SAMPLE_HANDLE;
    if (sample == INVALID_SAMPLE) return INVALID_SAMPLE_HANDLE;
    if (volume == 0) return INVALID_SAMPLE_HANDLE;

    for (v = 0; v < EFFECT_CHANNELS; v++) {
        if ((PSmp[v].priority) && (!SmpIsPlaying(PSmp[v].handle))) {
            PSmp[v].priority = 0;
            PSmp[v].handle = INVALID_SAMPLE_HANDLE;
        }
    }

    v = -1;
    for (i = 0; i < EFFECT_CHANNELS; i++) 
    {
        if (PSmp[i].priority == 0) 
        {
            v = i;
            break;
        }
    }
    if (v == -1)
    {
        for (i = 0; i < EFFECT_CHANNELS; i++) 
        {
            if (PSmp[i].priority <= priority) 
            {
                StopSample(PSmp[i].handle);
                v = i;
                break;
            }
        }
    }
    if (v == -1)
        return INVALID_SAMPLE_HANDLE;

    PSmp[v].priority = priority;
    PSmp[v].volume = volume;
    PSmp[v].handle = Mix_PlayChannel(v, Samples[sample].sample,
                                     Samples[sample].flags & PLAY_LOOPING ? -1 : 0);
    Mix_SetPanning(PSmp[v].handle, panning, 255-panning);  // FIXME?
    Mix_Volume(PSmp[v].handle, 2*volume/*FIXME?*/);
    return PSmp[v].handle;
}



void FadeOutSample(MIDASsamplePlayHandle sample, unsigned volume)
{
#if 0       // FIXME -- why was this commented out?!
    if (MIDAS_disabled) return;
    if (sample == NULL) return;

    Mix_Volume(sample, 2*volume);
    Mix_FadeOutChannel(sample, 50);
#endif
}

void FadeInSample(MIDASsamplePlayHandle sample, unsigned volume)
{
#if 0       // FIXME -- why was this commented out?!
    if (MIDAS_disabled) return;
    if (sample == NULL) return;

    Mix_Volume(sample, 2*volume);
    Mix_FadeInChannel(sample, Mix_GetChunk(sample), 
                      Samples[sample].flags & PLAY_LOOPING ? -1 : 0);
#endif
}



void StopSample(MIDASsamplePlayHandle sample)
{
    if (MIDAS_disabled) return;

    if (sample == INVALID_SAMPLE_HANDLE) return;
    for (int v = 0; v < EFFECT_CHANNELS; v++)
        if (PSmp[v].handle == sample) {
            PSmp[v].priority = 0;
            PSmp[v].handle = INVALID_SAMPLE_HANDLE;
            break;
        }

    Mix_HaltChannel(sample);
}



void PanSample(MIDASsamplePlayHandle sample, int panning)
{
    if (MIDAS_disabled) return;

    if (sample == INVALID_SAMPLE_HANDLE) return;
    
    Mix_SetPanning(sample, panning, 255 - panning); // FIXME?
}








/////////////// speech:

static char old_speech[400] = "???";

void SaySpeech(char *name, int priority)
{
    if (MIDAS_disabled) return;

    // zjisteni, jestli uz neskoncil;
    if (speech_handle == INVALID_SAMPLE_HANDLE)
        speech_handle_used = 0;
    if ((speech_handle_used) && (!SmpIsPlaying(speech_handle))) {
        StopSample(speech_handle);
        FreeSample(speech_sample);
        speech_handle_used = 0;
        speech_handle = NULL;
        strcpy(old_speech, "???");
    }
    if ((speech_handle_used) && (speech_priority > priority)) return;

    if (strcmp(name, old_speech) == 0) return;
    strcpy(old_speech, name);

    // odstraneni dokecani:
    if (speech_handle_used) {
        StopSample(speech_handle);
        FreeSample(speech_sample);
        speech_handle_used = 0;
    }
    speech_priority = priority;
    speech_sample = LoadSample(name, FALSE);
    speech_handle = PlaySample(speech_sample, speech_priority, SpeechVolume, 128);
    speech_handle_used = 1;
}
