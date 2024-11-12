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
  SUB,
  AND,
  OR,
  XOR,
  JUMP,
  LS,
  RS,
  RSA,
  SB,
  SW,
  SDW,
  LB,
  LW,
  LDW,
  GE,
  NE,
  LT,
  LE,
  EQ,
  GT
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

  // Operations
  RegValue ADD(); // Addition (A + B)
  RegValue SUB(); // Subtraction (A - B)
  RegValue AND(); // Bitwise AND (A & B)
  RegValue OR();  // Bitwise OR (A | B)
  RegValue XOR(); // Bitwise XOR (A ^ B)
  RegValue LS();  // Left shift (A << B)
  RegValue RS();  // Right shift (A >> B)
  RegValue RSA(); // Right shift arithmetic (A >> B)
  RegValue GE();  // Branch if greater than or equal to (A >= B)
  RegValue NE();  // Branch if not equal (A != B)
  RegValue LT();  // Branch if less than (A < B)
  RegValue LE();  // Branch if less than or equal to (A <= B)
  RegValue EQ();  // Branch if equal (A == B)
  RegValue GT();  // Branch if greater than (A > B)
};

#endif /* __ALU_H__ */
