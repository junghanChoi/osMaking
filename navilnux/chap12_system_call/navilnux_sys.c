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

void syscall_init(void)
{
	navilnux_syscallvec[SYS_MYSYSCALL] = (unsigned int)sys_mysyscall;
	navilnux_syscallvec[SYS_MYSYSCALL4] = (unsigned int)sys_mysyscall4;

}
