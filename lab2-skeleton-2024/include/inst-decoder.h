/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.h - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 */

#ifndef __INST_DECODER_H__
#define __INST_DECODER_H__

#include "reg-file.h"

#include <stdexcept>
#include <cstdint>

enum InstructionLocations
{
  OPCODE = 6,
  OPCODE_SIZE = 7,
  FUNCT3 = 14,
  FUNCT3_SIZE = 3,
  FUNCT7 = 31,
  FUNCT7_SIZE = 7,
  RS1 = 19,
  RS1_SIZE = 5,
  RS2 = 24,
  RS2_SIZE = 5,
  RD = 7,
  RD_SIZE = 5,

  IMM_I_LSB = 20,
  IMM_I_SIZE = 12,
  IMM_S_LSB = 7,
  IMM_S_SIZE = 12,
  IMM_B_LSB = 8,
  IMM_B_SIZE = 13,
  IMM_U_LSB = 12,
  IMM_U_SIZE = 20,
  IMM_J_LSB = 12,
  IMM_J_SIZE = 21
};

enum InstructionType
{
  R,
  I,
  U,
  S,
  B,
  F,
  J,
  NOP,
};

/* Exception that should be thrown when an illegal instruction
 * is encountered.
 */
class IllegalInstruction : public std::runtime_error
{
public:
  explicit IllegalInstruction(const std::string &what)
      : std::runtime_error(what)
  {
  }

  explicit IllegalInstruction(const char *what)
      : std::runtime_error(what)
  {
  }
};

/* InstructionDecoder component to be used by class Processor */
class InstructionDecoder
{
public:
  void setInstructionWord(const uint32_t instructionWord);
  uint32_t getInstructionWord() const;

  RegNumber getRS1() const;
  RegNumber getRS2() const;
  RegNumber getRD() const;

  RegNumber getOpcode() const;
  RegNumber getFunct3() const;
  RegNumber getFunct7() const;

  RegValue getImmediate() const;

  InstructionType getInstructionType() const;

private:
  uint32_t instructionWord;

  RegValue getImmediateI() const;
  RegValue getImmediateS() const;
  RegValue getImmediateB() const;
  RegValue getImmediateU() const;
  RegValue getImmediateJ() const;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
