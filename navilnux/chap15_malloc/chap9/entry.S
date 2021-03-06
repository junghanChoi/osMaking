/*========================================================================================================================

  화일명 : entry.S
  설  명 : 램 상에 존재하는 ezBoot의 메인 C 함수의 main을 호출하기 위한 루틴이다.
  
  작성자 : (주)제이닷디앤티  유영창
  작성일 : 2001년 10월 8일
  
  비  고 :  

==========================================================================================================================*/
/*
b <addr> -> pc� 옜옜 � 옜 옜� 옜옜.
ldr pc, [pc, #offset] -> 옜� 옜� 옜 옜옜 옜옜� 옜� pc� 옜옜 옜옜.
mov pc, #옜 -> pc� 옜� 옜�.
*/
.globl _ram_entry
_ram_entry:
	bl	kernel_init	   // C 함수의 main함수를 호출한다. 	
	b	_ram_entry // main 함수의 호출이 끝났다면 재 호출한다. 
	b	navilnux_swiHandler
	b	navilnux_irqHandler

#define irq_stack 0xa0380000
#define system_stack 0xa0400000
#define svc_stack	0xa0300000
.global kernel_init
kernel_init:
	//svc 10011 cpsr[4:0]
	msr	cpsr_c,#0xc0|0x13
	ldr	r0,=svc_stack // svc sp
	sub	sp,r0,#4

	msr	cpsr_c,#0xc0|0x12	// IRQ mode
	ldr	r0,=irq_stack
	sub	sp,r0,#4
	// 11111 system cpsr[4:0]
	msr	cpsr_c,#0xc0|0x1f
	ldr	r0,=system_stack
	sub	sp,r0,#4	// 4 is empty space between stack. growing Descending.

	msr	cpsr_c,#0xc0 | 0x13

	bl	main
	b	_ram_entry

.global	navilnux_swiHandler
navilnux_swiHandler:
	stmfd	sp!,{r0-r12,r14}	// STore Multi Full Descending
	mrs	r1,spsr		// Move to Register from Status regster
	stmfd	sp!,{r1}		
	ldr	r10,[lr,#-4]
	bic	r10,r10,#0xff000000
	mov	r0, r10
	bl	swiHandler	//function in navilnux.c 
	ldmfd	sp!,{r1}	//Stack is FIFO. so ldmfd {r1} first. 
	msr	spsr_cxsf,r1	//status pro~ reg. Control eXtension Status Flag regester.
	ldmfd	sp!,{r0-r12,pc}^	//stack pop(ldmfd)
.global navilnux_irqHandler
navilnux_irqHandler:
	sub	lr,lr,#4
	stmfd	sp!,{lr}
	stmfd	sp!,{r0-r14}^
	mrs	r1,spsr
	stmfd	sp!,{r1}
	bl	irqHandler
	ldmfd	sp!,{r1}
	msr	spsr_cxsf,r1
	ldmfd	sp!,{r0-r14}^
	ldmfd	sp!,{pc}^
