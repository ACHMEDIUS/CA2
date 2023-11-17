/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */

#include "inst-decoder.h"
#include <iomanip> // Include the iomanip header


void InstructionDecoder::setInstructionWord(const uint32_t instructionWord)
{
  this->instructionWord = instructionWord;
}

uint32_t InstructionDecoder::getInstructionWord() const
{
  return instructionWord;
}

RegValue InstructionDecoder::getA() const
{
  if (getFormat() == R_FORMAT || getFormat() == I_FORMAT || getFormat() == S_FORMAT || getFormat() == B_FORMAT) {
    return static_cast<RegValue>((instructionWord >> 15) & 0x1F);
  }
  return 0; 
}

RegValue InstructionDecoder::getB() const
{
  if (getFormat() == R_FORMAT || getFormat() == S_FORMAT || getFormat() == B_FORMAT) {
    return static_cast<RegValue>((instructionWord >> 20) & 0x1F);
  }
  return 0; 
}

RegValue InstructionDecoder::getD() const
{
  if (getFormat() == R_FORMAT) {
    return static_cast<RegValue>((instructionWord >> 7) & 0x1F);
  }
  return 0; 
}

Opcode InstructionDecoder::getOpcode() const
{
  return static_cast<Opcode>(instructionWord & 0x7F);
}

Funct3 InstructionDecoder::getFunct3() const
{
  return static_cast<Funct3>((instructionWord >> 12) & 0x7);
}

Funct7 InstructionDecoder::getFunct7() const
{
  return static_cast<Funct7>((instructionWord >> 25) & 0x7F);
}

int32_t InstructionDecoder::getImmediate() const
{
  // Assuming I-type instruction
  if (getFormat() == I_FORMAT) {
    return static_cast<int32_t>(instructionWord & 0xFFF00000) >> 20;
  }

  // Assuming S-type instruction
  if (getFormat() == S_FORMAT) {
    int32_t imm12 = (instructionWord & 0xFE000000) >> 20;
    int32_t imm11to5 = (instructionWord >> 7) & 0x1FE0;
    return imm12 | imm11to5;
  }

  // Assuming B-type instruction
  if (getFormat() == B_FORMAT) {
    int32_t imm12 = (instructionWord & 0x80000000) >> 19;
    int32_t imm11 = (instructionWord & 0x80) << 4;
    int32_t imm10to5 = (instructionWord >> 25) & 0x7E0;
    int32_t imm4to1 = (instructionWord >> 8) & 0xF;
    return imm12 | imm11 | imm10to5 | imm4to1;
  }

  // Assuming U-type instruction
  if (getFormat() == U_FORMAT) {
    return static_cast<int32_t>(instructionWord & 0xFFFFF000);
  }

  // Assuming J-type instruction
  if (getFormat() == J_FORMAT) {
    int32_t imm20 = (instructionWord & 0x80000000) >> 11;
    int32_t imm19to12 = (instructionWord & 0xFF000) >> 20;
    int32_t imm11 = (instructionWord & 0x100000) >> 9;
    int32_t imm10to1 = (instructionWord & 0x7FE00000) >> 20;
    return imm20 | imm19to12 | imm11 | imm10to1;
  }

  return 0; 
}

Format InstructionDecoder::getFormat() const
{
  uint32_t opcode = instructionWord & 0x7F;
  switch (opcode) {
    case OP_LUI:
    case OP_AUIPC:
      return U_FORMAT;
    case OP_JAL:
      return J_FORMAT;
    case OP_JALR:
      return I_FORMAT;
    case OP_BRANCH:
      return B_FORMAT;
    case OP_LOAD:
    case OP_OP_IMM:
      return I_FORMAT;
    case OP_STORE:
      return S_FORMAT;
    case OP_OP:
      return R_FORMAT;
    default:
      return UNKNOWN_FORMAT;
  }
}