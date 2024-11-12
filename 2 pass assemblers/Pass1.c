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

int isOp(char op[])
{
    int i;

    for(i = 0; i<3; i++)
    {
        if(strcmp(op,optab[i].op) == 0)
            return 1;
    }
    return 0;
}

void insertSymbol(symt *header,char sym[],char address[])
{
    symt *New = (symt *)malloc(sizeof(symt));
    strcpy(New->sym,sym); 
    strcpy(New->address,address);
    New->errflag = 0;
    New->link = NULL;

    symt *ptr = header;
    while(ptr->link != NULL)
    {
        ptr = ptr->link;
    }
    ptr->link = New;
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
    symt* symtab = (symt *)malloc(sizeof(symt));
    symtab->link = NULL;

    FILE *input, *output, *symboltable, *totallength;

    char line[100], label[10], opcode[10], operand[10];
    char str1[10],str2[10],str3[10];
    char Address[10];
    int locctr, start, totlen, inuse;
    symt *ptr;

    input = fopen("source.txt","r");

    // input file open error handling
    if(input == NULL)
    {
        printf("Error opening input file\n");
        return 1;
    }
    else
    {
        output = fopen("intermediate.txt","w");
        symboltable = fopen("symboltable.txt","w");
        totallength = fopen("totallength.txt","w");

        fgets(line,sizeof(line),input);
        printf("Input program:\n");
        printf("%s",line);
        
        // initializing
        {                                       
            str1[0] = '\0';
            str2[0] = '\0';
            str3[0] = '\0';
            label[0] = '\0';
            opcode[0] = '\0';
            operand[0] = '\0';
        }
        
        sscanf(line," %s %s %s",str1,str2,str3);

        if(str3[0] != '\0')
        {
            strcpy(label,str1);
            strcpy(opcode,str2);
            strcpy(operand,str3);
        }
        else if(str2[0] != '\0')
        {
            strcpy(label,"\t");
            strcpy(opcode,str1);
            strcpy(operand,str2);
        }
        else if(str1[0] != '\0')
        {
            strcpy(opcode,str1);
            strcpy(label,"\t");
            strcpy(operand,"\t");
        }

        if(strcmp(opcode,"START") == 0)
        {
            start = strtol(operand, NULL, 16);
            locctr = start;
        }
        else
        {
            locctr = 0;
        }
        sprintf(Address,"%04X",locctr);
        fprintf(output, "%-6s %-6s %-6s %-6s\n", Address, label, opcode, operand);
        fgets(line,sizeof(line),input);
        
        do
        {
            printf("%s",line);
            // initializing
            {                                       
                str1[0] = '\0';
                str2[0] = '\0';
                str3[0] = '\0';
                label[0] = '\0';
                opcode[0] = '\0';
                operand[0] = '\0';
            }
        
            sscanf(line," %s %s %s",str1,str2,str3);

            if(strcmp(str1,".") == 0)
                continue;

            if(str3[0] != '\0')
            {
                strcpy(label,str1);
                strcpy(opcode,str2);
                strcpy(operand,str3);

                if((ptr = isSymbol(symtab,label)) != NULL)
                {
                    printf("Duplicate Entry\n");
                    ptr->errflag = 1;
                    continue;
                }
                else
                {
                    sprintf(Address,"%04X",locctr);
                    insertSymbol(symtab,label,Address);
                }
            }
            else if(str2[0] != '\0')
            {
                strcpy(label,"\t");
                strcpy(opcode,str1);
                strcpy(operand,str2);
            }
            else if(str1[0] != '\0')
            {
                strcpy(opcode,str1);
                strcpy(label,"\t");
                strcpy(operand,"\t");
            }

            if(strcmp(opcode,"END") == 0)
            {
                printf("hi\n");
                sprintf(Address,"%04X",start);
                fprintf(output, "\t          END    %s\n",Address);
                break;
            }

            sprintf(Address,"%04X",locctr);
            fprintf(output, "%-6s %-6s %-6s %-6s\n", Address, label, opcode, operand);

            if(isOp(opcode))
                locctr += 3;
            else if(strcmp(opcode,"WORD") == 0)
                locctr += 3;
            else if(strcmp(opcode,"RESW") == 0)
                locctr += 3 * atoi(operand);
            else if(strcmp(opcode,"RESB") == 0)
                locctr += atoi(operand);
            else if(strcmp(opcode,"BYTE") == 0)
                locctr += strlen(operand) - 3;        

        }while (fgets(line,sizeof(line),input) != NULL);

        totlen = locctr - start;
        
        printf("\nTotal length of the program: %d",totlen);
        fprintf(totallength,"%d",totlen);

        ptr = symtab->link;
        printf("\nSYMTAB:\n");
        while(ptr != NULL)
        {
            fprintf(symboltable,"%-6s %-6s %-6d\n",ptr->sym,ptr->address,ptr->errflag);
            printf("%-6s %-6s %-6d\n",ptr->sym,ptr->address,ptr->errflag);
            ptr = ptr->link;
        }

        fclose(input);
        fclose(output);
        fclose(symboltable);
        fclose(totallength);
    }


}