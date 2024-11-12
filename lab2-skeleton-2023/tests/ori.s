	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.addi r1,r0,0x0F00
    l.ori r2,r1,0x000F
    l.ori r3,r1,0xF0FF
	
	.word	0x40ffccff
	.size	_start, .-_start
