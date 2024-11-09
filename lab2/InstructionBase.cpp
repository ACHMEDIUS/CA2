#include "InstructionBase.h"

#include <iostream>

#include "alu.h"

InstructionBase::InstructionBase()
  : AluInstruction_(EAluOp::NOP),
    RegD_(0),
    RegA_(0),
    RegB_(0),
    NeedD_(false),
    NeedA_(false),
    NeedB_(false),
    SExtDataMemoryOutput(false),
    AluInputA_(Instruction::EAluInput::AluIn_Nothing),
    AluInputB_(Instruction::EAluInput::AluIn_Nothing),
    ValueWriteBackToRD_(Instruction::EWriteBackValueType::Wb_Nothing),
    ImmediateProperties_(0, 0),
    DataMemoryBehaviour_(Instruction::EDataMemoryBehaviour::NoAction),
    DataMemorySize_(DataMemoryProperty::EDataSizes::Byte),
    DataMemoryAddressType_(DataMemoryProperty::EMemoryAddressType::MemA_Nothing),
    DataMemStoreValueType_(DataMemoryProperty::EMemoryValueType::MemV_Nothing),
    BranchType_(Instruction::EBranchType::BT_Nothing),
    FlagComparision_(Instruction::EFlagComparision::Flag_Nothing),
    IsInvalid_(false),
    IsNopInstruction_(false)
{
}

InstructionBase::~InstructionBase()
{
}

FImmediateProperties
InstructionBase::GetImmediateProperties() const
{
  return ImmediateProperties_;
}


EAluOp
InstructionBase::GetAluInstruction() const
{
  return AluInstruction_;
}

Instruction::EDataMemoryBehaviour
InstructionBase::GetDataMemoryBehaviour() const
{
  return DataMemoryBehaviour_;
}

bool
InstructionBase::NeedAlu() const
{
  return AluInstruction_ != EAluOp::NOP;
}

Nop::Nop()
  : InstructionBase()
{
  IsNopInstruction_ = true;
}

InvalidInstruction::InvalidInstruction()
  : InstructionBase()
{
  IsInvalid_ = true;
}
