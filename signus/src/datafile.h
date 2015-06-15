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
// Hlavickovy soubor pro modul DATAFILE
// Obsah: - trida TDataFile, implementujici indexovany datovy soubor 
//          (resource) pro obecna sekvencni data. 
//

#ifndef _DATAFILE_H
#define _DATAFILE_H

#include <stdlib.h>
#include <stdio.h>


// Pomocna struktura urcujici polohu datoveho prvku v souboru

typedef struct {
		char name[9];
		unsigned offset, size;
} TDataIndex;




// Typy funkci pouzitelnych pro zapis/cteni z DATu. Std. jsou pouzity
// metody StdDataWrite() a StdDataRead() [konstruktor s 1 param.]:

typedef unsigned (*TDataWriteFce)(FILE *f, void *ptr, size_t size);
extern unsigned StdDataWrite(FILE *f, void *ptr, size_t size);
  // vraci pocet zapsanych bajtu
typedef void *(*TDataReadFce)(FILE *f);
extern void *StdDataRead(FILE *f);
  // vraci ptr na alokovanou pamet s daty




// Atributy pro otevreni DATu:

#define dfOpenRead   1
#define dfOpenWrite  2
#define dfCreate     4



// Trida pro pristup do DAT souboru:

class TDataFile {
		private:
			FILE *resf;
			int count;
			TDataIndex *index;
			TDataWriteFce writefce;
			TDataReadFce readfce;
			int changed;
			const char *readprefix;
			char readreplacer;

		public:
			TDataFile(const char *name, int flags, const char *aprefix = NULL, char areplac = '?',
			          TDataWriteFce wfce = StdDataWrite, TDataReadFce rfce = StdDataRead);
			int put(const char *name, void *ptr, size_t size);
			void *get(const char *name);
			int lookfor(const char *name, int lo, int hi);
			int getcount() {return count;}
			TDataIndex *getinfo(int pos) {return &(index[pos]);}
			void sortindex(int bywhat);
			~TDataFile();
};



#endif
