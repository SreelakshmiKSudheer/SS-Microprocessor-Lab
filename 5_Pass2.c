#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//extern int pglen;
typedef struct 
{
    char op[6];
    char val[3];
} OPTAB;

typedef struct SYMTAB 
{
    char sym[6];
    char add[4];
    char errflag[1];
} SYMTAB;

OPTAB optab[3] = {{"LDA", "00"}, {"MUL", "20"}, {"STA", "0C"}};
SYMTAB symtab[10];

int power(int num, int p) 
{
    int ret = 1;
    if (p == 0)
        return ret;
    
    for (int i = 0; i < p; i++)
        ret *= 16;
    
    return ret;
}

char dechex(int dec) 
{
    if (dec >= 0 && dec <= 9)
        return '0' + dec;
    if (dec >= 10 && dec <= 15)
        return 'A' + (dec - 10);
    
    return '\0';
}

void dtoh(int dec, char hex[]) 
{
    int i = 0;
    char rev[6] = "000000";
    
    while (dec) 
    {
        rev[i++] = dechex(dec % 16);
        dec /= 16;
    }
    //printf("%s\n",rev);
    int n = strlen(rev);
    for (i = 0; i < n; i++) 
    {
        hex[i] = rev[n - i - 1];
    }
    hex[i] = '\0';
}

char* getObjectCode(char opcode[]) {
    // Loop through the optab to find the matching opcode
    for (int i = 0; i < 3; i++) {
        if (strcmp(optab[i].op, opcode) == 0) {
            return optab[i].val;  // Return the corresponding object code
        }
    }
    return NULL;  // Return NULL if opcode is not found
}

char* getAddress(char operand[]) {
    // Loop through the optab to find the matching opcode
    int i;
    for (i = 0; i < 3; i++) {
        if (strcmp(symtab[i].sym, operand) == 0) {
            return symtab[i].add;  
        }
    }
    strcpy(symtab[i].sym,operand);
    strcpy(symtab[i].add,"0000");
    return NULL;  
}




int main()
{
    FILE *intermediate, *output, *code, *symt, *totlen;
    
    intermediate = fopen("intermediate.txt", "r");
    symt = fopen("symtab4.txt", "r");
    totlen = fopen("totlen.txt", "r");
    output = fopen("output.txt", "w");
    code = fopen("code.txt", "w");

    if (!intermediate || !symt || !output || !code)
    {
        printf("Error opening one of the files.\n");
        return 1;
    }
    char line[64], add[10], label[10], opcode[10], operand[10];
    char str1[10], str2[10], str3[10], str4[10];
    char hex[6] = "000000", *ob, trec[60] = {0}, *Address,hrec[25],len[3],text[40],obj[7];
    int word,length;

    text[0] = '\0';
    int i = 0;
    while(fgets(line, sizeof(line), symt))
    {
        //printf("%s",line);
        sscanf(line, " %s %s", str1,str2);
        //printf("%s %s %s\n", str1,str2,str3);  
        strcpy(symtab[i].sym,str1);
        strcpy(symtab[i].add,str2);
        // strcpy(symtab[i].errflag,str3);
        //
        //printf("%-6s %-6s\n", symtab[i].sym,symtab[i].add);
        i++;
    }

    if(totlen == NULL)
    {
        printf("Error opening totlen file\n");
        return 1;   
    }
    else
    {
        fgets(line, sizeof(line), totlen);
        len[0] = line[0];
        len[1] = line[1];
        length = atoi(len);
    }
    
    if (intermediate == NULL) 
    {
        printf("Error opening intermediate file\n");
        return 1;
    } 
    else
    {
        printf("Program:\n");
        fgets(line, sizeof(line), intermediate);
        //fscanf(intermediate," %[^\n]",line);
        str1[0] = '\0';
        str2[0] = '\0';
        str3[0] = '\0';
        str4[0] = '\0';
        
        sscanf(line, " %s %s %s %s", str1, str2, str3, str4);

        if(str4[0] != '\0')
        {
            strcpy(add, str1);
            strcpy(label, str2);
            strcpy(opcode, str3);
            strcpy(operand, str4);
        }
        else
        {
            if(str3[0] != '\0')
            {
                strcpy(add, str1);
                strcpy(opcode, str2);
                strcpy(operand, str3);
                strcpy(label, "     ");
            }
            else
            {
                if(str2[0] != '\0')
                {
                    strcpy(opcode, str1);
                    strcpy(operand, str2);
                    strcpy(label, "     ");
                    strcpy(add, "    ");
                }
            }
        }
        //printf("%-6s %-6s %-6s %-6s", add, label, opcode, operand);

        if (strcmp(opcode, "START") == 0) 
        {
            fprintf(code, "%s", line);
            strcpy(hrec,"");
            strcat(hrec,"H");
            strcat(hrec,add);
            strcat(hrec,"00");
            strcat(hrec,operand);
            dtoh(length,hex);
            strcat(hrec,hex);
            strcat(hrec,"\0");
            //printf("%s",hrec);
            fprintf(output, "%s\n", hrec);
            strcpy(trec,"T");
            strcat(trec,"00");
            strcat(trec,operand);
            fprintf(output, "%s", trec);
            fgets(line, sizeof(line), intermediate);
            strcpy(hex,"");
        }

        do 
        {
            str1[0] = '\0';
            str2[0] = '\0';
            str3[0] = '\0';
            str4[0] = '\0';
            
            
            sscanf(line, " %s %s %s %s", str1, str2, str3, str4);
            //printf("%s\n",line);
            //printf(" %s %s %s %s\n", str1, str2, str3, str4);

            if (strcmp(".", str1) == 0)
                continue;
            
            if(str4[0] != '\0')
            {
                strcpy(add, str1);
                strcpy(label, str2);
                strcpy(opcode, str3);
                strcpy(operand, str4);
            }
            else
            {
                if(str3[0] != '\0')
                {
                    strcpy(add, str1);
                    strcpy(opcode, str2);
                    strcpy(operand, str3);
                    strcpy(label, "     ");
                }
                else
                {
                    if(str2[0] != '\0')
                    {
                        strcpy(opcode, str1);
                        strcpy(operand, str2);
                        strcpy(label, "     ");
                        strcpy(add, "    ");
                    }
                }
            }
        
            if(strcmp(opcode,"END") != 0)
            {
                fprintf(code, "%-6s %-6s %-6s %-6s ", add, label, opcode, operand);
                ob = getObjectCode(opcode);
                if(ob != NULL)
                {
                    Address = getAddress(operand);
                    if(Address != NULL)
                    {
                        obj[0] = '\0';
                        strcat(obj,ob);
                        strcat(obj,Address);
                        //("%s\n",obj);
                        fprintf(code, "%-6s",obj);
                        strcat(text,obj);
                        strcpy(obj, "");
                    }
                }
                else
                {
                    if(strcmp(opcode,"WORD") == 0)
                    {
                        word = atoi(operand);
                        
                        dtoh(word,hex);
                        //printf("%s\n",hex);
                        strcpy(obj,hex);
                        strcat(text,obj);
                        fprintf(code, "%-6s",obj);
                        strcpy(obj, "");
                        strcpy(hex,"");

                    }
                    else if(strcmp(opcode,"BYTE") == 0)
                    {
                        if(operand[0] == 'X')
                        {
                            strcpy(obj,operand);
                        }
                        if(operand[0] == 'C')
                        {
                            int obji = 0;
                            for(int i = 2; operand[i] != '\''; i++)
                            {
                                word = operand[i] - 0;
                                dtoh(word,hex);
                                obj[obji++] = hex[4];
                                obj[obji++] = hex[5];
                                strcpy(hex,"");

                            }
                        }
                        strcat(text,obj);
                        fprintf(code, "%-6s",obj);
                        strcpy(obj, "");
                    }
                }
                fprintf(code,"\n");
                
                

            }
            else
            {
                fprintf(code, "%s", line);
            }


        } while(fgets(line, sizeof(line), intermediate));  
    
                length = strlen(text)/2;
                printf("%d\n",length);
                dtoh(length,hex);
                
                fprintf(output,"%c%c",hex[4],hex[5]);
                fprintf(output,"%s",text);
                fprintf(output,"\nE00%s",operand);

    }
}
