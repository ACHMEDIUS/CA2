#include "DType.h"

#include "alu.h"

DTypeInstruction::DType::DType(const RegNumber D)
  : InstructionBase()
{
  NeedD_ = true;
  RegD_ = D;
  ImmediateProperties_.ImmediateSize_  = 16;
  ImmediateProperties_.ImmediateOffset_ = 0;
}

DTypeInstruction::Movhi::Movhi(const RegNumber D)
  :  DType(D)
{
  // Zero extend immediate.
  ImmediateProperties_.NeedsSignExtension_ = false;
  // Shift immediate by 16.
  ImmediateProperties_.ShiftAmount_ = 16;
  // Write back the shifted immediate value to RD.
  ValueWriteBackToRD_ = Instruction::EWriteBackValueType::Wb_Immediate;
  }
