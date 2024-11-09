#we don't have instructions that check flags but we can see if the remainders are as expected
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.add 	r4,r1,r2 # 1+maxint = negative no carry
	l.add 	r5,r4,r3 # maxneg + -1 = carry rest maxint
	l.add 	r6,r3,r3 # -1 + -1 = -2 carry
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	.word	0x40ffccff
	.size	_start, .-_start
