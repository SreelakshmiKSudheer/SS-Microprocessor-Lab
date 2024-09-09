#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char op[6];
	char val[2];
}OPTAB;

typedef struct SYMTAB
{
	char sym[6];
	char add[4];
	int errflag;
	struct SYMTAB *link;
} SYMTAB;

int power(int num, int p)
{
	int i, ret = 1;

	if(p == 0)
		return ret;
	else
	{
		for(i = 0; i < p; i++)
			ret *= 16;
		return ret;
	}
}

int hexdec(char hex)
{
	int dec;

	if(hex >= '0' && hex <= '9')
		dec = hex - '0';
	if(hex >= 'A' && hex <= 'F')
		dec = 10 + (hex - 'A');

	return dec;
}

char dechex(int dec)
{
	char hex;

	if(dec >= 0 && dec <= 9)
		hex = '0' + dec;
	if(dec >= 10 && dec <= 15)
		hex = 'A' + (dec - 10);

	return hex;
}

int htod(char str[])
{
	int i, num = 0;
	int n = strlen(str);
	char c;

	for(i = 0; i < n; i++)
	{
		c = str[n-i-1];
		num += hexdec(c) * power(16,i);
	}

	return num;
}

void dtoh(int dec, char hex[])
{
	int dig, i = 0;
	char rev[6] = "0000";

	while(dec)
	{
		dig = dec % 16;
		rev[i++] = dechex(dig);
		dec /= 16;
	}
	rev[i] = '\0';

	int n = strlen(rev);

	for(i = 0; i < n; i++)
	{
		hex[i] = rev[n-i-1];
	}
	hex[i] = '\0';
}

OPTAB optab[3] = {{"LDA","00"},{"MUL","20"},{"STA","0C"}};
SYMTAB *header = NULL;  // Initialize pointer

int is_op(char opc[])
{
	int i;

	for(i = 0; i < 3; i++)
	{
		if(strcmp(opc, optab[i].op) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int is_found(char sym[], SYMTAB *header)
{
	SYMTAB *ptr = header;

	while(ptr != NULL)
	{
		if(strcmp(sym, ptr->sym) == 0)
		{
			return 1;
		}
		ptr = ptr->link;
	}
	return 0;
}

int main()
{
	// Allocate memory for header
	header = (SYMTAB *)malloc(sizeof(SYMTAB));
	if(header == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}

	// Initialize the header SYMTAB
	strcpy(header->sym, "\0");
	strcpy(header->add, "\0");
	header->errflag = 0;
	header->link = NULL;

	FILE *input, *output;
	input = fopen("source_4.txt","r");
	output = fopen("intermediate.txt","w");
	char line[64], string[200], opcode[10], operand[10];
	char Address[4] = "0000";
	int StartAdd, locctr, s;

	if(input == NULL)
	{
		printf("Error opening input file\n");
		return 1;
	}
	else
	{
		printf("Program:\n");
		while(fgets(line, sizeof(line), input))
		{
			char str1[10] = "\0", str2[10] = "\0", str3[10] = "\0";
			sscanf(line, " %s %s %s", str1, str2, str3);

			// Process the strings
			//printf("Line read: %s %s %s\n", str1, str2, str3);
		}

		fclose(input);
		fclose(output);
	}

	// Free allocated memory
	free(header);

	return 0;
}
