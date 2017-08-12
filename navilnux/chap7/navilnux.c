#include <navilnux.h>

void swiHandler(unsigned int syscallnum)
{
	printf("System call %d\n", syscallnum);
}

void irqHandler(void)
{
	// IRQ -> ICIP 27th bit 1
	if ( (ICIP &(1<<27)) != 0){
		// get intrrupt 1 sec later. 
		OSMR1 = OSCR + 3686400;
		printf("Timer Interrupt!!!\n");
		// clear irq.
		OSSR = OSSR_M1;
	}
}

//using 1 timer. 27th bit
void os_timer_init(void)
{
	// DIM = 1
	ICCR = 0x01;

	// Match reg = 1 --> 27 ISR activate.
	ICMR |= (1<<27);
	// using '~' . set ICLR 0 to use IRQ not FIQ
	ICLR &= ~(1<<27);

	// init timer
	OSCR = 0;
	// cpu cycle is 3.6864MHz
	OSMR1 = OSCR + 3686400;

}
void os_timer_start(void)
{
	// 1 os timer interrupt activate.
	OIER |= (1<<1);
	// bit clear. 
	OSSR = OSSR_M1;
}
	// cpsr 6th bit is FIQ, 7is IRQ. set 0 to activate.
void irq_enable(void)
{
	// 0100 0000 | 0001 0011
	// set 7th bit 0 and set SVC mode
	__asm__("msr	cpsr_c,#0x40|0x13");
}
void irq_disable(void)
{
	// 1100 0000 | 0001 0011
	// set 7th bit 1 and set SVC mode
	__asm__("msr	cpsr_c,#0xc0|0x13");
}

int main(void)
{
	int a = 1;
	int b = 2;
	int c = 0;

	c = a + b;

	os_timer_init();
	os_timer_start();

	irq_enable();
	
	
	while(1){
		printf("Kernel stack a(%p), b(%p), c(%p)\n", &a, &b, &c);
		__asm__("swi 77");
		msleep(1000);
	}
	return 0;
}
