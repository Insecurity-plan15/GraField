
	.text

.globl memcpy, memset, memsetw, strlen

	.type	memcpy, @function
/* void *memcpy(void *dest, const void *src, unsigned long len) */
memcpy:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	pushl	%ecx
	movl	8(%ebp), %edi   /* dest */
	movl	12(%ebp), %esi  /* src  */
	movl	16(%ebp), %ecx  /* len  */

	rep	movsb

	movl	%edi, %eax

	popl	%ecx
	popl	%esi
	popl	%edi
	leave
	ret

	.type	memset, @function
/* void *memset(void *dest, char value, unsigned long len) */
memset:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%ecx
	movl	8(%ebp), %edi   /* dest  */
	movl	12(%ebp), %eax  /* value */
	movl	16(%ebp), %ecx  /* len   */

	rep	stosb

	movl	%edi, %eax

	popl	%ecx
	popl	%edi
	leave
	ret

	.type	memsetw, @function
/* void *memsetw(void *dest, short value, unsigned long len) */
memsetw:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%ecx
	movl	8(%ebp), %edi   /* dest  */
	movl	12(%ebp), %eax  /* value */
	movl	16(%ebp), %ecx  /* len   */

	rep	stosw

	movl	%edi, %eax

	popl	%ecx
	popl	%edi
	leave
	ret

	.type	strlen, @function
/* unsigned long strlen(const char *str) */
strlen:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%ecx
	movl	8(%ebp), %esi   /* str */

	xor	%ecx, %ecx
.L1:
	lodsb
	test	%eax, %eax
	jz	.L2
	inc	%ecx
	jmp	.L1

.L2:
	mov	%ecx, %eax

	popl	%eax
	popl	%esi
	leave
	ret
