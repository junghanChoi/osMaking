/*
 * =====================================================================================
 *
 *       Filename:  navilnux_sys.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/02/2017 12:52:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <navilnux.h>

unsigned int navilnux_syscallvec[SYSCALLNUM];
extern Navil_msg_mng	msgmng;

int sys_mysyscall(int a, int b, int c)
{
	printf("My systemcall - %d, %d, %d\n", a, b, c);
	return 333;
}

int sys_mysyscall4(int a, int b, int c, int d)
{
	printf("My systemcall 4 - %d %d %d %d\n", a, b, c, d);
	return 4444;
}

int sys_itcsend(int itcnum, int data)
{
	return msgmng.itc_send(itcnum, data);
}

int sys_itcget(int itcnum, int* data)
{
	return msgmng.itc_get(itcnum, data);
}
int sys_semp(int semnum){
	return msgmng.sem_p(semnum);
}
int sys_semv(int semnum){
	return msgmng.sem_v(semnum);
}
int sys_mutexwait(int mutexnum)
{
	return msgmng.mutex_wait(mutexnum);
}

int sys_mutexrelease(int mutexnum)
{
	return msgmng.mutex_release(mutexnum);
}
void syscall_init(void)
{
	navilnux_syscallvec[SYS_MYSYSCALL] = (unsigned int)sys_mysyscall;
	navilnux_syscallvec[SYS_MYSYSCALL4] = (unsigned int)sys_mysyscall4;
	navilnux_syscallvec[SYS_ITCSEND] = (unsigned int)sys_itcsend;
	navilnux_syscallvec[SYS_ITCGET] = (unsigned int)sys_itcget;
	navilnux_syscallvec[SYS_SEMP] = (unsigned int)sys_semp;
	navilnux_syscallvec[SYS_SEMV] = (unsigned int)sys_semv;
	navilnux_syscallvec[SYS_MUTEXWAIT] = (unsigned int)sys_mutexwait;
	navilnux_syscallvec[SYS_MUTEXREL] = (unsigned int)sys_mutexrelease;
}
