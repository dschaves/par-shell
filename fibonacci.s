	.file	"fibonacci.c"
	.section	.rodata
	.align 8
.LC0:
	.string	"Incorrect arguments: fibonacci number_of_terms"
	.align 8
.LC1:
	.string	"Process %d started to generate Fibonacci up to %lu.\n"
	.align 8
.LC2:
	.string	"Process %d finished Fibonacci series at value: %lu.\n"
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
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	movq	%rsi, -64(%rbp)
	cmpl	$2, -52(%rbp)
	je	.L2
	movl	$.LC0, %edi
	call	puts
	movl	$1, %edi
	call	exit
.L2:
	movq	$0, -8(%rbp)
	movq	$1, -16(%rbp)
	movq	-64(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi
	cltq
	movq	%rax, -48(%rbp)
	call	getpid
	movl	%eax, %ecx
	movq	-48(%rbp), %rax
	movq	%rax, %rdx
	movl	%ecx, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movq	$0, -32(%rbp)
	jmp	.L3
.L8:
	movq	$0, -40(%rbp)
	jmp	.L4
.L5:
	addq	$1, -40(%rbp)
.L4:
	cmpq	$99999, -40(%rbp)
	jbe	.L5
	cmpq	$1, -32(%rbp)
	ja	.L6
	movq	-32(%rbp), %rax
	movq	%rax, -24(%rbp)
	jmp	.L7
.L6:
	movq	-8(%rbp), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -24(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
.L7:
	addq	$1, -32(%rbp)
.L3:
	movq	-32(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jb	.L8
	call	getpid
	movl	%eax, %ecx
	movq	-24(%rbp), %rax
	movq	%rax, %rdx
	movl	%ecx, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %edi
	call	exit
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Debian 4.9.2-10) 4.9.2"
	.section	.note.GNU-stack,"",@progbits
