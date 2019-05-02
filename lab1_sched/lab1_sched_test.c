/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32131960, 32131841
*	    Student name : 김성현, 김준환
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
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

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
int choice=10;
struct process* pros;
while(choice != 5)
{
	pros = makeprocess();
	printf("실행시킬 스케쥴링을 선택하세요\n");
	printf("(1. FIFO  2. RR  3. MLFQ  4. Lottery  5. End)");
	scanf("%d", &choice);
	switch(choice)
	{
		case 1:FIFO(pros);break;
		case 2:RR(pros);break;
		case 3:MLFQ(pros);break;
		case 4:Lot();break;
	}
}
free(pros);
	return 0;
}

