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
// Hlavickovy soubor pro FONTS.CPP a FONTSASM.ASM
//
// Obsah: nacitani, ukladani a zobrazovani text. fontu
//


#ifndef _FONTS_H
#define _FONTS_H

#include <stdio.h>
#include "system.h"


// Typy pouzivane pri praci s fonty:
// Pozn.: protoze Struct aligment je nastaveno na 4, dochazi
//        k deformaci velikosti techto struktur. Proto je u kazdeho
//        prvku uvedeno, kolik skutecne zabira (kdyz se lisi)

#pragma pack(4)
typedef struct {
	char Width; // sz=4
	byte *Data;
} TFontChar;  // SZ=8


typedef struct {
	int TotalSize; 
	void *CharBuf; 
	char Height; //sz=4
	TFontChar Chars[256];
} TFont;
#pragma pack()


// Zakladni funkce:
// (pouz: FontDataXXXX() jsou parametry inicializace TDataFile)

extern unsigned FontDataWrite(FILE *f, void *ptr, size_t size);
extern void *FontDataRead(FILE *f);

extern void freefont(TFont *font);
extern void lockfont(TFont *font);
extern void unlockfont(TFont *font);

// Vykresli do TAR text dvema barvami. Funguji i znaky CR, LF
extern "C" void PutStr(void *tar, int tarwidth, int xpoz, int ypoz, char *s, TFont *f, byte clr1, byte clr2);

// Zjistuje vysku a sirku daneho textu:
extern int GetStrWidth(char *s, TFont *f);
extern int GetStrHeight(char *s, TFont *f);

#endif
