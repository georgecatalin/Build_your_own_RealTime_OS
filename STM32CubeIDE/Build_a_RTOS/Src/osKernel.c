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
#define BUS_FREQUENCY (16000000)

#define  CTRL_CLKSOURCE (1UL<<2)
#define CTRL_TICK_ENBL_INTERRUPT (1UL<<1)
#define CTRL_ENABLE (1UL<<0)
#define CTRL_COUNTFLAG (1UL<<16)

#define SYSTICK_RESET 0

uint32_t MILLIS_PRESCALER;


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


void osKernelStackInit(int thread_id)
{
	thread_control_blocks[thread_id].stckPtr = &TCB_STACK[thread_id][STACK_SIZE-16]; // Stack Pointer


	/* set the bit number 21 (T-bit) in PSR to 1 so that to operate in thumb mode */
	TCB_STACK[thread_id][STACK_SIZE-1] = (1UL<<24);

	/* Set dummy content in the remaining stack, except of PC where we will store the address of the function to run ->> this is not compulsory */
	/*
	TCB_STACK[thread_id][STACK_SIZE-3] = 0xAAAAAAAA; //R14 e.g LR
	TCB_STACK[thread_id][STACK_SIZE-4] = 0xAAAAAAAA; //R12
	TCB_STACK[thread_id][STACK_SIZE-5] = 0xAAAAAAAA; //R3
	TCB_STACK[thread_id][STACK_SIZE-6] = 0xAAAAAAAA;  //R2
	TCB_STACK[thread_id][STACK_SIZE-7] = 0xAAAAAAAA; //R1
	TCB_STACK[thread_id][STACK_SIZE-8] = 0xAAAAAAAA; //R0


	TCB_STACK[thread_id][STACK_SIZE-9] = 0xAAAAAAAA; //R11;
	TCB_STACK[thread_id][STACK_SIZE-10] = 0xAAAAAAAA; //R10
	TCB_STACK[thread_id][STACK_SIZE-11] = 0xAAAAAAAA; //R9
	TCB_STACK[thread_id][STACK_SIZE-12] = 0xAAAAAAAA; //R8
	TCB_STACK[thread_id][STACK_SIZE-13] = 0xAAAAAAAA; //R7
	TCB_STACK[thread_id][STACK_SIZE-14] = 0xAAAAAAAA; //R6
	TCB_STACK[thread_id][STACK_SIZE-15] = 0xAAAAAAAA; //R5
	TCB_STACK[thread_id][STACK_SIZE-16] = 0xAAAAAAAA; //R4
	*/

}


uint8_t osKernelAddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void)) // void (*task0)(void) the memory address of the function executed at task 0, there are 3 tasks in this program
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
	osKernelStackInit(0);

	/* Initialize the program counter of thread 0 to the function to be executed for thread 0*/
	TCB_STACK[0][STACK_SIZE-2] = (int32_t) (task0);

	/* Initialize the stack Pointer of thread 1 */
	osKernelStackInit(1);

	/* Initialize the program counter of thread 1 to the function to be executed for thread 1*/
	TCB_STACK[1][STACK_SIZE-2] = (int32_t) (task1);


	/* Initialize the stack Pointer of thread 2 */
	osKernelStackInit(2);

	/* Initialize the program counter of thread 2 to the function to be executed for thread 2*/
	TCB_STACK[2][STACK_SIZE-2] = (int32_t) (task2);

	/* Start from thread 0 */
	currentPtr = &thread_control_blocks[0];

	/* Enable global interrupts */
	__enable_irq();

	return 1;
}

void osKernelInit()
{
	 MILLIS_PRESCALER = (BUS_FREQUENCY/1000);
}


void osKernelLaunch(uint32_t quanta)
{
	/* reset Systick */
	SysTick->CTRL = SYSTICK_RESET;

	/* Clear Systick current register */
	SysTick -> VAL = 0;

	/* Load quanta */
	SysTick->LOAD = (quanta * MILLIS_PRESCALER) -1;

	/* Set systick priority to low priority */
	NVIC_SetPriority(SysTick_IRQn,15);

	/* Enable systick, select internal clock */
	SysTick ->CTRL = CTRL_CLKSOURCE | CTRL_ENABLE;

	/* Enable systick interrupt */
	SysTick->CTRL |= CTRL_TICK_ENBL_INTERRUPT;

	/* Launch Scheduler */
    osSchedulerLaunch();
}



/*  ********************************************************************************************************************************************************************************************************************
 * Theory:  In simpler terms, the __attribute__((naked)) is a special instruction or directive in the C programming language that tells the compiler to generate a function without any additional code.
 * When you declare a function with the __attribute__((naked)) attribute, it means that you want to take full control of the function's implementation, including the function entry and exit code.
 *  The compiler won't generate the usual setup and cleanup instructions that are automatically added to a function.
 *  This attribute is often used in low-level programming scenarios where you need to write highly optimized and specific assembly code for a particular function.
 *  ***************************************************************************************************************************************************************************
 *  By using __attribute__((naked)), you can directly write assembly code within the function and have complete control over how the function operates.
 *  However, it's important to note that using __attribute__((naked)) requires a good understanding of assembly language and low-level programming concepts.
 *  It is typically used in specific situations where fine-grained control over the function's implementation is necessary, such as when writing interrupt handlers or dealing with hardware directly.
 *  In summary, the __attribute__((naked)) attribute allows you to create a function without any additional compiler-generated code, giving you the ability to manually write assembly instructions or implement low-level operations within the function.
 *  ********************************************************************************************************************************************************************************************************************* */

__attribute__((naked)) void SysTick_Handler(void) // every cycle defined int the timebase , this interrupt is executed
{
	/*SUSPEND CURRENT THREAD*/

	/*Disable global interrupts*/
	__asm("CPSID	I");

	/*Save r4,r5,r6,r7,r8,r9,r10,11*/
	__asm("PUSH  {R4-R11}");

	/*Load address of currentPt into r0*/
	__asm("LDR R0, =currentPtr");

	/*Load r1 from address equals r0, i.e. r1 =currentPt*/
	__asm("LDR R1,[R0]");

	/*Store Cortex-M SP at address equals r1, i.e Save SP into tcb */
	__asm("STR SP,[R1]");

    /*CHOOSE NEXT THREAD*/

    /*Load r1 from a location 4bytes above address r1, i.e r1 = currentPt->next*/
	__asm("LDR R1,[R1,#4]");

	/*Store r1 at address equals r0, i.e currentPt =  r1*/
	__asm("STR	R1,[R0]");

	/*Load Cortex-M SP from address equals r1, i.e SP =  currentPt->stackPt*/
	__asm("LDR SP,[R1]");

	/*Restore r4,r5,r6,r7,r8,r9,r10,11*/
	__asm("POP {R4-R11}");

	/*Enable global interrupts*/
	__asm("CPSIE	I");

	/*Return from exception and restore r0,r1,r2,r3,r12,lr,pc,psr */
	__asm("BX	LR");

}


void osSchedulerLaunch(void)
{
	/*Load address of currentPt into R0*/
	__asm("LDR R0,=currentPtr");

	/*Load r2 from address equals r0,i.e r2 =currentPt*/
	__asm("LDR R2,[r0]");

	/*Load Cortex-M SP from address equals R2,i.e. SP = currentPt->stackPt*/
	__asm("LDR  SP,[R2]");

	/*Restore r4,r5,r6,r7,r8,r9,r10,r11*/
	__asm("POP {R4-R11}");

	/*Restore r12*/
	__asm("POP {R12}");

	/*Restore r0,r1,r2,r3*/
	__asm("POP	{R0-R3}");

	/*Skip LR */
	__asm("ADD  SP,SP,#4");

	/*Create a new start location by popping LR*/
	__asm("POP {LR}");

	/*Skip PSR by adding 4 to SP*/
	__asm("ADD  SP,SP,#4");

	/*Enable global interrupts*/
	__asm("CPSIE	I");

	/*Return from exception*/
	__asm("BX	LR");
}



