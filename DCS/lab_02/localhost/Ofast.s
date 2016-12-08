	.file	"sqrts.c"
	.text
	.p2align 4,,15
	.globl	get_time
	.type	get_time, @function
get_time:
.LFB33:
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
.LFE33:
	.size	get_time, .-get_time
	.p2align 4,,15
	.globl	sqrts
	.type	sqrts, @function
sqrts:
.LFB34:
	.cfi_startproc
	testl	%ecx, %ecx
	jle	.L30
	leaq	16(%rdi), %rax
	leaq	16(%rdx), %r8
	cmpq	%rax, %rdx
	setnb	%al
	cmpq	%r8, %rdi
	setnb	%r9b
	orl	%r9d, %eax
	cmpl	$7, %ecx
	seta	%r9b
	testb	%r9b, %al
	je	.L4
	leaq	16(%rsi), %rax
	cmpq	%rax, %rdx
	setnb	%r9b
	cmpq	%r8, %rsi
	setnb	%al
	orb	%al, %r9b
	je	.L4
	movq	%rdi, %rax
	xorl	%r8d, %r8d
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	shrq	$2, %rax
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	negq	%rax
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	andl	$3, %eax
	je	.L5
	movss	(%rdi), %xmm0
	cmpl	$1, %eax
	movl	$1, %r8d
	mulss	(%rsi), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%rdx)
	je	.L5
	movss	4(%rdi), %xmm0
	cmpl	$3, %eax
	movl	$2, %r8d
	mulss	4(%rsi), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, 4(%rdx)
	jne	.L5
	movss	8(%rdi), %xmm0
	movl	$3, %r8d
	mulss	8(%rsi), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, 8(%rdx)
.L5:
	movl	%ecx, %ebx
	pxor	%xmm3, %xmm3
	subl	%eax, %ebx
	movl	%eax, %eax
	xorl	%r9d, %r9d
	leal	-4(%rbx), %r10d
	salq	$2, %rax
	xorl	%r11d, %r11d
	movaps	.LC0(%rip), %xmm5
	leaq	(%rdi,%rax), %r13
	shrl	$2, %r10d
	leaq	(%rsi,%rax), %r12
	addq	%rdx, %rax
	addl	$1, %r10d
	movaps	.LC1(%rip), %xmm4
	leal	0(,%r10,4), %ebp
.L7:
	movups	(%r12,%r9), %xmm1
	movaps	%xmm3, %xmm2
	addl	$1, %r11d
	mulps	0(%r13,%r9), %xmm1
	cmpneqps	%xmm1, %xmm2
	rsqrtps	%xmm1, %xmm0
	andps	%xmm2, %xmm0
	mulps	%xmm0, %xmm1
	mulps	%xmm1, %xmm0
	mulps	%xmm4, %xmm1
	addps	%xmm5, %xmm0
	mulps	%xmm1, %xmm0
	movups	%xmm0, (%rax,%r9)
	addq	$16, %r9
	cmpl	%r11d, %r10d
	ja	.L7
	addl	%ebp, %r8d
	cmpl	%ebp, %ebx
	je	.L19
	movslq	%r8d, %rax
	movss	(%rdi,%rax,4), %xmm0
	mulss	(%rsi,%rax,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%rdx,%rax,4)
	leal	1(%r8), %eax
	cmpl	%eax, %ecx
	jle	.L19
	cltq
	addl	$2, %r8d
	movss	(%rdi,%rax,4), %xmm0
	cmpl	%r8d, %ecx
	mulss	(%rsi,%rax,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%rdx,%rax,4)
	jle	.L19
	movslq	%r8d, %rax
	movss	(%rdi,%rax,4), %xmm0
	mulss	(%rsi,%rax,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%rdx,%rax,4)
.L19:
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 32
	xorl	%eax, %eax
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_restore 13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	subl	$1, %ecx
	xorl	%eax, %eax
	addq	$1, %rcx
	.p2align 4,,10
	.p2align 3
.L12:
	movss	(%rdi,%rax,4), %xmm0
	mulss	(%rsi,%rax,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%rdx,%rax,4)
	addq	$1, %rax
	cmpq	%rcx, %rax
	jne	.L12
.L30:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE34:
	.size	sqrts, .-sqrts
	.p2align 4,,15
	.globl	matrix_init
	.type	matrix_init, @function
matrix_init:
.LFB35:
	.cfi_startproc
	testl	%esi, %esi
	jle	.L42
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
.L37:
	call	rand
	pxor	%xmm0, %xmm0
	addq	$4, %rbx
	cvtsi2ss	%eax, %xmm0
	mulss	.LC2(%rip), %xmm0
	movss	%xmm0, -4(%rbx)
	cmpq	%rbp, %rbx
	jne	.L37
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
.L42:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE35:
	.size	matrix_init, .-matrix_init
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"%f "
	.text
	.p2align 4,,15
	.globl	matrix_print
	.type	matrix_print, @function
matrix_print:
.LFB36:
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
	jle	.L48
	leal	-1(%rsi), %eax
	movq	%rdi, %rbx
	leaq	4(%rdi,%rax,4), %rbp
	.p2align 4,,10
	.p2align 3
.L47:
	pxor	%xmm0, %xmm0
	movl	$.LC3, %edi
	movl	$1, %eax
	addq	$4, %rbx
	cvtss2sd	-4(%rbx), %xmm0
	call	printf
	cmpq	%rbx, %rbp
	jne	.L47
.L48:
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
.LFE36:
	.size	matrix_print, .-matrix_print
	.section	.rodata.str1.1
.LC4:
	.string	"%ld cycles\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB37:
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
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	call	malloc
	movl	$4096, %edi
	movq	%rax, %rbx
	call	malloc
	movl	$4096, %edi
	movq	%rax, %rbp
	leaq	4096(%rbx), %r14
	call	malloc
	xorl	%edi, %edi
	movq	%rax, %r12
	movq	%rbx, %r13
	call	time
	movl	%eax, %edi
	call	srand
	.p2align 4,,10
	.p2align 3
.L53:
	call	rand
	pxor	%xmm0, %xmm0
	addq	$4, %r13
	cvtsi2ss	%eax, %xmm0
	mulss	.LC2(%rip), %xmm0
	movss	%xmm0, -4(%r13)
	cmpq	%r14, %r13
	jne	.L53
	leaq	4096(%rbp), %r14
	movq	%rbp, %r13
	.p2align 4,,10
	.p2align 3
.L54:
	call	rand
	pxor	%xmm0, %xmm0
	addq	$4, %r13
	cvtsi2ss	%eax, %xmm0
	mulss	.LC2(%rip), %xmm0
	movss	%xmm0, -4(%r13)
	cmpq	%r13, %r14
	jne	.L54
	movq	%rbx, %rcx
	shrq	$2, %rcx
	negq	%rcx
	andl	$3, %ecx
	je	.L69
	movss	(%rbx), %xmm0
	cmpl	$1, %ecx
	mulss	0(%rbp), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%r12)
	je	.L70
	movss	4(%rbx), %xmm0
	cmpl	$3, %ecx
	mulss	4(%rbp), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, 4(%r12)
	jne	.L71
	movss	8(%rbx), %xmm0
	movl	$1021, %r8d
	movl	$3, %eax
	mulss	8(%rbp), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, 8(%r12)
.L56:
	movl	%ecx, %esi
	movl	$1024, %r9d
	movl	$1020, %r10d
	movq	%rsi, %rdx
	subl	%ecx, %r9d
	movl	$255, %r11d
.L55:
	pxor	%xmm5, %xmm5
	salq	$2, %rdx
	movaps	.LC0(%rip), %xmm2
	leaq	(%rbx,%rdx), %r14
	leaq	0(%rbp,%rdx), %r13
	leaq	(%r12,%rdx), %rdi
	xorl	%r15d, %r15d
	movaps	.LC1(%rip), %xmm3
	xorl	%edx, %edx
.L57:
	movaps	%xmm5, %xmm4
	addl	$1, %r15d
	movups	0(%r13,%rdx), %xmm1
	mulps	(%r14,%rdx), %xmm1
	cmpneqps	%xmm1, %xmm4
	rsqrtps	%xmm1, %xmm0
	andps	%xmm4, %xmm0
	mulps	%xmm0, %xmm1
	mulps	%xmm1, %xmm0
	mulps	%xmm3, %xmm1
	addps	%xmm2, %xmm0
	mulps	%xmm0, %xmm1
	movups	%xmm1, (%rdi,%rdx)
	addq	$16, %rdx
	cmpl	%r15d, %r11d
	ja	.L57
	addl	%r10d, %eax
	subl	%r10d, %r8d
	cmpl	%r9d, %r10d
	je	.L59
	movslq	%eax, %rdx
	cmpl	$1, %r8d
	movss	(%rbx,%rdx,4), %xmm0
	mulss	0(%rbp,%rdx,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%r12,%rdx,4)
	leal	1(%rax), %edx
	je	.L59
	movslq	%edx, %rdx
	addl	$2, %eax
	cmpl	$2, %r8d
	movss	(%rbx,%rdx,4), %xmm0
	mulss	0(%rbp,%rdx,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%r12,%rdx,4)
	je	.L59
	cltq
	movss	(%rbx,%rax,4), %xmm0
	mulss	0(%rbp,%rax,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%r12,%rax,4)
.L59:
#APP
# 15 "sqrts.c" 1
	rdtsc

# 0 "" 2
#NO_APP
	movl	$1020, %r9d
	salq	$2, %rsi
	salq	$32, %rdx
	subl	%ecx, %r9d
	movl	%eax, %eax
	movl	$1024, %r15d
	shrl	$2, %r9d
	pxor	%xmm4, %xmm4
	addl	$1, %r9d
	orq	%rax, %rdx
	leaq	(%rbx,%rsi), %r14
	leal	0(,%r9,4), %r11d
	leaq	0(%rbp,%rsi), %r13
	leaq	(%r12,%rsi), %r10
	movq	%rdx, 8(%rsp)
	subl	%ecx, %r15d
	movl	$5, %r8d
.L61:
	testl	%ecx, %ecx
	je	.L72
	movss	(%rbx), %xmm0
	cmpl	$1, %ecx
	mulss	0(%rbp), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%r12)
	je	.L73
	movss	4(%rbx), %xmm0
	cmpl	$3, %ecx
	mulss	4(%rbp), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, 4(%r12)
	jne	.L74
	movss	8(%rbp), %xmm0
	movl	$1021, %esi
	movl	$3, %eax
	mulss	8(%rbx), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, 8(%r12)
.L67:
	xorl	%edx, %edx
	xorl	%edi, %edi
.L62:
	movaps	%xmm4, %xmm5
	addl	$1, %edi
	movups	0(%r13,%rdx), %xmm1
	mulps	(%r14,%rdx), %xmm1
	cmpneqps	%xmm1, %xmm5
	rsqrtps	%xmm1, %xmm0
	andps	%xmm5, %xmm0
	mulps	%xmm0, %xmm1
	mulps	%xmm1, %xmm0
	mulps	%xmm3, %xmm1
	addps	%xmm2, %xmm0
	mulps	%xmm0, %xmm1
	movups	%xmm1, (%r10,%rdx)
	addq	$16, %rdx
	cmpl	%edi, %r9d
	ja	.L62
	addl	%r11d, %eax
	subl	%r11d, %esi
	cmpl	%r15d, %r11d
	je	.L64
	movslq	%eax, %rdx
	cmpl	$1, %esi
	movss	(%rbx,%rdx,4), %xmm0
	mulss	0(%rbp,%rdx,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%r12,%rdx,4)
	leal	1(%rax), %edx
	je	.L64
	movslq	%edx, %rdx
	addl	$2, %eax
	cmpl	$2, %esi
	movss	(%rbx,%rdx,4), %xmm0
	mulss	0(%rbp,%rdx,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%r12,%rdx,4)
	je	.L64
	cltq
	movss	(%rbx,%rax,4), %xmm0
	mulss	0(%rbp,%rax,4), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%r12,%rax,4)
.L64:
	subl	$1, %r8d
	jne	.L61
#APP
# 15 "sqrts.c" 1
	rdtsc

# 0 "" 2
#NO_APP
	movl	%eax, %eax
	salq	$32, %rdx
	movabsq	$-3689348814741910323, %rcx
	orq	%rax, %rdx
	subq	8(%rsp), %rdx
	movl	$.LC4, %edi
	movq	%rdx, %rax
	mulq	%rcx
	xorl	%eax, %eax
	movq	%rdx, %rsi
	shrq	$2, %rsi
	call	printf
	movq	%rbx, %rdi
	call	free
	movq	%rbp, %rdi
	call	free
	movq	%r12, %rdi
	call	free
	addq	$24, %rsp
	.cfi_remember_state
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
.L69:
	.cfi_restore_state
	movl	$1024, %r10d
	movl	$256, %r11d
	movl	$1024, %r9d
	xorl	%edx, %edx
	movl	$1024, %r8d
	xorl	%eax, %eax
	xorl	%esi, %esi
	jmp	.L55
	.p2align 4,,10
	.p2align 3
.L72:
	xorl	%eax, %eax
	movl	$1024, %esi
	jmp	.L67
.L74:
	movl	$1022, %esi
	movl	$2, %eax
	jmp	.L67
.L73:
	movl	$1023, %esi
	movl	$1, %eax
	jmp	.L67
.L70:
	movl	$1023, %r8d
	movl	$1, %eax
	jmp	.L56
.L71:
	movl	$1022, %r8d
	movl	$2, %eax
	jmp	.L56
	.cfi_endproc
.LFE37:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	3225419776
	.long	3225419776
	.long	3225419776
	.long	3225419776
	.align 16
.LC1:
	.long	3204448256
	.long	3204448256
	.long	3204448256
	.long	3204448256
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC2:
	.long	888799232
	.ident	"GCC: (GNU) 6.2.1 20160916 (Red Hat 6.2.1-2)"
	.section	.note.GNU-stack,"",@progbits
