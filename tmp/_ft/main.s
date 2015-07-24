	.file	"main.c"
	.section	.rodata
.LC0:
	.string	"Usage: fly [IP filename]"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movq	$0, -16(%rbp)
	movl	-36(%rbp), %eax
	cmpl	$1, %eax
	je	.L3
	cmpl	$3, %eax
	je	.L4
	jmp	.L7
.L3:
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	getIPAddress
	movq	-16(%rbp), %rax
	movl	-20(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	call	prepareReceiver
	jmp	.L5
.L4:
	movq	-16(%rbp), %rdx
	movq	-48(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	getIPAddress
	jmp	.L5
.L7:
	movq	$.LC0, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	print
.L5:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
