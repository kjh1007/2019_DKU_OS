/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32131960, 32131841
*	    Student name : 김성현, 김준환
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"
#define NUMBER 5
/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */
struct process{
    char name;
    int arr;
    int ser;
};

struct process* makeprocess()
{
	int count = 0;
	srand(time(NULL));
    struct process* pros = malloc(sizeof(struct process) * 5);
	struct process* queue = malloc(sizeof(struct process) * 5);
    for(int i = 0; i < NUMBER; i++)
    {
        pros[i].name = 'A'+i;
        pros[i].arr = rand()%5;
        pros[i].ser = rand()%7 + 1;
    }
    for(int i = 0; i < 5; i++)
    {   
        for(int j = 0; j < NUMBER; j++)
        {
            if(pros[j].arr == i)
			{
				queue[count].name = pros[j].name;
				queue[count].arr = pros[j].arr;
				queue[count].ser = pros[j].ser;
				printf("%c %d %d\n", queue[count].name, queue[count].arr, queue[count].ser);
				count++;
	        }
	    }
	}
	free(pros);
	return queue;
}

void MakeArray(char* array)
{
	char* graph[7];
	for(int i=0;i<7;i++)
		graph[i] = malloc(sizeof(char) * 100);
	char num[3];
	strcpy(graph[0], " |");
	strcpy(graph[1], "-|");
	strcpy(graph[2], "A|");
	strcpy(graph[3], "B|");
	strcpy(graph[4], "C|");
	strcpy(graph[5], "D|");
	strcpy(graph[6], "E|");
	for(int i=0;array[i] != NULL;i++)
	{
		sprintf(num, "%d", i);
		if(i < 10)
		{
			num[1] = num[0];
			num[0] = '0';
		}
		switch(array[i])
		{
			case('A'):
			{
				strcat(graph[0], num);
				strcat(graph[1], "--");
				strcat(graph[2], "ㅁ");
				strcat(graph[3], "  ");
				strcat(graph[4], "  ");
				strcat(graph[5], "  ");
				strcat(graph[6], "  ");
				break;
			}
			case('B'):
			{
				strcat(graph[0], num);
				strcat(graph[1], "--");
				strcat(graph[2], "  ");
				strcat(graph[3], "ㅁ");
				strcat(graph[4], "  ");
				strcat(graph[5], "  ");
				strcat(graph[6], "  ");
				break;
			}
			case('C'):
			{
				strcat(graph[0], num);
				strcat(graph[1], "--");
				strcat(graph[2], "  ");
				strcat(graph[3], "  ");
				strcat(graph[4], "ㅁ");
				strcat(graph[5], "  ");
				strcat(graph[6], "  ");
				break;
			}
			case('D'):
			{
				strcat(graph[0], num);
				strcat(graph[1], "--");
				strcat(graph[2], "  ");
				strcat(graph[3], "  ");
				strcat(graph[4], "  ");
				strcat(graph[5], "ㅁ");
				strcat(graph[6], "  ");
				break;
			}
			case('E'):
			{
				strcat(graph[0], num);
				strcat(graph[1], "--");
				strcat(graph[2], "  ");
				strcat(graph[3], "  ");
				strcat(graph[4], "  ");
				strcat(graph[5], "  ");
				strcat(graph[6], "ㅁ");
				break;
			}
			case(' '):
			{
				strcat(graph[0], num);
				strcat(graph[1], "--");
				strcat(graph[2], "  ");
				strcat(graph[3], "  ");
				strcat(graph[4], "  ");
				strcat(graph[5], "  ");
				strcat(graph[6], "  ");
				break;
			}
		}
				strcat(graph[0], "|");
				strcat(graph[1], "|");
				strcat(graph[2], "|");
				strcat(graph[3], "|");
				strcat(graph[4], "|");
				strcat(graph[5], "|");
				strcat(graph[6], "|");
	}
	for(int i=0;i<7;i++)
	{
		printf("%s\n", graph[i]);
	}
}

void FIFO(struct process* pros)
{
	for(int i=0;i<5;i++)
		printf("%c  %d  %d\n", pros[i].name, pros[i].arr, pros[i].ser);
	int now = 0;
	char arr[40];
	for(int i=0;i<5;i++)
	{
		if(pros[i].arr > now)
		{
			for(;now < pros[i].arr;now++)
			{
				arr[now] = ' ';
			}
		}
		for(int j=0;j<pros[i].ser;j++)
		{
			arr[now] = pros[i].name;
			now++;
		}
	}
	printf("%s\n", arr);
	MakeArray(arr);
}

void RR(struct process* pros)
{
	int n=0, now=0, did = 0;
	char arr[40];
	printf("slice를 얼마로 하시겠습니까? : ");
	scanf("%d", &n);
	while(pros[0].ser+pros[1].ser+pros[2].ser+pros[3].ser+pros[4].ser)
	{
		did=0;
		for(int i=0;i<5;i++)
		{
			if(pros[i].arr <= now && pros[i].ser != 0)
			{
				if(pros[i].ser > n)
				{
					for(int j=0;j<n;j++)
					{
						arr[now] = pros[i].name;
						pros[i].ser--;
						now++;
					}
				}
				else
				{
					for(int j=0;j<pros[i].ser;j++)
					{
						arr[now] = pros[i].name;
						now++;
					}
					pros[i].ser = 0;
				}
				did = 1;
			}
		}
		if(did != 1)
		{
			arr[now] = ' ';
			now++;
		}
	}

	printf("%s\n", arr);
	MakeArray(arr);
}

void MLFQ(struct process* pros)
{
	int prior[5] = {-1, -1, -1, -1, -1};
	int n=0, now = 0, did = 0;
	char arr[40];
	while(pros[0].ser+pros[1].ser+pros[2].ser+pros[3].ser+pros[4].ser)
	{
		did=0;
		for(int k=0;k<3;k++)
		{
			for(int i=0;i<5;i++)
			{
			if(prior[i] == -1 && pros[i].arr <= now) prior[i] = 0;
				if(prior[i] == k)
				{
					if(pros[i].ser != 0)
					{
						if(prior[i] == 0) n = 1;
						else if(prior[i] == 1) n = 2;
						else if(prior[i] == 2) n = 4;
						else n = 0;
						if(pros[i].ser > n)
						{
							for(int j=0;j<n;j++)
							{
								arr[now] = pros[i].name;
								pros[i].ser--;
								now++;
								if(prior[i] != 2) prior[i]++;
							}
						}
						else
						{
							for(int j=0;j<pros[i].ser;j++)
							{
								arr[now] = pros[i].name;
								now++;
							}
							pros[i].ser = 0;
						}
						did = 1;
					}
				}
			}
		}
		if(did != 1)
		{
			arr[now] = ' ';
			now++;
		}
	}
	printf("%s\n", arr);
	MakeArray(arr);
}

void Lot()
{
	int sim=0;
	printf("시뮬레이션 범위를 지정해주세요 :");
	scanf("%d", &sim);
	char *arr = malloc(sizeof(char) * sim);
	int prop[5] = {0};
	double ratio[5] = {0.0};
	double rat[5] = {0.0};
	int count[5] = {0};
	int sum=0;
	for(int i=0;i<5;i++)
	{
		prop[i] = rand()%10+1;
		sum += prop[i];
	}
	for(int i=0;i<5;i++)
	{
		ratio[i] =(double)prop[i] / sum;
		rat[i] = ratio[i];
	}
	for(int i=1;i<5;i++)
		ratio[i] = ratio[i] + ratio[i-1];
	for(int i=0;i<sim;i++)
	{
		double seed = (double)(rand()%10000) / 10000;
		if(seed < ratio[0])
		{
			arr[i] = 'A'; count[0]++;
		}
		else if(seed < ratio[1])
		{
			arr[i] = 'B'; count[1]++;
		}
		else if(seed < ratio[2])
		{
			arr[i] = 'C'; count[2]++;
		}
		else if(seed < ratio[3])
		{
			arr[i] = 'D'; count[3]++;
		}
		else if(seed < ratio[4])
		{
			arr[i] = 'E'; count[4]++;
		}
	}
	for(int i=0;i<sim;i++)
	{
		printf("%c", arr[i]);
		if(i % 50 == 0)
			printf("\n");
		else if(i % 5 == 0)
			printf(" ");
	}
	printf("\n총 시행횟수 : %d", sim);
	printf("\n예상시행비율\n");
	printf("A : %.2lf, B : %.2lf, C : %.2lf, D : %.2lf, E : %.2lf\n",
			rat[0], rat[1], rat[2], rat[3], rat[4]);
	printf("\n각 시행횟수\n");
	printf("A : %d, B : %d, C : %d, D : %d, E : %d\n",
			count[0], count[1], count[2], count[3], count[4]);
	printf("\n실제시행비율\n");
	printf("A : %.2lf, B : %.2lf, C : %.2lf, D : %.2lf, E : %.2lf\n",
			(double)count[0] / sim, (double)count[1] / sim, (double)count[2] / sim, (double)count[3] / sim, (double)count[4] / sim);
}
