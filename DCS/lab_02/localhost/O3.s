	.file	"sqrts.c"
	.text
	.p2align 4,,15
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
	.p2align 4,,15
	.globl	sqrts
	.type	sqrts, @function
sqrts:
.LFB25:
	.cfi_startproc
	testl	%ecx, %ecx
	jle	.L10
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	leal	-1(%rcx), %r14d
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movq	%rdx, %r13
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rsi, %r12
	movq	%rdi, %rbp
	addq	$1, %r14
	xorl	%ebx, %ebx
	subq	$16, %rsp
	.cfi_def_cfa_offset 64
	.p2align 4,,10
	.p2align 3
.L5:
	movss	0(%rbp,%rbx,4), %xmm0
	pxor	%xmm2, %xmm2
	mulss	(%r12,%rbx,4), %xmm0
	ucomiss	%xmm0, %xmm2
	sqrtss	%xmm0, %xmm1
	jbe	.L4
	movss	%xmm1, 12(%rsp)
	call	sqrtf
	movss	12(%rsp), %xmm1
.L4:
	movss	%xmm1, 0(%r13,%rbx,4)
	addq	$1, %rbx
	cmpq	%rbx, %r14
	jne	.L5
	addq	$16, %rsp
	.cfi_def_cfa_offset 48
	xorl	%eax, %eax
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_restore 13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_restore 14
	.cfi_def_cfa_offset 8
	ret
.L10:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE25:
	.size	sqrts, .-sqrts
	.p2align 4,,15
	.globl	matrix_init
	.type	matrix_init, @function
matrix_init:
.LFB26:
	.cfi_startproc
	testl	%esi, %esi
	jle	.L21
	leal	-1(%rsi), %eax
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	leaq	4(%rdi,%rax,4), %rbp
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	.p2align 4,,10
	.p2align 3
.L16:
	call	rand
	pxor	%xmm0, %xmm0
	addq	$4, %rbx
	cvtsi2ss	%eax, %xmm0
	mulss	.LC1(%rip), %xmm0
	mulss	.LC2(%rip), %xmm0
	movss	%xmm0, -4(%rbx)
	cmpq	%rbp, %rbx
	jne	.L16
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	ret
.L21:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE26:
	.size	matrix_init, .-matrix_init
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"%f "
	.text
	.p2align 4,,15
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
	jle	.L27
	leal	-1(%rsi), %eax
	movq	%rdi, %rbx
	leaq	4(%rdi,%rax,4), %rbp
	.p2align 4,,10
	.p2align 3
.L26:
	pxor	%xmm0, %xmm0
	movl	$.LC3, %edi
	movl	$1, %eax
	addq	$4, %rbx
	cvtss2sd	-4(%rbx), %xmm0
	call	printf
	cmpq	%rbx, %rbp
	jne	.L26
.L27:
	movl	$10, %edi
	call	putchar
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
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
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB28:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movl	$4096, %edi
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
	xorl	%ebx, %ebx
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	call	malloc
	movl	$4096, %edi
	movq	%rax, %r12
	call	malloc
	movl	$4096, %edi
	movq	%rax, %rbp
	call	malloc
	xorl	%edi, %edi
	movq	%rax, %r13
	call	time
	movl	%eax, %edi
	call	srand
	movl	$1024, %esi
	movq	%r12, %rdi
	call	matrix_init
	movl	$1024, %esi
	movq	%rbp, %rdi
	call	matrix_init
	.p2align 4,,10
	.p2align 3
.L33:
	movss	(%r12,%rbx), %xmm0
	mulss	0(%rbp,%rbx), %xmm0
	pxor	%xmm3, %xmm3
	ucomiss	%xmm0, %xmm3
	sqrtss	%xmm0, %xmm1
	jbe	.L32
	movss	%xmm1, 12(%rsp)
	call	sqrtf
	movss	12(%rsp), %xmm1
.L32:
	movss	%xmm1, 0(%r13,%rbx)
	addq	$4, %rbx
	cmpq	$4096, %rbx
	jne	.L33
#APP
# 15 "sqrts.c" 1
	rdtsc

# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	movl	$5, %r14d
	orq	%rax, %rdx
	movq	%rdx, %r15
.L34:
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L36:
	movss	(%r12,%rbx), %xmm0
	pxor	%xmm2, %xmm2
	mulss	0(%rbp,%rbx), %xmm0
	ucomiss	%xmm0, %xmm2
	sqrtss	%xmm0, %xmm1
	jbe	.L35
	movss	%xmm1, 12(%rsp)
	call	sqrtf
	movss	12(%rsp), %xmm1
.L35:
	movss	%xmm1, 0(%r13,%rbx)
	addq	$4, %rbx
	cmpq	$4096, %rbx
	jne	.L36
	subl	$1, %r14d
	jne	.L34
#APP
# 15 "sqrts.c" 1
	rdtsc

# 0 "" 2
#NO_APP
	movl	%eax, %eax
	salq	$32, %rdx
	movabsq	$-3689348814741910323, %rsi
	orq	%rax, %rdx
	movl	$.LC4, %edi
	subq	%r15, %rdx
	movq	%rdx, %rax
	mulq	%rsi
	xorl	%eax, %eax
	movq	%rdx, %rsi
	shrq	$2, %rsi
	call	printf
	movq	%r12, %rdi
	call	free
	movq	%rbp, %rdi
	call	free
	movq	%r13, %rdi
	call	free
	addq	$24, %rsp
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
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
