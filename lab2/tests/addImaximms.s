#immediate value is sign-extended and then added so:
#first 3 lines should yield same result
#32768(0x8000) largest signed value for immediate
#2147483648(0x8000 0000) is max signed int 32b 
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:	
#	l.addi 	r6,r1,65536 throws assembler error
	l.addi 	r7,r1,65535 #does compile
	l.addi 	r4,r1,-1
	l.addi 	r3,r1,-1
	l.addi 	r5,r2,32767 #maximm positve 
	l.addi 	r6,r2,32768 #-32768, -32768
	l.addi 	r8,r5,32768 # gives -32768 of 67
	l.addi 	r9,r5,32767 #32768 largest signed value gives zero
	l.nop
	l.nop
	l.nop
	l.nop
	.word	0x40ffccff
	.size	_start, .-_start
