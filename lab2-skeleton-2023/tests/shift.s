	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
    l.sll r4,r1,r3 # shift left r1 (1) by r3 (1)

    l.sra r5,r2,r3 # shift right r2 by r3 (2) (arithmetic)

    l.srl r6,r2,r3 # shift right r2 by r3 (2) (logically)
	
	.word	0x40ffccff
	.size	_start, .-_start
