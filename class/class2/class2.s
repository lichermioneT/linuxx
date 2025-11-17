	.file	"class2.cc"
	.text
	.section	.rodata
	.type	_ZStL19piecewise_construct, @object
	.size	_ZStL19piecewise_construct, 1
_ZStL19piecewise_construct:
	.zero	1
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.text
	.globl	_Z6swap_cPiS_
	.type	_Z6swap_cPiS_, @function
_Z6swap_cPiS_:
.LFB1522:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	-32(%rbp), %rax
	movl	(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	movq	-32(%rbp), %rax
	movl	-4(%rbp), %edx
	movl	%edx, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1522:
	.size	_Z6swap_cPiS_, .-_Z6swap_cPiS_
	.globl	_Z8swap_cppRiS_
	.type	_Z8swap_cppRiS_, @function
_Z8swap_cppRiS_:
.LFB1523:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	-32(%rbp), %rax
	movl	(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	movq	-32(%rbp), %rax
	movl	-4(%rbp), %edx
	movl	%edx, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1523:
	.size	_Z8swap_cppRiS_, .-_Z8swap_cppRiS_
	.local	_ZZ6Count1vE1n
	.comm	_ZZ6Count1vE1n,4,4
	.globl	_Z6Count1v
	.type	_Z6Count1v, @function
_Z6Count1v:
.LFB1524:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	_ZZ6Count1vE1n(%rip), %eax
	addl	$1, %eax
	movl	%eax, _ZZ6Count1vE1n(%rip)
	movl	_ZZ6Count1vE1n(%rip), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1524:
	.size	_Z6Count1v, .-_Z6Count1v
	.local	_ZZ6Count2vE1n
	.comm	_ZZ6Count2vE1n,4,4
	.globl	_Z6Count2v
	.type	_Z6Count2v, @function
_Z6Count2v:
.LFB1525:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	_ZZ6Count2vE1n(%rip), %eax
	addl	$1, %eax
	movl	%eax, _ZZ6Count2vE1n(%rip)
	movl	$_ZZ6Count2vE1n, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1525:
	.size	_Z6Count2v, .-_Z6Count2v
	.globl	a
	.bss
	.align 32
	.type	a, @object
	.size	a, 40000
a:
	.zero	40000
	.text
	.globl	_Z5func1v
	.type	_Z5func1v, @function
_Z5func1v:
.LFB1526:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$a, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1526:
	.size	_Z5func1v, .-_Z5func1v
	.globl	_Z5func2v
	.type	_Z5func2v, @function
_Z5func2v:
.LFB1527:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$a, %ecx
	movl	$40000, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1527:
	.size	_Z5func2v, .-_Z5func2v
	.globl	main
	.type	main, @function
main:
.LFB1529:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movl	$1, -96(%rbp)
	movl	$3, -92(%rbp)
	movl	$4, -88(%rbp)
	movl	$56, -84(%rbp)
	movl	$6, -80(%rbp)
	movl	$6, -76(%rbp)
	movl	$7, -72(%rbp)
	movl	$88, -68(%rbp)
	movl	$0, -64(%rbp)
	leaq	-96(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-16(%rbp), %rax
	addq	$36, %rax
	movq	%rax, -24(%rbp)
.L13:
	movq	-8(%rbp), %rax
	cmpq	-24(%rbp), %rax
	je	.L12
	movq	-8(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	addl	%eax, %eax
	movl	%eax, %esi
	movl	$_ZSt4cout, %edi
	call	_ZNSolsEi
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
	addq	$4, -8(%rbp)
	jmp	.L13
.L12:
	movq	$0, -40(%rbp)
	movq	$0, -48(%rbp)
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1529:
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1952:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L17
	cmpl	$65535, -8(%rbp)
	jne	.L17
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	call	__cxa_atexit
.L17:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1952:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__Z6swap_cPiS_, @function
_GLOBAL__sub_I__Z6swap_cPiS_:
.LFB1953:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1953:
	.size	_GLOBAL__sub_I__Z6swap_cPiS_, .-_GLOBAL__sub_I__Z6swap_cPiS_
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z6swap_cPiS_
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 9.3.1 20200408 (Red Hat 9.3.1-2)"
	.section	.note.GNU-stack,"",@progbits
