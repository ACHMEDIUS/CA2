	.file	"comp.c"
	.section	.text
	.global	A
	.section	.bss
	.align 4
	.type	A, @object
	.size	A, 80
A:
	.zero	80
	.global	B
	.align 4
	.type	B, @object
	.size	B, 80
B:
	.zero	80
	.section	.text
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -16
	l.sw	8(r1), r2
	l.addi	r2, r1, 16
	l.sw	12(r1), r9
	l.sw	-12(r2), r0
	l.j	.L2
	 l.nop

.L3:
	l.movhi	r17, ha(B)
	l.lwz	r19, -12(r2)
	l.ori	r21, r0, 2
	l.sll	r19, r19, r21
	l.addi	r17, r17, lo(B)
	l.add	r17, r19, r17
	l.lwz	r19, -12(r2)
	l.sw	0(r17), r19
	l.lwz	r17, -12(r2)
	l.addi	r17, r17, 1
	l.sw	-12(r2), r17
.L2:
	l.lwz	r19, -12(r2)
	l.ori	r17, r0, 19
	l.sfles	r19, r17
	l.bf	.L3
	 l.nop

	l.ori	r17, r0, 42
	l.sw	-16(r2), r17
	l.ori	r17, r0, 1
	l.sw	-12(r2), r17
	l.j	.L4
	 l.nop

.L5:
	l.lwz	r17, -12(r2)
	l.addi	r19, r17, -1
	l.movhi	r17, ha(B)
	l.ori	r23, r0, 2
	l.sll	r19, r19, r23
	l.addi	r17, r17, lo(B)
	l.add	r17, r19, r17
	l.lwz	r19, 0(r17)
	l.lwz	r17, -16(r2)
	l.add	r19, r19, r17
	l.movhi	r17, ha(A)
	l.lwz	r21, -12(r2)
	l.sll	r21, r21, r23
	l.addi	r17, r17, lo(A)
	l.add	r17, r21, r17
	l.sw	0(r17), r19
	l.lwz	r17, -12(r2)
	l.addi	r17, r17, 1
	l.sw	-12(r2), r17
.L4:
	l.lwz	r19, -12(r2)
	l.ori	r17, r0, 19
	l.sfles	r19, r17
	l.bf	.L5
	 l.nop

	l.movhi	r17, ha(A)
	l.addi	r17, r17, lo(A)
	l.addi	r17, r17, 76
	l.lwz	r17, 0(r17)
	l.or	r3, r17, r17
	l.jal	print_roman
	 l.nop

	l.movhi	r17, hi(0)
	l.or	r11, r17, r17
	l.lwz	r2, 8(r1)
	l.lwz	r9, 12(r1)
	l.addi	r1, r1, 16
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 12.0.1 20220210 (experimental)"
