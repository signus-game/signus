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
// Artificial Intelligence of Signus
//
//       Local Intelligence
//
//         Object: TAirArmy
//
//          Header File
//
// Programmed by Richard Wunsch
//

#ifndef _AI_AIR_H
#define _AI_AIR_H

#include "ai.h"

class TAirArmy : public TArmy {
	public:
		TAirArmy ();	
		TAirArmy (FILE *f);
		
		virtual void InsertUnits ();
		virtual int ProblemJednotky (int Unit);
		virtual int PJIkaros (int UnID);
		virtual int PJKobra (int UnID);
		virtual int PJSaturn (int UnID);
		virtual int PJMystik (int UnID);
		virtual int PJRex (int UnID);
		virtual int PJProton (int UnID);
		
    virtual int MakeTurn ();
	
	
};



extern TAirArmy *AirArmy1, *AirArmy2, *AirArmy3, *AirArmy4, *AirArmy5;
extern TAirArmy *AirArmy6, *AirArmy7, *AirArmy8, *AirArmy9, *AirArmy10;

#endif
