# Compile graphics test programs for CA 2022 Assignment 2 (OpenRISC)

TARGETS = gfxtest.bin mandel-gfx.bin

SRC_DIR = src/
BIN_DIR =
CRT = $(SRC_DIR)minicrt-or1k.s
HEADERS = $(wildcard $(SRC_DIR)*.h)

BIN_TARGETS = $(patsubst %.bin,$(BIN_DIR)%.bin,$(TARGETS))

CC = or1k-elf-gcc
OBJDUMP = or1k-elf-objdump
CFLAGS_COMMON = -Wall -nostdlib -fno-builtin -nodefaultlibs
CFLAGS = -Wl,-Ttext,0x10000 -Wl,-e,_start $(CFLAGS_COMMON)


.phony: all clean

all:	$(TARGETS)


gfxtest.bin:	$(SRC_DIR)gfxtest.c $(HEADERS)
		$(CC) $(CFLAGS) -O1 -o $@ $< $(CRT)

mandel-gfx.bin:	$(SRC_DIR)mandel-gfx.c $(HEADERS)
		$(CC) $(CFLAGS) -Og -finline-functions-called-once -mhard-float -o $@ $< $(CRT)


%.dump.txt:	%.bin
		$(OBJDUMP) -D $< > $@


clean:
		rm -f $(BIN_TARGETS)
