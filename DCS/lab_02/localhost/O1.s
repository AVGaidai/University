	.file	"sqrts.c"
	.text
	.globl	get_time
	.type	get_time, @function
get_time:
.LFB24:
	.cfi_startproc
#APP
# 15 "sqrts.c" 1
	rdtsc

# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	orq	%rdx, %rax
	ret
	.cfi_endproc
.LFE24:
	.size	get_time, .-get_time
	.globl	sqrts
	.type	sqrts, @function
sqrts:
.LFB25:
	.cfi_startproc
	testl	%ecx, %ecx
	jle	.L8
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdx, %r14
	movq	%rsi, %r13
	movq	%rdi, %r12
	leal	-1(%rcx), %r15d
	addq	$1, %r15
	movl	$0, %ebx
	jmp	.L5
.L12:
	call	sqrtf
.L4:
	movl	%ebp, (%r14,%rbx,4)
	addq	$1, %rbx
	cmpq	%r15, %rbx
	je	.L11
.L5:
	movss	(%r12,%rbx,4), %xmm0
	mulss	0(%r13,%rbx,4), %xmm0
	sqrtss	%xmm0, %xmm1
	movd	%xmm1, %ebp
	pxor	%xmm2, %xmm2
	ucomiss	%xmm0, %xmm2
	jbe	.L4
	jmp	.L12
.L11:
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_restore 13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_restore 14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_restore 15
	.cfi_def_cfa_offset 8
	ret
.L8:
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE25:
	.size	sqrts, .-sqrts
	.globl	matrix_init
	.type	matrix_init, @function
matrix_init:
.LFB26:
	.cfi_startproc
	testl	%esi, %esi
	jle	.L18
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rbx
	leal	-1(%rsi), %eax
	leaq	4(%rdi,%rax,4), %rbp
.L15:
	call	rand
	pxor	%xmm0, %xmm0
	cvtsi2ss	%eax, %xmm0
	mulss	.LC1(%rip), %xmm0
	mulss	.LC2(%rip), %xmm0
	movss	%xmm0, (%rbx)
	addq	$4, %rbx
	cmpq	%rbp, %rbx
	jne	.L15
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	ret
.L18:
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE26:
	.size	matrix_init, .-matrix_init
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"%f "
	.text
	.globl	matrix_print
	.type	matrix_print, @function
matrix_print:
.LFB27:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	testl	%esi, %esi
	jle	.L22
	movq	%rdi, %rbx
	leal	-1(%rsi), %eax
	leaq	4(%rdi,%rax,4), %rbp
.L23:
	cvtss2sd	(%rbx), %xmm0
	movl	$.LC3, %edi
	movl	$1, %eax
	call	printf
	addq	$4, %rbx
	cmpq	%rbp, %rbx
	jne	.L23
.L22:
	movl	$10, %edi
	call	putchar
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE27:
	.size	matrix_print, .-matrix_print
	.section	.rodata.str1.1
.LC4:
	.string	"%ld cycles\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB28:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movl	$4096, %edi
	call	malloc
	movq	%rax, %r12
	movl	$4096, %edi
	call	malloc
	movq	%rax, %rbp
	movl	$4096, %edi
	call	malloc
	movq	%rax, %r13
	movl	$0, %edi
	call	time
	movl	%eax, %edi
	call	srand
	movl	$1024, %esi
	movq	%r12, %rdi
	call	matrix_init
	movl	$1024, %esi
	movq	%rbp, %rdi
	call	matrix_init
	movl	$1024, %ecx
	movq	%r13, %rdx
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	sqrts
	call	get_time
	movq	%rax, %r14
	movl	$5, %ebx
.L27:
	movl	$1024, %ecx
	movq	%r13, %rdx
	movq	%rbp, %rsi
	movq	%r12, %rdi
	call	sqrts
	subl	$1, %ebx
	jne	.L27
	call	get_time
	subq	%r14, %rax
	movabsq	$-3689348814741910323, %rdx
	mulq	%rdx
	shrq	$2, %rdx
	movq	%rdx, %rsi
	movl	$.LC4, %edi
	movl	$0, %eax
	call	printf
	movq	%r12, %rdi
	call	free
	movq	%rbp, %rdi
	call	free
	movq	%r13, %rdi
	call	free
	movl	$0, %eax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE28:
	.size	main, .-main
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC1:
	.long	805306368
	.align 4
.LC2:
	.long	1148846080
	.ident	"GCC: (GNU) 6.2.1 20160916 (Red Hat 6.2.1-2)"
	.section	.note.GNU-stack,"",@progbits
