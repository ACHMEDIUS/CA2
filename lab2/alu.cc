/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "alu.h"

#ifdef _MSC_VER
/* MSVC intrinsics */
#include <intrin.h>
#endif


ALU::ALU()
  : A(), B(), Op_(), Carry_(false), SignedOverflow_(false), ShiftValues_()
{
}


RegValue
ALU::getResult()
{
  RegValue result = 0;

  switch (Op_)
  {
  case EAluOp::NOP:
    break;
  case EAluOp::ADD:
    if (UINT32_MAX - A < B)//unsigned overflow: adding negative ints or big unsigned ints
    {
      Carry_ = true;
    } else if ((INT32_MAX < (int32_t)A + (int32_t)B))//signed overflow: positive numbers become negative
    {
      SignedOverflow_ = true;
    }
    result = A + B;
    break;
  case EAluOp::SUB:
    if (UINT32_MAX - A < (-1* B))
    {
      Carry_ = true;
    } else if ((INT32_MAX < (int32_t)A - (int32_t)B)) //signed overflow: positive numbers become negative
    {
      SignedOverflow_ = true;
    }
    result = A - B;
    break;
  case EAluOp::SHIFTL:
    result = A << B;
    break;
  case EAluOp::SHIFTR:
    result = A >> B;
    break;
  case EAluOp::SEXTSHIFTR:
    //size of result is: 32-B(max 31) bits, turn into normal 32bit sign extended result
    result = BitFieldOperations::SignExtend(A >> B, (32-B));
    break;
  case EAluOp::OR:
    result = A | B;
    break;
  case EAluOp::AND:
    result = A & B;
    break;
  case EAluOp::XOR:
    result = A ^ B;
    break;
  case EAluOp::LEQ:
    result = A <= B;
    break;
  case EAluOp::LEQS:
    result = (int32_t)A <= (int32_t)B;
    break;
  case EAluOp::GEQ:
    result = A >= B;
    break;
  case EAluOp::GEQS:
    result = (int32_t)A >= (int32_t)B;
    break;
  case EAluOp::R9ADD:
    result = A + 8;
    break;

  default:
    throw IllegalInstruction("Unimplemented or unknown ALU operation");
  }

  return result;
}
