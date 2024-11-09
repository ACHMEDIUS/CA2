#include "SType.h"

#include "alu.h"

STypeInstruction::SType::SType(
  const RegNumber A,
  const RegNumber B)
{
  RegA_ = A;
  RegB_ = B;
  ImmediateProperties_.ImmediateOffset_ = 0;
  ImmediateProperties_.ImmediateSize_ = 11;
  ImmediateProperties_.ImmediateOffset2_ = 21;
  ImmediateProperties_.ImmediateSize2_ = 5;
  NeedA_ = true;
  NeedB_ = true;
}

STypeInstruction::Sw::Sw(
  const RegNumber A,
  const RegNumber B)
  : SType(A, B)
{
  AluInstruction_ = EAluOp::ADD;
  // Add the Immediate with RegA.
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Immediate;
  DataMemoryBehaviour_ = Instruction::EDataMemoryBehaviour::Write;
  DataMemorySize_ = DataMemoryProperty::EDataSizes::Word;
  // Imm + RegA gives effective address.
  DataMemoryAddressType_ = DataMemoryProperty::EMemoryAddressType::MemA_AluResult;
  // Store the value in RegB.
  DataMemStoreValueType_ = DataMemoryProperty::EMemoryValueType::MemV_RegB;
}

STypeInstruction::Sb::Sb(
  const RegNumber A,
  const RegNumber B)
  : SType(A, B)
{
  // Add the Immediate with RegA.
  AluInstruction_ = EAluOp::ADD;
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Immediate;
  DataMemoryBehaviour_ = Instruction::EDataMemoryBehaviour::Write;
  DataMemorySize_ = DataMemoryProperty::EDataSizes::Byte;
  // Imm + RegA gives effective address.
  DataMemoryAddressType_ = DataMemoryProperty::EMemoryAddressType::MemA_AluResult;
  // Store the value in RegB.
  DataMemStoreValueType_ = DataMemoryProperty::EMemoryValueType::MemV_RegB;
}
