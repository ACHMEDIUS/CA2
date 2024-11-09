#Bitwise AND. 
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.and 	r1,r1,r2 #10&11
	l.and 	r3,r3,r2 #01&11
	l.and 	r4,r3,r4 #01&00
	.word	0x40ffccff
	.size	_start, .-_start
