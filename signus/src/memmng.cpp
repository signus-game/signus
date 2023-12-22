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




// Pametovy manager pro Signus:


#include "memmng.h"
#include <stdlib.h>
#include "global.h"
#include "system.h"
#include "engine.h"


////////////////////////////// ALOKACE POLI //////////////////////////////

#include "datafile.h"

#define MAXNAME 32

TSprite *alloc_sprite(int width, int height) {
	TSprite *ret;

	ret = (TSprite*)memalloc(width * height + offsetof(TSprite, data));
	ret->dx = ret->dy = 0;
	ret->w = width;
	ret->h = height;
	return ret;
}

TSprite *copy_sprite(const TSprite *s) {
	TSprite *ret = alloc_sprite(s->w, s->h);

	ret->dx = s->dx;
	ret->dy = s->dy;
	memcpy(ret->data, s->data, s->w * s->h);
	return ret;
}

// Fce pro nacteni pole z .DAT a jeho uvolneni z pameti:

TSprite *load_sprite(const void *data) {
	int x, y, width, height;
	MemoryReadStream *stream;
	TSprite *ret;

	stream = new MemoryReadStream(data, 16);
	x = stream->readSint32LE();
	y = stream->readSint32LE();
	width = stream->readSint32LE();
	height = stream->readSint32LE();
	delete stream;

	ret = alloc_sprite(width, height);
	ret->dx = x;
	ret->dy = y;
	memcpy(ret->data, ((char*)data) + 16, width * height);
	return ret;
}

TSprite *load_sprite(TDataFile *df, const char *name) {
	void *data;
	TSprite *ret = NULL;

	data = (char*)df->get(name);

	if (data) {
		ret = load_sprite(data);
		memfree(data);
	}

	return ret;
}

void LoadArray(void *array[], int count, TDataFile *df, const char *index, byte mask[])
{
	char nm[MAXNAME];

	if (mask == NULL) {
		for (int i = 0; i < count; i++) {
			snprintf(nm, MAXNAME, index, i);
			nm[MAXNAME - 1] = '\0';
			array[i] = df->get(nm);
		}
	}
	else {
		for (int i = 0; i < count; i++) {
			if (!mask[i]) continue;
			snprintf(nm, MAXNAME, index, i);
			nm[MAXNAME - 1] = '\0';
			array[i] = df->get(nm);
		}
	}
}

void LoadSpriteArray(TSprite *array[], int count, TDataFile *df,
	const char *nametpl, byte *mask) {
	int i;
	size_t size;
	char *assetname;

	size = strlen(nametpl) + 10;
	assetname = (char*)memalloc(size);

	for (i = 0; i < count; i++) {
		if (mask && !mask[i]) {
			continue;
		}

		snprintf(assetname, size, nametpl, i);
		assetname[size - 1] = '\0';
		array[i] = load_sprite(df, assetname);
	}

	memfree(assetname);
}

void DisposeArray(void *array[], int count)
{
	for (int i = 0; i < count; i++) {
		if (array[i] != NULL) {
			memfree(array[i]);
			array[i] = NULL;
		}
	}
}

