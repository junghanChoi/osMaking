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

#define STARTDYMEMADDR		0xA2C00000	//44M
#define ENDUSRSTACKADDR		0xA4000000	//64M // isn't this address HEAP area end?

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

	memmng.heap_start = STARTDYMEMADDR;
	memmng.used_end = memmng.heap_start;
	memmng.init = mem_init;
	memmng.alloc = mem_alloc;
	memmng.malloc = mem_malloc;
	memmng.free = mem_free;
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

int mem_free(void *addr)
{
	Dy_mem_header* dy_header;
	unsigned int add_header = (unsigned int)addr - sizeof(Dy_mem_header);
	dy_header = (Dy_mem_header*) add_header;

	if(dy_header->used != 1){
		return -1;
	}
	dy_header->used = 0;

	return 0;
}

void* mem_malloc(int size)
{
	int req_size = size + sizeof(Dy_mem_header);
	unsigned int cur_pos = memmng.heap_start;
	unsigned int ret_addr = 0;

	Dy_mem_header* dy_header;

	while(cur_pos != memmng.used_end){
		dy_header = (Dy_mem_header*) cur_pos;
		if(!dy_header->used){
			if(dy_header->size >= req_size){
				dy_header->used = 1;
				ret_addr = cur_pos;
				break;
			}
		}
		cur_pos += dy_header->size;
	}
	// there was no useable or empty one.
	if(!ret_addr){
		ret_addr = memmng.used_end;
		memmng.used_end += req_size;
		if(memmng.used_end > ENDUSRSTACKADDR){
			return (void*)0;
			//NULL pointer. no empty space.
		}
		dy_header = (Dy_mem_header*) ret_addr;
		dy_header -> used = 1;
		dy_header -> size = req_size;
	}
	ret_addr += sizeof(Dy_mem_header);
	return (void*) ret_addr;
}
