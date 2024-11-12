#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct symt
{
    char sym[10];
    char address[10];
    int errflag;
    struct symt *link;
}symt;

typedef struct opt
{
    char op[10];
    char val[4];
}opt;

opt optab[3] = {{"LDA","00"},{"MUL","20"},{"STA","0C"}}; // Example opcode table

char* isOp(char op[])
{
    int i;

    for(i = 0; i<3; i++)
    {
        if(strcmp(op,optab[i].op) == 0)
            return optab[i].val;
    }
    return "\0";
}

symt* isSymbol(symt *header, char sym[])
{
    symt *ptr = header->link;

    while(ptr != NULL)
    {
        if(strcmp(ptr->sym,sym) == 0)
            return ptr;
        ptr = ptr->link;
    }
    return NULL;
}

int main()
{
    symt *symtab = (symt *)malloc(sizeof(symt));
    symtab->link = NULL;

    FILE *input, *output, *symboltable, *totallength, *objpro;

    char line[100], label[10], opcode[10], operand[10],address[10];
    char str1[10],str2[10],str3[10],str4[10];
    char hex[10];
    int locctr, start, totlen, inuse;
    symt *ptr;

    input = fopen("intermediate.txt","r");
    output = fopen("output.txt","w");
    objpro = fopen("objectProgram.txt","w");
    symboltable = fopen("symboltable.txt","r");
    totallength = fopen("totallength.txt","r");

    // input file open error handling
    if(input == NULL || symboltable == NULL || totallength == NULL)
    {
        printf("Error opening input file\n");
        return 1;
    }
    else
    {
        if(output == NULL || objpro == NULL)
        {
            printf("Error opening output file\n");
            return 1;
        }

        fgets(line,sizeof(line),input);
        printf("intermediate file:\n");
        printf("%s",line);
        
        // initializing
        {                                       
            str1[0] = '\0';
            str2[0] = '\0';
            str3[0] = '\0';
            str4[0] = '\0';
            label[0] = '\0';
            opcode[0] = '\0';
            operand[0] = '\0';
            address[0] = '\0';
        }
        
        sscanf(line," %s %s %s %s",str1,str2,str3,str4);

        if(str4[0] != '\0')
        {
            strcpy(address,str1);
            strcpy(label,str2);
            strcpy(opcode,str3);
            strcpy(operand,str4);
        }
        else
        {
            if(str3[0] != '\0')
            {
                strcpy(address,str1);
                strcpy(label,"   ");
                strcpy(opcode,str2);
                strcpy(operand,str3);
            }
            else
            {
                if(str2[0] != '\0')
                {
                    strcpy(address,"   ");
                    strcpy(label,"   ");
                    strcpy(opcode,str1);
                    strcpy(operand,str2);
                }
            }
        }

        fclose(input);
        fclose(output);
        fclose(symboltable);
        fclose(objpro);
        fclose(totallength);

        return 0;
    }

}