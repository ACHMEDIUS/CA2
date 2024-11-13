/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */

#include "inst-decoder.h"

#include <map>

/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

void InstructionDecoder::setInstructionWord(const uint32_t instructionWord)
{
  this->instructionWord = instructionWord;
}

uint32_t
InstructionDecoder::getInstructionWord() const
{
  return instructionWord;
}
// Helper function to extract a field from the instruction word
template <typename T>
T extractField(uint32_t instructionWord, uint8_t startBit, uint8_t size)
{
  T mask = (1 << size) - 1;
  return static_cast<T>((instructionWord >> startBit) & mask);
}

RegNumber InstructionDecoder::getRS1() const
{
  return extractField<RegNumber>(instructionWord, RS1, RS1_SIZE);
}

RegNumber InstructionDecoder::getRS2() const
{
  return extractField<RegNumber>(instructionWord, RS2, RS2_SIZE);
}

RegNumber InstructionDecoder::getRD() const
{
  return extractField<RegNumber>(instructionWord, RD, RD_SIZE);
}

RegNumber InstructionDecoder::getOpcode() const
{
  return extractField<RegNumber>(instructionWord, OPCODE, OPCODE_SIZE);
}

RegNumber InstructionDecoder::getFunct3() const
{
  return extractField<RegNumber>(instructionWord, FUNCT3, FUNCT3_SIZE);
}

RegNumber InstructionDecoder::getFunct7() const
{
  return extractField<RegNumber>(instructionWord, FUNCT7, FUNCT7_SIZE);
}

// Helper functions to extract immediates for different instruction types
RegValue InstructionDecoder::getImmediateI() const
{
  int32_t imm = extractField<int32_t>(instructionWord, IMM_I_LSB, IMM_I_SIZE);
  // Sign-extend the immediate value
  if (imm & (1 << 11))
  {                    // Check the most significant bit
    imm |= 0xFFFFF000; // Set the upper bits to 1
  }
  return imm;
}

RegValue InstructionDecoder::getImmediateS() const
{
  int32_t imm = extractField<int32_t>(instructionWord, IMM_S_LSB, IMM_S_SIZE);
  imm |= (extractField<int32_t>(instructionWord, RS2, RS2_SIZE) << 5);
  // Sign-extend the immediate value
  if (imm & (1 << 11))
  {                    // Check the most significant bit
    imm |= 0xFFFFF000; // Set the upper bits to 1
  }
  return imm;
}

RegValue InstructionDecoder::getImmediateB() const
{
  int32_t imm = 0;
  imm |= extractField<int32_t>(instructionWord, IMM_B_LSB, 4) << 1;
  imm |= extractField<int32_t>(instructionWord, FUNCT3, 1) << 5;
  imm |= extractField<int32_t>(instructionWord, RS1, 4) << 6;
  imm |= extractField<int32_t>(instructionWord, RS2, 1) << 11;
  imm |= extractField<int32_t>(instructionWord, 31, 1) << 12;
  // Sign-extend the immediate value
  if (imm & (1 << 12))
  {                    // Check the most significant bit
    imm |= 0xFFFFE000; // Set the upper bits to 1
  }
  return imm;
}

RegValue InstructionDecoder::getImmediateU() const
{
  return extractField<RegValue>(instructionWord, IMM_U_LSB, IMM_U_SIZE) << 12;
}

RegValue InstructionDecoder::getImmediateJ() const
{
  int32_t imm = 0;
  imm |= extractField<int32_t>(instructionWord, IMM_J_LSB, 8) << 1;
  imm |= extractField<int32_t>(instructionWord, 25, 1) << 11;
  imm |= extractField<int32_t>(instructionWord, RS1, 4) << 12;
  imm |= extractField<int32_t>(instructionWord, 20, 1) << 20;
  imm |= extractField<int32_t>(instructionWord, 31, 1) << 21;
  // Sign-extend the immediate value
  if (imm & (1 << 20))
  {                    // Check the most significant bit
    imm |= 0xFFE00000; // Set the upper bits to 1
  }
  return imm;
}

RegValue InstructionDecoder::getImmediate() const
{
  switch (getInstructionType())
  {
  case I:
    return getImmediateI();
  case S:
    return getImmediateS();
  case B:
    return getImmediateB();
  case U:
    return getImmediateU();
  case J:
    return getImmediateJ();
  default: // R-type has no immediate
    return 0;
  }
}

InstructionType InstructionDecoder::getInstructionType() const
{
  switch (getOpcode())
  {
  case 0b0110011: // R-type
    return R;
  case 0b0010011: // I-type arithmetic
  case 0b0000011: // I-type load
  case 0b1100111: // I-type jalr
    return I;
  case 0b0100011: // S-type
    return S;
  case 0b1100011: // B-type
    return B;
  case 0b0110111: // U-type lui
  case 0b0010111: // U-type auipc
    return U;
  case 0b1101111: // J-type jal
    return J;
  default:
    throw IllegalInstruction("Unknown opcode encountered.");
  }
}