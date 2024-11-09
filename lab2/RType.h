#pragma once
#include "InstructionBase.h"

namespace RTypeInstruction
{
  /** @note: Instructions of Form: Opcode(6)-D(5)-A(5)-B(5)-Res(1)-Opcode2(2)-Res2(4)-Opcode3(4)*/
  class RInsType : public InstructionBase
  {
  public:
    RInsType(const RegNumber regD, const RegNumber regA, const RegNumber regB);
  };

  // Add.
  class Add : public RInsType
  {
  public:
    Add(const RegNumber regD, const RegNumber regA, const RegNumber regB);
  };

  // Subtract.
  class Sub : public RInsType
  {
  public:
    Sub(const RegNumber regD, const RegNumber regA, const RegNumber regB);
  };

  // Shift Left Logical.
  class Sll : public RInsType
  {
  public:
    Sll(const RegNumber regD, const RegNumber regA, const RegNumber regB);
  };

  // Shift Right Arithmetic.
  class Sra : public RInsType
  {
  public:
    Sra(const RegNumber regD, const RegNumber regA, const RegNumber regB);
  };

  // Or.
  class Or : public RInsType
  {
  public:
    Or(const RegNumber regD, const RegNumber regA, const RegNumber regB);
  };

  // And.
  class And : public RInsType
  {
  public:
    And(const RegNumber regD, const RegNumber regA, const RegNumber regB);
  };
}
