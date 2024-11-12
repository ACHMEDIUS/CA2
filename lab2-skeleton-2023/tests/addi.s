	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.addi 	r1,r1,21
	l.addi 	r2,r2,-2
	
	.word	0x40ffccff
	.size	_start, .-_start
