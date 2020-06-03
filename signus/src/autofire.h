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

AUTOFIRE.CPP

Modul na osetreni automaticke strelby behem tahu 
toho druheho

*/


#ifndef _AUTOFIRE_H
#define _AUTOFIRE_H

#include "units.h"
#include "stream.h"



extern void InitAutofire();
		// primarni inicializace, vola se z ReadUnits()
extern void DoneAutofire();
		// deinicializace, vola se z DoneUnits()
		
extern void ResetAutofire(int party);
		// nastavi parametry podle strany, ktera ma svuj tah

extern int CanAutofireOn(TUnit *unit);
		// vraci jestli je mozne autosejmout danou jednotku
		
extern void DoAutofire(TUnit *unit);
		// sejme veci na tom danem policku - jednotku u

extern void RemoveFromAutofire(int ID);
		// vyjme znicenou jednotku

extern void LoadAutofire(ReadStream &stream);
extern void SaveAutofire(WriteStream &stream);
		// I/O nad autofirem

#endif
