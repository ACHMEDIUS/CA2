#pragma once
#include "InstructionBase.h"

namespace FTypeInstruction
{
  class FType : public InstructionBase
  {
  public:
    FType(const RegNumber A,
          const RegNumber B);
  };

  // Set Flag if Less or Equal Than Signed.
  class Sfles : public FType
  {
  public:
    Sfles(const RegNumber A,
          const RegNumber B);
  };

  
  // Set Flag if Not Equal
  class Sfne : public FType
  {
  public:
    Sfne(const RegNumber A,
         const RegNumber B);
  };
  
  // Set Flag if Equal
  class Sfeq : public FType
  {
  public:
    Sfeq(const RegNumber A,
         const RegNumber B);
  };

  class Sfges : public FType
  {
  public:
    Sfges(const RegNumber A,
      const RegNumber B);
  };
}

