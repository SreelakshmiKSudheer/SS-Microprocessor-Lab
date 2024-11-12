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

void isOp(char op[],char obj[])
{
    int i;

    for(i = 0; i<3; i++)
    {
        if(strcmp(op,optab[i].op) == 0)
        {
            strcpy(obj,optab[i].val);
            return ;
        }
    }
    strcpy(obj,"\0");
    return ;
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
    symt *ptr = symtab;

    FILE *input, *output, *symboltable, *totallength, *objpro;

    char line[100], label[10], opcode[10], operand[10],address[10];
    char str1[10],str2[10],str3[10],str4[10];
    char hex[10],hrec[20],trec[61],erec[20],text[100],obj[10],object[10];
    int locctr, start, totlen, inuse,tlen;
    

    input = fopen("intermediate.txt","r");
    output = fopen("output.txt","w");
    objpro = fopen("objectProgram.txt","w");
    symboltable = fopen("symboltable.txt","r");
    totallength = fopen("totallength.txt","r");

    // fetch symbol table of the program
    if(symboltable == NULL)
    {
        printf("Error fetching symbol table\n");
        return 1;
    }
    else
    {
        while(fgets(line,sizeof(line),symboltable) != NULL)
        {
            sscanf(line," %s %s %s",str1, str2,str3);
            symt *New = (symt*)malloc(sizeof(symt));
            strcpy(New->sym,str1);
            strcpy(New->address,str2);
            New->errflag = atoi(str3);
            ptr->link = New;
            New->link = NULL;
            ptr = New;
        }       
    }

    // fetch total length of the program
    if(totallength == NULL)
    {
        printf("Error fetching total length\n");
        return 1;
    }
    else
    {
        fgets(line,sizeof(line),totallength);
        sscanf(line," %s",str1);

        totlen = atoi(str1);      
    }
    
    // input file open error handling
    if(input == NULL)
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

        if(strcmp(opcode,"START") == 0)
        {
            // setup header record
            start = strtol(operand,NULL,16);
            strcpy(hrec,"H");
            strcat(hrec,label);
            sprintf(hex,"%06X",start);
            strcat(hrec,hex);

            // text record
            strcpy(trec,"T");
            strcat(trec,hex);

            sprintf(hex,"%06X",totlen);
            strcat(hrec,hex);

            // header record is now complete - move to text
            strcat(text,hrec);
            strcat(text,"\n");


            //text record length spec
            tlen = strlen(trec);
            strcat(trec,"00");

            fprintf(output,"%s",line);
            // read next input line
            fgets(line,sizeof(line),input);
        }

        do
        {
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


            if(strcmp(str1,".") == 0)           // comment line
                continue;
            
            if(strcmp(opcode,"END") != 0)
            {
                fprintf(output, "%-6s %-6s %-6s %-6s ", address, label, opcode, operand);
                isOp(opcode,obj);
                // if valid opcode
                if(strcmp(obj,"\0") != 0)
                {
                    strcpy(object,obj);
                    // if symbol
                    if(operand[0] != '\0')
                    {
                        symt *sptr = isSymbol(symtab,operand);
                        if (sptr != NULL)
                        {
                            strcat(object,sptr->address);
                        }
                        else
                        {
                            printf("invalid symbol!!");
                        }
                    }
                    else
                    {
                        strcpy(object,"0000");
                    }
                    fprintf(output, "%-6s",object);
                    strcat(trec,object);   
                }
                else if(strcmp(opcode,"WORD") == 0)
                {
                    inuse = atoi(operand);
                    sprintf(hex,"%06x",inuse);
                    fprintf(output, "%-6s",hex);
                    strcat(trec,hex); 
                }
                else if(strcmp(opcode,"BYTE") == 0)
                {

                    int i;
                    if (operand[0] == 'X') // Hexadecimal constant
                    {  
                        strcpy(object,"");
                        strncat(object, operand + 2, strlen(operand) - 3);
                        fprintf(output, "%-6s",object);
                        strcat(trec,object);
                    } 
                    else if (operand[0] == 'C') // Character constant
                    {  
                        for (i = 2; i < strlen(operand) - 1; i++) 
                        {
                            sprintf(hex, "%02X", operand[i]);
                            strncat(object, operand + 2, strlen(operand) - 3);
                            fprintf(output, "%-6s",object);
                            strcat(trec,object);
                        }
                    }
                }
                fprintf(output,"\n");
            }
            else
            {
                fprintf(output, "%s", line);
                strcpy(erec,"E");
                sprintf(hex,"%06X",start);
                strcat(erec,hex);

            }
        }while(fgets(line,sizeof(line),input) != NULL);

        totlen = (strlen(trec)- 9)/2 ;
        //printf("%d\n",totlen);
        sprintf(hex,"%02X",totlen);

        trec[tlen] = hex[0];
        trec[tlen+1] = hex[1];

        strcat(text,trec);
        strcat(text,"\n");       
        strcat(text,erec);
        fprintf(objpro,text);

        fclose(input);
        fclose(output);
        fclose(symboltable);
        fclose(objpro);
        fclose(totallength);

        return 0;
    }

}