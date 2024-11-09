#The immediate value is shifted left two bits, sign-extended to program counter width, and
#then added to the address of the branch instruction. The result is the effective address of
#the branch. If the flag is set, the program branches to EA. If CPUCFGR[ND] is not set,
#the branch occurs with a delay of one instruction. 
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
    l.sfles r1,r1 #1 <=1 true so do branch
	l.bf 	12 #jump to one after next one
	l.nop
	l.addi  r2,r2,1
	l.addi  r2,r2,1
	.word	0x40ffccff
	.size	_start, .-_start
