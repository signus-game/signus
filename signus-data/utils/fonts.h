
//
// Hlavickovy soubor pro FONTS.CPP a FONTSASM.ASM
//
// Obsah: nacitani, ukladani a zobrazovani text. fontu
//


#ifndef _FONTS_H
#define _FONTS_H

#include <stdio.h>

typedef unsigned char byte;

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
