#sb unit test
        .bss
        .align 8
        .local  stack
        .comm   stack,4096,8
        .size   stack,4096
        .text
        .align 1
        .globl  _start
        .type   _start, @function
_start:
        l.addi  sp,sp,-32
        l.sb    24(sp),r6 #maybe must be different from sw but compilation goes fine
        .word	0x40ffccff
        .size	_start, .-_start
