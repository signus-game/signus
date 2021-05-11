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


// 
// Hlavickovy soubor pro nacitani a ukladani misi
//


#ifndef _LOADSAVE_H
#define _LOADSAVE_H

#include "stream.h"

#define DEFAULT_SAVEGAME_FORMAT 1

extern int SaveGame();
    // ulozi misi a vraci uspech
extern int LoadGame();
    // nacte misi, inicializuje engine a vraci uspech


int SaveGameState(WriteStream &stream);
int LoadGameState(ReadStream &stream, int format = DEFAULT_SAVEGAME_FORMAT);
    // provadi prime I/O daneho savu


#endif
