#Store and load signextended byte. We test max postive int, then max negative and also
#what happens when we try to save an int that is more than 8bits.
#also test if we don't accidentally overwrite data on stack
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
       l.sb    24(sp),r6 
       l.sb    26(sp),r8 
       l.sb    25(sp),r7 
       l.nop
       l.nop
       l.nop
       l.nop
       l.nop
       l.nop
       l.lbs 	r2,24(sp)
       l.lbs 	r3,25(sp)
       l.lbs 	r4,26(sp)
       .word	0x40ffccff
       .size	_start, .-_start
       