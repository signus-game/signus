
/*


Tento program slou‘¡ k vytv ©en¡ .DAT soubor–


*/


#include <assert.h>
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

#define TRUE 1
#define FALSE 0
#define NHDF_BLOCKSIZE (256*1024)

using std::cout;
using std::cerr;
using std::endl;

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

typedef MemoryWriteStream *(*convfunc_t)(const char *filename);

struct ConvMap {
	const char *ext, *name;
	convfunc_t conv;
};


unsigned char palette[768];
bool palette_loaded = false;

MemoryWriteStream *slurp_file(const char *filename, long offset, size_t size) {
	File fr(filename, File::READ);
	MemoryWriteStream *ret;

	if (!fr.isOpen()) {
		return NULL;
	}

	if (!size) {
		size = fr.size() - offset;
	}

	ret = new MemoryWriteStream(size);
	fr.seek(offset, SEEK_SET);
	ret->copy(fr, size);
	return ret;
}

MemoryWriteStream *convert_palette(const char *filename) {
	File fr(filename, File::READ);
	MemoryWriteStream *ret;
	size_t i, size = 768;

	if (!fr.isOpen()) {
		return NULL;
	}

	ret = new MemoryWriteStream(size);
	fr.seek(8, SEEK_SET);

	for (i = 0; i < size; i++) {
		ret->writeUint8(fr.readUint8() >> 2);
	}

	return ret;
}

MemoryWriteStream *convert_cel(const char *filename) {
	return slurp_file(filename, 800, 0);
}

MemoryWriteStream *convert_text(const char *filename) {
	MemoryWriteStream *ret = slurp_file(filename, 0, 0);

	if (ret) {
		ret->writeUint8('\0');
	}

	return ret;
}


inline unsigned clr_dist(int r1,int g1,int b1,
                         int r2,int g2,int b2)
{
    return (r1-r2)*(r1-r2) + (g1-g2)*(g1-g2) + (b1-b2)*(b1-b2);
}

unsigned char findNearestColor(
        unsigned char r, unsigned char g, unsigned char b)
{
    unsigned best = 1;
    unsigned bestdist = clr_dist(r,g,b,palette[3],palette[4],palette[5]);
    for (unsigned i = 2; i < 256; i++)
    {
        unsigned dist = clr_dist(r,g,b,
                                 palette[3*i+0],palette[3*i+1],palette[3*i+2]);
        if (dist < bestdist)
        {
            bestdist = dist;
            best = i;
        }
    }
    return best;
}

bool LoadPNG(FILE *fp, void*& _buffer, int& _w, int& _h)
{
	if (!palette_loaded) {
		File pf(getenv("MAKEDAT_PALETTE"), File::READ);

		assert(pf.isOpen());
		pf.seek(8, SEEK_SET);
		pf.read(palette, 768);
		palette_loaded = true;
	}

    
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
    unsigned char *buffer2 = NULL;
    
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

    assert( color_type == PNG_COLOR_TYPE_PALETTE );
    
    if (color_type == PNG_COLOR_TYPE_PALETTE) 
        png_set_palette_to_rgb(png_ptr);

    png_set_strip_16( png_ptr );
    png_set_packing( png_ptr );
    if (png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_expand( png_ptr );
    png_set_filler( png_ptr, 0xff, PNG_FILLER_AFTER );


    /* Create the image: */
    buffer = (unsigned char *)malloc(width * height * 4);
    buffer2 = (unsigned char *)malloc(width * height);

    lines = (unsigned char **)malloc( (size_t)(height * sizeof(unsigned char *)) );
    if (lines == NULL)
        goto error_nolines;

    for (i = 0; i < height; i++)
    {
        lines[i] = buffer + 4 * width * i;
    }

        png_read_image( png_ptr, lines );
        png_read_end( png_ptr, info_ptr );
        png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp) NULL );

        free( lines );

    for (i = 0; i < width * height; i++)
    {
        if (buffer[4*i+3] < 128)
            buffer2[i] = 0;
        else
            buffer2[i] = 
                findNearestColor(buffer[4*i+0],buffer[4*i+1],buffer[4*i+2]);
    }
        
    _w = width;
    _h = height;
    _buffer = (void*)buffer2;
    return true;

 error_nolines:
    lines = NULL; // called from before it was set

 error:
    if ( buffer )
        free(buffer);
    if ( buffer2 )
        free(buffer2);

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

bool LoadPNG(const char *filename, void*& _buffer, int& _w, int& _h) {
	FILE *fr;
	bool ret;

	fr = fopen(filename, "r");

	if (!fr) {
		return false;
	}

	ret = LoadPNG(fr, _buffer, _w, _h);
	fclose(fr);
	return ret;
}

MemoryWriteStream *convert_png(const char *filename) {
	void *buf;
	int width, height;
	MemoryWriteStream *ret;

	if (!LoadPNG(filename, buf, width, height)) {
		return NULL;
	}

	try {
		ret = new MemoryWriteStream(width * height);
	} catch (...) {
		free(buf);
		throw;
	}

	ret->write(buf, width * height);
	free(buf);
	return ret;
}

MemoryWriteStream *convert_sprite(const char *filename) {
	char *hsname, *ptr, text[64];
	void *buf;
	int i, width = 0, height = 0;
	size_t len;
	MemoryWriteStream *ret;
	File hsfile;

	len = strlen(filename);
	hsname = new char[len + 8];
	strcpy(hsname, filename);
	ptr = strrchr(hsname, '.');

	if (!ptr) {
		ptr = hsname + len;
	}

	strcpy(ptr, ".hotspot");

	if (!hsfile.open(hsname, File::READ)) {
		delete[] hsname;
		return NULL;
	}

	delete[] hsname;
	hsfile.readLine(text, 64);
	hsfile.close();
	sscanf(text, "%d %d", &width, &height);
	ret = new MemoryWriteStream;
	ret->writeSint32LE(width);
	ret->writeSint32LE(height);

	if (!LoadPNG(filename, buf, width, height)) {
		delete ret;
		return NULL;
	}

	if (width >= 256 || height >= 256) {
		cerr << "  SPRITE IS TOO LARGE (" << width << "x" << height << ")" << endl;
		delete ret;
		free(buf);
		return NULL;
	}

	ret->writeSint32LE(width);
	ret->writeSint32LE(height);
	ret->write(buf, width * height);
	free(buf);
	return ret;
}

MemoryWriteStream *convert_tile(const char *filename) {
	unsigned char *ptr;
	void *buf;
	int i, width, height;
	MemoryWriteStream *ret;

	if (!LoadPNG(filename, buf, width, height)) {
		return NULL;
	}

	try {
		ret = new MemoryWriteStream(width * height);
	} catch (...) {
		free(buf);
		throw;
	}

	ptr = (unsigned char*)buf;

	for (i = 0; i < width * height; i++) {
		if (ptr[i] != ptr[0]) {
			ret->writeUint8(ptr[i]);
		}
	}

	free(buf);
	return ret;
}



TDataFile *DataF;

time_t TCO = ourtime();


int temporary = 0;

int use_nhdf = FALSE;
File nhdf;

ConvMap conv_func_list[] = {
	{"col", "[palette]", convert_palette},
	{"png", "[image]", convert_png},
	{"cel", "[image]", convert_cel},
	{"spr.png", "[sprite]", convert_sprite},
	{"tile.png", "[tile]", convert_tile},
	{"txt", "[text]", convert_text},
	{NULL, NULL, NULL}
};

unsigned NohdrDataWrite(WriteStream &stream, void *ptr, size_t size) {
	return stream.copy(nhdf, size);
}

int AddFile(const char *prefix, const char *dir, const char *name) {
	void *buf = NULL;
	size_t bufsize;
	char jm[PATH_MAX];
	char resnm[1024];
	int i, ret = 1;
	MemoryWriteStream *stream = NULL;

	strcpy(jm, dir);
	strcat(jm, name);
  	
	char noext[256];
	char *basename = strrchr(jm, '/');

	if (basename) {
		basename++;
	} else {
		basename = jm;
	}

	strcpy(noext, basename);
	char *exten = strstr(noext, ".");

	if (exten) {
		*exten++ = '\0';
	} else {
		exten = noext + strlen(noext);
	}

	strcpy(resnm, prefix);
	strcat(resnm, noext);
	//strlwr(resnm);
	cout << "adding " << resnm << " ";

	if (!strcmp(exten, "fnt") && !temporary) {
		TFont *font;
		File fr(jm, File::READ);

		cout << "[font table]";

		if (!fr.isOpen()) {
			cerr << "Cannot open file " << jm << endl;
			return 0;
		}

		font = (TFont*)FontDataRead(fr);

		if (!(DataF->put(resnm, font, 1))) {
			cout << "  SIZE CHANGED!";
			ret = 0;
		}

		freefont(font);
		cout << endl;
		return ret;
	}

	for (i = 0; conv_func_list[i].ext; i++) {
		if (!strcmp(conv_func_list[i].ext, exten)) {
			break;
		}
	}

	if (conv_func_list[i].ext) {
		cout << conv_func_list[i].name;
		stream = conv_func_list[i].conv(jm);
	} else {
		cout << "[unknown data]";

		if (use_nhdf) {
			nhdf.open(jm, File::READ);
			bufsize = nhdf.size();
		} else {
			stream = slurp_file(jm, 0, 0);
		}
	}

	if (!stream && (!use_nhdf || !nhdf.isOpen())) {
		cerr << "Cannot read file " << jm << endl;
		return 0;
	}

	if (!use_nhdf) {
		buf = stream->dataPtr();
		bufsize = stream->size();
	}

	if (temporary) {
		File fw;

		sprintf(jm, "/tmp/signus-data-temp_%s", resnm);

		if (!fw.open(jm, File::WRITE | File::TRUNCATE)) {
			cerr << "Cannot write into file " << jm << endl;
			delete stream;
			return 0;
		}

		fw.writeUint32LE(bufsize);

		if (fw.write(buf, bufsize) != bufsize) {
			cerr << "Cannot write into file " << jm << endl;
			ret = 0;
		}
	} else if (!DataF->put(resnm, buf, bufsize)) {
		cout << "  SIZE CHANGED!";
		ret = 0;
	}

	delete stream;
	cout << endl;
	return ret;
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

		for (int i = start; i < argc; i++) {
			if (strcmp(argv[i]+strlen(argv[i])-4, "/CVS") == 0)
				continue;
			if (!AddFile("", "", argv[i])) {
				delete DataF;
				return 1;
			}
		}
#if 0
		for (int i = start; i < argc; i++)
			AddItem("", "", argv[i]);
#endif
		delete DataF;
		cout << endl << "Done..." << endl;
    }
}

