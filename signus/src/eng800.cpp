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


     ////////////////////////////////////////////////////////////
     //                   VESA 800x600                         //
     ////////////////////////////////////////////////////////////


// ENGINE fce zavisle na rozliseni






void DrawSprite(int x, int y, TSprite *s)
{
	int dx, dy;
	byte sx, sy;
	int ax = x - s->dx;
	int ay = y - s->dy;
	TRect r = {ax, ay, ax + s->w-1, ay + s ->h-1};

	if (!Intersect(&r, &DrawingArea)) return;
	dx = r.x1 - ax; dy = r.y1 - ay;
	sx = r.x2 - r.x1 + 1; sy = r.y2 - r.y1 + 1;
	PutSpritePart((byte *)FullBuf + (VIEW_OFS_X + r.x1 + (VIEW_OFS_Y + r.y1) * VIEW_PIXSZ_X),
								(sy << 8) + sx,
								s->data + (dx + dy * s->w),
								s->w - sx);
}







void DrawField(int x, int y)
{
	TField *f = GetField(x, y);
	word Ter1 = f->Terrain;
	word Ter2 = f->Terrain2;
	
	int rx = GetRelX(x), ry = GetRelY(y);
	int drawx = VIEW_OFS_X + 28 * (rx - ry);
	int drawy = VIEW_OFS_Y + 14 * (rx + ry - (f->Height));
	int localx = drawx/2, localy = drawy/2;
	int ldx = (x < 0) ? 0xFF : ((x >= MapSizeX) ? 0xFF : x);
	int ldy = (y < 0) ? 0xFF : ((y >= MapSizeY) ? 0xFF : y);

	if ((drawx < 0) || (drawy < 0) || (drawx > VIEW_PIXSZ_X - FIELD_X) || 
	    (drawy > VIEW_PIXSZ_Y - TerrOfssEnd[Ter1])) return;

	// PLNA VIDITELNOST:
	
	if (f->Visib == 2) {
		// Prvni vrstva terenu & lokalizace:
		if ((Ter1 % 256) < tofsL1B) {
			DrawTerrA(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalA(LocalBufX, localx, localy, ldx);
			DrawLocalA(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1C) {
			DrawTerrB(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalB(LocalBufX, localx, localy, ldx);
			DrawLocalB(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1D) {
			DrawTerrC(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalC(LocalBufX, localx, localy, ldx);
			DrawLocalC(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1E) {
			DrawTerrD(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalD(LocalBufX, localx, localy, ldx);
			DrawLocalD(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1F) {
			DrawTerrE(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalE(LocalBufX, localx, localy, ldx);
			DrawLocalE(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1G) {
			DrawTerrF(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalF(LocalBufX, localx, localy, ldx);
			DrawLocalF(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1H) {
			DrawTerrG(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalG(LocalBufX, localx, localy, ldx);
			DrawLocalG(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1I) {
			DrawTerrH(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalH(LocalBufX, localx, localy, ldx);
			DrawLocalH(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1J) {
			DrawTerrI(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalI(LocalBufX, localx, localy, ldx);
			DrawLocalI(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1K) {
			DrawTerrJ(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalJ(LocalBufX, localx, localy, ldx);
			DrawLocalJ(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1L) {
			DrawTerrK(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalK(LocalBufX, localx, localy, ldx);
			DrawLocalK(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1M) {
			DrawTerrL(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalL(LocalBufX, localx, localy, ldx);
			DrawLocalL(LocalBufY, localx, localy, ldy);
		}
		else {
			DrawTerrM(drawx, drawy, BmpTerr1[Ter1]);
			DrawLocalM(LocalBufX, localx, localy, ldx);
			DrawLocalM(LocalBufY, localx, localy, ldy);
		}
	
		// Druha vrstva terenu:
		if (Ter2 != 0) {
			if (Ter2 < tofsL2B)
				DrawUpTerrA(drawx, drawy, BmpTerr2[Ter2]);
			else if (Ter2 < tofsL2C)
				DrawUpTerrB(drawx, drawy, BmpTerr2[Ter2]);
			else if (Ter2 < tofsL2D)
				DrawUpTerrC(drawx, drawy, BmpTerr2[Ter2]);
			else if (Ter2 < tofsL2E)
				DrawUpTerrD(drawx, drawy, BmpTerr2[Ter2]);
			else if (Ter2 < tofsL2Spec)
				DrawUpTerrE(drawx, drawy, BmpTerr2[Ter2]);
		}
		
		
		// Pomocne zobrazovace:
		if (f->HasHelper) DrawRangesOnField(x, y, drawx, drawy);
		// Jednotky:
//		if (f->Unit != 0xFF) {}
	}
	
	
	
	// CASTECNA VIDITELNOST:
	else if (f->Visib == 1) {
		// Prvni vrstva terenu & lokalizace:
		if ((Ter1 % 256) < tofsL1B) {
			DrawTerrA(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalA(LocalBufX, localx, localy, ldx);
			DrawLocalA(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1C) {
			DrawTerrB(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalB(LocalBufX, localx, localy, ldx);
			DrawLocalB(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1D) {
			DrawTerrC(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalC(LocalBufX, localx, localy, ldx);
			DrawLocalC(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1E) {
			DrawTerrD(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalD(LocalBufX, localx, localy, ldx);
			DrawLocalD(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1F) {
			DrawTerrE(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalE(LocalBufX, localx, localy, ldx);
			DrawLocalE(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1G) {
			DrawTerrF(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalF(LocalBufX, localx, localy, ldx);
			DrawLocalF(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1H) {
			DrawTerrG(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalG(LocalBufX, localx, localy, ldx);
			DrawLocalG(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1I) {
			DrawTerrH(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalH(LocalBufX, localx, localy, ldx);
			DrawLocalH(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1J) {
			DrawTerrI(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalI(LocalBufX, localx, localy, ldx);
			DrawLocalI(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1K) {
			DrawTerrJ(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalJ(LocalBufX, localx, localy, ldx);
			DrawLocalJ(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1L) {
			DrawTerrK(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalK(LocalBufX, localx, localy, ldx);
			DrawLocalK(LocalBufY, localx, localy, ldy);
		}
		else if ((Ter1 % 256) < tofsL1M) {
			DrawTerrL(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalL(LocalBufX, localx, localy, ldx);
			DrawLocalL(LocalBufY, localx, localy, ldy);
		}
		else {
			DrawTerrM(drawx, drawy, BmpTerr1D[Ter1]);
			DrawLocalM(LocalBufX, localx, localy, ldx);
			DrawLocalM(LocalBufY, localx, localy, ldy);
		}
	
		// Druha vrstva terenu:
		if (Ter2 != 0) {
			if (!DontDrawL2[Ter2]) {
				if (Ter2 < tofsL2B)
					DrawUpTerrA(drawx, drawy, BmpTerr2D[Ter2]);
				else if (Ter2 < tofsL2C)
					DrawUpTerrB(drawx, drawy, BmpTerr2D[Ter2]);
				else if (Ter2 < tofsL2D)
					DrawUpTerrC(drawx, drawy, BmpTerr2D[Ter2]);
				else if (Ter2 < tofsL2E)
					DrawUpTerrD(drawx, drawy, BmpTerr2D[Ter2]);
				else if (Ter2 < tofsL2Spec)
					DrawUpTerrE(drawx, drawy, BmpTerr2D[Ter2]);
			}
		}

		// Pomocne zobrazovace:
		if (f->HasHelper) DrawRangesOnField(x, y, drawx, drawy);
	}
	
	
	
	// NULOVA VIDITELNOST:
	else {
		// Prvni vrstva terenu & lokalizace:
		if ((Ter1 % 256) < tofsL1B) {
			DrawTerrA(drawx, drawy, BmpTerr1[0]);
			DrawLocalA(LocalBufX, localx, localy, 0xFF);
			DrawLocalA(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1C) {
			DrawTerrB(drawx, drawy, BmpTerr1[tofsL1B]);
			DrawLocalB(LocalBufX, localx, localy, 0xFF);
			DrawLocalB(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1D) {
			DrawTerrC(drawx, drawy, BmpTerr1[tofsL1C]);
			DrawLocalC(LocalBufX, localx, localy, 0xFF);
			DrawLocalC(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1E) {
			DrawTerrD(drawx, drawy, BmpTerr1[tofsL1D]);
			DrawLocalD(LocalBufX, localx, localy, 0xFF);
			DrawLocalD(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1F) {
			DrawTerrE(drawx, drawy, BmpTerr1[tofsL1E]);
			DrawLocalE(LocalBufX, localx, localy, 0xFF);
			DrawLocalE(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1G) {
			DrawTerrF(drawx, drawy, BmpTerr1[tofsL1F]);
			DrawLocalF(LocalBufX, localx, localy, 0xFF);
			DrawLocalF(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1H) {
			DrawTerrG(drawx, drawy, BmpTerr1[tofsL1G]);
			DrawLocalG(LocalBufX, localx, localy, 0xFF);
			DrawLocalG(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1I) {
			DrawTerrH(drawx, drawy, BmpTerr1[tofsL1H]);
			DrawLocalH(LocalBufX, localx, localy, 0xFF);
			DrawLocalH(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1J) {
			DrawTerrI(drawx, drawy, BmpTerr1[tofsL1I]);
			DrawLocalI(LocalBufX, localx, localy, 0xFF);
			DrawLocalI(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1K) {
			DrawTerrJ(drawx, drawy, BmpTerr1[tofsL1J]);
			DrawLocalJ(LocalBufX, localx, localy, 0xFF);
			DrawLocalJ(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1L) {
			DrawTerrK(drawx, drawy, BmpTerr1[tofsL1K]);
			DrawLocalK(LocalBufX, localx, localy, 0xFF);
			DrawLocalK(LocalBufY, localx, localy, 0xFF);
		}
		else if ((Ter1 % 256) < tofsL1M) {
			DrawTerrL(drawx, drawy, BmpTerr1[tofsL1L]);
			DrawLocalL(LocalBufX, localx, localy, 0xFF);
			DrawLocalL(LocalBufY, localx, localy, 0xFF);
		}
		else {
			DrawTerrM(drawx, drawy, BmpTerr1[tofsL1M]);
			DrawLocalM(LocalBufX, localx, localy, 0xFF);
			DrawLocalM(LocalBufY, localx, localy, 0xFF);
		}
	}
	
}






void DrawL2Selector(int drawx, int drawy, word Ter1, void *BmpSl[])
{
	if ((Ter1 % 256) < tofsL1B)      DrawUpTerrA(drawx, drawy, BmpSl[0]);
	else if ((Ter1 % 256) < tofsL1C) DrawUpTerrB(drawx, drawy, BmpSl[1]);
	else if ((Ter1 % 256) < tofsL1D) DrawUpTerrC(drawx, drawy, BmpSl[2]);
	else if ((Ter1 % 256) < tofsL1E) DrawUpTerrD(drawx, drawy, BmpSl[3]);
	else if ((Ter1 % 256) < tofsL1F) DrawUpTerrE(drawx, drawy, BmpSl[4]);
	else if ((Ter1 % 256) < tofsL1G) DrawUpTerrF(drawx, drawy, BmpSl[5]);
	else if ((Ter1 % 256) < tofsL1H) DrawUpTerrG(drawx, drawy, BmpSl[6]);
	else if ((Ter1 % 256) < tofsL1I) DrawUpTerrH(drawx, drawy, BmpSl[7]);
	else if ((Ter1 % 256) < tofsL1J) DrawUpTerrI(drawx, drawy, BmpSl[8]);
	else if ((Ter1 % 256) < tofsL1K) DrawUpTerrJ(drawx, drawy, BmpSl[9]);
	else if ((Ter1 % 256) < tofsL1L) DrawUpTerrK(drawx, drawy, BmpSl[10]);
	else if ((Ter1 % 256) < tofsL1M) DrawUpTerrL(drawx, drawy, BmpSl[11]);
	else                     DrawUpTerrM(drawx, drawy, BmpSl[12]);
}


