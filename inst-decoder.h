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

static const int INSTRUCTION_SIZE = 4;

enum Opcode
{
  OP_LUI = 0b0110111,
  OP_AUIPC = 0b0010111,
  OP_JAL = 0b1101111,
  OP_JALR = 0b1100111,
  OP_BRANCH = 0b1100011,
  OP_LOAD = 0b0000011,
  OP_STORE = 0b0100011,
  OP_OP_IMM = 0b0010011,
  OP_OP = 0b0110011,
  OP_MADD = 0b1000011,
  OP_SYSTEM = 0b1110011
};

enum Funct3
{
  F3_ADD = 0b000,
  F3_SLL = 0b001,
  F3_SLT = 0b010,
  F3_SLTU = 0b011,
  F3_XOR = 0b100,
  F3_SRL = 0b101,
  F3_SRA = 0b101,
  F3_OR = 0b110,
  F3_AND = 0b111,
  F3_BEQ = 0b000,
  F3_BNE = 0b001,
  F3_BLT = 0b100,
  F3_BGE = 0b101,
  F3_BLTU = 0b110,
  F3_BGEU = 0b111,
  F3_LB = 0b000,
  F3_LH = 0b001,
  F3_LW = 0b010,
  F3_LBU = 0b100,
  F3_LHU = 0b101,
  F3_SB = 0b000,
  F3_SH = 0b001,
  F3_SW = 0b010,
  F3_ADDI = 0b000,
  F3_SLTI = 0b010,
  F3_SLTIU = 0b011,
  F3_XORI = 0b100,
  F3_ORI = 0b110,
  F3_ANDI = 0b111,
  F3_SLLI = 0b001,
  F3_SRLI = 0b101,
  F3_SRAI = 0b101,
  F3_MUL = 0b000,
  F3_MULH = 0b001,
  F3_MULHSU = 0b010,
  F3_MULHU = 0b011,
  F3_DIV = 0b100,
  F3_DIVU = 0b101,
  F3_REM = 0b110,
  F3_REMU = 0b111
};

enum Funct7
{
  F7_SRLI = 0b0000000,
  F7_SRAI = 0b0100000,
  F7_ADD = 0b0000000,
  F7_SUB = 0b0100000,
  F7_SLL = 0b0000000,
  F7_MUL = 0b0000001,
  F7_MULH = 0b0000001,
  F7_MULHSU = 0b0000101,
  F7_MULHU = 0b0000101,
  F7_DIV = 0b0001001,
  F7_DIVU = 0b0001101,
  F7_REM = 0b0010001,
  F7_REMU = 0b0010101,
  F7_OR = 0b0000000,
  F7_AND = 0b0000000,
  F7_XOR = 0b0000000,
  F7_SRL = 0b0000000,
  F7_SRA = 0b0100000
};

enum Format
{
  R_FORMAT,
  I_FORMAT,
  S_FORMAT,
  B_FORMAT,
  U_FORMAT,
  J_FORMAT,
  UNKNOWN_FORMAT
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

  RegValue getA() const;
  RegValue getB() const;
  RegValue getD() const;

  Opcode getOpcode() const;
  Funct3 getFunct3() const;
  Funct7 getFunct7() const;
  int32_t getImmediate() const;
  Format getFormat() const;

private:
  uint32_t instructionWord;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
