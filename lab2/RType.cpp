#include "RType.h"
#include "alu.h"

RTypeInstruction::RInsType::RInsType(
  const RegNumber regD,
  const RegNumber regA,
  const RegNumber regB)
  : InstructionBase()
{
  RegD_ = regD;
  RegA_ = regA;
  RegB_ = regB;
  NeedA_ = true;
  NeedB_ = true;
  NeedD_ = true;
  ValueWriteBackToRD_ = Instruction::EWriteBackValueType::Wb_AluResult;
}

RTypeInstruction::Add::Add(
  const RegNumber regD,
  const RegNumber regA,
  const RegNumber regB)
    : RInsType(regD, regA, regB)
{
  AluInstruction_ = EAluOp::ADD;
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Register;
}

RTypeInstruction::Sub::Sub(
  const RegNumber regD,
  const RegNumber regA,
  const RegNumber regB)
    : RInsType(regD, regA, regB)
{
  AluInstruction_ = EAluOp::SUB;
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Register;
}

RTypeInstruction::Sll::Sll(const RegNumber regD, const RegNumber regA, const RegNumber regB)
  : RInsType(regD, regA, regB)
{
  AluInstruction_ = EAluOp::SHIFTL;
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Register;
}

RTypeInstruction::Sra::Sra(const RegNumber regD, const RegNumber regA, const RegNumber regB)
  : RInsType(regD, regA, regB)
{
  AluInstruction_ = EAluOp::SEXTSHIFTR;
  AluInputA_ = Instruction::EAluInput::AluIn_Register;
  AluInputB_ = Instruction::EAluInput::AluIn_Register;
}

RTypeInstruction::Or::Or(const RegNumber regD, const RegNumber regA, const RegNumber regB)
  : RInsType(regD, regA, regB)
{
  AluInstruction_ = EAluOp::OR;
  AluInputA_ = Instruction::AluIn_Register;
  AluInputB_ = Instruction::AluIn_Register;
}

RTypeInstruction::And::And(const RegNumber regD, const RegNumber regA, const RegNumber regB)
  : RInsType(regD, regA, regB)
{
  AluInstruction_ = EAluOp::AND;
  AluInputA_ = Instruction::AluIn_Register;
  AluInputB_ = Instruction::AluIn_Register;
}
