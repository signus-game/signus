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


/*

implementace Infoboxu jednotky (TUnit::DetailedInfo()...)

*/


#include "headers.h"

#include "units.h"
#include "ui_toolkit.h"
#include "global.h"
#include "anims.h"
#include "sound.h"

#include "miniSDL_image.h"

    



////////////////////// flicer - prehrani vvf animace: ///////////////////////


#if 0 // FIXME
int flicer_x, flicer_y;

void flicer_DrawFrame(TMutationHeader *Mut, void *Buffer, int FromY, int ToY)
{
    PutBitmap32(flicer_x, flicer_y + FromY, ((byte*)Buffer) + 256 * FromY, 256, (ToY-FromY+1));
}

int flicer_TestInterrupt()
{
    TEvent e;

    do {
        GetEvent(&e);
        if ((e.What != evNothing) && (e.What != evMouseMove)) {
            PutEvent(&e);
            return 1;
        }
    } while (e.What != evNothing);
    return 0;
}

int flicer_SelectMutation(TMutationHeader *Muts, int Cnt) {return 0;}
int flicer_StartPlaying(TMutationHeader *Mut) {return 1;}
int flicer_StopPlaying(TMutationHeader *Mut) {return 1;}
#endif

extern void InitPalTimer();
extern void DonePalTimer();

int flicer_RunAnim(int x, int y, char *name)
{
#if 0 // FIXME
    FILE *animf;
    int (*ti)();
    int rt;
    
    DonePalTimer();
    flicer_x = x, flicer_y = y;
    VVF_SelectMutation = flicer_SelectMutation;
    VVF_PlayDirect = TRUE;
    VVF_InterpolateFrames = FALSE;
    VVF_free = free;
    VVF_malloc = malloc;
    VVF_DrawFrame = flicer_DrawFrame;
    VVF_StartPlaying = flicer_StartPlaying;
    VVF_StopPlaying = flicer_StopPlaying;
    VVF_CanSetPalette = FALSE;
    ti = VVF_TestInterrupt;
    VVF_TestInterrupt = flicer_TestInterrupt;
    
    do {
        animf = GetAnimFile(name);
        rt = PlayVVF(animf); 
        fclose(animf);
    } while (rt == 1);
    VVF_TestInterrupt = ti;
    InitPalTimer();
#endif
    
    return 1;
}









///////////////////////////// info dialog: //////////////////////////////////


double GetUnitExper(TObject *unit)
{
    TUnit *u;
    int need, act;

    if (unit->Type >= unRadar) return 0;
    u = (TUnit*)unit;
    if (u->Level == 9) return 1/*full*/;
    need = TabUnitsExperience[u->Type][u->Level+1] -
           TabUnitsExperience[u->Type][u->Level];
    act  = u->Experience -
           TabUnitsExperience[u->Type][u->Level];
           
    return (((double)act)/need);
}



// je to artefakt?:
int IsUnitAT(TObject *unit) 
{
    switch (unit->Type) {
        case unDirrac :
        case unAgni :
        case unGolias :
        case unSpektrum :
        case unNeptun :
        case unHelios :
        case unKraken :
        case unBarracuda :
        case unMystik :
        case unSatan :
            return TRUE;
    }   
    return FALSE;
}



#define cmNext 8888
#define cmAnim 9999

class TInfoDialog : public TDialog {
        public:
            void *pic;
            int  picnum;
            TObject *unit;
            TBitmap *bmp;
            TBitmap *ArtefaktLogo;
        
            TInfoDialog(int ax, int ay, TObject *u);
            int SpecialHandle(TEvent *e, int Cmd);          
            int RunAnimation();
    };



extern char DLG_backimg[9];

TInfoDialog::TInfoDialog(int ax, int ay, TObject *u) : TDialog(ax, ay, 620, 460, "dlginfo")
{
    unit = u;
    pic = bmp = NULL;
    
    Insert(new TButton(15, 409, SigText[TXT_OK], cmOk, TRUE));
    Insert(new TStaticText(300, 20, 380, 40, unit->GetName(), TRUE));
    Insert(new TStaticText2(294, 60, 308, 380, unit->GetDescript()));
    Insert(new TStaticText(20, 287, 100, 20, SigText[TXT_EXPERIENCE]));
    if (IsUnitAT(unit))
        Insert(new TBitmap(70, 238, FALSE, GraphicsDF->get("artetech"), 160, 17));
    PercentBar(DrwBuf, w, 122, 288, 160-29, 16, 55, 51, GetUnitExper(unit), "");
    if (u->Type < unRadar)
        Insert(new TBitmap(253, 288, FALSE, LevelBmps[((TUnit*)u)->Level], 29, 16, FALSE));
    {
        TEvent e;
        e.What = evMouseDown;
        e.Mouse.Buttons = mbBottomButton | mbLeftButton | mbRightButton;
        PutEvent(&e);
    }
    
    //DLG_backimg[0] = 0; // to disable redrawing
    
    char filename[1024];
    SDL_Surface *image;
    snprintf(filename, 1024,
             "%s/nolang/unit-images/unit%i.jpg",
             getSignusDataDir(), unit->GetType());
    image = IMG_Load(filename);
    byte *pall = (byte*)memalloc(256 * 192);
    paletizeSurface(pall, image, "pal_rgb");
    SDL_FreeSurface(image);
    Insert(new TBitmap(22, 22, FALSE, pall, 256, 192));
}



int TInfoDialog::SpecialHandle(TEvent *e, int Cmd)
{
    if ((e->What == evMouseDown) &&
        (e->Mouse.Buttons == mbBottomButton | mbLeftButton | mbRightButton)) {
        RunAnimation();
        return -1;
    }
    else if (Cmd == cmNext) {
        return -1;
    }
    else return TDialog::SpecialHandle(e, Cmd);
}



int TInfoDialog::RunAnimation()
{
    char b[20];
    int rt;
    
    sprintf(b, "unit%i", unit->Type);
    rt = flicer_RunAnim(x + 22, y + 22, b);
    return rt;
}










/////////////////////////////////////////


TDialog *TObject::DetailedInfo()
{
    return new TInfoDialog((iniResolution == SVGA_640x480) ? 8 : (10 + (VIEW_SX-620)/2), 
                               (iniResolution == SVGA_640x480) ? 8 : (35 + (VIEW_SY-460)/2),
                               this);
}
