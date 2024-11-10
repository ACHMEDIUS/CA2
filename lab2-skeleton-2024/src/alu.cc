/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "alu.h"

#include "arch.h"
#include "inst-decoder.h"
#include <cstdint>

#include <iostream>

#ifdef _MSC_VER
/* MSVC intrinsics */
#include <intrin.h>
#endif

ALU::ALU() : A(), B(), op() {}

RegValue ALU::getResult()
{
  RegValue result = 0;

  switch (op)
  {
  case ALUOp::ADD:
  case ALUOp::JUMP:
  case ALUOp::NOP:
  case ALUOp::SB:
  case ALUOp::SW:
  case ALUOp::SDW:
  case ALUOp::LB:
  case ALUOp::LW:
  case ALUOp::LDW:
    result = ALU::ADD();
    break;

  case ALUOp::SUB:
    result = ALU::SUB();
    break;

  case ALUOp::AND:
    result = ALU::AND();
    break;

  case ALUOp::OR:
    result = ALU::OR();
    break;

  case ALUOp::XOR:
    result = ALU::XOR();
    break;

  case ALUOp::SLL:
    result = ALU::SLL();
    break;

  case ALUOp::SRL:
    result = ALU::SRL();
    break;

  case ALUOp::SRA:
    result = ALU::SRA();
    break;

  case ALUOp::GE:
    result = ALU::GE();
    break;

  case ALUOp::NE:
    result = ALU::NE();
    break;

  case ALUOp::LT:
    result = ALU::LT();
    break;

  case ALUOp::LE:
    result = ALU::LE();
    break;

  case ALUOp::EQ:
    result = ALU::EQ();
    break;

  case ALUOp::GT:
    result = ALU::GT();
    break;

  default:
    throw IllegalInstruction("Unimplemented or unknown ALU operation");
  }

  return result;
}

RegValue ALU::ADD() { return A + B; }

RegValue ALU::SUB() { return A - B; }

RegValue ALU::AND() { return A & B; }

RegValue ALU::OR() { return A | B; }

RegValue ALU::XOR() { return A ^ B; }

RegValue ALU::SLL() { return A << B; }

RegValue ALU::SRL() { return A >> B; }

RegValue ALU::SRA() { return static_cast<int32_t>(A) >> B; }

RegValue ALU::GE() { return A >= B; }

RegValue ALU::NE() { return A != B; }

RegValue ALU::LT() { return A < B; }

RegValue ALU::LE() { return A <= B; }

RegValue ALU::EQ() { return A == B; }

RegValue ALU::GT() { return A > B; }
