#Shift left logical. inserting zero in low-order bits. shfmt = rB[4:0]
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l.sll 	r2,r2,r1 #2<<1
	l.sll 	r2,r2,r4 #4<<0
	l.sll 	r3,r3,r1 #-2<<1
	l.sll 	r5,r2,r3 #4=0x100 << 0x11100 =28 = 0x01000.. = 1073741824
	.word	0x40ffccff
	.size	_start, .-_start
