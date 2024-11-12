# Compiler
CXX = c++
CXXFLAGS = -std=c++17 -Wall -Weffc++ -g -Og -Iinclude

# Linker flags:
LDFLAGS = -lstdc++fs

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Source files in 'src/' directory
SOURCES = \
	$(SRC_DIR)/alu.cc \
	$(SRC_DIR)/config-file.cc \
	$(SRC_DIR)/control-signal-controller.cc \
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

ifdef ENABLE_FRAMEBUFFER
OBJECTS += $(OBJECTS_FB)
HEADERS += $(HEADERS_FB)

# Compiler and linker flags for SDL2 when framebuffer is enabled
CXXFLAGS += -DENABLE_FRAMEBUFFER $(shell pkg-config --cflags sdl2)
LDFLAGS  += $(shell pkg-config --libs sdl2)
endif

all: $(OBJ_DIR) rv64-emu

# Rule to create the 'obj/' directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

rv64-emu: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(ALL_OBJECTS) $(LDFLAGS)

# Pattern rule to compile '.cc' files in 'src/' into '.o' files in 'obj/'
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile 'main.cc' from project root into 'obj/main.o'
$(MAIN_OBJECT): main.cc $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c main.cc -o $(MAIN_OBJECT)

clean:
	rm -f rv64-emu
	rm -f $(ALL_OBJECTS)
	rm -rf $(OBJ_DIR)

check: rv64-emu
	./test_instructions.py