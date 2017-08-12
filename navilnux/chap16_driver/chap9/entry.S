/*========================================================================================================================

  È­ÀÏ¸í : entry.S
  ¼³  ¸í : ·¥ »ó¿¡ Á¸ÀçÇÏ´Â ezBootÀÇ ¸ŞÀÎ C ÇÔ¼öÀÇ mainÀ» È£ÃâÇÏ±â À§ÇÑ ·çÆ¾ÀÌ´Ù.
  
  ÀÛ¼ºÀÚ : (ÁÖ)Á¦ÀÌ´åµğ¾ØÆ¼  À¯¿µÃ¢
  ÀÛ¼ºÀÏ : 2001³â 10¿ù 8ÀÏ
  
  ºñ  °í :  

==========================================================================================================================*/
/*
b <addr> -> pc¿ ¿¿¿¿ ¿ ¿¿ ¿¿¿ ¿¿¿¿.
ldr pc, [pc, #offset] -> ¿¿¿ ¿¿¿ ¿¿ ¿¿¿¿ ¿¿¿¿¿ ¿¿¿ pc¿ ¿¿¿¿ ¿¿¿¿.
mov pc, #¿¿ -> pc¿ ¿¿¿ ¿¿¿.
*/
.globl _ram_entry
_ram_entry:
	bl	kernel_init	   // C ÇÔ¼öÀÇ mainÇÔ¼ö¸¦ È£ÃâÇÑ´Ù. 	
	b	_ram_entry // main ÇÔ¼öÀÇ È£ÃâÀÌ ³¡³µ´Ù¸é Àç È£ÃâÇÑ´Ù. 
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
