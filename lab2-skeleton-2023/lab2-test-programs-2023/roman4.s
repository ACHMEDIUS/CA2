	.file	"roman.c"
	.section	.text
	.align 4
	.global	print_roman
	.type	print_roman, @function
print_roman:
	l.movhi	r19, ha(.LANCHOR0)
	l.addi	r19, r19, lo(.LANCHOR0)
	l.ori	r21, r0, 512
.L2:
	l.lwz	r17, 0(r19)
	l.sfles	r17, r3
	l.bnf	.L9
	l.movhi	r23, hi(0)
.L3:
	l.lbs	r17, 4(r19)
	l.sb	0(r21), r17
	l.lwz	r17, 0(r19)
	l.sub	r3, r3, r17
	l.sfles	r17, r3
	l.bf	.L3
	l.movhi	r23, hi(0)
.L9:
	l.lwz	r17, 8(r19)
	l.sfeq	r17, r23
	l.bnf	.L2
	l.addi	r19, r19, 8
	l.ori	r17, r0, 512
	l.ori	r19, r0, 10
	l.sb	0(r17), r19
	l.jr	r9
	 l.nop

	.size	print_roman, .-print_roman
	.section	.rodata
	.align 4
	.set	.LANCHOR0,. + 0
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
	.ident	"GCC: (GNU) 12.0.1 20220210 (experimental)"
