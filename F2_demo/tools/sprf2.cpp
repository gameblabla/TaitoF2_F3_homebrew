//*****************************************************************
//* PCX to Taito F2 sprite converter.
//*****************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define PAL_SIZE        16
#define BANK_SIZE		(512 * 1024)
#define TILEDIMENSION	16

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef BYTE           BOOL;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

int  Rip(char *filename);// function declarations                
void WriteSprites(void);
int  LoadPCX(const char* file, BYTE *outpix, BYTE * pal);
long FileSize(const char *fname);
void GetSprite(BYTE *sprite, int x, int y, int width, int height);

FILE   *fp;
FILE   *stream;              // level output data file
WORD    size = 0;
int     Width, Height, pal, start;
WORD    palval;
FILE    *handle;
char    filebank1[32];
char    filebank2[32];
char    filepal[32];
BYTE    bank[BANK_SIZE * 2];
WORD    palette[PAL_SIZE];
long    Offset;

typedef struct PCXHeader
{
	BYTE id;
	BYTE version;
	BYTE rle;
	BYTE bpp;
	WORD xstart;                         
	WORD ystart;
	WORD xend;
	WORD yend;
	WORD hres;
	WORD vres;
	BYTE pal[48];
	BYTE rsvd1;
	BYTE nbitp;
	WORD bytesperline;
	WORD paltype;
	WORD hsize;
	WORD vsize;
	BYTE rsvd2[54];
} PCX;

BYTE PCXBuffer[64000];
BYTE PCXPalette[768];

void Usage();

void main(int argc, char *argv[])
{
	printf("PCX to Taito F2 sprite converter.\n");

	if (argc < 5)
	{
		Usage();
	}

	memset(bank, 0, sizeof(bank));
	memset(palette, 0, PAL_SIZE * 2);
	
	Offset = atol(argv[1]);

	strcpy(filebank1, argv[3]);
	strcpy(filebank2, argv[4]);

	if ("" == filebank1 || "" == filebank2)
	{
		Usage();
	}

	long length;

	handle = fopen(filebank1, "rb");

	if (handle != NULL)
	{
		fseek(handle, 0, SEEK_END);
		length = ftell(handle);
		rewind(handle);

		if (length > BANK_SIZE)
		{
			length = BANK_SIZE;
		}

		fread(bank, 1, length, handle);
		fclose(handle);
	}

	handle = fopen(filebank2, "rb");

	if (handle != NULL)
	{
		fseek(handle, 0, SEEK_END);
		length = ftell(handle);
		rewind(handle);

		if (length > BANK_SIZE)
		{
			length = BANK_SIZE;
		}

		fread(bank + BANK_SIZE, 1, length, handle);
		fclose(handle);
	}

	if (argc > 5)
	{
		strcpy(filepal, argv[5]);

		if (strlen(filepal) <= 0)
		{
			printf("Palette will not be saved.\n");

			goto  EnterWidth;
		}
	}

	else
	{
		memset(filepal, 0, sizeof(filepal));
	}

EnterWidth:
	if (-1 == Rip(argv[2]))
	{
		exit(1);
	}

	WriteSprites();
}

void Usage()
{
	printf("Command syntax is: SprF2 Offset Infile Rom1 Rom2 [Palette]\n");

	exit(1);
}

//**************************** Rip *******************************
int Rip(char *filename)
{
	BYTE	Sprite[TILEDIMENSION * TILEDIMENSION];
	BYTE	*Buffer	= bank + Offset;

	LoadPCX(filename, PCXBuffer, PCXPalette);

	if ((Width & 15) != 0 && (Height & 15) != 0)
	{
		printf("Sprite width must be a multiple of 16.\n");
		
		return -1;
	}

    for (int yloop = 0; yloop < (Height / TILEDIMENSION); yloop++)
    {
        for (int xloop = 0; xloop < (Width / TILEDIMENSION); xloop++)
        {
            GetSprite(&Sprite[0], xloop * TILEDIMENSION, yloop * TILEDIMENSION, TILEDIMENSION, 
				TILEDIMENSION);

            for (int tiley = TILEDIMENSION - 1; tiley >= 0; tiley--)
            {
				for (int tilex = 0; tilex < TILEDIMENSION; tilex += 2)
                {
					BYTE Pixel1	= Sprite[tiley * TILEDIMENSION + (TILEDIMENSION - 1) - (tilex + 1)] 
						& 0x0f;
					BYTE Pixel2	= Sprite[tiley * TILEDIMENSION + (TILEDIMENSION - 1) - tilex] 
						& 0x0f;
				
					*Buffer		= (Pixel1 << 4) | Pixel2;
					Buffer++;
				}
			}
		}
	}

	return 0;
}

void WriteSprites(void)
{
	if (strlen(filepal) > 0)
	{
		for (int palloop = 0; palloop < PAL_SIZE; palloop++)
		{
			BYTE red    = ((PCXPalette[palloop * 3]) >> 2);
			BYTE green  = ((PCXPalette[palloop * 3 + 1]) >> 2);
			BYTE blue   = ((PCXPalette[palloop * 3 + 2]) >> 2);

			red   &= 0x0f;
			green &= 0x0f;
			blue  &= 0x0f;

			WORD    value = ((WORD)blue << 12) | ((WORD)red << 4) | (WORD)green;
			palette[palloop] = value;   
		}

		stream = fopen(filepal, "wb");

		if (NULL == stream)
		{
			return;
		}

		fwrite(palette, 2, sizeof(palette) / sizeof(WORD), stream);
		fclose(stream);
	}

	// open a binary file for the level array
	stream = fopen(filebank1, "wb");

	if (NULL == stream)
	{
		return;
	}

	fwrite(bank, 1, BANK_SIZE, stream);

	fclose(stream);

	// open a binary file for the level array
	stream = fopen(filebank2, "wb");

	if (NULL == stream)
	{
		return;
	}

	fwrite(bank + BANK_SIZE, 1, BANK_SIZE, stream);

	fclose(stream);
}

int LoadPCX(const char* file, BYTE *outpix, BYTE * pal)
{
	int         i, w, h;
	PCXHeader   hdr;
	long        pos;
	int         ret = 0;

	fp = fopen(file, "rb");

	if (fp == NULL)
		goto bye;

	pos = ftell(fp);
	fread (&hdr, 1, sizeof(hdr), fp);

	if (hdr.id != 0x0A || hdr.version != 5 || hdr.rle != 1
	 || (hdr.bpp == 8 && hdr.nbitp != 1))
		goto bye;

	w       = hdr.xend - hdr.xstart + 1;
	Width   = w;

	h       = hdr.yend - hdr.ystart + 1;
	Height  = h;

	if (hdr.bpp == 1 && hdr.nbitp == 4)
	{
		 memset(pal, 0, 768);

		 for (i = 0; i < 48; i++)
			pal[i] = hdr.pal[i] >> 2;
	}

	else if (hdr.bpp == 8 && hdr.nbitp == 1)
	{
		fseek(fp, pos + FileSize(file) - 768, SEEK_SET);

		for (i = 0; i < 768; i++)
			pal[i] = ((BYTE)getc(fp)) >> 2;
	}

	else
		goto bye;

	ret = 1;

	fseek(fp, pos + sizeof(PCXHeader), SEEK_SET);

	while (h-- > 0)
	{
		BYTE   c;
		BYTE  *outpt;
		int             np = 0;

		outpt = outpix;

		memset(outpix, 0, w);

		for (np = 0; np < hdr.nbitp; np++)
		{
			i = 0;
			outpix = outpt;

			do
			{
				c = (BYTE)getc(fp);

				if ((c & 0xC0) != 0xC0)
				{
					if (hdr.bpp == 1)
					{
						int k;
						for (k = 7; k >= 0; k--)
							*outpix++ |= ((c >> k) & 1) << np;
						i += 8;
					}

					else
					{
						*outpix++ = c;
						i++;
					}
				}

				else
				{
					BYTE v;
					v = (BYTE)getc(fp);
					c &= ~0xC0;

					while (c > 0 && i < w)
					{
						if (hdr.bpp == 1)
						{
							int k;

							for (k = 7; k >= 0; k--)
								*outpix++ |= ((v >> k) & 1) << np;

							i += 8;
						}

						else
						{
							*outpix++ = v;
							i++;
						}

						c--;
					}
				}
			}
			while (i < w);
		}
	}

  bye:
	if (fp != NULL)
		fclose(fp);

	return ret;
}

long FileSize(const char *fname)
{
	FILE *f;
	long length;

	f = fopen(fname, "rb");

	if (f == NULL)
		return 0;

	fseek(f, 0, SEEK_END);

	length = ftell(f);

	fclose(f);

	return length;
}

void GetSprite(BYTE *tile, int x, int y, int width, int height)
{
	BYTE *localtile = tile;
	BYTE *buffer    = &PCXBuffer[0];

	buffer += y * Width + x;

	for (int outerloop = 0; outerloop < height; outerloop++)
	{
		for (int innerloop = 0; innerloop < width; innerloop++)
		{
			*localtile++ = *buffer++;
		}

		buffer += Width - width;
	}
}
