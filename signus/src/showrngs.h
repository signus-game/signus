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
// Hlavickovy soubor pro zobrazovac dostrelu a dojezdu
//


#ifndef _SHOWRNGS_H
#define _SHOWRNGS_H


#include "units.h"

extern void DrawRangesOnField(int x, int y, int drawx, int drawy);
	// pokud ma dane policko helper, vykresli jej

extern void HideRanges();
extern void ShowRanges();
	// povoli/zakaze zobraz. uk. dosahu


extern int ClearRanges();
	// smaze veskera ramovani

extern void GenerateShootRng(int x, int y, int minrng, int maxrng);
	// vytvori ramovani dostrelu
extern void GenerateMoveRng(int x1, int y1, int x2, int y2, TObject *un);
	// vytvori ramovani dojezdu
extern void GenerateVisibRng(int x, int y, int maxrng);
	// vytvori ramovani dohledu


#endif
