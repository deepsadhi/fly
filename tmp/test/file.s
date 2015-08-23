	.file	"file.c"
	.section	.rodata
.LC0:
	.string	"r"
.LC1:
	.string	"f.txt"
.LC2:
	.string	"size = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$56, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$1, -52(%rbp)
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	call	fopen
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	fseek
	jmp	.L2
.L6:
	movq	-48(%rbp), %rdx
	leaq	-32(%rbp), %rax
	movq	%rdx, %rcx
	movl	$5, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	cmpq	$4, -40(%rbp)
	ja	.L3
	movq	-40(%rbp), %rax
	subl	$1, %eax
	movl	%eax, -56(%rbp)
	jmp	.L4
.L5:
	movl	-56(%rbp), %eax
	cltq
	movb	$0, -32(%rbp,%rax)
	addl	$1, -56(%rbp)
.L4:
	cmpl	$4, -56(%rbp)
	jle	.L5
.L3:
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	puts
.L2:
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	feof
	testl	%eax, %eax
	je	.L6
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movl	$0, %eax
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L8
	call	__stack_chk_fail
.L8:
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
