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



#ifndef _AI_MARINE_H
#define _AI_MARINE_H

#include "ai.h"

class TMarine : public TArmy {
	public:
		TMarine ();	
		TMarine (FILE *f);
		
		virtual void InsertUnits ();
		virtual int ProblemJednotky (int Unit);
		virtual int PJShip (int UnID);
		
    virtual int MakeTurn ();
	
	
};



extern TMarine *Marine1, *Marine2, *Marine3, *Marine4, *Marine5;
extern TMarine *Marine6, *Marine7, *Marine8, *Marine9, *Marine10;

#endif
