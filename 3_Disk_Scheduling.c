#include <stdio.h>
#include <math.h>

void sort(int *r, int n)
{
    int i,j;                    // loop variables
    int temp;

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n-i-1; j++)
        {
            if(r[j] > r[j+1])
            {
                temp = r[j];
                r[j] = r[j+1];
                r[j+1] = temp;

            }
        }
    }
}

void fcfs(int r[],int n,int head)
{
    int i,seek_sum = 0;

    printf("\n\nFCFS Disk Scheduling\n");
    printf("Disk Traversal Order:\n");
    for(i = 0; i < n; i++)
    {
        printf("%d ",r[i]);
        seek_sum += abs(head - r[i]);
        head = r[i];
    }
    printf("\nTotal seek time: %d",seek_sum);
}

void scan(int r[],int n,int head,int limit)
{
    int i,seek_sum = 0,index = 0, direct;
    sort(r,n);

    printf("\n\nSCAN\n");
    printf("***Direction of traversal\n1. Increasing Order\n2. Decreasing order\n***");
    printf("Enter the direction of traversal: ");
    scanf("%d",&direct);
    printf("Accessing order of data: \n");
    
    if(direct == 1)
    {
        seek_sum += abs(limit - head);
        seek_sum += abs(limit - r[0]);

        for(i = 0; i < n; i++)
        {
            if(r[i] < head)
                continue;
            index++;
            printf("%d ",r[i]);
            
        }
        index = n - index;

        for(i = index-1 ; i >= 0; i--)
        {
            printf("%d ",r[i]);
        }
    }
    else if(direct == 2)
    {
        s
    }


    printf("\nTotal seek time: %d",seek_sum);
    printf("\nAvg seek time: %.2f\n\n",(float)seek_sum/n);
}

void cscan(int r[],int n,int head,int limit)
{
    int i,seek_sum = 0,index = 0;
    sort(r,n);

    printf("\n\nC-SCAN\n");
    printf("Accessing order of data: \n");
    
    seek_sum += abs(limit - head);
    seek_sum += limit;

    for(i = 0; i < n; i++)
    {
        if(r[i] < head)
            continue;
        index++;
        printf("%d ",r[i]);
        
    }
    index = n - index;
    seek_sum += r[index - 1];

    for(i = 0; i < index; i++)
    {
        printf("%d ",r[i]);
    }

    printf("\nTotal seek time: %d",seek_sum);
    printf("\nAvg seek time: %.2f\n\n",(float)seek_sum/n);
}

int main()
{
    int limit, N, head,i,choice;
    printf("Enter the maximum number of cylinders: ");
    scanf("%d",&limit);
    printf("\nEnter the number of disk queue elements: ");
    scanf("%d",&N);

    const int n = N;
    int r[n];

    printf("\nEnter the disk queue elements: ");
    for(i = 0; i < n; i++)
        scanf("%d",&r[i]);

    printf("\nEnter the disk head position: ");
    scanf("%d",&head);

    do
    {
        printf("----- MENU -----\n");
        printf("1. FCFS\n2.SCAN\n3. C-SCAN\n4. EXIT\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch (choice)
        {
            case 1 : fcfs(r,n,head);
                    break;
            case 2 : scan(r,n,head,limit);
                    break;
            case 3 : cscan(r,n,head,limit);
                    break;
            default : break;
                    break;
            
        }

    } while (choice > 0 && choice < 4);
    
}