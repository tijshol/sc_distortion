
.globl irq_handler

irq_handler:
	.func irq_handler

	/* Correct LR_irq; this is a quirk of how the ARM processor calls the
	 * IRQ handler.  */
	sub lr, lr, #4
	/* sub r14, r14, #4 */
    push    {fp, lr}

	stmfd sp!,{r0-r3,r12,r14}
/*
	stmfd sp!,{lr}
	stmfd sp!,{r0-r3}
*/

	B  handle_interrupt

/*
	ldmfd sp!,{r0-r3}
	ldmfd sp!,{r12,r14}
*/

	ldmfd sp!,{r0-r3,r12,pc}

    pop     {fp, pc}
	.endfunc


.globl irq_ena

irq_ena:
	.func irq_ena

	mrs   r1, cpsr
	bic r1,r1, #0x80
	msr  cpsr,r1
	.endfunc


.globl irq_dis

irq_dis:
	.func irq_dis
	mrs   r1, cpsr
	orr r1,r1, #0x80
	msr  cpsr,r1
	.endfunc

