/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "arch.h"
#include "inst-decoder.h"

#include <cstddef>
#include <functional>
#include <map>
#include <iostream>
#include <string>

// Helper function to convert an integer to a hexadecimal string
template <typename I>
std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1)
{
  static const char *digits = "0123456789abcdef";
  std::string rc(hex_len, '0');
  for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
    rc[i] = digits[(w >> j) & 0x0f];
  rc.erase(0, rc.find_first_not_of('0'));
  if (rc.length() == 0)
    rc = "0";
  return "0x" + rc;
}

// Helper function to get the immediate value as a hexadecimal string
std::string getImmediate(const InstructionDecoder &decoder)
{
  return n2hexstr(decoder.getImmediate());
}

std::ostream &
operator<<(std::ostream &os, const InstructionDecoder &decoder)
{
  uint32_t const instrword = decoder.getInstructionWord();

  // Check for end-of-test marker
  if (instrword == 0x40ffccff)
  {
    os << "End of test marker" << std::endl;
    return os;
  }

  RegNumber const opcode = decoder.getOpcode();
  RegNumber const rs1 = decoder.getRS1();
  RegNumber const rs2 = decoder.getRS2();
  RegNumber const rd = decoder.getRD();
  RegNumber const func3 = decoder.getFunct3();
  RegNumber const func7 = decoder.getFunct7();

  InstructionType const type = decoder.getInstructionType();

  os << n2hexstr(instrword) << "\t"
     << "op=" << (int)opcode << " ";

  switch (type)
  {
  case R:
    switch (func3)
    {
    case 0x0:
      if (func7 == 0x00)
      {
        os << "add ";
      }
      else if (func7 == 0x20)
      {
        os << "sub ";
      }
      else
      {
        os << "Unknown R-type instruction (func3=0x0, func7=" << (int)func7 << ")";
      }
      break;
    case 0x1:
      os << "sll ";
      break;
    case 0x2:
      os << "slt ";
      break;
    case 0x3:
      os << "sltu ";
      break;
    case 0x4:
      os << "xor ";
      break;
    case 0x5:
      if (func7 == 0x00)
      {
        os << "srl ";
      }
      else if (func7 == 0x20)
      {
        os << "sra ";
      }
      else
      {
        os << "Unknown R-type instruction (func3=0x5, func7=" << (int)func7 << ")";
      }
      break;
    case 0x6:
      os << "or ";
      break;
    case 0x7:
      os << "and ";
      break;
    default:
      os << "Unknown R-type instruction (func3=" << (int)func3 << ")";
      break;
    }
    os << "rd=" << (int)rd << " rs1=" << (int)rs1 << " rs2=" << (int)rs2;
    break;
  case I:
    switch (func3)
    {
    case 0x0:
      os << "addi ";
      break;
    case 0x1:
      os << "slli ";
      break;
    case 0x2:
      os << "slti ";
      break;
    case 0x3:
      os << "sltiu ";
      break;
    case 0x4:
      os << "xori ";
      break;
    case 0x5:
      if (func7 == 0x00)
      {
        os << "srli ";
      }
      else if (func7 == 0x20)
      {
        os << "srai ";
      }
      else
      {
        os << "Unknown I-type instruction (func3=0x5, func7=" << (int)func7 << ")";
      }
      break;
    case 0x6:
      os << "ori ";
      break;
    case 0x7:
      os << "andi ";
      break;
    // ... (add cases for I-type load instructions: lb, lh, lw, lbu, lhu)
    default:
      os << "Unknown I-type instruction (func3=" << (int)func3 << ")";
      break;
    }
    os << "rd=" << (int)rd << " rs1=" << (int)rs1 << " imm=" << getImmediate(decoder);
    break;
  case S:
    switch (func3)
    {
    case 0x0:
      os << "sb ";
      break;
    case 0x1:
      os << "sh ";
      break;
    case 0x2:
      os << "sw ";
      break;
    default:
      os << "Unknown S-type instruction (func3=" << (int)func3 << ")";
      break;
    }
    os << "rs1=" << (int)rs1 << " rs2=" << (int)rs2 << " imm=" << getImmediate(decoder);
    break;
  case B:
    switch (func3)
    {
    case 0x0:
      os << "beq ";
      break;
    case 0x1:
      os << "bne ";
      break;
    case 0x4:
      os << "blt ";
      break;
    case 0x5:
      os << "bge ";
      break;
    case 0x6:
      os << "bltu ";
      break;
    case 0x7:
      os << "bgeu ";
      break;
    default:
      os << "Unknown B-type instruction (func3=" << (int)func3 << ")";
      break;
    }
    os << "rs1=" << (int)rs1 << " rs2=" << (int)rs2 << " imm=" << getImmediate(decoder);
    break;
  case U:
    switch (opcode)
    {
    case 0b0110111:
      os << "lui ";
      break;
    case 0b0010111:
      os << "auipc ";
      break;
    default:
      os << "Unknown U-type instruction (opcode=" << (int)opcode << ")";
      break;
    }
    os << "rd=" << (int)rd << " imm=" << getImmediate(decoder);
    break;
  case J:
    os << "jal rd=" << (int)rd << " imm=" << getImmediate(decoder);
    break;
  case F:           // Assuming this is for fence instructions
    os << "fence "; // Add func3 if needed
    break;
  case NOP: // Assuming this is for NOP instructions
    os << "nop ";
    break;
  default:
    os << "Unknown instruction type";
    break;
  }

  return os;
}
