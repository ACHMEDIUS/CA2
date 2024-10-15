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

      /* TODO: implement necessary operations */

      default:
        throw IllegalInstruction("Unimplemented or unknown ALU operation");
    }


  return result;
}
