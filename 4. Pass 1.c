#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    char op[6];
    char val[2];
}
OPTAB;

OPTAB optab = {{"LDA","00"},{"MUL","20"},{"STA","0C"}}}

int main()
{
	FILE *input, *output;
	input = fopen("source_4.txt","r");
	output = fopen("intermediate.txt","w");
	char line[64], string[200], str1[10], str2[10], str3[10] = {'\0'}, opcode[10], label[10], operand[10];
	int hex = 0x10C;


	printf("%d",hex);
	if(input == NULL)
	{
		printf("Error opening\n");
		return 1;
	}
	else
	{
		printf("Program:\n");
		while(fgets(line,sizeof(line),input))
		{
			printf("%s",line);
			//strcat(string,line);
			sscanf(line," %s %s %s",str1,str2,str3);


			if(strcmp(str3,"\0"))
			{
				strcpy(opcode, str1);
				strcpy(operand, str2);
				printf("%s %s\n",opcode,operand);
			}
			else
			{
				strcpy(label, str1);
                                strcpy(opcode, str2);
                                strcpy(operand, str3);
				printf("%s %s %s\n",label,opcode,operand);


			}
			//if(strcmp(opcode,"START") == 0)
		//	{
		//	}
		}
		//printf("%s",string); 
		fclose(input);
		fclose(output);
	}
}


