/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002, 2003, 2004
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

#include <cstring>

#include "graphio.h"
#include "global.h"
#include "fonts.h"

void PutStr(void *tar, int width, int height, int xpoz, int ypoz,
	const char *s, TFont *f, byte clr1, byte clr2) {
	if (!s || !*s) {
		return;
	}

	// FIXME: is this noticeably slow?
	SDL_Color clr = {PaletteSDL[clr1].r, PaletteSDL[clr1].g,
		PaletteSDL[clr1].b, 0};
	SDL_Color black = {PaletteSDL[clr2].r, PaletteSDL[clr2].g,
		PaletteSDL[clr2].b, 0};
	SDL_Rect dest_rect = {(Sint16)xpoz, (Sint16)ypoz, 0, 0};
	SDL_Surface *text, *dest;
	int i, lineheight = GetStrHeight(s, f);
	char *line, *buf = strdup(s);

	if (!buf) {
		fprintf(stderr, "failed to render '%s':\n", s);
		return;
	}

	line = buf;
	dest = SDL_CreateRGBSurfaceFrom(tar, width, height, 8/*depth*/,
		width /*pitch*/, 0, 0, 0, 0);
	SDL_SetPaletteColors(dest->format->palette, PaletteSDL, 0, 256);

	for (line = buf, i = 0; *line; line += i) {
		for (i = 0; line[i] && line[i] != '\n'; i++) {
			if (line[i] == '\r') {
				line[i] = ' ';
			}
		}

		if (line[i]) {
			if (!i) {
				i++;
				dest_rect.y += lineheight;
				continue;
			}

			line[i++] = '\0';
		}

		text = TTF_RenderUTF8_Shaded(f, line, clr, black);

		if (!text) {
			fprintf(stderr, "failed to render '%s':\n", s);
			fprintf(stderr, "%s\n", SDL_GetError());
			SDL_FreeSurface(dest);
			free(buf);
			return;
		}

		SDL_SetColorKey(text, SDL_TRUE, 0);
		SDL_BlitSurface(text, NULL, dest, &dest_rect);
		SDL_FreeSurface(text);
		dest_rect.y += lineheight;
	}

	SDL_FreeSurface(dest);
	free(buf);
}
