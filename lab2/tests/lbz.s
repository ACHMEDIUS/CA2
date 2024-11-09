#load byte extend with zero
# Example of how to write tests for load instructions. In order to
# execute a load instruction, a readable memory must be available.
# To do so, we allocate a memory in the assembly program ("A" in
# this case). After assembling the program, the memory location of the
# array must be determined. This can be done using "objdump". In this
# case, the location is 0x11100 (69888 decimal). We initialize the "x1"
# (R1) register with the value 69888. This way, the load instructions in
# the program below will read from valid memory addresses where also
# initialized data is found, which allows the register values to be verified
# when the program terminates.
# also check if zero extension works properly

       .data
       .align 8
       .local  A
A:
       .byte 1, 2, 3, 4, 5, 6, 7, 128
       .size   A, .-A
       .text
       .align 1
       .globl  _start
       .type   _start, @function
_start:
       l.lwz r5,0(r1)             # [0x01 02 03 04] = 
       l.lbz r2,0(r1)             # 1
       l.lbz r3,1(r1)             # 2
       l.lbz r4,4(r1)             # 5
       l.lbz r7,7(r1)		  # 128
       .word  0x40ffccff
       .size   _start, .-_start
