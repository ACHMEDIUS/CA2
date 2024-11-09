The testdata/ directory that can be found here contains reference output for
levels 0, 1, 2, 3 and 4 (that is, all levels without pipelining), but with
delay slots fully implemented (!). See below for reference output for
levels 0, 1, 2 *without* delay slots implemented.

You can automatically run the programs and compare to the reference output
(with delay slots implemented) by running the following command from the
"rv64-emu" directory:

./test_output.py -C path/to/lab2-test-programs-2022


For the reference output, the following assumptions apply:

- Delay slots are implemented and are executed for all jumps and branches.
- "bytes read" includes memory reads for instruction fetch.
- The emulator terminates as soon as the "l.sw 0(r3),r11" instruction
  reaches and completes the memory stage. This means that this final "l.sw"
  instruction is issued but not completed (it does not reach write back) and
  this is reflected in the instruction counts.

We did not include reference output for pipelined execution because the
instruction counts and memory bytes may differ due to intricate differences
in implementation. However, the register state at program termination should
be equivalent if the test programs are executed with pipelining enabled.


Reference output without delay slots
====================================

For levels 0 to 3 delay slots did not have to be considered. This means that
for jumps, the delay slot in fact becomes the return address. In the case of
branches, the delay slot is only executed when the branch is not taken,
because the delay slot is then the successor instruction of the branch.
This leads to a slightly different instruction count compared to the
reference output in testdata/. The output for implementations without delay
slots is given below:

level 0: basic.bin
------------------

ABNORMAL PROGRAM TERMINATION; PC = 10014
Reason: Test end marker encountered at address 10014
R00 0x00000000	R16 0x00000000
R01 0x00000026	R17 0x00000000
R02 0x0000002a	R18 0x00000000
R03 0xffffff31	R19 0x00000000
R04 0x00000004	R20 0x00000000
R05 0x00000000	R21 0x00000000
R06 0x00000000	R22 0x00000000
R07 0x00000000	R23 0x00000000
R08 0x00000000	R24 0x00000000
R09 0x00000000	R25 0x00000000
R10 0x00000000	R26 0x00000000
R11 0x00000000	R27 0x00000000
R12 0x00000000	R28 0x00000000
R13 0x00000000	R29 0x00000000
R14 0x00000000	R30 0x00000000
R15 0x00000000	R31 0x00000000
25 clock cycles, 5 instructions issued, 5 instructions completed.
24 bytes read, 0 bytes written.


level 1: hellonods.bin
----------------------

hello
System halt requested.
R00 0x00000000	R16 0x00000000
R01 0x00013054	R17 0x00000200
R02 0x00013054	R18 0x00000000
R03 0x00000278	R19 0x0000000a
R04 0x00000000	R20 0x00000000
R05 0x00000000	R21 0x00000000
R06 0x00000000	R22 0x00000000
R07 0x00000000	R23 0x00000000
R08 0x00000000	R24 0x00000000
R09 0x0001004c	R25 0x00000000
R10 0x00000000	R26 0x00000000
R11 0x00000000	R27 0x00000000
R12 0x00000000	R28 0x00000000
R13 0x00000000	R29 0x00000000
R14 0x00000000	R30 0x00000000
R15 0x00000000	R31 0x00000000
104 clock cycles, 21 instructions issued, 20 instructions completed.
84 bytes read, 10 bytes written.


level 2: hello.bin
------------------

hello
System halt requested.
R00 0x00000000	R16 0x00000000
R01 0x000130dc	R17 0x00000000
R02 0x000130dc	R18 0x00000000
R03 0x00000278	R19 0x00000200
R04 0x00000000	R20 0x00000000
R05 0x00000000	R21 0x00000000
R06 0x00000000	R22 0x00000000
R07 0x00000000	R23 0x00000000
R08 0x00000000	R24 0x00000000
R09 0x000100d4	R25 0x00000000
R10 0x00000000	R26 0x00000000
R11 0x00000000	R27 0x00000000
R12 0x00000000	R28 0x00000000
R13 0x00000000	R29 0x00000000
R14 0x00000000	R30 0x00000000
R15 0x00000000	R31 0x00000000
744 clock cycles, 149 instructions issued, 148 instructions completed.
700 bytes read, 114 bytes written.


level 3: comp.bin
-----------------

LX
System halt requested.
R00 0x00000000	R16 0x00000000
R01 0x00013360	R17 0x00000000
R02 0x00013360	R18 0x00000000
R03 0x00000278	R19 0x00000200
R04 0x00000000	R20 0x00000000
R05 0x00000000	R21 0x00000003
R06 0x00000000	R22 0x00000000
R07 0x00000000	R23 0x00000002
R08 0x00000000	R24 0x00000000
R09 0x00010278	R25 0x00000000
R10 0x00000000	R26 0x00000000
R11 0x00000000	R27 0x00000000
R12 0x00000000	R28 0x00000000
R13 0x00000000	R29 0x00000000
R14 0x00000000	R30 0x00000000
R15 0x00000000	R31 0x00000000
5484 clock cycles, 1097 instructions issued, 1096 instructions completed.
5478 bytes read, 443 bytes written.

