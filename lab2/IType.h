#pragma once
#include "InstructionBase.h"


namespace ITypeInstruction
{
  /** @note: Instructions of Form: Opcode(6)-D(5)-A(5)-Imm(16)*/
  class IType : public InstructionBase
  {
  public:
    IType(
      const RegNumber regD,
      const RegNumber regA);
  };

  // Load Single Word and Extend with Zero.
  class Lwz : public IType
  {
  public:
    Lwz(
      const RegNumber regD,
      const RegNumber regA);
  };

  // Load Single Word atomic
  class Lwa : public Lwz
  {
  public:
    Lwa(
      const RegNumber regD,
      const RegNumber regA);
  };
  
  // Load Byte and Extend with Sign
  class Lbs : public Lwz
  {
  public:
    Lbs(
      const RegNumber regD,
      const RegNumber regA);
  };
  
  // Load Byte and Extend with Zero
      class Lbz : public Lwz
      {
      public:
        Lbz(
          const RegNumber regD,
          const RegNumber regA);
      };
  
  // Add Immediate.
  class Addi : public IType
  {
  public:
    Addi(
      const RegNumber regD,
      const RegNumber regA);
  };

  //Or with Immediate Half Word.
  class Ori : public IType
  {
  public:
    Ori(
      const RegNumber regD,
      const RegNumber regA);
  };

  //Shift right arithmetic with Immediate of 5 bits.
      class Srai : public IType
      {
      public:
        Srai(
          const RegNumber regD,
          const RegNumber regA);
      };
}
