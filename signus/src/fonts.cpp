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
// Implementace proporcialnich textovych fontu 
//

#include "headers.h"
#include "graphio.h"
#include "fonts.h"

void PutStr(void *tar, int tarwidth, int xpoz, int ypoz,
            char *s, TFont *f, byte clr1, byte /*clr2*/)
{
    if (s == NULL || *s == 0)
        return;

    // FIXME: is this noticeably slow?

    SDL_Color clr = { 0xFF, 0xFF, 0xFF, 0 };
    SDL_Color black = { 0, 0, 0, 0 };
    SDL_Surface *text = TTF_RenderText_Shaded(f, s, clr, black);

    if (text == NULL)
    {
        fprintf(stderr, "failed to render '%s':\n", s);
        fprintf(stderr, "%s\n", SDL_GetError());
        return;
    }

    SDL_SetColorKey(text, SDL_SRCCOLORKEY, 0);
    
    SDL_Surface *surf = 
        SDL_CreateRGBSurfaceFrom(tar, tarwidth, ypoz+text->h,
                                 8/*depth*/,
                                 tarwidth /*pitch*/,
                                 0,0,0,0);

    SDL_Palette *pal = GetScreenSurface()->format->palette;
    SDL_SetColors(surf, pal->colors, 0, pal->ncolors);
    
    SDL_Rect dest_rect = { xpoz, ypoz, 0, 0 };
    SDL_BlitSurface(text, NULL, surf, &dest_rect);
    
    SDL_FreeSurface(text);
    SDL_FreeSurface(surf);
}
