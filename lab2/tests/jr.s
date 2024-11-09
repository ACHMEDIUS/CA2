#The contents of general-purpose register rB is the effective address of the jump. The
#program unconditionally jumps to EA. If CPUCFGR[ND] is not set, the jump occurs
#with a delay of one instruction.
#Note that l.sys should not be placed in the delay slot after a jump.
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:

	l.jr	r5 #jump to one after the next one
	l.nop
	l.addi  r4,r4,1
	l.addi  r4,r4,1
	l.jr	r6 #jump to next one
	l.nop
	l.addi  r3,r3,1
	l.addi  r3,r3,1
	l.jr 	r1 #reads jumps to this effective address
	l.nop
	l.addi  r2,r2,1
	.word	0x40ffccff
	.size	_start, .-_start
