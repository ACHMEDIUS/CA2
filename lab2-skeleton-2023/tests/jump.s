.text
        .align 4
    .globl	_start
    .type	_start, @function
_start:
    l.jal B
    l.nop
    l.nop
    l.nop
    l.j END
B:
    l.add r1, r0, r2
    l.jr r9
END:
    l.add r1, r1, r2
    .word	0x40ffccff
    .size	_start, .-_start
