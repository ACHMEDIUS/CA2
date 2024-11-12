.text
        .align 4
    .globl	_start
    .type	_start, @function
_start:
    l.sfles r1,r2
    l.bf B
    l.nop 0x1
    
B:
    l.sfges r1,r2
    l.addi r4,r0,0x01F1
    l.bf C
    l.nop 0x1
    l.sfne r1,r2
    l.bf END
    l.nop 0x1
C:
    l.addi r1,r0,0
    l.addi r2,r0,0
    l.addi r3,r0,0
    l.j END
    l.nop 0x1
END: 
    .word	0x40ffccff
    .size	_start, .-_start
