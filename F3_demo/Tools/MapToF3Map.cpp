//*****************************************************************
//* Mappy MAR to F3 map converter.
//*****************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

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

char	FileIn[32];
char	FileOut[32];
DWORD	Offset			= 0;
DWORD	Length;
DWORD	Width;
DWORD	Height;
DWORD	MapWidth;
DWORD	MapHeight;
WORD	PaletteOverride	= 0x0;
DWORD	BankIn[128 * 128];
DWORD	BankOut[128 * 128];

void Usage();
void ConvertMap();

void main(int argc, char *argv[])
{
	printf("Mappy Map to F3 map converter.\n");

    if (argc < 6)
    {
        Usage();
    }

	if (7 == argc)
	{
		PaletteOverride	= (WORD)atol(argv[6]);
	}

	memset(BankIn, 0, sizeof(BankIn));
	memset(BankOut, 0, sizeof(BankOut));
    
	Offset		= atoi(argv[1]);
	MapWidth	= atoi(argv[2]);
	MapHeight	= atoi(argv[3]);

    strcpy(FileIn, argv[4]);
    strcpy(FileOut, argv[5]);

    if ("" == FileIn || "" == FileOut)
    {
        Usage();
    }

    FILE *Handle = fopen(FileIn, "rb");

	if (Handle != NULL)
    {
		fseek(Handle, 0, SEEK_END);
        Length	= (ftell(Handle) - 8) / 4;
		rewind(Handle);

		if (Length > sizeof(BankIn) / 4)
		{
			Length = sizeof(BankIn) / 4;
		}

		// Skip width and height
		fseek(Handle, 8, SEEK_SET);
        fread(BankIn, 4, Length, Handle);
		fclose(Handle);
    }

    ConvertMap();
}

void Usage()
{
    printf("Command syntax is: MapToF3Map Offset MapWidth MapHeight Infile Outfile [Palette]\n");

    exit(1);
}

void ConvertMap()
{
	DWORD	Counter	= 0;

	for (DWORD OuterLoop = 0; OuterLoop < MapHeight; OuterLoop++)
	{
		for (DWORD InnerLoop = 0; InnerLoop < MapWidth; InnerLoop++)
		{
			DWORD Map	= BankIn[OuterLoop * MapWidth + InnerLoop] + Offset;

			BankOut[Counter]		= (DWORD)(((Map & 0xFF00) >> 8) | ((Map & 0xFF) << 8)) << 16;
			BankOut[Counter]		|= (PaletteOverride << 8);

			Counter++;
		}
	}
			
	FILE *Handle = fopen(FileOut, "wb");

	if (NULL == Handle)
	{
		return;
	}

	fwrite(BankOut, 4, MapWidth * MapHeight, Handle);
	fclose(Handle);
}
