# This is hello.c compiled with -O1 and the instruction in the delay
# slot manually moved.
	.file	"hello.c"
	.section	.text
	.align 4
	.global	main
	.type	main, @function
main:
	l.ori	r17, r0, 512
	l.ori	r19, r0, 104
	l.sb	0(r17), r19
	l.ori	r19, r0, 101
	l.sb	0(r17), r19
	l.ori	r19, r0, 108
	l.sb	0(r17), r19
	l.sb	0(r17), r19
	l.ori	r19, r0, 111
	l.sb	0(r17), r19
	l.ori	r19, r0, 10
	l.sb	0(r17), r19
	l.movhi	r11, hi(0)
	l.jr	r9
	l.nop
	.size	main, .-main
	.ident	"GCC: (GNU) 12.0.1 20220210 (experimental)"
