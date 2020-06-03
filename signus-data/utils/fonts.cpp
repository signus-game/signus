
//
// Implementace proporcialnich textovych fontu 
//

#include <stdlib.h>

#include "fonts.h"

void *FontDataRead(ReadStream &stream) {
	TFont *ret = (TFont*)malloc(sizeof(TFont));
	int i;
	size_t size;
	byte *ptr;

	ret->TotalSize = stream.readSint32LE();
	ret->CharBuf = malloc(ret->TotalSize);
	ptr = (byte*)ret->CharBuf;
	ret->Height = stream.readSint8();

	for (i = 0; i < 256; i++) {
		ret->Chars[i].Width = stream.readSint8();

		if (ret->Chars[i].Width) {
			size = ret->Height * ret->Chars[i].Width;
			stream.read(ptr, size);
			ret->Chars[i].Data = ptr;
			ptr += size;
		} else {
			ret->Chars[i].Data = NULL;
		}
	}

	return ret;
}

unsigned FontDataWrite(WriteStream &stream, void *ptr, size_t size) {
	int i;
	unsigned tmp, ret = 0;
	TFont *p = (TFont*)ptr;

	stream.writeSint32LE(p->TotalSize);
	stream.writeSint8(p->Height);
	ret = 5;

	for (i = 0; i < 256; i++) {
		stream.writeSint8(p->Chars[i].Width);
		ret++;

		if (p->Chars[i].Width) {
			tmp = p->Height * p->Chars[i].Width;
			stream.write(p->Chars[i].Data, tmp);
			ret += tmp;
		}
	}

	return ret;
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
