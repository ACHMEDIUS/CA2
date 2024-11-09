        .bss
        .align 8
        .local  stack
        .comm   stack,4096,8
        .size   stack,4096
        .text
        .align 4
        .globl  _start
        .type   _start, @function
_start:
       l.addi  sp,sp,-32
       l.sw    24(sp),r6
       l.nop
       l.nop
       l.nop
       l.nop
       l.nop
       l.nop
       l.nop
       l.lwz r5,24(sp)            # load R6 into R5
       .word  0x40ffccff
       .size   _start, .-_start
       