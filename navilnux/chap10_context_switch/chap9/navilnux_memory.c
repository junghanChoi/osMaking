/*
 * =====================================================================================
 *
 *       Filename:  navilnux_memory.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/26/2017 11:20:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <navilnux.h>

Navil_mem_mng memmng;

#define STARTUSRSTACKADDR	0xA0400000	// 4M
#define USRSTACKSIZE		0x00100000	// 1M

void mem_init(void)
{
	int i;
	unsigned int pt = STARTUSRSTACKADDR;

	Navil_free_mem* temp = NULL;
	for(i=0; i< MAXMEMBLK ; i++){
		temp = &memmng.free_mem_pool[i];
		temp->block_start_addr = pt;
		temp->block_end_addr = pt + USRSTACKSIZE - 4; // -4 is for flag.
		temp->is_used = 0;
		pt += USRSTACKSIZE;
	}
	temp = NULL;
	memmng.init = mem_init;
	memmng.alloc = mem_alloc;
}

unsigned int mem_alloc(void)
{	
	int i;
	for(i=0; i<MAXMEMBLK; i++)
	{
		if(memmng.free_mem_pool[i].is_used == 0){
			memmng.free_mem_pool[i].is_used = 1;
			return memmng.free_mem_pool[i].block_end_addr;
			// not start but end. becuz it grows descending.
			// end is bigger one. start is smaller than 1M.
		}
	}
	return 0;	
}
