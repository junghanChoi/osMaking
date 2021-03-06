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
	msr	cpsr_c, #0xc0 | 0x13 //block interrupt
	ldr	sp, =svc_stack	// svc stack. 

	stmfd	sp!, {lr}
	stmfd	sp!, {r1-r14}^
	mrs	r10, spsr	// save program status
	stmfd	sp!, {r10}

	ldr	r10, [lr, #-4]	// swi XXX
	bic	r10, r10, #0xff000000 // extract param
	mov	r11, #4
	mul	r10, r10, r11		// 4 byte * param

	ldr	r11, =navilnux_syscallvec // sys call . 
	add	r11, r11, r10
	ldr	r11, [r11]
	mov	lr, pc
	mov	pc, r11

	ldmfd	sp!, {r1}
	msr	spsr_cxsf, r1
	ldmfd	sp!, {r1-r14}^
	ldmfd	sp!, {pc}^

.global navilnux_irqHandler
navilnux_irqHandler:
	msr	cpsr_c, #0xc0 | 0x12

	ldr	sp, =navilnux_current
	ldr	sp,[sp]

	sub	lr,lr,#4
	add	sp, sp, #4
	stmia	sp!, {r0-r12}^
	stmia	sp!, {sp, lr}^
	stmia	sp!, {lr}

	sub	sp, sp, #68
	mrs	r1, spsr
	stmia	sp!, {r1}

	ldr	sp, =irq_stack

	bl	irqHandler

	ldr	sp, =navilnux_next
	ldr	sp, [sp]

	ldmia	sp!, {r1}
	msr	spsr_cxsf, r1
	ldmia	sp!, {r0-r12}^
	ldmia	sp!, {r13,r14}^
	
	ldmia	sp!, {pc}^
