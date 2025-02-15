//*****************************************************************
//* PCX to Taito F3 font converter.
//*****************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define TILESIZE		(8 * 8)

typedef unsigned char	BYTE;
typedef unsigned long	DWORD;
typedef unsigned short	WORD;
typedef BYTE			BOOL;

#ifndef	TRUE
#define	TRUE			1
#endif
#ifndef	FALSE
#define	FALSE			0
#endif

DWORD	Width;
DWORD	Height;
DWORD	Characters	= 0;

void WriteFont(char *Filename);
int Rip(char *filename);
BOOL LoadPCX(char *File, BYTE *Buffer);
void GetSprite(BYTE *tile, int x, int y, int width, int height);

BYTE	PcxBuffer[1024 * 1024];
BYTE	Fonts[256][TILESIZE];
BYTE	Converted[256][TILESIZE / 2];

struct PCXHeader
{
	BYTE ID;
	BYTE Version;
	BYTE RLE;
	BYTE BPP;
	WORD StartX;
	WORD StartY;
	WORD EndX;
	WORD EndY;
	WORD HRes;
	WORD VRes;
	BYTE Palette[48];
	BYTE Reserved1;
	BYTE BitPlanes;
	WORD BytesPerLine;
	WORD PaletteType;
	WORD HSize;
	WORD VSize;
	BYTE Reserved2[54];
};

void Usage();

void main(int argc, char *argv[])
{
    printf("PCX to Taito F3 font converter.\n");

    if (argc < 3)
	{
		Usage();
	}

    if (-1 == Rip(argv[1]))
	{
		exit(1);
	}

	WriteFont(argv[2]);
}

void Usage()
{
    printf("Command syntax is: PcxF3Font InFile.pcx OutFile.bin\n");

	exit(1);
}

int Rip(char *filename)
{
    LoadPCX(filename, PcxBuffer);

    if ((Width & 7) != 0 && (Height & 7) != 0)
    {
        printf("Character width must be a multiple of 8.\n");
        
		return -1;
    }

    for (DWORD yloop = 0; yloop < (Height / 8); yloop++)
    {
        for (DWORD xloop = 0; xloop < (Width / 8); xloop++)
        {
            long	x	= xloop * 8;
            long	y	= yloop * 8;

            GetSprite(&Fonts[Characters][0], x, y, 8, 8);
			
			Characters++;
		}
    }

    for (DWORD Char = 0; Char < Characters; Char++)
	{
		for (DWORD yloop = 0; yloop < 8; yloop++)
		{
			DWORD	Position	= 0;

			for (long xloop = 7; xloop > 0; xloop -= 2)
			{
				BYTE	Value	= (Fonts[Char][yloop * 8 + xloop] << 4) | Fonts[Char][yloop * 8 + xloop - 1];
					
				Converted[Char][yloop * 4 + Position]	= Value;
			
				Position++;
			}
		}
	}
			
    return 0;
}

void WriteFont(char *Filename)
{
	FILE	*Handle	= fopen(Filename, "wb");

	if (NULL == Handle)
	{		
		return;
	}

	fwrite(Converted, Characters, 32, Handle);

	fclose(Handle);
}

BOOL LoadPCX(char *File, BYTE *Buffer)
{
	PCXHeader   PcxHeader;
	DWORD		Position;
	DWORD		Length;

	FILE	*Handle	= fopen(File, "rb");

	if (NULL == Handle)
	{
		return	FALSE;
	}

	Position	= ftell(Handle);
	fseek(Handle, 0, SEEK_END);
	Length		= ftell(Handle);
	rewind(Handle);

	fread(&PcxHeader, 1, sizeof(PcxHeader), Handle);

	if (PcxHeader.ID != 0x0A || PcxHeader.Version != 5 || PcxHeader.RLE != 1 
		|| (8 == PcxHeader.BPP && PcxHeader.BitPlanes != 1))
	{
		return	FALSE;
	}

	Width   = PcxHeader.EndX - PcxHeader.StartX + 1;
	Height  = PcxHeader.EndY - PcxHeader.StartY + 1;

	fseek(Handle, Position + sizeof(PCXHeader), SEEK_SET);

	DWORD	TmpHeight	= Height;

	while (TmpHeight-- > 0)
	{
		BYTE	Pixel;
		BYTE	*Output;

		Output	= Buffer;

		memset(Buffer, 0, Width);

		for (DWORD Planes = 0; Planes < PcxHeader.BitPlanes; Planes++)
		{
			DWORD	Done	= 0;
			Buffer	= Output;

			do
			{
				Pixel	= (BYTE)getc(Handle);

				if ((Pixel & 0xC0) != 0xC0)
				{
					if (1 == PcxHeader.BPP)
					{
						for (DWORD Bits = 7; Bits >= 0; Bits--)
						{
							*Buffer++	|= ((Pixel >> Bits) & 1) << Planes;
						}

						Done	+= 8;
					}

					else
					{
						*Buffer++	= Pixel;
						Done++;
					}
				}

				else
				{
					BYTE	Run	= (BYTE)getc(Handle);
					Pixel	&= ~0xC0;

					while (Pixel > 0 && Done < Width)
					{
						if (1 == PcxHeader.BPP)
						{
							for (DWORD Bits = 7; Bits >= 0; Bits--)
							{
								*Buffer++	|= ((Run >> Bits) & 1) << Planes;
							}

							Done	+= 8;
						}

						else
						{
							*Buffer++	= Run;
							Done++;
						}

						Pixel--;
					}
				}
			}
			
			while (Done < Width);
		}
	}

	if (Handle != NULL)
	{
		fclose(Handle);
	}
	
	return	TRUE;
}

void GetSprite(BYTE *tile, int x, int y, int width, int height)
{
    BYTE *localtile = tile;
    BYTE *buffer    = &PcxBuffer[0];

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
