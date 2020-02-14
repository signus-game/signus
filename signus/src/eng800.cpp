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


     ////////////////////////////////////////////////////////////
     //                   VESA 800x600                         //
     ////////////////////////////////////////////////////////////


// ENGINE fce zavisle na rozliseni


#define TILE_TYPE_COUNT 13

typedef struct {
	unsigned offset, length;
} tilerow_t;

tilerow_t tilescan[TILE_TYPE_COUNT][57] = {
	{	// tile type A (784 bytes)
		{26, 4}, {24, 8}, {22, 12}, {20, 16}, {18, 20}, {16, 24},
		{14, 28}, {12, 32}, {10, 36}, {8, 40}, {6, 44}, {4, 48},
		{2, 52}, {0, 56}, {2, 52}, {4, 48}, {6, 44}, {8, 40}, {10, 36},
		{12, 32}, {14, 28}, {16, 24}, {18, 20}, {20, 16}, {22, 12},
		{24, 8}, {26, 4}, {0, 0}
	},
	{	// tile type B (1176 bytes)
		{26, 4}, {25, 7}, {24, 10}, {23, 13}, {22, 16}, {21, 19},
		{20, 22}, {19, 25}, {18, 28}, {17, 31}, {16, 34}, {15, 37},
		{14, 40}, {13, 43}, {12, 42}, {11, 42}, {10, 42}, {9, 42},
		{8, 42}, {7, 42}, {6, 42}, {5, 42}, {4, 42}, {3, 42}, {2, 42},
		{1, 42}, {0, 42}, {0, 41}, {2, 38}, {4, 35}, {6, 32}, {8, 29},
		{10, 26}, {12, 23}, {14, 20}, {16, 17}, {18, 14}, {20, 11},
		{22, 8}, {24, 5}, {26, 2}, {0, 0}
	},
	{	// tile type C (1176 bytes)
		{26, 4}, {24, 7}, {22, 10}, {20, 13}, {18, 16}, {16, 19},
		{14, 22}, {12, 25}, {10, 28}, {8, 31}, {6, 34}, {4, 37},
		{2, 40}, {0, 43}, {2, 42}, {3, 42}, {4, 42}, {5, 42}, {6, 42},
		{7, 42}, {8, 42}, {9, 42}, {10, 42}, {11, 42}, {12, 42},
		{13, 42}, {14, 42}, {15, 41}, {16, 38}, {17, 35}, {18, 32},
		{19, 29}, {20, 26}, {21, 23}, {22, 20}, {23, 17}, {24, 14},
		{25, 11}, {26, 8}, {27, 5}, {28, 2}, {0, 0}
	},
	{	// tile type D (392 bytes)
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0},
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {2, 28},
		{4, 28}, {6, 28}, {8, 28}, {10, 28}, {12, 28}, {14, 28},
		{16, 28}, {18, 28}, {20, 28}, {22, 28}, {24, 28}, {26, 28},
		{28, 28}, {0, 0}
	},
	{	// tile type E (392 bytes)
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0},
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {26, 28},
		{24, 28}, {22, 28}, {20, 28}, {18, 28}, {16, 28}, {14, 28},
		{12, 28}, {10, 28}, {8, 28}, {6, 28}, {4, 28}, {2, 28},
		{0, 28}, {0, 0}
	},
	{	// tile type F (1230 bytes)
		{26, 4}, {25, 6}, {24, 8}, {23, 10}, {22, 12}, {21, 14},
		{20, 16}, {19, 18}, {18, 20}, {17, 22}, {16, 24}, {15, 26},
		{14, 28}, {13, 30}, {12, 32}, {11, 34}, {10, 36}, {9, 38},
		{8, 40}, {7, 42}, {6, 44}, {5, 46}, {4, 48}, {3, 50}, {2, 52},
		{1, 54}, {0, 56}, {0, 56}, {2, 52}, {4, 48}, {6, 44}, {8, 40},
		{10, 36}, {12, 32}, {14, 28}, {16, 24}, {18, 20}, {20, 16},
		{22, 12}, {24, 8}, {26, 4}, {0, 0}
	},
	{	// tile type G (743 bytes)
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0},
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {2, 28},
		{3, 29}, {4, 30}, {5, 31}, {6, 32}, {7, 33}, {8, 34}, {9, 35},
		{10, 36}, {11, 37}, {12, 38}, {13, 39}, {14, 40}, {15, 41},
		{16, 38}, {17, 35}, {18, 32}, {19, 29}, {20, 26}, {21, 23},
		{22, 20}, {23, 17}, {24, 14}, {25, 11}, {26, 8}, {27, 5},
		{28, 2}, {0, 0}
	},
	{	// tile type H (420 bytes)
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0},
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {26, 4},
		{24, 8}, {22, 12}, {20, 16}, {18, 20}, {16, 24}, {14, 28},
		{12, 32}, {10, 36}, {8, 40}, {6, 44}, {4, 48}, {2, 52},
		{0, 56}, {0, 0}
	},
	{	// tile type I (743 bytes)
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0},
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {26, 28},
		{24, 29}, {22, 30}, {20, 31}, {18, 32}, {16, 33}, {14, 34},
		{12, 35}, {10, 36}, {8, 37}, {6, 38}, {4, 39}, {2, 40},
		{0, 41}, {2, 38}, {4, 35}, {6, 32}, {8, 29}, {10, 26},
		{12, 23}, {14, 20}, {16, 17}, {18, 14}, {20, 11}, {22, 8},
		{24, 5}, {26, 2}, {0, 0}
	},
	{	// tile type J (1122 bytes)
		{26, 4}, {24, 8}, {22, 12}, {20, 16}, {18, 20}, {16, 24},
		{14, 28}, {12, 32}, {10, 36}, {8, 40}, {6, 44}, {4, 48},
		{2, 52}, {0, 56}, {2, 52}, {3, 50}, {4, 48}, {5, 46}, {6, 44},
		{7, 42}, {8, 40}, {9, 38}, {10, 36}, {11, 34}, {12, 32},
		{13, 30}, {14, 28}, {15, 26}, {16, 24}, {17, 22}, {18, 20},
		{19, 18}, {20, 16}, {21, 14}, {22, 12}, {23, 10}, {24, 8},
		{25, 6}, {26, 4}, {27, 2}, {0, 0}
	},
	{	// tile type K (825 bytes)
		{26, 4}, {24, 7}, {22, 10}, {20, 13}, {18, 16}, {16, 19},
		{14, 22}, {12, 25}, {10, 28}, {8, 31}, {6, 34}, {4, 37},
		{2, 40}, {0, 43}, {2, 42}, {4, 41}, {6, 40}, {8, 39}, {10, 38},
		{12, 37}, {14, 36}, {16, 35}, {18, 34}, {20, 33}, {22, 32},
		{24, 31}, {26, 30}, {28, 28}, {0, 0}
	},
	{	// tile type L (364 bytes)
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0},
		{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {2, 52},
		{4, 48}, {6, 44}, {8, 40}, {10, 36}, {12, 32}, {14, 28},
		{16, 24}, {18, 20}, {20, 16}, {22, 12}, {24, 8}, {26, 4},
		{0, 0}
	},
	{	// tile type M (825 bytes)
		{26, 4}, {25, 7}, {24, 10}, {23, 13}, {22, 16}, {21, 19},
		{20, 22}, {19, 25}, {18, 28}, {17, 31}, {16, 34}, {15, 37},
		{14, 40}, {13, 43}, {12, 42}, {11, 41}, {10, 40}, {9, 39},
		{8, 38}, {7, 37}, {6, 36}, {5, 35}, {4, 34}, {3, 33}, {2, 32},
		{1, 31}, {0, 30}, {0, 28}, {0, 0}
	}
};

// Terrain number to tilescan ID (sprite shape) mapping
unsigned tileTypeListL1[TILE_TYPE_COUNT + 1] = {
	tofsL1A, tofsL1B, tofsL1C, tofsL1D, tofsL1E, tofsL1F, tofsL1G, tofsL1H,
	tofsL1I, tofsL1J, tofsL1K, tofsL1L, tofsL1M, 0
};

static unsigned tileTypeListL2[] = {
	tofsL2B, tofsL2C, tofsL2D, tofsL2E, 0
};

// Terrain number to tilescan ID conversion functions
unsigned tileTypeL1(unsigned terrain) {
	unsigned ret = 1;

	for (; tileTypeListL1[ret] && terrain >= tileTypeListL1[ret]; ret++);
	return ret - 1;
}

unsigned tileTypeL2(unsigned terrain) {
	unsigned ret = 0;

	for (; tileTypeListL2[ret] && terrain >= tileTypeListL2[ret]; ret++);
	return ret;
}

// Tile rendering
void drawSolidTile(uint8_t *dest, unsigned destwidth, int x, int y,
	const uint8_t *tiledata, unsigned type) {
	const tilerow_t *scan;

	if (type >= TILE_TYPE_COUNT) {
		return;
	}

	scan = tilescan[type];
	dest += x + y * destwidth;

	for (; scan->offset || scan->length; scan++, dest += destwidth) {
		memcpy(dest + scan->offset, tiledata, scan->length);
		tiledata += scan->length;
	}
}

void drawTransparentTile(uint8_t *dest, unsigned destwidth, int x, int y,
	const uint8_t *tiledata, unsigned type) {
	const tilerow_t *scan;
	unsigned i;

	if (type >= TILE_TYPE_COUNT) {
		return;
	}

	scan = tilescan[type];
	dest += x + y * destwidth;

	for (; scan->offset || scan->length; scan++, dest += destwidth) {
		for (i = 0; i < scan->length; i++) {
			if (tiledata[i]) {
				dest[scan->offset + i] = tiledata[i];
			}
		}

		tiledata += scan->length;
	}
}

void fillTile(uint8_t *dest, unsigned destwidth, int x, int y, uint8_t color,
	unsigned type) {
	const tilerow_t *scan;

	if (type >= TILE_TYPE_COUNT) {
		return;
	}

	scan = tilescan[type];
	dest += x + y * destwidth;

	for (; scan->offset || scan->length; scan++, dest += destwidth) {
		memset(dest + scan->offset / 2, color, scan->length / 2);
		scan++;

		if (!scan->offset && !scan->length) {
			break;
		}
	}
}

void PutSpritePart1(uint8_t *screen, int sizes, uint8_t *data, int adding)
{
	int size_low = sizes & 0xff;
	int size_high = sizes >> 8;
	int cl = size_low;
	int ch = size_high;

	while (true) {
			if (*data != 0) {
				if (*data == 63) {
					*screen = DarkingTable[*screen];
				} else {
					*screen = *data;
				}
			}

			screen++;
			data++;

			cl--;
			if (cl != 0) {
				continue;
			}

			cl = size_low;
			data += adding;
			screen += VIEW_PIXSZ_X - size_low;

			ch--;
			if (ch != 0) {
				continue;
			}

			return;
	}
}

void PutSpritePart(void *screen, int sizes, void *data, int adding)
{
	PutSpritePart1((uint8_t*)screen, sizes, (uint8_t*)data, adding);
}

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
	int show_helpers = 1;
	unsigned tile_type;
	const uint8_t *sprite1 = NULL, *sprite2 = NULL;

	if ((drawx < 0) || (drawy < 0) || (drawx > VIEW_PIXSZ_X - FIELD_X) || 
	    (drawy > VIEW_PIXSZ_Y - TerrOfssEnd[Ter1])) return;

	tile_type = tileTypeL1(Ter1 % 256);

	// Full visibility
	if (f->Visib == 2) {
		sprite1 = (const uint8_t*)BmpTerr1[Ter1];

		if (Ter2 != 0 && Ter2 < tofsL2Spec) {
			sprite2 = (const uint8_t*)BmpTerr2[Ter2];
		}
	}
	// Partial visibility (fog of war)
	else if (f->Visib == 1) {
		sprite1 = (const uint8_t*)BmpTerr1D[Ter1];

		if (Ter2 != 0 && Ter2 < tofsL2Spec && !DontDrawL2[Ter2]) {
			sprite2 = (const uint8_t*)BmpTerr2D[Ter2];
		}
	}
	// Undiscovered tile
	else {
		sprite1 = (const uint8_t*)BmpTerr1[tileTypeListL1[tile_type]],
		ldx = ldy = 0xFF;
		show_helpers = 0;
	}

	// First terrain layer and tile coordinate mapping table
	drawSolidTile((uint8_t*)MapBuf, VIEW_PIXSZ_X, drawx, drawy, sprite1,
		tile_type);
	fillTile((uint8_t*)LocalBufX, VIEW_PIXSZ_X / 2, localx, localy, ldx,
		tile_type);
	fillTile((uint8_t*)LocalBufY, VIEW_PIXSZ_X / 2, localx, localy, ldy,
		tile_type);

	// Second terrain layer
	if (sprite2) {
		tile_type = tileTypeL2(Ter2);
		drawTransparentTile((uint8_t*)MapBuf, VIEW_PIXSZ_X, drawx,
			drawy, sprite2, tile_type);
	}
	
	// Tactical overlays
	if (show_helpers && f->HasHelper) {
		DrawRangesOnField(x, y, drawx, drawy);
	}
}






void DrawL2Selector(int drawx, int drawy, word Ter1, void *BmpSl[])
{
	unsigned tile_type = tileTypeL1(Ter1 % 256);
	drawTransparentTile((uint8_t*)MapBuf, VIEW_PIXSZ_X, drawx, drawy,
		(const uint8_t*)BmpSl[tile_type], tile_type);
}


