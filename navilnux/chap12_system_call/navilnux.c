#include <navilnux.h>

extern Navil_mem_mng memmng;
extern Navil_task_mng taskmng;

Navil_free_task* navilnux_current;
Navil_free_task* navilnux_next;
Navil_free_task dummyTCB;
int navilnux_current_index;

void scheduler(void)
{
	navilnux_current_index++;
	navilnux_current_index %= (taskmng.max_task_id + 1);

	navilnux_next = &taskmng.free_task_pool[navilnux_current_index];
	navilnux_current = navilnux_next;
}
int
sched_init(void)
{
	if(taskmng.max_task_id < 0){
		return -1;
	}
	navilnux_current = &dummyTCB;
	navilnux_next = &taskmng.free_task_pool[0];
	navilnux_current_index = -1;
	return 0;
}


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
		scheduler();
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

void navilnux_init(void)
{
	mem_init();
	task_init();
	syscall_init();

	os_timer_init();
	os_timer_start();
}

int main(void)
{
	navilnux_init();
	navilnux_user();

	if(sched_init() < 0){
		printf("Kernel Panic~!\n");
		return -1;
	}


	int i;
	for(i=0; i<= taskmng.max_task_id; i++){
		printf("TCB: TASK%d - init PC(%p)\tinit SP(%p)\n",
				i+1,
				taskmng.free_task_pool[i].context_pc,
				taskmng.free_task_pool[i].context_sp);
	}
	printf("REAL function TASK1 : %p\n", user_task_1);
	printf("REAL function TASK2 : %p\n", user_task_2);
	printf("REAL function TASK3 : %p\n", user_task_3);
	
	irq_enable();

	while(1){
		msleep(1000);
	}
	return 0;
}
