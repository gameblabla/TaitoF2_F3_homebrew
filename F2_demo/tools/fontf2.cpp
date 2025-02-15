//*****************************************************************
//* PCX to Taito F2 font converter.
//*****************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define PAL_SIZE        16
#define BANK_SIZE		4096
#define TILEDIMENSION	8

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef BYTE           BOOL;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

int  Rip(char *filename);
void WriteTiles(void);
int  LoadPCX(const char* file, BYTE *outpix, BYTE * pal);
long FileSize(const char *fname);
void GetTile(BYTE *tile, int x, int y, int width, int height);

FILE   *fp;
FILE   *stream;              // level output data file
WORD    size = 0;
int     Width, Height, pal, start;
WORD    palval;
FILE    *handle;
char    filebank[32];
char    filepal[32];
BYTE    bank[BANK_SIZE];
WORD    palette[PAL_SIZE];

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
	printf("PCX to Taito F2 font converter.\n");

	if (argc < 3)
	{
		Usage();
	}

	memset(bank, 0, sizeof(bank));
	memset(palette, 0, PAL_SIZE * 2);
	
	strcpy(filebank, argv[2]);

	if ("" == filebank)
	{
		Usage();
	}

	long length;

	handle = fopen(filebank, "rb");

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

	if (argc > 3)
	{
		strcpy(filepal, argv[3]);

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
	if (-1 == Rip(argv[1]))
	{
		exit(1);
	}

	WriteTiles();
}

void Usage()
{
	printf("Command syntax is: FontF2 Infile OutFile [Palette]\n");

	exit(1);
}

//**************************** Rip *******************************
int Rip(char *filename)
{
	BYTE	Tile[TILEDIMENSION * TILEDIMENSION];
	BYTE	*Buffer	= bank;

	LoadPCX(filename, PCXBuffer, PCXPalette);

	if ((Width & 7) != 0 && (Height & 7) != 0)
	{
		printf("Tile width must be a multiple of 8.\n");
		
		return -1;
	}

    for (int yloop = 0; yloop < (Height / TILEDIMENSION); yloop++)
    {
        for (int xloop = 0; xloop < (Width / TILEDIMENSION); xloop++)
        {
            GetTile(&Tile[0], xloop * TILEDIMENSION, yloop * TILEDIMENSION, TILEDIMENSION, 
				TILEDIMENSION);

            for (int tiley = TILEDIMENSION - 1; tiley >= 0; tiley--)
            {
				*Buffer			= 0;
				*(Buffer + 1)	= 0;
				
				for (int tilex = 0; tilex < TILEDIMENSION; tilex++)
                {
					BYTE Pixel	= Tile[tiley * TILEDIMENSION + tilex] & 0x03;
				
					*Buffer			|= ((Pixel & 2) >> 1) << tilex;
					*(Buffer + 1)	|= (Pixel & 1) << tilex;
				}
			
				Buffer += 2;
			}
		}
	}

	return 0;
}

void WriteTiles(void)
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
	stream = fopen(filebank, "wb");

	if (NULL == stream)
	{
		return;
	}

	fwrite(bank, 1, BANK_SIZE, stream);

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

void GetTile(BYTE *tile, int x, int y, int width, int height)
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
