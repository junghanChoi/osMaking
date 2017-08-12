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
	bl	main	   // C ÇÔ¼öÀÇ mainÇÔ¼ö¸¦ È£ÃâÇÑ´Ù. 	
	b	_ram_entry // main ÇÔ¼öÀÇ È£ÃâÀÌ ³¡³µ´Ù¸é Àç È£ÃâÇÑ´Ù. 
	b	navilnux_swiHandler
	b	navilnux_irqHandler

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
	bl	irqHandler
