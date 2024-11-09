#include "IType.h"

#include "alu.h"

ITypeInstruction::IType::IType(
  const RegNumber regD,
  const RegNumber regA)
  : InstructionBase()
{
  ImmediateProperties_.ImmediateOffset_ = 0;
  ImmediateProperties_.ImmediateSize_ = 16;
  RegA_ = regA;
  RegD_ = regD;
  NeedA_ = true;
  NeedD_ = true;
} 

ITypeInstruction::Lwz::Lwz(
  const RegNumber regD,
  const RegNumber regA)
  : IType(regD, regA)
{

  AluInstruction_ = EAluOp::ADD;
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Immediate;
  ValueWriteBackToRD_ = Instruction::EWriteBackValueType::Wb_DataMemory;
  DataMemoryBehaviour_ = Instruction::EDataMemoryBehaviour::Read;
  DataMemorySize_ = DataMemoryProperty::EDataSizes::Word;
  DataMemoryAddressType_ = DataMemoryProperty::EMemoryAddressType::MemA_AluResult;
}

// same as lwz, except: An atomic reservation is placed on the address formed from EA (not implemented)
ITypeInstruction::Lwa::Lwa(
  const RegNumber regD,
  const RegNumber regA)
  : Lwz(regD, regA)
{
}

// The offset is sign-extended and added to the contents of general-purpose register rA. The
// sum represents an effective address. The byte in memory addressed by EA is loaded into
// the low-order eight bits of general-purpose register rD.
// High-order bits of generalpurpose register rD are replaced with bit 7 of the loaded value. 
ITypeInstruction::Lbs::Lbs(
  const RegNumber regD,
  const RegNumber regA)
    : Lwz(regD, regA)
{
  DataMemorySize_ = DataMemoryProperty::EDataSizes::Byte;
  SExtDataMemoryOutput = true;
}

// The offset is sign-extended and added to the contents of general-purpose register rA. The
// sum represents an effective address. The byte in memory addressed by EA is loaded into
// the low-order eight bits of general-purpose register rD.
// High-order bits of general-purpose register rD are replaced with zero. 
ITypeInstruction::Lbz::Lbz(
  const RegNumber regD,
  const RegNumber regA)
    : Lwz(regD, regA)
{
  DataMemorySize_ = DataMemoryProperty::EDataSizes::Byte;
}

ITypeInstruction::Addi::Addi(
  const RegNumber regD,
  const RegNumber regA)
    : IType(regD, regA)
{
  AluInstruction_ = EAluOp::ADD;
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Immediate;
  ValueWriteBackToRD_ = Instruction::EWriteBackValueType::Wb_AluResult;
}

ITypeInstruction::Ori::Ori(
  const RegNumber regD,
  const RegNumber regA)
    : IType(regD, regA)
{
  // Zero extend immediate.
  ImmediateProperties_.NeedsSignExtension_ = false;
  AluInstruction_ = EAluOp::OR;
  // Add immediate to register A.
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Immediate;
  ValueWriteBackToRD_ = Instruction::EWriteBackValueType::Wb_AluResult;
}

ITypeInstruction::Srai::Srai(
  const RegNumber regD,
  const RegNumber regA)
    : IType(regD, regA)
{
  // Zero extend immediate.
  ImmediateProperties_.NeedsSignExtension_ = false;
  AluInstruction_ = EAluOp::SEXTSHIFTR;
  // Add immediate to register A.
  ImmediateProperties_.ImmediateSize_ = 6;
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Immediate;
  ValueWriteBackToRD_ = Instruction::EWriteBackValueType::Wb_AluResult;
}
