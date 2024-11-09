#Bitwise OR. 
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.or 	r5,r3,r2 #1001|0011
	l.or 	r6,r1,r2 #101|011
	l.or 	r1,r1,r4
	.word	0x40ffccff
	.size	_start, .-_start
