#Set flags if less than (or equal)
#The contents of general-purpose registers rA and rB are compared as signed integers. If
#the contents of the first register are less than or equa
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.sfles 	r1,r1 #1 <= 1
	l.bf		12 #hierboven true dus volgende
	l.nop
	l.addi		r6,r6,1
	l.sfles 	r2,r1 #2 <= 1
	l.bf		12 #hierboven false dus niks
	l.nop
	l.addi		r7,r7,1
	l.sfles 	r3,r5 #-2147483648 <= 0
	l.bf		12 #hierboven true dus volgende
	l.nop
	l.addi		r8,r8,1
	l.sfles 	r4,r3 #2147483647 <= -2147483648
	l.bf		12 #hierboven false dus niks
	l.nop
	l.addi		r9,r9,1
	.word	0x40ffccff
	.size	_start, .-_start
