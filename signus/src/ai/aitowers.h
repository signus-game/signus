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
// Artificial Intelligence of Signus
//
//       Local Intelligence
//
//         Object: TTowers
//
//          Header File
//
// Programmed by Richard Wunsch
//

#ifndef _AI_TOWERS_H
#define _AI_TOWERS_H

#include "ai.h"

class TTowers : public TArmy {
	public:
		TTowers ();	
		TTowers (FILE *f);
		
		virtual int ProblemJednotky(int Unit);
		virtual int MakeTurn ();
		virtual int DeleteKilled ();
	
	
};



extern TTowers *Towers;

#endif
