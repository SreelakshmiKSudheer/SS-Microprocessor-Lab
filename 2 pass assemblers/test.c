#include <stdio.h>
#include <string.h>

int main()
{
    char str[20] = "Hello";
    // char *ptr = &str[6];
    // char *ptr1 = &str[11];

    // strcpy(ptr,"Maya");
    printf("%s\n",str);
    printf("%c\n",str[strlen(str)-1]);

}