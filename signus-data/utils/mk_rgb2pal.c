
/* (adapted for SDL's SDL_FindColor by Sam Lantinga ==> GPL license) */

#include <stdio.h>


typedef unsigned char pixel;


static pixel find_color(pixel *pal, pixel r, pixel g, pixel b)
{
	/* Do colorspace distance matching */
	unsigned int smallest;
	unsigned int distance;
	int rd, gd, bd;
	int i;
	pixel pix=0;
		
	smallest = ~0;
	for ( i=0; i<256; ++i ) {
		rd = (pal[3*i+0]) - r;
		gd = (pal[3*i+1]) - g;
		bd = (pal[3*i+2]) - b;
		distance = (rd*rd)+(gd*gd)+(bd*bd);
		if ( distance < smallest ) {
			pix = i;
			if ( distance == 0 ) { /* Perfect match! */
				break;
			}
			smallest = distance;
		}
	}
	return pix;
}



#define PRECISION      5
#define VALUES         (1<<PRECISION)
#define STEP           (1<<(8-PRECISION))
pixel Table[32768];

static void compute(pixel *pal)
{
    pixel r,g,b;
    
    for (r = 0; r < VALUES; r++)
        for (g = 0; g < VALUES; g++)
            for (b = 0; b < VALUES; b++)
                Table[r*VALUES*VALUES + g*VALUES + b] =
                    find_color(pal, r*STEP, g*STEP, b*STEP);
}


int main(int argc, char *argv[])
{
   if (argc < 3)
   {
       fprintf(stderr, "Usage: %s palette.col table.tab\n", argv[0]);
       return 1;
   }
   
   pixel pal[768];
   FILE *f;
   
   f = fopen(argv[1], "rb");
   fseek(f, 8, SEEK_SET);
   fread(pal, 768, 1, f);
   fclose(f);
   
   printf("computing RGB->palette conversion table for %s...\n", argv[1]);
   
   compute(pal);

   f = fopen(argv[2], "wb");
   fwrite(Table, sizeof(Table), 1, f);
   fclose(f);
   
}

