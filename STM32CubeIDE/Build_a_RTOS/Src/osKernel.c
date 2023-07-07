/*
 * osKernel.c
 *
 *  Created on: 7 Jul 2023
 *  Author: George Calin
 * 	Target Development Board: STM32 Nucleo F429ZI
 */

#include "osKernel.h"

#define NUM_OF_THREAD 3
#define STACK_SIZE 100 //Blocks of 32bit each = 4 bytes => 400 bytes

struct tcb
{
	int32_t *stckPtr;
	struct tcb *nextPtr;
};

typedef struct tcb tcbTypedef;

tcbTypedef thread_control_blocks[NUM_OF_THREAD];
tcbTypedef *currentPtr;

/* Each thread will have a stacksize of 100 ie. 400 bytes */
int32_t TCB_STACK[NUM_OF_THREAD][STACK_SIZE]; // this is used to fill in the memory stack for each thread


void osKernelInit(int thread_id)
{
	thread_control_blocks[thread_id].stckPtr = &TCB_STACK[thread_id][STACK_SIZE -16]; // Stack Pointer


	/* set the bit number 21 (T-bit) in PSR to 1 so that to operate in thumb mode */
	TCB_STACK[thread_id][STACK_SIZE-1] = (1UL<<21);

	/* Set dummy content in the remaining stack, except of PC where we will store the address of the function to run */
	TCB_STACK[thread_id][STACK_SIZE -3] = 0xAAAAAAAA; //R14 e.g LR
	TCB_STACK[thread_id][STACK_SIZE -4] = 0xAAAAAAAA; //R12
	TCB_STACK[thread_id][STACK_SIZE -5] = 0xAAAAAAAA; //R3
	TCB_STACK[thread_id][STACK_SIZE -6] = 0xAAAAAAAA;  //R2
	TCB_STACK[thread_id][STACK_SIZE -7] = 0xAAAAAAAA; //R1
	TCB_STACK[thread_id][STACK_SIZE -8] = 0xAAAAAAAA; //R0


	TCB_STACK[thread_id][STACK_SIZE -9] = 0xAAAAAAAA; //R11;
	TCB_STACK[thread_id][STACK_SIZE -10] = 0xAAAAAAAA; //R10
	TCB_STACK[thread_id][STACK_SIZE -11] = 0xAAAAAAAA; //R9
	TCB_STACK[thread_id][STACK_SIZE -12] = 0xAAAAAAAA; //R8
	TCB_STACK[thread_id][STACK_SIZE -13] = 0xAAAAAAAA; //R7
	TCB_STACK[thread_id][STACK_SIZE -14] = 0xAAAAAAAA; //R6
	TCB_STACK[thread_id][STACK_SIZE -15] = 0xAAAAAAAA; //R5
	TCB_STACK[thread_id][STACK_SIZE -16] = 0xAAAAAAAA; //R4
}


uint8_t osKernelAddThreads(void (*task0)(void), void (*task1)(void), void (*taks2)(void)) // void (*task0)(void) the memory address of the function executed at task 0, there are 3 tasks in this program
{
	//Disable global interrupts cause we do not want to be interrupted when we are doing what we are doing here
	__disable_irq();

	/* after the thread 0 is executed for its time slice(quanta) , go the execution of thread 1 */
	/* point the next pointer of thread 0 to the thread 1 */
	thread_control_blocks[0].nextPtr = &thread_control_blocks[1];

	/* After thread 1 exhausts its time slice, then go to thread 2 */
	thread_control_blocks[1].nextPtr = &thread_control_blocks[2];

	/* After thread 2 exhausts its time slice(quanta) go to thread 0 */
	thread_control_blocks[2].nextPtr = &thread_control_blocks[0];

	/* Initialize the stack Pointer of thread 0 */
	osKernelInit(0);

	/* Initialize the program counter of thread 0 to the function to be executed for thread 0*/
	TCB_STACK[0][STACK_SIZE -2] = (int32_t) task0;

	/* Initialize the stack Pointer of thread 1 */
	osKernelInit(1);

	/* Initialize the program counter of thread 1 to the function to be executed for thread 1*/
	TCB_STACK[1][STACK_SIZE -2] = (int32_t) task1;


	/* Initialize the stack Pointer of thread 2 */
	osKernelInit(2);

	/* Initialize the program counter of thread 2 to the function to be executed for thread 2*/
	TCB_STACK[2][STACK_SIZE -2] = (int32_t) task2;

	/* Start from thread 0 */
	currentPtr = &thread_control_blocks[0];

	/* Enable global interrupts */
	__enable_irq();

	return 1;
}
