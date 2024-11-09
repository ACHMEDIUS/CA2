#pragma once
#include "InstructionBase.h"

namespace STypeInstruction
{
  /** @note: Instructions of Form: Opcode(6)-Imm(5)-A(5)-B(5)-Imm(11) */
  class SType : public InstructionBase
  {
  public:
    SType(const RegNumber A,
          const RegNumber B);
  };

  // Store Single Word.
  class Sw : public SType
  {
  public:
    Sw(const RegNumber A,
       const RegNumber B);
  };

  // Store Byte.
  class Sb : public SType
  {
  public:
    Sb(const RegNumber A,
       const RegNumber B);
  };

}
