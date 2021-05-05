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




// Pametovy manager pro Signus:


#include "memmng.h"
#include <stdlib.h>
#include "global.h"
#include "system.h"
#include "engine.h"


////////////////////////////// ALOKACE POLI //////////////////////////////

#include "datafile.h"

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

	ret = (TSprite*)memalloc(width * height + offsetof(TSprite, data));
	ret->dx = x;
	ret->dy = y;
	ret->w = width;
	ret->h = height;
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
	char nm[32];

	if (mask == NULL) {
		for (int i = 0; i < count; i++) {
			sprintf(nm, index, i);
			array[i] = df->get(nm);
		}
	}
	else {
		for (int i = 0; i < count; i++) {
			if (!mask[i]) continue;
			sprintf(nm, index, i);
			array[i] = df->get(nm);
		}
	}
}

void LoadSpriteArray(TSprite *array[], int count, TDataFile *df,
	const char *nametpl, byte *mask) {
	int i;
	char *assetname;

	assetname = (char*)memalloc(strlen(nametpl) + 10);

	for (i = 0; i < count; i++) {
		if (mask && !mask[i]) {
			continue;
		}

		sprintf(assetname, nametpl, i);
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

