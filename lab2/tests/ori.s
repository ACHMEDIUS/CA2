#Bitwise or using an immediate.
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.ori 	r5,r2,9 #1001|0011
	l.ori 	r6,r5,0 #11|0
	l.ori 	r7,r1,3 #101|011
	.word	0x40ffccff
	.size	_start, .-_start
