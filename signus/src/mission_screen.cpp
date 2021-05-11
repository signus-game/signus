/*
 *  This file is part of Signus: The Artefact Wars
 *  https://github.com/signus-game
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
#include <SDL_image.h>
#include "global.h"
#include "mouse.h"
#include "graphio.h"
#include "events.h"
#include "mission_screen.h"

static void drawPicture(int mission, byte *pal)
{
    char filename[64], *path;
    SDL_Surface *background, *name, *image;

    path = signus_nolang_path("screens/background.jpg");
    background = IMG_Load(path);
    memfree(path);
//    background = SDL_DisplayFormat(tmp);
    
    sprintf(filename, "screens/mis%i-image.jpg", mission);
    path = signus_nolang_path(filename);
    image = IMG_Load(path);
    memfree(path);

    sprintf(filename, "screens/mis%i-name.jpg", mission);
    path = signus_locale_path(filename);
    name = IMG_Load(path);
    memfree(path);
    
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
    uint8_t *pal2;
    TEvent e;
    
    FadeOut(Palette, 0);
    MouseHide();
    pal2 = (uint8_t*) GraphicsDF->get("scrpal");

    drawPicture(mission, (byte*)pal2);

    FadeIn(pal2, 0);
    do {GetEvent(&e);} while ((e.What != evKeyDown) && (e.What != evMouseDown));
    FadeOut(pal2, 0);
    ClearScr();
    MouseShow();
    memfree(pal2);
}

