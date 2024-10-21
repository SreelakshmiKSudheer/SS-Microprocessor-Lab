#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *in;
    char str[64];

    in = fopen("intermediate.txt","r");
    int i = 1;
    while (fgets(str,sizeof(str),in))
    {
        printf("%d %s\n",i,str);
        i++;
    }
    
    
}