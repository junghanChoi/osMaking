/*
 * =====================================================================================
 *
 *       Filename:  navilnux_lib.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/02/2017 01:03:16 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _NAVIL_LIB
#define _NAVIL_LIB


extern int mysyscall(int, int, int);
extern int mysyscall4(int, int, int,int);
extern int itc_send(int, int);
extern int itc_get(int, int*);
extern void call_scheduler(void);
extern int sem_p(int);
extern int sem_v(int);
extern int mutex_wait(int);
extern int mutex_release(int);
int navilnux_itc_send(int, int);
int navilnux_itc_get(int);
int navilnux_sem_p(int);
int navilnux_sem_v(int);
int navilnux_mutex_wait(int);
int navilnux_mutex_release(int);
void* navilnux_malloc(int);
int navilnux_free(void *);
int navilnux_sleep(int);
#endif
