	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.addi 	r1,r1,0
	l.addi 	r1,r2,-10
	l.addi 	r2,r2,-1
	l.addi 	r4,r1,123
	l.addi 	r5,r1,-1	
	l.nop
	l.nop
	l.nop
	l.nop
	.word	0x40ffccff
	.size	_start, .-_start
