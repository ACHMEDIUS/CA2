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
#include <iostream>

/* Enum for opcode types */
enum class Opcode : uint8_t
{
  LUI = 0b0110111,
  AUIPC = 0b0010111,
  JAL = 0b1101111,
  JALR = 0b1100111,
  BRANCH = 0b1100011,
  LOAD = 0b0000011,
  STORE = 0b0100011,
  IMM = 0b0010011,
  REG = 0b0110011,
  MISC = 0b0001111,
  SYSTEM = 0b1110011
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

  Opcode getOpcode() const;
  uint32_t getFunct3() const;
  uint32_t getFunct7() const;
  int32_t getImmediate() const;

private:
  uint32_t instructionWord;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
