/*
 * =====================================================================================
 *
 *       Filename:  navilnux_user.c
 *
 *    Description:  G
 *
 *        Version:  1.0
 *        Created:  07/26/2017 01:08:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <navilnux.h>

extern Navil_task_mng	taskmng;

void user_task_1(void)
{
	int a, b, c;
	int count = 0;
	a=1;
	b=2;
	c=a+b;
	while(1){
		printf("TASK1 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		printf("Count : %d\n", ++count);
		msleep(1000);
	}
}
void user_task_2(void)
{
	int a, b, c;

	a=1;
	b=2;
	c=a+b;

	while(1){
		printf("TASK2 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		c = mysyscall4(3,4,5,6);
		printf("Syscall4 return value is %d\n", c);
		msleep(1000);
	}
}
void user_task_3(void)
{
	int a, b, c;

	a=1;
	b=2;
	c = a+b;
	while(1){
		printf("TASK3 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		c = mysyscall(a,b,c);
		printf("Syscall return value is %d\n", c);
		msleep(1000);
	}

}

void navilnux_user(void)
{
	taskmng.create(user_task_1);
	taskmng.create(user_task_2);
	taskmng.create(user_task_3);
}
