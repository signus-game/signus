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


// 
// Modifikace jednotek, kter? v dan? misi pojedou do akce...
//


#ifndef _UNMODIF_H
#define _UNMODIF_H


extern void ModifyUnits();
    // umoRn? hr??i zm?nit si obsazen? pro misi

extern void MU_Reset();
    // p?iprav? MU k pouRit?
extern void MU_AddUnit(int ID);
    // d? dalU? jednotku do seznamu zpracovateln2ch



extern void MU_SaveList(FILE *f);
extern void MU_LoadList(FILE *f);
    // zajist? L/S seznamu pouRiteln2ch jednotek...
extern void MU_UpdateList();
    // vygeneruje na konci mise seznam p?eRivU?ch jednotek
extern void MU_ClearList();
    // vz?ist? seznam

#endif
