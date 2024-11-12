/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "arch.h"
#include "inst-decoder.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

std::string getImmediate(const InstructionDecoder &decoder);

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder)
{
  int32_t const instrword = decoder.getInstructionWord();

  if (instrword == 0x40ffccff)
  {
    os << "End of test marker" << std::endl;
    return os;
  }

  RegNumber const opCode = decoder.getOpcode();
  RegNumber const RS1 = decoder.getRS1();
  RegNumber const RS2 = decoder.getRS2();
  RegNumber const rd = decoder.getRD();

  InstructionType const type = decoder.getFormat();
  os << "op=" << (int)opCode << " ";

  switch (type)
  {
  case R:
    os << "R-type func=" << (int)decoder.getFunction() << " D=" << (int)rd
       << " A=" << (int)RS1 << " B=" << (int)RS2;
    break;
  case I:
    os << "I-type D=" << (int)rd << " A=" << (int)RS1
       << " imm=" << getImmediate(decoder);
    break;
  case S:
    os << "S-type A=" << (int)RS1 << " B=" << (int)RS2
       << " imm=" << getImmediate(decoder);
    break;

  case U:
    os << "U-type D=" << (int)rd << " A=" << (int)RS1
       << " imm=" << getImmediate(decoder);
    break;
  case J:
    os << "J-type imm=" << getImmediate(decoder);
    break;
  case F:

    if (opCode == 0x2F)
    { // l.sfi
      // set flags
      os << "F-type A=" << (int)RS1 << " imm=" << getImmediate(decoder)
         << " func=" << (int)decoder.getFunction();
      break;
    }

    os << "F-type A=" << (int)RS1 << " imm=" << getImmediate(decoder);
    break;
  case SH:
    os << "SH-type D=" << (int)rd << " A=" << (int)RS1
       << " Shift amount=" << (int)decoder.getShiftAmount();
    break;
  case SFR:
    os << "SFR-type func=" << (int)rd << " A=" << (int)RS1 << " B=" << (int)RS2;
    break;
  case NOP:
    os << "NOP-type func=" << (int)decoder.getFunction()
       << " imm=" << getImmediate(decoder);
    break;

  default:
    os << "Format not implemented" << std::endl;
    break;
  }

  // 0xaa20019c	op=42 U-type D=17 A=0 imm=0x19c

  return os;
}

template <typename I>
std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1)
{
  static const char *digits = "0123456789abcdef";
  std::string rc(hex_len, '0');
  for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
    rc[i] = digits[(w >> j) & 0x0f];

  // remove all leading zeros unless only one character is left
  rc.erase(0, rc.find_first_not_of('0'));
  if (rc.length() == 0)
    rc = "0";

  rc = "0x" + rc;
  return rc;
}

std::string getImmediate(const InstructionDecoder &decoder)
{
  int const RegValue = decoder.getImmediate(true);

  return n2hexstr(RegValue);
}
