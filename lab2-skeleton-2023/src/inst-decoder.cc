/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */

#include "inst-decoder.h"
#include "arch.h"

// #include <map>
#include <cstdint>
#include <iostream>

/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

void InstructionDecoder::setInstructionWord(const uint32_t instructionWord)
{
  this->instructionWord = instructionWord;
}

void InstructionDecoder::setPC(MemAddress PC) { this->PC = PC; }

uint32_t InstructionDecoder::getInstructionWord() const
{
  return instructionWord;
}

RegNumber InstructionDecoder::getRS1() const
{ // OpenRisc Cheat Sheet
  RegNumber const res = (instructionWord >> RS1) & ((1 << RS1_LENGTH) - 1);

  return res;
}

RegNumber InstructionDecoder::getRS2() const
{ // OpenRisc Cheat Sheet
  RegNumber const res = (instructionWord >> RS2) & ((1 << RS2_LENGTH) - 1);

  return res;
}

// OpCode |  Rd   |   RS1  |   RS2  |Res| Op2 | Res  | Op3
// 001000 | 00000 | 00001 | 00000 | 0 | 11  | 0000 | 0010

RegNumber InstructionDecoder::getRD() const
{ // OpenRisc Cheat Sheet

  if (getOpcode() == 0x1)
  {
    return 9; // link register
  }
  if (getFormat() == J)
  {
    return 0;
  }

  RegNumber const res = (instructionWord >> rd) & ((1 << rdSize) - 1);

  return res;
}

RegNumber InstructionDecoder::getOpcode() const
{ // OpenRisc Cheat Sheet

  RegNumber const res = (instructionWord >> opcode) & ((1 << opcodeSize) - 1);

  return res;
}

RegNumber InstructionDecoder::getFunction() const
{

  InstructionType const format = getFormat();

  if (format == NOP)
  {
    return 1;
  }

  if (format == SFR || format == F)
  {
    return getRD();
  }

  if (format == S || format == U || format == J)
  {
    // With S/U, no func (for now)
    return 0;
  }

  RegNumber const opc = getOpcode();
  RegNumber func = (instructionWord >> 0) & ((1 << 4) - 1);

  switch (opc)
  {
  case 0x38: // l.sub and or etc etc (like 20 instructions)
    if (func == 0b1100 || func == 0b1101 || func == 0b1000)
    {
      // extend op with 4 bits (9-6)
      RegNumber const cust = (instructionWord >> 6) & ((1 << 4) - 1);

      func |= (cust << 4) | func;
    }
    else
    {
      break;
    }
    break;

  case 0x11:
    return 0;

  case 0x06:
    // return single bit (last of A)
    return getRS1() & 1;

  default:
    if (format == I)
    {
      return 0;
    }

    std::cout << "Error: Instruction func not implemented: " << std::hex
              << (int)opc << std::endl;
    return 0;
  }

  return func;
}

RegNumber InstructionDecoder::getShiftAmount() const
{ // OpenRisc Cheat Sheet
  RegNumber const res = (instructionWord >> l) & ((1 << lSize) - 1);

  return res;
}

RegValue InstructionDecoder::getImmediateI(
    bool formatter, bool zeroextend) const
{ // OpenRisc Cheat Sheet
  RegValue res = (instructionWord) & ((1 << 16) - 1);

  // Sign extend
  if (res >> 15 == 1 && !formatter && zeroextend)
  {
    res |= 0xFFFF0000;
  }

  return res;
}

RegValue InstructionDecoder::getImmediateS(
    bool formatter) const
{ // OpenRisc Cheat Sheet
  RegValue const imm10_0 = (instructionWord) & ((1 << 11) - 1);
  RegValue const imm25_21 = (instructionWord >> rd) & ((1 << rdSize) - 1);

  RegValue res = (imm25_21 << 11) | imm10_0;

  // Sign extend
  if (res >> 15 == 1 && !formatter)
  {
    res |= 0xFFFF0000;
  }

  return res;
}

RegValue InstructionDecoder::getImmediateJ(
    bool formatter) const
{ // OpenRisc Cheat Sheet
  RegValue res = (instructionWord) & ((1 << 26) - 1);

  // std::cout << "Immediate: " << std::hex << res << std::endl;
  // std::cout << std::endl << "Immediate" << (int)res << std::endl;

  res <<= 2;

  // std::cout << "Immediate: " << std::hex << res << std::endl;

  // Sign extend
  if (res >> 27 == 1 && !formatter)
  {
    res |= 0xF0000000;
  }

  // std::cout << "Immediate: " << std::hex << (res | 0xF0000000) << std::endl;

  return res;
}

RegValue InstructionDecoder::getImmediateF(
    bool formatter) const
{ // OpenRisc Cheat Sheet
  RegValue res = (instructionWord) & ((1 << 16) - 1);

  // Sign extend
  if (res >> 15 == 1 && !formatter)
  {
    res |= 0xFFFF0000;
  }

  return res;
}

RegValue InstructionDecoder::getImmediate(bool formatter) const
{
  InstructionType const format = getFormat();
  switch (format)
  {
  case I:
    return getImmediateI(formatter, true);
  case S:
    return getImmediateS(formatter);
  case J:
    return getImmediateJ(formatter);
  case F:
    return getImmediateF(formatter);
  case U:
    return getImmediateI(formatter, false);

  case R:
  case NOP:
  case SH:
  case SFR:
    // Has no immediate
    return 0;

  default:
    std::cout
        << "Error getting immediate: Instruction format not found/implemented"
        << std::endl;
    return 0;
  }
}

InstructionType InstructionDecoder::getFormat() const
{
  RegNumber const opcode = getOpcode();
  switch (opcode)
  {
  case 0x00: // l.j
  case 0x01: // l.jal
  // case 0x03: // l.bnf
  case 0x04: // l.bf

    return J; // Jump

  case 0x05:    // l.nop
    return NOP; // No operation

  case 0x06:  // l.movhi
  case 0x27:  // l.addi
  case 0x21:  // l.lwz
  case 0x23:  // l.lbz
    return I; // Immediate (signed)

  case 0x2A:  // l.ori
  case 0x29:  // l.andi
    return U; // Immediate (unsigned)

  case 0x36:  // l.sb
  case 0x35:  // l.sw
  case 0x37:  // l.sh
    return S; // Store

  case 0x11:  // l.jr
  case 0x38:  // l.sub, l.add, l.and, l.or, l.xor, l.sll, l.srl, l.sra
    return R; // Register

  case 0x39:    // l.sf (set flag)
    return SFR; // Set Flag Register

  case 0x2F:  // l.sfi (set flag immediate)
    return F; // Set Flag Immediate

  case 0x2E:   // l.slli / l.srli (shift left / right immediate)
    return SH; // Shift Immediate

  default:
    std::cout << "Error: Instruction not implemented: " << (int)getOpcode()
              << std::endl;
    return NOP;
    break;
  }
}