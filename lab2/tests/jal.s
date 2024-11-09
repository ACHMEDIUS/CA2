#The immediate value is shifted left two bits, sign-extended to program counter width, and
#then added to the address of the jump instruction. The result is the effective address of the
#jump. The program unconditionally jumps to EA. If CPUCFGR[ND] is not set, the jump
#occurs with a delay of one instruction. The address of the instruction after the delay slot
#is placed in the link register r9 (see Register Usage on page 354).
#The value of the link register, if read as an operand in the delay slot will be the new
#value, not the old value. If the link register is written in the delay slot, the value written
#will replace the value of r9 stored by the l.jal instruction adress + 8.
#Note that l.sys should not be placed in the delay slot after a jump.
	
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.jal 	12 #jump to one after next one
	l.nop
	l.addi  r2,r2,1
	l.addi  r2,r2,1
	.word	0x40ffccff
	.size	_start, .-_start
