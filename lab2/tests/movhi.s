#The 16-bit immediate value is zero-extended, shifted left by 16 bits, and placed into
#general-purpose register rD.
#
#maxint imm 65535 or -32768
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.movhi 	r1,-1  # maxint zeroext. << 16
	l.movhi 	r5,2 #2 zeroext. << 16
	l.movhi 	r6,0 #0 zeroext. << 16
	l.movhi 	r7,65535  # maxint zeroext. << 16
	.word	0x40ffccff
	.size	_start, .-_start
