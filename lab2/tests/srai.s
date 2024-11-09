#The 6-bit immediate value specifies the number of bits shifted to the right. MSbit ignored in 32bit
#high-order bits are signextended
#2147483648 maxint 4294967296
#could be tested more for immediates negative edge values
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.srai 	r5,r1,2 #1 >> 2
	l.srai 	r7,r2,2 #-1 >> 2
	l.srai 	r8,r3,2 #32 >> 2
	l.srai 	r6,r3,0 #3 >> 0
	l.srai 	r11,r4,1 #maxint >> 1
	l.srai 	r9,r10,30 #-maxint >> 30
	#l.srai 	r10,r4,-1 #maxint >> -1 these throw compiler errors
	#l.srai 	r12,r10,-1 #-maxint >> -1
	.word	0x40ffccff
	.size	_start, .-_start
