/*
 * =====================================================================================
 *
 *       Filename:  navilnux_task.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/26/2017 12:15:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <navilnux.h>

extern Navil_mem_mng memmng;
Navil_task_mng taskmng;

#define STARTUSRCPSR	0x68000050
// 31-28	7-0
// NZCV		IFT(MODE)
// set user mode. IRQ, FIQ, Thumb (Mode)

int task_create(void(*startFunc)(void))
{
	int task_idx = 0;
	unsigned int stack_top = 0;
	taskmng.max_task_id++;
	task_idx = taskmng.max_task_id;
	if(task_idx > MAXTASKNUM){
		return -1;
	}
	stack_top = memmng.alloc();
	if(stack_top == 0){
		return -2;
	}
	taskmng.free_task_pool[task_idx].context_spsr = STARTUSRCPSR;
	taskmng.free_task_pool[task_idx].context_sp = stack_top;
	taskmng.free_task_pool[task_idx].context_pc = (unsigned int)startFunc;

	return task_idx;	
}

void task_init(void)
{
	int i;
	for(i = 0; i< MAXTASKNUM; i++)
	{
		taskmng.free_task_pool[i].context_spsr = 0x00;
		taskmng.free_task_pool[i].context_lr= 0x00;
		taskmng.free_task_pool[i].context_pc= 0x00;
		taskmng.free_task_pool[i].sleep_end_tick = 0;

	}
	taskmng.max_task_id = -1; // is there reason to init -1?
	taskmng.init = task_init;
	taskmng.create = task_create;
}

