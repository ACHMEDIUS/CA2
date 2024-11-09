#include "FType.h"

FTypeInstruction::FType::FType(
  const RegNumber A,
  const RegNumber B)
    : InstructionBase()
{
  RegA_ = A;
  RegB_ = B;
  NeedA_ = true;
  NeedB_ = true;
}

FTypeInstruction::Sfles::Sfles(
  const RegNumber A,
  const RegNumber B)
    : FType(A, B)
{
  BranchType_ = Instruction::EBranchType::BT_SetsFlag;
  FlagComparision_ = Instruction::EFlagComparision::Flag_SignedLessOrEqual;
}

FTypeInstruction::Sfne::Sfne(
  const RegNumber A,
  const RegNumber B)
    : FType(A,B)
{
  BranchType_ = Instruction::EBranchType::BT_SetsFlag;
  FlagComparision_ = Instruction::EFlagComparision::Flag_NotEqual;
}

FTypeInstruction::Sfges::Sfges(const RegNumber A, const RegNumber B)
  : FType(A, B)
{
  BranchType_ = Instruction::EBranchType::BT_SetsFlag;
  FlagComparision_ = Instruction::EFlagComparision::Flag_SignedGreaterOrEqual;
}

FTypeInstruction::Sfeq::Sfeq(const RegNumber A, const RegNumber B)
  : FType(A, B)
{
  BranchType_ = Instruction::EBranchType::BT_SetsFlag;
  FlagComparision_ = Instruction::EFlagComparision::Flag_Equal;
}