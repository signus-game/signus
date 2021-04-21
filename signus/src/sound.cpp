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

// Rozhrani mezi Signusem a MikModem - zvuky a hudba

#include <stdlib.h>
#include <math.h>
#include <climits>
#include "global.h"
#include "sound.h"



int MIDAS_disabled = FALSE; // FIXME

int EffectsVolume, SpeechVolume, MusicVolume;

int MusicOn;
int audio_formats = 0;



static MIDASsamplePlayHandle speech_handle = 0;
static int speech_handle_used = 0;
static int speech_priority = 0;
static MIDASsample speech_sample = 0;



// sekce popisujici prehravany modul:

Mix_Music   *PlayedModule = NULL;   // modul
int          MusicPlaying = FALSE;   // prehrava se vubec?



///////////////////////////// INIT & DONE /////////////////////////////////


void InitSamplesLoader();


int InitSound() {
	speech_handle_used = 0;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		return FALSE;
	}

	audio_formats = Mix_Init(MIX_INIT_OGG | MIX_INIT_MOD);

	if (!(audio_formats & MIX_INIT_OGG)) {
		fprintf(stderr, "SDL_mixer does not support Ogg Vorbis\n");
	}

	if (!(audio_formats & MIX_INIT_MOD)) {
		fprintf(stderr, "SDL_mixer does not support MOD music\n");
		MusicOn = 0;
	}

	OpenChannels(MUSIC_CHANNELS, EFFECT_CHANNELS);
	InitSamplesLoader();
	InitJukebox();
	SetVolume(iniSoundVol, iniSpeechVol, iniMusicVol);
	return TRUE;
}



int DoneSound() {
	if (MusicPlaying) {
		StopMusic();
	}

	Mix_Quit();
	CloseChannels();
	DoneJukebox();
	return TRUE;
}


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



void SetVolume(int effects, int speech, int music) {
	if (MIDAS_disabled) {
		return;
	}

	EffectsVolume = effects;
	SpeechVolume = speech;
	MusicVolume = music;
	Mix_VolumeMusic(MusicVolume * MIX_MAX_VOLUME / 64);

	if (MusicOn && (music == 0)) {
		StopMusic();
	}

	if (audio_formats & MIX_INIT_MOD) {
		MusicOn = MusicVolume > 0;
	}

	// FIXME __ update music channel volume here
	//          update ALL channels information1!!
}







/////////////////////////// MUSIC ROUTINES ////////////////////////////////


static int MusicJustStarted = FALSE;

int PlayMusic(const char *name) {
	// FIXME
	char filnm[PATH_MAX];
	SDL_RWops *rw;

	if (MIDAS_disabled) {
		return TRUE;
	}

	if (!MusicOn) {
		return TRUE;
	}

	StopMusic();
	WaitCursor(TRUE);
	snprintf(filnm, PATH_MAX, "%s/nolang/music/%s", getSignusDataDir(),
		name);
	rw = SDL_RWFromFile(filnm, "rb");

	if (!rw) {
		WaitCursor(FALSE);
		return FALSE;
	}

	PlayedModule = Mix_LoadMUS_RW(rw, 0);
	SDL_RWclose(rw);

	if (PlayedModule == NULL) {
		WaitCursor(FALSE);
		return FALSE;
	}

	MusicPlaying = TRUE;
	Mix_PlayMusic(PlayedModule, FALSE/*loop*/);
	MusicJustStarted = 0;
	WaitCursor(FALSE);
	return TRUE;
}

int PlayMusicBuffer(const void *data, int size) {
	SDL_RWops *rw;

	if (MIDAS_disabled) {
		return TRUE;
	}

	if (!MusicOn) {
		return TRUE;
	}

	StopMusic();
	rw = SDL_RWFromConstMem(data, size);

	if (!rw) {
		return FALSE;
	}

	PlayedModule = Mix_LoadMUS_RW(rw, 0);
	SDL_RWclose(rw);

	if (PlayedModule == NULL) {
		return FALSE;
	}

	MusicPlaying = TRUE;
	Mix_PlayMusic(PlayedModule, FALSE/*loop*/);
	MusicJustStarted = 0;
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
	byte loaded;
	int counter;
	char name[9];
	Mix_Chunk *sample;
	byte flags;
	Uint8 *buffer;
} TSampleRecord;

#define MAX_SAMPLES  512

static TSampleRecord Samples[MAX_SAMPLES];

typedef struct {
    int                   priority;
    int                   volume;
    MIDASsamplePlayHandle handle;
} TChannel;

TChannel PSmp[EFFECT_CHANNELS];


void Clear_PSmp()
{
    for (int i = 0; i < EFFECT_CHANNELS; i++)
    {
        PSmp[i].handle = INVALID_SAMPLE_HANDLE;
        PSmp[i].priority = 0;
    }
}

void InitSamplesLoader() {
	for (int i = 0; i < MAX_SAMPLES; i++) {
		Samples[i].loaded = Samples[i].counter = 0;
		Samples[i].buffer = NULL;
	}

	for (int i = 0; i < EFFECT_CHANNELS; i++) {
		PSmp[i].priority = 0;
	}
}


inline int SmpIsPlaying(MIDASsamplePlayHandle x)
{
    return Mix_Playing(x);
}



MIDASsample LoadSample(const char *name, int loop) {
	int i, pos;

	if (MIDAS_disabled || !name) {
		return INVALID_SAMPLE;
	}

	for (i = 0; i < MAX_SAMPLES; i++) {
		if (Samples[i].loaded && (strcmp(name, Samples[i].name) == 0)) {
			Samples[i].counter++;
			return i;
		}
	}

	for (pos = 1; pos < MAX_SAMPLES; pos++) {
		if (!Samples[pos].loaded) break;
	}

	if (pos == MAX_SAMPLES) {
		return INVALID_SAMPLE;
	}

	char filename[1024];
	snprintf(filename, 1024, "%s/nolang/sfx/%s.ogg", getSignusDataDir(),
		name);

	if (!fileExists(filename)) {
		snprintf(filename, 1024, "%s/%s/speech/%s.ogg",
			getSignusDataDir(), iniLocale, name);
	}

	// FIXME -- 1) look into ~/.signus, too (not sure yet, maybe...)
	Samples[pos].sample = Mix_LoadWAV(filename);

	if (Samples[pos].sample == NULL) {
		return INVALID_SAMPLE;
	}

	Samples[pos].loaded = TRUE;
	Samples[pos].counter = 1;
	strcpy(Samples[pos].name, name);
	Samples[pos].flags = (loop) ? PLAY_LOOPING : 0;
	Samples[pos].buffer = NULL;
	return pos;
}

MIDASsample ConvertSample(const void *src, size_t size, int freq, int bits, unsigned chans, int loop) {
	int ret, pos, outfreq, outchans;
	Uint16 fmt, outfmt;
	size_t newsize;
	SDL_AudioCVT cvt;
	Uint8 *buf;

	if (MIDAS_disabled) {
		return INVALID_SAMPLE;
	}

	for (pos = 1; pos < MAX_SAMPLES && Samples[pos].loaded; pos++);

	if (pos == MAX_SAMPLES) {
		return INVALID_SAMPLE;
	}

	switch (bits) {
	case -61:
		fmt = AUDIO_S16MSB;
		break;

	case -16:
		fmt = AUDIO_S16LSB;
		break;

	case -8:
		fmt = AUDIO_S8;
		break;

	case 8:
		fmt = AUDIO_U8;
		break;

	case 16:
		fmt = AUDIO_U16LSB;
		break;

	case 61:
		fmt = AUDIO_U16MSB;
		break;

	default:
		return INVALID_SAMPLE;
	}

	if (!Mix_QuerySpec(&outfreq, &outfmt, &outchans)) {
		return INVALID_SAMPLE;
	}

	ret = SDL_BuildAudioCVT(&cvt, fmt, chans, freq, outfmt, outchans,
		outfreq);

	if (ret < 0) {
		return INVALID_SAMPLE;
	} else if (ret) {
		cvt.len = size;
		cvt.buf = buf = new Uint8[size * cvt.len_mult];
		memcpy(buf, src, size);
		SDL_ConvertAudio(&cvt);
		newsize = cvt.len_cvt;
	} else {
		buf = new Uint8[size];
		memcpy(buf, src, size);
		newsize = size;
	}

	Samples[pos].sample = Mix_QuickLoad_RAW(buf, newsize);

	if (!Samples[pos].sample) {
		delete[] buf;
		return INVALID_SAMPLE;
	}

	Samples[pos].loaded = TRUE;
	Samples[pos].counter = 1;
	strcpy(Samples[pos].name, "");
	Samples[pos].flags = (loop) ? PLAY_LOOPING : 0;
	Samples[pos].buffer = buf;
	return pos;
}

void FreeSample(MIDASsample sample) {
	if (MIDAS_disabled) {
		return;
	}

	if (sample >= MAX_SAMPLES || !Samples[sample].loaded) {
		return;
	}

	if ((--Samples[sample].counter) == 0) {
		StopSample(sample);
		Mix_FreeChunk(Samples[sample].sample);
		Samples[sample].sample = NULL;
		Samples[sample].loaded = FALSE;
		Samples[sample].counter = 0;
		delete[] Samples[sample].buffer;
		Samples[sample].buffer = NULL;
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

void SaySpeech(const char *name, int priority)
{
    if (MIDAS_disabled) return;

    // zjisteni, jestli uz neskoncil;
    if (speech_handle == INVALID_SAMPLE_HANDLE)
        speech_handle_used = 0;
    if ((speech_handle_used) && (!SmpIsPlaying(speech_handle))) {
        StopSample(speech_handle);
        FreeSample(speech_sample);
        speech_handle_used = 0;
        speech_handle = 0;
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
