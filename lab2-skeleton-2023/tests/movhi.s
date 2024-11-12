	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.movhi r1,1
    l.addi r1,r1,-1
	
	.word	0x40ffccff
	.size	_start, .-_start
