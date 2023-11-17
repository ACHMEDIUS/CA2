# Compile all the test programs for CA 2022 Assignment 2 (OpenRISC)

# The x86 target will compile x86 versions of the main test files so you
# can check what the output is supposed to be

TARGETS = basic.bin hellonods.bin hello.bin comp.bin hello4.bin comp4.bin brainfuck.bin \
	hello.s comp.s roman.s hello4.s comp4.s roman4.s \
	hello.dump.txt comp.dump.txt hello4.dump.txt comp4.dump.txt

SRC_DIR = src/
BIN_DIR =
CRT = $(SRC_DIR)minicrt-or1k.s
HEADERS = $(wildcard $(SRC_DIR)*.h)

BIN_TARGETS = $(patsubst %.bin,$(BIN_DIR)%.bin,$(TARGETS))

CC = or1k-elf-gcc
OBJDUMP = or1k-elf-objdump
CFLAGS_COMMON = -Wall -nostdlib -fno-builtin -nodefaultlibs
CFLAGS = -Wl,-Ttext,0x10000 -Wl,-e,_start $(CFLAGS_COMMON)


.phony: all clean x86

all:	$(TARGETS)


#
# The targets for the different levels
# The .s files are generated for level 3, in which case stalls need to
# be inserted manually.
#

# Level 0: basic.bin
basic.bin:	$(SRC_DIR)basic.s $(HEADERS)
		$(CC) $(CFLAGS) -o $@ $<

# Level 1: hellonods.bin
hellonods.bin:	$(SRC_DIR)hellonods.s $(HEADERS)
		$(CC) $(CFLAGS) -o $@ $< $(CRT)

# Level 2: hello.bin, compiled with -O0 (so delay slots are l.nop)
# Compared to level 1, there are more function calls.
hello.bin:	$(SRC_DIR)hello.c $(HEADERS)
		$(CC) $(CFLAGS) -O0 -o $@ $< $(CRT)

hello.s:	$(SRC_DIR)hello.c $(HEADERS)
		$(CC) $(CFLAGS) -O0 -S -o $@ $<

# Level 3: comp.bin, compiled with -O0 (so delay slots are nop)
comp.bin:	$(SRC_DIR)comp.c $(SRC_DIR)roman.c $(HEADERS)
		$(CC) $(CFLAGS) -O0 -o $@ $< $(SRC_DIR)roman.c $(CRT)

comp.s:		$(SRC_DIR)comp.c $(HEADERS)
		$(CC) $(CFLAGS) -O0 -S -o $@ $<

roman.s:	$(SRC_DIR)roman.c $(HEADERS)
		$(CC) $(CFLAGS) -O0 -S -o $@ $<

# Level 4: hello.bin, comp.bin compiled with -O2 (with delay slots)
hello4.bin:	$(SRC_DIR)hello.c $(HEADERS)
		$(CC) $(CFLAGS) -O2 -o $@ $< $(CRT)

hello4.s:	$(SRC_DIR)hello.c $(HEADERS)
		$(CC) $(CFLAGS) -O2 -S -o $@ $<

comp4.bin:	$(SRC_DIR)comp.c $(SRC_DIR)roman.c $(HEADERS)
		$(CC) $(CFLAGS) -O2 -o $@ $< $(SRC_DIR)roman.c $(CRT)

comp4.s:	$(SRC_DIR)comp.c $(HEADERS)
		$(CC) $(CFLAGS) -O2 -S -o $@ $<

roman4.s:	$(SRC_DIR)roman.c $(HEADERS)
		$(CC) $(CFLAGS) -O2 -S -o $@ $<

# Level 6: brainfuck.bin, compiled with -O2
brainfuck.bin:	$(SRC_DIR)brainfuck.c $(HEADERS)
		$(CC) $(CFLAGS) -O2 -o $@ $< $(CRT)


%.dump.txt:	%.bin
		$(OBJDUMP) -D $< > $@


clean:
		rm -f $(BIN_TARGETS)
		rm -fr $(X86_DIR)

#
# x86 support
#
# Compile x86 binaries for all the test programs to see the correct output
#

X86_CC = gcc
X86_CFLAGS = -Wall -O2

# Subdirectory for output
X86_DIR = x86out
# Targets for everything but basic (can't compile RISC-V asm as x86)
X86_TARGETS = $(patsubst %.bin,$(X86_DIR)/%,$(filter-out basic.bin,$(TARGETS)))

x86: $(X86_DIR) $(X86_TARGETS)

$(X86_DIR):
		mkdir -p $(X86_DIR)

# The three examples that require roman.c
$(X86_DIR)/comp: $(SRC_DIR)comp.c $(SRC_DIR)roman.c $(HEADERS)
		$(X86_CC) $(X86_CFLAGS) -o $@ $< $(SRC_DIR)roman.c

# Generic rule for the rest
$(X86_DIR)/%: $(SRC_DIR)%.c $(HEADERS)
		$(X86_CC) $(X86_CFLAGS) -o $@ $<
