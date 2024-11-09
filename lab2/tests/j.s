#The immediate value is shifted left two bits, sign-extended to program counter width, and
#then added to the address of the jump instruction. The result is the effective address of the
#jump. The program unconditionally jumps to EA. If CPUCFGR[ND] is not set, the jump
#occurs with a delay of one instruction.
#Note that l.sys should not be placed in the delay slot after a jump.
#immediate is amount of instructions to jump from this one. Hence the bitshift 2 in 4byte instructions
#we test if the jump with adding immediates and also if it takes the correct path with backwards jumps

	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:

	l.j 12   #jump to one after next
	l.nop
	l.addi  r4,r4,1
	l.addi  r4,r4,1
	l.j	12 #jump to next one
	l.nop
	l.j 	36#jump to end
	l.nop
	l.j	8 #jump to next one
	l.nop
	l.addi  r3,r3,1
	l.addi  r3,r3,1
	l.j 	-24 #jump back
	l.nop
	l.addi  r2,r2,1
	.word	0x40ffccff
	.size	_start, .-_start
