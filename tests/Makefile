#
# rv64-emu -- Simple 64-bit RISC-V simulator
#
# Copyright (C) 2017  Leiden University, The Netherlands.
#

# Note on the compile command: we explicitly specify addresses for the
# text and data segments: some of our tests rely on it and the OpenRISC
# toolchain seems to sometimes change these.

%.bin:		%.s
		or1k-elf-gcc -Ttext=0x10000 -Tdata=0x11100 \
			-Wl,-e,_start -Wall -O0 \
			-nostdlib -fno-builtin -nodefaultlibs -o $@ $<
