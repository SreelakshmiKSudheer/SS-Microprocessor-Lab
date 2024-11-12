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

symt* insertSymbol(symt *header,char sym[],char address[])
{
    symt *New = (symt *)malloc(size_of(symt));
    strcpy(New->sym,sym); 
    strcpy(New->address,address);
    New->link = NULL;

    symt *ptr = header;
    while(ptr->link != NULL)
    {
        ptr = ptr->link;
    }
    ptr->link = New;
}

int isSymbol(symt *header, char sym[])
{
    symt *ptr = header->link;

    while(ptr != NULL)
    {
        if(strcmp(ptr->sym,sym) == 0)
            return 1;
        ptr = ptr->link;
    }
    return 0;
}

int main()
{
    symt* symtab = (symt *)malloc(size_of(symt));
    symtab->link = NULL;
}