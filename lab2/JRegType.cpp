#include "JRegType.h"

JRegInstruction::JRegType::JRegType(const RegNumber B)
{
  RegB_ = B;
  NeedB_ = true;
}

JRegInstruction::Jr::Jr(const RegNumber B)
  : JRegType(B)
{
  BranchType_ = Instruction::EBranchType::BT_Register;
}
