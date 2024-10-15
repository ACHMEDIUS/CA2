
In this directory two sets of tests can be found for the instruction
decoder. Depending on what you have implemented, you can use either set of
tests. "simplified" tests a simplified instruction decoder, "regular" the
regular decoder that gives output closer to "objdump".

Implementation choices made can affect the output of the simplified
instruction decoder. You can also choose to skip this step and go directly
to the regular instruction decoder output.

The regular instruction decoder output that we provide is close to that of
objdump, but not completely equivalent. The output that we produce is
easier to implement compared to that of objdump. Another notable difference
is that for jump and branch instructions we output the offset as found in
the immediate, whereas objdump adds the instruction address to this offset.

To run the tests, use either of the following commands from the "rv64-emu"
directory:

./test_output.py -C path/to/lab2-decoder-test/simplified

or:

./test_output.py -C path/to/lab2-decoder-test/regular


