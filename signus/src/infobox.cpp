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



#include <climits>
#include <SDL_image.h>
#include "units.h"
#include "ui_toolkit.h"
#include "global.h"
#include "anims.h"
#include "sound.h"
#include "mouse.h"

extern void InitPalTimer();
extern void DonePalTimer();

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
	void Draw();
	int SpecialHandle(TEvent *e, int Cmd);
	int Exec();
};



TInfoDialog::TInfoDialog(int ax, int ay, TObject *u) :
	TDialog(ax, ay, 620, 460, "dlginfo") {
	unit = u;
	pic = bmp = NULL;

	Insert(new TButton(15, 409, SigText[TXT_OK], cmOk, TRUE));
	Insert(new TStaticText(300, 20, 380, 40, unit->GetName(), TRUE));
	Insert(new TStaticText2(294, 60, 308, 380, unit->GetDescript()));
	Insert(new TStaticText(20, 287, 100, 20, SigText[TXT_EXPERIENCE]));

	if (IsUnitAT(unit)) {
		Insert(new TBitmap(70, 238, FALSE, GraphicsDF->get("artetech"), 160, 17));
	}

	if (u->Type < unRadar) {
		Insert(new TBitmap(253, 288, FALSE,
			LevelBmps[((TUnit*)u)->Level], 29, 16, FALSE));
	}
}

void TInfoDialog::Draw() {
	TDialog::Draw();
	PercentBar(DrwBuf, w, h, 122, 288, 160-29, 16, 55, 51,
		GetUnitExper(unit), "");
}

int TInfoDialog::SpecialHandle(TEvent *e, int Cmd) {
	if (Cmd == cmNext) {
		return -1;
	}

	return TDialog::SpecialHandle(e, Cmd);
}



int TInfoDialog::Exec() {
	File animfile;
	VVFStream *anim = NULL;
	int ret = -1;
	unsigned awidth = 0, aheight = 0, timer = 0;
	char filename[PATH_MAX];
	TEvent e;

	sprintf(filename, "unit%i", unit->Type);

	if (open_anim_file(animfile, filename)) {
		anim = new VVFStream(&animfile);
		awidth = anim->video_width();
		aheight = anim->video_height();
	} else {
		SDL_Surface *image;
		snprintf(filename, PATH_MAX,"%s/nolang/unit-images/unit%i.jpg",
			getSignusDataDir(), unit->GetType());
		image = IMG_Load(filename);
		byte *pall = (byte*)memalloc(256 * 192);
		paletizeSurface(pall, image, "pal_rgb");
		SDL_FreeSurface(image);
		Insert(new TBitmap(22, 22, FALSE, pall, 256, 192));
	}

	DonePalTimer();
	MouseSetCursor(mcurArrow);
	Draw();

	if (anim) {
		anim->next_frame();
		CopyBmp(DrwBuf, w, 22, 22, anim->videobuf(), awidth, aheight);
		timer = anim->frame_time();
	}

	FadeDlg(TRUE);
	MouseSetRect(x, y, x+w-1, y+h-1);
	timer += SDL_GetTicks();

	while (ret == -1) {
		if (anim && timer <= SDL_GetTicks()) {
			if (!anim->next_frame()) {
				anim->reset();
				anim->next_frame();

				if (anim->error()) {
					delete anim;
					anim = NULL;
					continue;
				}
			}

			timer += anim->frame_time();
			PutBitmap(x+22,y+22, anim->videobuf(), awidth,aheight);
			UpdateScreen();
		}

		if (MusicOn && (!IsMusicPlaying())) {
			JukeboxNext();
		}

		do {
			GetEvent(&e);
			ret = HandleEvent(&e);
		} while (ret == -1 && e.What != evNothing);
	}

	if (anim) {
		// Show the last anim frame during dialog fade
		CopyBmp(DrwBuf, w, 22, 22, anim->videobuf(), awidth, aheight);
		delete anim;
	}

	MouseSetRect(0, 0, RES_X-1, RES_Y-1);
	FadeDlg(FALSE);
	InitPalTimer();
	return ret;
}






/////////////////////////////////////////


TDialog *TObject::DetailedInfo()
{
    return new TInfoDialog((iniResolution == SVGA_640x480) ? 8 : (10 + (VIEW_SX-620)/2), 
                               (iniResolution == SVGA_640x480) ? 8 : (35 + (VIEW_SY-460)/2),
                               this);
}
