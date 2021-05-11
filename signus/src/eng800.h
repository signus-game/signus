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

extern void *BmpTerr1[1024];
extern void *BmpTerr2[1024];
extern void *BmpTerr1D[1024];
extern void *BmpTerr2D[1024];
extern void *BmpSel[13], *BmpSelBold[13];
extern void *LocalBufX, *LocalBufY;


// Tile rendering
void DrawSprite(int x, int y, TSprite *s);
void DrawField(int x, int y);
void DrawL2Selector(int drawx, int drawy, word Ter1, void *BmpSl[]);
