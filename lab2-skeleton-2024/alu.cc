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
        result = A + B;
        break;

      case ALUOp::SUB:
        result = A - B;
        break;

      case ALUOp::AND:
        result = A & B;
        break;

      case ALUOp::OR:
        result = A | B;
        break;

      case ALUOp::XOR:
        result = A ^ B;
        break;

      case ALUOp::SLL:
        result = A << (B & 0x3F);  
        break;

      case ALUOp::SRL:
        result = static_cast<uint64_t>(A) >> (B & 0x3F);  
        break;

      case ALUOp::SRA:
        result = static_cast<int64_t>(A) >> (B & 0x3F);  
        break;

      default:
        throw IllegalInstruction("Unimplemented or unknown ALU operation");
    }

  return result;
}
