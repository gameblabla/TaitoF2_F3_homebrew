#include <stdio.h>
#include <windows.h>

#define		BANK_SIZE	(128 * 1024)
void Usage();

void main(int argc, char *argv[ ]) 
{
	char	filename[128];
	char	oddfile1[128];
	char	evenfile1[128];
	char	oddfile2[128];
	char	evenfile2[128];
	char	*buffer		= NULL;
	char	*odd1		= NULL;
	char	*odd2		= NULL;
	char	*even1		= NULL;
	char	*even2		= NULL;
	FILE	*oddhandle	= NULL;
	FILE	*evenhandle	= NULL;
	long	loop;
	
	if (argc < 4)
	{	   
		Usage();
	}

	strcpy(filename, argv[1]);
	strcpy(oddfile1, argv[2]);
	strcpy(evenfile1, argv[3]);
	strcpy(oddfile2, argv[4]);
	strcpy(evenfile2, argv[5]);

	FILE *handle = fopen(filename, "rt");

	if (NULL == handle)
	{
		printf("Unable to open %s\n", filename);

		return;
	}

	fseek(handle, 0, SEEK_END);

	long length		= ftell(handle);
	long length2	= 0;

	rewind(handle);

	if (length <= 0)
	{
		printf("%s is an invalid file.\n", filename);
		
		goto End;
	}

	buffer	= new char[length];

	if (NULL == buffer)
	{
		printf("Unable to allocate memory.");
		
		goto End;
	}

	fread(buffer, 1, length, handle);
	fclose(handle);

	if (length >= 256 * 1024)
	{
		length2	= length - (256 * 1024);
		length	= 256 * 1024;
	}

	odd1	= new char[BANK_SIZE];

	if (NULL == odd1)
	{
		printf("Unable to allocate memory.");
		
		goto End;
	}

	memset(odd1, 0, BANK_SIZE);

	even1	= new char[BANK_SIZE];

	if (NULL == even1)
	{
		printf("Unable to allocate memory.");
		
		goto End;
	}

	memset(even1, 0, BANK_SIZE);

	odd2	= new char[BANK_SIZE];

	if (NULL == odd2)
	{
		printf("Unable to allocate memory.");
		
		goto End;
	}

	memset(odd2, 0, BANK_SIZE);

	even2	= new char[BANK_SIZE];

	if (NULL == even2)
	{
		printf("Unable to allocate memory.");
		
		goto End;
	}

	memset(even2, 0, BANK_SIZE);

	for (loop = 0; loop < length; loop++)
	{
		if (loop & 1)
		{
			odd1[loop / 2] = buffer[loop];
		}

		else
		{
			even1[loop / 2] = buffer[loop];
		}
	}

	for (loop = 0; loop < length2; loop++)
	{
		if (loop & 1)
		{
			odd2[loop / 2] = buffer[loop + 256 * 1024];
		}

		else
		{
			even2[loop / 2] = buffer[loop + 256 * 1024];
		}
	}

	oddhandle	= fopen(oddfile1, "wb");

	if (NULL == oddhandle)
	{
		printf("Unable to create %s\n", oddfile1);
		
		goto End;
	}

	fwrite(odd1, 1, BANK_SIZE, oddhandle);

	fclose(oddhandle);

	evenhandle	= fopen(evenfile1, "wb");

	if (NULL == evenhandle)
	{
		printf("Unable to create %s\n", evenfile1);
		
		goto End;
	}

	fwrite(even1, 1, BANK_SIZE, evenhandle);

	fclose(evenhandle);

	oddhandle	= fopen(oddfile2, "wb");

	if (NULL == oddhandle)
	{
		printf("Unable to create %s\n", oddfile2);
		
		goto End;
	}

	fwrite(odd2, 1, BANK_SIZE, oddhandle);

	fclose(oddhandle);

	evenhandle	= fopen(evenfile2, "wb");

	if (NULL == evenhandle)
	{
		printf("Unable to create %s\n", evenfile2);
		
		goto End;
	}

	fwrite(even2, 1, BANK_SIZE, evenhandle);

	fclose(evenhandle);

End:	
	if (buffer != NULL)
	{
		delete buffer;
		buffer	= NULL;
	}

	if (odd1 != NULL)
	{
		delete odd1;
		odd1	= NULL;
	}
	
	if (odd2 != NULL)
	{
		delete odd2;
		odd2	= NULL;
	}
	
	if (even1 != NULL)
	{
		delete even1;
		even1	= NULL;
	}

	if (even2 != NULL)
	{
		delete even2;
		even2	= NULL;
	}
}

void Usage()
{
	printf("Command syntax is: SplitF2 Infile OddRom1 EvenRom1 OddRom2 EvenRom2\n" );

	exit(1);
}
