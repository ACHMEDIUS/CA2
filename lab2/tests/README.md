In this directory all test files are stored.
We use makenewtest.py to create unit tests and modify them
according to the instruction.
Look at the description below for more information on each test

Note there seems to be a bug with store+loadbyte.s but the existing binary does work fine.

How to use:
python makenewtest.py
only refreshing binaries: make *.bin
running the tests: python ../test_instructions.py -v

===========================================================================================
Implemented:
===========================================================================================

add: basic adds on itself
	sub: very similar, but also testing maxints
	addflags: we test for correct remainders with maxints
	adddatahazard: concise test for stalling
addI: basic adds with imm
	maximms: testing max imms
	maxints: testing with maxints
AND: testing all permutations two bits and negative inputs
bf: first set flag and then do a jump skipping one addi
bnf: first do not set flag and then do a jump skipping one addi
j:use addis to check if jumps happen and also jump backwards
	jal:similar but we also check the jump adress in r9
	jr:similar but we read EA from imm
lbs: Read 3 single byte elements from byte array and check if signextension works
	lbz:similar only no sign extend but zero, also test if reading multiple bytes with lwz works
	lwa: similar only with 4bytes. We don't check atomic functionality
	lwz: similar
movhi: tests if bitshifting works properly
nop: testing if positve, negative, 0 values remain themselves
OR: testing some permutations LSB 4 bits
	ori: similar, only with imms
store: only tests if able to execute for l.sw
	similar for l.sb
	similar for l.sh
store+load: test if we can write and then read
	store+loadbyte: similar with bytes instead of words
sfeq: tests all cases as well as maxint
	sfne: similar but diff outcomes
	sfles: similar
	sfges: similar
sll: shifting positives and negatives, also testing zero and big number for amountshift
sra: testing normal cases and negatives and maxints. also tests "negative shift amounts" which just shifts a lot
	srai: similar but negative shift amounts throw assembler error