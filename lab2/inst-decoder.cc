/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */

#include "inst-decoder.h"
#include "RType.h"
#include "ReadBitFunctions.h"

#include <map>

#include "DType.h"
#include "FType.h"
#include "IType.h"
#include "JRegType.h"
#include "JType.h"
#include "SType.h"

/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

InstructionDecoder::InstructionDecoder()
  : InstructionWord_(0),
    SizeInstructionParts_(),
    OffsetInstructionParts_()
{
}

void
InstructionDecoder::SetInstructionWord(const uint32_t instructionWord)
{
  this->InstructionWord_ = instructionWord;
}

uint32_t
InstructionDecoder::GetInstructionWord() const
{
  return InstructionWord_;
}


RegNumber
InstructionDecoder::GetA() const
{
  return BitFieldOperations::GetFieldValue<RegNumber>(InstructionWord_,
                                                      OffsetInstructionParts_.AReg,
                                                      SizeInstructionParts_.AReg);
}

RegNumber
InstructionDecoder::GetB() const
{
  return BitFieldOperations::GetFieldValue<RegNumber>(InstructionWord_,
                                                      OffsetInstructionParts_.BReg,
                                                      SizeInstructionParts_.BReg);
}

RegNumber
InstructionDecoder::GetD() const
{
  return BitFieldOperations::GetFieldValue<RegNumber>(InstructionWord_,
                                                      OffsetInstructionParts_.DReg,
                                                      SizeInstructionParts_.DReg);
}

InstructionBase
InstructionDecoder::GetInstruction()
{
  return InstructionFactory(GetOpcode());
}

RegValue
InstructionDecoder::GetImmediate(FImmediateProperties immediateProperties) const
{
  if (immediateProperties.NeedsSignExtension_)
  {
    return GetImmediateSignExtended(immediateProperties);
  }
  else
  {
    return GetImmediateZeroExtended(immediateProperties);
  }
}

uint32_t
InstructionDecoder::GetOpcode() const
{
  return BitFieldOperations::GetFieldValue<uint32_t>(InstructionWord_,
                                                     OffsetInstructionParts_.Opcode,
                                                     SizeInstructionParts_.Opcode);
}

uint32_t
InstructionDecoder::GetOpcode2() const
{
  return BitFieldOperations::GetFieldValue<uint32_t>(InstructionWord_,
                                                     OffsetInstructionParts_.Opcode2,
                                                     SizeInstructionParts_.Opcode2);
}

uint32_t
InstructionDecoder::GetOpcode3() const
{
  return BitFieldOperations::GetFieldValue<uint32_t>(InstructionWord_,
                                                     OffsetInstructionParts_.Opcode3,
                                                     SizeInstructionParts_.Opcode3);
}

uint32_t
InstructionDecoder::GetOpcodeD() const
{
  return BitFieldOperations::GetFieldValue<uint8_t>(InstructionWord_,
                                                    OffsetInstructionParts_.OpcodeD,
                                                    SizeInstructionParts_.OpcodeD);
}


uint32_t
InstructionDecoder::GetOpcodeF() const
{
  return BitFieldOperations::GetFieldValue<uint32_t>(InstructionWord_,
                                                     OffsetInstructionParts_.OpcodeF,
                                                     SizeInstructionParts_.OpcodeF);
}

InstructionBase
InstructionDecoder::InstructionFactory(
  const uint32_t opcode)
{
  InstructionBase instruction = InvalidInstruction();
  switch (opcode)
  {
  case 0x00:
  case 0x01:
  case 0x03:
  case 0x04:
    instruction = JTypeFactory(opcode);
    break;
  case 0x05:
    if (BitFieldOperations::GetFieldValue<uint32_t>(this->InstructionWord_, this->OffsetInstructionParts_.BigOpcode,
                                                    this->SizeInstructionParts_.BigOpcode) == 0x15)
    {
      instruction = Nop();
    }
    break;
  case 0x6:
    instruction = DTypeFactory(opcode, GetOpcodeD());
    break;
  case 0x11:
    instruction = JRegTypeFactory(opcode);
    break;
  //these are all shifts with immediate (and rotate with imm but we only implement srai)
  case 0x2e:
    if (BitFieldOperations::GetFieldValue<uint32_t>(this->InstructionWord_, this->OffsetInstructionParts_.Opcode4,
                                                    this->SizeInstructionParts_.Opcode4) != 0x2)
    {
      break; //this is not actually srai
    }
  case 0x1b:
  case 0x21:
  case 0x23:
  case 0x24:
  case 0x27:
  case 0x2a:
    instruction = ITypeFactory(opcode);
    break;
  case 0x35:
  case 0x36:
    instruction = STypeFactory(opcode);
    break;
  case 0x38:
    instruction = RTypeFactory(GetOpcode2(), GetOpcode3());
    break;
  default: //ftype has no matching opcode because a different format is used
    instruction = FTypeFactory(GetOpcodeF());
    break;
  } // switch
  if(!instruction.GetIsValidInstruction())
  {
    throw std::runtime_error("Unknown instruction / unimplemented instruction");
  }
  return instruction;
}

InstructionBase
InstructionDecoder::RTypeFactory(
  const uint32_t opcode2,
  const uint32_t opcode3) const
{
  if (opcode2 == 0x0)
  {
    switch (opcode3)
    {
    case 0x0:
      return RTypeInstruction::Add(GetD(), GetA(), GetB());
    case 0x2:
      return RTypeInstruction::Sub(GetD(), GetA(), GetB());
    case 0x3:
      return RTypeInstruction::And(GetD(), GetA(), GetB());
    case 0x4:
      return RTypeInstruction::Or(GetD(), GetA(), GetB());
    case 0x8:
      return RTypeInstruction::Sll(GetD(), GetA(), GetB());
    default:
      return InvalidInstruction();
    }
  }
  if (opcode2 == 0x2)
  {
    switch (opcode3)
    {
    case 0x8:
      return RTypeInstruction::Sra(GetD(), GetA(), GetB());
    }
  }
  return InvalidInstruction();
}

InstructionBase
InstructionDecoder::ITypeFactory(const uint32_t opcode) const
{
  switch (opcode)
  {
  case 0x1b: //load word atomic
    return ITypeInstruction::Lwa(GetD(), GetA());
  case 0x21:
    return ITypeInstruction::Lwz(GetD(), GetA());
  case  0x23:
    return ITypeInstruction::Lbz(GetD(), GetA());
  case 0x24: //load byte shift
    return ITypeInstruction::Lbs(GetD(), GetA());
  case 0x27:
    return ITypeInstruction::Addi(GetD(), GetA());
  case 0x2a:
    return ITypeInstruction::Ori(GetD(), GetA());
  case 0x2e: //could be any shift immediate but we only implement srai
    return ITypeInstruction::Srai(GetD(), GetA());
  default:
    return InvalidInstruction();
  }
}

InstructionBase
InstructionDecoder::STypeFactory(const uint32_t opcode) const
{
  switch (opcode)
  {
  case 0x35:
    return STypeInstruction::Sw(GetA(), GetB());
  case 0x36:
    return STypeInstruction::Sb(GetA(), GetB());
  }
  return InvalidInstruction();
}

InstructionBase
InstructionDecoder::DTypeFactory(
  const uint32_t opcode,
  const uint32_t dOpcode) const
{
  if (opcode == 0x6)
  {
    switch (dOpcode)
    {
    case 0x0:
      return DTypeInstruction::Movhi(GetD());
    }
  }
  return InvalidInstruction();
}

InstructionBase
InstructionDecoder::FTypeFactory(const uint32_t opcodeF) const
{
  switch (opcodeF)
  {
  case 0x72D:
    return FTypeInstruction::Sfles(GetA(), GetB());
  case 0x721:
    return FTypeInstruction::Sfne(GetA(), GetB());
  case 0x72b:
    return FTypeInstruction::Sfges(GetA(), GetB());
  case 0x720:
    return FTypeInstruction::Sfeq(GetA(), GetB());
  }
  return InvalidInstruction();
}


InstructionBase
InstructionDecoder::JRegTypeFactory(const uint32_t opcode) const
{
  switch (opcode)
  {
  case 0x11:
    return JRegInstruction::Jr(GetB());
  }
  return InvalidInstruction();
}


InstructionBase
InstructionDecoder::JTypeFactory(const uint32_t opcode) const
{
  switch (opcode)
  {
  case 0x00:
    return JTypeInstruction::J();
  case 0x01:
    return JTypeInstruction::Jal();
  case 0x03:
    return JTypeInstruction::Bnf();
  case 0x04:
    return JTypeInstruction::Bf();
  }
  return InvalidInstruction();
}

RegValue
InstructionDecoder::GetImmediateSignExtended(const FImmediateProperties immediateProperties) const
{
  uint16_t leastSignificant, mostSignificant;
  RegValue signExtended;
  if (immediateProperties.ImmediateSize_ <= 16)
  {
    if (immediateProperties.IsMultiFieldImmediate())
    {
      leastSignificant = BitFieldOperations::GetFieldValue<uint16_t>(
        InstructionWord_,
        immediateProperties.ImmediateOffset_,
        immediateProperties.ImmediateSize_);
      mostSignificant = BitFieldOperations::GetFieldValue<uint16_t>(
        InstructionWord_,
        immediateProperties.ImmediateOffset2_,
        immediateProperties.ImmediateSize2_);
      // Make room for least significant bits. Will not lose precision since total size is at most 16 bits
      mostSignificant = mostSignificant << immediateProperties.ImmediateSize_;
      mostSignificant += leastSignificant;
      signExtended = BitFieldOperations::SignExtend<uint16_t>(
        mostSignificant,
        immediateProperties.ImmediateSize_ + immediateProperties.ImmediateSize2_);
      return signExtended << immediateProperties.ShiftAmount_;
    } // isMultiField
    else
    {
      mostSignificant = BitFieldOperations::GetFieldValue<uint16_t>(
        InstructionWord_,
        immediateProperties.ImmediateOffset_,
        immediateProperties.ImmediateSize_);
      signExtended = BitFieldOperations::SignExtend<uint16_t>(
        mostSignificant,
        immediateProperties.ImmediateSize_);
      return signExtended << immediateProperties.ShiftAmount_;
    }
  }
  else
  {
    // We need 32 bits to read since this immediate has 16 < size <= 32. 
    signExtended = BitFieldOperations::GetFieldValue<uint32_t>(
      InstructionWord_,
      immediateProperties.ImmediateOffset_,
      immediateProperties.ImmediateSize_);
    signExtended = BitFieldOperations::SignExtend<uint32_t>(
      signExtended,
      immediateProperties.ImmediateSize_);
    return signExtended << immediateProperties.ShiftAmount_;
  }
}

RegValue
InstructionDecoder::GetImmediateZeroExtended(const FImmediateProperties immediateProperties) const
{
  uint16_t leastSignificant, mostSignificant;
  RegValue zeroExtended;
  if (immediateProperties.ImmediateSize_ <= 16)
  {
    if (immediateProperties.IsMultiFieldImmediate())
    {
      leastSignificant = BitFieldOperations::GetFieldValue<uint16_t>(
        InstructionWord_,
        immediateProperties.ImmediateOffset_,
        immediateProperties.ImmediateSize_);
      mostSignificant = BitFieldOperations::GetFieldValue<uint16_t>(
        InstructionWord_,
        immediateProperties.ImmediateOffset2_,
        immediateProperties.ImmediateSize2_);
      // Make room for least significant bits. Will not lose precision since total size is at most 16 bits
      mostSignificant = mostSignificant << immediateProperties.ImmediateSize_;
      mostSignificant += leastSignificant;
      zeroExtended = (uint32_t)mostSignificant;
      return zeroExtended << immediateProperties.ShiftAmount_;
    } // isMultiField
    else
    {
      mostSignificant = BitFieldOperations::GetFieldValue<uint16_t>(
        InstructionWord_,
        immediateProperties.ImmediateOffset_,
        immediateProperties.ImmediateSize_);
      zeroExtended = (uint32_t)mostSignificant;
      return zeroExtended << immediateProperties.ShiftAmount_;
    }
  }
  else
  {
    // We need 32 bits to read since this immediate has 16 < size <= 32. 
    zeroExtended = BitFieldOperations::GetFieldValue<uint32_t>(
      InstructionWord_,
      immediateProperties.ImmediateOffset_,
      immediateProperties.ImmediateSize_);
    return zeroExtended << immediateProperties.ShiftAmount_;
  }
}
