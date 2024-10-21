#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int num;
    struct node *link;
}node;

void pint(node *ptr)
{
    int i;
    for(i = 0; i < 5; i++)
    {
        ptr = ptr->link;
        if(i == 3)
        {
            break;
        }
        
    }
}
int main()
{
    node *header = (node *)malloc(sizeof(node));
    header->link = NULL;
    node *New = (node *)malloc(sizeof(node));
    node *ptr;
    ptr = header;
    int i;

    for(i = 0; i < 5; i++)
    {
        New->num = i;
        //printf("%d\n",New->num);
        New->link = NULL;
        ptr->link = New;
        ptr = ptr->link;
        //printf("%d\n",ptr->num);
    }

    ptr = header->link;

    pint(ptr);
    printf("%d\n",ptr->num);

}