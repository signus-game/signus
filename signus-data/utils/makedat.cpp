
/*


Tento program slou‘¡ k vytv ©en¡ .DAT soubor–


*/


#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <iostream>
#include "datafile.h"
#include "fonts.h"
#include <string.h>
#include <dirent.h>
#include <png.h>

using namespace std;
#define TRUE 1
#define FALSE 0

time_t ourtime()
{
	struct tm t;
	
	t.tm_year = 1991 - 1900;
	t.tm_mon = 2;
	t.tm_mday = 2;
	t.tm_hour = 2;
	t.tm_min = 2;
	t.tm_sec = 2;
	return mktime(&t);
}






typedef struct {
			byte Terrain;        // prvn¡ level
			byte Terrain2;       // druh  £rove¤ ter‚nu
			byte Unit;           // where 0xFF = none
			byte Height    : 3;  // v˜¨ka ter‚nu (0..7)
			byte IsAnim    : 1;  // je to animovan‚?
			byte Visib     : 2;  // viditelnost 00-nic, 01-¨edˆ, 11-plnˆ
} TField;




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
        (voidp) NULL,
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







TDataFile *DataF;

time_t TCO = ourtime();


int temporary = 0;

int use_nhdf = FALSE;
FILE *nhdf = NULL;


unsigned NohdrDataWrite(FILE *f, void *ptr, size_t size)
{
	void *buf = malloc(256*1024);
	size_t sz = size;
	
	
	while (sz > 0) {
		if (sz > 256*1024) {
            fread(buf, 256*1024, 1, nhdf);
			fwrite(buf, 256*1024, 1, f);
			sz -= 256*1024;
		}
		else {
			fread(buf, sz, 1, nhdf);
			fwrite(buf, sz, 1, f);
			sz = 0;
		}
	}
	free(buf);
	return (size);
}



unsigned GetFileSize(FILE *f)
{
   long pos = ftell(f); 
   fseek(f, 0, SEEK_END);
   long pos2 = ftell(f);
   fseek(f, pos, SEEK_SET);
   return (unsigned)pos2;
}


void AddFile(char *prefix, char *dir, char *name)
{
	void *buf;
	unsigned bufsize;
	char jm[1024];
	char resnm[1024];
	FILE *fi;
	int i;
	unsigned short words[4];
	
	strcpy(jm, dir);
	strcat(jm, name);
  	
	fi = fopen(jm, "rb");

    unsigned filesize = GetFileSize(fi);

	char noext[256];
    char *basename = strrchr(jm, '/');
    if (basename) basename++;
    else basename = jm;    
	strcpy(noext, basename);
	char *exten = strstr(noext, ".") + 1;
	*(exten - 1) = 0;

	strcpy(resnm, prefix);
	strcat(resnm, noext);
	//strlwr(resnm);
	cout << "adding " << resnm << " ";
	if (strcmp(exten, "col") == 0) {  	
		cout << "[palette]";
		fseek(fi, 8, SEEK_SET);
		bufsize = 768;
		buf = malloc(bufsize);
		fread(buf, bufsize, 1, fi);
		for (int i = 0; i < 768; i++)
			((unsigned char*)buf)[i] >>= 2;
	}
	else if (strcmp(exten, "png") == 0) {
		cout << "[image]";
        int w, h;
        LoadPNG(fi, buf, w, h);
		bufsize = w*h;
	}
	else if (strcmp(exten, "cel") == 0) {
		cout << "[image]";
		fseek(fi, 800, SEEK_SET);
		bufsize = filesize - 800;
		buf = malloc(bufsize);
		fread(buf, bufsize, 1, fi);
	}
	else if (strcmp(exten, "spr.png") == 0) {
		cout << "[sprite]";
        int w, h;
        if (!LoadPNG(fi, buf, w, h))
           cerr << "   ERROR LOADING PNG!!! " << jm <<  endl;
		bufsize = w*h;
        
        byte *buf2 = (byte*)malloc(bufsize+16);
        memcpy(buf2+16, buf, bufsize);
        free(buf);
        buf = buf2;
        bufsize += 16;
        
        char hotspot[1024];
        int dx, dy;
        strcpy(hotspot, jm);
        strcpy(hotspot + strlen(jm)-4, ".hotspot");
        FILE *hsf = fopen(hotspot, "rt");
        fscanf(hsf, "%i %i", &dx, &dy);
        fclose(hsf); 
        
        memcpy(buf2, &dx, 4);
        memcpy(buf2+4, &dy, 4);
        memcpy(buf2+8, &w, 4);
        memcpy(buf2+12, &h, 4);

		if ((w >= 256) || (h >= 256)) 
			cerr << "  SPRITE IS TOO LARGE (" << words[0] << "x" << words[1] << ")\7" << endl;
	}
	else if (strcmp(exten, "tile.png") == 0) {
		cout << "[tile]";
        void *_buf2;
        int w, h, z;
        if (!LoadPNG(fi, _buf2, w, h))
           cerr << "   ERROR LOADING PNG!!! " << jm << endl;
        byte *buf2 = (byte*)_buf2;
        byte key = buf2[0];
        bufsize = 0;
        for (z = 0; z < w*h; z++)
            if (buf2[z] != key) bufsize++;
        buf = malloc(bufsize);        
        byte *buf3 = (byte*)buf;
        for (z = 0; z < w*h; z++)
            if (buf2[z] != key) *(buf3++) = buf2[z];      
        
        free(buf2);
	}
	else if (strcmp(exten, "fnt") == 0) {
		cout << "[font table]";
		bufsize = filesize;
		buf = (void *) FontDataRead(fi);
	}
	else if (strcmp(exten, "txt") == 0) {
		cout << "[text]";
		bufsize = filesize+1;
		buf = malloc(bufsize);
		fread(buf, bufsize-1, 1, fi);
		((byte*)buf)[bufsize-1] = 0;
	}
	else {
		cout << "[unknown data]";
		bufsize = filesize;
		if (use_nhdf) {
			nhdf = fi;
		}
		else {
			buf = malloc(bufsize);
			fread(buf, bufsize, 1, fi);
		}
	}
	if (temporary) {
		char bbb[200];
		FILE *fff;
		
		sprintf(bbb, "/tmp/signus-data-temp_%s", resnm);
		fff = fopen(bbb, "wb");
		fwrite(&bufsize, 4, 1, fff);
		fwrite(buf, bufsize, 1, fff);
		fclose(fff);
	}
	else {
		if (!(DataF -> put(resnm, buf, bufsize))) cout << "  SIZE CHANGED!";
	}
	if (!use_nhdf) free(buf);
	cout << endl;
	fclose(fi);
}


#if 0
void AddItem(char *prefix, char *dirname, char *name)
{
    DIR *dir;
	struct dirent *di;
	
    dir = opendir(dirname);
    if (!dir) return;
    
    for (di = readdir(dir); di; di = readdir(dir))
    {
            string name(di->d_name);
            string fullname(prefix);
            fullname += name;

			if (name == "." || name == "..")
				continue;
            
			AddFile(prefix, dirname, di->d_name);
	}
    
    closedir(dir);
}
#endif


int main(int argc, char *argv[])
{
	if (argc < 3) {
		cout << "Parameters: makedat <datafile.dat> [/nohdr | /fnt | /mark] <directories...>\n";
		return 1;
	}
	{	
		int start = 2;
		if (strcmp(argv[2], "/fnt") == 0) {
			cout << "Using font module..." << endl;
			DataF = new TDataFile(argv[1], dfCreate, NULL, '?', FontDataWrite, FontDataRead);
			++start;
		}
		else if (strcmp(argv[2], "/mark") == 0) {
			cout << "Using updating module..." << endl;
			DataF = new TDataFile(argv[1], dfOpenWrite);
			++start;
		}
		else if (strcmp(argv[2], "/temp") == 0) {
			cout << "Using temporary dir module..." << endl;
			DataF = new TDataFile(argv[1], dfOpenWrite);
			++start;
			temporary = 1;
		}
		else if (strcmp(argv[2], "/nohdr") == 0) {
			cout << "Using special module for no-header data..." << endl;
			DataF = new TDataFile(argv[1], dfCreate, NULL, '?', NohdrDataWrite);
			use_nhdf = TRUE;
			++start;
		}
		else
			DataF = new TDataFile(argv[1], dfCreate);

		for (int i = start; i < argc; i++)
        {
            if (strcmp(argv[i]+strlen(argv[i])-4, "/CVS") == 0)
                continue;
			AddFile("", "", argv[i]);
        }
#if 0
		for (int i = start; i < argc; i++)
			AddItem("", "", argv[i]);
#endif
		delete DataF;
		cout << endl << "Done..." << endl;
    }
}
