/*
 * =====================================================================================
 *
 *       Filename:  navilnux_task.h
 *
 *    Description:  Task Manager
 *
 *        Version:  1.0
 *        Created:  07/26/2017 12:13:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _NAVIL_TASK
#define _NAVIL_TASK

#define MAXTASKNUM	40
#define CONTEXTNUM	13

// it's okay to declare context[17], but for readerbility, seperate them.
// same register structure. 
typedef struct _navil_free_task{
	unsigned int context_spsr;
	unsigned int context[CONTEXTNUM]; // each reg is 32bit size. 
	unsigned int context_sp;
	unsigned int context_lr;
	unsigned int context_pc;

	unsigned int sleep_end_tick;
} Navil_free_task;

typedef struct _navil_task_mng{
	Navil_free_task free_task_pool[MAXTASKNUM];
	int max_task_id;

	void (*init)(void);
	int (*create)(void (*startFunc)(void));
}Navil_task_mng;

void task_init(void);
int task_create(void(*startFunc)(void));

#endif
