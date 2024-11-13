/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "alu.h"

#include "inst-decoder.h"

#ifdef _MSC_VER
/* MSVC intrinsics */
#include <intrin.h>
#endif

ALU::ALU()
    : A(), B(), op()
{
}

RegValue
ALU::getResult()
{
  RegValue result = 0;

  switch (op)
  {
  case ALUOp::NOP:
    break;
  case ALUOp::ADD:
    return ADD();
  case ALUOp::ADD_I:
    return ADD_I();
  case ALUOp::SUB:
    return SUB();
  case ALUOp::AND:
    return AND();
  case ALUOp::OR:
    return OR();
  case ALUOp::XOR:
    return XOR();
  case ALUOp::SLL:
    return SLL();
  case ALUOp::SRL:
    return SRL();
  case ALUOp::SRA:
    return SRA();
  case ALUOp::SLT:
    return SLT();
  case ALUOp::SLT_I:
    return SLT_I();
  case ALUOp::SLTU:
    return SLTU();
  case ALUOp::SLTU_I:
    return SLTU_I();
  case ALUOp::GE:
    return GE();
  case ALUOp::NE:
    return NE();
  case ALUOp::EQ:
    return EQ();

  default:
    throw IllegalInstruction("Unimplemented or unknown ALU operation");
  }

  return result;
}

RegValue ALU::ADD() { return A + B; }

RegValue ALU::ADD_I() { return A + B; }

RegValue ALU::SUB() { return A - B; }

RegValue ALU::AND() { return A & B; }

RegValue ALU::OR() { return A | B; }

RegValue ALU::XOR() { return A ^ B; }

RegValue ALU::SLL() { return A << B; }

RegValue ALU::SRL() { return A >> B; }

RegValue ALU::SRA()
{
  return static_cast<int32_t>(A) >> B;
}

RegValue ALU::SLT() { return (static_cast<int32_t>(A) < static_cast<int32_t>(B)) ? 1 : 0; }

RegValue ALU::SLT_I()
{
  return (static_cast<int32_t>(A) < static_cast<int32_t>(B)) ? 1 : 0;
}

RegValue ALU::SLTU() { return (A < B) ? 1 : 0; }

RegValue ALU::SLTU_I()
{
  return (A < B) ? 1 : 0;
}

RegValue ALU::GE() { return (static_cast<int32_t>(A) >= static_cast<int32_t>(B)) ? 1 : 0; }

RegValue ALU::NE() { return (A != B) ? 1 : 0; }

RegValue ALU::EQ() { return (A == B) ? 1 : 0; }