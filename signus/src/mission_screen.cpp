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


#include <string.h>
#include "global.h"
#include "mouse.h"
#include "graphio.h"
#include "events.h"
#include "mission_screen.h"

#include <SDL/SDL_image.h>

#define PRECISION      5
#define VALUES         (1<<PRECISION)
#define STEP           (1<<(8-PRECISION))


static void paletizeSurface(byte *output, SDL_Surface *surf, char *tableName)
{
    byte *table = (byte*) GraphicsDF->get(tableName);
    byte *Pout, *Pin;
    int x, y;
    int delta = surf->pitch - surf->w*3;
    byte r,g,b;
  
    for (Pout = output, Pin = (byte*)surf->pixels, y = surf->h; y > 0; 
         y--, Pin += delta)
    {
        for (x = surf->w; x > 0; x--)
        {
            r = *(Pin++)/STEP;
            g = *(Pin++)/STEP;
            b = *(Pin++)/STEP;
            *(Pout++) = table[r*VALUES*VALUES + g*VALUES + b];
        }
    }
    
    free(table);
}




static void drawPicture(int mission, byte *pal)
{
    char filename[1024];
    SDL_Surface *background, *name, *image;

    snprintf(filename, 1024, "%s/nolang/screens/background.jpg", getSignusDataDir());
    background = IMG_Load(filename);
//    background = SDL_DisplayFormat(tmp);
    
    snprintf(filename, 1024, "%s/nolang/screens/mis%i-image.jpg", getSignusDataDir(), mission);
    image = IMG_Load(filename);

    snprintf(filename, 1024, "%s/%s/screens/mis%i-name.jpg", getSignusDataDir(), iniLocale, mission);
    name = IMG_Load(filename);
    
    SDL_Rect srcRect, dstRect;
    
    srcRect.x = srcRect.y = 0;
    srcRect.w = 686; srcRect.h = 72;
    dstRect.x = 58; dstRect.y = 64;
    dstRect.w = dstRect.h = 0;    
    SDL_BlitSurface(name, &srcRect, background, &dstRect);

    srcRect.x = srcRect.y = 0;
    srcRect.w = 586; srcRect.h = 302;
    dstRect.x = 108; dstRect.y = 226;
    dstRect.w = dstRect.h = 0;    
    SDL_BlitSurface(image, &srcRect, background, &dstRect);

    SDL_FreeSurface(name);
    SDL_FreeSurface(image);
    
    byte pall[800*600];
    paletizeSurface(pall, background, "scr_rgb");
    DrawPicture(pall);
//    SDL_BlitSurface(background, &srcRect, SDL_GetVideoSurface(), &srcRect);
    
    SDL_FreeSurface(background);
}

void ShowMissionIntroScreen(int mission)
{
    char *pal2;
    TEvent e;
    
    FadeOut(Palette, 0);
    MouseHide();
    pal2 = (char*) GraphicsDF->get("scrpal");

    drawPicture(mission, (byte*)pal2);

    FadeIn(pal2, 0);
    do {GetEvent(&e);} while ((e.What != evKeyDown) && (e.What != evMouseDown));
    FadeOut(pal2, 0);
    ClearScr();
    MouseShow();
    memfree(pal2);
}

