/*
 * =====================================================================================
 *
 *       Filename:  navilnux_memory.h
 *
 *    Description:  Kernel memory 
 *
 *        Version:  1.0
 *        Created:  07/26/2017 11:08:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _NAVIL_MEM
#define _NAVIL_MEM

#define MAXMEMBLK	40

// each block should be 1M size.
// manage Mem by check start and end memory.
typedef struct _navil_free_mem {
	unsigned int block_start_addr;
	unsigned int block_end_addr;
	int is_used; // flag
} Navil_free_mem;

typedef struct _dy_mem_header{
	int used;
	int size;
}Dy_mem_header;

// linear search.
typedef struct _navil_mem_mng{
	Navil_free_mem free_mem_pool[MAXMEMBLK];

	unsigned int heap_start;
	unsigned int used_end;

	void (*init)(void);
	unsigned int (*alloc)(void);

	void* (*malloc)(int);
	int (*free)(void*);
}Navil_mem_mng;
// init mem_mng struct. assign init function and alloc function.
void mem_init(void);

// alloc a new block. linear search.
// if find a new block, set is_used 1 and return memory
unsigned int mem_alloc(void);

void* mem_malloc(int);
int mem_free(void *);

#endif
