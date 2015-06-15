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
// Modul pro prehravani animaci - vyuziva VVFLIB, je tu jenom pro
// pohodlnost - proste se zavola jedna fce a ta se postara o vsechno
//


#if 0 // FIXME

#include "global.h"
#include "graphio.h"
#include "events.h"
#include "ui_toolkit.h"
#include "sound.h"
#include "anims.h"
#include "vvflib.h"
#include "engtimer.h"



FILE *GetAnimFile(char *name)
{
    TDataIndex *index;
    int i;
    TDataFile *a;
    FILE *f;


//#ifdef DEBUG
    {
        char nam[30];

        strcpy(nam, "DATAA/");
        strcat(nam, name);
        strcat(nam, ".VVF");
        f = fopensafe(nam, "rb");
        if (f != NULL) return f;
    }
//#endif

    a = new TDataFile("anims.dat", dfOpenRead);
    i = a->lookfor(name, 0, a->getcount() - 1);
    if (i == -1) return NULL;
    {
        int ofs;
        
        index = a->getinfo(i);
        ofs = index->offset;
        delete a;
        f = fopensafe("anims.dat", "rb");
        fseek(f, ofs, SEEK_SET);
    }
    return f;
}








////////////////////////////////////////////////


// Prehravani doprovodnych animaci:


static int sig_SelectMutation(TMutationHeader *Muts, int Cnt)
{
    int lngmod, titmod;

    lngmod = 0;
    if (iniTitledAnims) titmod = 1; else titmod = 0;
    return lngmod + titmod;
}




#define TIT_FREQ    (3*10) /*2 sec*/

static char *titles_ptr = NULL, *titles_act = NULL;
static int titles_cnt = 0;
static char titles_buf[300];

static void sig_Subtitles(int ID, int Size, byte *Data, int Stream)
{
    titles_ptr = (char*) memalloc(strlen((char*)Data) + 1);
    titles_act = titles_ptr + 1;
    titles_cnt = 0;
    strcpy(titles_ptr, (char*) Data);
}


extern void ClearScr();
extern void *LFB_Lock();
extern void LFB_Unlock();
extern int SwitchDisplayMode(int mode);

extern byte *VideoLFB;
extern unsigned LFB_Pitch;
extern LPDIRECTDRAW2                  lpDD2;          // DirectDraw2 interface
extern LPDIRECTDRAWSURFACE2           lpDDSPrimary3;  // DirectSurface3 interface




static void draw_titles()
{
    char *bx;
    byte *adr = ((byte*)VideoLFB) + 430 * 640;

    if (iniLanguage[0] == 'c') return;
    if (titles_cnt == 0) {
        for (bx = titles_buf; (*titles_act != '*') && (*titles_act != '#'); titles_act++, bx++) 
            {*bx = *titles_act;}
        if (*titles_act == '#') { // end
            memset(adr, 0, 12800);
            memfree(titles_ptr);
            titles_ptr = NULL;
            return;
        }
        titles_act++;
        *bx = 0;
        memset(adr, 0, 12800);
        PutStr(adr, 640, 
          (640 - GetStrWidth(titles_buf, NormalFont)) / 2, 0, titles_buf, NormalFont, 255, 0);
    }
    if (++titles_cnt == TIT_FREQ) titles_cnt = 0;
}





static int svga_StartPlaying(TMutationHeader *Mut)
{
    if (Mut->AudioChannels) OpenChannels(0, Mut->AudioChannels);
    return 1;
}



static int svga_StopPlaying(TMutationHeader *Mut)
{
    if (Mut->AudioChannels) CloseChannels();
    if (titles_ptr) {
        memfree(titles_ptr);
        titles_ptr = NULL;
    }
    return 1;
}




static void svga_DrawFrame(TMutationHeader *Mut, void *Buffer, int FromY, int ToY)
{
    int delta = ToY - FromY + 1;
    byte *b = (byte*) Buffer + FromY * 320;
    
    LFB_Lock();

    _asm {
        push esi
        push edi
        push eax
        push edx
        push ecx
        mov  esi, b

        mov  edi, VideoLFB
        add  edi, 38400 // 640 * 60
        mov  eax, FromY
        mov  edx, 1280 // 2 * 640
        mul  edx
        add  edi, eax

        mov  edx, delta
        mov  ecx, 320
    rep_lab:
        lodsb
        mov  ah, al
        stosw
        loop rep_lab

        mov  ecx, 320
        sub  esi, 320
    rep_lab2:
        lodsb
        mov  ah, al
        stosw
        loop rep_lab2

    
        mov  ecx, 320
        dec  edx        
        jnz  rep_lab

        pop  ecx
        pop  edx
        pop  eax
        pop  edi
        pop  esi
    }
    
    if (titles_ptr) draw_titles();
    LFB_Unlock();
}



static void svga_InterlaceDrawFrame(TMutationHeader *Mut, void *Buffer, int FromY, int ToY)
{
    int delta = ToY - FromY + 1;
    byte *b = (byte*) Buffer + FromY * 320;
    
    LFB_Lock();

    _asm {
        push esi
        push edi
        push eax
        push edx
        push ecx
        mov  esi, b

        mov  edi, VideoLFB
        add  edi, 38400 // 640 * 60
        mov  eax, FromY
        mov  edx, 1280 // 2 * 640
        mul  edx
        add  edi, eax

        mov  edx, delta
        mov  ecx, 320
    rep_lab:
        lodsb
        mov  ah, al
        stosw
        loop rep_lab
    
        mov  ecx, 320
        add  edi, 640
        dec  edx        
        jnz  rep_lab

        pop  ecx
        pop  edx
        pop  eax
        pop  edi
        pop  esi
    }
    
    if (titles_ptr) draw_titles();
    LFB_Unlock();
}







void svga_SetPalette(void *pal)
{
    SetPalette((char*)pal);
}



int my_TI()
{
    TEvent e;

    do {
        GetEvent(&e);
        if ((e.What == evKeyDown) || (e.What == evMouseDown)) {
            return 1;
        }
    } while (e.What != evNothing);
    return 0;
}


///////////////

int PlayAnimation(char *name)
{
    FILE *animf;
    int rt;
    TEvent e;

    // shozeni Signusu - uvolni to potrebnou pamet:
    MouseHide();
    DoneInteract();
    StopMusic();
    CloseChannels();

    ClearScr();
    if (iniResolution != SVGA_640x480) SwitchDisplayMode(SVGA_640x480);
    MouseSetRect(639, 479, 640, 480);
    MouseSetPos(640, 480);
    
    // pripraveni prehravace:
    VVF_SelectMutation = sig_SelectMutation;
    VVF_PlayDirect = TRUE;
    VVF_InterpolateFrames = FALSE;
    VVF_free = free;
    VVF_malloc = malloc;
    VVF_DrawFrame = (iniInterpolateAnims) ? svga_InterlaceDrawFrame : svga_DrawFrame;
    VVF_StartPlaying = svga_StartPlaying;
    VVF_StopPlaying = svga_StopPlaying;
    VVF_CanSetPalette = TRUE;
    VVF_ExtendedChunk = sig_Subtitles;
    VVF_TestInterrupt = my_TI;
    VVF_SetPalette = svga_SetPalette;
    
    // prehrani animace:
    animf = GetAnimFile(name);
#ifdef DEBUG
    if (animf == NULL) return 0;
#endif
    do {GetEvent(&e);} while (e.What != evNothing);
    DoneTimer();
    rt = PlayVVF(animf);
    InitTimer();
    do {GetEvent(&e);} while (e.What != evNothing);
    fclose(animf);
    VVF_ExtendedChunk = NULL;    
    
    // reinicializace:
    // (test uspechu uz se neprovadi, provedl se pri startu Signsus)
    {
        LFB_Lock();
        for (int i = 0; i < 480; i++)
            memset(VideoLFB + LFB_Pitch * i, 0, 640);
        LFB_Unlock();
    }
    if (iniResolution != SVGA_640x480) SwitchDisplayMode(iniResolution);
    ClearScr();
    SetPalette(Palette);
    OpenChannels(MUSIC_CHANNELS, EFFECT_CHANNELS);
    InitInteract();
    MouseSetRect(0, 0, RES_X-1, RES_Y-1);
    MouseSetPos(RES_X / 2, RES_Y / 2);
    MouseShow();
    return rt;
}

#endif
