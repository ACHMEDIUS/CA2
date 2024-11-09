#pragma once
#include "InstructionBase.h"

namespace JRegInstruction
{
  /** @note: Instructions of Form: Opcode(6)-Res(10)-B(5)-Res2(11)*/
  class JRegType : public InstructionBase
  {
  public:
    JRegType(const RegNumber B);
  };

  // Jump Register.
  class Jr : public JRegType
  {
  public:
    Jr(const RegNumber B);
  };
}


