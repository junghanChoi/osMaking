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
	bl	swiHandler

.global navilnux_irqHandler
navilnux_irqHandler:
	bl	irqHandler
