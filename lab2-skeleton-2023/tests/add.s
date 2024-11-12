	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.add 	r1,r1,r2
	l.add 	r1,r1,r3
	l.add 	r1,r1,r4
	l.add 	r1,r1,r5
	l.add 	r1,r1,r2
	l.add 	r1,r1,r3
	l.add 	r1,r1,r4
	l.add 	r1,r1,r5
	l.sub   r2,r1,r2
	l.sub   r4,r1,r4
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	.word	0x40ffccff
	.size	_start, .-_start
