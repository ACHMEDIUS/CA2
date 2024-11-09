#subtract and set overflow flags. Similar to add#high-order bits are signextended
#2147483648 maxint 4294967296
#could be faulty because of overflows
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.sub 	r1,r1,r2 #100 - 5
	l.sub 	r1,r1,r3 #95 + 4
	l.sub 	r1,r1,r4 #99-maxint 
	l.sub 	r1,r1,r5 #-maxint+99 - maxneg
	l.sub 	r6,r5,r2 #maxneg - 5
	l.sub 	r7,r3,r4 #-4(4294967292) - 2147483647
	l.sub 	r2,r5,r4 #maxneg -  maxint
	l.sub 	r3,r4,r5 #maxint - maxneg
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	.word	0x40ffccff
	.size	_start, .-_start
