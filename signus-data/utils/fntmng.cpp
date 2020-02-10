
/*


Projekt SIGNUS

Mana‘er .DAT soubor– obsahuj¡c¡ch fonty (fonts.dat)



*/


#include <png.h>
#include <stdio.h>
#include <string.h>
#include "fonts.h"
#include "datafile.h"

bool LoadPNG(FILE *fp, void*& _buffer, int& _w, int& _h)
{
    /* Taken (& modified) from wxWindows code, credits go to 
       Robert Roebling... */
    
    png_structp png_ptr;
    unsigned char **lines = NULL;
    unsigned int i;
    png_infop info_ptr = (png_infop) NULL;   
    png_uint_32 width,height;
    int bit_depth,color_type,interlace_type;  
    unsigned x, y;
    unsigned char *buffer = NULL;
    
    png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING,
        NULL,
        (png_error_ptr) NULL,
        (png_error_ptr) NULL );

    if (!png_ptr)
        goto error_nolines;

    info_ptr = png_create_info_struct( png_ptr );
    if (!info_ptr)
        goto error_nolines;

    if (setjmp(png_jmpbuf(png_ptr)))
        goto error_nolines;

   png_init_io(png_ptr, fp);

//    if (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
//        goto error_nolines;

    png_read_info( png_ptr, info_ptr );
    png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, (int*) NULL, (int*) NULL );

    if (color_type != PNG_COLOR_TYPE_PALETTE)
        goto error_nolines;

    png_set_strip_16( png_ptr );
    png_set_packing( png_ptr );
//    if (png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS))
//        png_set_expand( png_ptr );
    png_set_filler( png_ptr, 0xff, PNG_FILLER_AFTER );


    /* Create the image: */
    buffer = (unsigned char *)malloc(width * height);

    lines = (unsigned char **)malloc( (size_t)(height * sizeof(unsigned char *)) );
    if (lines == NULL)
        goto error_nolines;

    for (i = 0; i < height; i++)
    {
        lines[i] = buffer + width * i;
    }

        png_read_image( png_ptr, lines );
        png_read_end( png_ptr, info_ptr );
        png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp) NULL );

        free( lines );

    _w = width;
    _h = height;
    _buffer = (void*)buffer;
    return true;

 error_nolines:
    lines = NULL; // called from before it was set

 error:
    if ( buffer )
        delete buffer;

    if ( lines )
        free( lines );

    if ( png_ptr )
    {
        if ( info_ptr )
        {
            png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp) NULL );
            free(info_ptr);
        }
        else
            png_destroy_read_struct( &png_ptr, (png_infopp) NULL, (png_infopp) NULL );
    }

    
    return false;
}


TFont *fnt;
TDataFile *df;




void LoadChar(const char *prefix, int ch)
{
	char fil[1024];
	FILE *f;
	int i;
	unsigned short int aword;
	unsigned char abyte;
	
	snprintf(fil, 1024, "%s/%i.png", prefix, ch);
	f = fopen(fil, "rb");
	if (f == NULL) {fnt -> Chars[ch].Data = NULL; return;}
	printf("%i ", ch);

    void *buf;
    int w, h;
    LoadPNG(f, buf, w, h);
	fclose(f);

    fnt->Chars[ch].Width = w;
    fnt->Height = h;
    fnt->Chars[ch].Data = (byte*)buf;
	fnt -> TotalSize += fnt -> Height * fnt -> Chars[ch].Width;    
}




void SaveChar(const char *prefix, int ch)
{
	char fil[1024];
	FILE *f;
	int i;
	unsigned short int aword;
	unsigned char abyte;
	
	snprintf(fil, 1024, "%s/%i.cel", prefix, ch);
	f = fopen(fil, "wb");

	abyte = 0x19;	fwrite(&abyte, 1, 1, f);
	abyte = 0x91;	fwrite(&abyte, 1, 1, f);
	aword = fnt -> Chars[ch].Width; fwrite(&aword, 2, 1, f);
	aword = fnt -> Height; fwrite(&aword, 2, 1, f);
	abyte = 0; for (i = 0; i < 28; i++) fwrite(&abyte, 1, 1, f);
	abyte = 0xFF; for (i = 0; i < 4; i++) fwrite(&abyte, 1, 1, f);
	abyte = 0; for (i = 0; i < 762; i++) fwrite(&abyte, 1, 1, f);	
    fwrite(fnt -> Chars[ch].Data, fnt -> Height * fnt -> Chars[ch].Width, 1, f);
	fclose(f);
}


int main(int argc, char *argv[])
{
	int i;
	FILE *fi;
	
	if (argc != 5) {	
		printf("Font Manager\nUsage: FNTMNG.EXE <fontsfile.dat> <fontname> x|c <prefix>\n"
		       "       where x means to extract chars, c means to create font\n");
		return 1;
	}
	printf("Font Manager\nFonts file:  %s\nFont name:   %s\n\n", argv[1], argv[2]);

	if (strcmp("x", argv[3]) == 0) {
		df = new TDataFile(argv[1], dfOpenRead, NULL, '?', FontDataWrite, FontDataRead);
		fnt = (TFont *) df -> get(argv[2]);
		printf("Font height: %i\n", fnt -> Height);
		printf("Extracting... [");
		for (i = 0; i < 0xFF; i++) 
			if (fnt -> Chars[i].Width != 0) {
				printf("%i ", i);
				SaveChar(argv[4], i);
			}
		printf("]\n");
		freefont(fnt);
	}
	
	else if (strcmp("c", argv[3]) == 0) {
		fi = fopen(argv[1], "rb");
		if (fi == NULL)
			df = new TDataFile(argv[1], dfCreate, NULL, '?', FontDataWrite, FontDataRead);
		else{
			fclose(fi);
			df = new TDataFile(argv[1], dfOpenWrite, NULL, '?', FontDataWrite, FontDataRead);
		}

		fnt = new TFont;
		fnt -> TotalSize = 0;
		for (i = 0; i < 0xFF; i++) fnt -> Chars[i].Width = 0;
		printf("Creating... [");
		for (i = 0; i < 0xFF; i++) 
			LoadChar(argv[4], i);
		printf("]\n");
		printf("Font height: %i\n", fnt -> Height);		
		df -> put(argv[2], fnt, 1/*dummy size*/);
	}
	
	else { printf("Error in 3rd parameter!\n"); return 2; }
	
	delete df;
}
