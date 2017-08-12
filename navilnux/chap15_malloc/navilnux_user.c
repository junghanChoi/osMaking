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
	int* ptr_a, *ptr_b, *ptr_c, *ptr_d;

	a=1;
	b=2;
	c=a+b;

	ptr_a = (int*)navilnux_malloc(2048);
	printf("Dynamic mem alloc 2K - %p\n", ptr_a);
	ptr_b = (int*)navilnux_malloc(3072);
	printf("Dynamic mem alloc 3K - %p\n", ptr_b);

	navilnux_free(ptr_b);

	ptr_c = (int*)navilnux_malloc(4096);
	printf("Dynamic mem alloc 4K - %p\n", ptr_c);
	ptr_d = (int*)navilnux_malloc(1024);
	printf("Dynamic mem alloc 1K - %p\n", ptr_d);
	while(1){
		printf("TASK1 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		navilnux_sleep(1);
	}
}
void navilnux_user(void)
{
	taskmng.create(user_task_1);
}
