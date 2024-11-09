#include "ControlSignal.h"

#include "stages.h"

ControlSignal::ControlSignal()
  :
  Instruction_(Nop()),
  IsDelayInstruction_(false)
{
}

EAluOp
ControlSignal::GetAluInstruction() const
{
  return Instruction_.GetAluInstruction();
}

void
ControlSignal::SetInstruction(InstructionBase instruction)
{
  Instruction_ = instruction; 
}

RegNumber
ControlSignal::GetA() const
{
  return Instruction_.GetA();
}

RegNumber
ControlSignal::GetB() const
{
  return Instruction_.GetB();
}

RegNumber
ControlSignal::GetD() const
{
  return Instruction_.GetD();
}

void 
ControlSignal::SetIsStall()
{
  Instruction_ = Nop();
}

bool
ControlSignal::NeedA() const
{
  return Instruction_.NeedA();
}

bool
ControlSignal::NeedB() const
{
  return Instruction_.NeedB();
}

bool
ControlSignal::NeedD() const
{
  return Instruction_.NeedD();
}

bool
ControlSignal::NeedAlu() const
{
  return Instruction_.NeedAlu();
}

RegValue
ControlSignal::GetAluInputA(const ID_EXRegisters* pipelineRegs) const
{
  switch (Instruction_.GetAluInputA())
  {
  case Instruction::EAluInput::AluIn_Register:
    return pipelineRegs->RS1_;
  case Instruction::EAluInput::AluIn_Immediate:
    return pipelineRegs->Immediate_;
  case Instruction::EAluInput::AluIn_PC:
    return pipelineRegs->PC;
  case Instruction::EAluInput::AluIn_Nothing:
    return 0;
  }
  return 0;
}

RegValue
ControlSignal::GetAluInputB(const ID_EXRegisters* pipelineRegs) const
{
  switch (Instruction_.GetAluInputB())
  {
  case Instruction::EAluInput::AluIn_Register:
    return pipelineRegs->RS2_;
  case Instruction::EAluInput::AluIn_Immediate:
    return pipelineRegs->Immediate_;
  case Instruction::EAluInput::AluIn_PC:
    return pipelineRegs->PC;
  case Instruction::EAluInput::AluIn_Nothing:
    return 0;
  }
  return 0;
}

bool
ControlSignal::NeedWriteBackToRd() const
{
  return Instruction_.NeedWriteBackToRd();
}

bool
ControlSignal::NeedImmediate() const
{
  return Instruction_.NeedImmediate();
}

FImmediateProperties
ControlSignal::GetImmediateProperties() const
{
  return Instruction_.GetImmediateProperties();
}

bool
ControlSignal::NeedReadDataMemory() const
{
  return Instruction_.GetDataMemoryBehaviour() == Instruction::EDataMemoryBehaviour::Read;
}

bool
ControlSignal::NeedWriteDataMemory() const
{
  return Instruction_.GetDataMemoryBehaviour() == Instruction::EDataMemoryBehaviour::Write;
}

bool
ControlSignal::NeedSExtDataMemoryOutput() const
{
  return Instruction_.NeedSExtDataMemoryOutput();
}


DataMemoryProperty::EDataSizes
ControlSignal::GetDataMemorySize() const
{
  return Instruction_.GetDataMemorySize();
}


RegValue
ControlSignal::GetWriteBackValueRD(const M_WBRegisters* pipelineRegs) const
{
  switch (Instruction_.GetValueTypeWriteBackRd())
  {
  case Instruction::EWriteBackValueType::Wb_AluResult:
  case Instruction::EWriteBackValueType::Wb_R9ADD:
    return pipelineRegs->ALUOutput_;
  case Instruction::EWriteBackValueType::Wb_DataMemory:
    return pipelineRegs->DataMemoryOutput_;
  case Instruction::EWriteBackValueType::Wb_Immediate:
    return pipelineRegs->Immediate_;
  case Instruction::EWriteBackValueType::Wb_Nothing:
    throw std::runtime_error("No type to write back to RD");
  }
  return 0;
}

Instruction::EWriteBackValueType
ControlSignal::GetWriteBackType() const
{
  return Instruction_.GetValueTypeWriteBackRd();
}

MemAddress
ControlSignal::GetMemAddress(const EX_MRegisters* pipelineRegs) const
{
  switch (Instruction_.GetMemoryAddressType())
  {
  case DataMemoryProperty::EMemoryAddressType::MemA_Pc:
    return pipelineRegs->PC;
  case DataMemoryProperty::EMemoryAddressType::MemA_AluResult:
    return pipelineRegs->ALUOutput_;
  case DataMemoryProperty::EMemoryAddressType::MemA_Nothing:
  default:
    throw std::runtime_error("No store to memory address type");
  }
}

RegValue
ControlSignal::GetMemWriteValue(const EX_MRegisters* pipelineRegs) const
{
  switch (Instruction_.GetMemoryValueType())
  {
  case DataMemoryProperty::EMemoryValueType::MemV_AluResult:
    return pipelineRegs->ALUOutput_;
  case DataMemoryProperty::EMemoryValueType::MemV_Nothing:
    throw std::runtime_error("No store to memory value type");
  case DataMemoryProperty::EMemoryValueType::MemV_RegB:
    return pipelineRegs->RS2_;
  case DataMemoryProperty::EMemoryValueType::MemV_PC:
    return pipelineRegs->PC;
  default:
    throw std::runtime_error("No store to memory value type");
  }
}

Instruction::EBranchType
ControlSignal::GetBranchType() const
{
  return Instruction_.GetBranchType();
}

bool ControlSignal::GetIsBranch() const
{
  return Instruction_.GetBranchType() != Instruction::EBranchType::BT_Nothing &&
    Instruction_.GetBranchType() != Instruction::EBranchType::BT_SetsFlag;
}

void
ControlSignal::SetIsDelayInstruction(const bool isDelayInstruction)
{
  IsDelayInstruction_ = isDelayInstruction;
}

bool
ControlSignal::GetIsDelayInstruction() const
{
  return IsDelayInstruction_;
}

void
ControlSignal::SetBranchAdderValues(
  Adder* adder,
  MemAddress Pc,
  uint32_t immediate) const
{
  switch (Instruction_.GetBranchType())
  {
  case Instruction::EBranchType::BT_Immediate:
  case Instruction::EBranchType::BT_JumpToImmediateOnFlag:
  case Instruction::EBranchType::BT_JumpToImmediateNoFlag:
    adder->SetInputA(Pc);
    adder->SetInputB(immediate);
    break;
  case Instruction::BT_Register: 
  case Instruction::BT_SetsFlag: 
  case Instruction::BT_Nothing: 
  default:
    break;
  }
}

bool
ControlSignal::GetFlagValue(
  RegValue RS1,
  RegValue RS2) const
{
  switch (Instruction_.GetFlagComparision())
  {
  case Instruction::EFlagComparision::Flag_SignedLessOrEqual:
    return (int32_t)RS1 <= (int32_t)RS2;
  case Instruction::EFlagComparision::Flag_SignedGreaterOrEqual:
    return (int32_t)RS1 >= (int32_t)RS2;
  case Instruction::EFlagComparision::Flag_NotEqual:
    return RS1 != RS2;
  case Instruction::EFlagComparision::Flag_Equal:
    return RS1 == RS2;
  case Instruction::EFlagComparision::Flag_Nothing:
    default:
      throw std::runtime_error("No flag comparision type");
  } 
}

bool
ControlSignal::GetIsNop() const
{
  return Instruction_.GetIsNopInstruction();
}

bool
ControlSignal::GetUsesDelaySlot() const
{
  return Instruction_.GetBranchType() != Instruction::EBranchType::BT_SetsFlag ||
    Instruction_.GetBranchType() != Instruction::EBranchType::BT_Nothing;
}

