
.text
.globl start
.globl spin

start:
	mov	$_sys_stack, %esp
	jmp	stublet

.align 4
	/* Multi-boot header */
	.long	0x1badb002
	.long	0x3
	.long	0xe4524ffb

stublet:
	/* Call startup code written in C */
	push	%ebx /* Multiboot information structure */
	call	cstart

	/* Stop system running */
spin:
	cli
.L1:
	hlt
	jmp	.L1

	/* Should never reach here */
	int3
	jmp	.

.bss
	.space	8192
_sys_stack:
