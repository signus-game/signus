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
// Modul datovych souboru
//

#include <cstring>
#include <stdexcept>

#include "datafile.h"
#include "system.h"


/////////////////////////////// STD funkce i/o /////////////////////////////


void multipath_fopen(File &f, const char *name, unsigned mode);
// fce obsazena v global.cpp, neni tu inc global.h, protoze to se moc
// casto meni, tenhle modul chce mit taky chvilku klidu...



unsigned StdDataWrite(WriteStream &stream, void *ptr, size_t size) {
	stream.writeUint32LE(size);
	stream.write(ptr, size);
	return size + 4;
}

void *StdDataRead(ReadStream &stream) {
	char *ptr;
	size_t size;

	size = stream.readUint32LE();
	ptr = (char*)memalloc(size + 1);

	if (ptr) {
		stream.read(ptr, size);
		ptr[size] = '\0';
	}

	return ptr;
}






///////////////////////////// trida TDataFile //////////////////////////////




TDataFile::TDataFile(const char *name, int flags, const char *aprefix,
	char areplac, TDataWriteFce wfce, TDataReadFce rfce) {
	writefce = wfce;
	readfce = rfce;
	changed = 0;
	readprefix = (aprefix == NULL) ? "" : aprefix;
	readreplacer = areplac;
	int mode = 0;

	if (flags == dfOpenRead) {
		mode = File::READ;
	} else if (flags & dfOpenWrite) {
		mode = File::READ | File::WRITE;
	} else if (flags & dfCreate) {
		mode = File::WRITE | File::TRUNCATE;
	} else {
		// FIXME: throw exception
		return;
	}

	if (flags == dfCreate) {     // create new empty file
		count = 0;
		index = NULL;
		resf.open(name, mode);
	} else {
		// open existing file for reading or writing
		unsigned tableofs;

		multipath_fopen(resf, name, mode);
		tableofs = resf.readUint32LE();
		resf.seek(tableofs, SEEK_SET);
		count = resf.readSint32LE();
		index = (TDataIndex*)memalloc(count * sizeof(TDataIndex));

		for (int i = 0; i < count; i++) {
			resf.read(index[i].name, 12);
			index[i].name[11] = '\0';
			index[i].offset = resf.readUint32LE();
			index[i].size = resf.readUint32LE();
		}

		// Write access needs index sorted by offset
		if (flags == dfOpenWrite) {
			sortindex(0);
		}
	}
}



			
int TDataFile::put(const char *name, void *ptr, size_t size) {
	int indx = -1;

	// test of existence:
	for (int i = 0; i < count; i++) {
		if (strcmp(name, index[i].name) == 0) {
			indx = i;
			break;
		}
	}
		
	// replacing an existing item (must have the same size):
	if (indx != -1) {
		size_t oldsize = index[indx].size;
		changed = 1;
		resf.seek(index[indx].offset, SEEK_SET);
		index[indx].size = writefce(resf, ptr, size);

		// FIXME: Congratulations, you've just corrupted the DAT file
		if (oldsize != index[indx].size) {
			return 0;
		}
	} else {
		// adding a new item:
		changed = 1;
		count++;
		index = (TDataIndex*)realloc(index, sizeof(TDataIndex) * count);
		strcpy(index[count-1].name, name);

		if (count == 1) {
			index[0].offset = 4;
		} else {
			index[count-1].offset = index[count-2].offset + index[count-2].size;
		}

		resf.seek(index[count-1].offset, SEEK_SET);
		index[count-1].size = writefce(resf, ptr, size);
	}

	return 1;
}


// FIXME: bisecting doesn't work for R+W access, index will be sorted by offset
int TDataFile::lookfor(const char *name, int lo, int hi)
{
	int pos = (hi + lo) / 2;
	int res = strcmp(name, index[pos].name);
	if (res == 0) return pos;
	else {
		if (pos == lo) 
			if (strcmp(name, index[hi].name) != 0) return -1;
		if (pos == hi) 
		  if (strcmp(name, index[lo].name) != 0) return -1;			
		if (res < 0) return lookfor(name, lo, pos-1);
		else return lookfor(name, pos+1, hi);
	}
}



void *TDataFile::get(const char *name) {
	void *ptr = NULL;
	char nm[32];
	
	strncpy(nm, readprefix, sizeof(nm) - 1);
	nm[sizeof(nm)-1] = '\0';
	strncat(nm, name, sizeof(nm) - strlen(nm) - 1);
	nm[sizeof(nm)-1] = '\0';
	if (nm[0] == '?') nm[0] = readreplacer;
	
	int i = lookfor(nm, 0, count-1);
	if (i != -1) {
		resf.seek(index[i].offset, SEEK_SET);
		return readfce(resf);
	}
	return ptr;
}



int compare_by_name(const void *o1, const void *o2)
{
	TDataIndex *i1 = (TDataIndex*)o1;
	TDataIndex *i2 = (TDataIndex*)o2;
	return strcmp(i1->name, i2->name);
}

int compare_by_offset(const void *o1, const void *o2)
{
	TDataIndex *i1 = (TDataIndex*)o1;
	TDataIndex *i2 = (TDataIndex*)o2;
	return (i1->offset) - (i2->offset);
}



void TDataFile::sortindex(int bywhat)
{
	if (bywhat == 1)  // compare by name
		qsort(index, count, sizeof(TDataIndex), compare_by_name);
	else 
		qsort(index, count, sizeof(TDataIndex), compare_by_offset);	
}

const char *TDataFile::filename(void) const {
	return resf.getName();
}

TDataFile::~TDataFile() {
	// DAT file contents have changed, write new item index
	if (changed) {
		unsigned tableofs = 4 + index[count-1].offset + index[count-1].size;

		resf.seek(0, SEEK_SET);
		resf.writeUint32LE(tableofs);
		resf.seek(tableofs, SEEK_SET);
		resf.writeSint32LE(count);
		sortindex(1);

		for (int i = 0; i < count; i++) {
			resf.write(index[i].name, 12);
			resf.writeUint32LE(index[i].offset);
			resf.writeUint32LE(index[i].size);
		}
	}

	resf.close();

	if (count != 0) {
		memfree(index);
	}
}

