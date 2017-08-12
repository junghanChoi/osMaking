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
extern Navil_msg_mng	msgmng;

void user_task_1(void)
{
	int a, b, c;
	a=1;
	b=2;
	c=a+b;
	while(1){
		printf("TASK1 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		msleep(1000);
	}
}
void user_task_2(void)
{
	int a, b, c;

	a=1;
	b=2;
	c=a+b;
	int send_data = 342;
	while(1){
		printf("TASK2 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		printf("ITC Count is %d\n", a);
		if ( a == 3){
			navilnux_itc_send(2, send_data++);
			a = 1;
			printf("ITC Send!!!\n");
		}
		a++;
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
		c = navilnux_itc_get(2);
		printf("TASK3 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		printf("ITC get !!! ----> %d\n", c);
		msleep(1000);
	}

}
void user_task_4(void)
{
	int a, b, c;

	a=1;
	b=2;
	c = a+b;
	while(1){
		navilnux_sem_p(5);
		printf("TASK4 entered critical section SEMAPHORE\n");
		printf("TASK4 - a: %p\tb:%p\tc:%p\n", &a, &b, &c);
		msleep(2000);
		navilnux_sem_v(5);
		printf("TASK4 out critical section SEMAPHORE\n");
		msleep(4000);
	}

}
void user_task_5(void)
{
	int a, b, c;

	a=1;
	b=2;
	c = a+b;
	while(1){
		navilnux_sem_p(5);
		printf("TASK5 entered critical section SEMAPHORE\n");
		printf("TASK5 - a: %p\tb:%p\tc:%p\n", &a, &b, &c);
		msleep(2000);
		navilnux_sem_v(5);
		printf("TASK5 out critical section SEMAPHORE\n");
		msleep(4000);
	}

}
void user_task_6(void)
{
	int a, b, c;

	a=1;
	b=2;
	c = a+b;
	while(1){
		navilnux_sem_p(5);
		printf("TASK6 entered critical section SEMAPHORE\n");
		printf("TASK6 - a: %p\tb:%p\tc:%p\n", &a, &b, &c);
		msleep(2000);
		navilnux_sem_v(5);
		printf("TASK6 out critical section SEMAPHORE\n");
		msleep(4000);
	}

}
void user_task_7(void)
{
	int a, b, c;

	a=1;
	b=2;
	c = a+b;
	while(1){
		navilnux_mutex_wait(3);
		printf("TASK7 entered critical section MUTEX\n");
		printf("TASK7 - a: %p\tb:%p\tc:%p\n", &a, &b, &c);
		msleep(2000);
		navilnux_mutex_release(3);
		printf("TASK7 out critical section MUTEX\n");
		msleep(4000);
	}

}
void user_task_8(void)
{
	int a, b, c;

	a=1;
	b=2;
	c = a+b;
	while(1){
		navilnux_mutex_wait(3);
		printf("TASK8 entered critical section MUTEX\n");
		printf("TASK8 - a: %p\tb:%p\tc:%p\n", &a, &b, &c);
		msleep(2000);
		navilnux_mutex_release(3);
		printf("TASK8 out critical section MUTEX\n");
		msleep(4000);
	}

}
void navilnux_user(void)
{
	taskmng.create(user_task_1);
	taskmng.create(user_task_2);
	taskmng.create(user_task_3);
	taskmng.create(user_task_4);
	taskmng.create(user_task_5);
	taskmng.create(user_task_6);
	taskmng.create(user_task_7);
	taskmng.create(user_task_8);

	msgmng.sem_init(5, 3);
}
