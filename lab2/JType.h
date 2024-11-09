#pragma once
#include "InstructionBase.h"

namespace JTypeInstruction
{
  /** @note: Instructions of Form: Opcode(6)-Imm(26)*/
  class JType : public InstructionBase
  {
  public:
  JType();
  };

  // Jump
  class J : public JType
  {
  public:
    J();
  };

  // Jump and link
  class Jal : public JType
  {
  public:
    Jal();
  };

  
  // Branch if flag is set
  class Bf : public JType
  {
  public:
    Bf();
  };

  // Branch if No Flag.
  class Bnf : public JType
  {
  public:
    Bnf();
  };

}

