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


#include "headers.h"

#include "memmng.h"
#include <malloc.h>
#include "global.h"
#include "system.h"



////////////////////////////// ALOKACE POLI //////////////////////////////

#include "datafile.h"

// Fce pro nacteni pole z .DAT a jeho uvolneni z pameti:

void LoadArray(void *array[], int count, TDataFile *df, char *index, byte mask[])
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



void DisposeArray(void *array[], int count)
{
	for (int i = 0; i < count; i++) {
		if (array[i] != NULL) {
			memfree(array[i]);
			array[i] = NULL;
		}
	}
}

