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
// Hlavickovy soubor pro MEMMNG.CPP
//
// Obsah: Manager pameti, castecne swapovani ridce pouzivanych 
//        alokovanych dat (like animace strelby u jednotek)
//


#ifndef _MEMMNG_H
#define _MEMMNG_H


#include "system.h"

#include "datafile.h"


#define allocate_memory(x) malloc(x)
#define memrealloc(x, y) realloc(x, y)
#define memalloc allocate_memory
#define memfree(x)  { free(x); x = NULL; }


///////////////////////////
// Fce pro nacteni pole z .DAT a jeho uvolneni z pameti:
//

extern void LoadArray(void *array[], int count, TDataFile *df, const char *index, byte mask[] = NULL);
extern void DisposeArray(void *array[], int count);

#endif
