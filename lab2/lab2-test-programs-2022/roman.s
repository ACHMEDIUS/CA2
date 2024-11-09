	.file	"roman.c"
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
	.section	.data
	.align 4
	.type	roman_table, @object
	.size	roman_table, 64
roman_table:
	.long	1000
	.byte	77
	.zero	3
	.long	500
	.byte	68
	.zero	3
	.long	100
	.byte	67
	.zero	3
	.long	50
	.byte	76
	.zero	3
	.long	10
	.byte	88
	.zero	3
	.long	5
	.byte	86
	.zero	3
	.long	1
	.byte	73
	.zero	3
	.long	0
	.byte	0
	.zero	3
	.section	.text
	.align 4
	.global	print_roman
	.type	print_roman, @function
print_roman:
	l.addi	r1, r1, -20
	l.sw	8(r1), r16
	l.sw	12(r1), r2
	l.addi	r2, r1, 20
	l.sw	16(r1), r9
	l.sw	-20(r2), r3
	l.sw	-16(r2), r0
	l.j	.L4
	 l.nop

.L6:
	l.movhi	r17, ha(roman_table)
	l.lwz	r19, -16(r2)
	l.ori	r16, r0, 3
	l.sll	r19, r19, r16
	l.addi	r17, r17, lo(roman_table)
	l.add	r17, r19, r17
	l.addi	r17, r17, 4
	l.lbz	r17, 0(r17)
	l.ori	r19, r0, 24
	l.sll	r17, r17, r19
	l.sra	r17, r17, r19
	l.or	r3, r17, r17
	l.jal	putchar
	 l.nop

	l.movhi	r17, ha(roman_table)
	l.lwz	r19, -16(r2)
	l.sll	r19, r19, r16
	l.addi	r17, r17, lo(roman_table)
	l.add	r17, r19, r17
	l.lwz	r17, 0(r17)
	l.lwz	r19, -20(r2)
	l.sub	r17, r19, r17
	l.sw	-20(r2), r17
.L5:
	l.movhi	r17, ha(roman_table)
	l.lwz	r19, -16(r2)
	l.ori	r21, r0, 3
	l.sll	r19, r19, r21
	l.addi	r17, r17, lo(roman_table)
	l.add	r17, r19, r17
	l.lwz	r17, 0(r17)
	l.lwz	r19, -20(r2)
	l.sfges	r19, r17
	l.bf	.L6
	 l.nop

	l.lwz	r17, -16(r2)
	l.addi	r17, r17, 1
	l.sw	-16(r2), r17
.L4:
	l.movhi	r17, ha(roman_table)
	l.lwz	r19, -16(r2)
	l.ori	r21, r0, 3
	l.sll	r19, r19, r21
	l.addi	r17, r17, lo(roman_table)
	l.add	r17, r19, r17
	l.lwz	r17, 0(r17)
	l.movhi	r19, hi(0)
	l.sfne	r17, r19
	l.bf	.L5
	 l.nop

	l.ori	r3, r0, 10
	l.jal	putchar
	 l.nop

	l.nop
	l.lwz	r16, 8(r1)
	l.lwz	r2, 12(r1)
	l.lwz	r9, 16(r1)
	l.addi	r1, r1, 20
	l.jr	r9
	 l.nop

	.size	print_roman, .-print_roman
	.ident	"GCC: (GNU) 12.0.1 20220210 (experimental)"
