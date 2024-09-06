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
}SYMTAB;


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
	int dig,i = 0;
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
SYMTAB symtab[3] = {{"\0","\0",0},{"\0","\0",0},{"\0","\0",0}};




int main()
{
	FILE *input, *output;
	input = fopen("source_4.txt","r");
	output = fopen("intermediate.txt","w");
	char line[64], string[200], opcode[10], operand[10];
	char Address[4] = "0000";
	int StartAdd, locctr,s;


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
			char str1[10] = "\0", str2[10] = "\0", str3[10] = "\0", label[10] = "\0";
			printf("%s",line);
			//strcat(string,line);
			sscanf(line," %s %s %s",str1,str2,str3);


			if(strcmp(str3,"\0"))
			{
				strcpy(opcode, str1);
				strcpy(operand, str2);


			}
			else
			{
				strcpy(label, str1);
                                strcpy(opcode, str2);
                                strcpy(operand, str3);
			}
			if(strcmp(opcode,"START") == 0)
			{
				StartAdd = htod(operand);
				locctr = StartAdd;
				fprintf(output,"\t%s\n",line);
				fgets(line,sizeof(line),input);
			}
			else
				locctr = 0;


			while(strcmp(opcode,"END") != 0)
			{
				if(strcmp(label,"\0") != 0)
				{
					str3[0] = '\0';
					int found = 0, i = 0;
					


					if(strcmp(str3,"\0"))
	                        	{
        	                        	strcpy(opcode, str1);
	                	                strcpy(operand, str2);
						strcpy(label, "\t");
	                        	}
        	                	else
                	        	{
                        	        	strcpy(label, str1);
                                		strcpy(opcode, str2);
                                		strcpy(operand, str3);
                        		}


					for(i = 0; i < 3; i++)
					{
					
						if(strcmp(symtab[i].sym,label) == 0)
						{
							printf("Error! Symbol repeated\n");
							symtab[i].errflag = 1;
							found = 1;
						}
					}
					if(!found)
					{
						strcpy(symtab[s].sym,label);
						dtoh(locctr,Address);
						strcpy(symtab[s++].add,Address);
						
					}


				}


				int i, found = 0;
				for( i = 0; i < 3; i++)
				{
					
					if(strcmp(opcode, optab[i].op) == 0)
					{
						locctr += 3;
						found = 1;
					}
				}
				
				if(!found)
				{
					if(strcmp(opcode,"WORD") == 0)
						locctr += 3;
					else if(strcmp(opcode,"RESW") == 0)
                                                locctr += 3 * htod(operand);
					else if(strcmp(opcode,"RESB") == 0)
                                                locctr += htod(operand);
					else if(strcmp(opcode,"BYTE") == 0)
                                                locctr += strlen(operand);
				}
			dtoh(locctr,Address);
			fprintf(output,"%s\t%s\n",Address, line);
			}


		}
		//printf("%s",string); 
		fclose(input);
		fclose(output);
	}
}
