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
// Artificial Intelligence of Signus
//
//       Local Intelligence
//
//        Global Functions
//
//          Header File
//
// Programmed by Richard Wunsch
//

#ifndef _AIGLOBAL_H
#define _AIGLOBAL_H


#include <math.h>
#include "units.h"


typedef struct {
	byte *w0;
	byte *w1;
	byte *w2;
	byte *w3;
} TUnAnalysePart;


typedef struct {
	byte x, y;
	int Unit;
} TAttackedField;



extern int DeletedUnits [];
extern int DUPos;

extern TAttackedField AttackedField [];
extern int AttackFieldPos;

extern TUnAnalysePart *UnAnalyse;    

extern void AI_DeleteUnit (int ID);

extern void AIError ();

void AnalyseLandscape();
void free_landscape_analysis();

extern void IntegrityTest ();

void loadAttackedFields(ReadStream &stream, int format);
void saveAttackedFields(WriteStream &stream);

#endif
