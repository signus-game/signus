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
// Implementace proporcialnich textovych fontu 
//

#include "headers.h"

#include "fonts.h"

void *FontDataRead(FILE *f)
{
	TFont *p = (TFont *) memalloc(sizeof(TFont));
	int i;
	byte *dummy;
	
	fread(&(p->TotalSize), sizeof(p->TotalSize), 1, f);
	p->CharBuf = memalloc(p->TotalSize);
	dummy = (byte *) p->CharBuf;
	fread(&(p->Height), sizeof(p->Height), 1, f);
	for (i = 0; i < 256; i++) {
		fread(&(p->Chars[i].Width), sizeof(p->Chars[i].Width), 1, f);
		if (p->Chars[i].Width == 0) p->Chars[i].Data = NULL;
		else {
			p->Chars[i].Data = (byte *) dummy;
			dummy += p->Height * p->Chars[i].Width;
			fread(p->Chars[i].Data, p->Height * p->Chars[i].Width, 1, f);
		}
	}
	return (void *)p;
}



unsigned FontDataWrite(FILE *f, void *ptr, size_t size)
{
	int i;
	unsigned cnt = 0;
	TFont *p = (TFont *) ptr;
	
	fwrite(&(p->TotalSize), sizeof(p->TotalSize), 1, f); cnt += sizeof(p->TotalSize);
	fwrite(&(p->Height), sizeof(p->Height), 1, f); cnt += sizeof(p->Height);
	for (i = 0; i < 256; i++) {
		fwrite(&(p->Chars[i].Width), sizeof(p->Chars[i].Width), 1, f); cnt += sizeof(p->Chars[i].Width);
		if (p->Chars[i].Width != 0) {
			fwrite(p->Chars[i].Data, p->Height * p->Chars[i].Width, 1, f);
			cnt += p->Height * p->Chars[i].Width;
		}
	}
	return cnt;
}



void freefont(TFont *font)
{	
	memfree(font->CharBuf);
	memfree(font);
}



void lockfont(TFont *font)
{
	lockmem(font->CharBuf, font->TotalSize);
	lockmem(font, sizeof(TFont));
}



void unlockfont(TFont *font)
{
	unlockmem(font->CharBuf, font->TotalSize);
	unlockmem(font, sizeof(TFont));
}








int GetStrWidth(char *s, TFont *f)
{
	int w = 0;
	
	while (*s != 0) w += f->Chars[*(s++)].Width;
	return w;
}



int GetStrHeight(char *s, TFont *f)
{
	int h = f->Height;

	while (*s != 0)
		if (*(s++) == '\n') h += f->Height;
	return h;
}


extern "C" void PutStr(void *tar, int tarwidth, 
                       int xpoz, int ypoz, 
                       char *s, TFont *f, 
                       byte clr1, byte clr2)
{
    register int i,j;
    int w, h = f->Height;
    byte *buf = ((byte*)tar) + xpoz + ypoz * tarwidth;
    byte *buf2;
    byte *data;
    
    for (char *c = s; *c; c++, buf += w)
    {
        if (*c == '\n')
        {
            w = 0;
            ypoz += h;
            buf = ((byte*)tar) + xpoz + ypoz * tarwidth;
        }
        else
        {
            w = f->Chars[*((unsigned char*)c)].Width;
            data = f->Chars[*((unsigned char*)c)].Data;
            buf2 = buf;
            for (j = h; j > 0; j--, buf2 += tarwidth - w)
            {
                for (i = w; i > 0; i--, buf2++, data++)
                {
                    if (*data == 1)
                        *buf2 = clr1;
                    else if (*data == 2)
                        *buf2 = clr2;
                }
            }
        }
    }
}
