//*****************************************************************
//* MapMaker to Taito F2 map converter.
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
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef struct
{
    DWORD   Size;       // Size of header
    DWORD   Magic;      // Magic number
    BYTE    Version;    // Version number
    BYTE    SubVersion; // Sub Version number
    BYTE    Transparent;// Transparent color
    BYTE    Padding;    // Padding
    DWORD   Width;      // Width of Map
    DWORD   Height;     // Height of Map
    char    Path[255];  // Path to tile file
    char    Tile[32];   // Name of tile file used
    BYTE    Padding2;   // Padding 2
} MAPHEADER;

char        filein[32];
char        fileout[32];
WORD        bankin[256 * 64 * 10];
WORD        bankout[256 * 64];
long        offset;
long        length;
MAPHEADER   MapHeader;

void Usage();
void ConvertMap();

void main(int argc, char *argv[])
{
    printf("Mapmaker to Taito F2 map converter.\n");

    if (argc < 4)
    {
        Usage();
    }

	memset(bankin, 0, sizeof(bankin));
	memset(bankout, 0, sizeof(bankout));
    
	offset = atoi(argv[1]);

    strcpy(filein, argv[2]);
    strcpy(fileout, argv[3]);

    if ("" == filein || "" == fileout)
    {
        Usage();
    }

    FILE *handle = fopen(filein, "rb");

	if (handle != NULL)
    {
		fseek(handle, 0, SEEK_END);
        length = (ftell(handle) - sizeof(MAPHEADER)) / 2;
		rewind(handle);

		if (length > sizeof(bankin) / 2)
		{
			length = sizeof(bankin) / 2;
		}

        fread(&MapHeader, sizeof(MAPHEADER), 1, handle);
        fread(bankin, 2, length, handle);
		fclose(handle);
    }

    ConvertMap();
}

void Usage()
{
    printf("Command syntax is: MapF2 Offset Infile Outfile\n");

    exit(1);
}

void ConvertMap()
{
	for (DWORD loop = 0; loop < MapHeader.Width * MapHeader.Height; loop++)
	{
		WORD Map		= bankin[loop * 10];
		
		bankout[loop]	= ((Map & 0xff00) >> 8) | (((Map & 0xff) + (WORD)offset) << 8);
	}
	
	FILE *handle = fopen(fileout, "wb");

	if (NULL == handle)
	{
		return;
	}

	fwrite(bankout, 2, MapHeader.Width * MapHeader.Height, handle);
	fclose(handle);
}
