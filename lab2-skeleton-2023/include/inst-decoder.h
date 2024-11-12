/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.h - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 */

#ifndef __INST_DECODER_H__
#define __INST_DECODER_H__

#include "reg-file.h"

#include <cstdint>
#include <stdexcept>

// End bit, // Size
enum InstructionBitsLocations
{
  opcode = 26,
  opcodeSize = 6,
  RS1 = 16,
  RS1_LENGTH = 5,
  RS2 = 11,
  RS2_LENGTH = 5,
  rd = 21,
  rdSize = 5,
  l = 0,
  lSize = 6
};

enum InstructionType
{
  R,
  I,
  U,
  S,
  J,
  F,
  SH,
  NOP,
  SFR
};

/* Exception that should be thrown when an illegal instruction
 * is encountered.
 */
class IllegalInstruction : public std::runtime_error
{
public:
  explicit IllegalInstruction(const std::string &what)
      : std::runtime_error(what) {}

  explicit IllegalInstruction(const char *what) : std::runtime_error(what) {}
};

/* InstructionDecoder component to be used by class Processor */
class InstructionDecoder
{
public:
  void setPC(MemAddress PC);
  void setInstructionWord(const uint32_t instructionWord);
  uint32_t getInstructionWord() const;

  RegNumber getRS1() const;
  RegNumber getRS2() const;
  RegNumber getRD() const;
  RegNumber getShiftAmount() const;

  InstructionType getFormat() const;

  /* methods to get opcode, function code */
  RegNumber getOpcode() const;

  // OpCode F-type = Opcode + D

  RegNumber getFunction() const;

  /* TODO: need a method to obtain the immediate */

  RegValue getImmediate(bool formatter = false) const;

private:
  uint32_t instructionWord;
  MemAddress PC{};

  RegValue getImmediateI(bool formatter = false, bool zeroextend = true) const;
  RegValue getImmediateS(bool formatter = false) const;
  RegValue getImmediateJ(bool formatter = false) const;
  RegValue getImmediateF(bool formatter = false) const;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
