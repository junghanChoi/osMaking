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

	a=1;
	b=2;
	c=a+b;

	printf("TASK1 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
}
void user_task_2(void)
{
	int a, b, c;

	a=1;
	b=2;
	c=a+b;

	printf("TASK2 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
}
void user_task_3(void)
{
	int a, b, c;

	a=1;
	b=2;
	c=a+b;

	printf("TASK3 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
}

void navilnux_user(void)
{
	taskmng.create(user_task_1);
	taskmng.create(user_task_2);
	taskmng.create(user_task_3);
}
