#
# rv64-emu -- Simple 64-bit RISC-V simulator
#
# Copyright (C) 2016,2019  Leiden University, The Netherlands.
#

# Compiler
CXX = c++

# Compiler flags:
# - std=c++17: Use C++17 standard
# - Wall: Enable all warnings
# - Weffc++: Enable warnings about violations of the C++ Core Guidelines
# - g: Include debugging information
# - Og: Optimize for debugging
# - -Iinclude: Add 'include/' to the list of directories to search for header files
CXXFLAGS = -std=c++17 -Wall -Weffc++ -g -Og -Iinclude

# Linker flags:
# - Removed '-lstdc++fs' as it's unnecessary for modern compilers (GCC 9+, Clang 7+)
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Source files in 'src/' directory
SOURCES = \
	$(SRC_DIR)/alu.cc \
	$(SRC_DIR)/config-file.cc \
	$(SRC_DIR)/csc.cc \
	$(SRC_DIR)/elf-file.cc \
	$(SRC_DIR)/inst-decoder.cc \
	$(SRC_DIR)/inst-formatter.cc \
	$(SRC_DIR)/memory.cc \
	$(SRC_DIR)/memory-bus.cc \
	$(SRC_DIR)/memory-control.cc \
	$(SRC_DIR)/pipeline.cc \
	$(SRC_DIR)/processor.cc \
	$(SRC_DIR)/serial.cc \
	$(SRC_DIR)/stages.cc \
	$(SRC_DIR)/sys-status.cc \
	$(SRC_DIR)/testing.cc

# Object files corresponding to source files
OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(SOURCES))

# Object file for 'main.cc' located in project root
MAIN_OBJECT = $(OBJ_DIR)/main.o

# All object files
ALL_OBJECTS = $(OBJECTS) $(MAIN_OBJECT)

# Additional object and header files for framebuffer support
OBJECTS_FB = $(OBJ_DIR)/framebuffer.o
HEADERS_FB = $(INC_DIR)/framebuffer.h

# All header files in 'include/' directory
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Conditional compilation for framebuffer support
ifdef ENABLE_FRAMEBUFFER
OBJECTS += $(OBJECTS_FB)
HEADERS += $(HEADERS_FB)

# Compiler and linker flags for SDL2 when framebuffer is enabled
CXXFLAGS += -DENABLE_FRAMEBUFFER $(shell pkg-config --cflags sdl2)
LDFLAGS  += $(shell pkg-config --libs sdl2)
endif

# Default target: build the emulator executable
all: $(OBJ_DIR) rv64-emu

# Rule to create the 'obj/' directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to link object files into the final executable
rv64-emu: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(ALL_OBJECTS) $(LDFLAGS)

# Pattern rule to compile '.cc' files in 'src/' into '.o' files in 'obj/'
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile 'main.cc' from project root into 'obj/main.o'
$(MAIN_OBJECT): main.cc $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c main.cc -o $(MAIN_OBJECT)

# Clean target: remove the executable and all object files
clean:
	rm -f rv64-emu
	rm -f $(ALL_OBJECTS)
	rm -rf $(OBJ_DIR)

# Check target: build the emulator and run tests
check: rv64-emu
	./test_instructions.py
