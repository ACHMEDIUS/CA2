# Adapted from crt_simple.s in the MGSim project.
        .section delay
        .section .text
        .align  4
        .proc   _start
        .global _start
        .type   _start, @function
_start:
        # prepare the pseudo-stack (SP, FP)
        l.movhi r1,hi(_stack+4064)
        l.ori   r1,r1,lo(_stack+4064)
        l.addi  r2,r1,0

        # call main
        l.jal   main
        l.nop

        # end simulation with exit code
        l.addi  r3,r0,632
        l.sw    0(r3),r11
        l.nop
        l.nop
        l.nop
        l.nop
        l.nop
        l.nop
        l.nop
        .size   _start, .-_start
        .comm   _stack,4096,4
