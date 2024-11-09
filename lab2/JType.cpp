#include "JType.h"

#include "alu.h"

JTypeInstruction::JType::JType()
  : InstructionBase()
{
  ImmediateProperties_.ImmediateSize_  = 26;
  ImmediateProperties_.ImmediateOffset_ = 0;
  // Shift immediate by 2.
  ImmediateProperties_.ShiftAmount_ = 2;
  ImmediateProperties_.NeedsSignExtension_ = true;
}

/**
 * @brief Jump  PC ← exts(Immediate << 2) + JumpInsnAddr
 */
JTypeInstruction::J::J() : JType()
{
  BranchType_ = Instruction::EBranchType::BT_Immediate;
}

/**
 * @brief Jump and link.
 */
JTypeInstruction::Jal::Jal() : JType()
{
  BranchType_ = Instruction::EBranchType::BT_Immediate;
  // Add 8 to our current address to obtain Delayinstr address + 4.
  AluInputA_ = Instruction::EAluInput::AluIn_PC;
  AluInstruction_ = EAluOp::R9ADD;
  ValueWriteBackToRD_ = Instruction::Wb_R9ADD;
}

/**
 * @brief Branch if SR[F] flag is set
 */
JTypeInstruction::Bf::Bf() : JType()
{
  BranchType_ = Instruction::EBranchType::BT_JumpToImmediateOnFlag;
}

/**
 * @brief Branch if SR[F] flag is not set
 */
JTypeInstruction::Bnf::Bnf()
{
  BranchType_ = Instruction::EBranchType::BT_JumpToImmediateNoFlag;
};
