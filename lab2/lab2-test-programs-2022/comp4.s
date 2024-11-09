	.file	"comp.c"
	.section	.text
	.section	.text.startup,"ax",@progbits
	.align 4
	.global	main
	.type	main, @function
main:
	l.movhi	r25, ha(.LANCHOR0)
	l.addi	r25, r25, lo(.LANCHOR0)
	l.addi	r1, r1, -4
	l.sw	0(r1), r9
	l.or	r21, r25, r25
	l.or	r19, r25, r25
	l.movhi	r17, hi(0)
	l.ori	r23, r0, 20
.L2:
	l.sw	0(r19), r17
	l.addi	r17, r17, 1
	l.sfne	r17, r23
	l.bf	.L2
	l.addi	r19, r19, 4
	l.addi	r17, r25, 84
	l.addi	r23, r25, 160
.L3:
	l.lwz	r19, 0(r21)
	l.addi	r19, r19, 42
	l.sw	0(r17), r19
	l.addi	r17, r17, 4
	l.sfne	r17, r23
	l.bf	.L3
	l.addi	r21, r21, 4
	l.jal	print_roman
	l.lwz	r3, 156(r25)
	l.movhi	r11, hi(0)
	l.lwz	r9, 0(r1)
	l.jr	r9
	l.addi	r1, r1, 4
	.size	main, .-main
	.global	B
	.global	A
	.section	.bss
	.align 4
	.set	.LANCHOR0,. + 0
	.type	B, @object
	.size	B, 80
B:
	.zero	80
	.type	A, @object
	.size	A, 80
A:
	.zero	80
	.ident	"GCC: (GNU) 12.0.1 20220210 (experimental)"
