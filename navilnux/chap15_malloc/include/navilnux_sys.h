/*
 * =====================================================================================
 *
 *       Filename:  navilnux_sys.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/02/2017 12:56:49 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _NAVIL_SYS
#define _NAVIL_SYS

#include <syscalltbl.h>

void syscall_init(void);

int sys_mysyscall(int, int, int);
int sys_mysyscall4(int, int, int, int);
int sys_itcsend(int, int);
int sys_itcget(int, int*);
int sys_semp(int);
int sys_semv(int);
int sys_mutexwait(int);
int sys_mutexrelease(int);
void* sys_malloc(int);
int sys_free(void*);
extern void sys_scheduler(void);
#endif
