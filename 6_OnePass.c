#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char op[6];
    char val[2];
} OPTAB;

typedef struct SYMTAB {
    char sym[6];
    char add[4];
    int def;
    int *list;
    struct SYMTAB *link;
} SYMTAB;

OPTAB optab[3] = {{"LDA", "00"}, {"MUL", "20"}, {"STA", "0C"}};
SYMTAB *header = NULL;

int power(int num, int p) {
    int i, ret = 1;
    if (p == 0)
        return ret;
    for (i = 0; i < p; i++)
        ret *= 16;
    return ret;
}

int hexdec(char hex) {
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'A' && hex <= 'F')
        return 10 + (hex - 'A');
    return -1; // Error case
}

char dechex(int dec) {
    if (dec >= 0 && dec <= 9)
        return '0' + dec;
    if (dec >= 10 && dec <= 15)
        return 'A' + (dec - 10);
    return '\0'; // Error case
}

int htod(char str[]) {
    int i, num = 0;
    int n = strlen(str);
    for (i = 0; i < n; i++) {
        num += hexdec(str[n - i - 1]) * power(16, i);
    }
    return num;
}

void dtoh(int dec, char hex[]) {
    int i = 0;
    char rev[6] = "0000";
    while (dec) {
        rev[i++] = dechex(dec % 16);
        dec /= 16;
    }
    rev[i] = '\0';

    int n = strlen(rev);
    for (i = 0; i < n; i++) {
        hex[i] = rev[n - i - 1];
    }
    hex[i] = '\0';
}


int is_op(char opc[]) {
    for (int i = 0; i < 3; i++) {
        if (strcmp(opc, optab[i].op) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_found(char sym[], SYMTAB *ptr) {
    while (ptr != NULL) {
        if (strcmp(sym, ptr->sym) == 0) {
            return 1;
        }
        ptr = ptr->link;
    }
    return 0;
}

void symNull(char label[],int locctr, SYMTAB *ptr)
{
    
}
void sumNew(char label[], int locctr, SYMTAB *ptr) {
    SYMTAB *New = (SYMTAB *)malloc(sizeof(SYMTAB));
    char address[6];
    strcpy(New->sym, label);
    dtoh(locctr, address);
    strcpy(New->add, address);
    New->def = 1;
    New->list = NULL;
    New->link = header;
    header = New;
}

int main() {
    header = (SYMTAB *)malloc(sizeof(SYMTAB));
    if (header == NULL) 
    {
        printf("Memory allocation failed\n");
        return 1;
    }
    strcpy(header->sym, "\0");
    strcpy(header->add, "\0");
    header->def = 0;
    header->link = NULL;

    FILE *input, *output;
    input = fopen("source_4.txt", "r");
    output = fopen("intermediate.txt", "w");
    char line[64], opcode[10], operand[10], label[10];
    char str1[10], str2[10], str3[10];
    char Address[4] = "0000", start[4] = "0000";
    int StartAdd, locctr, flag = 0;
    SYMTAB *ptr = header;

    if (input == NULL) 
    {
        printf("Error opening input file\n");
        return 1;
    } 
    else 
    {
        printf("Program:\n");
        fgets(line, sizeof(line), input);
        printf("%s",line);
        str1[0] = '\0';
        str2[0] = '\0';
        str3[0] = '\0';
        sscanf(line, " %s %s %s", str1, str2, str3);


        if (str3[0] == '\0') 
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


        if (strcmp(opcode, "START") == 0) 
        {
            StartAdd = htod(operand);
            dtoh(StartAdd,start);
            locctr = StartAdd;
            fprintf(output, "\t      %s", line);
            fgets(line, sizeof(line), input);
        } 
        else 
        {
            locctr = 0;
        }

        do 
        {
            printf("%s",line);
            str1[0] = '\0';
            str2[0] = '\0';
            str3[0] = '\0';
            sscanf(line, " %s %s %s", str1, str2, str3);

            if (strcmp(".", str1) == 0)
                continue;
            if (str3[0] == '\0') 
            {
                strcpy(label, "\t");
                strcpy(opcode, str1);
                strcpy(operand, str2);
            } 
            else 
            {
                strcpy(label, str1);
                strcpy(opcode, str2);
                strcpy(operand, str3);
                ptr = header;
                if(is_found(label,ptr))
                {
                    
                }
                else
                {
                    sumNew(label, locctr, ptr);
                }
            }

            if (strcmp(opcode, "END") == 0) {
                dtoh(StartAdd,Address);
                fprintf(output, "\t      END    %s\n", Address);
                break;
            }

            dtoh(locctr, Address);
			fprintf(output, "%-6s %-6s %-6s %-6s\n", Address, label, opcode, operand);

            if (is_op(opcode))
                locctr += 3;
 
            else if (strcmp(opcode, "WORD") == 0)
                locctr += 3;
            else if (strcmp(opcode, "RESW") == 0)
                locctr += 3 * atoi(operand);
            else if (strcmp(opcode, "RESB") == 0)
                locctr += atoi(operand);
            else if (strcmp(opcode, "BYTE") == 0)
                locctr += strlen(operand) - 3;   

        } while (fgets(line, sizeof(line), input));

        printf("\nTotal length of program: %d\n", locctr - StartAdd);
        fclose(input);
        fclose(output);
        
        FILE *symt, *opt;
        symt = fopen("symtab4.txt", "w");
        opt = fopen("optab4.txt", "w");

        if (symt == NULL) 
        {
            printf("Error opening symtab file\n");
            return 1;
        } 
        else
        {
            printf("\nSymtab:\n");
            ptr = header;
            while(ptr->link != NULL)
            {    
                    printf("%-6s %-6s %-6d\n", ptr->sym, ptr->add, ptr->errflag);
                    fprintf(symt,"%-6s %-6s %-6d\n", ptr->sym, ptr->add, ptr->errflag);
                    ptr = ptr->link;
            }
            fclose(symt);
        }

        if (opt == NULL) 
        {
            printf("Error opening optab file\n");
            return 1;
        } 
        else
        {
            int i;
            for(i = 0; i < 3; i++)
            {
                fprintf(opt,"%-6s %-6s\n",optab[i].op,optab[i].val);
            }
            fclose(opt);
        }
    }
    
    free(header);
    return 0;
}

