/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002
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


// Tabulky pro ENGINE a UNITS atd.

#ifndef _ETABLES_H
#define _ETABLES_H

#include "system.h"



// Tabulka typu terenu podle tvaru:

extern byte L1TerrainType[1024];


// Tabulka optimalizace kresleni:

extern byte DontDrawL2[1024];



// Tabulky pro mapu:

extern byte TabTerrIcons[1024];
extern byte TabTerr2Icons[1024];

extern byte TabTerrIconsD[1024];
extern byte TabTerr2IconsD[1024];




// Tabulky pro pohyb jednotek:

extern int TabTerrMoveGround[1024];
extern int TabTerr2MoveGround[1024];

extern int TabTerrMoveWater[1024];
extern int TabTerr2MoveWater[1024];

extern int TabTerrMoveTrain[1024];
extern int TabTerr2MoveTrain[1024];

extern int TabTerrMoveAmphi[1024];
extern int TabTerr2MoveAmphi[1024];

extern int TabTerrMoveAir[1024];
extern int TabTerr2MoveAir[1024];



// Tabulka modifikaci vysek behem vypoctu viditelnosti a strelby:
// (vztahuje se na L2 tereny...)

extern int TabHghtModif[1024];
extern int TabHghtModifVisib[1024];


// Postupove tabulky zkusenosti:

extern int TabUnitsExperience[][10];
extern int TabUnitsCost[];
extern int TabUnitsProducTime[];

extern double TabCareful[55];

extern double TabDanger[55];

#endif

