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


// Zobrazovani dojezdu a dostrelu na mape


#include "headers.h"

#include "showrngs.h"
#include "engine.h"
#include "units.h"
#include <math.h>



static int RangesLocks = 0;
	// zamky na kresleni

typedef struct {
		byte x, y;
		byte terrain;
		byte up    : 3;
		byte down  : 3;
		byte left  : 3;
		byte right : 3;
	} TRngItem;

#define MAX_LISTSIZE  400

static TRngItem RngList[MAX_LISTSIZE];
static int RngListSize;



static void AddField(int x, int y, int u, int d, int l, int r)
{
	int pos;
	
	if (GetField(x, y)->HasHelper) {
        int i;
		for (i = 0; i < RngListSize; i++) 
			if ((RngList[i].x == x) && (RngList[i].y == y)) break;
		pos = i;
		RngList[pos].up |= u;
		RngList[pos].down |= d;
		RngList[pos].left |= l;
		RngList[pos].right |= r;
	}
	else {
		pos = RngListSize;
		RngListSize++;
#ifdef DEBUG
		if (RngListSize > MAX_LISTSIZE) PromtBox("DEBUG: Error in SHOWRNG::AddField()", cmOk);
#endif
		RngList[pos].up = u;
		RngList[pos].down = d;
		RngList[pos].left = l;
		RngList[pos].right = r;
	}
	RngList[pos].x = x;
	RngList[pos].y = y;
	RngList[pos].terrain = L1TerrainType[GetField(x, y)->Terrain];
	GetField(x, y)->HasHelper = 1;
}






////////////////// kresleni:

static void RngLine1(int x, int y, int delta, int clr)
{
	int i;
	byte *b = ((byte*)MapBuf) + VIEW_PIXSZ_X * y + x;

	for (i = 0; i < 7; i++) {
		*(b++) = clr, *(b++) = clr, b += delta * VIEW_PIXSZ_X;
		*(b++) = 1, *(b++) = 1, b += delta * VIEW_PIXSZ_X;
	}
}

static void RngLine2(int x, int y, int delta, int clr)
{
	int i;
	byte *b = ((byte*)MapBuf) + VIEW_PIXSZ_X * y + x;

	for (i = 0; i < 7; i++) {
		*(b++) = clr, b += delta * VIEW_PIXSZ_X;
		*(b++) = clr, b += delta * VIEW_PIXSZ_X;
		*(b++) = 1, b += delta * VIEW_PIXSZ_X;
		*(b++) = 1, b += delta * VIEW_PIXSZ_X;
	}	
}

static void RngLine3(int x, int y, int clr)
{
	int i;
	byte *b = ((byte*)MapBuf) + VIEW_PIXSZ_X * y + x;

	for (i = 0; i < 7; i++) 
		*(b++) = clr, *(b++) = clr,	*(b++) = 1, *(b++) = 1;
}





static int candraw(int i) 
{
	return ((i & 1) && iniShowShootRange) || 
	       ((i & 2) && iniShowMoveRange) ||
	       ((i & 4) && iniShowVisibRange);
}

void DrawRangesOnField(int x, int y, int drawx, int drawy)
{
	int l, r, u, d;
	int i;
	int clr;
	TRngItem *it;
	static int clrtab[8] = {0, 9, 4, 4, 92, 9, 4, 4};
	
	if (RangesLocks > 0) return;
	clrtab[3] = iniShowMoveRange ? 4 : 9;
	clrtab[5] = iniShowShootRange ? 9 : 92;
	clrtab[6] = iniShowMoveRange ? 4 : 92;
	clrtab[7] = iniShowMoveRange ? 4 : (iniShowShootRange ? 9 : 92);
	for (i = 0, it = RngList; i < RngListSize; i++, it++) 
		if ((it->x == x) && (it->y == y)) break;
	
	// upper frontier:	
	if ((it->up) && (candraw(it->up))) {
		clr = clrtab[it->up];
		switch (it->terrain) {
			case tofsL1A : case tofsL1B : case tofsL1J : case tofsL1M : 
				RngLine1(drawx + 28, drawy, 1, clr); break;
			case tofsL1G : case tofsL1H : 
				RngLine1(drawx + 28, drawy + 14, 1, clr); break;
			case tofsL1C : case tofsL1F : case tofsL1K :  
				RngLine2(drawx + 28, drawy, 1, clr); break;
			case tofsL1E : case tofsL1I : case tofsL1L : 
				RngLine3(drawx + 28, drawy + 14, clr); break;
			case tofsL1D : 
				RngLine1(drawx + 28, drawy + 14, 1, clr); break;
		}
	}
	// lower frontier:	
	if ((it->down) && (candraw(it->down))) {
		clr = clrtab[it->down];
		switch (it->terrain) {
			case tofsL1A : case tofsL1D : case tofsL1L : case tofsL1K : 
				RngLine1(drawx, drawy + 13, 1, clr); break;
			case tofsL1B : case tofsL1F : case tofsL1I :
				RngLine1(drawx, drawy + 27, 1, clr); break;
			case tofsL1C : case tofsL1G : case tofsL1J :
				RngLine2(drawx, drawy + 12, 1, clr); break;
			case tofsL1E : case tofsL1M : case tofsL1H : 
				RngLine3(drawx, drawy + 27, clr); break;
		}
	}		
	// left frontier:	
	if ((it->left) && (candraw(it->left))) {
		clr = clrtab[it->left];
		switch (it->terrain) {
			case tofsL1A : case tofsL1J : case tofsL1K : case tofsL1C : 
				RngLine1(drawx, drawy + 13, -1, clr); break;
			case tofsL1I : case tofsL1H : case tofsL1E :
				RngLine1(drawx, drawy + 27, -1, clr); break;
			case tofsL1B : case tofsL1F : case tofsL1M :
				RngLine2(drawx, drawy + 27, -1, clr); break;
			case tofsL1D : case tofsL1G : case tofsL1L : 
				RngLine3(drawx, drawy + 14, clr); break;
		}
	}
	// right frontier:	
	if ((it->right) && (candraw(it->right))) {
		clr = clrtab[it->right];
		switch (it->terrain) {
			case tofsL1A : case tofsL1E : case tofsL1M : case tofsL1L : 
				RngLine1(drawx + 27, drawy + 26, -1, clr); break;
			case tofsL1F : case tofsL1C : case tofsL1G :
				RngLine1(drawx + 27, drawy + 40, -1, clr); break;
			case tofsL1B : case tofsL1J : case tofsL1I :
				RngLine2(drawx + 27, drawy + 40, -1, clr); break;
			case tofsL1D : case tofsL1K : case tofsL1H : 
				RngLine3(drawx + 27, drawy + 27, clr); break;
			}
	}
}












////////////////// zamykani:

void HideRanges()
{
	RangesLocks++;
	if (RangesLocks == 1) RedrawMap();
}

void ShowRanges()
{
	RangesLocks--;
	if (RangesLocks == 0) RedrawMap();
}











////////////////// generovani dostrelu:


int ClearRanges()
{
	int rn = RngListSize;	
	RngListSize = 0;
	for (int i = 0; i < MapSizeX * MapSizeY; i++) Map[i].HasHelper = 0;
	return ((rn != 0) && (iniShowMoveRange || iniShowShootRange));
}




//// pomocne fce a promnene:

static double Pythagoras(double x, double y)
{
	return sqrt(x * x + y * y);
}


static byte rngMap[256*256/8];   //8kB buf. na pomoc. info

static void rngMapClear() {memset(rngMap, 0, 256*256/8);}

static int rngMapGet(int x, int y) {
	return (rngMap[(x + y * 256) / 8] & (1 << (x % 8)));
}

static void rngMapSet(int x, int y, int val) {
	rngMap[(x + y * 256) / 8] |= (1 << (x % 8));
}








/// dostrel:

void GenerateShootRng(int x, int y, int minrng, int maxrng)
{
	int i, j, p;
	int u, l, r, d;
	int x1 = x - maxrng, y1 = y - maxrng, 
	    x2 = x + maxrng, y2 = y + maxrng;
	if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0;
	if (x2 > MapSizeX-1) x2 = MapSizeX-1; if (y2 > MapSizeY-1) y2 = MapSizeY-1;
	
	rngMapClear();
	for (i = x1; i <= x2; i++)
		for (j = y1; j <= y2; j++) {
			p = (int)Pythagoras(x - i, y - j);
			if ((p <= maxrng) && (p >= minrng))	rngMapSet(i, j, 1);
		}
	if (minrng == 1) rngMapSet(x, y, 1);

	for (i = x1; i <= x2; i++)
		for (j = y1; j <= y2; j++) {
			if (rngMapGet(i, j) == 0) continue;
			u = l = r = d = 0;
			if ((i > 0) && (rngMapGet(i-1, j) == 0)) l = 1;
			if ((i < MapSizeX-1) && (rngMapGet(i+1, j) == 0)) r = 1;
			if ((j > 0) && (rngMapGet(i, j-1) == 0)) u = 1;
			if ((j < MapSizeY-1) && (rngMapGet(i, j+1) == 0)) d = 1;
			if ((u != 0) || (d != 0) || (l != 0) || (r != 0)) 
				AddField(i, j, u, d, l, r);
		}
}



/// dojezd:

void GenerateMoveRng(int x1, int y1, int x2, int y2, TObject *un)
{
	int i, j;
	int u, d, l, r;

	rngMapClear();
	for (i = x1; i <= x2; i++)
		for (j = y1; j <= y2; j++) {
			if (un->TimeToMove(i, j) > 0) rngMapSet(i, j, 1);
		}
	for (i = x1+1; i <= x2-1; i++)
		for (j = y1+1; j <= y2-1; j++) {
			if ((!rngMapGet(i, j)) &&
					rngMapGet(i-1, j) && rngMapGet(i+1, j) &&
			    rngMapGet(i, j-1) && rngMapGet(i, j+1))
				rngMapSet(i, j, 1);
		}

	for (i = x1; i <= x2; i++)
		for (j = y1; j <= y2; j++) {
			if (rngMapGet(i, j) == 0) continue;
			u = l = r = d = 0;
			if ((i > 0) && (rngMapGet(i-1, j) == 0)) l = 2;
			if ((i < MapSizeX-1) && (rngMapGet(i+1, j) == 0)) r = 2;
			if ((j > 0) && (rngMapGet(i, j-1) == 0)) u = 2;
			if ((j < MapSizeY-1) && (rngMapGet(i, j+1) == 0)) d = 2;
			if ((u != 0) || (d != 0) || (l != 0) || (r != 0)) 
				AddField(i, j, u, d, l, r);
		}
}








/// dohled:

void GenerateVisibRng(int x, int y, int maxrng)
{
	int i, j, p;
	int u, l, r, d;
	int x1 = x - maxrng, y1 = y - maxrng, 
	    x2 = x + maxrng, y2 = y + maxrng;
	if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0;
	if (x2 > MapSizeX-1) x2 = MapSizeX-1; if (y2 > MapSizeY-1) y2 = MapSizeY-1;
	
	rngMapClear();
	for (i = x1; i <= x2; i++)
		for (j = y1; j <= y2; j++) {
			p = (int)Pythagoras(x - i, y - j);
			if (p <= maxrng) rngMapSet(i, j, 1);
		}

	for (i = x1; i <= x2; i++)
		for (j = y1; j <= y2; j++) {
			if (rngMapGet(i, j) == 0) continue;
			u = l = r = d = 0;
			if ((i > 0) && (rngMapGet(i-1, j) == 0)) l = 4;
			if ((i < MapSizeX-1) && (rngMapGet(i+1, j) == 0)) r = 4;
			if ((j > 0) && (rngMapGet(i, j-1) == 0)) u = 4;
			if ((j < MapSizeY-1) && (rngMapGet(i, j+1) == 0)) d = 4;
			if ((u != 0) || (d != 0) || (l != 0) || (r != 0)) 
				AddField(i, j, u, d, l, r);
		}
}


