#32768(0x8000) largest signed value for immediate
#2147483648(0x8000 0000) is max signed int 32b
#any big number whose MSB after signed bit is 1 might cause overflow with negatives
#trouble with adding max negatives either: 2147483647 or 2147483649 (-2147483647)
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:	
	l.addi  r5,r2,-1 #maxint + -1 (overflow of value to signed bit)
	l.addi  r6,r2,1  #maxint +  1 (clips to negative value)
	l.addi  r7,r3,-1 #maxnegative + -1 overflow of value to signed bit with double negative
	l.addi  r8,r3,1  #maxnegative + 1
	l.addi  r9,r1,2  #-1 + 2 overflow
	l.addi  r10,r1,-1  #-1 +-1
	l.addi r11,r3,0 #should be the same numbers
	l.addi r12,r4,0
	l.nop
	l.nop
	l.nop
	l.nop
	.word	0x40ffccff
	.size	_start, .-_start
