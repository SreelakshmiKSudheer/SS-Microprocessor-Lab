#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void sort(int r[], int n)
{
	int temp,i,j;


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


void fcfs(int r[], int n, int head)
{
	int i, seek_sum = 0;


	printf("\n\nFCFS Disk Scheduling\n");
	printf("Disk traversal order\n");
	for(i = 0; i < n; i++)
	{
		printf("%d ", r[i]);
		seek_sum += abs(head - r[i]);
		head = r[i];
	}
	printf("\nSeek Sum = %d",seek_sum);
}




void scan(int r[], int n, int head, int limit)
{
	sort(r,n);


	int i, seek_sum = 0, direction, index = 0;


        printf("\n\nSCAN Disk Scheduling\n");
	printf("\n***** Direction of Traversal *****\n");
	printf("1. Increasing order\n");
	printf("2. Decreasing order\n");
	printf("Enter the direction of Traversal: ");
	scanf("%d",&direction);


	
	if(direction == 1)
	{
		for(i = 0; i < n; i++)
		{
			if(r[i] < head)
				continue;
			index++;
			printf("%d ", r[i]);
		}
		index = n-index-1;


		for(i = index; i >= 0; i--)
			printf("%d ",r[i]);
		printf("\n\n");


		seek_sum = (limit-1-head) + (limit-1-r[0]);
	}
	else
	{
                for(i = n-1; i >= 0; i--)
                { 
                        if(r[i] > head)
                                continue;
                        index++;
                        printf("%d ", r[i]);
                }


                for(i = index; i < n; i++)
                        printf("%d ",r[i]);
                printf("\n\n");


                seek_sum = head + r[n-1];


	}


	printf("Total Seek time = %d",seek_sum);
}


void cscan(int r[], int n, int head, int limit)
{
        sort(r,n);


        int i, seek_sum = 0, direction, index = 0;


        printf("\n\nC-SCAN Disk Scheduling\n");
        printf("\n***** Direction of Traversal *****\n");
        printf("1. Increasing order\n");
        printf("2. Decreasing order\n");
        printf("Enter the direction of Traversal: ");
        scanf("%d",&direction);




        if(direction == 1)
        {
                for(i = 0; i < n; i++)
                {
                        if(r[i] < head)
                                continue;
                        index++;
                        printf("%d ", r[i]);
                }
                index = n-index-1;


                for(i = 0; i <= index; i++)
                        printf("%d ",r[i]);
                printf("\n\n");


                seek_sum = (limit-1-head) + (limit-1) + r[index];
        }
        else
        {
                for(i = n-1; i >= 0; i--)
                { 
                        if(r[i] > head)
                                continue;
                        index++;
                        printf("%d ", r[i]);
                }


                for(i = n-1; i >= index; i--)
                        printf("%d ",r[i]);
                printf("\n\n");


                seek_sum = head + (limit-1) + (limit-1-r[index]);


        }


        printf("Total Seek time = %d",seek_sum);
}


int main()
{
	int head, limit, choice, N, i;


	printf("Enter the maximum no. of cylinders: ");
	scanf("%d", &limit);


	printf("\nEnter the no. of disk queue elements: ");
        scanf("%d", &N);


	const int n = N;
	int r[n];


	printf("\nEnter the disk queue elements: ");
	for(i = 0; i < n; i++)
        	scanf("%d", &r[i]);


	printf("\nEnter the current head position: ");
        scanf("%d", &head);


	do
	{
		printf("\n\n----- MENU -----\n");
		printf("1. FCFS\n2. SCAN\n3. C-SCAN\n4. EXIT\n");
		printf("Enter your choice: ");
        	scanf("%d", &choice);


		switch(choice)
		{
			case 1: fcfs(r,n,head);
				break;
			case 2: scan(r,n,head,limit);
				break;
			case 3: cscan(r,n,head,limit);
				break;
			default : break;
		}
	}while(choice > 0 && choice < 4);


}
