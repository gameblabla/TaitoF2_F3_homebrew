//*****************************************************************
//* PCX to F3 tile converter. (Paletted)
//*****************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define PAL_SIZE		64
#define TILEDIMENSION	16

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef BYTE			BOOL;

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

#define BANKSIZE   (1024 * 1024)

DWORD	Width;
DWORD	Height;
char	FileBank1[32];
char	FileBank2[32];
char	FileBank3[32];
char	FilePal[32];
BYTE    Bank1[BANKSIZE];
BYTE    Bank2[BANKSIZE];
BYTE    Bank3[BANKSIZE];
DWORD	Palette[PAL_SIZE];
DWORD	Offset		= 0;
DWORD	Palettes	= 1;
BYTE	PcxBuffer[1024 * 1024];
BYTE	PcxPalette[768];

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
long Rip(char *Filename);
void WriteTiles();
BOOL LoadPCX(char *File, BYTE *Buffer, BYTE *Palette);
void GetTile(BYTE *Tile, DWORD X, DWORD Y);
DWORD CheckTile(BYTE *Tile);

void main(int argc, char *argv[])
{
    printf("PCX to F3 tile converter.\n");

    if (argc < 6)
    {
        Usage();
    }

	if (8 == argc)
	{
		Palettes	= (DWORD)atol(argv[7]);
	}

    memset(Bank1, 0xFF, BANKSIZE);
    memset(Bank2, 0xFF, BANKSIZE);
    memset(Bank3, 0xFF, BANKSIZE);
    memset(Palette, 0, PAL_SIZE * 4);
    
    Offset	= atol(argv[1]);

	strcpy(FileBank1, argv[3]);

    if ("" == FileBank1)
    {
        Usage();
    }

    strcpy(FileBank2, argv[4]);

    if ("" == FileBank2)
    {
        Usage();
    }

    strcpy(FileBank3, argv[5]);

    if ("" == FileBank3)
    {
        Usage();
    }

	if (Offset > 0)
	{
		FILE	*Handle = fopen(FileBank1, "rb");

		if (Handle != NULL)
		{
			fread(Bank1, 1, BANKSIZE, Handle);
			fclose(Handle);
		}

		Handle = fopen(FileBank2, "rb");

		if (Handle != NULL)
		{
			fread(Bank2, 1, BANKSIZE, Handle);
			fclose(Handle);
		}

		Handle = fopen(FileBank3, "rb");

		if (Handle != NULL)
		{
			fread(Bank3, 1, BANKSIZE, Handle);
			fclose(Handle);
		}
	}

    if (argc > 6)
    {
        strcpy(FilePal, argv[6]);

        if (strlen(FilePal) <= 0)
        {
            printf("Palette will not be saved.\n");
        }
    }

    else
    {
		memset(FilePal, 0, sizeof(FilePal));
    }

    if (-1 == Rip(argv[2]))
    {
        exit(1);
    }

    WriteTiles();

	printf("Next Offset = %d", Offset); 
}

void Usage()
{
    printf("Command syntax is: PcxToF3 Offset Infile.pcx Rom1 Rom2 Rom3 [Palette] [Palettes]\n");

    exit(1);
}

long Rip(char *Filename)
{
	BYTE	Tile[TILEDIMENSION * TILEDIMENSION];
	DWORD	Position	= Offset;
	DWORD	ExtraPos	= Offset / 32;

	LoadPCX(Filename, PcxBuffer, PcxPalette);

	if ((Width % TILEDIMENSION) != 0 && (Height % TILEDIMENSION) != 0)
	{
		printf("Picture must be a multiple of %d.\n", TILEDIMENSION);
        
		return	-1;
	}

    for (DWORD YLoop = 0; YLoop < (Height / TILEDIMENSION); YLoop++)
    {
        for (DWORD XLoop = 0; XLoop < (Width / TILEDIMENSION); XLoop++)
        {
            DWORD	x	= XLoop * TILEDIMENSION;
            DWORD	y	= YLoop * TILEDIMENSION;
		
            GetTile(Tile, x, y);

            for (DWORD TileY = 0; TileY < TILEDIMENSION; TileY++)
            {
				WORD	Extra1	= 0;
				WORD	Extra2	= 0;
				DWORD	Bank	= 0;

				for (long TileX = 0; TileX < TILEDIMENSION; TileX += 2)
                {
					BYTE	Pixel1	= Tile[TileY * TILEDIMENSION + TileX + 1] & 0x0F;
					BYTE	Over1	= (Tile[TileY * TILEDIMENSION + TileX + 1] & 0x30) >> 4;
					BYTE	Pixel2	= Tile[TileY * TILEDIMENSION + TileX] & 0x0F;
					BYTE	Over2	= (Tile[TileY * TILEDIMENSION + TileX] & 0x30) >> 4;
				
					if (0 == Bank)
					{
						Bank1[Position]	= (Pixel1 << 4) | Pixel2;

						if (Position & 1)
						{
							Position--;
							Bank	= 1;
						}
						
						else
						{
							Position++;
						}
					}

					else
					{
						Bank2[Position]	= (Pixel1 << 4) | Pixel2;

						if (Position & 1)
						{
							Position++;
							Bank	= 0;
						}
						
						else
						{
							Position++;
						}
					}
				
					Extra1	|= (Over1 & 1) << (TileX + 1);
					Extra1	|= (Over2 & 1) << TileX;
					
					Extra2	|= (Over1 & 2) << (TileX + 1);
					Extra2	|= (Over2 & 2) << TileX;
				}

				Bank3[ExtraPos + 0]	= (BYTE)(Extra1 & 0xFF);
				Bank3[ExtraPos + 1]	= (BYTE)(Extra2 & 0xFF);
				Bank3[ExtraPos + 2]	= (BYTE)((Extra1 & 0xFF00) >> 8);
				Bank3[ExtraPos + 3]	= (BYTE)((Extra2 & 0xFF00) >> 8);
				
				ExtraPos	+= 4;
			}
		}
    }

	Offset	= Position;
    
	return	0;
}

void WriteTiles()
{
    FILE	*Handle;

    for (DWORD Loop = 0; Loop < PAL_SIZE; Loop++)
    {
        BYTE	Red		= (PcxPalette[Loop * 3] << 2);
        BYTE	Green	= (PcxPalette[Loop * 3 + 1] << 2);
        BYTE	Blue	= (PcxPalette[Loop * 3 + 2] << 2);

		DWORD	Value	= ((DWORD)Red << 8) | ((DWORD)Green << 16) | ((DWORD)Blue << 24);
        Palette[Loop]	= Value;	
    }

	if (strlen(FilePal) > 0)
	{
		Handle	= fopen(FilePal, "wb");

		if (NULL == Handle)
		{
			return;
		}
		
		fwrite(Palette, 4, Palettes * 64, Handle);
		fclose(Handle);
	}

    // Open a binary file for the even tile bytes
    Handle	= fopen(FileBank1, "wb");

	if (NULL == Handle)
	{
		return;
	}

    fwrite(Bank1, 1, BANKSIZE, Handle);
    fclose(Handle);

    // Open a binary file for the odd tile bytes
    Handle	= fopen(FileBank2, "wb");

	if (NULL == Handle)
	{
		return;
	}

    fwrite(Bank2, 1, BANKSIZE, Handle);
    fclose(Handle);

    // Open a binary file for the tile upper bits
    Handle	= fopen(FileBank3, "wb");

	if (NULL == Handle)
	{
		return;
	}

    fwrite(Bank3, 1, BANKSIZE, Handle);
    fclose(Handle);
}

BOOL LoadPCX(char *File, BYTE *Buffer, BYTE *Palette)
{
	FILE		*handle;
	PCXHeader   PcxHeader;
	DWORD		Position;
	DWORD		Length;

	handle	= fopen(File, "rb");

	if (NULL == handle)
	{
		return	FALSE;
	}

	Position	= ftell(handle);
	fseek(handle, 0, SEEK_END);
	Length		= ftell(handle);
	rewind(handle);

	fread(&PcxHeader, 1, sizeof(PcxHeader), handle);

	if (PcxHeader.ID != 0x0A || PcxHeader.Version != 5 || PcxHeader.RLE != 1 
		|| (8 == PcxHeader.BPP && PcxHeader.BitPlanes != 1))
	{
		return	FALSE;
	}

	Width   = PcxHeader.EndX - PcxHeader.StartX + 1;
	Height  = PcxHeader.EndY - PcxHeader.StartY + 1;

	if (1 == PcxHeader.BPP && 4 == PcxHeader.BitPlanes)
	{
		 memset(Palette, 0, 768);

		for (DWORD loop = 0; loop < 48; loop++)
		{
			Palette[loop]	= PcxHeader.Palette[loop] >> 2;
		}
	}

	else if (8 == PcxHeader.BPP && 1 == PcxHeader.BitPlanes)
	{
		fseek(handle, Position + Length - 768, SEEK_SET);

		for (DWORD loop = 0; loop < 768; loop++)
		{
			Palette[loop]	= ((BYTE)getc(handle)) >> 2;
		}
	}

	else
	{
		return	FALSE;
	}

	fseek(handle, Position + sizeof(PCXHeader), SEEK_SET);

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
				Pixel	= (BYTE)getc(handle);

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
					BYTE	Run	= (BYTE)getc(handle);
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

	if (handle != NULL)
	{
		fclose(handle);
	}
	
	return	TRUE;
}

void GetTile(BYTE *Tile, DWORD X, DWORD Y)
{
	BYTE *Buffer	= PcxBuffer;

	Buffer += Y * Width + X;

	for (DWORD outerloop = 0; outerloop < TILEDIMENSION; outerloop++)
	{
		for (DWORD innerloop = 0; innerloop < TILEDIMENSION; innerloop++)
		{
			*Tile	= *Buffer;
			Tile++;
			Buffer++;
		}

		Buffer += Width - TILEDIMENSION;
	}
}

DWORD CheckTile(BYTE *Tile)
{
	for (DWORD Loop = 0; Loop < TILEDIMENSION * TILEDIMENSION; Loop++)
	{
		if (Tile[Loop] != 0)
		{
			return	TRUE;
		}
	}
	
	return	FALSE;
}

