
//
// Implementace proporcialnich textovych fontu 
//

#include <stdlib.h>

#include "fonts.h"

void *FontDataRead(FILE *f)
{
	TFont *p = (TFont *) malloc(sizeof(TFont));
	int i;
	byte *dummy;
	
	fread(&(p->TotalSize), sizeof(p->TotalSize), 1, f);
	p->CharBuf = malloc(p->TotalSize);
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
	free(font->CharBuf);
	free(font);
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
    int x = xpoz, y = ypoz;
    byte *buf = ((byte*)tar) + x + y * tarwidth;
    byte *buf2;
    byte *data;
    
    for (char *c = s; *c; c++, buf += w)
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
