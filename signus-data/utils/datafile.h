
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
			TDataFile(char *name, int flags, char *aprefix = NULL, char areplac = '?',
			          TDataWriteFce wfce = StdDataWrite, TDataReadFce rfce = StdDataRead);
			int put(char *name, void *ptr, size_t size);
			void *get(char *name);
			int lookfor(char *name, int lo, int hi);
			int getcount() {return count;}
			TDataIndex *getinfo(int pos) {return &(index[pos]);}
			void sortindex(int bywhat);
			~TDataFile();
};



#endif
