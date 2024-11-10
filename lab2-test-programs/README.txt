The testdata/ directory contains reference output for levels 1, 2, 3, 4, 5
and 8 (that is, all levels without pipelining).

You can automatically run the programs and compare to the reference output
by running the following command from your "rv64-emu" directory:

./test_output.py -C path/to/lab2-test-programs/


For the reference output, the following assumptions apply:

- "bytes read" includes memory reads for instruction fetch.
- The emulator terminates as soon as the "sw a0,632(zero)" instruction
  reaches and completes the memory stage. This means that this final "sw"
  instruction is issued but not completed (it does not reach write back) and
  this is reflected in the instruction counts.

We did not include reference output for pipelined execution because the
instruction counts and memory bytes may differ due to intricate differences
in implementation. However, the register state at program termination should
be equivalent if the test programs are executed with pipelining enabled.


