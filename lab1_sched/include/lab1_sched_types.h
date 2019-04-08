/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
 */
struct process;
struct process* makeprocess();
void MakeArray(char array[]);
void FIFO(struct process* pros);
void RR(struct process* pros);
void MLFQ(struct process* pros);
void Lot();
#endif /* LAB1_HEADER_H*/



