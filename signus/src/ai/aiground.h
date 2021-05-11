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
//      Object: TGroundArmy
//
//          Header File
//
// Programmed by Richard Wunsch
//

#ifndef _AI_GROUND_H
#define _AI_GROUND_H

#include "ai.h"

class TGroundArmy : public TArmy {
	public:
		TGroundArmy ();	
		TGroundArmy (ReadStream &stream);
		
		virtual void InsertUnits ();		
		virtual int ProblemJednotky (int Unit);
		virtual int PJGroundFight (int UnID);
		virtual int PJBumerang (int Unit);
		virtual int PJCeres (int UnID);
		virtual int PJGnom (int UnID);
		virtual int PJUran (int UnID);
		virtual int PJRex (int UnID);
		virtual int PJMedea (int UnID);
		virtual int PJSkiatron (int UnID);
		virtual int PJIris (int UnID);
		virtual int PJGanymedes (int UnID);
		virtual int PJSatan (int UnID);
		virtual TPoint SatanBP (TPoint start, int UnID, int Time);
		virtual	double SatanPS (int moverID, TPoint pozice);	
		virtual int PJGargantua (int UnID);


    virtual int MakeTurn ();
	
	
};



extern TGroundArmy *Army1, *Army2, *Army3, *Army4, *Army5, *Army6, *Army7, *Army8;
extern TGroundArmy *Army9, *Army10, *Army11, *Army12, *Army13, *Army14, *Army15, *Army16;
extern TPoint *MBPlaces;
extern TPoint *DeniedPlaces;

#endif
