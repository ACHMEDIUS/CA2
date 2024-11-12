	.file	"hello.c"
	.section	.text
	.section	.text.startup,"ax",@progbits
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
	l.jr	r9
	l.movhi	r11, hi(0)
	.size	main, .-main
	.ident	"GCC: (GNU) 12.0.1 20220210 (experimental)"
