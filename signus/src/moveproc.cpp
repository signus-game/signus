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



/*

SUBSOUCAST MODULU "UNITS"


zde varianty PrepareFieldTime (fce kolem pohybu)
pro nejruznejsi nejzvrhlejsi jednotky


*/




void TToweredTrainUnit::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	if ((ft = Terr2Tbl[f->Terrain2]) == 0) ft = TerrTbl[f->Terrain];
	FieldTimeTbl[8][tbofs] = ft;
	if (((f->Unit != NO_UNIT) && (f->Unit != ID)) ||
	    ((GetMineAt(x, y) == (ID & BADLIFE)) && (f->Unit != ID)) ||
	    ((GetMineAt(x, y) != -1) && (MineIsSeen(x, y, ID & BADLIFE)) && (f->Unit != ID))) {
		for (i = 0; i < 9; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1B) ||
	         (L1TerrainType[ter] == tofsL1D)) {
		FieldTimeTbl[0][tbofs] = ft;
		FieldTimeTbl[4][tbofs] = ft;
		for (i = 1; i < 4; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		for (i = 5; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;	    
	}
	else if ((L1TerrainType[ter] == tofsL1C) ||
	         (L1TerrainType[ter] == tofsL1E)) {
		FieldTimeTbl[2][tbofs] = ft;
		FieldTimeTbl[6][tbofs] = ft;
		for (i = 3; i < 6; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else { //terA
		for (i = 0; i < 8; i++) FieldTimeTbl[i][tbofs] = 0xFF;
		switch (f->Terrain2) {
		case 71+1 :
				FieldTimeTbl[0][tbofs] = FieldTimeTbl[4][tbofs] = ft; break;
		case 71+2 :
				FieldTimeTbl[2][tbofs] = FieldTimeTbl[6][tbofs] = ft; break;
		case 71+16 :
				FieldTimeTbl[3][tbofs] = FieldTimeTbl[7][tbofs] = ft; break;
		case 71+19 :
				FieldTimeTbl[1][tbofs] = FieldTimeTbl[5][tbofs] = ft; break;
		default:
				for (i = 0; i < 8; i++)	FieldTimeTbl[i][tbofs] = ft; break;
		}
	}
}


void TOlymp::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	if ((ft = Terr2Tbl[f->Terrain2]) == 0) ft = TerrTbl[f->Terrain];
	FieldTimeTbl[8][tbofs] = ft;
	if (((f->Unit != NO_UNIT) && (f->Unit != ID)) ||
	    ((GetMineAt(x, y) == (ID & BADLIFE)) && (f->Unit != ID)) ||
	    ((GetMineAt(x, y) != -1) && (MineIsSeen(x, y, ID & BADLIFE)) && (f->Unit != ID))) {
		for (i = 0; i < 9; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1B) ||
	         (L1TerrainType[ter] == tofsL1D)) {
		FieldTimeTbl[0][tbofs] = ft;
		FieldTimeTbl[4][tbofs] = ft;
		for (i = 1; i < 4; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		for (i = 5; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;	    
	}
	else if ((L1TerrainType[ter] == tofsL1C) ||
	         (L1TerrainType[ter] == tofsL1E)) {
		FieldTimeTbl[2][tbofs] = ft;
		FieldTimeTbl[6][tbofs] = ft;
		for (i = 3; i < 6; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else { //terA
		for (i = 0; i < 8; i++) FieldTimeTbl[i][tbofs] = 0xFF;
		switch (f->Terrain2) {
		case 71+1 :
				FieldTimeTbl[0][tbofs] = FieldTimeTbl[4][tbofs] = ft; break;
		case 71+2 :
				FieldTimeTbl[2][tbofs] = FieldTimeTbl[6][tbofs] = ft; break;
		case 71+16 :
				FieldTimeTbl[3][tbofs] = FieldTimeTbl[7][tbofs] = ft; break;
		case 71+19 :
				FieldTimeTbl[1][tbofs] = FieldTimeTbl[5][tbofs] = ft; break;
		default:
				for (i = 0; i < 8; i++)	FieldTimeTbl[i][tbofs] = ft; break;
		}
	}
}


void TTrainSupportUnit::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	if ((ft = Terr2Tbl[f->Terrain2]) == 0) ft = TerrTbl[f->Terrain];
	FieldTimeTbl[8][tbofs] = ft;
	if (((f->Unit != NO_UNIT) && (f->Unit != ID)) ||
	    ((GetMineAt(x, y) == (ID & BADLIFE)) && (f->Unit != ID)) ||
	    ((GetMineAt(x, y) != -1) && (MineIsSeen(x, y, ID & BADLIFE)) && (f->Unit != ID))) {
		for (i = 0; i < 9; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1B) ||
	         (L1TerrainType[ter] == tofsL1D)) {
		FieldTimeTbl[0][tbofs] = ft;
		FieldTimeTbl[4][tbofs] = ft;
		for (i = 1; i < 4; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		for (i = 5; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;	    
	}
	else if ((L1TerrainType[ter] == tofsL1C) ||
	         (L1TerrainType[ter] == tofsL1E)) {
		FieldTimeTbl[2][tbofs] = ft;
		FieldTimeTbl[6][tbofs] = ft;
		for (i = 3; i < 6; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else { //terA
		for (i = 0; i < 8; i++) FieldTimeTbl[i][tbofs] = 0xFF;
		switch (f->Terrain2) {
		case 71+1 :
				FieldTimeTbl[0][tbofs] = FieldTimeTbl[4][tbofs] = ft; break;
		case 71+2 :
				FieldTimeTbl[2][tbofs] = FieldTimeTbl[6][tbofs] = ft; break;
		case 71+16 :
				FieldTimeTbl[3][tbofs] = FieldTimeTbl[7][tbofs] = ft; break;
		case 71+19 :
				FieldTimeTbl[1][tbofs] = FieldTimeTbl[5][tbofs] = ft; break;
		default:
				for (i = 0; i < 8; i++)	FieldTimeTbl[i][tbofs] = ft; break;
		}
	}
}





void TXenon::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	if ((ft = Terr2Tbl[f->Terrain2]) == 0) ft = TerrTbl[f->Terrain];
	FieldTimeTbl[8][tbofs] = ft;
	if (((f->Unit != NO_UNIT) && (f->Unit != ID))) {
		for (i = 0; i < 9; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1B) ||
	         (L1TerrainType[ter] == tofsL1D)) {
		FieldTimeTbl[0][tbofs] = ft;
		FieldTimeTbl[4][tbofs] = ft;
		for (i = 1; i < 4; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		for (i = 5; i < 8; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;	    
	}
	else if ((L1TerrainType[ter] == tofsL1C) ||
	         (L1TerrainType[ter] == tofsL1E)) {
		FieldTimeTbl[2][tbofs] = ft;
		FieldTimeTbl[6][tbofs] = ft;
		for (i = 3; i < 6; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1H) ||
	         (L1TerrainType[ter] == tofsL1F) ||
	         (L1TerrainType[ter] == tofsL1L) ||
	         (L1TerrainType[ter] == tofsL1J)) {
		FieldTimeTbl[1][tbofs] = ft;
		FieldTimeTbl[5][tbofs] = ft;
		for (i = 2; i < 5; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[6][tbofs] = 0xFF;
		FieldTimeTbl[7][tbofs] = 0xFF;
	}
	else if ((L1TerrainType[ter] == tofsL1I) ||
	         (L1TerrainType[ter] == tofsL1G) ||
	         (L1TerrainType[ter] == tofsL1K) ||
	         (L1TerrainType[ter] == tofsL1M)) {
		FieldTimeTbl[3][tbofs] = ft;
		FieldTimeTbl[7][tbofs] = ft;
		for (i = 4; i < 7; i++)
			FieldTimeTbl[i][tbofs] = 0xFF;
		FieldTimeTbl[0][tbofs] = 0xFF;
		FieldTimeTbl[1][tbofs] = 0xFF;
		FieldTimeTbl[2][tbofs] = 0xFF;
	}
	else {
		for (i = 0; i < 8; i++)
			FieldTimeTbl[i][tbofs] = ft;
	}
}



void TAircraft::PrepareFieldTime(int x, int y)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	ter = f->Terrain;
	if ((ft = Terr2Tbl[f->Terrain2]) == 0) ft = TerrTbl[f->Terrain];
	if (((x != X) || (y != Y)) && (GetAircraftAt(x, y) != NULL)) ft = 0xFF;
	for (i = 0; i < 9; i++)	FieldTimeTbl[i][tbofs] = ft;
}










//// lode:

static word ShipFieldTime(int x, int y, int ID, int Ctr = FALSE)
{
	word ft, ter;
	TField *f = GetField(x, y);
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;

	if ((x < 0) || (y < 0) || (x >= MapSizeX) || (y >= MapSizeY)) return 0xFF;
	ter = f->Terrain;
	if ((ft = Terr2Tbl[f->Terrain2]) == 0) ft = TerrTbl[f->Terrain];
	if (((f->Unit != NO_UNIT) && (f->Unit != ID))) return 0xFF;
	if ((Ctr) && (ter != 18 /*hluboka voda*/)) return 0xFF;
	else return ft;
}


static int ship_Xdir[8] = {  0, -1, -1, -1,  0,  1,  1,  1};
static int ship_Ydir[8] = { -1, -1,  0,  1,  1,  1,  0, -1};

static word Ship3_FT(int x, int y, int ID, int Orient)
{
	int ft1;
	
	ft1 = ShipFieldTime(x, y, ID, TRUE);
	if (ft1 == 0xFF) return 0xFF;
	if (ShipFieldTime(x + ship_Xdir[Orient], y + ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (ShipFieldTime(x - ship_Xdir[Orient], y - ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (Orient % 2 == 1) {
		if (ShipFieldTime(x-1, y, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x+1, y, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x, y-1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x, y+1, ID) == 0xFF) return 0xFF;
	}
	return ft1;
}



void TPoseidon::PrepareFieldTime(int x, int y)
{
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;
	
	for (i = 0; i < 9; i++)	{
		FieldTimeTbl[i][tbofs] = Ship3_FT(x, y, this->ID, i);
//	 	if (FieldTimeTbl[i][tbofs] != 0xFF)
//			FieldTimeTbl[i][tbofs] = Ship3_FT(x - ship_Xdir[i], y - ship_Ydir[i], ID, i);
	}
}




static word Ship5_FT(int x, int y, int ID, int Orient)
{
	int ft1, xX, yY;
	
	ft1 = ShipFieldTime(x, y, ID, TRUE);
	if (ft1 == 0xFF) return 0xFF;
	if (ShipFieldTime(x + ship_Xdir[Orient], y + ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (ShipFieldTime(x - ship_Xdir[Orient], y - ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (ShipFieldTime(x + 2*ship_Xdir[Orient], y + 2*ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (ShipFieldTime(x - 2*ship_Xdir[Orient], y - 2*ship_Ydir[Orient], ID) == 0xFF) return 0xFF;
	if (Orient % 2 == 1) {
		if (ShipFieldTime(x-1, y, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x+1, y, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x, y-1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(x, y+1, ID) == 0xFF) return 0xFF;
	
		xX = x + ship_Xdir[Orient],	yY = y + ship_Ydir[Orient];
		if (ShipFieldTime(xX-2, yY-1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX+2, yY+1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX-1, yY-2, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX+1, yY+2, ID) == 0xFF) return 0xFF;
		xX = x - ship_Xdir[Orient],	yY = y - ship_Ydir[Orient];
		if (ShipFieldTime(xX-2, yY-1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX+2, yY+1, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX-1, yY-2, ID) == 0xFF) return 0xFF;
		if (ShipFieldTime(xX+1, yY+2, ID) == 0xFF) return 0xFF;
	}
	return ft1;
}




void TKraken::PrepareFieldTime(int x, int y)
{
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;
	
	for (i = 0; i < 9; i++)	{
		FieldTimeTbl[i][tbofs] = Ship5_FT(x, y, this->ID, i);
//	 	if (FieldTimeTbl[i][tbofs] != 0xFF)
//			FieldTimeTbl[i][tbofs] = Ship5_FT(x - ship_Xdir[i], y - ship_Ydir[i], ID, i);
	}
}




void TLaguna::PrepareFieldTime(int x, int y)
{
	int i;
	int tbofs = (x - MTofsX) + (y - MTofsY) * MTsizeX;
	
	for (i = 0; i < 9; i++)	{
		FieldTimeTbl[i][tbofs] = Ship5_FT(x, y, this->ID, i);
//	 	if (FieldTimeTbl[i][tbofs] != 0xFF)
//			FieldTimeTbl[i][tbofs] = Ship5_FT(x - ship_Xdir[i], y - ship_Ydir[i], ID, i);
	}
}

