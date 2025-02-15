Taito F2 conversion tools by Charles Doty (cdoty@earthlink.net)

This archive contains:
MapF2.cpp and MapF2.exe (Win32 console app).
	Converts maps from MapMaker
	(http://www.geocities.com/SiliconValley/Vista/6774/Projects.html#Map) to
	Taito F2 format.

	To use type: 
	'MapF2 Offset Infile Outfile'
	
	The offset is added to the tile number to determine where the tiles comes 
	from. Offset is the starting location in the tile bank of the tiles for
	this map. This program also converts the numbers to Motorola 68000 byte 
	ordering.

TileF2.cpp TileF2.exe (Win32 console app).
	Converts a 256 color (only first 16 colors indexes used) PCX to a 8 x 8
	Taito F2 Tile bank.

	To use type:
	'TileF2 Offset Infile Rom [Palette]'
	
	The palette is optional. Width and height of the PCX must be a multiple of 
	8.
	
SprF2.cpp SprF2.exe (Win32 console app).
	Converts a 256 color (only first 16 colors indexes used) PCX to 16 x 16
	Taito F2 Sprite banks(2).

	To use type:
	'SprF2 Offset Infile Rom1 Rom2 [Palette]'
	
	The palette is optional. Width and height of the PCX must be a multiple of 
	16.
	
FontF2.cpp FontF2.exe (Win32 console app).
	Converts a 256 color (only first 4 colors indexes used) PCX to 8 x 8
	Taito F2 font data.

	To use type:
	'FontF2 Infile OutFile [Palette]'
	
	The palette is optional. Width and height of the PCX must be a multiple of 
	8. Palette is saved out with 16 indexes. Color 0 is transparent.
	
SplitF2.cpp and SplitF2.exe (Win32 console app).
	Splits a binary file into odd and even banks.

	To use type: 
	'SplitF2 Infile OddRom1 EvenRom1 OddRom2 EvenRom2'


A site dedicated to arcade system development is available at 
http://arcadedev.emuvibes.com.
