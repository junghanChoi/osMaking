/*
 * =====================================================================================
 *
 *       Filename:  navilnux_clib.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/03/2017 01:38:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <navilnux.h>
extern unsigned int navilnux_time_tick;
extern Navil_free_task* navilnux_current;


int navilnux_itc_send(int itcnum, int data)
{
	return itc_send(itcnum, data);
}

int navilnux_itc_get(int itcnum)
{
	int ret_value = 0;
	int data = 0;

	while(1){
		ret_value = itc_get(itcnum, &data);
		if( 0 == ret_value ){
			return data;
		}
		else if( -1 == ret_value){
			return ret_value;
		}else{
			// block till profer data comes.
			call_scheduler();
		}

	}
}
int navilnux_sem_p(int semnum)
{
	int ret_value = 0;
	while(1){
		ret_value = sem_p(semnum);
		if(ret_value == 0){
			return 0;
		}else if(ret_value == -1){
			return -1;
		}else{
			call_scheduler();
		}
	}
	return 0;	
}

int navilnux_sem_v(int semnum)
{
	return sem_v(semnum);
}

int navilnux_mutex_wait(int mutexnum)
{
	int ret_value = 0;
	while(1){
		ret_value = mutex_wait(mutexnum);
		if( ret_value == 0){
			return 0;
		}else if(ret_value == -1){
			// error numver error.
			return -1;
		}
		else{
			//received -2. not mine. wait.
			call_scheduler();
		}
	}
}
int navilnux_mutex_release(int mutexnum)
{
	return mutex_release(mutexnum);
}

int navilnux_sleep(int sec)
{
	while(1){
		if(navilnux_current->sleep_end_tick == 0){
			navilnux_current->sleep_end_tick = navilnux_time_tick + sec;
		}else if(navilnux_current->sleep_end_tick <= navilnux_time_tick){
			navilnux_current->sleep_end_tick = 0;
			// return 0;
			break;
		}else{
			call_scheduler();
		}
	}
	return 0;
}

int navilnux_open(int drvnum, int mode)
{
	return open(drvnum, mod);
}

int navilnux_close(int drvnum)
{
	return close(drvnum);
}

int navilnux_read(int drvnum, void* buf, int size)
{
	int ret_value = 0;
	while(1){
		ret_value = read(drvnum, buf, size);
		if(ret_value >= 0){
			return ret_value;
		}else if(ret_value == -1){
			return -1;
		}else{
			call_scheduler();
		}
		
		
	}
}

int navilnux_write(int drvnum, void* buf, int size)
{
	return write(drvnum, buf, size);
}

