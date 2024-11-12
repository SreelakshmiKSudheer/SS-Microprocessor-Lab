#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// linked list of operands
typedef struct filler
{
    char addr[5];
    struct filler *link;
}filler;

// symbols
typedef struct symt
{
    char sym[10];
    int addr;
    int errflag;
    filler *fill;
    struct symt *link;
}symt;

// opcodes
typedef struct opt
{
    char op[10];
    char val[5];
}opt;

opt optab[3] = {{"LDA","00"},{"MUL","20"},{"STA","0C"}}; // Example opcode table

// initialize a symbol
symt* getSymt(char sym[])
{
    symt *New = (symt*)malloc(sizeof(symt));
    if (New != NULL)
    {
        strcpy(New->sym, sym);
        New->link = NULL;
        New->errflag = 0;
        New->fill = NULL;
    }
    return New;
}

// insert a symbol to symtab with known address
void insertSymbol(symt *header, char sym[], int addr)
{
    symt *New = getSymt(sym);
    symt *ptr;
    if (New != NULL)
    {
        New->addr = addr;
        New->fill = NULL;
        ptr = header;
        while (ptr->link != NULL)
        {
            ptr = ptr->link;
        }
        ptr->link = New;
    }
}

// insert a symbol to symtab with unknown address
void insertSymbolUnknown(symt *header, char sym[], int locctr)
{
    char addr[6];
    symt *New = getSymt(sym);
    symt *ptr;
    filler *fptr;

    if (New != NULL)
    {
        New->addr = -99999; // Unknown address
        New->fill = (filler *)malloc(sizeof(filler));
        fptr = (filler *)malloc(sizeof(filler));
        sprintf(addr, "%04X", locctr + 1);
        strcpy(fptr->addr, addr);
        fptr->link = New->fill->link;
        New->fill->link = fptr;
        ptr = header;
        while (ptr->link != NULL)
        {
            ptr = ptr->link;
        }
        ptr->link = New;
    }
}

// print symtab
void prints(symt *header)
{
    symt *ptr = header->link;
    printf("Symbol Table\n");
    while (ptr != NULL)
    {
        printf("%s\t %04X\t %d\n", ptr->sym, ptr->addr, ptr->errflag);
        ptr = ptr->link;
    }
}

// free symtab
void Free(symt *header)
{
    symt *ptr = header;
    symt *ptr1;

    while (ptr->link != NULL)
    {
        ptr1 = ptr->link;
        if (ptr->fill != NULL)
        {
            filler *fptr = ptr->fill;
            while (fptr != NULL)
            {
                filler *tmp = fptr;
                fptr = fptr->link;
                free(tmp);
            }
        }
        free(ptr);
        ptr = ptr1;
    }
}

// search for a symbol in the symtab
symt* searchSymbol(symt *header, char sym[])
{
    symt *ptr = header->link;
    while (ptr != NULL)
    {
        if (strcmp(ptr->sym, sym) == 0)
        {
            return ptr;
        }
        ptr = ptr->link;
    }
    return NULL;
}

// check for valid opcode
char* isOp(char op[])
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (strcmp(optab[i].op, op) == 0)
        {
            return optab[i].val;
        }
    }
    return "\0";
}

int main()
{
    symt *symtab = (symt *)malloc(sizeof(symt));
    symt *ptr;

    FILE *input, *obprogram, *prog;
    char line[100], opcode[10], operand[10], label[10], temp[60], str1[10], str2[10], str3[10];
    char hrec[20], trec[61], address[10], temp2[200], obj[3], hex[10];
    char mrec[20] = "T0000000000", *mloc = &mrec[3], *mlen = &mrec[7], *mmod = &mrec[9];
    int locctr, startAddr, addr;

    input = fopen("source6.txt", "r");
    obprogram = fopen("onePassObjProgram.txt", "w");
    prog = fopen("listing.txt", "w");

    if (input == NULL)
    {
        printf("Error opening input file!\n");
        return 1;
    }
    else
    {
        if (obprogram == NULL)
        {
            printf("Error creating the output object program!\n");
            return 1;
        }
        else
        {
            if (prog == NULL)
            {
                printf("Error creating the output program!\n");
                return 1;
            }
            else
            {
                str1[0] = '\0';
                str2[0] = '\0';
                str3[0] = '\0';

                fgets(line, sizeof(line), input);
                sscanf(line, "%s %s %s", str1, str2, str3);

                if (str3[0] != '\0')
                {
                    strcpy(label, str1);
                    strcpy(opcode, str2);
                    strcpy(operand, str3);
                }
                else if (str2[0] != '\0')
                {
                    strcpy(opcode, str1);
                    strcpy(operand, str2);
                }
                else if (str1[0] != '\0')
                {
                    strcpy(opcode, str1);
                }

                if (strcmp(opcode, "START") == 0)
                {
                    startAddr = strtol(operand, NULL, 16);
                    locctr = startAddr;
                }
                else
                {
                    locctr = 0;
                }
                strcat(hrec, "H");
                sprintf(address, "%06X", locctr);
                strcat(hrec, address);
                fgets(line, sizeof(line), input);
                strcpy(trec, "T");
                strcat(trec, address);

                do
                {
                    str1[0] = '\0';
                    str2[0] = '\0';
                    str3[0] = '\0';
                    label[0] = '\0';
                    opcode[0] = '\0';
                    operand[0] = '\0';

                    sscanf(line, "%s %s %s", str1, str2, str3);

                    if (strcmp(str1, ".") == 0)
                        continue;

                    if (str3[0] != '\0')
                    {
                        strcpy(label, str1);
                        strcpy(opcode, str2);
                        strcpy(operand, str3);
                    }
                    else if (str2[0] != '\0')
                    {
                        strcpy(opcode, str1);
                        strcpy(operand, str2);
                    }
                    else if (str1[0] != '\0')
                    {
                        strcpy(opcode, str1);
                    }

                    if (strcmp(opcode, "END") != 0)
                    {
                        if (label[0] != '\0')
                        {
                            if ((ptr = searchSymbol(symtab, label)) != NULL)
                            {
                                if (ptr->addr == -99999)
                                {
                                    ptr->addr = locctr;
                                    sprintf(address, "%04X", locctr);
                                    filler *fptr = ptr->fill->link;
                                    filler *fptr2 = fptr;
                                    strcat(temp, trec);
                                    strcpy(trec, "T");
                                    while (fptr != NULL)
                                    {
                                        strcpy(mloc, fptr->addr);
                                        strcpy(mlen, "02");
                                        strcpy(mmod, address);
                                        strcat(temp, "\n");
                                        strcat(temp, mrec);
                                        fptr2 = fptr;
                                        fptr = fptr->link;
                                        free(fptr2);
                                    }
                                    ptr->fill->link = NULL;
                                }
                                else
                                {
                                    printf("Duplicate Entry!\n");
                                    ptr->errflag = 1;
                                }
                            }
                            else
                            {
                                insertSymbol(symtab, label, locctr);
                            }
                        }
                    }
                } while (strcmp(opcode, "END") != 0);
                printf("Program successfully assembled.\n");

                fclose(input);
                fclose(obprogram);
                fclose(prog);
            }
        }
    }

    Free(symtab);
}
