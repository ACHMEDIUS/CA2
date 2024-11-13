/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ALU_H__
#define __ALU_H__

#include "arch.h"
#include "inst-decoder.h"

#include <map>

enum class ALUOp
{
  NOP,
  ADD,
  ADD_I,
  SUB,
  AND,
  OR,
  XOR,
  JUMP,
  SLL,
  SRL,
  SRA,
  SLT,
  SLT_I,
  SLTU,
  SLTU_I,
  GE,
  NE,
  EQ,
};

/* The ALU component performs the specified operation on operands A and B
 * when asked to propagate the result. The operation is specified through
 * the ALUOp.
 */
class ALU
{
public:
  ALU();

  void setA(RegValue A) { this->A = A; }
  void setB(RegValue B) { this->B = B; }

  RegValue getResult();

  void setOp(ALUOp op) { this->op = op; }

private:
  RegValue A;
  RegValue B;

  ALUOp op;

  RegValue ADD();
  RegValue ADD_I();
  RegValue SUB();
  RegValue AND();
  RegValue OR();
  RegValue XOR();
  RegValue SLL();
  RegValue SRL();
  RegValue SRA();
  RegValue SLT();
  RegValue SLT_I();
  RegValue SLTU();
  RegValue SLTU_I();
  RegValue GE();
  RegValue NE();
  RegValue EQ();
};

#endif /* __ALU_H__ */
