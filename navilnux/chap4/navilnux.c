#include <navilnux.h>

void swiHandler(unsigned int syscallnum)
{
	printf("System call %d\n", syscallnum);
}

void irqHandler(void)
{

}

int main(void)
{
	asm("swi 77");
	return 0;
}
