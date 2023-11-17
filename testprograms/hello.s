	.file	"hello.c"
	.section	.text
	.align 4
	.type	putchar, @function
putchar:
	l.addi	r1, r1, -16
	l.sw	8(r1), r2
	l.addi	r2, r1, 16
	l.sw	12(r1), r9
	l.sw	-16(r2), r3
	l.ori	r17, r0, 512
	l.sw	-12(r2), r17
	l.lwz	r17, -16(r2)
	l.ori	r19, r0, 24
	l.sll	r17, r17, r19
	l.sra	r17, r17, r19
	l.lwz	r19, -12(r2)
	l.sb	0(r19), r17
	l.movhi	r17, hi(0)
	l.or	r11, r17, r17
	l.lwz	r2, 8(r1)
	l.lwz	r9, 12(r1)
	l.addi	r1, r1, 16
	l.jr	r9
	 l.nop

	.size	putchar, .-putchar
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -8
	l.sw	0(r1), r2
	l.addi	r2, r1, 8
	l.sw	4(r1), r9
	l.ori	r3, r0, 104
	l.jal	putchar
	 l.nop

	l.ori	r3, r0, 101
	l.jal	putchar
	 l.nop

	l.ori	r3, r0, 108
	l.jal	putchar
	 l.nop

	l.ori	r3, r0, 108
	l.jal	putchar
	 l.nop

	l.ori	r3, r0, 111
	l.jal	putchar
	 l.nop

	l.ori	r3, r0, 10
	l.jal	putchar
	 l.nop

	l.movhi	r17, hi(0)
	l.or	r11, r17, r17
	l.lwz	r2, 0(r1)
	l.lwz	r9, 4(r1)
	l.addi	r1, r1, 8
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 12.0.1 20220210 (experimental)"
