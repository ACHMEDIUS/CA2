	.text
	.align 4
	.global _start
	.type _start, @function
_start:
	l.addi r1,r0,38
	l.addi r2,r1,4
	l.addi r3,r1,-245
	l.sub r4,r2,r1
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	.word 0x40ffccff # test end marker
	.size _start, .-_start
# Post conditions:
#  x1 contains 38 (0x26)
#  x2 contains 42 (0x2a)
#  x3 contains -207 (0xffffffffffffff31)
#  x4 contains 4 (0x4)
