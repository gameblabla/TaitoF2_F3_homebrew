#include <stdio.h>
#include <stdlib.h>
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

#define	BANK_SIZE	(512 * 1024)

void Usage();

int main(int argc, char *argv[])
{
	BYTE	*Buffer		= NULL;
	BYTE	*Odd1		= NULL;
	BYTE	*Odd2		= NULL;
	BYTE	*Even1		= NULL;
	BYTE	*Even2		= NULL;
	DWORD	Loop;

	if (argc < 4)
	{
		Usage();
	}

	FILE *Handle	= fopen(argv[1], "rb");

	if (NULL == Handle)
	{
		printf("Unable to open %s\n", argv[1]);

		return	1;
	}

	fseek(Handle, 0, SEEK_END);

	DWORD	Length	= ftell(Handle);

	rewind(Handle);

	if (Length <= 0)
	{
		printf("%s is an invalid file.\n", argv[1]);

		goto End;
	}

	Buffer	= new BYTE[Length];

	if (NULL == Buffer)
	{
		printf("Unable to allocate memory.");

		goto End;
	}

	fread(Buffer, 1, Length, Handle);
	fclose(Handle);

	Odd1	= new BYTE[BANK_SIZE];

	if (NULL == Odd1)
	{
		printf("Unable to allocate memory.");

		goto End;
	}

	memset(Odd1, 0, BANK_SIZE);

	Even1	= new BYTE[BANK_SIZE];

	if (NULL == Even1)
	{
		printf("Unable to allocate memory.");

		goto End;
	}

	memset(Even1, 0, BANK_SIZE);

	Odd2	= new BYTE[BANK_SIZE];

	if (NULL == Odd2)
	{
		printf("Unable to allocate memory.");

		goto End;
	}

	memset(Odd2, 0, BANK_SIZE);

	Even2	= new BYTE[BANK_SIZE];

	if (NULL == Even2)
	{
		printf("Unable to allocate memory.");

		goto End;
	}

	memset(Even2, 0, BANK_SIZE);

	for (Loop = 0; Loop < Length; Loop++)
	{
		// The bytes are swapped.
		if (3 == (Loop & 3))
		{
			Even2[Loop / 4]	= Buffer[Loop];
		}

		else if (2 == (Loop & 2))
		{
			Odd2[Loop / 4] = Buffer[Loop];
		}

		else if (1 == (Loop & 1))
		{
			Even1[Loop / 4]	= Buffer[Loop];
		}

		else
		{
			Odd1[Loop / 4]	= Buffer[Loop];
		}
	}

	Handle	= fopen(argv[2], "wb");

	if (NULL == Handle)
	{
		printf("Unable to create %s\n", argv[2]);

		goto End;
	}

	fwrite(Odd1, 1, BANK_SIZE, Handle);

	fclose(Handle);

	Handle	= fopen(argv[3], "wb");

	if (NULL == Handle)
	{
		printf("Unable to create %s\n", argv[3]);

		goto End;
	}

	fwrite(Even1, 1, BANK_SIZE, Handle);

	fclose(Handle);

	Handle	= fopen(argv[4], "wb");

	if (NULL == Handle)
	{
		printf("Unable to create %s\n", argv[4]);

		goto End;
	}

	fwrite(Odd2, 1, BANK_SIZE, Handle);

	fclose(Handle);

	Handle	= fopen(argv[5], "wb");

	if (NULL == Handle)
	{
		printf("Unable to create %s\n", argv[5]);

		goto End;
	}

	fwrite(Even2, 1, BANK_SIZE, Handle);

	fclose(Handle);

End:
	if (Buffer != NULL)
	{
		delete [] Buffer;
		
		Buffer	= NULL;
	}

	if (Odd1 != NULL)
	{
		delete [] Odd1;
		
		Odd1	= NULL;
	}

	if (Odd2 != NULL)
	{
		delete [] Odd2;
		
		Odd2	= NULL;
	}

	if (Even1 != NULL)
	{
		delete [] Even1;
		
		Even1	= NULL;
	}

	if (Even2 != NULL)
	{
		delete [] Even2;
		
		Even2	= NULL;
	}

	return	0;
}

void Usage()
{
	printf("Command syntax is: SplitF3 Infile EvenRom1 OddRom1 EvenRom2 OddRom2\n" );

	exit(1);
}
