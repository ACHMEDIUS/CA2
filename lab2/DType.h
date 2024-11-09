#pragma once
#include "arch.h"
#include "InstructionBase.h"

namespace DTypeInstruction
{
  /** @note: Instructions of Form: Opcode(6)-D(5)-Res(4)-Opcode2(1)-Imm(16)*/
  class DType : public InstructionBase
  {
  public:
    DType(const RegNumber D);
  };

  // Move Immediate High.
  class Movhi : public DType
  {
  public:
    Movhi(const RegNumber D);
  };
}

