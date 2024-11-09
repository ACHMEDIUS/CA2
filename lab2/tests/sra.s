#Shift right arithmethic. 
#The 6-bit immediate value specifies the number of bits shifted to the right. MSbit ignored in 32bit
#high-order bits are signextended
#2147483648 maxint 4294967296
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.sra 	r8,r1,r2 #1 >> 2
	l.sra 	r9,r13,r2 #-1 >> 2 sign extended means it remains -1
	l.sra 	r10,r3,r2 #32 >> 2
	l.sra 	r11,r3,r4 #32 >> 0
	l.sra 	r12,r6,r1 #maxint >> 1
	l.sra 	r4,r7,r5 #-maxint >> 30
	l.sra 	r14,r6,r13 #maxint >> -2
	l.sra 	r15,r7,r13 #-maxint >> -2
	.word	0x40ffccff
	.size	_start, .-_start
