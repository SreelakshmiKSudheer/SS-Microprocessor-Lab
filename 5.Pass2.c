#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//extern int pglen;
typedef struct 
{
    char op[6];
    char val[2];
} OPTAB;

typedef struct SYMTAB 
{
    char sym[6];
    char add[4];
    char errflag[1];
} SYMTAB;

OPTAB optab[3] = {{"LDA", "00"}, {"MUL", "20"}, {"STA", "0C"}};
SYMTAB symtab[4];

void is_op(char opc[], char ob[])
{
    for (int i = 0; i < 3; i++) 
    {
        if (strcmp(opc, optab[i].op) == 0) 
        {
            strcpy(ob, optab[i].val);
            return; // Stop loop once found
        }
    }
    ob[0] = '\0'; // If no match, ensure ob is empty
}

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
    
    int n = strlen(rev);
    for (i = 0; i < n; i++) 
    {
        hex[i] = rev[n - i - 1];
    }
}

void is_sym(SYMTAB symtab[], char operand[], char address[])
{
    address[0] = '\0'; // Initialize address to an empty string
    
    for (int i = 0; i < 4; i++) 
    {
        if (strcmp(symtab[i].sym, operand) == 0) 
        {
            strcpy(address, symtab[i].add);
            return; // Stop loop once found
        }
    }
}

int main()
{
    FILE *intermediate, *output, *code, *symt;
    
    intermediate = fopen("intermediate.txt", "r");
    symt = fopen("symtab4.txt", "r");
    output = fopen("output.txt", "w");
    code = fopen("code.txt", "w");

    if (!intermediate || !symt || !output || !code)
    {
        printf("Error opening one of the files.\n");
        return 1;
    }

    char line[64], add[10], label[10], opcode[10], operand[10];
    char str1[10], str2[10], str3[10], str4[10];
    char hex[6] = "000000", ob[2], trec[60] = {0}, Address[4] = "0000";
    int StartAdd, locctr, length, tloc = 0;

    int i = 0;
    while (fgets(line, sizeof(line), symt))
    {
        sscanf(line, " %s %s %s", symtab[i].sym, symtab[i].add, symtab[i].errflag);
        i++;
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

        str1[0] = '\0';
        str2[0] = '\0';
        str3[0] = '\0';
        str4[0] = '\0';
        
        sscanf(line, " %s %s %s %s", str1, str2, str3, str4);

        if (str4[0] == '\0' && str3[0] == '\0' && str2[0] == '\0') 
        {
            strcpy(opcode, str1);
        } 
        else if (str4[0] == '\0' && str3[0] == '\0') 
        {
            strcpy(opcode, str1);
            strcpy(operand, str2);
        } 
        else if (str4[0] == '\0') 
        {
            strcpy(add, str1);
            strcpy(opcode, str2);
            strcpy(operand, str3);
        } 
        else 
        {
            strcpy(add, str1);
            strcpy(label, str2);
            strcpy(opcode, str3);
            strcpy(operand, str4);
        }

        if (strcmp(opcode, "START") == 0) 
        {
            fprintf(output, "%s\n", line);
            dtoh(19, hex);
            fprintf(code, "HPGM1 00%s %s\n", operand, hex);
            fgets(line, sizeof(line), intermediate);
            strcat(trec, "T00");
            strcat(trec, operand);
            strcat(trec, hex);
        }

        do 
        {
            str1[0] = '\0';
            str2[0] = '\0';
            str3[0] = '\0';
            str4[0] = '\0';
            
            sscanf(line, " %s %s %s %s", str1, str2, str3, str4);

            if (strcmp(".", str1) == 0)
                continue;

            if (str4[0] == '\0' && str3[0] == '\0' && str2[0] == '\0') 
            {
                strcpy(opcode, str1);
            } 
            else if (str4[0] == '\0' && str3[0] == '\0') 
            {
                strcpy(opcode, str1);
                strcpy(operand, str2);
                strcpy(add, "\t");
                strcpy(label, "\t");
            } 
            else if (str4[0] == '\0') 
            {
                strcpy(add, str1);
                strcpy(opcode, str2);
                strcpy(operand, str3);
                strcpy(label, "\t");
            } 
            else 
            {
                strcpy(add, str1);
                strcpy(label, str2);
                strcpy(opcode, str3);
                strcpy(operand, str4);
            }

            ob[0] = '\0'; // Initialize ob to avoid issues
            is_op(opcode, ob);

            if (ob[0] != '\0') 
            {
                if (operand[0] != '\0') 
                {
                    is_sym(symtab, operand, Address);

                    if (Address[0] != '\0') 
                    {
                        if (strlen(trec) + strlen(ob) + strlen(Address) < sizeof(trec)) 
                        {
                            strcat(trec, ob);
                            strcat(trec, Address);
                        } 
                        else 
                        {
                            printf("Error: trec buffer overflow\n");
                        }
                    }
                }
            } 
            else if (strcmp(opcode, "BYTE") == 0) 
            {
                dtoh(strlen(operand), hex);
                strcat(trec, hex);
            }
        } while (fgets(line, sizeof(line), intermediate));
    }

    fclose(intermediate);
    fclose(symt);
    fclose(output);
    fclose(code);

    return 0;
}
