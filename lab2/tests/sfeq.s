#Set flag equal
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.sfeq 	r2,r2 #2 == 2
	l.bf		12 #hierboven true dus volgende
	l.nop
	l.addi		r6,r6,1
	l.sfeq 	r2,r1 #2 == 1
	l.bf		12 #hierboven false dus niks
	l.nop
	l.addi		r7,r7,1
	l.sfeq 	r3,r3 #-2147483648 == -2147483648
	l.bf		12 #hierboven true dus volgende
	l.nop
	l.addi		r8,r8,1
	l.sfeq 	r4,r3 #-2147483648 == 2147483647
	l.bf		12 #hierboven false dus niks
	l.nop
	l.addi		r9,r9,1
	.word	0x40ffccff
	.size	_start, .-_start
