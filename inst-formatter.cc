/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"
#include <functional>
#include <map>
#include <iostream>

// Define the ostream operator to print the InstructionDecoder object
std::ostream& operator<<(std::ostream& os, const InstructionDecoder& decoder)
{
  os << "Instruction: 0x" << std::hex << decoder.getInstructionWord() << std::dec << "\n";
  os << "Opcode: " << decoder.getOpcode() << "\n";
  os << "Funct3: " << decoder.getFunct3() << "\n";
  os << "Funct7: " << decoder.getFunct7() << "\n";
  os << "Format: " << decoder.getFormat() << "\n";
  os << "Immediate: " << decoder.getImmediate() << "\n";
  os << "Register A: " << decoder.getA() << "\n";
  os << "Register B: " << decoder.getB() << "\n";
  os << "Register D: " << decoder.getD() << "\n";
  return os;
}
