/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"
#include <iostream>

std::ostream &
operator<<(std::ostream &os, const InstructionDecoder &decoder)
{
  uint32_t instructionWord = decoder.GetInstructionWord();

  // Print the raw instruction word in hexadecimal format
  os << "Instruction: 0x" << std::hex << instructionWord;

  try {
    // Print opcode and basic instruction fields, ensuring only public methods are used
    os << " | Opcode: 0x" << std::hex << decoder.GetOpcode();

    // Print relevant registers if accessible
    os << " | DReg: x" << static_cast<int>(decoder.GetD());
    os << " | AReg: x" << static_cast<int>(decoder.GetA());
    os << " | BReg: x" << static_cast<int>(decoder.GetB());

    // Print immediate value if applicable (using public access methods)
    os << " | Immediate: 0x" << std::hex << decoder.GetImmediate(FImmediateProperties());
  } catch (const IllegalInstruction &e) {
    // Handle cases where the instruction is not valid
    os << " | [Illegal Instruction: " << e.what() << "]";
  }

  return os;
}
